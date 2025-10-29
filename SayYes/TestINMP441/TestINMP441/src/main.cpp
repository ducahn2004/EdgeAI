#include <Arduino.h>
#include <driver/i2s.h>

// ================== Pin mapping ==================
#define I2S_WS      25   // LRCL (Word Select)
#define I2S_SD      33   // DOUT từ mic
#define I2S_SCK     26   // BCLK (Serial Clock) or 32?
#define I2S_PORT    I2S_NUM_1   // dùng bộ I2S số 1
#define SEL_PIN     32  // chân chọn kênh (LEFT/RIGHT)


#define LED_RED_PIN    13
#define LED_GREEN_PIN  12
#define LED_BLUE_PIN   14

#define LEDC_CHANNEL_RED    0
#define LEDC_CHANNEL_GREEN  1
#define LEDC_CHANNEL_BLUE   2
#define LEDC_TIMER_BITS     8
#define LEDC_BASE_FREQ      5000


void setup_led() {
    ledcSetup(LEDC_CHANNEL_RED, LEDC_BASE_FREQ, LEDC_TIMER_BITS);
    ledcAttachPin(LED_RED_PIN, LEDC_CHANNEL_RED);

    ledcSetup(LEDC_CHANNEL_GREEN, LEDC_BASE_FREQ, LEDC_TIMER_BITS);
    ledcAttachPin(LED_GREEN_PIN, LEDC_CHANNEL_GREEN);

    ledcSetup(LEDC_CHANNEL_BLUE, LEDC_BASE_FREQ, LEDC_TIMER_BITS);
    ledcAttachPin(LED_BLUE_PIN, LEDC_CHANNEL_BLUE);
}

void set_rgb_color(uint8_t r, uint8_t g, uint8_t b) {
    ledcWrite(LEDC_CHANNEL_RED, r);
    ledcWrite(LEDC_CHANNEL_GREEN, g);
    ledcWrite(LEDC_CHANNEL_BLUE, b);
}

// ================== Cấu hình I2S ==================
void i2s_install(bool left_channel) {
    const i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX), // ESP32 master, nhận dữ liệu
        .sample_rate = 16000,                                // tần số lấy mẫu 16kHz
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,        // mỗi mẫu 32 bit
        .channel_format = left_channel ? I2S_CHANNEL_FMT_ONLY_LEFT : I2S_CHANNEL_FMT_ONLY_RIGHT, // đọc kênh trái (L/R = GND)
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false, 
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0
    };

    const i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCK,   // BCLK = GPIO26
        .ws_io_num = I2S_WS,     // LRCLK = GPIO25
        .data_out_num = I2S_PIN_NO_CHANGE, 
        .data_in_num = I2S_SD    // DOUT = GPIO33
    };

    i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_PORT, &pin_config);
}

// ================== Đọc mẫu từ I2S ==================
int32_t i2s_read_sample() {
    int32_t sample = 0;
    size_t bytes_read;
    i2s_read(I2S_PORT, &sample, sizeof(sample), &bytes_read, portMAX_DELAY);
    return sample >> 14;  // scale giá trị nhỏ lại
}

// ================== Setup ==================
void setup() {
    Serial.begin(115200);
    pinMode(SEL_PIN, INPUT_PULLUP); // Thiết lập chân SEL là input, dùng nút nhấn hoặc jumper
    setup_led();
    // Khởi tạo I2S với kênh mặc định (trái)
    i2s_install(true);
}

// ================== Loop ==================
void loop() {
    // Đọc trạng thái SEL để chọn kênh
    bool left_channel = digitalRead(SEL_PIN) == HIGH;
    static bool last_channel = !left_channel;

    // Nếu chuyển kênh thì cài đặt lại I2S
    if (left_channel != last_channel) {
        i2s_driver_uninstall(I2S_PORT);
        i2s_install(left_channel);
        last_channel = left_channel;
        Serial.println(left_channel ? "Kênh trái" : "Kênh phải");
    }

    int32_t sample = i2s_read_sample();
    Serial.println(sample);   // mở Serial Plotter để xem sóng âm

    // Chuyển giá trị sample về dải 0-20000
    int32_t abs_sample = abs(sample);
    if (abs_sample > 3000) abs_sample = 3000;

    // Tính giá trị màu: từ xanh dương (0,0,255) sang đỏ (255,0,0)
    uint8_t red = map(abs_sample, 0, 3000, 0, 255);
    uint8_t blue = map(abs_sample, 0, 3000, 255, 0);

    set_rgb_color(red, 0, blue);

    delay(10); // giảm tốc độ cập nhật LED
}
