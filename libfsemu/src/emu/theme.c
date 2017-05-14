#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/emu.h>
#include "theme.h"

#include <fs/conf.h>
#include <fs/base.h>
#include <fs/data.h>
#include <fs/filesys.h>
#include <fs/i18n.h>
#include <fs/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "texture.h"
#include "libfsemu.h"

struct fs_emu_theme g_fs_emu_theme = {};

#ifdef WITH_LUA

#include "emu_lua.h"

void fs_emu_theme_init_lua(void)
{
    fs_log("fs_emu_theme_init_lua\n");

    char *path = g_build_filename(g_fs_emu_theme.path, "theme.lua", NULL);
    if (fs_path_exists(path)) {
        int result = luaL_dofile(fs_emu_lua_state, path);
        if (result != 0) {
            fs_emu_warning("Error loading/running theme.lua");
            fs_emu_lua_log_error("Error loading/running theme.lua");
        }
    }
    free(path);
}

#endif

void fs_emu_init_overlays(const char **overlay_names)
{
    int k = FS_EMU_FIRST_CUSTOM_OVERLAY;
    const char **name = overlay_names;
    while(*name) {
        if (k < FS_EMU_MAX_OVERLAYS) {
            g_fs_emu_theme.overlays[k].name = g_strdup(*name);
        }
        k = k + 1;
        name++;
    }
}

char *fs_emu_theme_get_resource_path(const char *name)
{
    printf("WARNING: fs_emu_theme_get_resource_path (%s) is deprecated\n",
        name);
    if (fs_path_exists(name)) {
        return g_strdup(name);
    }
    char *path, *p;

    p = g_build_filename(g_fs_emu_theme.path, name, NULL);
    if (fs_path_exists(p)) {
        return p;
    }
    free(p);

    p = g_build_filename(g_fs_emu_theme.name, name, NULL);
    path = fs_get_program_data_file(p);
    free(p);
    if (path) {
        return path;
    }
    //p = g_build_filename("default", name, NULL);
    path = fs_get_program_data_file(name);
    //free(p);
    if (path) {
        return path;
    }
    return NULL;
}

int fs_emu_theme_get_resource_data(const char *name, char **data, int *size)
{
    char *p;
    p = g_build_filename(g_fs_emu_theme.path, name, NULL);
    if (fs_path_exists(p)) {
        FILE *f = g_fopen(p, "rb");
        free(p);
        if (fseek(f, 0, SEEK_END) != 0) {
            fclose(f);
            return 1;
        }
        *size = ftell(f);
        if (*size < 1) {
            fclose(f);
            return 2;
        }
        if (fseek(f, 0, SEEK_SET) != 0) {
            fclose(f);
            return 1;
        }
        *data = malloc(*size);
        if (fread(*data, *size, 1, f) != 1) {
            free(*data);
            fclose(f);
            return 3;
        }
        fclose(f);
        return 0;
    } else {
        free(p);
    }

    p = g_build_filename(g_fs_emu_theme.name, name, NULL);
    int error = fs_get_program_data(p, data, size);
    free(p);
    if (error == 0) {
        return 0;
    }

    error = fs_get_program_data(name, data, size);
    // printf("fs_get_program_data error %d\n", error);
    return error;
}

static void set_color(float *c, float r, float g, float b, float a)
{
    c[0] = r;
    c[1] = g;
    c[2] = b;
    c[3] = a;
}

static void set_color_component(float *c, const char *s)
{
    int val = 0;
    if (s[0] >= '0' && s[0] <= '9') {
        val = val + (s[0] - '0') * 16;
    }
    else if (s[0] >= 'a' && s[0] <= 'f') {
        val = val + (10 + s[0] - 'a') * 16;
    }
    else if (s[0] >= 'A' && s[0] <= 'F') {
        val = val + (10 + s[0] - 'A') * 16;
    }
    if (s[1] >= '0' && s[1] <= '9') {
        val = val + (s[1] - '0');
    }
    else if (s[1] >= 'a' && s[1] <= 'f') {
        val = val + (10 + s[1] - 'a');
    }
    else if (s[1] >= 'A' && s[1] <= 'F') {
        val = val + (10 + s[1] - 'A');
    }
    *c = val / 255.0;
}

static void set_color_from_string(float *c, const char *s)
{
    //printf("set_color_from_string %s\n", s);
    if (!s) {
        return;
    }
    int len = strlen(s);
    if ((len != 7 && len != 9) || s[0] != '#') {
        fs_log("invalid color: %s\n", s);
        return;
    }
    set_color_component(c + 0, s + 1);
    set_color_component(c + 1, s + 3);
    set_color_component(c + 2, s + 5);
    if (len == 9) {
        set_color_component(c + 3, s + 7);
        // pre-multipled alpha
        c[0] = c[0] * c[3];
        c[1] = c[1] * c[3];
        c[2] = c[2] * c[3];
    }
    else {
        c[3] = 1.0;
    }
}

