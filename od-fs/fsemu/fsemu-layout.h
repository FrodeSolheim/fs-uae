#ifndef FSEMU_LAYOUT_H
#define FSEMU_LAYOUT_H

#include <stdbool.h>

#include "fsemu-common.h"
#include "fsemu-types.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_layout_init(void);

void fsemu_layout_set_size(fsemu_size_t *size);
void fsemu_layout_set_size_2(int width, int height);

void fsemu_layout_set_client_margins(int margins[4]);

void fsemu_layout_set_video_size(int width, int height);
// void fsemu_layout_set_video_aspect(double aspect);
void fsemu_layout_set_pixel_aspect(double aspect);

double fsemu_layout_pixel_aspect(void);

// The emulation surface's rectangle in the host window, where the video is laid out. Set on the
// drawing side when the picture rectangle is worked out (fsemu-video.cpp).
void fsemu_layout_set_client_area(const fsemu_rect_t *rect);
void fsemu_layout_client_area(fsemu_rect_t *rect);

// FIXME: Deprecated
#define fsemu_layout_client_rect fsemu_layout_client_area

void fsemu_layout_client_offset_scale(double *ox, double *oy, double *sx, double *sy);

void fsemu_layout_video_area(fsemu_rect_t *rect);
void fsemu_layout_video_rect(fsemu_rect_t *rect);

// The stretch mode option (FSEMU_STRETCH_MODE_*). FILL_SCREEN sets the fit policy to stretch; the
// other modes only choose the pixel aspect when the picture keeps its aspect.
int fsemu_layout_stretch_mode(void);
void fsemu_layout_set_stretch_mode(int stretch_mode);

int fsemu_layout_zoom_mode(void);
int fsemu_layout_cycle_zoom_mode(void);
void fsemu_layout_set_zoom_mode(int zoom_mode);

// Whether the emulated display is NTSC. The layout then uses a pixel aspect of 1.2 instead of 1.0
// (also with integer scaling). The emulator sets it as it posts each frame.
bool fsemu_layout_ntsc_mode(void);
void fsemu_layout_set_ntsc_mode(bool ntsc_mode);

// The picture policies: settings that the top (or fsemu's event hook: Alt+C, Alt+S) changes from
// any thread, atomically, and that fsemu reads where it decides the crop, the fit and the filter.

// Which part of the emulated frame is shown. The emulator reads it when it sets the limits of a
// frame: a fixed area (FIXED, the default) or the area in use (AUTO). Alt+C toggles it (also on
// macOS).
typedef enum {
    FSEMU_LAYOUT_CROP_POLICY_FIXED,
    FSEMU_LAYOUT_CROP_POLICY_AUTO,
} fsemu_layout_crop_policy_t;

fsemu_layout_crop_policy_t fsemu_layout_get_crop_policy(void);
void fsemu_layout_set_crop_policy(fsemu_layout_crop_policy_t policy);

// How the picture is fitted into the emulation surface: keeping its aspect (FIT, the default),
// scaled by whole (low resolution) or half (high resolution or interlaced) steps (INTEGER, Alt+S
// toggles it, also on macOS), or filling the surface (STRETCH, the FILL_SCREEN stretch mode).
typedef enum {
    FSEMU_LAYOUT_FIT_POLICY_FIT,
    FSEMU_LAYOUT_FIT_POLICY_INTEGER,
    FSEMU_LAYOUT_FIT_POLICY_STRETCH,
} fsemu_layout_fit_policy_t;

fsemu_layout_fit_policy_t fsemu_layout_get_fit_policy(void);
void fsemu_layout_set_fit_policy(fsemu_layout_fit_policy_t policy);

// How the picture is sampled when it is scaled: LINEAR (the default) or NEAREST.
typedef enum {
    FSEMU_LAYOUT_FILTER_LINEAR,
    FSEMU_LAYOUT_FILTER_NEAREST,
} fsemu_layout_filter_t;

fsemu_layout_filter_t fsemu_layout_get_filter(void);
void fsemu_layout_set_filter(fsemu_layout_filter_t filter);

int fsemu_scale_mode(void);
void fsemu_set_scale_mode(int scale_mode);

int fsemu_stretch_clamp(void);
bool fsemu_custom_frame(void);

enum { FSEMU_MAX_SCALING };

enum {
    FSEMU_STRETCH_MODE_CORRECT_ASPECT,
    FSEMU_STRETCH_MODE_FILL_SCREEN,
    FSEMU_STRETCH_MODE_SQUARE_PIXELS,
    FSEMU_STRETCH_MODE_MAX // Do not use
};

#ifdef __cplusplus
}
#endif

#endif /* FSEMU_LAYOUT_H */
