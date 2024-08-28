#include "music.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MusicFile* load_music_file(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) return NULL;

    MusicFile* music = (MusicFile*)malloc(sizeof(MusicFile));
    fread(&music->magic_number, sizeof(uint32_t), 1, file);
    fread(&music->version, sizeof(uint8_t), 1, file);
    fread(&music->track_count, sizeof(uint8_t), 1, file);
    fread(&music->sample_rate, sizeof(uint32_t), 1, file);

    music->tracks = (Track*)malloc(sizeof(Track) * music->track_count);
    for (uint32_t i = 0; i < music->track_count; ++i) {
        fread(&music->tracks[i].instrument, sizeof(InstrumentType), 1, file);
        fread(&music->tracks[i].note_count, sizeof(uint32_t), 1, file);

        music->tracks[i].notes = (Note*)malloc(sizeof(Note) * music->tracks[i].note_count);
        for (uint32_t j = 0; j < music->tracks[i].note_count; ++j) {
            fread(&music->tracks[i].notes[j].start_time_ms, sizeof(uint32_t), 1, file);
            fread(&music->tracks[i].notes[j].frequency, sizeof(float), 1, file);
            fread(&music->tracks[i].notes[j].duration_ms, sizeof(uint32_t), 1, file);
            fread(&music->tracks[i].notes[j].adsr, sizeof(uint8_t), 4, file);
            fread(&music->tracks[i].notes[j].amplitude, sizeof(float), 1, file);
            fread(&music->tracks[i].notes[j].panning, sizeof(uint8_t), 1, file);
        }
    }

    fclose(file);
    return music;
}

void free_music_file(MusicFile* music) {
    for (uint32_t i = 0; i < music->track_count; ++i) {
        free(music->tracks[i].notes);
    }
    free(music->tracks);
    free(music);
}

int write_music_file(const char* filename, MusicFile* music) {
    FILE* file = fopen(filename, "wb");
    if (!file) return -1;

    fwrite(&music->magic_number, sizeof(uint32_t), 1, file);
    fwrite(&music->version, sizeof(uint8_t), 1, file);
    fwrite(&music->track_count, sizeof(uint8_t), 1, file);
    fwrite(&music->sample_rate, sizeof(uint32_t), 1, file);

    for (uint32_t i = 0; i < music->track_count; ++i) {
        fwrite(&music->tracks[i].instrument, sizeof(InstrumentType), 1, file);
        fwrite(&music->tracks[i].note_count, sizeof(uint32_t), 1, file);

        for (uint32_t j = 0; j < music->tracks[i].note_count; ++j) {
            fwrite(&music->tracks[i].notes[j].start_time_ms, sizeof(uint32_t), 1, file);
            fwrite(&music->tracks[i].notes[j].frequency, sizeof(float), 1, file);
            fwrite(&music->tracks[i].notes[j].duration_ms, sizeof(uint32_t), 1, file);
            fwrite(&music->tracks[i].notes[j].adsr, sizeof(uint8_t), 4, file);
            fwrite(&music->tracks[i].notes[j].amplitude, sizeof(float), 1, file);
            fwrite(&music->tracks[i].notes[j].panning, sizeof(uint8_t), 1, file);
        }
    }

    fclose(file);
    return 0;
}
