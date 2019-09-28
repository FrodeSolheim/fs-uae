#define FSEMU_INTERNAL
#include "fsemu/fsemu-window.h"

#include "fsemu/fsemu-config.h"
#include "fsemu/fsemu-layout.h"
#include "fsemu/fsemu-mouse.h"
#include "fsemu/fsemu-option.h"
#include "fsemu/fsemu-sdlwindow.h"
#include "fsemu/fsemu-types.h"
#include "fsemu/fsemu-util.h"

// ---------------------------------------------------------------------------

static struct fsemu_window {
    // True if the window (or will be) shown fullscreen.
    bool fullscreen;
} fsemu_window;

static SDL_Window *fsemu_sdl_window;
static fsemu_rect fsemu_window_rect;

// ---------------------------------------------------------------------------

void fsemu_window_init(void)
{
    fsemu_return_if_already_initialized();

    // FIXME: Consider running fsemu_mouse init from fsemu_window_init ?
    // Running mouse init so initial input grab option is initialized
    fsemu_mouse_init();

    fsemu_config_read_bool_default(
        FSEMU_OPTION_FULLSCREEN, &fsemu_window.fullscreen, false);

    fsemu_sdlwindow_init();
    fsemu_sdlwindow_create();
    fsemu_sdlwindow_show();
}

// ---------------------------------------------------------------------------

void fsemu_window_work(int timeout)
{
    SDL_Event event;
    if (SDL_WaitEventTimeout(&event, timeout)) {
        fsemu_sdlwindow_handle_event(&event);
    }
    while (SDL_PollEvent(&event)) {
        fsemu_sdlwindow_handle_event(&event);
    }

    // Perform periodic updates, check if fullscreen should be toggled, etc.
    fsemu_sdlwindow_update();
}

// ---------------------------------------------------------------------------

bool fsemu_window_fullscreen(void)
{
    return fsemu_window.fullscreen;
}

void fsemu_window_set_fullscreen(bool fullscreen)
{
    fsemu_window.fullscreen = fullscreen;
}

// ---------------------------------------------------------------------------

void fsemu_assert_window(void)
{
    SDL_assert(fsemu_sdl_window != NULL);
}

void fsemu_window_size(fsemu_size *size)
{
    // fsemu_assert_window();
    // FIXME: Look up cached size
    SDL_GetWindowSize(fsemu_sdlwindow_window(), &size->w, &size->h);
}

void fsemu_set_window_size(int width, int height)
{
    fsemu_window_rect.w = width;
    fsemu_window_rect.h = height;
    SDL_SetWindowSize(fsemu_sdl_window, width, height);
}

void fsemu_set_window_fullscreen(bool fullscreen)
{
    if (fullscreen) {
        SDL_GetWindowPosition(
            fsemu_sdl_window, &fsemu_window_rect.x, &fsemu_window_rect.y);
        /* At least on Linux X11, SDL_GetWindowPosition returns coordinates
         * for the window without border, but SDL_SetWindowPositions
         * sets the position of the top-left border coordinate.
         * Compensating. */
        int wbt, wbl;
        if (SDL_GetWindowBordersSize(
                fsemu_sdl_window, &wbt, &wbl, NULL, NULL) == 0) {
            printf("[FSEMU] Hack:Subtracting borders -%d-%d\n", wbt, wbl);
            fsemu_window_rect.x -= wbl;
            fsemu_window_rect.y -= wbt;
        }
        SDL_GetWindowSize(
            fsemu_sdl_window, &fsemu_window_rect.w, &fsemu_window_rect.h);
        printf("[FSEMU] Window dimensions were %dx%d +%d+%d\n",
               fsemu_window_rect.w,
               fsemu_window_rect.h,
               fsemu_window_rect.x,
               fsemu_window_rect.y);
        SDL_SetWindowFullscreen(fsemu_sdl_window,
                                SDL_WINDOW_FULLSCREEN_DESKTOP);
    } else {
        printf("[FSEMU] Setting window dimensions to %dx%d +%d+%d\n",
               fsemu_window_rect.w,
               fsemu_window_rect.h,
               fsemu_window_rect.x,
               fsemu_window_rect.y);
        SDL_SetWindowFullscreen(fsemu_sdl_window, 0);
        SDL_SetWindowSize(
            fsemu_sdl_window, fsemu_window_rect.w, fsemu_window_rect.h);
        SDL_SetWindowPosition(
            fsemu_sdl_window, fsemu_window_rect.x, fsemu_window_rect.y);
    }
}

void fsemu_set_sdl_window(SDL_Window *window)
{
    printf("[FSEMU] Register SDL window\n");
    fsemu_sdl_window = window;
}

static void read_inital_window_pos(fsemu_point *pos)
{
    static bool initialized;
    static int x = -1, y = -1;
    if (!initialized) {
        sscanf(fsemu_getenv("FSGS_WINDOW_POS"), "%d,%d", &x, &y);
        initialized = true;
    }
    if (x != -1 && y != -1) {
        pos->x = x;
        pos->y = y;
    }
}

static void read_inital_window_size(fsemu_size *size)
{
    static bool initialized;
    static int w = 0, h = 0;
    if (!initialized) {
        sscanf(fsemu_getenv("FSGS_WINDOW_SIZE"), "%d,%d", &w, &h);
        initialized = true;
    }
    if (w > 0 && h > 0) {
        size->w = w;
        size->h = h;
    }
}

static void read_initial_window_rect(fsemu_rect *rect)
{
    fsemu_point pos = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED};
    fsemu_size size = {960, 540};
    read_inital_window_pos(&pos);
    read_inital_window_size(&size);
    rect->x = pos.x;
    rect->y = pos.y;
    rect->w = size.w;
    rect->h = size.h;
}
