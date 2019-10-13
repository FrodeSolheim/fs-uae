#ifndef FSEMU_GUI_H_
#define FSEMU_GUI_H_

#include "fsemu-common.h"
#include "fsemu-font.h"
#include "fsemu-image.h"
#include "fsemu-sdl.h"
#include "fsemu-types.h"

#include <stdbool.h>
#include <stdint.h>

struct fsemu_gui_item_struct;
typedef struct fsemu_gui_item_struct fsemu_gui_item_t;

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_gui_init(void);
void fsemu_gui_lock(void);
void fsemu_gui_unlock(void);

fsemu_gui_item_t *fsemu_gui_create_item(void);
// fsemu_gui_item_t *fsemu_gui_create_rectangle(
//     int x, int y, int w, int h, int c);

void fsemu_gui_item_set_visible(fsemu_gui_item_t *item, bool visible);

void fsemu_gui_rectangle(
    fsemu_gui_item_t *item, int x, int y, int w, int h, int c);
void fsemu_gui_image(
    fsemu_gui_item_t *item, int x, int y, int w, int h, fsemu_image_t *image);

void fsemu_gui_add_item(fsemu_gui_item_t *item);

fsemu_gui_item_t *fsemu_gui_snapshot();
void fsemu_gui_free_snapshot(fsemu_gui_item_t *snapshot);

#ifdef FSEMU_INTERNAL

#define fsemu_gui_log(format, ...) \
    fsemu_log("[FSEMU] [ GUI ] " format, ##__VA_ARGS__)

// void fsemu_gui_item_hide(fsemu_gui_item_t* item);

#define FSEMU_COORD_REAL 0
#define FSEMU_COORD_1080P 1
#define FSEMU_COORD_1080P_LEFT 2
#define FSEMU_COORD_1080P_RIGHT 3

#define FSEMU_GUI_FLAG_FLIP_X (1 << 0)

typedef struct fsemu_gui_item_struct {
    fsemu_rect_t rect;
    int coordinates;
    bool translucent;
    uint32_t color;
    const char *text;
    fsemu_font_t *font;
    fsemu_image_t *image;
    bool dirty;
    bool visible;
    uint32_t flags;
    fsemu_gui_item_t *next;
#ifdef FSEMU_SDL
    SDL_Texture *texture;
#endif
} fsemu_gui_item_t;

#endif  // FSEMU_INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_GUI_H_
