#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/emu.h>
#include "audio.h"

int64_t g_fs_emu_audio_buffer_underrun_time = 0;

#ifdef USE_SDL
extern fs_emu_audio_driver g_fs_emu_audio_sdl_driver;
#endif // USE_SDL

#if 0
void init_audio() {

}
#endif
