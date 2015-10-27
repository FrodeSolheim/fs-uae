#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef FS_EMU_DRIVERS

#define FS_EMU_INTERNAL
#include <fs/emu/video.h>
#include <fs/emu/buffer.h>
#include <fs/emu/input.h>
#include <fs/emu/options.h>
#include <fs/conf.h>
#include <fs/log.h>
#include <string.h>

void fs_emu_video_init(void)
{
    fs_log("fs_emu_video_init\n");

    fs_emu_buffer_init();

    fs_emu_video_dummy_init();

    const char *driver = fs_config_get_const_string(OPTION_VIDEO_DRIVER);
    if (0) {
#ifdef USE_SDL2
    } else if (!driver || strcmp(driver, "sdl") == 0) {
        fs_emu_video_sdl_init();
    } else if (!driver || strcmp(driver, "sdl-software") == 0) {
        fs_emu_video_sdl_software_init();
#endif
    }

    fs_log("creating window\n");
    fs_emu_video_create_window(640, 512);
}

#endif /* FS_EMU_DRIVERS */
