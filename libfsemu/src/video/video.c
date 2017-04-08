#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define FSE_INTERNAL_API
#include <fs/conf.h>
#include <fs/emu/buffer.h>
#include <fs/emu/input.h>
#include <fs/emu/options.h>
#include <fs/emu/video.h>
#include <fs/log.h>

#include <stdbool.h>
#include <string.h>

fse_video_t fse_video = {};

bool fse_drivers(void)
{
    return fse_video.drivers;
}

void fs_emu_buffer_configure(int width, int height)
{
    fse_video.configure_buffer(width, height);
}

void fs_emu_buffer_finish(fs_emu_buffer *buffer)
{
    fse_video.finish_buffer(buffer);
}

fs_emu_buffer *fs_emu_buffer_get(void)
{
    return fse_video.get_buffer();
}

void fs_emu_video_create_window(int width, int height)
{
    fse_video.create_window(width, height);
}

void fs_emu_video_render(void)
{
    fse_video.render();
}

void fse_init_video(void)
{
    fse_log("[VIDEO] Initialize\n");

    fs_emu_buffer_init();
    fs_emu_video_dummy_init();
    fse_main_loop = fs_emu_main_loop;

    fse_video.drivers = true;

    const char *driver = fs_config_get_const_string(OPTION_VIDEO_DRIVER);
    if (0) {
#ifdef USE_SDL2
    } else if (!driver || strcmp(driver, "legacy") == 0) {
        fse_init_legacy_video();
        fse_main_loop = fs_ml_main_loop;
        fse_video.drivers = false;
    } else if (!driver || strcmp(driver, "sdl") == 0) {
        fs_emu_video_sdl_init();
    } else if (!driver || strcmp(driver, "sdl-software") == 0) {
        fs_emu_video_sdl_software_init();
#endif
    }

    fse_log("[VIDEO] Creating window\n");
    fs_emu_video_create_window(752, 572);
}
