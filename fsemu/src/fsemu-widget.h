#ifndef FSEMU_WIDGET_H_
#define FSEMU_WIDGET_H_

#include <stdbool.h>

#include "fsemu-color.h"
#include "fsemu-common.h"
#include "fsemu-config.h"
#include "fsemu-gui.h"
#include "fsemu-image.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FSEMU_WIDGET_REAL 0
#define FSEMU_WIDGET_1080 1
#define FSEMU_WIDGET_1080_LEFT 2
#define FSEMU_WIDGET_1080_RIGHT 3

enum {
    FSEMU_WIDGET_ANCHOR_INVALID,

    FSEMU_WIDGET_SCREEN_LEFT,
    FSEMU_WIDGET_SCREEN_TOP,
    FSEMU_WIDGET_SCREEN_RIGHT,
    FSEMU_WIDGET_SCREEN_BOTTOM,

    FSEMU_WIDGET_VIDEO_LEFT,
    FSEMU_WIDGET_VIDEO_TOP,
    FSEMU_WIDGET_VIDEO_RIGHT,
    FSEMU_WIDGET_VIDEO_BOTTOM,

    FSEMU_WIDGET_WINDOW_LEFT,
    FSEMU_WIDGET_WINDOW_TOP,
    FSEMU_WIDGET_WINDOW_RIGHT,
    FSEMU_WIDGET_WINDOW_BOTTOM,

    FSEMU_WIDGET_PARENT_LEFT,
    FSEMU_WIDGET_PARENT_TOP,
    FSEMU_WIDGET_PARENT_RIGHT,
    FSEMU_WIDGET_PARENT_BOTTOM,
};

#define FSEMU_WIDGET_FLAG_FLIPX (1 << 0)
#define FSEMU_WIDGET_FLAG_DEBUG (1 << 1)

typedef fsemu_gui_item_t fsemu_widget_t;

void fsemu_widget_module_init(void);

fsemu_widget_t *fsemu_widget_new(void);
fsemu_widget_t *fsemu_widget_new_with_name(const char *name);

void fsemu_widget_ref(fsemu_widget_t *widget);
void fsemu_widget_unref(fsemu_widget_t *widget);

void fsemu_widget_init(fsemu_widget_t *widget);

const char *fsemu_widget_name(fsemu_widget_t *widget);
void fsemu_widget_set_name(fsemu_widget_t *widget, const char *name);

void fsemu_widget_add_child(fsemu_widget_t *widget, fsemu_widget_t *child);
void fsemu_widget_remove_child(fsemu_widget_t *widget, fsemu_widget_t *child);

// Will increase image ref count
void fsemu_widget_set_image(fsemu_widget_t *widget,
                            fsemu_image_t *image,
                            bool image_owner);

void fsemu_widget_set_coords(fsemu_widget_t *widget,
                             double x,
                             double y,
                             double w,
                             double h,
                             int coords);
void fsemu_widget_set_z_index(fsemu_widget_t *widget, int z_index);
void fsemu_widget_set_visible(fsemu_widget_t *widget, bool visible);
void fsemu_widget_set_color(fsemu_widget_t *widget, uint32_t color);
void fsemu_widget_set_flag(fsemu_widget_t *widget, uint32_t flag);
void fsemu_widget_clear_flag(fsemu_widget_t *widget, uint32_t flag);

void fsemu_widget_set_top(fsemu_widget_t *widget, double offset_top);
void fsemu_widget_set_right(fsemu_widget_t *widget, double offset_right);
void fsemu_widget_set_bottom(fsemu_widget_t *widget, double offset_bottom);
void fsemu_widget_set_left(fsemu_widget_t *widget, double offset_left);
void fsemu_widget_set_top_2(fsemu_widget_t *widget,
                            double offset_top,
                            int anchor_top);
void fsemu_widget_set_right_2(fsemu_widget_t *widget,
                              double offset_right,
                              int anchor_right);
void fsemu_widget_set_bottom_2(fsemu_widget_t *widget,
                               double offset_bottom,
                               int anchor_bottom);
void fsemu_widget_set_left_2(fsemu_widget_t *widget,
                             double offset_left,
                             int anchor_left);

// FIXME: Change argument order, top, right, bottom, right (css style)
void fsemu_widget_set_coordinates(fsemu_widget_t *widget,
                                  int anchor_left,
                                  double offset_left,
                                  int anchor_top,
                                  double offset_top,
                                  int anchor_right,
                                  double offset_right,
                                  int anchor_bottom,
                                  double offset_bottom);

// FIXME: Change argument order (offset first, then anchor)
void fsemu_widget_set_coordinates_2(fsemu_widget_t *widget,
                                    int anchor_top,
                                    double offset_top,
                                    int anchor_left,
                                    double offset_left,
                                    int anchor_bottom,
                                    double offset_bottom,
                                    int anchor_right,
                                    double offset_right);

void fsemu_widget_set_text(fsemu_widget_t *widget, const char *text);
void fsemu_widget_set_text_color(fsemu_widget_t *widget,
                                 fsemu_color_t text_color);
void fsemu_widget_set_text_valign(fsemu_widget_t *widget, float text_valign);
#define FSEMU_WIDGET_TEXT_TRANSFORM_NONE 0
#define FSEMU_WIDGET_TEXT_TRANSFORM_UPPERCASE 1
void fsemu_widget_set_text_transform(fsemu_widget_t *widget,
                                     int text_transform);
void fsemu_widget_set_font_size(fsemu_widget_t *widget, int font_size);

// Possibly temporary
void fsemu_widget_update_text_image(fsemu_widget_t *widget);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_WIDGET_H_
