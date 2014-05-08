#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef USE_SDL

#include <fs/emu.h>
#include "audio.h"

static void sdl_audio_init() {

}

static void sdl_audio_shutdown() {

}

fs_emu_audio_driver g_fs_emu_audio_sdl_driver = {
    "sdl",
    sdl_audio_init,
    sdl_audio_shutdown,
};

#endif // USE_SDL
