#ifndef FSEMU_SDLINPUT_H
#define FSEMU_SDLINPUT_H

#include <SDL3/SDL.h>

#include "fsemu-common.h"
#include "fsemu-sdl.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_sdlinput_init(void);
void fsemu_sdlinput_work(void);

bool fsemu_sdlinput_handle_event(SDL_Event* event);

void fsemu_sdlinput_add_controller(SDL_Gamepad* sdl_controller);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_SDLINPUT_H
