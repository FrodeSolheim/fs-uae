#define FSEMU_INTERNAL
#include "fsemu-titlebar.h"

#include "fsemu-common.h"
#include "fsemu-control.h"
#include "fsemu-font.h"
#include "fsemu-gui.h"
#include "fsemu-image.h"
#include "fsemu-option.h"
#include "fsemu-options.h"
#include "fsemu-video.h"
#include "fsemu-window.h"

/*
 * FIXME: Mouse cursor should not disappear while hovering on top of title bar.
 */

enum {
    FSEMU_TITLEBAR_NONE,
    FSEMU_TITLEBAR_MINIMIZE,
    FSEMU_TITLEBAR_MAXIMIZE,
    FSEMU_TITLEBAR_CLOSE
};

#define FSEMU_TITLEBAR_MINIMIZE_HOVER FSEMU_RGBA(0x555555ff)
#define FSEMU_TITLEBAR_MINIMIZE_ACTIVE FSEMU_RGBA(0x777777ff)
#define FSEMU_TITLEBAR_MAXIMIZE_HOVER FSEMU_RGBA(0x555555ff)
#define FSEMU_TITLEBAR_MAXIMIZE_ACTIVE FSEMU_RGBA(0x777777ff)
#define FSEMU_TITLEBAR_CLOSE_HOVER FSEMU_RGBA(0x880000ff)
#define FSEMU_TITLEBAR_CLOSE_ACTIVE FSEMU_RGBA(0xaa0000ff)

// ---------------------------------------------------------------------------

static struct fsemu_titlebar {
    int active_button;
    int hover_button;
    int width;
    int height;
    // True if we want to keep the status bar visible. Note, it can be visible
    // also when this is false, see fsemu_titlebar_visible function.
    bool visible;
    bool mouse_trapped;
    bool want_cursor;
    // Use system decorations
    bool use_system;
    fsemu_gui_item_t background_item;
    uint32_t background_color;
    uint32_t background_color_inactive;
    fsemu_gui_item_t background_ws_item;
    fsemu_image_t *background_ws_image;
    fsemu_gui_item_t title_item;
    fsemu_image_t *title_image;
    uint32_t title_color;
    // uint32_t title_color_inactive;
    int title_opacity_inactive;
    fsemu_gui_item_t maximize_bg_item;
    fsemu_gui_item_t maximize_item;
    fsemu_gui_item_t minimize_bg_item;
    fsemu_gui_item_t minimize_item;
    fsemu_gui_item_t close_bg_item;
    fsemu_gui_item_t close_item;

} fsemu_titlebar;

// ---------------------------------------------------------------------------

