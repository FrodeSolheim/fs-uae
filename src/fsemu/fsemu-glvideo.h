#ifndef FSEMU_GLVIDEO_H_
#define FSEMU_GLVIDEO_H_

#include "fsemu-common.h"
#include "fsemu-gui.h"
#include "fsemu-sdl.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_glvideo_init(void);

void fsemu_glvideo_work(int timeout_us);

void fsemu_glvideo_render_gui_early(fsemu_gui_item_t *items);
void fsemu_glvideo_render(void);
void fsemu_glvideo_render_gui(fsemu_gui_item_t *items);

void fsemu_glvideo_display(void);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_GLVIDEO_H_
