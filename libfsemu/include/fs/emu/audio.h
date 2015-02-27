#ifndef FS_EMU_AUDIO_H
#define FS_EMU_AUDIO_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdint.h>
#include <stdbool.h>

#define FS_EMU_AUDIO_MASTER -1
#define FS_EMU_AUDIO_MAX_STREAMS 2

#ifdef FS_EMU_DRIVERS

typedef struct fs_emu_audio_stream_options {
    int struct_size;
    int frequency;
    int channels;
    int sample_size;
    int buffer_size;
    int min_buffers;
} fs_emu_audio_stream_options;

fs_emu_audio_stream_options **fs_emu_audio_alloc_stream_options(int streams);

void fs_emu_audio_free_stream_options(fs_emu_audio_stream_options **options);

int fs_emu_audio_volume(int stream);

void fs_emu_audio_set_volume(int stream, int volume);

bool fs_emu_audio_muted(int stream);

void fs_emu_audio_set_muted(int stream, bool muted);

extern void (*fs_emu_audio_configure)(fs_emu_audio_stream_options **options);

extern int (*fs_emu_audio_queue_buffer)(int stream, int16_t* buffer, int size);

extern int (*fs_emu_audio_check_buffer)(int stream, int buffer);

extern void (*fs_emu_audio_set_paused)(int stream, bool paused);

extern int (*fs_emu_audio_output_frequency)(void);

#else

int fs_emu_audio_volume(int stream);
void fs_emu_audio_set_volume(int stream, int volume);

bool fs_emu_audio_muted(int stream);
void fs_emu_audio_set_muted(int stream, int muted);

typedef struct fs_emu_audio_stream_options {
    int struct_size;
    int frequency;
    int channels;
    int sample_size;
    int buffer_size;
    int min_buffers;
} fs_emu_audio_stream_options;

void fs_emu_init_audio_stream(int stream,
        fs_emu_audio_stream_options *options);
void fs_emu_init_audio_stream_options(fs_emu_audio_stream_options *options);
void fs_emu_audio_set_paused(int stream, bool paused);
int fs_emu_audio_queue_buffer(int stream, int16_t* buffer, int size);
int fs_emu_audio_check_buffer(int stream, int buffer);
int fs_emu_audio_output_frequency();

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

#ifdef FS_EMU_INTERNAL

void fs_emu_audio_init(void);

void fs_emu_audio_openal_init(void);

void fs_emu_audio_dummy_init(void);

extern double g_fs_emu_audio_want_volume[FS_EMU_AUDIO_MAX_STREAMS];

#endif /* FS_EMU_INTERNAL*/

#endif /* FS_EMU_AUDIO_H */
