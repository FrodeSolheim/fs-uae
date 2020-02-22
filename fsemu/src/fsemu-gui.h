#ifndef FSEMU_GUI_H_
#define FSEMU_GUI_H_

#include <stdbool.h>
#include <stdint.h>

#include "fsemu-common.h"
#include "fsemu-font.h"
#include "fsemu-image.h"
#include "fsemu-sdl.h"
#include "fsemu-types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct fsemu_gui_item_struct;
typedef struct fsemu_gui_item_struct fsemu_gui_item_t;

enum {
    FSEMU_GUI_NAVIGATE_NONE,
    FSEMU_GUI_NAVIGATE_UP,
    FSEMU_GUI_NAVIGATE_RIGHT,
    FSEMU_GUI_NAVIGATE_DOWN,
    FSEMU_GUI_NAVIGATE_LEFT,
    FSEMU_GUI_NAVIGATE_PRIMARY,
    FSEMU_GUI_NAVIGATE_SECONDARY,
    FSEMU_GUI_NAVIGATE_TERTIARY,
    FSEMU_GUI_NAVIGATE_BACK,
    FSEMU_GUI_NAVIGATE_CLOSE,
};

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

fsemu_gui_item_t *fsemu_gui_snapshot(void);
void fsemu_gui_free_snapshot(fsemu_gui_item_t *snapshot);

#ifdef FSEMU_INTERNAL

#define fsemu_gui_log(format, ...) \
    fsemu_log("[FSEMU] [ GUI ] " format, ##__VA_ARGS__)

// void fsemu_gui_item_hide(fsemu_gui_item_t* item);

#define FSEMU_COORD_REAL 0
#define FSEMU_COORD_1080P 1
#define FSEMU_COORD_1080P_LEFT 2
#define FSEMU_COORD_1080P_RIGHT 3

typedef struct fsemu_gui_coord {
    int anchor;
    double offset;
} fsemu_gui_coord_t;

struct fsemu_gui_item_struct {
    fsemu_rect_t rect;
    int coordinates;
    bool translucent;
    uint32_t color;
    const char *text;
    fsemu_font_t *font;
    fsemu_image_t *image;
    // bool image_owner;
    bool dirty;
    bool visible;
    uint32_t flags;
    int z_index;
#ifdef FSEMU_SDL
    SDL_Texture *texture;
#endif

    fsemu_gui_coord_t left;
    fsemu_gui_coord_t top;
    fsemu_gui_coord_t right;
    fsemu_gui_coord_t bottom;
    fsemu_drect_t render_rect;

    char *name;
    fsemu_gui_item_t *parent;
    fsemu_gui_item_t *next;
    fsemu_gui_item_t *first_child;
    fsemu_gui_item_t *last_child;
};

#endif  // FSEMU_INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_GUI_H_
