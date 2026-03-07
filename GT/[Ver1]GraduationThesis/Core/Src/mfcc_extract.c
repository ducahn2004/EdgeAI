/*
 * mfcce_xtract.c
 *
 * Created on: Feb 7, 2026
 * Author: edoph
 */

#include "mfcc_extract.h"
#include "main.h"
#include <string.h>     // cho memmove, memcpy
#include <math.h>       // cho HUGE_VALF nếu cần
#include "arm_math.h"


// Cấu hình MFCC (đồng bộ với I2S nếu thay đổi sample rate)
#define SAMPLE_RATE     2000U   // Hz → NÊN SỬA THÀNH 16000 hoặc 48000 cho âm thanh thực tế
#define FRAME_LEN_MS    25      // ms
#define HOP_LEN_MS      15      // ms

#define FRAME_LEN       ((SAMPLE_RATE * FRAME_LEN_MS) / 1000)   // 50 @ 2kHz
#define HOP_LEN         ((SAMPLE_RATE * HOP_LEN_MS) / 1000)     // 30 @ 2kHz

#define FFT_LEN         128U
#define NUM_MFCC        13U
#define NUM_MFCC_TOTAL  (NUM_MFCC * 3)  // 39
#define NUM_MELS        40U
#define NUM_STAGES 2 // Bậc 4 Butterworth = 2 tầng Biquad


// Global buffers (khai báo extern trong .h nếu cần share)
arm_rfft_fast_instance_f32 S_Rfft;
MelFilterTypeDef S_MelFilter;
DCT_InstanceTypeDef S_DCT;
SpectrogramTypeDef S_Spectr;
MelSpectrogramTypeDef S_MelSpectr;
LogMelSpectrogramTypeDef S_LogMelSpectr;
MfccTypeDef S_Mfcc;


static arm_biquad_casd_df1_inst_f32 S_Filter;
static float32_t filter_state[4 * NUM_STAGES];

/* Coeffs are in CMSIS order: {b0,b1,b2, a1, a2} per stage.
 Note: CMSIS expects feedback coefficients with sign such that
 the processing uses the form with +a1*y[n-1], +a2*y[n-2]
 (see CMSIS docs). The arrays below are already prepared.
*/
/* Heart bandpass ≈ 20 - 600 Hz (fs = 16 kHz) - 4th order (2 stages) */
float32_t filter_coeffs[5 * NUM_STAGES] = {
/* stage 0 */ 1.0f, 2.0f, 1.0f, 1.9889416051454945f, -0.9890070054442315f,
/* stage 1 */ 1.0f, -2.0f, 1.0f, 1.6892470409865235f, -0.7338101667589392f
};


uint32_t NUM_MEL_COEFS = 0;
// Scratch buffers
float32_t pInFrame[FRAME_LEN];
float32_t pOutColBuffer[NUM_MFCC];
float32_t pWindowFuncBuffer[FRAME_LEN];
float32_t pSpectrScratchBuffer[FFT_LEN];
float32_t pDCTCoefsBuffer[NUM_MELS * NUM_MFCC];
float32_t pMfccScratchBuffer[NUM_MELS];
float32_t pMelFilterCoefs[4096];  // MAX_MEL_COEFS = 4096
uint32_t pMelFilterStartIndices[NUM_MELS];
uint32_t pMelFilterStopIndices[NUM_MELS];

// Global MFCC matrix và counter (dùng cho streaming)
float32_t mfcc_final_features[MFCC_FEATURES][MFCC_TIME_FRAMES] = {0};
uint32_t mfcc_collected = 0;

// Static state cho delta (per-frame)
static float32_t prev_mfcc[NUM_MFCC]   = {0};
static float32_t prev_delta[NUM_MFCC]  = {0};

