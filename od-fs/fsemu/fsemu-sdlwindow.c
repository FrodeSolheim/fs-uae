#define FSEMU_INTERNAL
#include "fsemu-sdlwindow.h"

#ifdef FSEMU_SDL

#include "fsemu-action.h"
#include "fsemu-control.h"
#include "fsemu-frame.h"
#include "fsemu-input.h"
#include "fsemu-layout.h"
#include "fsemu-module.h"
#include "fsemu-monitor.h"
#include "fsemu-mouse.h"
#include "fsemu-opengl.h"
#include "fsemu-perfgui.h"
#include "fsemu-quit.h"
#include "fsemu-screenshot.h"
#include "fsemu-sdlinput.h"
#include "fsemu-startupinfo.h"
#include "fsemu-time.h"
#include "fsemu-titlebar.h"
#include "fsemu-types.h"
#include "fsemu-util.h"
#include "fsemu-video.h"
#include "fsemu-window.h"

// #ifdef FSUAE
// #include <fs/emu/input.h>
// #endif

// ----------------------------------------------------------------------------

static struct {
    bool initialized;

    SDL_Window* window;
    fsemu_rect_t rect;
    bool fullscreen;
    int swap_interval;
    bool was_fullscreen_initially;
    bool was_borderless_initially;

    bool f12_pressed;
    int64_t mod_press_release_at;
    bool full_keyboard_emulation;
    SDL_GLContext gl_context;

    // FIXME: Might have to separate between mouse captured and
    // mouse captured+relative.
    bool mouse_captured;
    bool cursor_visible;
    int64_t last_cursor_motion_at;
    // When switching to fullscreen (on Linux at least) without moving the
    // mouse first, we get a motion event with x=0 y=0 xrel=0 yrel=0 right
    // after to switching to fullscreen. This does not reflect the true
    // location of the cursor. For now, we ignore this. Later, we might want
    // to construct a synthentic event based on the true location.
    bool ignore_motion_after_fullscreen;

    SDL_Cursor* current_cursor;
    SDL_Cursor* default_cursor;
    SDL_Cursor* size_we_cursor;
    SDL_Cursor* size_ns_cursor;
    SDL_Cursor* size_nesw_cursor;
    SDL_Cursor* size_nwse_cursor;

    uint32_t frame_rendered_event_type;
    uint32_t quit_event_type;

    bool no_event_polling;
} fsemu_sdlwindow;

void fsemu_sdlwindow_hack(SDL_Window* window) {
    fsemu_sdlwindow.window = window;
}

// FIXME: Replace
bool fsemu_sdlwindow_full_keyboard_emulation(void) {
    return fsemu_sdlwindow.full_keyboard_emulation;
}

// ----------------------------------------------------------------------------

#if defined(FSEMU_OS_WINDOWS)
#elif defined(FSEMU_OS_MACOS)
#else

// Wrapper function in case SDL_SetCursor is not optimized for the case
// where you set the same cursor as the existing one.
static void fsemu_sdlwindow_set_cursor(SDL_Cursor* cursor) {
    if (fsemu_sdlwindow.current_cursor == cursor) {
        return;
    }
    SDL_SetCursor(cursor);
    fsemu_sdlwindow.current_cursor = cursor;
}

#endif

// ----------------------------------------------------------------------------

bool fsemu_sdlwindow_no_event_polling(void) {
    return fsemu_sdlwindow.no_event_polling;
}

// ----------------------------------------------------------------------------

void fsemu_sdlwindow_set_no_event_polling(bool no_event_polling) {
    fsemu_sdlwindow.no_event_polling = no_event_polling;
}

// ----------------------------------------------------------------------------

void fsemu_sdlwindow_work(int timeout_ms) {
    if (timeout_ms == -1) {
        timeout_ms = INT_MAX;
    } else {
        fsemu_assert(timeout_ms >= 0);
    }
    if (!fsemu_sdlwindow_no_event_polling()) {
        SDL_Event event;
        if (SDL_WaitEventTimeout(&event, timeout_ms)) {
            fsemu_sdlwindow_handle_event(&event);
        }
        while (SDL_PollEvent(&event)) {
            fsemu_sdlwindow_handle_event(&event);
        }
    }
}

// Internal function called by fsemu_sdlwindow_update.
static void fsemu_sdlwindow_set_cursor_visible(int cursor_visible) {
    if (fsemu_sdlwindow.cursor_visible == cursor_visible) {
        return;
    }
    if (cursor_visible) {
        fsemu_mouse_log_debug("Showing mouse cursor\n");
    } else {
        fsemu_mouse_log_debug("Hiding mouse cursor\n");
    }

    fsemu_mouse_log_debug("Cursor set visible? %d\n", cursor_visible);
    SDL_ShowCursor();
    fsemu_sdlwindow.cursor_visible = cursor_visible;
}

// Internal function called by fsemu_sdlwindow_update.
static void fsemu_sdlwindow_set_swap_interval(int swap_interval) {
    if (fsemu_sdlwindow.swap_interval == swap_interval) {
        return;
    }
    fsemu_window_log("SDL_GL_SetSwapInterval(%d)%s\n", swap_interval,
                     swap_interval ? " (vsync)" : "");
    SDL_GL_SetSwapInterval(swap_interval);
    fsemu_sdlwindow.swap_interval = swap_interval;
}

// Internal function called by fsemu_sdlwindow_update.
static void fsemu_sdlwindow_set_mouse_captured(int mouse_captured) {
    if (mouse_captured == fsemu_sdlwindow.mouse_captured) {
        return;
    }
    if (mouse_captured) {
        fsemu_mouse_log_debug("Capturing mouse cursor\n");
    } else {
        fsemu_mouse_log_debug("Releasing mouse cursor capture\n");
    }
    SDL_SetWindowRelativeMouseMode(fsemu_sdlwindow.window, mouse_captured ? true : false);
    fsemu_sdlwindow.mouse_captured = mouse_captured;
}

// Internal function called by fsemu_sdlwindow_update.
static bool fsemu_sdlwindow_want_cursor(int64_t now) {
    bool want_cursor = false;
    if (fsemu_sdlwindow.current_cursor != fsemu_sdlwindow.default_cursor) {
        // When we're showing resize cursors, we want it to remain visible.
        want_cursor = true;
    } else if (now - fsemu_sdlwindow.last_cursor_motion_at < 1500000) {
        // printf("MOUSE last cursor motion!\n");
        want_cursor = true;
    }
    return want_cursor;
}

static void fsemu_sdlwindow_open_menu(void) {
    fsemu_input_process_action(FSEMU_ACTION_OSMENU, FSEMU_ACTION_STATE_MAX);
    fsemu_input_process_action(FSEMU_ACTION_OSMENU, 0);
}

static void fsemu_sdlwindow_open_menu_delayed(void) {
    // We need to wait a bit and see if this window looses focus in order
    // to conclude whether Mod+Tab was pressed or not.
    fsemu_sdlwindow.mod_press_release_at = fsemu_time_us();
}

static void fsemu_sdlwindow_open_menu_abort(void) {
    fsemu_window_log_debug("fsemu_sdlwindow_open_menu_abort\n");
    fsemu_sdlwindow.mod_press_release_at = 0;
}

