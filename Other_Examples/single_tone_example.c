// fm4_tone_intr.c
#include "fm4_wm8731_init.h"
#include "sine8000_table.h"  // one cycle with 8000 points

#define SAMPLING_FREQ 8000.0
#define N             8000
#define FREQUENCY     1000.0   // change this to get different tones
#define AMPLITUDE     10
#define INCR          (FREQUENCY * N / SAMPLING_FREQ)  // = 1000 steps per sample

float32_t float_index = 0.0;

void PRGCRC_I2S_IRQHandler(void) 
{
    union WM8731_data sample;
    int16_t int_index;

    gpio_toggle(DIAGNOSTIC_PIN);

    sample.uint32bit = i2s_rx();

    // Advance through the sine table
    float_index += INCR;
    if (float_index >= N) float_index -= N;

    int_index = (int16_t)(float_index);

    sample.uint16bit[LEFT]  = (int16_t)(AMPLITUDE * sine8000[int_index]);
    sample.uint16bit[RIGHT] = sample.uint16bit[LEFT];

    i2s_tx(sample.uint32bit);
    NVIC_ClearPendingIRQ(PRGCRC_I2S_IRQn);
}

int main(void)
{
    fm4_wm8731_init(FS_8000_HZ, WM8731_LINE_IN, IO_METHOD_INTR,
                    WM8731_HP_OUT_GAIN_0_DB, WM8731_LINE_IN_GAIN_0_DB);
    while(1){}
}