#ifndef SYNTH_H
#define SYNTH_H

#include "music.h"

void generate_sine_wave(float* buffer, uint32_t length, Note* note);
void generate_drum_sound(float* buffer, uint32_t length, Note* note);
void generate_square_wave(float* buffer, uint32_t length, Note* note);
void apply_adsr(float* buffer, uint32_t length, uint8_t* adsr);
void pan_audio(float* buffer, uint32_t length, uint8_t panning);

#endif
