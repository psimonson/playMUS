#include "music.h"
#include "synth.h"
#include <pulse/simple.h>
#include <pulse/error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAMPLE_RATE 44100

// Function to create a complex 1-minute song
MusicFile* create_complex_song() {
    MusicFile* music = (MusicFile*)malloc(sizeof(MusicFile));
    if (!music) {
        perror("Failed to allocate MusicFile");
        exit(EXIT_FAILURE);
    }

    music->track_count = 3;
    music->tracks = (Track*)malloc(sizeof(Track) * music->track_count);
    if (!music->tracks) {
        perror("Failed to allocate tracks");
        free(music);
        exit(EXIT_FAILURE);
    }

    // Guitar track
    Track* guitar_track = &music->tracks[0];
	guitar_track->instrument = INSTRUMENT_GUITAR;
    guitar_track->note_count = 10;
    guitar_track->notes = (Note*)malloc(sizeof(Note) * guitar_track->note_count);
    if (!guitar_track->notes) {
        perror("Failed to allocate guitar notes");
        free(music->tracks);
        free(music);
        exit(EXIT_FAILURE);
    }

    // Add notes to guitar track
    for (uint32_t i = 0; i < guitar_track->note_count; i++) {
        guitar_track->notes[i] = (Note){
            .start_time_ms = i * 3000,
            .frequency = 320.0f + i * 2,
            .duration_ms = 1000,
            .adsr = {10, 200, 50, 100},
            .amplitude = 0.8f,
            .panning = 128
        };
    }

    // Snare drum track
    Track* snare_track = &music->tracks[1];
	snare_track->instrument = INSTRUMENT_SNARE_DRUM;
    snare_track->note_count = 4;
    snare_track->notes = (Note*)malloc(sizeof(Note) * snare_track->note_count);
    if (!snare_track->notes) {
        perror("Failed to allocate snare notes");
        free(guitar_track->notes);
        free(music->tracks);
        free(music);
        exit(EXIT_FAILURE);
    }

    // Add notes to snare drum track
    for (uint32_t i = 0; i < snare_track->note_count; i++) {
        snare_track->notes[i] = (Note){
            .start_time_ms = i * 15000,
            .frequency = 100.0f,
            .duration_ms = 1000,
            .adsr = {5, 100, 50, 50},
            .amplitude = 0.5f,
            .panning = 128
        };
    }

    // Bass drum track
    Track* bass_track = &music->tracks[2];
	bass_track->instrument = INSTRUMENT_BASS_DRUM;
    bass_track->note_count = 4;
    bass_track->notes = (Note*)malloc(sizeof(Note) * bass_track->note_count);
    if (!bass_track->notes) {
        perror("Failed to allocate bass notes");
        free(snare_track->notes);
        free(guitar_track->notes);
        free(music->tracks);
        free(music);
        exit(EXIT_FAILURE);
    }

    // Add notes to bass drum track
    for (uint32_t i = 0; i < bass_track->note_count; i++) {
        bass_track->notes[i] = (Note){
            .start_time_ms = i * 15000 + 500,
            .frequency = 60.0f,
            .duration_ms = 1000,
            .adsr = {5, 100, 50, 50},
            .amplitude = 0.7f,
            .panning = 128
        };
    }

    return music;
}

void play_music(MusicFile* music) {
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_FLOAT32LE,
        .rate = 44100,
        .channels = 2
    };
    int error;

    pa_simple* s = pa_simple_new(NULL, "MusicPlayer", PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error);
    if (!s) {
        fprintf(stderr, "pa_simple_new() failed: %s\n", pa_strerror(error));
        return;
    }

    for (uint32_t i = 0; i < music->track_count; ++i) {
        Track* track = &music->tracks[i];
        for (uint32_t j = 0; j < track->note_count; ++j) {
            Note* note = &track->notes[j];
            uint32_t length = (note->duration_ms * ss.rate) / 1000;
            float* buffer = (float*)calloc(length * ss.channels, sizeof(float));

            switch (track->instrument) {
                case INSTRUMENT_GUITAR:
                    generate_sine_wave(buffer, length, note);
                    break;
                case INSTRUMENT_BASS_DRUM:
                case INSTRUMENT_SNARE_DRUM:
					generate_drum_sound(buffer, length, note);
					break;
            }

            pan_audio(buffer, length, note->panning);

            if (pa_simple_write(s, buffer, length * sizeof(float) * ss.channels, &error) < 0) {
                fprintf(stderr, "pa_simple_write() failed: %s\n", pa_strerror(error));
                free(buffer);
                pa_simple_free(s);
                return;
            }

            free(buffer);
        }
    }

    if (pa_simple_drain(s, &error) < 0) {
        fprintf(stderr, "pa_simple_drain() failed: %s\n", pa_strerror(error));
    }

    pa_simple_free(s);
}

int main() {
    MusicFile* music = create_complex_song();

    // Save the song to a file
    write_music_file("complex_song.mus", music);

    // Load and play the song
    MusicFile* loaded_music = load_music_file("complex_song.mus");
    play_music(loaded_music);

    // Clean up
    free_music_file(music);
    free_music_file(loaded_music);

    return 0;
}

