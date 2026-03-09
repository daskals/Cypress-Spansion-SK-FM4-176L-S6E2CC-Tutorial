// fm4_amplify_intr.c
#include "fm4_wm8731_init.h"

#define GAIN                20.0f
#define SPEECH_THRESHOLD    500      // minimum level to be considered speech
#define GATE_HOLD_SAMPLES   3200     // how long to stay open after speech stops (0.1s at 32kHz)

static int32_t  gate_hold_counter = 0;
static float32_t dc_filter_state  = 0.0f;
#define DC_COEFF 0.995f

void PRGCRC_I2S_IRQHandler(void) 
{
    union WM8731_data sample;
    int16_t left;
    int32_t amplified;
    float32_t filtered;

    gpio_toggle(DIAGNOSTIC_PIN);

    sample.uint32bit = i2s_rx();
    left = sample.uint16bit[LEFT];

    // DC blocking filter
    filtered = (float32_t)left - dc_filter_state;
    dc_filter_state = DC_COEFF * dc_filter_state + 
                      (1.0f - DC_COEFF) * (float32_t)left;

    // Amplify
    amplified = (int32_t)(filtered * GAIN);

    // Clamp
    if (amplified >  32767) amplified =  32767;
    if (amplified < -32768) amplified = -32768;

    // Noise gate logic
    // If signal is loud enough, reset the hold counter (gate stays open)
    if (amplified > SPEECH_THRESHOLD || amplified < -SPEECH_THRESHOLD)
    {
        gate_hold_counter = GATE_HOLD_SAMPLES;
    }

    // Only output if gate is open, otherwise output silence
    if (gate_hold_counter > 0)
    {
        gate_hold_counter--;
        sample.uint16bit[LEFT]  = (int16_t)amplified;
        sample.uint16bit[RIGHT] = (int16_t)amplified;
    }
    else
    {
        // Gate closed — output silence, breaks the feedback loop
        sample.uint16bit[LEFT]  = 0;
        sample.uint16bit[RIGHT] = 0;
    }

    i2s_tx(sample.uint32bit);
    NVIC_ClearPendingIRQ(PRGCRC_I2S_IRQn);
}

int main(void)
{
    fm4_wm8731_init(FS_32000_HZ, WM8731_MIC_IN, IO_METHOD_INTR,
                    WM8731_HP_OUT_GAIN_6_DB, WM8731_LINE_IN_GAIN_6_DB);
    while(1){}
}