#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define FSE_INTERNAL_API
#include <fs/emu/audio.h>
#include <fs/emu/options.h>
#include <fs/conf.h>
#include <fs/log.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

fse_audio_t fse_audio = {};
static int g_volume = 100;
static bool g_muted = false;
int64_t g_fs_emu_audio_buffer_underrun_time = 0;

int fse_audio_volume(int stream)
{
    assert(stream == FS_EMU_AUDIO_MASTER);
    return g_volume;
}

void fse_set_set_audio_volume(int stream, int volume)
{
    assert(stream == FS_EMU_AUDIO_MASTER);
    g_volume = volume;
    for (int i = 0; i < FSE_MAX_AUDIO_STREAMS; i++) {
        fse_audio.want_volume[i] = g_muted ? 0.0 : g_volume / 100.0;
    }
}

bool fse_audio_muted(int stream)
{
    assert(stream == FS_EMU_AUDIO_MASTER);
    return g_muted || g_volume == 0;
}

void fse_set_audio_muted(int stream, bool muted)
{
    assert(stream == FS_EMU_AUDIO_MASTER);
    g_muted = muted;
    for (int i = 0; i < FSE_MAX_AUDIO_STREAMS; i++) {
        fse_audio.want_volume[i] = g_muted ? 0.0 : g_volume / 100.0;
    }
}

void fse_set_audio_paused(int stream, bool paused)
{
    fse_audio.set_paused(stream, paused);
}

int fse_queue_audio_buffer(int stream, int16_t *buffer, int size)
{
    return fse_audio.queue_buffer(stream, buffer, size);
}

int fse_check_audio_buffer(int stream, int buffer)
{
    return fse_audio.check_buffer(stream, buffer);
}

void fs_emu_audio_configure(fse_audio_stream_options **options)
{
    fse_audio.configure(options);
}

int fs_emu_audio_output_frequency(void)
{
    return fse_audio.frequency();
}


fse_audio_stream_options **fs_emu_audio_alloc_stream_options(int streams)
{
    fse_audio_stream_options **options = \
            malloc(sizeof(fse_audio_stream_options *) * (streams + 1));
    for (int i = 0; i < streams; i++) {
        options[i] = (fse_audio_stream_options *) \
                malloc(sizeof(fse_audio_stream_options));
        memset(options[i], 0x00, sizeof(fse_audio_stream_options));

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

void fs_emu_audio_free_stream_options(fse_audio_stream_options **options)
{
    for (fse_audio_stream_options **o = options; *o; o++) {
        free(*o);
    }
    free(options);
}

void fse_init_audio(void)
{
    fs_log("[AUDIO] Initialize\n");

    const char *driver = fs_config_get_const_string(OPTION_AUDIO_DRIVER);
    if (0) {
#ifdef WITH_OPENAL
    } else if (!driver || strcmp(driver, "openal") == 0) {
        fse_init_openal_audio();
#endif
    } else {
        fse_init_dummy_audio();
    }

    for (int i = 0; i < FSE_MAX_AUDIO_STREAMS; i++) {
        fse_audio.want_volume[i] = 1.0;
    }
    int volume = fs_config_get_int_clamped("volume", 0, 100);
    if (volume != FS_CONFIG_NONE) {
        if (volume == 0) {
            fse_set_audio_muted(FS_EMU_AUDIO_MASTER, 1);
        } else {
            fse_set_set_audio_volume(FS_EMU_AUDIO_MASTER, volume);
        }
    }
}
