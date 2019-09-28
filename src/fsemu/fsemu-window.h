#ifndef FSEMU_WINDOW_H_
#define FSEMU_WINDOW_H_

#include "fsemu/fsemu-common.h"
#include "fsemu/fsemu-log.h"
#include "fsemu/fsemu-types.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_window_init(void);
void fsemu_window_work(int timeout);

bool fsemu_window_fullscreen(void);
void fsemu_window_set_fullscreen(bool fullscreen);

void fsemu_assert_window(void);
void fsemu_window_size(fsemu_size *size);
void fsemu_set_window_size(int width, int height);
void fsemu_set_window_fullscreen(bool fullscreen);
void fsemu_set_sdl_window(SDL_Window *window);
SDL_Window *fsemu_create_sdl_window(const char *title);

#ifdef FSEMU_INTERNAL

#define fsemu_window_log(format, ...) \
    fsemu_log("[FSEMU] [WINDW] " format, ##__VA_ARGS__)

#endif

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_WINDOW_H_
