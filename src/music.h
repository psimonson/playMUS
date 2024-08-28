#ifndef MUSIC_H
#define MUSIC_H

#include <stdint.h>

#define MAGIC_NUMBER 0x4D555349 // "MUSIC"
#define VERSION 0x01

typedef enum {
    INSTRUMENT_GUITAR = 0x01,
    INSTRUMENT_SNARE_DRUM = 0x02,
    INSTRUMENT_BASS_DRUM = 0x03
} InstrumentType;

typedef struct {
    uint32_t start_time_ms;
    float frequency;
    uint32_t duration_ms;
    uint8_t adsr[4]; // Attack, Decay, Sustain, Release
    float amplitude;
    uint8_t panning;
} Note;

typedef struct {
    InstrumentType instrument;
    uint32_t note_count;
    Note* notes;
} Track;

typedef struct {
    uint32_t magic_number;
    uint8_t version;
    uint8_t track_count;
    uint32_t sample_rate;
    Track* tracks;
} MusicFile;

MusicFile* load_music_file(const char* filename);
void free_music_file(MusicFile* music);
void play_music_file(MusicFile* music);
int write_music_file(const char* filename, MusicFile* music); // New function prototype

#endif
