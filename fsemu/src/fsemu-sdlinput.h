#ifndef FSEMU_SDLINPUT_H_
#define FSEMU_SDLINPUT_H_

#include "fsemu-common.h"
#include "fsemu-sdl.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_sdlinput_init(void);
void fsemu_sdlinput_work(void);

bool fsemu_sdlinput_handle_event(SDL_Event *event);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_SDLINPUT_H_
