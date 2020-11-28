#include "fsemu-internal.h"
#include "fsemu-titlebar.h"

#include "fsemu-color.h"
#include "fsemu-common.h"
#include "fsemu-control.h"
#include "fsemu-fontcache.h"
#include "fsemu-glib.h"
#include "fsemu-gui.h"
#include "fsemu-image.h"
#include "fsemu-layer.h"
#include "fsemu-module.h"
#include "fsemu-option.h"
#include "fsemu-osmenu.h"
#include "fsemu-widget.h"
#include "fsemu-window.h"

enum {
    FSEMU_TITLEBAR_NONE,
    FSEMU_TITLEBAR_MENU,
    FSEMU_TITLEBAR_MINIMIZE,
    FSEMU_TITLEBAR_MAXIMIZE,
    FSEMU_TITLEBAR_CLOSE
};

#define FSEMU_TITLEBAR_BACKGROUND FSEMU_COLOR_RGBA(0x282828ff)
#define FSEMU_TITLEBAR_BACKGROUND_INACTIVE FSEMU_COLOR_RGBA(0x303030ff)

#define FSEMU_TITLEBAR_CLOSE_HOVER FSEMU_COLOR_RGBA(0x880000ff)
#define FSEMU_TITLEBAR_CLOSE_ACTIVE FSEMU_COLOR_RGBA(0xaa0000ff)

#define FSEMU_TITLEBAR_MAXIMIZE_HOVER FSEMU_COLOR_RGBA(0x555555ff)
#define FSEMU_TITLEBAR_MAXIMIZE_ACTIVE FSEMU_COLOR_RGBA(0x777777ff)

#define FSEMU_TITLEBAR_MENU_HOVER FSEMU_COLOR_RGBA(0x555555ff)
#define FSEMU_TITLEBAR_MENU_ACTIVE FSEMU_COLOR_RGBA(0x777777ff)

// Same as in FS-UAE Launcher
#define FSEMU_TITLEBAR_MENU_SPACING 6

#define FSEMU_TITLEBAR_MINIMIZE_HOVER FSEMU_COLOR_RGBA(0x555555ff)
#define FSEMU_TITLEBAR_MINIMIZE_ACTIVE FSEMU_COLOR_RGBA(0x777777ff)

// ---------------------------------------------------------------------------

#define module fsemu_titlebar

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

    uint32_t background_color;
    uint32_t background_color_inactive;
    fsemu_gui_item_t background_item;
    fsemu_image_t *background_ws_image;
    fsemu_gui_item_t background_ws_item;

    fsemu_gui_item_t close_bg_item;
    fsemu_image_t *close_icon;
    fsemu_gui_item_t close_item;

    fsemu_gui_item_t maximize_bg_item;
    fsemu_image_t *maximize_icon;
    fsemu_gui_item_t maximize_item;

    fsemu_gui_item_t menu_bg_item;
    fsemu_image_t *menu_icon;
    fsemu_gui_item_t menu_item;

    fsemu_gui_item_t minimize_bg_item;
    fsemu_image_t *minimize_icon;
    fsemu_gui_item_t minimize_item;

    uint32_t title_color;
    // uint32_t title_color_inactive;
    fsemu_image_t *title_image;
    fsemu_gui_item_t title_item;
    int title_opacity_inactive;

    struct {
        fsemu_widget_t *container;
        fsemu_widget_t *close;
        fsemu_widget_t *close_bg;
    } widgets;

} module;

// ---------------------------------------------------------------------------

uint32_t fsemu_titlebar_background_color(void)
{
    return module.background_color;
}

bool fsemu_titlebar_use_system(void)
{
    return module.use_system;
}

int fsemu_titlebar_unscaled_height(void)
{
    return 40;
}

int fsemu_titlebar_height(void)
{
    return module.height;
}

bool fsemu_titlebar_visible(void)
{
    return module.visible || fsemu_titlebar_static();
}

// ---------------------------------------------------------------------------

