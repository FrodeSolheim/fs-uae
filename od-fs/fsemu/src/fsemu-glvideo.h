#ifndef FSEMU_GLVIDEO_H_
#define FSEMU_GLVIDEO_H_

#include "fsemu-common.h"
#include "fsemu-gui.h"
#include "fsemu-sdl.h"

#ifdef __cplusplus
extern "C" {
#endif

// FIXME: Temporary hack
void fsemu_glvideo_set_rect_temp(double left,
                                 double top,
                                 double right,
                                 double bottom);

void fsemu_glvideo_set_frame_rendered_externally(void);

#ifdef FSEMU_INTERNAL

void fsemu_glvideo_init(void);

void fsemu_glvideo_set_size_2(int width, int height);

void fsemu_glvideo_work(int timeout_us);

// This must be called from the video thread.
void fsemu_glvideo_init_gl_state(void);

void fsemu_glvideo_render_gui_early(fsemu_gui_item_t *items);
void fsemu_glvideo_render(void);

void fsemu_glvideo_render_gui(fsemu_gui_item_t *items);

void fsemu_glvideo_display(void);

#endif  // FSEMU_INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_GLVIDEO_H_