static inline void fsemu_sdlwindow_open_menu_maybe(int64_t now_us) {
    if (fsemu_sdlwindow.mod_press_release_at) {
        if (now_us - fsemu_sdlwindow.mod_press_release_at > 10 * 1000) {
            fsemu_window_log_debug("fsemu_sdlwindow_open_maybe -> yes\n");
            // 50 ms has passed without the window having lost focus.
            // Let's assume the modifier key was pressed/released alone.
            fsemu_sdlwindow_open_menu();
            fsemu_sdlwindow.mod_press_release_at = 0;
        }
    }
}

void fsemu_sdlwindow_update(void) {
    int64_t now_us = fsemu_time_us();

    fsemu_sdlwindow_open_menu_maybe(now_us);

    // FIXME: The cursor-visible code should be moved to fsemu_window
    // or fsemu_mouse..

    bool want_fullscreen = fsemu_window_fullscreen();
    fsemu_sdlwindow_set_fullscreen(want_fullscreen);

    bool want_mouse_captured = fsemu_mouse_captured();
    fsemu_sdlwindow_set_mouse_captured(want_mouse_captured);

    int want_swap_interval = fsemu_video_vsync() > 0;
    fsemu_sdlwindow_set_swap_interval(want_swap_interval);

    bool want_cursor = fsemu_sdlwindow_want_cursor(now_us);
    if (fsemu_titlebar_want_cursor()) {
        want_cursor = true;
        // So we get the nice delay after we no longer want the cursor for
        // the titlebar. Actually, that doesn't make sense, the delay must
        // be calculated from when we no longer want the cursor.
        // fsemu_sdlwindow.last_cursor_motion_at = now;
    }
    fsemu_sdlwindow_set_cursor_visible(want_cursor);

    if (fsemu_frame_counter_mod(50) == 0 && fsemu_frame_emutime_avg_us()) {
        char title[512];
        snprintf(title, 512, "FSEMU [%d fps max]", 1000000 / fsemu_frame_emutime_avg_us());
        SDL_SetWindowTitle(fsemu_sdlwindow.window, title);
    }
}

// See if mouse should interact with a special area. Factored out into a
// separate function so this can be called directly and also from SDL
// hit test callback.
static SDL_HitTestResult fsemu_sdlwindow_hit_test_2(SDL_Window* window, int x, int y) {
    Uint32 flags = SDL_GetWindowFlags(window);
    int window_w, window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);
    bool fullscreen = flags & SDL_WINDOW_FULLSCREEN;
    if (fullscreen) {
        return SDL_HITTEST_NORMAL;
    }
    if (fsemu_mouse_captured()) {
        return SDL_HITTEST_NORMAL;
    }
    int titlebar_height = fsemu_titlebar_height();
#ifdef FSEMU_OS_MACOS
    // Resize controls are available by default, just need to add support for
    // dragging the title bar.
    if (y < titlebar_height) {
        if (x < window_w - 120) {
            return SDL_HITTEST_DRAGGABLE;
        }
    }
#else
    int corner_size = 40;
    int edge_width = 10;
    if (y < titlebar_height) {
        // FIXME: Call into titlebar module for button hit test
        if (x > 40 && x < window_w - 120 * fsemu_window_ui_scale()) {
            if (y < edge_width) {
                return SDL_HITTEST_RESIZE_TOP;
            }
            return SDL_HITTEST_DRAGGABLE;
        }
    } else if (x < edge_width) {
        if (y >= window_h - corner_size) {
            return SDL_HITTEST_RESIZE_BOTTOMLEFT;
        }
        return SDL_HITTEST_RESIZE_LEFT;
    } else if (x >= window_w - edge_width) {
        if (y >= window_h - corner_size) {
            return SDL_HITTEST_RESIZE_BOTTOMRIGHT;
        }
        return SDL_HITTEST_RESIZE_RIGHT;
    } else if (y >= window_h - edge_width) {
        if (x < corner_size) {
            return SDL_HITTEST_RESIZE_BOTTOMLEFT;
        } else if (x >= window_w - corner_size) {
            return SDL_HITTEST_RESIZE_BOTTOMRIGHT;
        }
        return SDL_HITTEST_RESIZE_BOTTOM;
    }
#endif
    return SDL_HITTEST_NORMAL;
}

// Called by SDL to see if mouse should interact with a special area.
static SDL_HitTestResult fsemu_sdlwindow_hit_test(SDL_Window* window, const SDL_Point* point,
                                                  void* data) {
    // fsemu_window_log("Hit test x=%d y=%d\n", point->x, point->y);
    return fsemu_sdlwindow_hit_test_2(window, point->x, point->y);
}

/*
static void fsemu_sdlwindow_find_mode(int display_index, SDL_DisplayMode *mode)
{
    fsemu_window_log("Finding suitable mode for display %d:\n", display_index);
    SDL_DisplayMode m = {SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0};
    int num_display_modes = SDL_GetNumDisplayModes(display_index);
    // Hack - Try to find an 1080p@50 and use that
    for (int i = 0; i < num_display_modes; i++) {
        int error = SDL_GetDisplayMode(display_index, i, &m);
        if (error) {
            break;
        }
        if (m.w == 1920 && m.h == 1080 && m.refresh_rate == 50) {
            *mode = m;
            return;
        }
    }
    // Hack (2) - fallback to 720p@50 if found
    for (int i = 0; i < num_display_modes; i++) {
        int error = SDL_GetDisplayMode(display_index, i, &m);
        if (error) {
            break;
        }
        if (m.w == 1280 && m.h == 720 && m.refresh_rate == 50) {
            *mode = m;
            return;
        }
    }
}
*/

static bool fsemu_sdlwindow_kmsdrm(void) {
    static bool result;
    static int initialized;
    if (!initialized) {
        const char* driver = SDL_GetCurrentVideoDriver();
        result = strcmp(driver, "KMSDRM") == 0;
        // printf("%s %d\n", driver, result);
        // exit(1);
    }
    return result;
}

