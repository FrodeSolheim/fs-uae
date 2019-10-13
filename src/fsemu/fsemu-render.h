#if 0

#ifndef FSEMU_RENDER_H_
#define FSEMU_RENDER_H_

#include "fsemu-common.h"

typedef struct fsemu_render {
    void (*rectangle)(int x, int y, int w, int h, int c);
} fsemu_render_t;

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_render_init(void);

/**
 * This render pass runs before the video frame is rendered.
 */
void fsemu_render_render_early(void);


/**
 * This render pass runs after the video frame is rendered, and can render
 * transparent overlays on top of the video frame.
 */
void fsemu_render_render_late(void);

#ifdef FSEMU_INTERNAL

#define fsemu_render_log(format, ...) \
    fsemu_log("[FSEMU] [RENDR] " format, ##__VA_ARGS__)

#endif  // FSEMU_INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_RENDER_H_

#endif
