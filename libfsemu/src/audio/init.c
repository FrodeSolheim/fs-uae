#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef FS_EMU_DRIVERS

#define FS_EMU_INTERNAL
#include <fs/emu/audio.h>
#include <fs/log.h>

void fs_emu_audio_init(void)
{
    fs_log("fs_emu_audio_init\n");
    fs_emu_openal_audio_init();
}

#endif /* FS_EMU_DRIVERS */