static void load_defaults()
{
    // this determines the coordinate system for overlays, etc
    g_fs_emu_theme.width = 1920;
    g_fs_emu_theme.height = 1080;

    g_fs_emu_theme.display_x = 0;
    g_fs_emu_theme.display_y = 0;
    g_fs_emu_theme.display_w = g_fs_emu_theme.width;
    g_fs_emu_theme.display_h = g_fs_emu_theme.height;

    g_fs_emu_theme.floor_height = 361;

    set_color(g_fs_emu_theme.floor_color_1, 20.0 / 255.0, 22.0 / 255.0,
            26.0 / 255.0, 1.0);
    set_color(g_fs_emu_theme.floor_color_2, 0.0, 0.0, 0.0, 1.0);

    set_color(g_fs_emu_theme.wall_color_1, 0.0, 0.0, 0.0, 1.0);
    set_color(g_fs_emu_theme.wall_color_2, 39.0 / 255.0, 44.0 / 255.0,
            51.0 / 255.0, 1.0);
    //set_color(g_fs_emu_theme.wall_color_2, 239.0 / 255.0, 44.0 / 255.0,
    //      51.0 / 255.0, 1.0);

    set_color(g_fs_emu_theme.heading_color, 0.0, 1.0 * 0x99 / 0xff,
            1.0 * 0xcc / 0xff, 1.0);
    set_color(g_fs_emu_theme.item_color, 1.0, 1.0, 1.0, 1.0);
    set_color(g_fs_emu_theme.fade_color, 0.0, 0.0, 0.0, 1.0);
    g_fs_emu_theme.overlay_image = g_strdup("overlay.png");
}

static void load_theme(void)
{
    fs_log("THEME: Loading \"%s\"\n", g_fs_emu_theme.path);
    char *p = g_build_filename(g_fs_emu_theme.path, "theme.conf", NULL);
    if (fs_path_exists(p)) {
        fs_config_read_file(p, 1);
    }
    free(p);

    char *cv;
    int iv;
    const char *csval;
    char *name;
    int val;

    iv = fs_config_get_int("theme_width");
    if (iv != FS_CONFIG_NONE && iv > 0) {
        g_fs_emu_theme.width = iv;
    }
    iv = fs_config_get_int("theme_height");
    if (iv != FS_CONFIG_NONE && iv > 0) {
        g_fs_emu_theme.height = iv;
    }

    csval = fs_config_get_const_string("theme_display_rect");
    if (csval) {
        int x = -1, y = -1, w = -1, h = -1;
        if (sscanf(csval, "%d,%d,%d,%d", &x, &y, &w, &h) == 4) {
            g_fs_emu_theme.display_x = x;
            g_fs_emu_theme.display_y = y;
            g_fs_emu_theme.display_w = w;
            g_fs_emu_theme.display_h = h;
        }
    }

    set_color_from_string(g_fs_emu_theme.floor_color_1,
            fs_config_get_const_string("theme_floor_color_1"));
    set_color_from_string(g_fs_emu_theme.floor_color_2,
            fs_config_get_const_string("theme_floor_color_2"));
    set_color_from_string(g_fs_emu_theme.wall_color_1,
            fs_config_get_const_string("theme_wall_color_1"));
    set_color_from_string(g_fs_emu_theme.wall_color_2,
            fs_config_get_const_string("theme_wall_color_2"));

    iv = fs_config_get_int("theme_floor_height");
    if (iv != FS_CONFIG_NONE) {
        g_fs_emu_theme.floor_height = iv;
    }
    cv = fs_config_get_string("theme_overlay_image");
    if (cv) {
        free(g_fs_emu_theme.overlay_image);
        g_fs_emu_theme.overlay_image = cv;
    }
    set_color_from_string(g_fs_emu_theme.fade_color,
            fs_config_get_const_string("theme_fade_color"));
    set_color_from_string(g_fs_emu_theme.heading_color,
            fs_config_get_const_string("theme_heading_color"));
    set_color_from_string(g_fs_emu_theme.item_color,
            fs_config_get_const_string("theme_item_color"));

    for (int i = 0; i < FS_EMU_MAX_OVERLAYS; i++) {
        // the first options read here are old compatibility options

        name = g_strdup_printf("theme_custom_%d_x",
                i - FS_EMU_FIRST_CUSTOM_OVERLAY);
        val = fs_config_get_int(name);
        free(name);
        if (val != FS_CONFIG_NONE) {
            //printf("x is %d\n", val);
            g_fs_emu_theme.overlays[i].x = (double) val /
                    g_fs_emu_theme.width;
        }
        name = g_strdup_printf("theme_custom_%d_y",
                i - FS_EMU_FIRST_CUSTOM_OVERLAY);
        val = fs_config_get_int(name);
        free(name);
        if (val != FS_CONFIG_NONE) {
            g_fs_emu_theme.overlays[i].y = (double) val /
                    g_fs_emu_theme.height;
        }

        if (!g_fs_emu_theme.overlays[i].name) {
            continue;
        }

        // these are new theme / overlay options

        name = g_strdup_printf("theme_%s_pos",
                g_fs_emu_theme.overlays[i].name);
        csval = fs_config_get_const_string(name);
        free(name);
        if (csval) {
            int x, y;
            if (sscanf(csval, "%d,%d", &x, &y) == 2) {
                g_fs_emu_theme.overlays[i].x = (double) x /
                        g_fs_emu_theme.width;
                g_fs_emu_theme.overlays[i].y = (double) y /
                        g_fs_emu_theme.height;
            }
        }

        name = g_strdup_printf("theme_%s_prefix",
                g_fs_emu_theme.overlays[i].name);
        cv = fs_config_get_string(name);
        free(name);
        if (cv) {
            // free(g_fs_emu_theme.overlays[i].name);
            g_fs_emu_theme.overlays[i].name = cv;
        }
    }

#ifdef WITH_LUA
    fs_emu_theme_init_lua();
#endif
}