void fsemu_titlebar_init(void)
{
    fsemu_return_if_already_initialized();

    fsemu_gui_init();
    // fsemu_window_init();

    fsemu_option_read_bool_default(
        FSEMU_OPTION_SYSTEM_TITLEBAR, &fsemu_titlebar.use_system, false);
    // fsemu_titlebar.use_system = false;

    fsemu_titlebar.width = 640;  // FIXME
    fsemu_titlebar.height = fsemu_titlebar_unscaled_height();

    fsemu_titlebar.background_color = FSEMU_RGBA(0x202020ff);
    fsemu_titlebar.background_color_inactive = FSEMU_RGBA(0x303030ff);

    fsemu_titlebar.title_color = FSEMU_RGBA(0xffffffcc);
    // FIXME: This does not have any effect yet, since text is only rendered
    // to image initially.
    // fsemu_titlebar.title_color_inactive = FSEMU_RGBA(0xffffff99);
    // Can use opacity though
    fsemu_titlebar.title_opacity_inactive = 0x80;

    // background = fsemu_gui_create_rectangle(0, 0, 10000, 40);
    // fsemu_gui_rectangle(&fsemu_titlebar.background, 0, 0, 1000, 40,
    // 0x181818); fsemu_gui_rectangle(&fsemu_titlebar.background, 0, 0, 1000,
    // 40, 0x202020);
    // fsemu_gui_rectangle(
    //     &fsemu_titlebar.background, 0, 0, 1000, 40, 0x20303030);
    fsemu_gui_item_t *item;
    fsemu_image_t *image;

    item = &fsemu_titlebar.background_item;
    // titlebar_color = FSEMU_RGBA(0xff202080);
    fsemu_gui_rectangle(
        item, 0, 0, 0, fsemu_titlebar.height, fsemu_titlebar.background_color);
    item->z_index = 9000;
    fsemu_gui_add_item(item);

    // With shadow
    item = &fsemu_titlebar.background_ws_item;
    fsemu_titlebar.background_ws_image =
        fsemu_image_load("TitleBarWithShadow.png");
    fsemu_gui_image(item,
                    0,
                    0,
                    1000,
                    fsemu_titlebar.background_ws_image->height,
                    fsemu_titlebar.background_ws_image);
    item->z_index = 9000;
    fsemu_gui_add_item(item);

#if 0
    double ui_scale = fsemu_window_ui_scale();

    item = &fsemu_titlebar.title_item;
    // FIXME: Semi-bold ?
    fsemu_font_t *font = fsemu_font_load("SairaCondensed-SemiBold.ttf", 19 * ui_scale);
    // 0xcc is the same alpha value as used by the window control icons
    // for the custom frame.
    image = fsemu_font_render_text_to_image(
        font, fsemu_emulator_name(), fsemu_titlebar.title_color);
    fsemu_gui_image(item, 20 * ui_scale, 0, image->width, image->height, image);
    item->z_index = 9001;
    fsemu_gui_add_item(item);
#endif

    item = &fsemu_titlebar.minimize_bg_item;
    fsemu_gui_rectangle(item, 0, 0, 40, 40, 0);
    item->z_index = 9001;
    fsemu_gui_add_item(item);
    item = &fsemu_titlebar.minimize_item;
    image = fsemu_image_load("TitleBarMinimizeIcon.png");
    fsemu_gui_image(item, 0, 0, image->width, image->height, image);
    item->z_index = 9002;
    fsemu_gui_add_item(item);

    item = &fsemu_titlebar.maximize_bg_item;
    fsemu_gui_rectangle(item, 0, 0, 40, 40, 0);
    item->z_index = 9001;
    fsemu_gui_add_item(item);
    item = &fsemu_titlebar.maximize_item;
    image = fsemu_image_load("TitleBarMaximizeIcon.png");
    fsemu_gui_image(item, 0, 0, image->width, image->height, image);
    item->z_index = 9002;
    fsemu_gui_add_item(item);

    item = &fsemu_titlebar.close_bg_item;
    fsemu_gui_rectangle(item, 0, 0, 40, 40, 0);
    item->z_index = 9001;
    fsemu_gui_add_item(item);
    item = &fsemu_titlebar.close_item;
    image = fsemu_image_load("TitleBarCloseIcon.png");
    fsemu_gui_image(item, 0, 0, image->width, image->height, image);
    item->z_index = 9002;
    fsemu_gui_add_item(item);

    fsemu_titlebar_update();
}

uint32_t fsemu_titlebar_background_color(void)
{
    return fsemu_titlebar.background_color;
}

bool fsemu_titlebar_use_system(void)
{
    return fsemu_titlebar.use_system;
}

int fsemu_titlebar_unscaled_height(void)
{
    return 40;
}

int fsemu_titlebar_height(void)
{
    return fsemu_titlebar.height;
}

bool fsemu_titlebar_visible(void)
{
    return fsemu_titlebar.visible || fsemu_titlebar_static();
}

// ---------------------------------------------------------------------------

