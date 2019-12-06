#ifndef FSEMU_WINDOW_H_
#define FSEMU_WINDOW_H_

#include "fsemu-common.h"
#include "fsemu-log.h"
#include "fsemu-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** This can be called before fsemu_window_init. */
void fsemu_window_set_title(const char *title);
void fsemu_window_set_fullscreen(bool fullscreen);

void fsemu_window_init(void);
void fsemu_window_work(int timeout);

void fsemu_window_initial_rect(fsemu_rect_t *rect);
void fsemu_window_initial_fullscreen_rect(fsemu_rect_t *rect);

const char *fsemu_window_title(void);

bool fsemu_window_fullscreen(void);
void fsemu_window_toggle_fullscreen(void);

void fsemu_assert_window(void);

bool fsemu_window_active(void);

void fsemu_window_size(fsemu_size_t *size);
void fsemu_window_set_size(fsemu_size_t *size);
void fsemu_window_set_size_2(int width, int height);

// void fsemu_set_window_size(int width, int height);

// void fsemu_set_window_fullscreen(bool fullscreen);
// void fsemu_set_sdl_window(SDL_Window *window);
// SDL_Window *fsemu_create_sdl_window(const char *title);

#ifdef FSEMU_INTERNAL

void fsemu_window_set_active(bool active);

#define fsemu_window_log(format, ...) \
    fsemu_log("[FSEMU] [WINDW] " format, ##__VA_ARGS__)

#endif

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_WINDOW_H_
