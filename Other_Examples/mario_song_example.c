// fm4_mario_intr.c
#include "fm4_wm8731_init.h"
#include "sine8000_table.h"

#define SAMPLING_FREQ   8000.0f
#define N               8000
#define AMPLITUDE       10
#define BPM             200
#define GAP             160

// Note durations
#define E   (int)(SAMPLING_FREQ * 30  / BPM)   // eighth
#define Q   (int)(SAMPLING_FREQ * 60  / BPM)   // quarter
#define DQ  (int)(SAMPLING_FREQ * 90  / BPM)   // dotted quarter
#define H   (int)(SAMPLING_FREQ * 120 / BPM)   // half

// Frequencies
#define REST  0.0f
#define E4    329.63f
#define F4    349.23f
#define G4    392.00f
#define A4    440.00f
#define Bb4   466.16f
#define B4    493.88f
#define C5    523.25f
#define D5    587.33f
#define E5    659.25f
#define F5    698.46f
#define G5    783.99f
#define A5    880.00f

typedef struct {
    float32_t freq;
    int       duration;
} Note;

Note melody[] = {
    // Phrase 1
    {E5,E}, {E5,E}, {REST,E}, {E5,E}, {REST,E}, {C5,E}, {E5,Q}, {G5,Q},
    {REST,Q}, {G4,Q}, {REST,Q},

    // Phrase 2
    {C5,DQ}, {G4,E}, {REST,Q},
    {E4,DQ}, {REST,E},
    {A4,Q}, {B4,Q}, {Bb4,E}, {A4,Q},
    {G4,E}, {E5,E}, {G5,Q}, {A5,Q},
    {F5,E}, {G5,E}, {REST,E}, {E5,Q},
    {C5,E}, {D5,E}, {B4,DQ},

    // Phrase 2 repeat
    {C5,DQ}, {G4,E}, {REST,Q},
    {E4,DQ}, {REST,E},
    {A4,Q}, {B4,Q}, {Bb4,E}, {A4,Q},
    {G4,E}, {E5,E}, {G5,Q}, {A5,Q},
    {F5,E}, {G5,E}, {REST,E}, {E5,Q},
    {C5,E}, {D5,E}, {B4,DQ},

    // End flourish
    {REST,E},
    {G5,E}, {F5,E}, {E5,E}, {D5,E}, {E5,Q},
    {A4,E}, {B4,Q}, {A4,E},
    {G4,E}, {E5,E}, {G5,Q}, {A5,Q},
    {F5,E}, {G5,E}, {REST,E}, {E5,Q},
    {C5,E}, {D5,E}, {B4,DQ},

    {REST,H}
};

#define NUM_NOTES (sizeof(melody) / sizeof(Note))

static int       note_idx     = 0;
static int       note_counter = 0;
static int       in_gap       = 0;
static float32_t float_index  = 0.0f;
static float32_t float_incr   = 0.0f;

void PRGCRC_I2S_IRQHandler(void) 
{
    union WM8731_data sample;
    int16_t out = 0;

    gpio_toggle(DIAGNOSTIC_PIN);
    sample.uint32bit = i2s_rx();

    if (note_idx < NUM_NOTES)
    {
        if (in_gap)
        {
            out = 0;
            note_counter--;
            if (note_counter <= 0)
            {
                in_gap = 0;
                note_idx++;
                if (note_idx < NUM_NOTES)
                {
                    float_index  = 0.0f;
                    float_incr   = melody[note_idx].freq * N / SAMPLING_FREQ;
                    note_counter = melody[note_idx].duration;
                }
            }
        }
        else
        {
            if (melody[note_idx].freq > 0.0f)
            {
                float_index += float_incr;
                if (float_index >= N) float_index -= N;
                out = (int16_t)(AMPLITUDE * sine8000[(int16_t)float_index]);
            }
            else
            {
                out = 0;
            }

            note_counter--;
            if (note_counter <= 0)
            {
                in_gap       = 1;
                note_counter = GAP;
            }
        }
    }

    sample.uint16bit[LEFT]  = out;
    sample.uint16bit[RIGHT] = out;
    i2s_tx(sample.uint32bit);
    NVIC_ClearPendingIRQ(PRGCRC_I2S_IRQn);
}

int main(void)
{
    float_incr   = melody[0].freq * N / SAMPLING_FREQ;
    note_counter = melody[0].duration;

    fm4_wm8731_init(FS_8000_HZ, WM8731_LINE_IN, IO_METHOD_INTR,
                    WM8731_HP_OUT_GAIN_0_DB, WM8731_LINE_IN_GAIN_0_DB);
    while(1){}
}