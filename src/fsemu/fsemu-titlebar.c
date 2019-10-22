#define FSEMU_INTERNAL
#include "fsemu-titlebar.h"

#include "fsemu-gui.h"
#include "fsemu-video.h"
#include "fsemu-window.h"

// ---------------------------------------------------------------------------

static struct fsemu_titlebar {
    int width;
    int height;
    bool visible;
    bool mouse_trapped;
    fsemu_gui_item_t background;
    fsemu_gui_item_t close_bg;
} fsemu_titlebar;

// ---------------------------------------------------------------------------

void fsemu_titlebar_init(void)
{
    fsemu_return_if_already_initialized();

    fsemu_gui_init();
    fsemu_window_init();

    fsemu_titlebar.width = 640;  // FIXME
    fsemu_titlebar.height = 40;

    // background = fsemu_gui_create_rectangle(0, 0, 10000, 40);
    // fsemu_gui_rectangle(&fsemu_titlebar.background, 0, 0, 1000, 40,
    // 0x181818); fsemu_gui_rectangle(&fsemu_titlebar.background, 0, 0, 1000,
    // 40, 0x202020);
    // fsemu_gui_rectangle(
    //     &fsemu_titlebar.background, 0, 0, 1000, 40, 0x20303030);
    fsemu_gui_rectangle(
        &fsemu_titlebar.background, 0, 0, 1000, 40, FSEMU_RGBA(0x303030e0));
    fsemu_gui_add_item(&fsemu_titlebar.background);

    fsemu_gui_rectangle(&fsemu_titlebar.close_bg,
                        fsemu_titlebar.width - 40,
                        0,
                        40,
                        40,
                        FSEMU_RGB(0x800000));
    fsemu_gui_add_item(&fsemu_titlebar.close_bg);
}

// ---------------------------------------------------------------------------

void fsemu_titlebar_update(void)
{
    fsemu_size_t window_size;
    fsemu_window_size(&window_size);
    // fsemu_titlebar_log("Width %d\n", window_size.w);
    fsemu_titlebar_set_width(window_size.w);

    if (fsemu_titlebar.visible) {
        if (fsemu_titlebar.background.rect.y < 0) {
            fsemu_titlebar.background.rect.y += 5;
            fsemu_titlebar.close_bg.rect.y += 5;
            fsemu_titlebar.background.dirty = true;
        }
    } else {
        if (fsemu_titlebar.background.rect.y > -fsemu_titlebar.height) {
            // fsemu_titlebar.background.rect.y -= 4;
            fsemu_titlebar.background.rect.y = -fsemu_titlebar.height;
            fsemu_titlebar.background.dirty = true;
            fsemu_titlebar.close_bg.rect.y = -fsemu_titlebar.height;
        }
    }
}

// ---------------------------------------------------------------------------

void fsemu_titlebar_set_width(int w)
{
    fsemu_titlebar.width = w;

    fsemu_titlebar.background.rect.w = w;
    fsemu_titlebar.background.dirty = true;

    fsemu_titlebar.close_bg.rect.x = w - 40;
}

// ---------------------------------------------------------------------------

static void fsemu_titlebar_set_visible(bool visible)
{
    fsemu_titlebar.visible = visible;
    fsemu_gui_item_set_visible(&fsemu_titlebar.background, visible);
    fsemu_gui_item_set_visible(&fsemu_titlebar.close_bg, visible);
}

// ---------------------------------------------------------------------------

bool fsemu_titlebar_handle_mouse(fsemu_mouse_event_t *event)
{
    // bool is_open = fsemu_titlebar.visible;
    // FIXME: Check mouse grab / relative mode

    bool cursor_visible = !fsemu_mouse_captured();

    if (fsemu_titlebar.mouse_trapped) {
        return true;
    }

    if (!cursor_visible) {
        return false;
    }

    if (fsemu_titlebar.visible && event->y < fsemu_titlebar.height) {
        return true;
    }

    if (event->y == 0 && event->button == -1) {
        fsemu_titlebar_log("Opening title bar\n");
        // fsemu_titlebar.visible = true;
        fsemu_titlebar_set_visible(true);
        return true;
    }

    if (fsemu_titlebar.visible) {
        fsemu_titlebar_log("Closing title bar\n");
        // fsemu_titlebar.visible = false;
        fsemu_titlebar_set_visible(false);
    }

    return false;
#if 0
    if (event->button >= 0) {
        if (event->state) {
            // Mouse button press
            if (event->y < fsemu_titlebar.height) {
                if (!fsemu_titlebar.mouse_trapped) {
                    fsemu_titlebar.mouse_trapped = 1 + event->button;
                }
            }
        } else {
            // Mouse button release
            if (fsemu_titlebar.mouse_trapped == 1 + event->button) {
                fsemu_titlebar.mouse_trapped = 0;
                if (event->y >= fsemu_titlebar.height) {
                    fsemu_titlebar.visible = false;
                    return false;
                }
            }
        }
    } else {
        // Mouse motion
        if (fsemu_titlebar.mouse_trapped) {
            //
            return true;
        } else if (is_open) {
            if (event->y >= fsemu_titlebar.height) {
                fsemu_titlebar.visible = false;
                return false;
            }
            return true;
        } else {
            if (event->y == 0) {
                fsemu_titlebar_log("Opening title bar");
                fsemu_titlebar.visible = true;
                return true;
            }
        }
    }

    return false;
#endif
}

// ---------------------------------------------------------------------------
#if 0
void fsemu_titlebar_render(fsemu_render_t *render)
{
    uint32_t color = 255;
    render->rectangle(0, 0, 0, 0, color);
}
#endif
// ---------------------------------------------------------------------------