SDL_Window* fsemu_sdlwindow_create(void) {
    fsemu_assert(fsemu_sdlwindow.window == NULL);
#if 0
    if (fsemu_sdlwindow_kmsdrm()) {
        // Move cursor away from the top-left corner, we do not want to start
        // with the drop-down titlebar visible.
        // fsemu_sdlwindow_center_cursor();
        // SDL_WarpMouseGlobal(rect.w / 2, rect.h / 2);
        SDL_WarpMouseGlobal(100, 100);
    }
#endif
    // In case we want to start with the cursor not visible, we hide it before
    // window creation to avoid having the cursor flicker.
    SDL_HideCursor();

    int flags = 0;
    // If we want to have an OpenGL-powered renderer later on, it is better to
    // specify SDL_WINDOW_OPENGL right now, to avoid "flickering" due to window
    // being recreated later (?) by SDL.
    flags |= SDL_WINDOW_OPENGL;
    // We always want a resizable window. Annoying for users without.
    flags |= SDL_WINDOW_RESIZABLE;
#if 1
    // Create the window hidden initially, and then show later via
    // fsemu_sdlwindow_show.
    flags |= SDL_WINDOW_HIDDEN;
#endif

    // Since we do not (necessarily) know what monitor the window will be
    // displayed on (?), we for now try to find display scale only for
    // monitor 0 for determinining window size
    SDL_Rect display_bounds;
    double ui_scale = 1.0;
    if (SDL_GetDisplayBounds(0, &display_bounds) == 0) {
        fsemu_monitor_t monitor;
        if (fsemu_monitor_get_by_rect(&display_bounds, &monitor)) {
            // rect.w *= monitor.scale;
            // rect.h *= monitor.scale;
            ui_scale = monitor.scale;
            // fsemu_window_log("Scale window size by %0.2f => %dx%d\n",
            //                  monitor.scale,
            //                  rect.w,
            //                  rect.h);
        }
    }

    if (fsemu_sdlwindow_kmsdrm()) {
        // flags &= ~SDL_WINDOW_FULLSCREEN_DESKTOP;
        flags |= SDL_WINDOW_FULLSCREEN;
        fsemu_window_set_fullscreen(true);
    }

    fsemu_rect_t rect;
    if (fsemu_window_fullscreen()) {
        flags |= SDL_WINDOW_FULLSCREEN;
        fsemu_sdlwindow.fullscreen = true;
        fsemu_sdlwindow.was_fullscreen_initially = true;
#if 0
        // Using borderless window here to make the fullscreen window appear
        // more smoothly and without resizing animation.
        flags |= SDL_WINDOW_BORDERLESS;
        fsemu_sdlwindow.was_borderless_initially = true;
#endif
        fsemu_window_initial_fullscreen_rect(&rect);
    } else {
        fsemu_window_initial_rect(&rect, ui_scale);
        if (rect.x == -1 && rect.y == -1) {
            rect.x = SDL_WINDOWPOS_CENTERED;
            rect.y = SDL_WINDOWPOS_CENTERED;
        }
    }
    fsemu_window_log("Initial window rect: %dx%d +%d+%d\n", rect.w, rect.h, rect.x, rect.y);

    if (!fsemu_titlebar_use_system()) {
        fsemu_window_log("System titlebar is false: borderless window\n");
        flags |= SDL_WINDOW_BORDERLESS;
        // if (fsemu_window_fullscreen()) {
        // } else {
        //     rect.h += fsemu_titlebar_height();
        //     fsemu_window_log("Initial window rect (final): %dx%d +%d+%d\n",
        //                      rect.w,
        //                      rect.h,
        //                      rect.x,
        //                      rect.y);
        // }
    }

    // FIXME: Initial input grab?

#ifdef FSEMU_OS_MACOS
    // Sometimes, it seems we get a high-DPI window even though we didn't
    // request it, causing problems for glViewport, and video output
    // being limited to a quarter of the display. So instead, we explicitly
    // request a high-DPI window and support that.
    flags |= SDL_WINDOW_HIGH_PIXEL_DENSITY;

    // TODO: Test (and enable?) on Windows as well?
#else
    flags |= SDL_WINDOW_HIGH_PIXEL_DENSITY;
#endif

    SDL_DisplayMode fullscreen_mode = {0};

    /*
    if (fsemu_sdlwindow_kmsdrm()) {
        // We must use SDL_WINDOW_FULLSCREEN in order for mode switch to
        // happen. And no reason to not use SDL_WINDOW_FULLSCREN with KMSDRM.
        // flags &= ~SDL_WINDOW_FULLSCREEN_DESKTOP;
        flags |= SDL_WINDOW_FULLSCREEN;

        // Switch to 50Hz mode on Raspberry PI.

        // FIXME: Lookup correct mode, do not hardcode index
        // SDL_GetDisplayMode(0, 1, &mode);
        fsemu_sdlwindow_find_mode(0, &fullscreen_mode);
        // Also setting vsync. FIXME: Unconditionally?
        if (fullscreen_mode.refresh_rate == 50) {
            fsemu_video_set_vsync(true);
        }
    }
    */

    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

    if (fullscreen_mode.refresh_rate == 50) {
        fsemu_video_set_vsync(true);
        rect.x = 0;
        rect.y = 0;
        rect.w = fullscreen_mode.w;
        rect.h = fullscreen_mode.h;
    }

    // FIXME: There seems to be a bug (?) with SDL on X11. If you specify
    // fullscreen desktop window *without decorations*, the position seems to
    // ignored and it always opens on the same monitor (for me, always the
    // right-most one).
    // bool borderless_fullscreen_workaround = false;
    // if ((flags & SDL_WINDOW_FULLSCREEN_DESKTOP) && (flags &
    // SDL_WINDOW_BORDERLESS)) {
    //     flags = flags & ~SDL_WINDOW_BORDERLESS;
    //     // borderless_fullscreen_workaround = true;
    // }

    const char* title = fsemu_window_title();
    fsemu_window_log("SDL_CreateWindow(\"%s\", %d, %d, flags=%d)\n", title, rect.w, rect.h, flags);
    // flags = flags & ~SDL_WINDOW_FULLSCREEN_DESKTOP;

    SDL_Window* window = SDL_CreateWindow(title, rect.w, rect.h, flags);
    fsemu_window_log("Window %p (Driver: %s)\n", window, SDL_GetCurrentVideoDriver());

    // if (borderless_fullscreen_workaround) {
    //     SDL_SetWindowBordered(window, false);
    // }

    // SDL_SetWindowPosition(window, 1, 1);
    // SDL_SetWindowPosition(window, 0, 0);
    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    // SDL_SetWindowSize(window, 1918, 1080);

#if 1
    if (fullscreen_mode.refresh_rate) {
        fsemu_window_log("Switching to mode %dx%d@%d\n", fullscreen_mode.w, fullscreen_mode.h,
                         fullscreen_mode.refresh_rate);
        int error = SDL_SetWindowFullscreenMode(window, &fullscreen_mode);
        if (error) {
            // FIXME: log_error or log_warning
            fsemu_window_log("Error setting mode %dx%d@%d\n", fullscreen_mode.w, fullscreen_mode.h,
                             fullscreen_mode.refresh_rate);
        }
    }
#endif

#if 1
    // FIXME: Should find correct ui_scale now?
    int display_index = SDL_GetDisplayForWindow(window);
    if (SDL_GetDisplayBounds(display_index, &display_bounds) == 0) {
        fsemu_monitor_t monitor;
        if (fsemu_monitor_get_by_rect(&display_bounds, &monitor)) {
            fsemu_window_log("Using scale %0.2f\n", monitor.scale);
            fsemu_window_set_ui_scale(monitor.scale);
        }
    }
#endif

    fsemu_sdlwindow_set_window(window);
    /*
        if (fsemu_custom_frame()) {
            SDL_SetWindowBordered(window, false);
        }
    */
    fsemu_sdlwindow.rect.x = rect.x;
    fsemu_sdlwindow.rect.y = rect.y;
    fsemu_sdlwindow.rect.w = rect.w;
    fsemu_sdlwindow.rect.h = rect.h;

    fsemu_window_set_size_2(rect.w, rect.h);
    fsemu_layout_set_size_2(rect.w, rect.h);

    int min_width = 1920 / 4;
    int min_height = 1080 / 4;
    if (!fsemu_titlebar_use_system()) {
        if (fsemu_sdlwindow_kmsdrm()) {
            // SDL_SetWindowHitTest will fail (and is not needed)
        } else {
            int result = SDL_SetWindowHitTest(window, fsemu_sdlwindow_hit_test, NULL);
            if (result == -1) {
                fsemu_window_log("SDL_SetWindowHitTest failed: %s\n", SDL_GetError());
            }
        }
        min_height += fsemu_titlebar_height();
    }
    SDL_SetWindowMinimumSize(window, min_width, min_height);

    // FIXME: Not sure if this is decided yet?
    if (fsemu_video_get_renderer() == FSEMU_VIDEO_RENDERER_GL) {
        // FIXME: Needed?
        // SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        // We want context in main for startup rendering.
#if 0
        // FIXME: Don't think this is decided yet?
        if (fsemu_video_is_threaded()) {
            fsemu_window_log(
                "Threaded video: Not creating GL context in main thread\n");
        } else {
#endif
        fsemu_window_log("Creating OpenGL context from main/video thread\n");
        // FIXME: Only if using OpenGL
        fsemu_sdlwindow.gl_context = SDL_GL_CreateContext(fsemu_sdlwindow_window());

        // Check OpenGL properties
#ifdef FSEMU_GLAD
        fsemu_window_log("Loading OpenGL symbols via GLAD\n");
        gladLoadGLLoader(SDL_GL_GetProcAddress);
#endif
        fsemu_window_log("OpenGL vendor:   %s\n", glGetString(GL_VENDOR));
        fsemu_window_log("OpenGL renderer: %s\n", glGetString(GL_RENDERER));
        fsemu_window_log("OpenGL version:  %s\n", glGetString(GL_VERSION));
#if 0
        }
#endif
    }

    // Don't need to set viewport? Seems we get a window-covering viewport.
    // printf("glViewport 0, 0, %d, %d\n", rect.w, rect.h);
    // fflush(stdout);
    // glViewport(0, 0, rect.w, rect.h);

    fsemu_sdlwindow_set_swap_interval(fsemu_video_vsync());

#if 0
    if (fsemu_sdlwindow_kmsdrm()) {
        // Move cursor away from the top-left corner, we do not want to start
        // with the drop-down titlebar visible.
        // fsemu_sdlwindow_center_cursor();
        // SDL_WarpMouseGlobal(rect.w / 2, rect.h / 2);
        SDL_WarpMouseInWindow(window, rect.w / 2, rect.h / 2);
    }
#endif

    return window;
}

