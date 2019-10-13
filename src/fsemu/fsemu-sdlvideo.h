#ifndef FSEMU_SDLVIDEO_H_
#define FSEMU_SDLVIDEO_H_

#include "fsemu-common.h"
#include "fsemu-gui.h"
#include "fsemu-sdl.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_sdlvideo_init(void);

void fsemu_sdlvideo_work(int timeout_us);

void fsemu_sdlvideo_render_gui_early(fsemu_gui_item_t *items);
void fsemu_sdlvideo_render(void);
void fsemu_sdlvideo_render_gui(fsemu_gui_item_t *items);

void fsemu_sdlvideo_display(void);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_SDLVIDEO_H_
