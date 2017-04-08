#ifndef FS_EMU_AUDIO_H
#define FS_EMU_AUDIO_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdint.h>
#include <stdbool.h>

#define FS_EMU_AUDIO_MASTER -1
#define FSE_MAX_AUDIO_STREAMS 2

//#ifdef FSE_DRIVERS
#if 1

typedef struct fse_audio_stream_options {
    int struct_size;
    int frequency;
    int channels;
    int sample_size;
    int buffer_size;
    int min_buffers;
} fse_audio_stream_options;

fse_audio_stream_options **fs_emu_audio_alloc_stream_options(int streams);

void fs_emu_audio_free_stream_options(fse_audio_stream_options **options);

void fs_emu_audio_configure(fse_audio_stream_options **options);
int fs_emu_audio_output_frequency(void);

int fse_audio_volume(int stream);
void fse_set_set_audio_volume(int stream, int volume);

bool fse_audio_muted(int stream);
void fse_set_audio_muted(int stream, bool muted);

void fse_set_audio_paused(int stream, bool paused);

int fse_queue_audio_buffer(int stream, int16_t *buffer, int size);
int fse_check_audio_buffer(int stream, int buffer);

#else

int fse_audio_volume(int stream);
void fse_set_set_audio_volume(int stream, int volume);

bool fse_audio_muted(int stream);
void fse_set_audio_muted(int stream, int muted);

typedef struct fse_audio_stream_options {
    int struct_size;
    int frequency;
    int channels;
    int sample_size;
    int buffer_size;
    int min_buffers;
} fse_audio_stream_options;

void fs_emu_init_audio_stream(int stream,
        fse_audio_stream_options *options);
void fs_emu_init_audio_stream_options(fse_audio_stream_options *options);
void fse_set_audio_paused(int stream, bool paused);
int fse_queue_audio_buffer(int stream, int16_t* buffer, int size);
int fse_check_audio_buffer(int stream, int buffer);
int fs_emu_audio_output_frequency(void);

#endif

#if 0
// start deprecated
void fs_emu_enable_audio_stream(int stream);
void fs_emu_disable_audio_stream(int stream);
int fs_emu_get_audio_buffer_size();
void fs_emu_audio_sample(int stream, int16_t left, int16_t right);
void fs_emu_set_max_audio_buffers(int buffers);
void fs_emu_set_audio_buffer_frequency(int stream, int frequency);
// end deprecated
#endif

double fs_emu_audio_get_measured_avg_buffer_fill(int stream);
double fs_emu_audio_get_measured_output_frequency();

#if 1 // def FSE_INTERNAL_API

void fse_init_audio(void);

void fse_init_openal_audio(void);

void fse_init_dummy_audio(void);

typedef struct fse_audio {
    void (*configure)(fse_audio_stream_options **options);
    int (*queue_buffer)(int stream, int16_t *buffer, int size);
    int (*check_buffer)(int stream, int buffer);
    void (*set_paused)(int stream, bool paused);
    int (*frequency)(void);

    double want_volume[FSE_MAX_AUDIO_STREAMS];
} fse_audio_t;

extern fse_audio_t fse_audio;

#endif /* FSE_INTERNAL_API*/

#endif /* FS_EMU_AUDIO_H */