void fsemu_sdlwindow_show(void) {
    static bool first = true;
    fsemu_assert(fsemu_sdlwindow.window != NULL);
    fsemu_window_log_debug("Show window\n");
    SDL_ShowWindow(fsemu_sdlwindow.window);

    // Get actual window position and size
    SDL_GetWindowPosition(fsemu_sdlwindow.window, &fsemu_sdlwindow.rect.x, &fsemu_sdlwindow.rect.y);
    SDL_GetWindowSize(fsemu_sdlwindow.window, &fsemu_sdlwindow.rect.w, &fsemu_sdlwindow.rect.h);
    fsemu_window_log_debug("Window rect is now: %d %d %d %d\n", fsemu_sdlwindow.rect.x,
                           fsemu_sdlwindow.rect.y, fsemu_sdlwindow.rect.w, fsemu_sdlwindow.rect.h);

#if 0
    SDL_Event event;
    while (SDL_PollEvent(&event)) {

    }
#endif
    // if (first && fsemu_sdlwindow_kmsdrm()) {
    if (first && fsemu_sdlwindow.fullscreen) {
        // Move cursor away from the top-left corner, we do not want to start
        // with the drop-down titlebar visible.
        // fsemu_sdlwindow_center_cursor();
        // SDL_WarpMouseGlobal(100, 100);
        fsemu_window_log_debug("%d %d\n", fsemu_sdlwindow.rect.y, fsemu_sdlwindow.rect.h);
#if 1
#if 1
        //        SDL_ShowCursor(SDL_ENABLE);
        SDL_WarpMouseInWindow(fsemu_sdlwindow.window,
                              fsemu_sdlwindow.rect.x + fsemu_sdlwindow.rect.w / 2,
                              fsemu_sdlwindow.rect.y + fsemu_sdlwindow.rect.h / 2);
#endif
        // SDL_ShowCursor(SDL_DISABLE);
//        SDL_ShowCursor(SDL_DISABLE);
#endif
        // exit(1);
    }
    first = false;
}

// Called by FSEMU internally after then window has transitioned from
// fullscreen to  window, in order to work around shortcomings in SDL
// and restore window size properly the first time this happens. Also fixes
// a problem with restoring borderless windows on macOS.
static void fsemu_sdlwindow_fullscreen_to_window(void) {
#ifdef FSEMU_OS_MACOS
#if 1
    if (!fsemu_titlebar_use_system()) {
        // On macOS 10.14, going back to windowed mode seems to ignore the
        // bordered status (we get a titlebar) , and on macOS 10.15, it does
        // kind of work, but we get a rounded bordless window instead. In both
        // cases, we enable/disable borders to work around this.
        fsemu_window_log("Workaround fullscreen->window borderless on macOS\n");

        // fsemu_rect_t rect;
        // SDL_GetWindowPosition(fsemu_sdlwindow.window, &rect.x, &rect.y);
        // SDL_GetWindowSize(fsemu_sdlwindow.window, &rect.w, &rect.h);

        // SDL_SetWindowBordered(fsemu_sdlwindow.window, true);
        SDL_SetWindowBordered(fsemu_sdlwindow.window, false);

        // SDL_SetWindowPosition(fsemu_sdlwindow.window, rect.x, rect.y);
        // SDL_SetWindowSize(fsemu_sdlwindow.window, rect.w, rect.h);
        // fsemu_window_log("Window size is now %dx%d\n", rect.w, rect.h);
    }
#endif
#else
#endif
    if (!fsemu_sdlwindow.was_fullscreen_initially) {
        return;
    }
    // The rest of the code only runs first time we switch to windowed mode,
    // and only if the started the emulation in fullscreen mode. We do this
    // so we can set a sane window size. Otherwise, we often end up with a
    // maximized window, and SDL forgets the window dimentions we wanted for
    // the window in window mode.
    fsemu_window_log_debug("First fullscren -> window transition\n\n");
    fsemu_sdlwindow.was_fullscreen_initially = false;
    fsemu_rect_t rect;
    fsemu_window_initial_rect(&rect, fsemu_window_ui_scale());
    if (rect.x == -1 && rect.y == -1) {
        rect.x = SDL_WINDOWPOS_CENTERED;
        rect.y = SDL_WINDOWPOS_CENTERED;
    }

    if (fsemu_sdlwindow.was_borderless_initially) {
        // If we opened a fullscreen window borderless, we want
        // to add in borders now.
        SDL_SetWindowBordered(fsemu_sdlwindow.window, true);
        fsemu_sdlwindow.was_borderless_initially = false;
    }

    SDL_RestoreWindow(fsemu_sdlwindow.window);

    // We might have opened a fullscreen window with borders in order to
    // workaround a bug where the window is not opened on the correct monitor.
    // If so, make sure the borders are removed now.
    // EDIT: This seems to cause memory corruption and/or crash. Another
    // bug in SDL? Maybe a bit unusual use of SDL...
    // if (!fsemu_titlebar_use_system()) {
    //     fsemu_window_log(
    //         "Workaround fullscreen->window borderless\n");
    //     SDL_SetWindowBordered(fsemu_sdlwindow.window, false);
    // }

    SDL_SetWindowSize(fsemu_sdlwindow.window, rect.w, rect.h);
    fsemu_window_log_debug("Set position %d %d\n", rect.x, rect.y);
    SDL_SetWindowPosition(fsemu_sdlwindow.window, rect.x, rect.y);

#if 0
    // On Linux / GNOME, for some reason, when returning from initial
    // fullscreen mode to window mode, the window kind of loses focus.
    // FIXME: Only when using borderless fullscreen window hack
    SDL_RaiseWindow(fsemu_sdlwindow.window);
#endif
}

