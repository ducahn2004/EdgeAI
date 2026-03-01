/*
#include <mfcc_extract.h>
 * audio_capture.c
 *
 *  Created on: Feb 8, 2026
 *      Author: edoph
 */

#include "stm32h7xx_hal.h"
#include "audio_sd.h"
#include "audio_capture.h"
#include "app_x-cube-ai.h"
#include "audio_sd.h"

extern I2S_HandleTypeDef hi2s1;
extern TIM_HandleTypeDef htim2;

int16_t audio_bufferA[AUDIO_BUFFER_SIZE];
int16_t audio_bufferB[AUDIO_BUFFER_SIZE];
volatile int16_t* current_buffer = audio_bufferA;
volatile uint8_t audio_ready = 0;

int16_t ring_buffer[RING_BUFFER_SIZE] = {0};  // khởi tạo = 0 để an toàn
volatile uint32_t rb_write = 0;
volatile uint32_t rb_read = 0;

void StartAudioCapture(void) {
	HAL_I2S_Receive_DMA(&hi2s1, (uint16_t*)audio_bufferA, AUDIO_BUFFER_SIZE);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);  // Đỏ
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);  // Xanh lá
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);  // Xanh
}


static void audio_push_to_ring(int16_t *data, uint32_t len)
{
    for(uint32_t i = 0; i < len; i++)
    {
        ring_buffer[rb_write++] = data[i];
        if(rb_write >= RING_BUFFER_SIZE) rb_write = 0;
    }
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

void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    // Switch buffer và restart DMA NGAY LẬP TỨC để không mất dữ liệu
	audio_push_to_ring(audio_bufferA, AUDIO_BUFFER_SIZE / 2);
}


void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    // Switch buffer và restart DMA NGAY LẬP TỨC để không mất dữ liệu
	audio_push_to_ring(&audio_bufferA[AUDIO_BUFFER_SIZE / 2], AUDIO_BUFFER_SIZE / 2);
}

