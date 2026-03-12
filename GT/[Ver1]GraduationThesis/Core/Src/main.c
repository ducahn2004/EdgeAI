/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "app_x-cube-ai.h"

/* USER CODE BEGIN Includes */
#include "audio_capture.h"   // định nghĩa FRAME_LEN, HOP_SAMPLES, RING_BUFFER_SIZE
#include "mfcc_extract.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2S_HandleTypeDef hi2s1;
SD_HandleTypeDef  hsd1;
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2S1_Init(void);
static void MX_SDMMC1_SD_Init(void);
static void MX_TIM2_Init(void);

/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
extern int16_t           ring_buffer[];
extern volatile uint32_t rb_write;
extern volatile uint32_t rb_read;
extern float32_t         mfcc_final_features[MFCC_FEATURES][MFCC_TIME_FRAMES];
extern uint32_t          mfcc_collected;
/* USER CODE END 0 */

int main(void)
{
    /* USER CODE BEGIN 1 */
    /* USER CODE END 1 */

    MPU_Config();
    SCB_EnableICache();
    SCB_EnableDCache();
    HAL_Init();

    /* USER CODE BEGIN Init */
    /* USER CODE END Init */

    SystemClock_Config();

    /* USER CODE BEGIN SysInit */
    /* USER CODE END SysInit */

    MX_GPIO_Init();
    MX_I2S1_Init();
    MX_SDMMC1_SD_Init();
    MX_FATFS_Init();
    MX_TIM2_Init();
    MX_X_CUBE_AI_Init();

    /* USER CODE BEGIN 2 */
    Preprocessing_Init();
    /* USER CODE END 2 */

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);   // PA0 - Đỏ (cường độ)
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);   // PA2 - Xanh lá (MFCC)
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);   // PA3 - Xanh dương (AI)

    current_buffer = audio_bufferA;
    HAL_I2S_Receive_DMA(&hi2s1, (uint16_t*)audio_bufferA, AUDIO_BUFFER_SIZE);

    /* USER CODE BEGIN WHILE */

    /*
     * frame_accum: buffer tích lũy FRAME_LEN=50 samples @2kHz.
     * Mỗi vòng lặp đọc thêm HOP_SAMPLES=30 samples mới,
     * slide window bằng memmove, rồi tính MFCC.
     */
    static int16_t frame_accum[FRAME_LEN];   // FRAME_LEN từ audio_capture.h = 50

    while (1)
    {
        /* Đọc available một cách atomic */
        __disable_irq();
        uint32_t available = (rb_write - rb_read + RING_BUFFER_SIZE) % RING_BUFFER_SIZE;
        __enable_irq();

        while (available >= HOP_SAMPLES)   // HOP_SAMPLES từ audio_capture.h = 30
        {
            /* 1. Slide window: bỏ HOP_SAMPLES cũ ở đầu, nhường chỗ cho mới */
            memmove(frame_accum,
                    frame_accum + HOP_SAMPLES,
                    (FRAME_LEN - HOP_SAMPLES) * sizeof(int16_t));

            /* 2. Đọc HOP_SAMPLES mới từ ring buffer vào cuối frame_accum */
            for (uint32_t i = 0; i < HOP_SAMPLES; i++)
            {
                frame_accum[FRAME_LEN - HOP_SAMPLES + i] = ring_buffer[rb_read];
                rb_read = (rb_read + 1) % RING_BUFFER_SIZE;
            }

            /* 3. Tính MFCC + Δ + ΔΔ cho frame hiện tại */
            float mfcc_frame[MFCC_FEATURES];   // 39 features
            compute_mfcc_one_frame(frame_accum, mfcc_frame);

            /* 4. Append vào matrix [39][333] — sliding window */
            mfcc_append_frame(mfcc_frame);

            available -= HOP_SAMPLES;

            /* 5. Khi đã đủ 333 frame → chạy inference */
            if (mfcc_collected >= MFCC_TIME_FRAMES)
            {
                /* Bật LED xanh dương báo đang chạy AI */
                __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 800);

                MX_X_CUBE_AI_Process();

                /* Tắt LED */
                __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);

                /*
                 * Giữ nguyên mfcc_collected (không reset) → sliding window:
                 * inference chạy lại sau mỗi HOP_SAMPLES mới.
                 * Nếu muốn batch mode (chờ 333 frame mới hoàn toàn):
                 *   mfcc_collected = 0;
                 */
            }
        }
        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/* ===== Phần còn lại giữ nguyên từ CubeMX ===== */

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);
    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_BYPASS;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM       = 13;
    RCC_OscInitStruct.PLL.PLLN       = 480;
    RCC_OscInitStruct.PLL.PLLP       = 2;
    RCC_OscInitStruct.PLL.PLLQ       = 20;
    RCC_OscInitStruct.PLL.PLLR       = 2;
    RCC_OscInitStruct.PLL.PLLRGE     = RCC_PLL1VCIRANGE_1;
    RCC_OscInitStruct.PLL.PLLVCOSEL  = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN   = 0;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) Error_Handler();

    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                     | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2
                                     | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) Error_Handler();
}