void fsemu_titlebar_update(void)
{
    fsemu_size_t window_size;
    fsemu_window_size(&window_size);
    fsemu_titlebar_set_width(window_size.w);

    double ui_scale = fsemu_window_ui_scale();
    fsemu_titlebar.height = 40 * ui_scale;

    // We need to create the title image after ui scale is known, which is why
    // we didn't initialize this in fsemu_titlebar_init. Not entirely happy
    // with how this works now, but it's fair enough.
    if (!fsemu_titlebar.title_item.image && ui_scale > 0) {
        fsemu_gui_item_t *item;
        item = &fsemu_titlebar.title_item;
        // FIXME: Semi-bold ?
        fsemu_font_t *font =
            fsemu_font_load("SairaCondensed-SemiBold.ttf", 19 * ui_scale);
        // 0xcc is the same alpha value as used by the window control icons
        // for the custom frame.
        fsemu_titlebar.title_image = fsemu_font_render_text_to_image(
            font, fsemu_emulator_name(), fsemu_titlebar.title_color);
#if 0
        int y = (fsemu_titlebar.height - image->height) / 2;
        // Hand-tuned y offset for common ui scales for better text centering.
        if (ui_scale == 1.0) {
            y = 0;
        } else if (ui_scale == 2.0) {
            y = 20;
        }
#endif
        fsemu_gui_image(item,
                        20 * ui_scale,
                        0,
                        fsemu_titlebar.title_image->width,
                        fsemu_titlebar.title_image->height,
                        fsemu_titlebar.title_image);
        item->z_index = 9001;
        fsemu_gui_add_item(item);
    }

    // printf("%d %0.2f\n", fsemu_titlebar.height, fsemu_window_ui_scale());

    fsemu_titlebar.background_item.rect.h = fsemu_titlebar.height;
    fsemu_titlebar.background_ws_item.rect.h =
        fsemu_titlebar.background_ws_image->height * ui_scale;

    fsemu_titlebar.minimize_bg_item.rect.h = fsemu_titlebar.height;
    fsemu_titlebar.minimize_bg_item.rect.w = fsemu_titlebar.height;
    fsemu_titlebar.minimize_item.rect.h = fsemu_titlebar.height;
    fsemu_titlebar.minimize_item.rect.w = fsemu_titlebar.height;

    fsemu_titlebar.maximize_bg_item.rect.h = fsemu_titlebar.height;
    fsemu_titlebar.maximize_bg_item.rect.w = fsemu_titlebar.height;
    fsemu_titlebar.maximize_item.rect.h = fsemu_titlebar.height;
    fsemu_titlebar.maximize_item.rect.w = fsemu_titlebar.height;

    fsemu_titlebar.close_bg_item.rect.h = fsemu_titlebar.height;
    fsemu_titlebar.close_bg_item.rect.w = fsemu_titlebar.height;
    fsemu_titlebar.close_item.rect.h = fsemu_titlebar.height;
    fsemu_titlebar.close_item.rect.w = fsemu_titlebar.height;
    // FIXME: set dirty flags?

    int y = 0;
    bool is_visible = fsemu_titlebar.visible;
    bool is_static = fsemu_titlebar_static();
    bool window_is_active = fsemu_window_active();

    // fsemu_titlebar_log("Update width=%d visible=%d static=%d active=%d\n",
    //                    window_size.w,
    //                    is_visible,
    //                    is_static,
    //                    window_is_active);

    uint32_t opacity_color =
        FSEMU_RGB_A(0xffffff,
                    window_is_active ? fsemu_titlebar.title_color
                                     : fsemu_titlebar.title_opacity_inactive);

    if (is_static) {
        // Static titlebar in window mode
        is_visible = true;
    } else if (fsemu_titlebar.visible) {
        if (fsemu_titlebar.background_item.rect.y < 0) {
            y = fsemu_titlebar.background_item.rect.y + 5 * ui_scale;
        }
    } else {
        y = -fsemu_titlebar.height;
    }

    fsemu_titlebar.background_item.rect.y = y;
    fsemu_titlebar.background_ws_item.rect.y = y;

    if (fsemu_titlebar.title_image) {
        int title_y =
            (fsemu_titlebar.height - fsemu_titlebar.title_image->height) / 2;
        // Hand-tuned y offset for common ui scales for better text centering.
        if (ui_scale == 1.0) {
            // FIXME: Mayne not needed, doublecheck
            title_y = 5;
        }
        fsemu_titlebar.title_item.rect.y = y + title_y;
    }

    fsemu_titlebar.minimize_bg_item.rect.y = y;
    fsemu_titlebar.minimize_item.rect.y = y;
    fsemu_titlebar.maximize_bg_item.rect.y = y;
    fsemu_titlebar.maximize_item.rect.y = y;
    fsemu_titlebar.close_bg_item.rect.y = y;
    fsemu_titlebar.close_item.rect.y = y;

    int active = fsemu_titlebar.active_button;
    int hover = fsemu_titlebar.hover_button;

    fsemu_gui_item_set_visible(&fsemu_titlebar.background_item,
                               is_visible && is_static);
    fsemu_titlebar.background_item.color =
        window_is_active ? fsemu_titlebar.background_color
                         : fsemu_titlebar.background_color_inactive;

    fsemu_gui_item_set_visible(&fsemu_titlebar.background_ws_item,
                               is_visible && !is_static);

    fsemu_gui_item_set_visible(&fsemu_titlebar.title_item, is_visible);
    // fsemu_titlebar.title_item.color =
    //     window_is_active ? fsemu_titlebar.title_color
    //                      : fsemu_titlebar.title_color_inactive;
    fsemu_titlebar.title_item.color = opacity_color;

    fsemu_gui_item_set_visible(&fsemu_titlebar.minimize_item, is_visible);
    fsemu_gui_item_set_visible(&fsemu_titlebar.maximize_item, is_visible);
    fsemu_gui_item_set_visible(&fsemu_titlebar.close_item, is_visible);

    fsemu_titlebar.minimize_item.color = opacity_color;
    fsemu_titlebar.maximize_item.color = opacity_color;
    fsemu_titlebar.close_item.color = opacity_color;

    fsemu_gui_item_set_visible(&fsemu_titlebar.minimize_bg_item,
                               is_visible && hover == FSEMU_TITLEBAR_MINIMIZE);
    fsemu_titlebar.minimize_bg_item.color =
        active == FSEMU_TITLEBAR_MINIMIZE ? FSEMU_TITLEBAR_MINIMIZE_ACTIVE
                                          : FSEMU_TITLEBAR_MINIMIZE_HOVER;

    fsemu_gui_item_set_visible(&fsemu_titlebar.maximize_bg_item,
                               is_visible && hover == FSEMU_TITLEBAR_MAXIMIZE);
    fsemu_titlebar.maximize_bg_item.color =
        active == FSEMU_TITLEBAR_MAXIMIZE ? FSEMU_TITLEBAR_MAXIMIZE_ACTIVE
                                          : FSEMU_TITLEBAR_MAXIMIZE_HOVER;

    fsemu_gui_item_set_visible(&fsemu_titlebar.close_bg_item,
                               is_visible && hover == FSEMU_TITLEBAR_CLOSE);
    fsemu_titlebar.close_bg_item.color = active == FSEMU_TITLEBAR_CLOSE
                                             ? FSEMU_TITLEBAR_CLOSE_ACTIVE
                                             : FSEMU_TITLEBAR_CLOSE_HOVER;
}

