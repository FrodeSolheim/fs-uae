#include "fsuae-bridge.h"

#include <SDL3/SDL.h>
#include <stdlib.h>
#include <uae/uae.h>

#include "fsemu-audio.h"
#include "fsuae-events.h"
#include "libuae.h"

static void fsuae_bridge_gui_message(const char *message)
{
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "GUI message: %s", message);
}

static void fsuae_bridge_write_log(const char *message)
{
    SDL_Log("[UAE] %s", message);
}

void fsuae_bridge_configure_libuae(void)
{
    // Register libuae callbacks
    libuae_cb_free = free;
    libuae_cb_gui_message = fsuae_bridge_gui_message;
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

    amiga_set_event_function(fsuae_events_on_line);
}
