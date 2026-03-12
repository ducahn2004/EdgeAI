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
#include <arm_math.h>

#define DOWNSAMPLE_RATIO  24        // 48000 / 2000
#define FRAME_LEN         50        // 25ms @ 2kHz  ← giờ accessible ở main.c
#define HOP_SAMPLES       30        // 15ms @ 2kHz  ← thay vì 480
#define AUDIO_BUFFER_SIZE 1440      // 720*2 raw samples @48kHz
#define RING_BUFFER_SIZE  10000     // ~5 giây @2kHz

// Double buffers (extern để dùng ở nhiều file)
extern int16_t audio_bufferA[AUDIO_BUFFER_SIZE];

extern volatile int16_t* current_buffer;
extern volatile uint8_t  audio_ready;


extern int16_t ring_buffer[RING_BUFFER_SIZE];
extern volatile uint32_t rb_write;
extern volatile uint32_t rb_read;
// Loại bỏ: extern float32_t mfcc_features[39 * 333];  // Vì đã có trong mfcce_xtract.h
// Loại bỏ: extern float32_t mfcc_final_features[39 * 333];  // Dư thừa

void StartAudioCapture(void);

#endif /* AUDIO_CAPTURE_H */