void fsemu_sdlwindow_set_window(SDL_Window* window) {
    fsemu_assert(!fsemu_sdlwindow.window);

    SDL_GetWindowPosition(window, &fsemu_sdlwindow.rect.x, &fsemu_sdlwindow.rect.y);
    SDL_GetWindowSize(window, &fsemu_sdlwindow.rect.w, &fsemu_sdlwindow.rect.h);

    fsemu_sdlwindow.window = window;

    Uint32 flags = SDL_GetWindowFlags(fsemu_sdlwindow.window);

    fsemu_sdlwindow.fullscreen = flags & SDL_WINDOW_FULLSCREEN;

    fsemu_window_log_debug("Window is %p (%d %d %d %d) fullscreen %d\n", fsemu_sdlwindow.window,
                           fsemu_sdlwindow.rect.x, fsemu_sdlwindow.rect.y, fsemu_sdlwindow.rect.w,
                           fsemu_sdlwindow.rect.h, fsemu_sdlwindow.fullscreen);
}

bool fsemu_sdlwindow_fullscreen(void) {
    return fsemu_sdlwindow.fullscreen;
}

void fsemu_sdlwindow_set_fullscreen(bool fullscreen) {
    if (fsemu_sdlwindow.fullscreen == fullscreen) {
        return;
    }
    fsemu_window_log_debug("fsemu_sdlwindow_set_fullscreen %d\n", fullscreen);
    int fullscreen_mode = 0;
#ifdef FSEMU_OS_MACOS
    if (!fsemu_titlebar_use_system() && fullscreen) {
        // On macOS 10.14, going back to windowed mode seems to ignore the
        // bordered status (we get a titlebar) , and on macOS 10.15, it does
        // kind of work, but we get a rounded bordless window instead. In both
        // cases, we enable + disable borders now to work around this.
        fsemu_window_log("macOS workaround to avoid titlebar when restoring\n");
        SDL_SetWindowBordered(fsemu_sdlwindow.window, true);
        SDL_SetWindowBordered(fsemu_sdlwindow.window, false);
    }
#endif
    if (fullscreen) {
        fullscreen_mode = SDL_WINDOW_FULLSCREEN;
    }
    SDL_SetWindowFullscreen(fsemu_sdlwindow.window, fullscreen_mode);
    fsemu_sdlwindow.fullscreen = fullscreen;

    if (fullscreen) {
        fsemu_sdlwindow.ignore_motion_after_fullscreen = true;
    } else {
        if (fsemu_titlebar_use_system()) {
            // If we opened a fullscreen window borderless, we want
            // to add in borders now.
            // FIXME: Only once, and only if we opened fullscreen without
            // FIXME: Handled by fsemu_sdlwindow_fullscreen_to_window now??
            // FIXME: Remove this?
            SDL_SetWindowBordered(fsemu_sdlwindow.window, true);
            // FIXME: Also specify a window size explicitly to avoid
            // getting a big fullscreen-ish window when we originally opened
            // a fullscreen window.
        } else {
            // We do not want to add border
        }
        // When starting in fullscreen, the window size is not correct when
        // restoring. Calling workaround function.
        fsemu_sdlwindow_fullscreen_to_window();
    }
}

SDL_Window* fsemu_sdlwindow_window(void) {
    fsemu_assert(fsemu_sdlwindow.window != NULL);
    return fsemu_sdlwindow.window;
}

SDL_GLContext fsemu_sdlwindow_gl_context(void) {
    return fsemu_sdlwindow.gl_context;
}

bool fsemu_sdlwindow_has_window(void) {
    return fsemu_sdlwindow.window != NULL;
}

static bool fsemu_sdlwindow_handle_keyboard_ctrl_shortcut(SDL_Event* event) {
    printf("fsemu_sdlwindow_handle_keyboard_ctrl_shortcut\n");
    int sym = event->key.key;
    if (sym == SDLK_S) {
        fsemu_frame_toggle_sleep_busywait();
    }
    return true;
}

// This should be replaced with actions triggered by the keyboard shortcut
// system.
static bool fsemu_sdlwindow_handle_keyboard_shortcut(SDL_Event* event) {
    if (event->key.mod & SDL_KMOD_CTRL) {
        return fsemu_sdlwindow_handle_keyboard_ctrl_shortcut(event);
    }

    if (event->key.key == SDLK_A) {
        fsemu_window_log_debug("SDLK_a (cycle aspect/stretch mode)\n");
        fsemu_layout_cycle_stretch_mode();
        return true;
    } else if (event->key.key == SDLK_F) {
        fsemu_window_log_debug("SDLK_f (toggle fullscreen)\n");
        fsemu_window_toggle_fullscreen();
        return true;
    } else if (event->key.key == SDLK_G) {
        fsemu_window_log_debug("SDLK_g (grab/ungrab mouse)\n");
        fsemu_mouse_toggle_captured();
        return true;
    } else if (event->key.key == SDLK_I) {
        fsemu_window_log_debug("SDLK_i (toggle game information)\n");
        fsemu_startupinfo_toggle();
        return true;
    } else if (event->key.key == SDLK_K) {
        fsemu_window_log_debug("SDLK_k (keyboard)\n");
        // FIXME: Not fully implement, only affects modifier key right now.
        fsemu_sdlwindow.full_keyboard_emulation = !fsemu_sdlwindow.full_keyboard_emulation;
        fsemu_window_log_debug("Full keyboard emulation: %d\n",
                               fsemu_sdlwindow.full_keyboard_emulation);
        return true;
    } else if (event->key.key == SDLK_O) {
        fsemu_window_log_debug("SDLK_o (cycle performance overlay)\n");
        fsemu_perfgui_cycle_mode();
        return true;
    } else if (event->key.key == SDLK_P) {
        fsemu_window_log_debug("SDLK_w (toggle paused)\n");
        fsemu_control_toggle_paused();
        return true;
    } else if (event->key.key == SDLK_Q) {
        fsemu_window_log_debug("SDLK_q (quit)\n");
        fsemu_quit_maybe();
        return true;
    } else if (event->key.key == SDLK_S) {
        fsemu_window_log_debug("SDLK_s (screenshot)\n");
        fsemu_screenshot_capture();
        return true;
    } else if (event->key.key == SDLK_V) {
        // fsemu_window_log_debug("SDLK_v (toggle v-sync)\n");
        // fsemu_video_toggle_vsync();
        return true;
    } else if (event->key.key == SDLK_W) {
        fsemu_window_log_debug("SDLK_w (toggle warp mode)\n");
        fsemu_control_toggle_warp();
        return true;
    } else if (event->key.key == SDLK_X) {
        // fsemu_window_log_debug("SDLK_z (cycle zoom mode)\n");
        // fsemu_layout_cycle_zoom_mode();
        return true;
    } else if (event->key.key == SDLK_RETURN) {
        fsemu_window_log_debug("SDLK_RETURN (toggle fullscreen)\n");
        fsemu_window_toggle_fullscreen();
        return true;
    }
    return false;
}

