/*
 * audio_capture.c
 *
 *  Created on: Feb 8, 2026
 *      Author: edoph
 */

#include "stm32h7xx_hal.h"
#include "audio_sd.h"
#include "audio_capture.h"
#include "app_x-cube-ai.h"
#include "mfcce_xtract.h"
#include "audio_sd.h"



extern I2S_HandleTypeDef hi2s1;
extern TIM_HandleTypeDef htim2;

int16_t audio_bufferA[AUDIO_BUFFER_SIZE];
int16_t audio_bufferB[AUDIO_BUFFER_SIZE];
volatile int16_t* current_buffer = audio_bufferA;
volatile uint8_t audio_ready = 0;

void StartAudioCapture(void) {
	HAL_I2S_Receive_DMA(&hi2s1, (uint16_t*)audio_bufferA, AUDIO_BUFFER_SIZE);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);  // Đỏ
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);  // Xanh lá
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);  // Xanh
}
void DisplayAudioIntensity(int16_t* buffer, uint32_t size) {
    float32_t rms = 0.0f;
    for (uint32_t i = 0; i < size; i++) {
        float32_t sample = (float32_t)buffer[i] / 32768.0f;  // Normalize -1 to 1
        rms += sample * sample;
    }
    rms = sqrtf(rms / size);  // RMS
    uint32_t duty = (uint32_t)(rms * 1000.0f);  // Map 0-1 to 0-1000 (ARR=1000 giả sử)
    if (duty > 1000) duty = 1000;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, duty);  // Set PWM đỏ
}

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    // Switch buffer và restart DMA NGAY LẬP TỨC để không mất dữ liệu
    current_buffer = (current_buffer == audio_bufferA) ? audio_bufferB : audio_bufferA;
    HAL_I2S_Receive_DMA(&hi2s1, (uint16_t*)current_buffer, AUDIO_BUFFER_SIZE);

    audio_ready = 1;   // Báo cho main loop xử lý buffer cũ
}

/**
 * @brief  Xử lý audio khi buffer DMA đã đầy (gọi trong main loop hoặc timer)
 * @note   Sử dụng double buffering (ping-pong) để tránh mất dữ liệu
 */
void ProcessAudioIfReady(void)
{
    if (audio_ready == 0) {
        return;
    }

    // Đánh dấu đã xử lý xong flag
    audio_ready = 0;

    // Xác định buffer nào vừa được thu xong (buffer trước đó)
    // current_buffer là buffer đang được DMA ghi vào lúc này
    int16_t* process_buffer = (current_buffer == audio_bufferA)
                            ? audio_bufferB
                            : audio_bufferA;

    // ---------------------------------------------------------------
    // Stage 1: Hiển thị mức cường độ âm thanh bằng LED ĐỎ (PA0 - TIM2_CH1)
    // ---------------------------------------------------------------
    DisplayAudioIntensity(process_buffer, AUDIO_BUFFER_SIZE);

    // ---------------------------------------------------------------
    // Stage 2: Tính MFCC + Delta + Delta-Delta → bật LED XANH LÁ (PA2 - TIM2_CH3)
    // ---------------------------------------------------------------
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 1000);  // Bật LED xanh lá (giả sử ARR=999 → 100% duty)

    // Chạy preprocessing MFCC trên buffer vừa thu được
    AudioPreprocessing_Run(process_buffer, mfcc_features, AUDIO_BUFFER_SIZE);

    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);     // Tắt LED xanh lá

    // ---------------------------------------------------------------
    // Chuẩn bị dữ liệu cho AI (copy MFCC features)
    // ---------------------------------------------------------------
    extern float32_t mfcc_final_features[39 * 333];
    memcpy(mfcc_final_features, mfcc_features, sizeof(mfcc_features));

    // ---------------------------------------------------------------
    // Stage 3: Chạy AI inference → bật LED XANH DƯƠNG (PA3 - TIM2_CH4)
    // ---------------------------------------------------------------
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 1000);  // Bật LED xanh dương

    // Chạy inference và post-process (bật LED abnormal nếu phát hiện bất thường)
    MX_X_CUBE_AI_Process();

    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);     // Tắt LED xanh dương

    // (Tùy chọn) Reset hoặc làm gì đó với buffer nếu cần
    // Ví dụ: arm_fill_f32(0.0f, process_buffer, AUDIO_BUFFER_SIZE); // nếu muốn xóa
}