void fsemu_titlebar_update(void)
{
    fsemu_size_t window_size;
    fsemu_window_size(&window_size);
    fsemu_titlebar_set_width(window_size.w);

    double ui_scale = fsemu_window_ui_scale();
    module.height = 40 * ui_scale;

    // We need to create the title image after ui scale is known, which is why
    // we didn't initialize this in fsemu_titlebar_init. Not entirely happy
    // with how this works now, but it's fair enough.
    if (!module.title_item.image && ui_scale > 0) {
        fsemu_gui_item_t *item;
        item = &module.title_item;
        // FIXME: Semi-bold ?
        fsemu_font_t *font = fsemu_fontcache_font(
            "Fonts/SairaCondensed-SemiBold.ttf", 19 * ui_scale);
        // 0xcc is the same alpha value as used by the window control icons
        // for the custom frame.       
        // char *title_upper = g_utf8_strup(fsemu_emulator_name(), -1);
        const char *title = fsemu_window_title();
        char *title_upper = g_utf8_strup(title, -1);
        module.title_image = fsemu_font_render_text_to_image(
            font, title_upper, module.title_color);
        free(title_upper);
#if 0
        int y = (module.height - image->height) / 2;
        // Hand-tuned y offset for common ui scales for better text centering.
        if (ui_scale == 1.0) {
            y = 0;
        } else if (ui_scale == 2.0) {
            y = 20;
        }
#endif
        fsemu_gui_image(
            item,
            // 20 * ui_scale
            fsemu_titlebar_height() + FSEMU_TITLEBAR_MENU_SPACING * ui_scale,
            0,
            module.title_image->width,
            module.title_image->height,
            module.title_image);
        item->z_index = 9001;
        fsemu_gui_add_item(item);
    }

    // printf("%d %0.2f\n", module.height, fsemu_window_ui_scale());

    module.background_item.rect.h = module.height;
    module.background_ws_item.rect.h =
        module.background_ws_image->height * ui_scale;

    module.minimize_bg_item.rect.h = module.height;
    module.minimize_bg_item.rect.w = module.height;
    module.minimize_item.rect.h = module.height;
    module.minimize_item.rect.w = module.height;

    module.maximize_bg_item.rect.h = module.height;
    module.maximize_bg_item.rect.w = module.height;
    module.maximize_item.rect.h = module.height;
    module.maximize_item.rect.w = module.height;

    module.menu_bg_item.rect.h = module.height;
    module.menu_bg_item.rect.w = module.height;
    module.menu_item.rect.h = module.height;
    module.menu_item.rect.w = module.height;

#if 0
    module.widgets.close_bg->rect.h = module.height;
    module.widgets.close_bg->rect.w = module.height;
    module.widgets.close->rect.h = module.height;
    module.widgets.close->rect.w = module.height;
#else
    module.close_bg_item.rect.h = module.height;
    module.close_bg_item.rect.w = module.height;
    module.close_item.rect.h = module.height;
    module.close_item.rect.w = module.height;
#endif
    // FIXME: set dirty flags?

    int y = 0;
    bool is_visible = module.visible;
    bool is_static = fsemu_titlebar_static();
    bool window_is_active = fsemu_window_active();

    // fsemu_titlebar_log("Update width=%d visible=%d static=%d active=%d\n",
    //                    window_size.w,
    //                    is_visible,
    //                    is_static,
    //                    window_is_active);

    uint32_t opacity_color = FSEMU_COLOR_RGB_A(
        0xffffff,
        window_is_active ? module.title_color : module.title_opacity_inactive);

    if (is_static) {
        // Static titlebar in window mode
        is_visible = true;
    } else if (module.visible) {
        if (module.background_item.rect.y < 0) {
            y = module.background_item.rect.y + 5 * ui_scale;
        }
    } else {
        y = -module.height;
    }

    module.background_item.rect.y = y;
    module.background_ws_item.rect.y = y;

    if (module.title_image) {
        int title_y = (module.height - module.title_image->height) / 2;
        // Hand-tuned y offset for common ui scales for better text centering.
        if (ui_scale == 1.0) {
            // FIXME: Mayne not needed, doublecheck
            title_y = 5;
        }
        module.title_item.rect.y = y + title_y;
    }

    module.maximize_bg_item.rect.y = y;
    module.maximize_item.rect.y = y;

    module.menu_bg_item.rect.y = y;
    module.menu_item.rect.y = y;

    module.minimize_bg_item.rect.y = y;
    module.minimize_item.rect.y = y;

#if 0
    module.widgets.close_bg->rect.y = y;
    module.widgets.close->rect.y = y;
#else
    module.close_bg_item.rect.y = y;
    module.close_item.rect.y = y;
#endif

    int active = module.active_button;
    int hover = module.hover_button;

    fsemu_gui_item_set_visible(&module.background_item,
                               is_visible && is_static);
    module.background_item.color = window_is_active
                                       ? module.background_color
                                       : module.background_color_inactive;

    fsemu_gui_item_set_visible(&module.background_ws_item,
                               is_visible && !is_static);

    fsemu_gui_item_set_visible(&module.title_item, is_visible);
    // module.title_item.color =
    //     window_is_active ? module.title_color
    //                      : module.title_color_inactive;
    module.title_item.color = opacity_color;

    fsemu_gui_item_set_visible(&module.maximize_item, is_visible);
    fsemu_gui_item_set_visible(&module.menu_item, is_visible);
    fsemu_gui_item_set_visible(&module.minimize_item, is_visible);
#if 0
    fsemu_gui_item_set_visible(module.widgets.close, is_visible);
#else
    fsemu_gui_item_set_visible(&module.close_item, is_visible);
#endif

    module.maximize_item.color = opacity_color;
    module.menu_item.color = opacity_color;
    module.minimize_item.color = opacity_color;
#if 0
    module.widgets.close->color = opacity_color;
#else
    module.close_item.color = opacity_color;
#endif

    fsemu_gui_item_set_visible(&module.maximize_bg_item,
                               is_visible && hover == FSEMU_TITLEBAR_MAXIMIZE);
    module.maximize_bg_item.color = active == FSEMU_TITLEBAR_MAXIMIZE
                                        ? FSEMU_TITLEBAR_MAXIMIZE_ACTIVE
                                        : FSEMU_TITLEBAR_MAXIMIZE_HOVER;

    fsemu_gui_item_set_visible(&module.menu_bg_item,
                               is_visible && hover == FSEMU_TITLEBAR_MENU);
    module.menu_bg_item.color = active == FSEMU_TITLEBAR_MENU
                                    ? FSEMU_TITLEBAR_MENU_ACTIVE
                                    : FSEMU_TITLEBAR_MENU_HOVER;

    fsemu_gui_item_set_visible(&module.minimize_bg_item,
                               is_visible && hover == FSEMU_TITLEBAR_MINIMIZE);
    module.minimize_bg_item.color = active == FSEMU_TITLEBAR_MINIMIZE
                                        ? FSEMU_TITLEBAR_MINIMIZE_ACTIVE
                                        : FSEMU_TITLEBAR_MINIMIZE_HOVER;

#if 0
    fsemu_gui_item_set_visible(module.widgets.close_bg,
                               is_visible && hover == FSEMU_TITLEBAR_CLOSE);
    module.widgets.close_bg->color = active == FSEMU_TITLEBAR_CLOSE
                                             ? FSEMU_TITLEBAR_CLOSE_ACTIVE
                                             : FSEMU_TITLEBAR_CLOSE_HOVER;
#else
    fsemu_gui_item_set_visible(&module.close_bg_item,
                               is_visible && hover == FSEMU_TITLEBAR_CLOSE);
    module.close_bg_item.color = active == FSEMU_TITLEBAR_CLOSE
                                     ? FSEMU_TITLEBAR_CLOSE_ACTIVE
                                     : FSEMU_TITLEBAR_CLOSE_HOVER;
#endif
}

