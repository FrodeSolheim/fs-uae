#ifndef FSEMU_SDLWINDOW_H_
#define FSEMU_SDLWINDOW_H_

#include "fsemu/fsemu-common.h"
#include "fsemu/fsemu-sdl.h"

#ifdef FSEMU_SDL

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_sdlwindow_init(void);

/**
 * Run this periodically to performance maintenance tasks such as toggling
 * fullscreen or mouse capture. In general, this should change the actual
 * window state to the desired state.
 */
void fsemu_sdlwindow_update(void);

SDL_Window *fsemu_sdlwindow_create(void);
void fsemu_sdlwindow_set_window(SDL_Window *window);
SDL_Window *fsemu_sdlwindow_window(void);
void fsemu_sdlwindow_show(void);

bool fsemu_sdlwindow_fullscreen(void);
void fsemu_sdlwindow_set_fullscreen(bool fullscreen);

bool fsemu_sdlwindow_handle_event(SDL_Event *event);
bool fsemu_sdlwindow_handle_window_event(SDL_Event *event);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_SDL

#endif  // FSEMU_SDLWINDOW_H_
