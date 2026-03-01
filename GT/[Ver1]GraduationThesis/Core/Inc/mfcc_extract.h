/**
 ******************************************************************************
 * @file    mfcc_example.c
 * @author  MCD Application Team
 * @brief   MFCC computation example
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Software License Agreement
 * SLA0055, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *        www.st.com/resource/en/license_agreement/dm00251784.pdf
 *
 ******************************************************************************
 */

#ifndef MFCCE_XTRACT_H
#define MFCCE_XTRACT_H


#include "feature_extraction.h"
#include "arm_math.h"

#define MFCC_FEATURES       39U
#define MFCC_TIME_FRAMES    333U
#define MFCC_TOTAL_SIZE (MFCC_FEATURES * MFCC_TIME_FRAMES)

extern float32_t mfcc_final_features[39][333];
extern uint32_t mfcc_collected;
extern void compute_mfcc_one_frame(int16_t *frame, float *out_39);
extern void mfcc_append_frame(float *new_frame);



/* =====================================================================
 * Thông số phù hợp với PhysioNet 2016 + model input 1×39×333
 * ===================================================================== */

void Preprocessing_Init(void);
void AudioPreprocessing_Run(int16_t *pInSignal, float32_t *pOutMfcc, uint32_t signal_len);


#endif /* MFCCE_XTRACT_H */
