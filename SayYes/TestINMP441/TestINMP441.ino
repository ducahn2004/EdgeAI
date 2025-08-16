#include "driver/i2s.h"

#define I2S_WS      4   // LRCK
#define I2S_SD      15  // DOUT
#define I2S_SCK     2   // BCLK
#define LR_PIN      16  // L/R select pin

#define SAMPLE_RATE 16000

void i2s_install() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,  // đọc kênh phải
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 1024,
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
}

void setup() {
  Serial.begin(115200);

  // Chọn kênh phải
  pinMode(LR_PIN, OUTPUT);
  digitalWrite(LR_PIN, HIGH);

  i2s_install();
}

void loop() {
  int32_t buffer[1024];
  size_t bytes_read;

  i2s_read(I2S_NUM_0, (char *)buffer, sizeof(buffer), &bytes_read, portMAX_DELAY);
  int samples_read = bytes_read / sizeof(int32_t);

  for (int i = 0; i < samples_read; i++) {
    int16_t sample = (buffer[i] >> 14); // scale về 16-bit
    Serial.println(sample);
  }
}
