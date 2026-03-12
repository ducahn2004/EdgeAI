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

extern I2S_HandleTypeDef hi2s1;
extern TIM_HandleTypeDef htim2;

int16_t           audio_bufferA[AUDIO_BUFFER_SIZE];
volatile int16_t* current_buffer = audio_bufferA;
volatile uint8_t  audio_ready    = 0;

int16_t           ring_buffer[RING_BUFFER_SIZE] = {0};
volatile uint32_t rb_write = 0;
volatile uint32_t rb_read  = 0;

void StartAudioCapture(void)
{
    HAL_I2S_Receive_DMA(&hi2s1, (uint16_t*)audio_bufferA, AUDIO_BUFFER_SIZE);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);   // Đỏ  — cường độ âm
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);   // Xanh lá — MFCC
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);   // Xanh dương — AI
}

/*
 * Downsample 48kHz → 2kHz rồi push vào ring buffer.
 * Lấy 1 sample mỗi DOWNSAMPLE_RATIO=24 samples (decimation).
 * Mỗi lần gọi: len = AUDIO_HALF_RAW = 720 raw samples
 *              → push 720/24 = 30 samples @2kHz vào ring buffer.
 */
static void audio_push_to_ring(int16_t *data, uint32_t len)
{
    for (uint32_t i = 0; i < len; i += DOWNSAMPLE_RATIO)
    {
        ring_buffer[rb_write] = data[i];
        rb_write = (rb_write + 1) % RING_BUFFER_SIZE;
    }
}

/*
 * Hiển thị cường độ âm qua LED đỏ (PWM TIM2 CH1).
 * Tính RMS từ raw buffer 48kHz trước khi downsample.
 */
void DisplayAudioIntensity(int16_t* buffer, uint32_t size)
{
    float32_t rms = 0.0f;
    for (uint32_t i = 0; i < size; i++)
    {
        float32_t sample = (float32_t)buffer[i] / 32768.0f;
        rms += sample * sample;
    }
    rms = sqrtf(rms / (float32_t)size);

    uint32_t duty = (uint32_t)(rms * 1000.0f);
    if (duty > 1000) duty = 1000;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, duty);
}

/* Half-callback: nửa đầu buffer đã đầy */
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    audio_push_to_ring(audio_bufferA, AUDIO_BUFFER_SIZE / 2);
    DisplayAudioIntensity(audio_bufferA, AUDIO_BUFFER_SIZE / 2);
}

/* Full-callback: nửa sau buffer đã đầy */
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    audio_push_to_ring(&audio_bufferA[AUDIO_BUFFER_SIZE / 2], AUDIO_BUFFER_SIZE / 2);
    DisplayAudioIntensity(&audio_bufferA[AUDIO_BUFFER_SIZE / 2], AUDIO_BUFFER_SIZE / 2);
}