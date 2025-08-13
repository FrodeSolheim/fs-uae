#ifndef FSEMU_EVENTS_H_
#define FSEMU_EVENTS_H_

#include "fsemu-common.h"
#include "fsemu-types.h"

#ifdef __cplusplus
extern "C" {
#endif

bool fsemu_handle_sdl_event(SDL_Event* event);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_EVENTS_H_
