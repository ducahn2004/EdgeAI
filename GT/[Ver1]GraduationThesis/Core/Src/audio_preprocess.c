/*
 * audio_preprocess.c
 *
 *  Created on: Feb 8, 2026
 *      Author: edoph
 */


#include "arm_math.h"  // CMSIS-DSP

#define FS 2000.0f
#define HOP_MS 15
#define WIN_MS 25
#define N_MFCC 13
#define N_FRAMES 333
#define AUDIO_LEN 10010
#define HOP_SIZE (int)(FS * HOP_MS / 1000)  // 30
#define WIN_SIZE (int)(FS * WIN_MS / 1000)  // 50

// Bandpass coefficients (tính từ Python scipy.signal.butter(5, [25,400]/ (FS/2), btype='band'))
// Export sang C: b = [b0,b1,...], a = [1,a1,...] → cascade biquad (5 sections cho order 10? Wait, order5 → 3 biquad).
// Ví dụ placeholder (tính chính xác bằng code Python):
float32_t bandpass_coeffs[30] = {  // 6 coeffs/section x 5 sections (order 10? Sửa: order5 → butter return b,a len6 → 3 biquad)
    // b0,b1,b2,a1,a2 per section... (chạy Python để lấy)
    // e.g., from butter_bandpass(25,400,2000,5)
};
arm_biquad_casd_df1_inst_f32 filter_inst;
float32_t filter_state[12];  // 4*num_sections (3 sections)

// MFCC: Cần implement FFT + Mel + DCT. Sử dụng kissfft hoặc CMSIS arm_rfft.
// Để đơn giản, port lib mfcc đơn giản (e.g., từ GitHub: libmfcc).
// Hoặc implement basic:
void extract_mfcc(float32_t* audio, float32_t* mfcc_out, uint32_t fs, float win_ms, float hop_ms, uint32_t n_mfcc);

// Implement extract_mfcc: Framing → Hamming → FFT → Mel filter (precompute matrix) → Log → DCT.
// Precompute: Hamming window, Mel filterbank (40-60 filters, giữ 13 coeffs sau DCT).
