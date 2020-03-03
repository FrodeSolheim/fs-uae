#define FSEMU_INTERNAL
#include "fsemu-widget.h"

#include "fsemu-color.h"
#include "fsemu-fontcache.h"
#include "fsemu-glib.h"
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
    widget->color = FSEMU_COLOR_WHITE;
    // fsemu_gui_rectangle(widget, 0, 0, 0, 0, 0);
    widget->top.anchor = FSEMU_WIDGET_PARENT_TOP;
    widget->right.anchor = FSEMU_WIDGET_PARENT_RIGHT;
    widget->bottom.anchor = FSEMU_WIDGET_PARENT_BOTTOM;
    widget->left.anchor = FSEMU_WIDGET_PARENT_LEFT;

    widget->text_valign = 0.5;
    widget->font_size = 32;
}

void fsemu_widget_ref(fsemu_widget_t *widget)
{
    fsemu_refable_ref(widget);
}

void fsemu_widget_unref(fsemu_widget_t *widget)
{
    fsemu_refable_unref(widget);
}

static void fsemu_widget_finalize(void *object)
{
#if 1
    fsemu_widget_t *widget = (fsemu_widget_t *) object;
    fsemu_log("Finalizing widget %p \"%s\"\n",
              widget,
              widget->name ? widget->name : "unnamed");

    fsemu_widget_t *child = widget->first_child;
    while (child) {
        fsemu_widget_t *next = child->next;
        fsemu_widget_unref(child);
        child = next;
    }

    if (widget->image) {
        printf("- Unrefing image...\n");
        fsemu_image_unref(widget->image);
        // widget->image = NULL;
    }
    if (widget->textimage) {
        printf("- Unrefing textimage...\n");
        fsemu_image_unref(widget->textimage);
        // widget->image = NULL;
    }
    if (widget->text) {
        free(widget->text);
    }
    if (widget->text_cached) {
        free(widget->text_cached);
    }
    if (widget->name) {
        free(widget->name);
    }
    free(widget);
#endif
}

fsemu_widget_t *fsemu_widget_new(void)
{
    // Not allocating zeroed memory here, since fsemu_widget_init does this.
    fsemu_widget_t *widget = FSEMU_UTIL_MALLOC(fsemu_widget_t);
    fsemu_widget_init(widget);
    fsemu_refable_init_with_finalizer(widget, fsemu_widget_finalize);
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
    // The parent widget takes ownership of the child. When removing a child,
    // or when the parent is finalized, children must be unrefed as well.
    fsemu_widget_ref(child);
    if (widget->last_child) {
        widget->last_child->next = child;
        widget->last_child = child;
    } else {
        widget->first_child = child;
        widget->last_child = child;
    }
    child->parent = widget;
}

void fsemu_widget_remove_child(fsemu_widget_t *widget,
                               fsemu_widget_t *remove_child)
{
    fsemu_widget_t *child = widget->first_child;
    fsemu_widget_t *prev = NULL;
    while (child) {
        fsemu_widget_t *next = child->next;
        if (child == remove_child) {
            if (child == widget->first_child) {
                widget->first_child = next;
            } else {
                prev->next = next;
            }
            if (child == widget->last_child) {
                widget->last_child = prev;
            }
            fsemu_widget_unref(child);
            return;
        }
        prev = child;
        child = next;
    }
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

// ----------------------------------------------------------------------------

void fsemu_widget_set_top(fsemu_widget_t *widget, double offset_top)
{
    widget->top.offset = offset_top;
}
void fsemu_widget_set_right(fsemu_widget_t *widget, double offset_right)
{
    widget->right.offset = offset_right;
}
void fsemu_widget_set_bottom(fsemu_widget_t *widget, double offset_bottom)
{
    widget->bottom.offset = offset_bottom;
}
void fsemu_widget_set_left(fsemu_widget_t *widget, double offset_left)
{
    widget->left.offset = offset_left;
}

// ----------------------------------------------------------------------------

void fsemu_widget_set_top_2(fsemu_widget_t *widget,
                            double offset_top,
                            int anchor_top)
{
    widget->top.offset = offset_top;
    widget->top.anchor = anchor_top;
}

void fsemu_widget_set_right_2(fsemu_widget_t *widget,
                              double offset_right,
                              int anchor_right)
{
    widget->right.offset = offset_right;
    widget->right.anchor = anchor_right;
}

void fsemu_widget_set_bottom_2(fsemu_widget_t *widget,
                               double offset_bottom,
                               int anchor_bottom)
{
    widget->bottom.offset = offset_bottom;
    widget->bottom.anchor = anchor_bottom;
}

void fsemu_widget_set_left_2(fsemu_widget_t *widget,
                             double offset_left,
                             int anchor_left)
{
    widget->left.offset = offset_left;
    widget->left.anchor = anchor_left;
}

// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------

void fsemu_widget_set_text_color(fsemu_widget_t *widget,
                                 fsemu_color_t text_color)
{
    widget->text_color = text_color;
}

void fsemu_widget_set_text_transform(fsemu_widget_t *widget,
                                     int text_transform)
{
    widget->text_transform = text_transform;
}

void fsemu_widget_set_text_valign(fsemu_widget_t *widget, float text_valign)
{
    widget->text_valign = text_valign;
}

void fsemu_widget_set_font_size(fsemu_widget_t *widget, int font_size)
{
    widget->font_size = font_size;
}

void fsemu_widget_set_text(fsemu_widget_t *widget, const char *text)
{
    if (widget->text) {
        free(widget->text);
    }
    widget->text = strdup(text);

    // FIXME: We don't want to do this here, just doing it right now for
    // simplicity
    fsemu_widget_update_text_image(widget);
}

void fsemu_widget_update_text_image(fsemu_widget_t *widget)
{
    if (widget->text == NULL) {
        return;
    }
    if (widget->text_cached != NULL) {
        if (strcmp(widget->text, widget->text_cached) == 0) {
            return;
        }
    }
    if (widget->text_cached != NULL) {
        free(widget->text_cached);
    }

    char *text;
    bool free_text;
    if (widget->text_transform == FSEMU_WIDGET_TEXT_TRANSFORM_UPPERCASE) {
        text = g_utf8_strup(widget->text, -1);
        widget->text_cached = strdup(widget->text);
        free_text = true;
    } else {
        text = strdup(widget->text);
        // The text is not modified, so we can re-use this for cache.
        widget->text_cached = text;
        free_text = false;
    }

    fsemu_font_t *font =
        fsemu_fontcache_font("SairaCondensed-SemiBold.ttf", widget->font_size);
    fsemu_image_t *textimage =
        fsemu_font_render_text_to_image(font, text, widget->text_color);
    if (widget->textimage) {
        fsemu_image_unref(widget->textimage);
    }
    // printf("teximage %p\n", textimage);
    widget->textimage = textimage;
    widget->dirty = true;
    if (free_text) {
        free(text);
    }

    // fsemu_font_unref(font);
}