// ---------------------------------------------------------------------------

void fsemu_titlebar_set_width(int w)
{
    module.width = w;

    module.background_item.rect.w = w;
    module.background_item.dirty = true;

    module.background_ws_item.rect.w = w;
    module.background_ws_item.dirty = true;

    module.maximize_bg_item.rect.x = w - module.height * 2;
    module.maximize_item.rect.x = w - module.height * 2;

    module.menu_bg_item.rect.x = 0;
    module.menu_item.rect.x = 0;

    module.minimize_bg_item.rect.x = w - module.height * 3;
    module.minimize_item.rect.x = w - module.height * 3;

#if 0
    module.widgets.close_bg->rect.x = w - module.height * 1;
    module.widgets.close->rect.x = w - module.height * 1;
#else
    module.close_bg_item.rect.x = w - module.height * 1;
    module.close_item.rect.x = w - module.height * 1;
#endif
}

// ---------------------------------------------------------------------------

static void fsemu_titlebar_set_visible(bool visible)
{
    module.visible = visible;
    // fsemu_gui_item_set_visible(&module.background_item, visible);
    /*
    fsemu_gui_item_set_visible(&module.background_ws_item, visible);
    fsemu_gui_item_set_visible(&module.title_item, visible);
    fsemu_gui_item_set_visible(&module.close_bg_item, visible);
    fsemu_gui_item_set_visible(&module.close_item, visible);
    */
}

