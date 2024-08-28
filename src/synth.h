#ifndef SYNTH_H
#define SYNTH_H

#include "music.h"

void generate_guitar_wave(float* buffer, uint32_t length, float frequency, uint8_t* adsr);
void generate_square_wave(float* buffer, uint32_t length, float frequency, uint8_t* adsr);
void apply_adsr(float* buffer, uint32_t length, uint8_t* adsr);
void pan_audio(float* buffer, uint32_t length, uint8_t panning);

#endif
