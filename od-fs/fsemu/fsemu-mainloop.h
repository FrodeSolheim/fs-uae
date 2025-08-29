#ifndef FSEMU_MAINLOOP_H
#define FSEMU_MAINLOOP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <SDL3/SDL.h>

void fsemu_mainloop_handle_event(SDL_Event* event);

void fsemu_mainloop_iteration(void);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_MAINLOOP_H