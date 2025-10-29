#include <Arduino.h>
#include "audio_provider.h"
#include "driver/i2s.h"
#include "driver/gpio.h"
#include "micro_model_settings.h"

namespace
{
  bool g_is_audio_initialized = false;
  constexpr int kAudioCaptureBufferSize = 16000; // 1 giây @ 16kHz
  // int16_t g_audio_capture_buffer[kAudioCaptureBufferSize];
  int16_t g_audio_output_buffer[kMaxAudioSampleSize];
  volatile int32_t g_latest_audio_timestamp = 0;
} // namespace

#define I2S_SAMPLE_SHIFT 14



static void InitI2SMic() {
  Serial.println("[I2S] Bắt đầu khởi tạo mic...");
  SemaphoreHandle_t i2s_mutex = nullptr;

  // Thử gỡ driver (không kiểm lỗi => nếu chưa cài sẽ trả lỗi nhưng không dừng)
  esp_err_t r = i2s_driver_uninstall(I2S_PORT);
  if (r == ESP_ERR_INVALID_STATE) {
    Serial.printf("[I2S] Driver uninstall: port %d not installed (ok)\n", (int)I2S_PORT);
  } else {
    Serial.printf("[I2S] Driver uninstall result: %d\n", (int)r);
  }

  // Cấu hình I2S (cấu hình an toàn, buffer lớn hơn)
  i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 44100,
    .bits_per_sample = i2s_bits_per_sample_t(16),
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };

  r = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  Serial.printf("[I2S] Driver install result: %d\n", (int)r);
  if (r != ESP_OK) {
    Serial.println("[I2S] Driver install FAILED");
    return;
  } else {
    Serial.println("[I2S] Driver install OK");
  }

  r = i2s_set_pin(I2S_PORT, &pin_config);
  Serial.printf("[I2S] Pin config result: %d\n", (int)r);
  if (r != ESP_OK) {
    Serial.println("[I2S] Pin config FAILED");
  } else {
    Serial.println("[I2S] Pin config OK");
  }
  r = i2s_start(I2S_PORT);
  Serial.printf("[I2S] i2s_start result: %d\n", (int)r);
  vTaskDelay(pdMS_TO_TICKS(100));

  // Khởi động I2S (bắt buộc trước khi đọc)
  r = i2s_zero_dma_buffer(I2S_PORT);
  Serial.printf("[I2S] Zero DMA result: %d\n", (int)r);
  r = i2s_start(I2S_PORT);
  Serial.printf("[I2S] i2s_start result: %d\n", (int)r);

  // Zero local buffer
  memset(g_audio_output_buffer, 0, sizeof(g_audio_output_buffer));
  Serial.println("[I2S] Mic ready!");
}

TfLiteStatus InitAudioRecording(tflite::ErrorReporter *error_reporter)
{
  InitI2SMic();
  g_latest_audio_timestamp = 0;
  g_is_audio_initialized = true;
  Serial.println("[I2S] InitAudioRecording hoàn tất.");
  return kTfLiteOk;
}

TfLiteStatus GetAudioSamples(tflite::ErrorReporter *error_reporter,
                             int start_ms, int duration_ms,
                             int *audio_samples_size, int16_t **audio_samples)
{
  if (!g_is_audio_initialized)
  {
    TfLiteStatus init_status = InitAudioRecording(error_reporter);
    if (init_status != kTfLiteOk)
    {
      return init_status;
    }
  }

  int samples_needed = duration_ms * (kAudioSampleFrequency / 1000);
  if (samples_needed < 512)
    samples_needed = 512;
  if (samples_needed > kMaxAudioSampleSize)
    samples_needed = kMaxAudioSampleSize;

  int filled = 0;
  const int32_t block_read_samples = 64; // đọc từng block 128 mẫu (each sample is 32-bit)
  int32_t read_buf[block_read_samples];
  size_t bytes_read = 0;

  const uint32_t start_tick = millis();
  const uint32_t timeout_ms = 300;

  i2s_zero_dma_buffer(I2S_PORT);
  vTaskDelay(pdMS_TO_TICKS(10));

  while (filled < samples_needed)
  {
    if (millis() - start_tick > timeout_ms)
    {
      Serial.printf("GetAudioSamples: timeout after %u ms, filled=%d/%d\n", timeout_ms, filled, samples_needed);
      break;
    }
    Serial.printf("[I2S] i2s_read: port=%d need=%d filled=%d\n", (int)I2S_PORT, samples_needed, filled);
    // đọc block (sizeof = block_read_samples * sizeof(int32_t))
    esp_err_t res = i2s_read(I2S_PORT, (void *)read_buf, sizeof(read_buf), &bytes_read, pdMS_TO_TICKS(10));
    if (res != ESP_OK)
    {
      // không cần panic; chờ 1 tick rồi thử lại
      // nếu lỗi liên tục thì log để debug
      Serial.printf("[I2S] read error: %d\n", (int)res);
      delay(1);
      continue;
    }
    if (bytes_read == 0) {
      vTaskDelay(pdMS_TO_TICKS(1));
      continue;
    }
    int samples = bytes_read / sizeof(int32_t);
    for (int i = 0; i < samples && filled < samples_needed; ++i)
    {
      int32_t v = read_buf[i] >> I2S_SAMPLE_SHIFT;
      if (v > INT16_MAX)
        v = INT16_MAX;
      if (v < INT16_MIN)
        v = INT16_MIN;
      g_audio_output_buffer[filled++] = (int16_t)v;
    }
  }

  // Nếu không đủ, zero phần còn lại
  if (filled < samples_needed)
  {
    Serial.printf("GetAudioSamples: filled only %d of %d -> zeroing remainder\n", filled, samples_needed);
    for (int i = filled; i < samples_needed; ++i)
      g_audio_output_buffer[i] = 0;
    filled = samples_needed;
  }

  *audio_samples_size = filled;
  *audio_samples = g_audio_output_buffer;
  // cập nhật timestamp theo số mẫu thực tế
  g_latest_audio_timestamp += (filled * 1000) / kAudioSampleFrequency;

  return kTfLiteOk;
}

int32_t LatestAudioTimestamp()
{
  return g_latest_audio_timestamp;
}
