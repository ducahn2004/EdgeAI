/*
 * audio_capture.h
 *
 *  Created on: Feb 8, 2026
 *      Author: edoph
 */

/* audio_capture.h */
/*
 * audio_capture.h
 * ...
 */

#ifndef AUDIO_CAPTURE_H
#define AUDIO_CAPTURE_H

#include "stm32h7xx_hal.h"
#include "mfcce_xtract.h"  // Include để có MFCC_TOTAL_SIZE và extern mfcc_features

#define AUDIO_BUFFER_SIZE 10010U  // ~5s @ 2000Hz

// Double buffers (extern để dùng ở nhiều file)
extern int16_t audio_bufferA[AUDIO_BUFFER_SIZE];
extern int16_t audio_bufferB[AUDIO_BUFFER_SIZE];
extern volatile int16_t* current_buffer;
extern volatile uint8_t  audio_ready;

// Loại bỏ: extern float32_t mfcc_features[39 * 333];  // Vì đã có trong mfcce_xtract.h
// Loại bỏ: extern float32_t mfcc_final_features[39 * 333];  // Dư thừa

// Prototypes giữ nguyên...
void StartAudioCapture(void);
void ProcessAudioIfReady(void);
void DisplayAudioIntensity(int16_t* buffer, uint32_t size);

// Từ audio_sd.h (giữ nguyên nếu cần)
void sd_card_init(void);

#endif /* AUDIO_CAPTURE_H */

