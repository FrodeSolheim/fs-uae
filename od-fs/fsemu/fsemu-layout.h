#ifndef FSEMU_LAYOUT_H_
#define FSEMU_LAYOUT_H_

#include <stdbool.h>

#include "fsemu-common.h"
#include "fsemu-types.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_layout_init(void);

void fsemu_layout_set_size(fsemu_size_t* size);
void fsemu_layout_set_size_2(int width, int height);

void fsemu_layout_set_client_margins(int margins[4]);

void fsemu_layout_set_video_size(int width, int height);
// void fsemu_layout_set_video_aspect(double aspect);
void fsemu_layout_set_pixel_aspect(double aspect);

double fsemu_layout_pixel_aspect(void);

void fsemu_layout_client_area(fsemu_rect_t* rect);

// FIXME: Deprecated
#define fsemu_layout_client_rect fsemu_layout_client_area

void fsemu_layout_client_offset_scale(double* ox, double* oy, double* sx, double* sy);

void fsemu_layout_video_area(fsemu_rect_t* rect);
void fsemu_layout_video_rect(fsemu_rect_t* rect);

int fsemu_layout_stretch_mode(void);
int fsemu_layout_cycle_stretch_mode(void);
void fsemu_layout_set_stretch_mode(int stretch_mode);

int fsemu_layout_zoom_mode(void);
int fsemu_layout_cycle_zoom_mode(void);
void fsemu_layout_set_zoom_mode(int zoom_mode);

int fsemu_scale_mode(void);
void fsemu_set_scale_mode(int scale_mode);

int fsemu_stretch_clamp(void);
bool fsemu_custom_frame(void);

enum { FSEMU_MAX_SCALING };

enum {
    FSEMU_STRETCH_MODE_CORRECT_ASPECT,
    FSEMU_STRETCH_MODE_FILL_SCREEN,
    FSEMU_STRETCH_MODE_SQUARE_PIXELS,
    FSEMU_STRETCH_MODE_MAX  // Do not use
};

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_LAYOUT_H_