static bool fsemu_sdlwindow_prevent_modifier_pollution(SDL_Event* event) {
    if (!fsemu_sdlwindow.full_keyboard_emulation && event->key.scancode == FSEMU_KMOD_SCANCODE) {
        // For now, also prevent the modifier from being sent at all.
        return true;
    }
    return false;
}

// Called on mouse motion events when using custom window frame.
static void fsemu_sdlwindow_handle_cursor(fsemu_mouse_event_t* event) {
    if (event->x == -1 && event->y == -1) {
        // Bogus event sent by FSEMU when losing focus
        return;
    }
#ifdef FSEMU_OS_WINDOWS
    // Handled by Windows itself in response to hit testing.
#elif defined(FSEMU_OS_MACOS)
    // Also not needed on macOS
#else
    if (fsemu_titlebar_use_system()) {
        // Using system titlebar, so we don't provide move/resize areas
        return;
    }
#if 0
    printf("%d %d\n", event->x, event->y);
#endif
    // At least on Linux/X11, the SDL hit test function works, but only on
    // mouse clicks - so we do not get the resize cursors when hovering over
    // the edges of the Window. We therefore implement this manually here.
    // FIXME: Maybe not needed on all platforms?
    SDL_HitTestResult hit_test =
        fsemu_sdlwindow_hit_test_2(fsemu_sdlwindow.window, event->x, event->y);
    if (hit_test == SDL_HITTEST_RESIZE_LEFT || hit_test == SDL_HITTEST_RESIZE_RIGHT) {
        fsemu_sdlwindow_set_cursor(fsemu_sdlwindow.size_we_cursor);
    } else if (hit_test == SDL_HITTEST_RESIZE_TOP || hit_test == SDL_HITTEST_RESIZE_BOTTOM) {
        fsemu_sdlwindow_set_cursor(fsemu_sdlwindow.size_ns_cursor);
    } else if (hit_test == SDL_HITTEST_RESIZE_BOTTOMLEFT) {
        fsemu_sdlwindow_set_cursor(fsemu_sdlwindow.size_nesw_cursor);
    } else if (hit_test == SDL_HITTEST_RESIZE_BOTTOMRIGHT) {
        fsemu_sdlwindow_set_cursor(fsemu_sdlwindow.size_nwse_cursor);
    } else {
        fsemu_sdlwindow_set_cursor(fsemu_sdlwindow.default_cursor);
    }
#endif
}

static bool fsemu_sdlwindow_handle_mouse_motion(int x, int y, int xrel, int yrel) {
    fsemu_mouse_event_t mouse_event;
    memset(&mouse_event, 0, sizeof(fsemu_mouse_event_t));

    mouse_event.moved = true;
    mouse_event.x = x;
    mouse_event.y = y;
    mouse_event.rel_x = xrel;
    mouse_event.rel_y = yrel;
    // mouse_event.buttons[0] = 0;
    // mouse_event.button = 0;
    // mouse_event.state = 0;

    if (!fsemu_titlebar_use_system()) {
        fsemu_sdlwindow_handle_cursor(&mouse_event);
    }
    if (fsemu_mouse_handle_mouse(&mouse_event)) {
        return true;
    }

    fsemu_sdlwindow.ignore_motion_after_fullscreen = false;
    // FIXME: Move to mouse/window modules?
    fsemu_sdlwindow.last_cursor_motion_at = fsemu_time_us();

    return false;
}

// Returns true if the sdlwindow module handled this event, and the event
// should not propagate further.
bool fsemu_sdlwindow_handle_event(SDL_Event* event) {
    // FIXME: Missing a dirty frame system, so for now, always render at least
    // 10 seconds after an event is received, so animations will have time
    // to finish.
    fsemu_video_must_render_frame_until(fsemu_time_us() + 10 * 1000 * 1000);

    if (fsemu_sdlwindow_handle_window_event(event)) {
        return true;
    }

    static bool mod_press_only;
    static bool f12_press_only;

    if (event->type == SDL_EVENT_KEY_DOWN) {
        mod_press_only = false;
        f12_press_only = false;
        // fsemu_window_log("SDL_KEYDOWN\n");
        if (event->key.scancode == SDL_SCANCODE_F12) {
            fsemu_sdlwindow.f12_pressed = event->key.down;
            f12_press_only = true;
        }

        if (!fsemu_sdlwindow.full_keyboard_emulation &&
            event->key.scancode == FSEMU_KMOD_SCANCODE) {
            fsemu_window_log_debug("MOD key pressed\n");
            mod_press_only = true;
        }

        bool shortcut = false;
        if (fsemu_sdlwindow.f12_pressed) {
            fsemu_window_log_debug("F12 key held down\n");
            shortcut = true;
        } else if (!fsemu_sdlwindow.full_keyboard_emulation && event->key.mod & FSEMU_KMOD_MOD) {
            fsemu_window_log_debug("MOD key held down\n");
            shortcut = true;
        }
        if (event->key.scancode == SDL_SCANCODE_PRINTSCREEN) {
            fsemu_window_log_debug("SDL_SCANCODE_PRINTSCREEN\n");
            fsemu_screenshot_capture();
            return true;
        }
        if (shortcut) {
            if (fsemu_sdlwindow_handle_keyboard_shortcut(event)) {
                return true;
            }
            // EDIT: Changed so we always avoid propagating key press when MOD
            // is held.
            return true;
        }
        if (fsemu_sdlwindow_prevent_modifier_pollution(event)) {
            return true;
        }
    } else if (event->type == SDL_EVENT_KEY_UP) {
        if (event->key.scancode == SDL_SCANCODE_F12) {
            fsemu_sdlwindow.f12_pressed = false;
        }
#if 0
        if (!fsemu_sdlwindow.full_keyboard_emulation &&
            event->key.scancode == FSEMU_KMOD_SCANCODE) {
            if (mod_press_only) {
                fsemu_window_log("MOD key press/release\n");
            }
            return true;
        }
#endif

        // Ideally, we would want to make these (Mod/F12) modifiers
        // configurable, but they are not at this time.
        if (f12_press_only) {
            // F12 key was pressed and released without any intervening key
            // strokes. In this case, we open the menu.
            fsemu_window_log_debug("F12 key press/release\n");
            fsemu_sdlwindow_open_menu();
            return true;
        }
        if (mod_press_only || f12_press_only) {
            // Modifier key was pressed and released without any intervening
            // key strokes. In this case, we open the menu.
            fsemu_window_log_debug("MOD key press/release (maybe)\n");
            // Actually, if we've just alt-tabbed, the tab press might have
            // been caught by the windowing system, so it looks to the emulator
            // like a single mod press, but it really isn't...
            fsemu_sdlwindow_open_menu_delayed();
            return true;
        }
    }

    fsemu_mouse_event_t mouse_event;
    memset(&mouse_event, 0, sizeof(fsemu_mouse_event_t));

    switch (event->type) {
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP:
            fsemu_mouse_log_debug("%s button=%d\n",
                                  event->button.down ? "SDL_MOUSEBUTTONDOWN" : "SDL_MOUSEBUTTONUP",
                                  event->button.button);
            mouse_event.pressed = event->button.down ? event->button.button : 0;
            mouse_event.released = event->button.down ? 0 : event->button.button;
            // mouse_event.moved = false;
            mouse_event.x = event->button.x;
            mouse_event.y = event->button.y;
            // mouse_event.buttons[0] = 0;
            // mouse_event.buttons[1] = event->button.
            mouse_event.button = event->button.button;
            mouse_event.state = event->button.down;
            if (fsemu_mouse_handle_mouse(&mouse_event)) {
                return true;
            }
            break;
        case SDL_EVENT_MOUSE_MOTION:
            fsemu_mouse_log_debug("SDL_MOUSEMOTION x=%d y=%d xrel=%d yrel=%d\n", event->motion.x,
                                  event->motion.y, event->motion.xrel, event->motion.yrel);
            // fsemu_mouse_handle_position(event->motion.x, event->motion.y);
            // mouse_event.pressed = 0;
            // mouse_event.released = 0;
            if (fsemu_sdlwindow.ignore_motion_after_fullscreen) {
                if (event->motion.x == 0 && event->motion.y == 0 && event->motion.xrel == 0 &&
                    event->motion.yrel == 0) {
                    fsemu_mouse_log_debug(
                        "Ignoring bogus event after switching to "
                        "fullscreen\n");
                    return true;
                }
            }
            if (fsemu_sdlwindow_handle_mouse_motion(event->motion.x, event->motion.y,
                                                    event->motion.xrel, event->motion.yrel)) {
                return true;
            }
            break;
    }

#if 0
    switch (event->type) {
        case SDL_USEREVENT:

#if 0
            fsemu_window_log("User event %d received\n", event->user.code);
            if (event->user.code == 2) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                printf("Mouse perhaps at %d %d\n", x, y);
            }
#endif

            break;

        case SDL_JOYAXISMOTION:
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
        case SDL_JOYHATMOTION:
        case SDL_KEYDOWN:
        case SDL_KEYUP:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEMOTION:
        case SDL_MOUSEWHEEL:
        case SDL_TEXTINPUT:
// #ifdef FSUAE
//             fs_ml_event_loop_iteration(event);
// #endif
            break;
    }