static void MX_I2S1_Init(void)
{
    hi2s1.Instance                  = SPI1;
    hi2s1.Init.Mode                 = I2S_MODE_MASTER_RX;
    hi2s1.Init.Standard             = I2S_STANDARD_PHILIPS;
    hi2s1.Init.DataFormat           = I2S_DATAFORMAT_16B;
    hi2s1.Init.MCLKOutput           = I2S_MCLKOUTPUT_DISABLE;
    hi2s1.Init.AudioFreq            = I2S_AUDIOFREQ_48K;
    hi2s1.Init.CPOL                 = I2S_CPOL_LOW;
    hi2s1.Init.FirstBit             = I2S_FIRSTBIT_MSB;
    hi2s1.Init.WSInversion          = I2S_WS_INVERSION_DISABLE;
    hi2s1.Init.Data24BitAlignment   = I2S_DATA_24BIT_ALIGNMENT_RIGHT;
    hi2s1.Init.MasterKeepIOState    = I2S_MASTER_KEEP_IO_STATE_DISABLE;
    if (HAL_I2S_Init(&hi2s1) != HAL_OK) Error_Handler();
}

static void MX_SDMMC1_SD_Init(void)
{
    hsd1.Instance                        = SDMMC1;
    hsd1.Init.ClockEdge                  = SDMMC_CLOCK_EDGE_RISING;
    hsd1.Init.ClockPowerSave             = SDMMC_CLOCK_POWER_SAVE_DISABLE;
    hsd1.Init.BusWide                    = SDMMC_BUS_WIDE_4B;
    hsd1.Init.HardwareFlowControl        = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
    hsd1.Init.ClockDiv                   = 4;
}

static void MX_TIM2_Init(void)
{
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef      sConfigOC     = {0};

    htim2.Instance               = TIM2;
    htim2.Init.Prescaler         = 479;
    htim2.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim2.Init.Period            = 999;
    htim2.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) Error_Handler();

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) Error_Handler();

    sConfigOC.OCMode     = TIM_OCMODE_PWM1;
    sConfigOC.Pulse      = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) Error_Handler();
    if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK) Error_Handler();
    if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK) Error_Handler();

    HAL_TIM_MspPostInit(&htim2);
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin   = GPIO_PIN_1;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA1, SYSCFG_SWITCH_PA1_CLOSE);
}

void MPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct = {0};

    HAL_MPU_Disable();

    MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
    MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
    MPU_InitStruct.BaseAddress      = 0x0;
    MPU_InitStruct.Size             = MPU_REGION_SIZE_4GB;
    MPU_InitStruct.SubRegionDisable = 0x87;
    MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
    MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
    MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
    MPU_InitStruct.IsShareable      = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

void Error_Handler(void)
{
    __disable_irq();
    while (1) {}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {}
#endif