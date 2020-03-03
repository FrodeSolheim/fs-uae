#define FSEMU_INTERNAL
#include "fsemu-window.h"

#include "fsemu-layout.h"
#include "fsemu-mouse.h"
#include "fsemu-option.h"
#include "fsemu-options.h"
#include "fsemu-sdlwindow.h"
#include "fsemu-titlebar.h"
#include "fsemu-types.h"
#include "fsemu-util.h"
#include "fsemu-video.h"
#include "fsemu-videothread.h"

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
    fsemu_point_t initial_center;
    double ui_scale;
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

    fsemu_point_t *point = &fsemu_window.initial_center;
    fsemu_option_read_int(FSEMU_OPTION_WINDOW_CENTER_X, &point->x);
    fsemu_option_read_int(FSEMU_OPTION_WINDOW_CENTER_Y, &point->y);
    fsemu_window_log("Initial window center: %d %d\n", point->x, point->y);

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

    // Set default value for ui_scale, it is only changed if a specific scale
    // factor can be deduced for Linux. Must be set _before_
    // fsemu_sdlwindow_create is called.
    fsemu_window.ui_scale = 1.0;

    fsemu_sdlwindow_init();
    fsemu_sdlwindow_create();
    fsemu_sdlwindow_show();

    // Assume window is active upon startup, to avoid a single frame of
    // flickering on the titlebar due to inactive frame being rendered before
    // window activation event occurs.
    fsemu_window.active = true;
}

// ---------------------------------------------------------------------------

void fsemu_window_initial_rect(fsemu_rect_t *rect, double ui_scale)
{
    *rect = fsemu_window.initial_rect;
    if (rect->w == 0 || rect->h == 0) {
        rect->x = -1;
        rect->y = -1;
        rect->w = 960 * ui_scale;
        rect->h = 540 * ui_scale;
        if (fsemu_window.initial_center.x) {
            rect->x = fsemu_window.initial_center.x - rect->w / 2;
        }
        if (fsemu_window.initial_center.y) {
            rect->y = fsemu_window.initial_center.y - rect->h / 2;
        }
    }
    if (!fsemu_titlebar_use_system()) {
        fsemu_window_log(
            "System titlebar is false: increase initial window rect\n");
        rect->h += fsemu_titlebar_unscaled_height() * ui_scale;
    }
}

void fsemu_window_initial_fullscreen_rect(fsemu_rect_t *rect)
{
    *rect = fsemu_window.initial_fullscreen_rect;
    if (rect->w == 0 || rect->h == 0) {
#if 0
        // FIXME... get via monitor module?
        rect->x = -1;
        rect->y = -1;
        rect->w = 960;
        rect->h = 540;
#endif
    }
}

// ---------------------------------------------------------------------------

double fsemu_window_ui_scale(void)
{
    return fsemu_window.ui_scale;
}

void fsemu_window_set_ui_scale(double ui_scale)
{
    fsemu_window.ui_scale = ui_scale;
}

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

// FIXME: Move/refactor
static void fsemu_window_sync_data_to_video_thread(void)
{
    // FIXME: ui / video sync point - not very elegantly put here, do it
    // elsewhere?
    fsemu_size_t window_size;
    fsemu_size_t drawable_size;
    fsemu_rect_t client_area;
    fsemu_rect_t video_area;
    fsemu_rect_t video_rect;

    fsemu_window_size(&window_size);
    fsemu_video_drawable_size(&drawable_size);
    fsemu_layout_client_area(&client_area);
    fsemu_layout_video_area(&video_area);
    fsemu_layout_video_rect(&video_rect);

    fsemu_videothread_lock();
    fsemu_videothread_set_data_from_ui_thread(
        &window_size, &drawable_size, &client_area, &video_area, &video_rect);
    fsemu_videothread_unlock();
}

// ---------------------------------------------------------------------------

void fsemu_window_work(int timeout)
{
    fsemu_sdlwindow_work(timeout);

    // Perform periodic updates, check if fullscreen should be toggled, etc.
    fsemu_sdlwindow_update();

    // printf("fsemu_window_work\n");

    // FIXME: Move/refactor
    fsemu_window_sync_data_to_video_thread();
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
