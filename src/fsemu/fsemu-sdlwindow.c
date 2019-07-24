#define FSEMU_INTERNAL
#include "fsemu/fsemu-sdlwindow.h"

#ifdef FSEMU_SDL

#include "fsemu/fsemu-layout.h"
#include "fsemu/fsemu-mouse.h"
#include "fsemu/fsemu-quit.h"
#include "fsemu/fsemu-types.h"
#include "fsemu/fsemu-util.h"
#include "fsemu/fsemu-window.h"

#ifdef FSUAE
#include <fs/emu/input.h>
#endif

// ---------------------------------------------------------------------------

static struct {
    SDL_Window *window;
    fsemu_rect rect;
    bool fullscreen;
    bool mouse_captured;
} fsemu_sdlwindow;

// ---------------------------------------------------------------------------

void fsemu_sdlwindow_init(void)
{
    fsemu_return_if_already_initialized();

    // fsemu_window_log("SDL_Init(SDL_INIT_VIDEO)\n");
    // SDL_Init(SDL_INIT_VIDEO);
    fsemu_window_log("SDL_Init(SDL_INIT_EVERYTHING\n");
    SDL_Init(SDL_INIT_EVERYTHING);
}

// ---------------------------------------------------------------------------

void fsemu_sdlwindow_update(void)
{
    bool want_fullscreen = fsemu_window_fullscreen();
    if (want_fullscreen != fsemu_sdlwindow.fullscreen) {
        fsemu_sdlwindow_set_fullscreen(want_fullscreen);
    }
    bool want_mouse_captured = fsemu_mouse_captured();
    if (want_mouse_captured != fsemu_sdlwindow.mouse_captured) {
        if (want_mouse_captured) {
            fsemu_mouse_log("Capturing mouse\n");
        } else {
            fsemu_mouse_log("Releasing mouse capture\n");
        }
        // SDL_CaptureMouse(want_mouse_captured);
        // SDL_ShowCursor(want_mouse_captured ? SDL_DISABLE : SDL_ENABLE);
        SDL_SetRelativeMouseMode(want_mouse_captured);
        fsemu_sdlwindow.mouse_captured = want_mouse_captured;
    }
}

// ---------------------------------------------------------------------------

SDL_Window *fsemu_sdlwindow_create(void)
{
    fsemu_assert(fsemu_sdlwindow.window == NULL);

    const char *title = "FSEMU";

    fsemu_rect rect;
    rect.x = SDL_WINDOWPOS_CENTERED;
    rect.y = SDL_WINDOWPOS_CENTERED;
    rect.w = 960;
    rect.h = 540;
    // read_initial_window_rect(&rect);
    fsemu_window_log(
        "Initial window rect: %dx%d +%d+%d\n", rect.w, rect.h, rect.x, rect.y);
    int flags = 0;
    // flags |= SDL_WINDOW_OPENGL;
    flags |= SDL_WINDOW_RESIZABLE;
    // flags |= SDL_WINDOW_HIDDEN;

    if (fsemu_window_fullscreen()) {
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        fsemu_sdlwindow.fullscreen = true;
    }
    // FIXME: initial mouse capture

    SDL_Window *window =
        SDL_CreateWindow(title, rect.x, rect.y, rect.w, rect.h, flags);
    fsemu_window_log("Window %p\n", window);
    fsemu_sdlwindow_set_window(window);
    if (fsemu_custom_frame()) {
        SDL_SetWindowBordered(window, SDL_FALSE);
    }
    fsemu_sdlwindow.rect.x = rect.x;
    fsemu_sdlwindow.rect.y = rect.y;
    fsemu_sdlwindow.rect.w = rect.w;
    fsemu_sdlwindow.rect.h = rect.h;

    fsemu_layout_set_size(rect.w, rect.h);

    return window;
}

void fsemu_sdlwindow_show(void)
{
    fsemu_assert(fsemu_sdlwindow.window != NULL);
    fsemu_window_log("Show window\n");
    SDL_ShowWindow(fsemu_sdlwindow.window);
#if 0
    SDL_Event event;
    while (SDL_PollEvent(&event)) {

    }
#endif
}

void fsemu_sdlwindow_set_window(SDL_Window *window)
{
    fsemu_assert(!fsemu_sdlwindow.window);
    SDL_GetWindowPosition(
        window, &fsemu_sdlwindow.rect.x, &fsemu_sdlwindow.rect.y);
    SDL_GetWindowSize(
        window, &fsemu_sdlwindow.rect.w, &fsemu_sdlwindow.rect.h);
    fsemu_sdlwindow.window = window;

    Uint32 flags = SDL_GetWindowFlags(fsemu_sdlwindow.window);
    // FIXME: Also check for old SDL_WINDOW_FULLSCREEN?
    fsemu_sdlwindow.fullscreen = flags & SDL_WINDOW_FULLSCREEN_DESKTOP;

    fsemu_window_log("Window is %p\n", fsemu_sdlwindow.window);
}

bool fsemu_sdlwindow_fullscreen(void)
{
    return fsemu_sdlwindow.fullscreen;
}

void fsemu_sdlwindow_set_fullscreen(bool fullscreen)
{
    int fullscreen_mode = 0;
    if (fullscreen) {
        fullscreen_mode = SDL_WINDOW_FULLSCREEN_DESKTOP;
        // FIXME: Support for legacy fullscreen?
    }
    SDL_SetWindowFullscreen(fsemu_sdlwindow.window, fullscreen_mode);
    fsemu_sdlwindow.fullscreen = fullscreen;
}