#endif

    if (fsemu_sdlinput_handle_event(event)) {
        return true;
    }

    return false;
}

bool fsemu_sdlwindow_handle_window_event(SDL_Event* event) {
    if (event->type == SDL_EVENT_QUIT) {
        fsemu_quit_maybe();
        return true;
    }

    // if (event->type != SDL_WINDOWEVENT) {
    //     return false;
    // }

    switch (event->window.type) {
        case SDL_EVENT_WINDOW_SHOWN:
            fsemu_window_log_debug("Window %d shown\n", event->window.windowID);
            break;
        case SDL_EVENT_WINDOW_HIDDEN:
            fsemu_window_log_debug("Window %d hidden\n", event->window.windowID);
            break;
        case SDL_EVENT_WINDOW_EXPOSED:
            fsemu_window_log_debug("Window %d exposed\n", event->window.windowID);
            break;
        case SDL_EVENT_WINDOW_MOVED:
#if 0
            fsemu_window_log("Window %d moved to %d,%d\n",
                             event->window.windowID,
                             event->window.data1,
                             event->window.data2);
#endif
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            // Window has been resized to data1xdata2; this event is
            // always preceded by SDL_EVENT_WINDOW_SIZE_CHANGED.
            fsemu_window_log_debug("Window %d resized to %dx%d\n", event->window.windowID,
                                   event->window.data1, event->window.data2);
            break;
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
            // Window size has changed, either as a result of an API call
            // or through the system or user changing the window size;
            // this event is followed by SDL_EVENT_WINDOW_RESIZED if the
            // size was changed by an external event, i.e. the user or the
            // window manager.
            fsemu_window_log_debug("Window %d size changed to %dx%d\n", event->window.windowID,
                                   event->window.data1, event->window.data2);
            {
                int w = event->window.data1;
                int h = event->window.data2;

#if 0
                // FIXME: Don't do OpenGL stuff here
                glViewport(0, 0, w, h);
                glMatrixMode(GL_PROJECTION);
                // glLoadIdentity();
                // glOrtho(0, w, h, 0, 0.0, 1.0);
                glMatrixMode(GL_MODELVIEW);
#endif
                fsemu_window_set_size_2(w, h);
                fsemu_video_set_size_2(w, h);
                fsemu_layout_set_size_2(w, h);

                /*
                if (fsemu_sdlwindow.was_fullscreen_initially) {
                    int flags = SDL_GetWindowFlags(fsemu_sdlwindow.window);
                    if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP == 0) {
                        fsemu_sdlwindow_fullscreen_to_window();
                    }
                }
                */
            }

            break;
        case SDL_EVENT_WINDOW_MINIMIZED:
            fsemu_window_log_debug("Window %d minimized\n", event->window.windowID);
            break;
        case SDL_EVENT_WINDOW_MAXIMIZED:
            fsemu_window_log_debug("Window %d maximized\n", event->window.windowID);
            break;
        case SDL_EVENT_WINDOW_RESTORED:
            fsemu_window_log_debug("Window %d restored\n", event->window.windowID);
            break;
        case SDL_EVENT_WINDOW_MOUSE_ENTER:
            fsemu_window_log_debug("Mouse entered window %d\n", event->window.windowID);

#if 0
            // Hmm, we don't even seem to be able to read the correct mouse
            // position yet, --perhaps later? Nope, postponing read to later
            // does not help either -- on Linux.
#if 0
            SDL_Event user_event;
            SDL_zero(user_event);
            user_event.type = SDL_USEREVENT;
            user_event.user.code = 2;
            user_event.user.data1 = NULL;
            user_event.user.data2 = NULL;
            SDL_PushEvent(&user_event);
#endif
            // At least on Linux, we do not seem to get a motion event
            // when entering the window, so we create a synthetic one
            // (but not with relative motion)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                fsemu_window_log(
                    "Read mouse position manually: %d %d\n", x, y);
                fsemu_sdlwindow_handle_mouse_motion(x, y, 0, 0);
            }
