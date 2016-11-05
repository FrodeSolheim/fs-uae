#ifndef LIBFSEMU_TEXTURE_H_
#define LIBFSEMU_TEXTURE_H_

#include "fs/image.h"

#define TEXTURE_GLOSS 1
#define TEXTURE_GLOW_LEFT 2
#define TEXTURE_GLOW_TOP_LEFT 3
#define TEXTURE_GLOW_TOP 4
#define TEXTURE_SIDEBAR 5
#define TEXTURE_SIDEBAR_EDGE 6
#define TEXTURE_CLOSE 7
#define TEXTURE_VOLUME 8
#define TEXTURE_VOLUME_MUTED 9
#define TEXTURE_ASPECT 10
#define TEXTURE_STRETCH 11
#define TEXTURE_ITEM_BACKGROUND 12
#define TEXTURE_TOP_ITEM_BG 13
#define TEXTURE_HEADING_BG 14
#define TEXTURE_PAUSE 15
#define TEXTURE_LOGO_32 16
#define TEXTURE_LAST 17

typedef struct _fs_emu_texture {
    unsigned int texture;
    unsigned int width;
    unsigned int height;
    fs_image* image;
    int opengl_context_stamp;
} fs_emu_texture;

fs_emu_texture *fs_emu_texture_new_from_file(const char *name);
fs_emu_texture *fs_emu_texture_new_from_data(char *data, int size);
void fs_emu_set_texture(fs_emu_texture* texture);
void fs_emu_texture_render(fs_emu_texture *texture, int x, int y);
void fs_emu_render_texture_with_size(fs_emu_texture *texture, int x, int y,
        int w, int h);

#endif // LIBFSEMU_TEXTURE_H_
