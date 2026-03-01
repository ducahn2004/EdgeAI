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

#define AUDIO_BUFFER_SIZE       1024    // ví dụ - phải chia hết cho 2 (DMA half/full)
#define RING_BUFFER_SIZE        (16000 * 4)     // 4 giây @ 16kHz
#define HOP_SAMPLES             480     // thường 10–20 ms → 160–640 mẫu @16kHz

// Double buffers (extern để dùng ở nhiều file)
extern int16_t audio_bufferA[AUDIO_BUFFER_SIZE];
extern int16_t audio_bufferB[AUDIO_BUFFER_SIZE];
extern volatile int16_t* current_buffer;
extern volatile uint8_t  audio_ready;


extern int16_t ring_buffer[RING_BUFFER_SIZE];
extern volatile uint32_t rb_write;
extern volatile uint32_t rb_read;
// Loại bỏ: extern float32_t mfcc_features[39 * 333];  // Vì đã có trong mfcce_xtract.h
// Loại bỏ: extern float32_t mfcc_final_features[39 * 333];  // Dư thừa

void StartAudioCapture(void);

#endif /* AUDIO_CAPTURE_H */