#endif

            // FIXME: Check if this hack is needed on other platforms, needed
            // on X11 as of SDL 2.0.10 at least.
            {
                float x, y;
                int winx, winy;
                SDL_GetGlobalMouseState(&x, &y);
                fsemu_window_log_debug("Read mouse position manually (screen): %d %d\n", x, y);
                SDL_GetWindowPosition(fsemu_sdlwindow.window, &winx, &winy);
                x -= winx;
                y -= winy;
                fsemu_window_log_debug("Read mouse position manually: %d %d\n", x, y);
                bool mouse_not_moved = fsemu_sdlwindow.last_cursor_motion_at == 0;
                fsemu_sdlwindow_handle_mouse_motion(x, y, 0, 0);
                if (mouse_not_moved) {
                    // If we fake a mouse motion event on first time the
                    // window opens, we do not want that to keep the cursor
                    // alive.
                    fsemu_sdlwindow.last_cursor_motion_at = 0;
                }
            }

            break;
        case SDL_EVENT_WINDOW_MOUSE_LEAVE:
            fsemu_window_log_debug("Mouse left window %d\n", event->window.windowID);
            // Sending fake mouse event (outside window) when mouse
            // leaves the window, useful for GUI hovering effects.
            // FIXME: Might need to make sure these events do not
            // reach the emulation layer...
            // Handled by fsemu-titlebar module currently, but might want
            // to do this in a better way

            fsemu_sdlwindow_handle_mouse_motion(-1, -1, 0, 0);

            // fsemu_mouse_event_t mouse_event;
            // memset(&mouse_event, 0, sizeof(fsemu_mouse_event_t));
            // mouse_event.moved = true;
            // mouse_event.x = -1;
            // mouse_event.y = -1;
            // if (!fsemu_titlebar_use_system()) {
            //     fsemu_sdlwindow_handle_cursor(&mouse_event);
            // }
            // if (fsemu_mouse_handle_mouse(&mouse_event)) {
            //     return true;
            // }

            break;
        case SDL_EVENT_WINDOW_FOCUS_GAINED:
            fsemu_window_log_debug("Window %d gained keyboard focus\n", event->window.windowID);
            fsemu_window_set_active(true);
            break;
        case SDL_EVENT_WINDOW_FOCUS_LOST:
            // We lost focus and this might be due to a Mod+Alt key
            // combination. In which case we do not want to open the
            // on-screen menu.
            fsemu_sdlwindow_open_menu_abort();
            fsemu_window_log_debug("Window %d lost keyboard focus\n", event->window.windowID);
            fsemu_window_set_active(false);
            break;
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            fsemu_window_log_debug("Window %d closed\n", event->window.windowID);
            break;
        // case SDL_EVENT_WINDOW_TAKE_FOCUS:
        //     fsemu_window_log_debug("Window %d is offered a focus\n",
        //                            event->window.windowID);
        //     break;
        case SDL_EVENT_WINDOW_HIT_TEST:
            fsemu_window_log_debug("Window %d has a special hit test\n", event->window.windowID);
            break;
        default:
            // fsemu_window_log_debug("Window %d got unknown event %d\n",
            //                        event->window.windowID,
            //                        event->window.type);
            break;
    }

    return true;
}

void fsemu_sdlwindow_notify_frame_rendered_vt(void) {
    SDL_Event user_event;
    SDL_zero(user_event);
    user_event.type = fsemu_sdlwindow.frame_rendered_event_type;
    SDL_PushEvent(&user_event);
}

void fsemu_sdlwindow_notify_quit(void) {
    SDL_Event user_event;
    SDL_zero(user_event);
    user_event.type = fsemu_sdlwindow.quit_event_type;
    SDL_PushEvent(&user_event);
}

// ----------------------------------------------------------------------------

static void fsemu_sdlwindow_log_modes(void) {
    /*
    fsemu_window_log("Listing display modes for display 0:\n");
    // int display_count = 0;
    int display_index = 0;
    int mode_index = 0;
    SDL_DisplayMode mode = {SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0};
    while (1) {
        int error = SDL_GetDisplayMode(display_index, mode_index, &mode);
        if (error) {
            break;
        }
        fsemu_window_log("Mode %d: %d %d %d\n",
                         mode_index,
                         mode.w,
                         mode.h,
                         mode.refresh_rate);
        mode_index += 1;
    }
    */
}

// ----------------------------------------------------------------------------

static void fsemu_sdlwindow_quit(void) {
    fsemu_window_log("Quit sdlwindow module\n");
    if (fsemu_sdlwindow.gl_context) {
        fsemu_window_log_debug("Deleting OpenGL context\n");
        SDL_GL_DestroyContext(fsemu_sdlwindow.gl_context);
        fsemu_sdlwindow.gl_context = NULL;
    }
    if (fsemu_sdlwindow.window) {
        fsemu_window_log_debug("Destroying SDL window\n");
        SDL_DestroyWindow(fsemu_sdlwindow.window);
        fsemu_sdlwindow.window = NULL;
    }
    // FIXME: Move to fsemu-module or fsemu-main, run after all modules are
    // shut down instead?
    fsemu_window_log("Calling SDL_Quit\n");
    SDL_Quit();
}

// ----------------------------------------------------------------------------

void fsemu_sdlwindow_init(void) {
    if (FSEMU_MODULE_INIT(sdlwindow)) {
        return;
    }
#if 0
    if (fsemu_sdlwindow.init) {
        return;
    }
    fsemu_sdlwindow.init = true;
    fsemu_window_log("Init sdlwindow module\n");
    fsemu_module_on_quit(fsemu_sdlwindow_quit);
#endif
#if 0
    if (fsemu_module_init(
            "sdlwindow", &fsemu_sdlwindow.init, fsemu_sdlwindow_quit)) {
        return;
    }
#endif
#if 1
#else
    FSEMU_MODULE_INIT_ONCE(sdlwindow);
#endif
    fsemu_window_log_debug("SDL_Init(SDL_INIT_VIDEO)\n");
    SDL_Init(SDL_INIT_VIDEO);

    uint32_t event_types_start = SDL_RegisterEvents(2);
    fsemu_sdlwindow.frame_rendered_event_type = event_types_start + 0;
    fsemu_sdlwindow.quit_event_type = event_types_start + 0;

#ifdef FSEMU_OS_MACOS
    // Default to off for smoother transitions, can enable with environment
    // SDL_VIDEO_MACOS_FULLSCREEN_SPACES=1
    SDL_SetHint(SDL_HINT_VIDEO_MAC_FULLSCREEN_SPACES, "0");
#endif

#if SDL_VERSION_ATLEAST(2, 0, 5)
    SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
#else
    // FIXME: log_warning
    fsemu_window_log("Failed to set SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH (too old SDL)\n");
#endif

    fsemu_titlebar_init();
    fsemu_monitor_init();

    fsemu_sdlwindow.default_cursor = SDL_GetDefaultCursor();
    fsemu_sdlwindow.size_we_cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_EW_RESIZE);
    fsemu_sdlwindow.size_ns_cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NS_RESIZE);
    fsemu_sdlwindow.size_nesw_cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NESW_RESIZE);
    fsemu_sdlwindow.size_nwse_cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NWSE_RESIZE);
    fsemu_sdlwindow.current_cursor = fsemu_sdlwindow.default_cursor;

    // Do not minimize SDL_Window if it loses key focus in fullscreen mode.
    // Override with environment variable SDL_VIDEO_MINIMIZE_ON_FOCUS_LOSS=1
    SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");

    // Bogus initial value to ensure swap interval is set at least once.
    // One would think the default value was 0 already, but this does not
    // seem to always be the case.
    fsemu_sdlwindow.swap_interval = -1337;

    fsemu_sdlwindow_log_modes();
}

#endif  // FSEMU_SDL
