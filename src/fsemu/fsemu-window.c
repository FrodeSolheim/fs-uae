#define FSEMU_INTERNAL
#include "fsemu-window.h"

#include "fsemu-layout.h"
#include "fsemu-mouse.h"
#include "fsemu-option.h"
#include "fsemu-options.h"
#include "fsemu-sdlwindow.h"
#include "fsemu-types.h"
#include "fsemu-util.h"

#define FSEMU_WINDOW_TITLE_MAX 128

// ---------------------------------------------------------------------------

static struct fsemu_window {
    bool active;
    // True if the window (or will be) shown fullscreen.
    bool fullscreen;
    char title[FSEMU_WINDOW_TITLE_MAX];
    // fsemu_point_t position;
    fsemu_size_t size;
    fsemu_rect_t initial_fullscreen_rect;
    fsemu_rect_t initial_rect;
} fsemu_window;

// static SDL_Window *fsemu_sdl_window;
// static fsemu_rect fsemu_window_rect;

static void fsemu_window_read_options(void)
{
    fsemu_option_read_bool_default(
        FSEMU_OPTION_FULLSCREEN, &fsemu_window.fullscreen, false);

    fsemu_rect_t *rect = &fsemu_window.initial_rect;
    fsemu_option_read_int(FSEMU_OPTION_WINDOW_X, &rect->x);
    fsemu_option_read_int(FSEMU_OPTION_WINDOW_Y, &rect->y);
    fsemu_option_read_int(FSEMU_OPTION_WINDOW_W, &rect->w);
    fsemu_option_read_int(FSEMU_OPTION_WINDOW_H, &rect->h);
    fsemu_window_log("Initial window rect: %d %d %d %d\n",
                     rect->x,
                     rect->y,
                     rect->w,
                     rect->h);

    rect = &fsemu_window.initial_fullscreen_rect;
    fsemu_option_read_int(FSEMU_OPTION_FULLSCREEN_X, &rect->x);
    fsemu_option_read_int(FSEMU_OPTION_FULLSCREEN_Y, &rect->y);
    fsemu_option_read_int(FSEMU_OPTION_FULLSCREEN_W, &rect->w);
    fsemu_option_read_int(FSEMU_OPTION_FULLSCREEN_H, &rect->h);
    fsemu_window_log("Initial fullscreen rect: %d %d %d %d\n",
                     rect->x,
                     rect->y,
                     rect->w,
                     rect->h);
}

// ---------------------------------------------------------------------------

void fsemu_window_init(void)
{
    fsemu_return_if_already_initialized();

    fsemu_layout_init();
    // FIXME: Consider running fsemu_mouse init from fsemu_window_init ?
    // Running mouse init so initial input grab option is initialized
    fsemu_mouse_init();

    fsemu_window_read_options();

    fsemu_sdlwindow_init();
    fsemu_sdlwindow_create();
    fsemu_sdlwindow_show();
}

// ---------------------------------------------------------------------------

void fsemu_window_initial_rect(fsemu_rect_t *rect)
{
    *rect = fsemu_window.initial_rect;
}

void fsemu_window_initial_fullscreen_rect(fsemu_rect_t *rect)
{
    *rect = fsemu_window.initial_fullscreen_rect;
}

// ---------------------------------------------------------------------------

bool fsemu_window_active(void)
{
    return fsemu_window.active;
}

void fsemu_window_set_active(bool active)
{
    fsemu_window.active = active;
}

void fsemu_window_size(fsemu_size_t *size)
{
    size->w = fsemu_window.size.w;
    size->h = fsemu_window.size.h;
}

void fsemu_window_set_size(fsemu_size_t *size)
{
    fsemu_window_set_size_2(size->w, size->h);
}

void fsemu_window_set_size_2(int width, int height)
{
    fsemu_window.size.w = width;
    fsemu_window.size.h = height;
}

// ---------------------------------------------------------------------------

const char *fsemu_window_title(void)
{
    if (fsemu_window.title[0] == '\0') {
        return "FSEMU";
    }
    return fsemu_window.title;
}

void fsemu_window_set_title(const char *title)
{
    // FIXME: Also set a window title dirty flag so that we can update
    // the window after it is created (in fsemu_window_work for example).
    strncpy(fsemu_window.title, title, FSEMU_WINDOW_TITLE_MAX);
    fsemu_window.title[FSEMU_WINDOW_TITLE_MAX - 1] = '\0';
}

// ---------------------------------------------------------------------------

void fsemu_window_work(int timeout)
{
    fsemu_sdlwindow_work(timeout);

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

void fsemu_window_toggle_fullscreen(void)
{
    fsemu_window_set_fullscreen(!fsemu_window_fullscreen());
}

// ---------------------------------------------------------------------------

/*
void fsemu_assert_window(void)
{
    SDL_assert(fsemu_sdl_window != NULL);
}
*/

/*
void fsemu_window_size(fsemu_size *size)
{
    // fsemu_assert_window();
    // FIXME: Look up cached size
    SDL_GetWindowSize(fsemu_sdlwindow_window(), &size->w, &size->h);
}
*/

/*
void fsemu_set_window_size(int width, int height)
{
    fsemu_window_rect.w = width;
    fsemu_window_rect.h = height;
    SDL_SetWindowSize(fsemu_sdl_window, width, height);
}
*/

#if 0
void fsemu_set_window_fullscreen(bool fullscreen)
{
    fsemu_window_log("fsemu_set_window_fullscreen %d\n", fullscreen);
    if (fullscreen) {
        SDL_GetWindowPosition(
            fsemu_sdl_window, &fsemu_window_rect.x, &fsemu_window_rect.y);
        /* At least on Linux X11, SDL_GetWindowPosition returns coordinates
         * for the window without border, but SDL_SetWindowPositions
         * sets the position of the top-left border coordinate.
         * Compensating. */
#if SDL_VERSION_ATLEAST(2, 0, 5)
        int wbt, wbl;
        if (SDL_GetWindowBordersSize(
                fsemu_sdl_window, &wbt, &wbl, NULL, NULL) == 0) {
            printf("[FSEMU] Hack:Subtracting borders -%d-%d\n", wbt, wbl);
            fsemu_window_rect.x -= wbl;
            fsemu_window_rect.y -= wbt;
        }
#else
        fsemu_window_log(
            "Cannot compensate for window borders size (SDL < 2.0.5)");
#endif
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
#endif

/*
void fsemu_set_sdl_window(SDL_Window *window)
{
    printf("[FSEMU] Register SDL window\n");
    fsemu_sdl_window = window;
}
*/

/*
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
*/