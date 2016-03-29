#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/emu.h>
#include <stdlib.h>
#include <string.h>

#include <fs/emu.h>
#include <fs/ml.h>

#include "libfsemu.h"
#include "video.h"

#define FS_EMU_TEXTURE_FORMAT_RGB 0x1907
#define FS_EMU_TEXTURE_FORMAT_RGB8 0x8051
#define FS_EMU_TEXTURE_FORMAT_RGBA 0x1908
#define FS_EMU_TEXTURE_FORMAT_RGBA8 0x8058
#define FS_EMU_TEXTURE_FORMAT_RGB5 0x8050
#define FS_EMU_TEXTURE_FORMAT_RGB5_A1 0x8057

extern int g_fs_ml_benchmarking;

void fs_emu_video_init_options(void)
{
	//int auto_sync_mode = 1;
    //int sync_to_vblank = 1;
    //int sync_with_emu = 0;
    char *sync_mode_str = NULL;

    int fsaa = fs_config_get_int_clamped("fsaa", 0, 4);
    if (fsaa != FS_CONFIG_NONE) {
        fs_log("setting full-scene anti-aliasing (FSAA) to %dx%d\n",
                fsaa, fsaa);
        fs_ml_set_video_fsaa(fsaa);
    }
    else {
        fs_log("full-scene anti-aliasing is not requested\n");
    }

#if 0
    fs_ml_video_mode mode;
    memset(&mode, 0, sizeof(fs_ml_video_mode));
    if (fs_ml_video_mode_get_current(&mode) == 0) {
        fs_log("current display mode is %dx%d @ %d Hz\n", mode.width,
                mode.height, mode.fps);
        int assume_refresh_rate = fs_config_get_int("assume_refresh_rate");
        if (assume_refresh_rate != FS_CONFIG_NONE) {
            fs_log("assuming host refresh rate: %d Hz (from config)\n",
                    assume_refresh_rate);
            g_fs_emu_video_frame_rate_host = assume_refresh_rate;
        }
        else {
            g_fs_emu_video_frame_rate_host = mode.fps;
        }
    }
    else {
        fs_log("could not get display mode\n");
    }
#endif

    fs_log("checking video sync mode\n");

    sync_mode_str = fs_config_get_string("video_sync");
    if (!sync_mode_str) {
        // compatibility key, FIXME: remove when FS-UAE translates compat
        // option names when loading config.
        sync_mode_str = fs_config_get_string("sync");
    }
    if (sync_mode_str) {
        if (g_ascii_strcasecmp(sync_mode_str, "auto") == 0) {
            g_fs_emu_video_sync_to_vblank = 1;
            g_fs_emu_video_allow_full_sync = 1;
        }
        else if (g_ascii_strcasecmp(sync_mode_str, "1") == 0) {
            // shortcut option, nice from command line (e.g. --video-sync)
            g_fs_emu_video_sync_to_vblank = 1;
            g_fs_emu_video_allow_full_sync = 1;
        }
        else if (g_ascii_strcasecmp(sync_mode_str, "full") == 0) {
            // old compatibility option
            g_fs_emu_video_sync_to_vblank = 1;
            g_fs_emu_video_allow_full_sync = 1;
        }
        else if (g_ascii_strcasecmp(sync_mode_str, "0") == 0) {
            // shortcut option, nice from command line (e.g. --no-video-sync)
            g_fs_emu_video_sync_to_vblank = 0;
            g_fs_emu_video_allow_full_sync = 0;
        }
        else if (g_ascii_strcasecmp(sync_mode_str, "off") == 0) {
            //g_fs_emu_video_sync_to_vblank = 0;
            //g_fs_emu_video_allow_full_sync = 0;
        }
#if 0
        else if (g_ascii_strcasecmp(sync_mode_str, "vblank") == 0) {
            g_fs_emu_video_sync_to_vblank = 1;
        }
#endif
        else {
            fs_log("WARNING: invalid value for video-sync: %s\n",
                    sync_mode_str);
        }
        free(sync_mode_str);
    }
    else {
        //fs_log("not specified: using automatic video sync mode\n");

        fs_log("not specified: no video sync\n");
        //g_fs_emu_video_sync_to_vblank = 0;
        //g_fs_emu_video_allow_full_sync = 0;
    }

/*
    if (auto_sync_mode) {
        fs_log("auto sync mode is enabled\n");
        if (frame_rate && frame_rate == mode.fps) {
            fs_log("frame rate (%d) equals screen refresh (%d)\n",
                    frame_rate, mode.fps);
            sync_to_vblank = 1;
            sync_with_emu = 1;
        }
        else {
            fs_log("frame rate (%d) does not equal screen refresh (%d)\n",
                    frame_rate, mode.fps);
            sync_to_vblank = 1;
        }
    }
*/

    if (fs_emu_netplay_enabled()) {
        fs_log("netplay is enabled, disabling full video/emulator sync\n");
        g_fs_emu_video_allow_full_sync = 0;
        //sync_with_emu = 0;
        //sync_to_vblank = 0;
    }

    if (fs_config_get_boolean("benchmark") != FS_CONFIG_NONE) {
        fs_log("benchmarking enable, disabling video sync\n");
        g_fs_emu_video_sync_to_vblank = 0;
        //sync_with_emu = 0;
        g_fs_emu_benchmarking = 1;
        g_fs_ml_benchmarking = 1;

        g_fs_emu_video_allow_full_sync = 0;
    }

    //if (sync_with_emu && !g_fs_emu_full_sync_allowed) {

    // FIXME: check where g_fs_emu_full_sync_allowed is used
    if (!g_fs_emu_full_sync_allowed) {
        fs_log("full video/emu sync is not allowed in this mode\n");
        //sync_with_emu = 0;
        g_fs_emu_video_allow_full_sync = 0;
    }

    /*
    if (sync_with_emu) {
        fs_log("will sync emulation and video with vblank\n");
        fs_ml_video_sync_enable();

        if (frame_rate && mode.fps) {
            double pitch = (1.0 * mode.fps) / frame_rate;
            fs_log("changing audio pitch to %0.2f based on frame rates\n",
                    pitch);
            fs_emu_audio_set_default_pitch(pitch);
        }
    }
    else if (sync_to_vblank) {
        fs_log("will sync video output only to vblank (no tearing)\n");
        fs_ml_vblank_sync_enable();
    }
    else {
        fs_log("no video sync\n");
    }
    */

    if (g_fs_emu_video_sync_to_vblank) {
        fs_log("will sync video updates to vblank\n");
        fs_ml_vblank_sync_enable();
    }
    else {
        fs_log("no video sync (using timers only)\n");
    }

    if (fs_config_get_boolean("disable_aspect_correction") == 1) {
        g_fs_emu_disable_aspect_correction = 1;
    }
    else if (fs_config_get_boolean("keep_aspect") == 1) {
        fs_emu_video_set_aspect_correction(1);
    }

    // the default texture format is RGB, set here because some video
    // formats implicitly changes the default texture format
    g_fs_emu_texture_format = FS_EMU_TEXTURE_FORMAT_RGB;

    const char *s = fs_config_get_const_string("video_format");
    if (s) {
        if (g_ascii_strcasecmp(s, "bgra") == 0) {
            fs_log("using video format BGRA\n");
            g_fs_emu_video_format = FS_EMU_VIDEO_FORMAT_BGRA;
            g_fs_emu_video_bpp = 4;
        }
        else if (g_ascii_strcasecmp(s, "rgba") == 0) {
            fs_log("using video format RGBA\n");
            g_fs_emu_video_format = FS_EMU_VIDEO_FORMAT_RGBA;
            g_fs_emu_video_bpp = 4;
        }
        else if (g_ascii_strcasecmp(s, "rgb") == 0) {
            fs_log("using video format RGB\n");
            g_fs_emu_video_format = FS_EMU_VIDEO_FORMAT_RGB;
            g_fs_emu_video_bpp = 3;
        }
        else if (g_ascii_strcasecmp(s, "rgb565") == 0) {
            fs_log("using video format RGB565\n");
            g_fs_emu_video_format = FS_EMU_VIDEO_FORMAT_R5G6B5;
            g_fs_emu_video_bpp = 2;
            g_fs_emu_texture_format = FS_EMU_TEXTURE_FORMAT_RGB5;
        }
        else if (g_ascii_strcasecmp(s, "rgba5551") == 0) {
            fs_log("using video format RGBA5551\n");
            g_fs_emu_video_format = FS_EMU_VIDEO_FORMAT_R5G5B5A1;
            g_fs_emu_video_bpp = 2;
            g_fs_emu_texture_format = FS_EMU_TEXTURE_FORMAT_RGB5_A1;
        }
        else {
            fs_emu_warning("Unknown video format");
        }
    }
    if (!g_fs_emu_video_format) {
        fs_log("using default video format BGRA\n");
        g_fs_emu_video_format = FS_EMU_VIDEO_FORMAT_BGRA;
        g_fs_emu_video_bpp = 4;
    }

    s = fs_config_get_const_string("texture_format");
    if (s) {
        if (g_ascii_strcasecmp(s, "rgb") == 0) {
            fs_log("using texture format RGB\n");
            g_fs_emu_texture_format = FS_EMU_TEXTURE_FORMAT_RGB;
        }
        else if (g_ascii_strcasecmp(s, "rgb8") == 0) {
            fs_log("using texture format RGB8\n");
            g_fs_emu_texture_format = FS_EMU_TEXTURE_FORMAT_RGB8;
        }
        else if (g_ascii_strcasecmp(s, "rgba") == 0) {
            fs_log("using texture format RGBA\n");
            g_fs_emu_texture_format = FS_EMU_TEXTURE_FORMAT_RGBA;
        }
        else if (g_ascii_strcasecmp(s, "rgba8") == 0) {
            fs_log("using texture format RGBA8\n");
            g_fs_emu_texture_format = FS_EMU_TEXTURE_FORMAT_RGBA8;
        }
        else if (g_ascii_strcasecmp(s, "rgb5") == 0) {
            fs_log("using texture format RGB5\n");
            g_fs_emu_texture_format = FS_EMU_TEXTURE_FORMAT_RGB5;
        }
        else if (g_ascii_strcasecmp(s, "rgb5_a1") == 0) {
            fs_log("using texture format RGB5_A1\n");
            g_fs_emu_texture_format = FS_EMU_TEXTURE_FORMAT_RGB5_A1;
        }
        else {
            fs_emu_warning("Unknown texture format (using default)");
        }
    }
    else {
        fs_log("using default texture format\n");
    }

    double dval;

    if (fs_config_get_boolean("scanlines") == 1) {
        g_fs_emu_scanlines = 1;
    }
    dval = fs_config_get_double_clamped("scanlines_light", 0, 100);
    if (dval != FS_CONFIG_NONE) {
        g_fs_emu_scanlines_light = 255.0 * dval / 100.0;
    }
    dval = fs_config_get_double_clamped("scanlines_dark", 0, 100);
    if (dval != FS_CONFIG_NONE) {
        g_fs_emu_scanlines_dark = 255.0 * dval / 100.0;
    }
}
