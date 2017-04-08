#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define FSE_INTERNAL_API
#include <fs/emu/video.h>
#include <fs/emu/buffer.h>
#include <fs/log.h>
#include "../emu/video.h"
#include "../ml/ml_internal.h"

#include <stdlib.h>

static void legacy_video_create_window(int width, int height)
{
    char *title;
    if (fs_emu_get_title()) {
        title = g_strdup(fs_emu_get_title());
    }
    else {
        title = g_strdup("Emulator");
    }
    if (fs_emu_get_sub_title() && fs_emu_get_sub_title()[0] != '\0') {
        char *temp = title;
        // using 'MIDDLE DOT' (U+00B7) in UTF-8 format as separator
        title = g_strdup_printf("%s %c%c %s", temp, 0xC2, 0xB7,
                fs_emu_get_sub_title());
        free(temp);
    }
    fs_ml_video_create_window(title);
    free(title);
}

static void register_functions(void)
{
    fse_video.create_window = legacy_video_create_window;
}

void fse_init_legacy_video(void)
{
    fs_log("[VIDEO] Initialize legacy video\n");
    register_functions();

    fse_init_video_2();

    fs_ml_video_set_update_function(fs_emu_video_update_function);
    fs_ml_video_set_render_function(fs_emu_video_render_function);
    fs_ml_video_set_post_render_function(fs_emu_video_after_update);

    fs_ml_video_init();
}
