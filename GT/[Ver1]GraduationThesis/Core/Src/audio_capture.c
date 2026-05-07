/*
 * audio_capture.c
 */

#include "stm32h7xx_hal.h"
#include "audio_capture.h"
#include "app_x-cube-ai.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>

extern I2S_HandleTypeDef hi2s1;
extern UART_HandleTypeDef huart3;

/* Audio buffers */
int16_t           audio_bufferA[AUDIO_BUFFER_SIZE];
volatile int16_t* current_buffer = audio_bufferA;
volatile uint8_t  audio_ready    = 0;

int16_t           ring_buffer[RING_BUFFER_SIZE] = {0};
volatile uint32_t rb_write = 0;
volatile uint32_t rb_read  = 0;

/* Debug counters */
volatile uint32_t dbg_i2s_half_count     = 0;
volatile uint32_t dbg_i2s_full_count     = 0;
volatile uint32_t dbg_ring_push_count    = 0;
volatile uint32_t dbg_ring_overflow_count = 0;
volatile uint32_t dbg_audio_ready_count  = 0;

/* Debug timing */
volatile uint32_t dbg_last_half_time_ms = 0;
volatile uint32_t dbg_last_full_time_ms = 0;
volatile uint32_t dbg_last_push_time_ms = 0;
volatile uint32_t dbg_last_rms_time_ms  = 0;

/* Flags để log ngoài interrupt */
volatile uint8_t dbg_half_flag = 0;
volatile uint8_t dbg_full_flag = 0;
volatile uint8_t dbg_overflow_flag = 0;

static uint32_t dbg_last_log_ms = 0;

/* UART log helper */
static void UART_Log(const char *fmt, ...)
{
    char buf[160];
    va_list args;

    va_start(args, fmt);
    int len = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    if (len <= 0) return;
    if (len > sizeof(buf)) len = sizeof(buf);

    HAL_UART_Transmit(&huart3, (uint8_t*)buf, len, HAL_MAX_DELAY);
}

/* Tính số sample đang có trong ring buffer */
static uint32_t RingBuffer_Used(void)
{
    if (rb_write >= rb_read)
        return rb_write - rb_read;
    else
        return RING_BUFFER_SIZE - rb_read + rb_write;
}

void StartAudioCapture(void)
{
    UART_Log("\r\n[AUDIO] Start capture\r\n");

    HAL_StatusTypeDef st;

    st = HAL_I2S_Receive_DMA(&hi2s1,
                             (uint16_t*)audio_bufferA,
                             AUDIO_BUFFER_SIZE);

    if (st == HAL_OK)
        UART_Log("[AUDIO] I2S DMA started OK\r\n");
    else
        UART_Log("[AUDIO_ERR] I2S DMA start failed, status=%d\r\n", st);

    UART_Log("[AUDIO] PWM LEDs started\r\n");
}

/*
 * Downsample 48kHz -> 2kHz
 */
static void audio_push_to_ring(int16_t *data, uint32_t len)
{
    uint32_t t0 = HAL_GetTick();

    for (uint32_t i = 0; i < len; i += DOWNSAMPLE_RATIO)
    {
        uint32_t next = (rb_write + 1) % RING_BUFFER_SIZE;

        if (next == rb_read)
        {
            dbg_ring_overflow_count++;
            dbg_overflow_flag = 1;
            break;
        }

        ring_buffer[rb_write] = data[i];
        rb_write = next;
        dbg_ring_push_count++;
    }

    dbg_last_push_time_ms = HAL_GetTick() - t0;
}

/* Half-callback: không log UART trực tiếp trong interrupt */
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    uint32_t t0 = HAL_GetTick();

    if (hi2s->Instance == SPI1)
    {
        dbg_i2s_half_count++;

        audio_push_to_ring(audio_bufferA, AUDIO_BUFFER_SIZE / 2);

        audio_ready = 1;
        dbg_audio_ready_count++;
        dbg_half_flag = 1;
    }

    dbg_last_half_time_ms = HAL_GetTick() - t0;
}

/* Full-callback: không log UART trực tiếp trong interrupt */
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    uint32_t t0 = HAL_GetTick();

    if (hi2s->Instance == SPI1)
    {
        dbg_i2s_full_count++;

        audio_push_to_ring(&audio_bufferA[AUDIO_BUFFER_SIZE / 2],
                           AUDIO_BUFFER_SIZE / 2);

        audio_ready = 1;
        dbg_audio_ready_count++;
        dbg_full_flag = 1;
    }

    dbg_last_full_time_ms = HAL_GetTick() - t0;
}

/*
 * Gọi hàm này trong while(1)
 */
void Audio_DebugLog_Process(void)
{
    uint32_t now = HAL_GetTick();

    if (dbg_half_flag)
    {
        dbg_half_flag = 0;
        UART_Log("[I2S] HALF count=%lu time=%lu ms\r\n",
                 dbg_i2s_half_count,
                 dbg_last_half_time_ms);
    }

    if (dbg_full_flag)
    {
        dbg_full_flag = 0;
        UART_Log("[I2S] FULL count=%lu time=%lu ms\r\n",
                 dbg_i2s_full_count,
                 dbg_last_full_time_ms);
    }

    if (dbg_overflow_flag)
    {
        dbg_overflow_flag = 0;
        UART_Log("[RING_ERR] overflow=%lu rb_w=%lu rb_r=%lu used=%lu\r\n",
                 dbg_ring_overflow_count,
                 rb_write,
                 rb_read,
                 RingBuffer_Used());
    }

    /* Log tổng mỗi 1 giây */
    if (now - dbg_last_log_ms >= 1000)
    {
        dbg_last_log_ms = now;

        UART_Log("[AUDIO_STAT] half=%lu full=%lu ready=%lu push=%lu used=%lu overflow=%lu push_t=%lu ms rms_t=%lu ms\r\n",
                 dbg_i2s_half_count,
                 dbg_i2s_full_count,
                 dbg_audio_ready_count,
                 dbg_ring_push_count,
                 RingBuffer_Used(),
                 dbg_ring_overflow_count,
                 dbg_last_push_time_ms,
                 dbg_last_rms_time_ms);
    }
}