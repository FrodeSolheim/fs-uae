#ifndef FSAPP_MAINLOOP_H
#define FSAPP_MAINLOOP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <SDL3/SDL.h>

void fsapp_mainloop_handle_event(SDL_Event* event);

void fsapp_mainloop_iteration(void);

#ifdef __cplusplus
}
#endif

#endif  // FSAPP_MAINLOOP_H
