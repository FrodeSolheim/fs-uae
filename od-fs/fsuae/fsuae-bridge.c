#include "fsuae-bridge.h"
#include "fsemu-audio.h"
#include "fslib-data.h"
#include "fsuae-events.h"

#include "libuae.h"
#include <uae/uae.h>

#include <stdlib.h>

static void fsuae_bridge_gui_message(const char* message)
{
    printf(
        ",--------------------------------------------------------------------"
        "-----------\n"
    );
    printf("| GUI MSG: %s\n", message);
}

static void fsuae_bridge_write_log(const char* message)
{
    printf("[UAE] %s\n", message);
}

void fsuae_bridge_configure_libuae(void)
{
    // Register libuae callbacks
    libuae_cb_free = free;
    libuae_cb_gui_message = fsuae_bridge_gui_message;
    libuae_cb_load_data = fslib_data_load;
    libuae_cb_write_log = fsuae_bridge_write_log;

    amiga_init();

    // libuae_set_plugin_dir();

    amiga_set_audio_buffer_size(128);
    amiga_set_audio_frequency(fsemu_audio_frequency());
    // amiga_set_audio_callback(audio_callback_function);
    // amiga_set_cd_audio_callback(audio_callback_function);

    // amiga_set_video_format(AMIGA_VIDEO_FORMAT_RGBA);

    // FIXME: Check if this is the most efficient format?
    amiga_set_video_format(AMIGA_VIDEO_FORMAT_BGRA);

    amiga_set_event_function(fsuae_events_event_handler);
}
