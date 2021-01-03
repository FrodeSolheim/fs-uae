#ifndef FSEMU_WINDOW_H_
#define FSEMU_WINDOW_H_

#include "fsemu-common.h"
#include "fsemu-log.h"
#include "fsemu-types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    FSEMU_WINDOW_DRIVER_NULL,
    FSEMU_WINDOW_DRIVER_SDL,
} fsemu_window_driver_t;

/** This can be called before fsemu_window_init. */
void fsemu_window_set_title(const char *title);
void fsemu_window_set_fullscreen(bool fullscreen);

void fsemu_window_init(void);
void fsemu_window_work(int timeout);

void fsemu_window_initial_rect(fsemu_rect_t *rect, double ui_scale);
void fsemu_window_initial_fullscreen_rect(fsemu_rect_t *rect);

const char *fsemu_window_title(void);

bool fsemu_window_fullscreen(void);
void fsemu_window_toggle_fullscreen(void);

void fsemu_assert_window(void);

bool fsemu_window_active(void);

void fsemu_window_size(fsemu_size_t *size);
void fsemu_window_set_size(fsemu_size_t *size);
void fsemu_window_set_size_2(int width, int height);

double fsemu_window_ui_scale(void);
void fsemu_window_set_ui_scale(double ui_scale);

// void fsemu_set_window_size(int width, int height);

// void fsemu_set_window_fullscreen(bool fullscreen);
// void fsemu_set_sdl_window(SDL_Window *window);
// SDL_Window *fsemu_create_sdl_window(const char *title);

#ifdef FSEMU_INTERNAL

void fsemu_window_set_active(bool active);

extern int fsemu_window_log_level;

#define fsemu_window_log(format, ...)                             \
    if (FSEMU_LIKELY(fsemu_window_log_level >= FSEMU_LOG_INFO)) { \
        fsemu_log("[FSEMU] [WINDW] " format, ##__VA_ARGS__);      \
    }

#define fsemu_window_log_debug(format, ...)                          \
    if (FSEMU_UNLIKELY(fsemu_window_log_level >= FSEMU_LOG_DEBUG)) { \
        fsemu_log("[FSEMU] [WINDW] " format, ##__VA_ARGS__);         \
    }

/*
#define fsemu_window_log(format, ...) \
    fsemu_log("[FSEMU] [WINDW] " format, ##__VA_ARGS__)
*/

#endif  // FSEMU_INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_WINDOW_H_