SDL_Window *fsemu_sdlwindow_window(void)
{
    fsemu_assert(fsemu_sdlwindow.window != NULL);
    return fsemu_sdlwindow.window;
}

static bool fsemu_sdlwindow_handle_keyboard_shortcut(SDL_Event *event)
{
    if (event->key.keysym.sym == SDLK_q) {
        fsemu_window_log("SDLK_q\n");
        fsemu_quit_maybe();
        return true;
    }
    if (event->key.keysym.sym == SDLK_RETURN) {
        fsemu_window_log("SDLK_RETURN\n");
        // fsemu_window_set_fullscreen(!fsemu_window_fullscreen());
        // fsemu_sdlwindow_set_fullscreen(!fsemu_sdlwindow_fullscreen());
        fsemu_window_set_fullscreen(!fsemu_window_fullscreen());
        return true;
    }
    return false;
}

bool fsemu_sdlwindow_handle_event(SDL_Event *event)
{
    if (fsemu_sdlwindow_handle_window_event(event)) {
        return true;
    }

    if (event->type == SDL_KEYDOWN) {
        fsemu_window_log("SDL_KEYDOWN\n");
        if (event->key.keysym.mod & KMOD_LALT) {  // FIXME: macOS
            fsemu_window_log("KMOD_LALT\n");
            if (fsemu_sdlwindow_handle_keyboard_shortcut(event)) {
                return true;
            }
        }
    }

    fsemu_mouse_event_t mouse_event;

    switch (event->type) {
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            mouse_event.x = event->button.x;
            mouse_event.y = event->button.y;
            mouse_event.button = event->button.button;
            mouse_event.state = event->button.state;
            if (fsemu_mouse_handle_mouse(&mouse_event)) {
                return true;
            }
            break;
        case SDL_MOUSEMOTION:
            // fsemu_mouse_handle_position(event->motion.x, event->motion.y);
            mouse_event.x = event->button.x;
            mouse_event.y = event->button.y;
            mouse_event.button = -1;
            mouse_event.state = 0;
            if (fsemu_mouse_handle_mouse(&mouse_event)) {
                return true;
            }
            break;
    }

    switch (event->type) {
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
#ifdef FSUAE
            fs_ml_event_loop_iteration(event);
#endif
            break;
    }

    return false;
}

bool fsemu_sdlwindow_handle_window_event(SDL_Event *event)
{
    if (event->type == SDL_QUIT) {
        fsemu_quit_maybe();
        return true;
    }

    if (event->type == SDL_WINDOWEVENT) {
        switch (event->window.event) {
            case SDL_WINDOWEVENT_SHOWN:
                fsemu_window_log("Window %d shown\n", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_HIDDEN:
                fsemu_window_log("Window %d hidden\n", event->window.windowID);
                break;
            case SDL_WINDOWEVENT_EXPOSED:
                fsemu_window_log("Window %d exposed\n",
                                 event->window.windowID);
                break;
            case SDL_WINDOWEVENT_MOVED:
                fsemu_window_log("Window %d moved to %d,%d\n",
                                 event->window.windowID,
                                 event->window.data1,
                                 event->window.data2);
                break;
            case SDL_WINDOWEVENT_RESIZED:
                // Window has been resized to data1xdata2; this event is
                // always preceded by SDL_WINDOWEVENT_SIZE_CHANGED.
                fsemu_window_log("Window %d resized to %dx%d\n",
                                 event->window.windowID,
                                 event->window.data1,
                                 event->window.data2);
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                // Window size has changed, either as a result of an API call
                // or through the system or user changing the window size;
                // this event is followed by SDL_WINDOWEVENT_RESIZED if the
                // size was changed by an external event, i.e. the user or the
                // window manager.
                fsemu_window_log("Window %d size changed to %dx%d\n",
                                 event->window.windowID,
                                 event->window.data1,
                                 event->window.data2);
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

                fsemu_layout_set_size(w, h);

                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                fsemu_window_log("Window %d minimized\n",
                                 event->window.windowID);
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                fsemu_window_log("Window %d maximized\n",
                                 event->window.windowID);
                break;
            case SDL_WINDOWEVENT_RESTORED:
                fsemu_window_log("Window %d restored\n",
                                 event->window.windowID);
                break;
            case SDL_WINDOWEVENT_ENTER:
                fsemu_window_log("Mouse entered window %d\n",
                                 event->window.windowID);
                break;
            case SDL_WINDOWEVENT_LEAVE:
                fsemu_window_log("Mouse left window %d\n",
                                 event->window.windowID);
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                fsemu_window_log("Window %d gained keyboard focus\n",
                                 event->window.windowID);
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                fsemu_window_log("Window %d lost keyboard focus\n",
                                 event->window.windowID);
                break;
            case SDL_WINDOWEVENT_CLOSE:
                fsemu_window_log("Window %d closed\n", event->window.windowID);
                break;
#if SDL_VERSION_ATLEAST(2, 0, 5)
            case SDL_WINDOWEVENT_TAKE_FOCUS:
                fsemu_window_log("Window %d is offered a focus\n",
                                 event->window.windowID);
                break;
            case SDL_WINDOWEVENT_HIT_TEST:
                fsemu_window_log("Window %d has a special hit test\n",
                                 event->window.windowID);
                break;
#endif
            default:
                fsemu_window_log("Window %d got unknown event %d\n",
                                 event->window.windowID,
                                 event->window.event);
                break;
        }
        return true;
    }
    return false;
}

#endif  // FSEMU_SDL
