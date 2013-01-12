#ifndef LIBFSEMU_THEME_H_
#define LIBFSEMU_THEME_H_

#include "texture.h"

#define MAX_CUSTOM_OVERLAYS 20

struct fs_emu_theme {
    char *name;
    char *path;
    float wall_color_1[4];
    float wall_color_2[4];
    float floor_color_1[4];
    float floor_color_2[4];
    int floor_height;
    char *overlay_image;
    float fade_color[4];
    float heading_color[4];
    float item_color[4];

    fs_emu_texture *overlay_textures[MAX_CUSTOM_OVERLAYS];
    int overlay_x[MAX_CUSTOM_OVERLAYS];
    int overlay_y[MAX_CUSTOM_OVERLAYS];
};

void fs_emu_theme_init();
char *fs_emu_theme_get_resource(const char *name);

extern struct fs_emu_theme g_fs_emu_theme;

#endif // LIBFSEMU_THEME_H_
