#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define FSE_INTERNAL_API
#include <fs/emu/audio.h>
#include <fs/log.h>

static void configure(fse_audio_stream_options **options)
{

}

static int queue_buffer(int stream, int16_t* data, int size)
{
    return 0;
}

static int check_buffer(int stream, int buffer)
{
    return 1;
}

static void set_paused(int stream, bool paused)
{

}

static int output_frequency(void)
{
    return 44100;
}

static void register_functions(void)
{
    fse_audio.configure = configure;
    fse_audio.queue_buffer = queue_buffer;
    fse_audio.check_buffer = check_buffer;
    fse_audio.set_paused = set_paused;
    fse_audio.frequency = output_frequency;
}

void fse_init_dummy_audio(void)
{
    fs_log("[AUDIO] fse_init_dummy_audio\n");
    register_functions();
}
