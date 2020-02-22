#define FSEMU_INTERNAL
#include "fsemu-theme.h"

#include "fsemu-gui.h"
#include "fsemu-image.h"
#include "fsemu-widget.h"

static struct {
    // Widgets
    fsemu_widget_t background_w;
} fsemu_theme;

void fsemu_theme_module_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_log("[FSEMU][THEME] Initializing theme module\n");

    return;

    fsemu_widget_t *widget = &fsemu_theme.background_w;
    fsemu_widget_init(widget);
    fsemu_widget_set_image(
        widget,
        fsemu_image_from_stream(fsemu_data_stream("Background.png"), true),
        true);
    widget->rect.x = 0;
    widget->rect.y = 0;
    widget->rect.w = 1920;
    widget->rect.h = 1080;
    widget->z_index = -9999;
    widget->visible = true;
    widget->color = FSEMU_RGB(0xffffff);
    widget->coordinates = FSEMU_COORD_1080P;
    fsemu_gui_add_item(&fsemu_theme.background_w);
}

void fsemu_theme_margins(int *top, int *right, int *bottom, int *left)
{
    *top = 60;
    *right = 60;
    *bottom = 60;
    *left = 60;
}

void fsemu_theme_update(void)
{
}
