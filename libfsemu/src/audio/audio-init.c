#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef FS_EMU_DRIVERS

#define FS_EMU_INTERNAL
#include <fs/emu/audio.h>
#include <fs/emu/options.h>
#include <fs/conf.h>
#include <fs/log.h>
#include <string.h>

void fs_emu_audio_init(void)
{
    fs_log("fs_emu_audio_init\n");

    for (int i = 0; i < FS_EMU_AUDIO_MAX_STREAMS; i++) {
        g_fs_emu_audio_want_volume[i] = 1.0;
    }

    const char *driver = fs_config_get_const_string(OPTION_AUDIO_DRIVER);

    if (0) {
#ifdef USE_OPENAL
    } else if (!driver || strcmp(driver, "openal") == 0) {
        fs_emu_audio_openal_init();
#endif
    } else {
        fs_emu_audio_dummy_init();
    }

    int volume = fs_config_get_int_clamped("volume", 0, 100);
    if (volume != FS_CONFIG_NONE) {
        if (volume == 0) {
            fs_emu_audio_set_muted(FS_EMU_AUDIO_MASTER, 1);
        } else {
            fs_emu_audio_set_volume(FS_EMU_AUDIO_MASTER, volume);
        }
    }
}

#endif /* FS_EMU_DRIVERS */