// ---------------------------------------------------------------------------

static int fsemu_titlebar_button_test(fsemu_mouse_event_t *event)
{
    if (event->y >= 0 && event->y < module.height) {
        if (event->x < module.height * 1) {
            return FSEMU_TITLEBAR_MENU;
        }
        if (event->x > module.width - module.height * 1) {
            return FSEMU_TITLEBAR_CLOSE;
        }
        if (event->x > module.width - module.height * 2) {
            return FSEMU_TITLEBAR_MAXIMIZE;
        }
        if (event->x > module.width - module.height * 3) {
            return FSEMU_TITLEBAR_MINIMIZE;
        }
    }
    return 0;
}

bool fsemu_titlebar_want_cursor(void)
{
    return module.want_cursor;
}

#include "fsemu-sdl.h"
#include "fsemu-sdlwindow.h"

static void fsemu_titlebar_capture_mouse(bool capture)
{
    module.mouse_trapped = capture;
    // Breaking the abstraction here, but might be important to do
    // this synchronously/now.
    SDL_CaptureMouse(capture ? SDL_TRUE : SDL_FALSE);
    module.mouse_trapped = capture;
}

static void fsemu_titlebar_on_menu_button(void)
{
    printf("on menu\n");
    fsemu_osmenu_toggle_open();
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
    // bool is_open = module.visible;
    // FIXME: Check mouse grab / relative mode
#if 0
    printf(
        "fsemu_titlebar_handle_mouse visible=%d y<height=%d "
        "trapped=%d\n",
        module.visible,
        event->y < module.height,
        module.mouse_trapped);
#endif

    if (fsemu_mouse_captured()) {
        module.want_cursor = false;
        return false;
    }

    bool visible = fsemu_titlebar_visible();

    if ((visible && event->y < module.height) || module.mouse_trapped) {
        module.want_cursor = true;
        if (event->moved) {
            if (module.mouse_trapped) {
                // Do not set hover_button while left mouse button is
                // pressed
            } else {
                module.hover_button = fsemu_titlebar_button_test(event);
            }
            // } else if (event->left_window) {  FIXME!

        } else if (event->pressed == 1) {
            module.active_button = fsemu_titlebar_button_test(event);
            /*
            if (module.active_button) {
                module.hover_button =
            module.active_button
            }
            */
            fsemu_titlebar_capture_mouse(true);
        } else if (event->released == 1) {
            int active = module.active_button;
            if (active && active == fsemu_titlebar_button_test(event)) {
                if (active == FSEMU_TITLEBAR_CLOSE) {
                    fsemu_titlebar_on_close_button();
                } else if (active == FSEMU_TITLEBAR_MAXIMIZE) {
                    fsemu_titlebar_on_maximize_button();
                } else if (active == FSEMU_TITLEBAR_MENU) {
                    fsemu_titlebar_on_menu_button();
                } else if (active == FSEMU_TITLEBAR_MINIMIZE) {
                    fsemu_titlebar_on_minimize_button();
                }
            }
            // printf("release trap mode\n");
            module.active_button = 0;
            fsemu_titlebar_capture_mouse(false);
            // Check if we are now hovering above another element, when
            // the button was released
            module.hover_button = fsemu_titlebar_button_test(event);
        }
    }

    if (fsemu_titlebar_static() && event->y > fsemu_titlebar_height() &&
        !module.mouse_trapped) {
        module.hover_button = FSEMU_TITLEBAR_NONE;
    }

    // We re-run the check because mouse_trapped may have changed
    if ((visible && event->y < module.height) || module.mouse_trapped) {
        return true;
    }

    if (event->y == 0 && !event->button) {
        fsemu_titlebar_log("Opening title bar (x=%d y=%d button=%d)\n",
                           event->x,
                           event->x,
                           event->button);
        fsemu_titlebar_set_visible(true);
        module.want_cursor = true;
        return true;
    }

    if (module.visible) {
        fsemu_titlebar_log("Closing title bar\n");
        fsemu_titlebar_set_visible(false);
    }

    module.want_cursor = false;

    if (event->moved && event->x == -1 && event->y == -1) {
        fsemu_titlebar_log("Mouse left window, stopping propagation\n");
        return true;
    }
    return false;
#if 0
    if (event->button >= 0) {
        if (event->state) {
            // Mouse button press
            if (event->y < module.height) {
                if (!module.mouse_trapped) {
                    module.mouse_trapped = 1 + event->button;
                }
            }
        } else {
            // Mouse button release
            if (module.mouse_trapped == 1 + event->button) {
                module.mouse_trapped = 0;
                if (event->y >= module.height) {
                    module.visible = false;
                    return false;
                }
            }
        }
    } else {
        // Mouse motion
        if (module.mouse_trapped) {
            //
            return true;
        } else if (is_open) {
            if (event->y >= module.height) {
                module.visible = false;
                return false;
            }
            return true;
        } else {
            if (event->y == 0) {
                fsemu_titlebar_log("Opening title bar");
                module.visible = true;
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

static void fsemu_titlebar_init_container(void)
{
#if 0
    module.widgets.container =
        fsemu_widget_new_with_name("fsemu:titlebar:container");
    fsemu_widget_t *widget = module.widgets.container;

    fsemu_widget_set_top_2(widget, 0, FSEMU_WIDGET_WINDOW_TOP);
    fsemu_widget_set_right_2(widget, 0, FSEMU_WIDGET_WINDOW_RIGHT);
    fsemu_widget_set_bottom_2(widget, 40, FSEMU_WIDGET_SCREEN_TOP);
    fsemu_widget_set_left_2 (widget, 0, FSEMU_WIDGET_SCREEN_RIGHT);
    // FIXME: FSEMU_LAYER_OSMENU
    fsemu_widget_set_z_index(widget, FSEMU_LAYER_OSMENU);

    // Transparent container widget. Only used to provide position/size and
    // visibility toggling for children.
    fsemu_widget_set_color(widget, FSEMU_COLOR_RGB(0xff0000));
    // The on-screen menu container determinies whether the entire menu
    // is visible. Initially hidden.
    fsemu_widget_set_visible(widget, true);
    // This widget is added to the top level.
    fsemu_gui_add_item(widget);
#endif
}

static void fsemu_titlebar_init_widgets(void)
{
    fsemu_gui_item_t *item;
    // fsemu_image_t *image;

    item = &module.background_item;
    // titlebar_color = FSEMU_COLOR_RGBA(0xff202080);
    fsemu_gui_rectangle(item, 0, 0, 0, module.height, module.background_color);
    item->z_index = 9000;
    fsemu_gui_add_item(item);

    // With shadow
    item = &module.background_ws_item;
    module.background_ws_image =
        fsemu_image_load("Images/TitleBarWithShadow.png");
    fsemu_gui_image(item,
                    0,
                    0,
                    1000,
                    module.background_ws_image->height,
                    module.background_ws_image);
    item->z_index = 9000;
    fsemu_gui_add_item(item);

#if 0
    double ui_scale = fsemu_window_ui_scale();

    item = &module.title_item;
    // FIXME: Semi-bold ?
    fsemu_font_t *font = fsemu_font_load("Fonts/SairaCondensed-SemiBold.ttf", 19 * ui_scale);
    // 0xcc is the same alpha value as used by the window control icons
    // for the custom frame.
    image = fsemu_font_render_text_to_image(
        font, fsemu_emulator_name(), module.title_color);
    fsemu_gui_image(item, 20 * ui_scale, 0, image->width, image->height, image);
    item->z_index = 9001;
    fsemu_gui_add_item(item);
#endif

    item = &module.menu_bg_item;
    fsemu_gui_rectangle(item, 0, 0, 40, 40, 0);
    item->z_index = 9001;
    fsemu_gui_add_item(item);
    item = &module.menu_item;
    module.menu_icon = fsemu_image_load("Images/TitleBarMenuIcon.png");
    fsemu_gui_image(item,
                    0,
                    0,
                    module.menu_icon->width,
                    module.menu_icon->height,
                    module.menu_icon);
    item->z_index = 9002;
    fsemu_gui_add_item(item);

    item = &module.minimize_bg_item;
    fsemu_gui_rectangle(item, 0, 0, 40, 40, 0);
    item->z_index = 9001;
    fsemu_gui_add_item(item);
    item = &module.minimize_item;
    module.minimize_icon = fsemu_image_load("Images/TitleBarMinimizeIcon.png");
    fsemu_gui_image(item,
                    0,
                    0,
                    module.minimize_icon->width,
                    module.minimize_icon->height,
                    module.minimize_icon);
    item->z_index = 9002;
    fsemu_gui_add_item(item);

    item = &module.maximize_bg_item;
    fsemu_gui_rectangle(item, 0, 0, 40, 40, 0);
    item->z_index = 9001;
    fsemu_gui_add_item(item);
    item = &module.maximize_item;
    module.maximize_icon = fsemu_image_load("Images/TitleBarMaximizeIcon.png");
    fsemu_gui_image(item,
                    0,
                    0,
                    module.maximize_icon->width,
                    module.maximize_icon->height,
                    module.maximize_icon);
    item->z_index = 9002;
    fsemu_gui_add_item(item);

#if 0
    fsemu_widget_t *widget;

    module.widgets.close = fsemu_widget_new_with_name("fsemu:titlebar:close");
    widget = module.widgets.close;
    image = fsemu_image_load("Images/TitleBarCloseIcon.png");
    fsemu_gui_image(widget, 0, 0, image->width, image->height, image);
    fsemu_widget_set_z_index(widget, 9002);
    fsemu_gui_add_item(widget);

    module.widgets.close_bg = fsemu_widget_new_with_name("fsemu:titlebar:close_bg");
    widget = module.widgets.close_bg;
    fsemu_gui_rectangle(widget, 0, 0, 40, 40, 0);
    fsemu_widget_set_z_index(widget, 9001);
    fsemu_gui_add_item(widget);
#else
    item = &module.close_bg_item;
    fsemu_gui_rectangle(item, 0, 0, 40, 40, 0);
    item->z_index = 9001;
    fsemu_gui_add_item(item);
    item = &module.close_item;
    module.close_icon = fsemu_image_load("Images/TitleBarCloseIcon.png");
    fsemu_gui_image(item,
                    0,
                    0,
                    module.close_icon->width,
                    module.close_icon->height,
                    module.close_icon);
    item->z_index = 9002;
    fsemu_gui_add_item(item);
#endif
}

static void fsemu_titlebar_quit(void)
{
    fsemu_titlebar_log("fsemu_titlebar_quit\n");

    fsemu_image_unref(module.background_ws_image);
    fsemu_image_unref(module.close_icon);
    fsemu_image_unref(module.maximize_icon);
    fsemu_image_unref(module.menu_icon);
    fsemu_image_unref(module.minimize_icon);
    fsemu_image_unref(module.title_image);
}

void fsemu_titlebar_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_gui_init();
    fsemu_titlebar_log("Initializing titlebar module\n");
    fsemu_module_on_quit(fsemu_titlebar_quit);

    // fsemu_window_init();

    fsemu_option_read_bool_default(
        FSEMU_OPTION_SYSTEM_TITLEBAR, &module.use_system, false);
    // module.use_system = false;

    module.width = 640;  // FIXME
    module.height = fsemu_titlebar_unscaled_height();

    module.background_color = FSEMU_TITLEBAR_BACKGROUND;
    module.background_color_inactive = FSEMU_TITLEBAR_BACKGROUND_INACTIVE;

    module.title_color = FSEMU_COLOR_RGBA(0xffffffcc);
    // FIXME: This does not have any effect yet, since text is only rendered
    // to image initially.
    // module.title_color_inactive = FSEMU_COLOR_RGBA(0xffffff99);
    // Can use opacity though
    module.title_opacity_inactive = 0x80;

    // background = fsemu_gui_create_rectangle(0, 0, 10000, 40);
    // fsemu_gui_rectangle(&module.background, 0, 0, 1000, 40,
    // 0x181818); fsemu_gui_rectangle(&module.background, 0, 0, 1000,
    // 40, 0x202020);
    // fsemu_gui_rectangle(
    //     &module.background, 0, 0, 1000, 40, 0x20303030);

    fsemu_titlebar_init_container();
    fsemu_titlebar_init_widgets();
    fsemu_titlebar_update();
}

#undef module
