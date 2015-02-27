#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef FS_EMU_DRIVERS

#define FS_EMU_INTERNAL
#include <fs/emu/audio.h>
#include <fs/log.h>

static void configure(fs_emu_audio_stream_options **options)
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
    fs_emu_audio_configure = configure;
    fs_emu_audio_queue_buffer = queue_buffer;
    fs_emu_audio_check_buffer = check_buffer;
    fs_emu_audio_set_paused = set_paused;
    fs_emu_audio_output_frequency = output_frequency;
}

void fs_emu_audio_dummy_init(void)
{
    fs_log("fs_emu_audio_dummy_init\n");
    register_functions();
}

#endif /* FS_EMU_DRIVER */