void Preprocessing_Init(void)
{
    // Init window
    if (Window_Init(pWindowFuncBuffer, FRAME_LEN, WINDOW_HAMMING) != 0)
    {
        Error_Handler();
    }

    // Init RFFT
    arm_rfft_fast_init_f32(&S_Rfft, FFT_LEN);

    // Init Mel filterbank
    S_MelFilter.pStartIndices = pMelFilterStartIndices;
    S_MelFilter.pStopIndices  = pMelFilterStopIndices;
    S_MelFilter.pCoefficients = pMelFilterCoefs;
    S_MelFilter.NumMels       = NUM_MELS;
    S_MelFilter.FFTLen        = FFT_LEN;
    S_MelFilter.SampRate      = SAMPLE_RATE;
    S_MelFilter.FMin          = 0.0f;
    S_MelFilter.FMax          = SAMPLE_RATE / 2.0f;
    S_MelFilter.Formula       = MEL_SLANEY;
    S_MelFilter.Normalize     = 1;
    S_MelFilter.Mel2F         = 1;

    MelFilterbank_Init(&S_MelFilter);

    if (S_MelFilter.CoefficientsLength > sizeof(pMelFilterCoefs)/sizeof(float32_t))
    {
        Error_Handler();
    }
    NUM_MEL_COEFS = S_MelFilter.CoefficientsLength;

    // Init DCT
    S_DCT.NumFilters    = NUM_MFCC;
    S_DCT.NumInputs     = NUM_MELS;
    S_DCT.Type          = DCT_TYPE_II_ORTHO;
    S_DCT.RemoveDCTZero = 0;
    S_DCT.pDCTCoefs     = pDCTCoefsBuffer;
    if (DCT_Init(&S_DCT) != 0)
    {
        Error_Handler();
    }

    // Init Spectrogram → MelSpectrogram → LogMel → MFCC
    S_Spectr.pRfft     = &S_Rfft;
    S_Spectr.Type      = SPECTRUM_TYPE_POWER;
    S_Spectr.pWindow   = pWindowFuncBuffer;
    S_Spectr.SampRate  = SAMPLE_RATE;
    S_Spectr.FrameLen  = FRAME_LEN;
    S_Spectr.FFTLen    = FFT_LEN;
    S_Spectr.pScratch  = pSpectrScratchBuffer;

    S_MelSpectr.SpectrogramConf = &S_Spectr;
    S_MelSpectr.MelFilter       = &S_MelFilter;

    S_LogMelSpectr.MelSpectrogramConf = &S_MelSpectr;
    S_LogMelSpectr.LogFormula         = LOGMELSPECTROGRAM_SCALE_DB;
    S_LogMelSpectr.Ref                = 1.0f;
    S_LogMelSpectr.TopdB              = HUGE_VALF;

    S_Mfcc.LogMelConf    = &S_LogMelSpectr;
    S_Mfcc.pDCT          = &S_DCT;
    S_Mfcc.NumMfccCoefs  = NUM_MFCC;   // 13
    S_Mfcc.pScratch      = pMfccScratchBuffer;
}

void mfcc_append_frame(float *new_frame)
{
    // Shift left (dịch các frame cũ sang trái)
    memmove(&mfcc_final_features[0][0],
            &mfcc_final_features[0][1],
            sizeof(float) * MFCC_FEATURES * (MFCC_TIME_FRAMES - 1));

    // Copy frame mới vào cột cuối
    for (int i = 0; i < MFCC_FEATURES; i++)
    {
        mfcc_final_features[i][MFCC_TIME_FRAMES - 1] = new_frame[i];
    }

    // Tăng counter
    if (mfcc_collected < MFCC_TIME_FRAMES)
    {
        mfcc_collected++;
    }
}
void Filter_Init(void) {
    arm_biquad_cascade_df1_init_f32(&S_Filter, NUM_STAGES, filter_coeffs, filter_state);
}
/**
 * @brief Tính MFCC + Δ + ΔΔ cho **một frame** (dùng trong main loop streaming)
 * @param pInSignal: con trỏ đến frame int16_t (kích thước HOP_SAMPLES, nhưng chỉ dùng FRAME_LEN mẫu)
 * @param pOutMfccFrame: output float[39]
 */
void compute_mfcc_one_frame(int16_t *pInSignal, float *pOutMfccFrame)
{
    if (!pInSignal || !pOutMfccFrame) return;

    // Normalize int16_t -> float [-1.0 .. 1.0]
    buf_to_float_normed(pInSignal, pInFrame, FRAME_LEN);

    // 2. ÁP DỤNG BỘ LỌC BANDPASS TẠI ĐÂY
    arm_biquad_cascade_df1_f32(&S_Filter, pInFrame, pInFrame, FRAME_LEN);

    // Tính MFCC cơ bản
    MfccColumn(&S_Mfcc, pInFrame, pOutColBuffer);

    // Tính delta & delta-delta
    float32_t delta[NUM_MFCC];
    float32_t delta_delta[NUM_MFCC];

    for (uint32_t i = 0; i < NUM_MFCC; i++)
    {
        delta[i] = 0.5f * (pOutColBuffer[i] - prev_mfcc[i]);
    }

    for (uint32_t i = 0; i < NUM_MFCC; i++)
    {
        delta_delta[i] = 0.5f * (delta[i] - prev_delta[i]);
    }

    // Update trạng thái
    memcpy(prev_mfcc,  pOutColBuffer,   sizeof(prev_mfcc));
    memcpy(prev_delta, delta,           sizeof(prev_delta));

    // Ghép thành vector 39
    for (uint32_t i = 0; i < NUM_MFCC; i++)
    {
        pOutMfccFrame[i]                  = pOutColBuffer[i];
        pOutMfccFrame[NUM_MFCC + i]       = delta[i];
        pOutMfccFrame[2 * NUM_MFCC + i]   = delta_delta[i];
    }
}

// (Tùy chọn) Nếu vẫn muốn giữ hàm batch cũ, có thể comment lại hoặc xóa
// void AudioPreprocessing_Run(...) { ... }
