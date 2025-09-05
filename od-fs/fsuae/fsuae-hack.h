#ifndef FSUAE_HACK_H
#define FSUAE_HACK_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// For adding quick-and dirty global variables - usually for testing
// As in; "quick hacks"...

extern int hack_window_width;
extern int hack_window_height;
extern int hack_window_rect_x;
extern int hack_window_rect_y;
extern int hack_window_rect_w;
extern int hack_window_rect_h;

extern bool hack_ntsc_mode;
extern int hack_integer_scaling_mode;
extern int hack_auto_crop_mode;

#ifdef __cplusplus
}
#endif

#endif  // FSUAE_HACK_H
