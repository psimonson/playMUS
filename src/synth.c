#include "synth.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

void generate_sine_wave(float *buffer, uint32_t length, Note* note) {
    for (size_t i = 0; i < length; i++) {
        buffer[i] = sin(2 * M_PI * note->frequency * i / 44100.0f);
    }
    apply_adsr(buffer, length, note->adsr);
}

void generate_drum_sound(float *buffer, uint32_t length, Note* note) {
    for (size_t i = 0; i < length; i++) {
        buffer[i] = (exp(-((double)i / (length / 10))) * sin(2 * M_PI * note->frequency * i / 44100.0f));
    }
    apply_adsr(buffer, length, note->adsr);
}

void generate_square_wave(float* buffer, uint32_t length, Note *note) {
	float period = 44100.0f / note->frequency;
    for (uint32_t i = 0; i < length; ++i) {
        buffer[i] = (fmodf((float)i, period) < period / 2) ? 1.0f : -1.0f;
    }
    apply_adsr(buffer, length, note->adsr);
}

void apply_adsr(float* buffer, uint32_t length, uint8_t* adsr) {
    uint32_t attack = adsr[0] * length / 255;
    uint32_t decay = adsr[1] * length / 255;
    uint32_t sustain = adsr[2];
    uint32_t release = adsr[3] * length / 255;

    for (uint32_t i = 0; i < attack; ++i) {
        buffer[i] *= (float)i / attack;
    }
    for (uint32_t i = attack; i < attack + decay; ++i) {
        buffer[i] *= 1.0f - (1.0f - sustain / 255.0f) * ((float)(i - attack) / decay);
    }
    for (uint32_t i = length - release; i < length; ++i) {
        buffer[i] *= (float)(length - i) / release;
    }
}

void pan_audio(float* buffer, uint32_t length, uint8_t panning) {
    float left_gain = 1.0f - (float)panning / 255.0f;
    float right_gain = (float)panning / 255.0f;
    for (uint32_t i = 0; i < length; i += 2) {
        buffer[i] *= left_gain;
        buffer[i + 1] *= right_gain;
    }
}
