#include "audio_provider.h"
#include "driver/i2s.h"
#include "driver/gpio.h"
#include "micro_model_settings.h"

namespace {
bool g_is_audio_initialized = false;
constexpr int kAudioCaptureBufferSize = 16000; // 1 giây @ 16kHz
int16_t g_audio_capture_buffer[kAudioCaptureBufferSize];
int16_t g_audio_output_buffer[kMaxAudioSampleSize];
volatile int32_t g_latest_audio_timestamp = 0;
}  // namespace

#define I2S_WS   4
#define I2S_SD   15
#define I2S_SCK  2
#define I2S_LR   16   // Chân L/R để chọn kênh

static void InitI2SMic() {
  // Cấu hình chân L/R ở mức cao
  gpio_config_t io_conf = {};
  io_conf.intr_type = GPIO_INTR_DISABLE;
  io_conf.mode = GPIO_MODE_OUTPUT;
  io_conf.pin_bit_mask = (1ULL << I2S_LR);
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
  gpio_config(&io_conf);
  gpio_set_level((gpio_num_t)I2S_LR, 1); // Set mức cao

  // Cấu hình I2S
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = kAudioSampleFrequency,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_zero_dma_buffer(I2S_NUM_0);
}

TfLiteStatus InitAudioRecording(tflite::ErrorReporter* error_reporter) {
  InitI2SMic();
  g_latest_audio_timestamp = 0;
  g_is_audio_initialized = true;
  return kTfLiteOk;
}

TfLiteStatus GetAudioSamples(tflite::ErrorReporter* error_reporter,
                             int start_ms, int duration_ms,
                             int* audio_samples_size, int16_t** audio_samples) {
  if (!g_is_audio_initialized) {
    TfLiteStatus init_status = InitAudioRecording(error_reporter);
    if (init_status != kTfLiteOk) {
      return init_status;
    }
  }

  size_t bytes_read = 0;
  const int samples_to_read = duration_ms * (kAudioSampleFrequency / 1000);
  int32_t temp32;
  for (int i = 0; i < samples_to_read; i++) {
    i2s_read(I2S_NUM_0, &temp32, sizeof(temp32), &bytes_read, portMAX_DELAY);
    g_audio_output_buffer[i] = (int16_t)(temp32 >> 14); // Giảm từ 32-bit về 16-bit
  }

  *audio_samples_size = kMaxAudioSampleSize;
  *audio_samples = g_audio_output_buffer;
  g_latest_audio_timestamp += duration_ms;

  return kTfLiteOk;
}

int32_t LatestAudioTimestamp() {
  return g_latest_audio_timestamp;
}

