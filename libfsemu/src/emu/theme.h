#ifndef LIBFSEMU_THEME_H_
#define LIBFSEMU_THEME_H_

#include "texture.h"

#define FS_EMU_MAX_OVERLAYS 32
#define FS_EMU_MAX_OVERLAY_STATES 5

enum {
    FS_EMU_TOP_LEFT_OVERLAY,
    FS_EMU_TOP_RIGHT_OVERLAY,
    FS_EMU_BOTTOM_RIGHT_OVERLAY,
    FS_EMU_BOTTOM_LEFT_OVERLAY,
    FS_EMU_VSYNC_LED_OVERLAY,
    FS_EMU_FPS_LED_OVERLAY,
    FS_EMU_AUDIO_LED_OVERLAY,
    FS_EMU_FIRST_CUSTOM_OVERLAY,
};

#define FS_EMU_ANCHOR_RIGHT_BIT 1
#define FS_EMU_ANCHOR_BOTTOM_BIT 2

#define FS_EMU_ANCHOR_TOP_LEFT 0
#define FS_EMU_ANCHOR_TOP_RIGHT (FS_EMU_ANCHOR_RIGHT_BIT)
#define FS_EMU_ANCHOR_BOTTOM_RIGHT (FS_EMU_ANCHOR_BOTTOM_BIT | \
        FS_EMU_ANCHOR_RIGHT_BIT)
#define FS_EMU_ANCHOR_BOTTOM_LEFT (FS_EMU_ANCHOR_BOTTOM_BIT)

typedef struct fs_emu_theme_overlay {
    char *name;
    fs_emu_texture *textures[FS_EMU_MAX_OVERLAY_STATES];
    float x;
    float y;
    float w;
    float h;
    int anchor;
} fs_emu_theme_overlay;

struct fs_emu_theme {
    char *name;
    int width;
    int height;
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

    fs_emu_theme_overlay overlays[FS_EMU_MAX_OVERLAYS];
};

void fs_emu_theme_init();
char *fs_emu_theme_get_resource(const char *name);
void fs_emu_set_overlay_state(int overlay, int state);

extern struct fs_emu_theme g_fs_emu_theme;

#endif // LIBFSEMU_THEME_H_
