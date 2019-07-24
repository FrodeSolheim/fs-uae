#ifndef FSEMU_LAYOUT_H_
#define FSEMU_LAYOUT_H_

#include <stdbool.h>
#include "fsemu/fsemu-common.h"
#include "fsemu/fsemu-types.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_layout_set_size(int width, int height);
void fsemu_layout_set_client_margins(int margins[4]);

void fsemu_layout_set_video_size(int width, int height);
// void fsemu_layout_set_video_aspect(double aspect);
void fsemu_layout_set_pixel_aspect(double aspect);

double fsemu_layout_pixel_aspect(void);

void fsemu_layout_client_rect(fsemu_rect *rect);
void fsemu_layout_video_rect(fsemu_rect *rect);

int fsemu_scale_mode(void);
int fsemu_stretch_clamp(void);
bool fsemu_custom_frame(void);

enum { FSEMU_MAX_SCALING };

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_LAYOUT_H_
