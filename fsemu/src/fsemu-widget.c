#define FSEMU_INTERNAL
#include "fsemu-widget.h"

#include "fsemu-gui.h"
#include "fsemu-image.h"
#include "fsemu-util.h"
#include "fsemu-video.h"

/*
static struct {
    fsemu_gui_item_t a;
} fsemu_widget;
*/

void fsemu_widget_module_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_log("[FSEMU][WIDGT] Initializing widget module\n");
}

void fsemu_widget_init(fsemu_widget_t *widget)
{
    memset(widget, 0, sizeof(fsemu_widget_t));
    widget->color = FSEMU_RGBA(0xffffffff);
    // fsemu_gui_rectangle(widget, 0, 0, 0, 0, 0);
}

fsemu_widget_t *fsemu_widget_new(void)
{
    // Not allocating zeroed memory here, since fsemu_widget_init does this.
    fsemu_widget_t *widget = FSEMU_UTIL_MALLOC(fsemu_widget_t);
    fsemu_widget_init(widget);
    return widget;
}

fsemu_widget_t *fsemu_widget_new_with_name(const char *name)
{
    fsemu_widget_t *widget = fsemu_widget_new();
    fsemu_widget_set_name(widget, name);
    return widget;
}

const char *fsemu_widget_name(fsemu_widget_t *widget)
{
    if (!widget->name) {
        return "";
    }
    return widget->name;
}

void fsemu_widget_set_name(fsemu_widget_t *widget, const char *name)
{
    if (widget->name) {
        free(widget->name);
    }
    widget->name = strdup(name);
}

void fsemu_widget_add_child(fsemu_widget_t *widget, fsemu_widget_t *child)
{
    if (widget->last_child) {
        widget->last_child->next = child;
        widget->last_child = child;
    } else {
        widget->first_child = child;
        widget->last_child = child;
    }
    child->parent = widget;
}

void fsemu_widget_set_image(fsemu_widget_t *widget,
                            fsemu_image_t *image,
                            bool image_owner)
{
    if (widget->image) {
        fsemu_refable_unref(widget->image);
    }
    widget->image = image;
    if (!image_owner) {
        fsemu_refable_ref(widget->image);
    }
    // widget->image_owner = true;
    widget->dirty = true;
}

void fsemu_widget_set_coords(
    fsemu_widget_t *widget, double x, double y, double w, double h, int coords)
{
    // FIXME: Widget rect coordinates -> double ?
    widget->rect.x = x;
    widget->rect.y = y;
    widget->rect.w = w;
    widget->rect.h = h;
    widget->coordinates = coords;
}

void fsemu_widget_set_z_index(fsemu_widget_t *widget, int z_index)
{
    widget->z_index = z_index;
}

void fsemu_widget_set_visible(fsemu_widget_t *widget, bool visible)
{
    widget->visible = visible;
}

void fsemu_widget_set_color(fsemu_widget_t *widget, uint32_t color)
{
    widget->color = color;
}

void fsemu_widget_set_flag(fsemu_widget_t *widget, uint32_t flag)
{
    widget->flags |= flag;
}

void fsemu_widget_clear_flag(fsemu_widget_t *widget, uint32_t flag)
{
    widget->flags &= ~flag;
}

void fsemu_widget_set_top(fsemu_widget_t *widget,
                          double offset_top,
                          int anchor_top)
{
    widget->top.offset = offset_top;
    widget->top.anchor = anchor_top;
}

void fsemu_widget_set_right(fsemu_widget_t *widget,
                            double offset_right,
                            int anchor_right)
{
    widget->right.offset = offset_right;
    widget->right.anchor = anchor_right;
}

void fsemu_widget_set_bottom(fsemu_widget_t *widget,
                             double offset_bottom,
                             int anchor_bottom)
{
    widget->bottom.offset = offset_bottom;
    widget->bottom.anchor = anchor_bottom;
}

void fsemu_widget_set_left(fsemu_widget_t *widget,
                           double offset_left,
                           int anchor_left)
{
    widget->left.offset = offset_left;
    widget->left.anchor = anchor_left;
}

void fsemu_widget_set_coordinates(fsemu_widget_t *widget,
                                  int anchor_left,
                                  double offset_left,
                                  int anchor_top,
                                  double offset_top,
                                  int anchor_right,
                                  double offset_right,
                                  int anchor_bottom,
                                  double offset_bottom)
{
    widget->left.anchor = anchor_left;
    widget->left.offset = offset_left;
    widget->top.anchor = anchor_top;
    widget->top.offset = offset_top;
    widget->right.anchor = anchor_right;
    widget->right.offset = offset_right;
    widget->bottom.anchor = anchor_bottom;
    widget->bottom.offset = offset_bottom;
}

void fsemu_widget_set_coordinates_2(fsemu_widget_t *widget,
                                    int anchor_top,
                                    double offset_top,
                                    int anchor_left,
                                    double offset_left,
                                    int anchor_bottom,
                                    double offset_bottom,
                                    int anchor_right,
                                    double offset_right)
{
    widget->top.anchor = anchor_top;
    widget->top.offset = offset_top;
    widget->left.anchor = anchor_left;
    widget->left.offset = offset_left;
    widget->bottom.anchor = anchor_bottom;
    widget->bottom.offset = offset_bottom;
    widget->right.anchor = anchor_right;
    widget->right.offset = offset_right;
}