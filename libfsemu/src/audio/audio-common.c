#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef FS_EMU_DRIVERS

#define FS_EMU_INTERNAL
#include <fs/emu/audio.h>
#include <fs/log.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void (*fs_emu_audio_configure)(fs_emu_audio_stream_options **options) = 0;
int (*fs_emu_audio_queue_buffer)(int stream, int16_t* buffer, int size) = 0;
int (*fs_emu_audio_check_buffer)(int stream, int buffer) = 0;
void (*fs_emu_audio_set_paused)(int stream, bool paused) = 0;
int (*fs_emu_audio_output_frequency)(void) = 0;

double g_fs_emu_audio_want_volume[FS_EMU_AUDIO_MAX_STREAMS];

static int g_volume = 100;
static bool g_muted = false;

int fs_emu_audio_volume(int stream)
{
    assert(stream == FS_EMU_AUDIO_MASTER);
    return g_volume;
}

void fs_emu_audio_set_volume(int stream, int volume)
{
    assert(stream == FS_EMU_AUDIO_MASTER);
    g_volume = volume;
    for (int i = 0; i < FS_EMU_AUDIO_MAX_STREAMS; i++) {
        g_fs_emu_audio_want_volume[i] = g_muted ? 0.0 : g_volume / 100.0;
    }
}

bool fs_emu_audio_muted(int stream)
{
    assert(stream == FS_EMU_AUDIO_MASTER);
    return g_muted || g_volume == 0;
}

void fs_emu_audio_set_muted(int stream, bool muted)
{
    assert(stream == FS_EMU_AUDIO_MASTER);
    g_muted = muted;
    for (int i = 0; i < FS_EMU_AUDIO_MAX_STREAMS; i++) {
        g_fs_emu_audio_want_volume[i] = g_muted ? 0.0 : g_volume / 100.0;
    }
}

fs_emu_audio_stream_options **fs_emu_audio_alloc_stream_options(int streams)
{
    fs_emu_audio_stream_options **options = \
            malloc(sizeof(fs_emu_audio_stream_options *) * (streams + 1));
    for (int i = 0; i < streams; i++) {
        options[i] = (fs_emu_audio_stream_options *) \
                malloc(sizeof(fs_emu_audio_stream_options));
        memset(options[i], 0x00, sizeof(fs_emu_audio_stream_options));

        options[i]->frequency = 44100;
        options[i]->channels = 2;
        options[i]->sample_size = 16;
        //options->buffer_size = 1024;
        options[i]->buffer_size = 512;
        //options->min_buffers = 10;
        options[i]->min_buffers = 1;
    }
    options[streams] = NULL;
    return options;
}

void fs_emu_audio_free_stream_options(fs_emu_audio_stream_options **options)
{
    for (fs_emu_audio_stream_options **o = options; *o; o++) {
        free(*o);
    }
    free(options);
}


#endif /* FS_EMU_DRIVERS */
