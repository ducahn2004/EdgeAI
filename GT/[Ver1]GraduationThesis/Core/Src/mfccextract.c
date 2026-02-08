/*
 * mfccextract.c
 *
 *  Created on: Feb 7, 2026
 *      Author: edoph
 */

#include "mfccextract.h"



arm_rfft_fast_instance_f32 S_Rfft;
MelFilterTypeDef           S_MelFilter;
DCT_InstanceTypeDef        S_DCT;
SpectrogramTypeDef         S_Spectr;
MelSpectrogramTypeDef      S_MelSpectr;
LogMelSpectrogramTypeDef   S_LogMelSpectr;
MfccTypeDef                S_Mfcc;

float32_t pInFrame[FRAME_LEN];
float32_t pOutColBuffer[NUM_MFCC];
float32_t pWindowFuncBuffer[FRAME_LEN];
float32_t pSpectrScratchBuffer[FFT_LEN];
float32_t pDCTCoefsBuffer[NUM_MELS * NUM_MFCC];
float32_t pMfccScratchBuffer[NUM_MELS];
float32_t pMelFilterCoefs[NUM_MEL_COEFS];
uint32_t pMelFilterStartIndices[NUM_MELS];
uint32_t pMelFilterStopIndices[NUM_MELS];


void Preprocessing_Init(void)
{
  /* Init window function */
  if (Window_Init(pWindowFuncBuffer, FRAME_LEN, WINDOW_HANN) != 0)
  {
    while(1);
  }

  /* Init RFFT */
  arm_rfft_fast_init_f32(&S_Rfft, FFT_LEN);

  /* Init mel filterbank */
  S_MelFilter.pStartIndices = pMelFilterStartIndices;
  S_MelFilter.pStopIndices  = pMelFilterStopIndices;
  S_MelFilter.pCoefficients = pMelFilterCoefs;
  S_MelFilter.NumMels   = NUM_MELS;
  S_MelFilter.FFTLen    = FFT_LEN;
  S_MelFilter.SampRate  = SAMPLE_RATE;
  S_MelFilter.FMin      = 0.0;
  S_MelFilter.FMax      = S_MelFilter.SampRate / 2.0;
  S_MelFilter.Formula   = MEL_SLANEY;
  S_MelFilter.Normalize = 1;
  S_MelFilter.Mel2F     = 1;
  MelFilterbank_Init(&S_MelFilter);
  if (S_MelFilter.CoefficientsLength != NUM_MEL_COEFS)
  {
    while(1); /* Adjust NUM_MEL_COEFS to match S_MelFilter.CoefficientsLength */
  }

  /* Init DCT operation */
  S_DCT.NumFilters    = NUM_MFCC;
  S_DCT.NumInputs     = NUM_MELS;
  S_DCT.Type          = DCT_TYPE_II_ORTHO;
  S_DCT.RemoveDCTZero = 0;
  S_DCT.pDCTCoefs     = pDCTCoefsBuffer;
  if (DCT_Init(&S_DCT) != 0)
  {
    while(1);
  }

  /* Init Spectrogram */
  S_Spectr.pRfft    = &S_Rfft;
  S_Spectr.Type     = SPECTRUM_TYPE_POWER;
  S_Spectr.pWindow  = pWindowFuncBuffer;
  S_Spectr.SampRate = SAMPLE_RATE;
  S_Spectr.FrameLen = FRAME_LEN;
  S_Spectr.FFTLen   = FFT_LEN;
  S_Spectr.pScratch = pSpectrScratchBuffer;

  /* Init MelSpectrogram */
  S_MelSpectr.SpectrogramConf = &S_Spectr;
  S_MelSpectr.MelFilter       = &S_MelFilter;

  /* Init LogMelSpectrogram */
  S_LogMelSpectr.MelSpectrogramConf = &S_MelSpectr;
  S_LogMelSpectr.LogFormula         = LOGMELSPECTROGRAM_SCALE_DB;
  S_LogMelSpectr.Ref                = 1.0;
  S_LogMelSpectr.TopdB              = HUGE_VALF;

  /* Init MFCC */
  S_Mfcc.LogMelConf   = &S_LogMelSpectr;
  S_Mfcc.pDCT         = &S_DCT;
  S_Mfcc.NumMfccCoefs = 20;
  S_Mfcc.pScratch     = pMfccScratchBuffer;
}

void AudioPreprocessing_Run(int16_t *pInSignal, float32_t *pOutMfcc, uint32_t signal_len)
{
  const uint32_t num_frames = 1 + (signal_len - FRAME_LEN) / HOP_LEN;

  for (uint32_t frame_index = 0; frame_index < num_frames; frame_index++)
  {
    buf_to_float_normed(&pInSignal[HOP_LEN * frame_index], pInFrame, FRAME_LEN);
    MfccColumn(&S_Mfcc, pInFrame, pOutColBuffer);
    /* Reshape column into pOutMfcc */
    for (uint32_t i = 0; i < NUM_MFCC; i++)
    {
      pOutMfcc[i * num_frames + frame_index] = pOutColBuffer[i];
    }
  }
}