// ---------------------------------------------------------------------------

void fsemu_titlebar_set_width(int w)
{
    fsemu_titlebar.width = w;

    fsemu_titlebar.background_item.rect.w = w;
    fsemu_titlebar.background_item.dirty = true;

    fsemu_titlebar.background_ws_item.rect.w = w;
    fsemu_titlebar.background_ws_item.dirty = true;

    fsemu_titlebar.minimize_bg_item.rect.x = w - fsemu_titlebar.height * 3;
    fsemu_titlebar.minimize_item.rect.x = w - fsemu_titlebar.height * 3;

    fsemu_titlebar.maximize_bg_item.rect.x = w - fsemu_titlebar.height * 2;
    fsemu_titlebar.maximize_item.rect.x = w - fsemu_titlebar.height * 2;

    fsemu_titlebar.close_bg_item.rect.x = w - fsemu_titlebar.height * 1;
    fsemu_titlebar.close_item.rect.x = w - fsemu_titlebar.height * 1;
}

// ---------------------------------------------------------------------------

static void fsemu_titlebar_set_visible(bool visible)
{
    fsemu_titlebar.visible = visible;
    // fsemu_gui_item_set_visible(&fsemu_titlebar.background_item, visible);
    /*
    fsemu_gui_item_set_visible(&fsemu_titlebar.background_ws_item, visible);
    fsemu_gui_item_set_visible(&fsemu_titlebar.title_item, visible);
    fsemu_gui_item_set_visible(&fsemu_titlebar.close_bg_item, visible);
    fsemu_gui_item_set_visible(&fsemu_titlebar.close_item, visible);
    */
}

// ---------------------------------------------------------------------------

static int fsemu_titlebar_button_test(fsemu_mouse_event_t *event)
{
    if (event->y >= 0 && event->y < fsemu_titlebar.height) {
        if (event->x > fsemu_titlebar.width - fsemu_titlebar.height * 1) {
            return FSEMU_TITLEBAR_CLOSE;
        }
        if (event->x > fsemu_titlebar.width - fsemu_titlebar.height * 2) {
            return FSEMU_TITLEBAR_MAXIMIZE;
        }
        if (event->x > fsemu_titlebar.width - fsemu_titlebar.height * 3) {
            return FSEMU_TITLEBAR_MINIMIZE;
        }
    }
    return 0;
}

bool fsemu_titlebar_want_cursor(void)
{
    return fsemu_titlebar.want_cursor;
}

#include "fsemu-sdl.h"
#include "fsemu-sdlwindow.h"

