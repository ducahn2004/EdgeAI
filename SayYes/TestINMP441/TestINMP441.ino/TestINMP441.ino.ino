#include <Arduino.h>
#include "driver/i2s.h"

#define I2S_WS    25   // LRCLK (Word Select)
#define I2S_SD    34   // DOUT từ mic -> ESP32
#define I2S_SCK   26   // BCLK (Bit Clock)
#define I2S_PORT  I2S_NUM_0

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("[I2S] Bắt đầu khởi tạo mic...");

  // Gỡ driver nếu có
  i2s_driver_uninstall(I2S_PORT);

  // Cấu hình I2S
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
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

  if (i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL) == ESP_OK) {
    Serial.println("[I2S] Driver install OK");
  } else {
    Serial.println("[I2S] Driver install FAILED");
  }

  if (i2s_set_pin(I2S_PORT, &pin_config) == ESP_OK) {
    Serial.println("[I2S] Pin config OK");
  } else {
    Serial.println("[I2S] Pin config FAILED");
  }

  i2s_zero_dma_buffer(I2S_PORT);
  Serial.println("[I2S] Mic ready!");
}

void loop() {
  int32_t sample = 0;
  size_t bytes_read;

  // Đọc 1 mẫu 32-bit từ I2S
  i2s_read(I2S_PORT, &sample, sizeof(sample), &bytes_read, portMAX_DELAY);

  if (bytes_read > 0) {
    // Dữ liệu INMP441 là 24-bit left aligned trong 32-bit
    int16_t s16 = (int16_t)(sample >> 14); // scale xuống 16-bit

    //Serial.println(s16);
  }

  delay(10); // để log không quá nhanh
}