void fse_init_theme()
{
    fs_log("THEME: Init\n");

    fs_emu_theme_overlay* o = g_fs_emu_theme.overlays;
    o[FS_EMU_TOP_LEFT_OVERLAY].name = g_strdup("top_left_overlay");
    o[FS_EMU_TOP_RIGHT_OVERLAY].name = g_strdup("top_right_overlay");
    o[FS_EMU_TOP_RIGHT_OVERLAY].anchor = FS_EMU_ANCHOR_TOP_RIGHT;
    o[FS_EMU_BOTTOM_RIGHT_OVERLAY].name = g_strdup("bottom_right_overlay");
    o[FS_EMU_BOTTOM_RIGHT_OVERLAY].anchor = FS_EMU_ANCHOR_BOTTOM_RIGHT;
    o[FS_EMU_BOTTOM_LEFT_OVERLAY].name = g_strdup("bottom_left_overlay");
    o[FS_EMU_BOTTOM_LEFT_OVERLAY].anchor = FS_EMU_ANCHOR_BOTTOM_LEFT;

    o[FS_EMU_AUDIO_LED_OVERLAY].name = g_strdup("audio_led");
    o[FS_EMU_FPS_LED_OVERLAY].name = g_strdup("fps_led");
    o[FS_EMU_VSYNC_LED_OVERLAY].name = g_strdup("vsync_led");
    o[FS_EMU_FPS_D0_OVERLAY].name = g_strdup("fps_d0");
    o[FS_EMU_FPS_D1_OVERLAY].name = g_strdup("fps_d1");

    const char *theme = fs_config_get_const_string("theme");
    if (theme) {
        g_fs_emu_theme.name = g_strdup(theme);
        // first try to find the theme in the user's theme dir
        const char *themes_dir = fs_config_get_const_string("themes_dir");
        if (themes_dir) {
            g_fs_emu_theme.path = g_build_filename(themes_dir,
                    g_fs_emu_theme.name, NULL);
            if (!fs_path_exists(g_fs_emu_theme.path)) {
                free(g_fs_emu_theme.path);
                g_fs_emu_theme.path = NULL;
            }
        }
        // or by direct path lookup
        if (!g_fs_emu_theme.path) {
            if (fs_path_exists(theme)) {
                g_fs_emu_theme.path = g_strdup(theme);
            }
        }
        // then try to find a bundled / installed theme
        if (!g_fs_emu_theme.path) {
            g_fs_emu_theme.path = fs_get_program_data_file(
                    g_fs_emu_theme.name);
        }
        if (g_fs_emu_theme.path) {
            fs_log("theme found at %s\n", g_fs_emu_theme.path);
        } else {
            fs_emu_warning(_("Theme not found: %s"), g_fs_emu_theme.name);
            free(g_fs_emu_theme.name);
            // resources will not be found, but path should not be NULL...
            g_fs_emu_theme.path = g_strdup("");
        }
    } else {
        g_fs_emu_theme.name = g_strdup("");
        g_fs_emu_theme.path = g_strdup("");
    }

    load_defaults();
    load_theme();
}