static void fsemu_titlebar_capture_mouse(bool capture)
{
    fsemu_titlebar.mouse_trapped = capture;
    // Breaking the abstraction here, but might be important to do
    // this synchronously/now.
    SDL_CaptureMouse(capture ? SDL_TRUE : SDL_FALSE);
    fsemu_titlebar.mouse_trapped = capture;
}

static void fsemu_titlebar_on_minimize_button(void)
{
    // FIXME: Do not use SDL directly here
    SDL_MinimizeWindow(fsemu_sdlwindow_window());
}

static void fsemu_titlebar_on_maximize_button(void)
{
    // FIXME: Do not use SDL directly here
    SDL_Window *window = fsemu_sdlwindow_window();
    if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) {
        fsemu_window_toggle_fullscreen();
    } else if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
        fsemu_window_toggle_fullscreen();
    } else if (SDL_GetWindowFlags(window) & SDL_WINDOW_MAXIMIZED) {
        SDL_RestoreWindow(window);
    } else {
        SDL_MaximizeWindow(window);
    }
}

static void fsemu_titlebar_on_close_button(void)
{
    fsemu_control_quit();
}

bool fsemu_titlebar_handle_mouse(fsemu_mouse_event_t *event)
{
    // bool is_open = fsemu_titlebar.visible;
    // FIXME: Check mouse grab / relative mode
#if 0
    printf(
        "fsemu_titlebar_handle_mouse visible=%d y<height=%d "
        "trapped=%d\n",
        fsemu_titlebar.visible,
        event->y < fsemu_titlebar.height,
        fsemu_titlebar.mouse_trapped);
#endif

    if (fsemu_mouse_captured()) {
        fsemu_titlebar.want_cursor = false;
        return false;
    }

    bool visible = fsemu_titlebar_visible();

    if ((visible && event->y < fsemu_titlebar.height) ||
        fsemu_titlebar.mouse_trapped) {
        fsemu_titlebar.want_cursor = true;
        if (event->moved) {
            if (fsemu_titlebar.mouse_trapped) {
                // Do not set hover_button while left mouse button is
                // pressed
            } else {
                fsemu_titlebar.hover_button =
                    fsemu_titlebar_button_test(event);
            }
            // } else if (event->left_window) {  FIXME!

        } else if (event->pressed == 1) {
            fsemu_titlebar.active_button = fsemu_titlebar_button_test(event);
            /*
            if (fsemu_titlebar.active_button) {
                fsemu_titlebar.hover_button =
            fsemu_titlebar.active_button
            }
            */
            fsemu_titlebar_capture_mouse(true);
        } else if (event->released == 1) {
            int active = fsemu_titlebar.active_button;
            if (active && active == fsemu_titlebar_button_test(event)) {
                if (active == FSEMU_TITLEBAR_MINIMIZE) {
                    fsemu_titlebar_on_minimize_button();
                }
                if (active == FSEMU_TITLEBAR_MAXIMIZE) {
                    fsemu_titlebar_on_maximize_button();
                }
                if (active == FSEMU_TITLEBAR_CLOSE) {
                    fsemu_titlebar_on_close_button();
                }
            }
            // printf("release trap mode\n");
            fsemu_titlebar.active_button = 0;
            fsemu_titlebar_capture_mouse(false);
            // Check if we are now hovering above another element, when
            // the button was released
            fsemu_titlebar.hover_button = fsemu_titlebar_button_test(event);
        }
    }

    if (fsemu_titlebar_static() && event->y > fsemu_titlebar_height() &&
        !fsemu_titlebar.mouse_trapped) {
        fsemu_titlebar.hover_button = FSEMU_TITLEBAR_NONE;
    }

    // We re-run the check because mouse_trapped may have changed
    if ((visible && event->y < fsemu_titlebar.height) ||
        fsemu_titlebar.mouse_trapped) {
        return true;
    }

    if (event->y == 0 && !event->button) {
        fsemu_titlebar_log("Opening title bar (x=%d y=%d button=%d)\n",
                           event->x,
                           event->x,
                           event->button);
        fsemu_titlebar_set_visible(true);
        fsemu_titlebar.want_cursor = true;
        return true;
    }

    if (fsemu_titlebar.visible) {
        fsemu_titlebar_log("Closing title bar\n");
        fsemu_titlebar_set_visible(false);
    }

    fsemu_titlebar.want_cursor = false;

    if (event->moved && event->x == -1 && event->y == -1) {
        fsemu_titlebar_log("Mouse left window, stopping propagation\n");
        return true;
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
