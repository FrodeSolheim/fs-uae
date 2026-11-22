/*
 * The conversions between SDL's window coordinates and pixels.
 *
 * fsapp's surface side (the compositor, routing's hit test) is in pixels of the host window's
 * drawable (device pixels). Each surface's client works in its own buffer pixels, which cover a
 * ratio of device pixels each (1 for a surface with the scale support "any"): fsapp draws and hit
 * tests a surface at its device rectangle and gives it pointer positions in its buffer pixels, with
 * the conversions below. SDL gives the host window's size and mouse positions in window coordinates
 * (pixels on Windows, X11 and KMSDRM, points on macOS and Wayland). fsapp converts them to pixels
 * when an event arrives, and converts back only in its host window operations (creating the
 * window, its hit test), with these functions.
 *
 * The density is taken per axis from the window's two sizes, not from SDL_GetWindowPixelDensity,
 * which is one ratio of rounded sizes. Positions become pixels as floats, not rounded; lengths
 * going back to window coordinates are rounded to the nearest, halves away from zero.
 *
 * A configure tells the UI the host window's size and a scale. The host window side has one when
 * the window is created and whenever its size in pixels or the display scale changes, from
 * whichever of SDL's window events comes first. The host window's handler gets that one; each
 * surface with a handler gets its own, made from it for its scale support
 * (fsapp_pixels_get_surface_configure). fsapp_pixels_take_configure compares a configure with what
 * was last sent to the same receiver, so the other events of the same change, and changes that
 * leave a surface's configure as it was, send nothing.
 *
 * The borderless host window's hit test compares a point in pixels with zones in pixels: lengths in
 * design units scaled by the display scale, as fsgui scales its title bar.
 *
 * A pure unit with no SDL, so that its tests (od-fs/tests/fsapp) compile its source file alone.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#ifndef FSAPP_PIXELS_H
#define FSAPP_PIXELS_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// The host window's size in window coordinates (SDL_GetWindowSize) and in pixels
// (SDL_GetWindowSizeInPixels).
typedef struct fsapp_pixels_sizes {
    int width;
    int height;
    int pixel_width;
    int pixel_height;
} fsapp_pixels_sizes_t;

// Converts a position in window coordinates (a mouse position, a point of the window's hit test) to
// pixels, with the density per axis: the size in pixels over the size in window coordinates, or 1
// along an axis where either is 0 or less.
void fsapp_pixels_convert_from_window_position(const fsapp_pixels_sizes_t *sizes, float x, float y,
                                               float *pixel_x, float *pixel_y);

// Converts a length (a width or a height) in pixels to window coordinates at a density (a predicted
// density before the window exists; 1 when 0 or less), rounded, and at least 1 when the length is
// more than 0.
int fsapp_pixels_convert_to_window_length(int pixels, double density);

// A length in design units (the UI's sizes at a display scale of 1) in pixels at a display scale:
// the scale rounded to 1/120 (Wayland's unit for fractional scales), so that a scale off in its
// last digits (Wayland's, from rounded sizes) counts as the scale it was meant to be, and the
// length rounded to the nearest, halves away from zero, and at least 1 when more than 0. A scale of
// 0 or less counts as 1. fsgui rounds its sizes by the same rule (fsgui/scale.py).
int fsapp_pixels_scale_length(int length, float display_scale);

// Where a point of the borderless host window is: the title bar drags the window except where its
// buttons are (and the space kept free at its left), and the side and bottom edges and the bottom
// corners resize it.
typedef enum fsapp_pixels_hit {
    FSAPP_PIXELS_HIT_NORMAL,
    FSAPP_PIXELS_HIT_DRAGGABLE,
    FSAPP_PIXELS_HIT_RESIZE_LEFT,
    FSAPP_PIXELS_HIT_RESIZE_RIGHT,
    FSAPP_PIXELS_HIT_RESIZE_BOTTOM,
    FSAPP_PIXELS_HIT_RESIZE_BOTTOM_LEFT,
    FSAPP_PIXELS_HIT_RESIZE_BOTTOM_RIGHT,
} fsapp_pixels_hit_t;

// The height of the host window's title bar in design units (_fsapp.TITLE_BAR_HEIGHT, which the
// main window's title bar in Python takes).
#define FSAPP_PIXELS_TITLE_BAR_HEIGHT 36

// The hit test of a point in pixels in a host window of a size in pixels. The zones are lengths in
// design units scaled with fsapp_pixels_scale_length, so that they match the title bar: its
// height (FSAPP_PIXELS_TITLE_BAR_HEIGHT) and its three square buttons at the right, a square kept
// free at the left, the edges 4 and the bottom corners 8.
fsapp_pixels_hit_t fsapp_pixels_hit_test(int pixel_width, int pixel_height, float x, float y,
                                         float display_scale);

// A surface's scale support: what its client says it can draw at. fsapp chooses its surface scale
// from it: 1 for "1 only", the display scale for "any". Its ratio is the display scale over its
// surface scale: how many device pixels (pixels of the host window's drawable) one of its buffer
// pixels covers.
typedef enum fsapp_pixels_scale_support {
    FSAPP_PIXELS_SCALE_ONE,
    FSAPP_PIXELS_SCALE_ANY,
} fsapp_pixels_scale_support_t;

// The ratio of a surface with the scale support at a display scale: the display scale rounded to
// 1/120 (as fsapp_pixels_scale_length rounds it) for "1 only", 1 for "any". A scale of 0 or less
// counts as 1.
double fsapp_pixels_get_ratio(fsapp_pixels_scale_support_t support, float display_scale);

// A length in device pixels (0 or more) in buffer pixels at a ratio (fsapp_pixels_get_ratio; 0 or
// less counts as 1): rounded down, so that a surface of that size fits.
int fsapp_pixels_convert_to_buffer_length(int device_length, double ratio);

// A rectangle: a surface's in its buffer pixels (its position and size), or in device pixels.
typedef struct fsapp_pixels_rectangle {
    int x;
    int y;
    int width;
    int height;
} fsapp_pixels_rectangle_t;

// A surface's device rectangle (where it is drawn and hit tested) from its rectangle in its buffer
// pixels and its ratio (0 or less counts as 1): each edge times the ratio, rounded to the nearest,
// halves away from zero, and the size the difference of the rounded edges, so that adjacent
// surfaces share their edges. An empty rectangle stays empty.
void fsapp_pixels_get_device_rectangle(const fsapp_pixels_rectangle_t *rectangle, double ratio,
                                       fsapp_pixels_rectangle_t *device);

// A position in device pixels relative to a surface, in its buffer pixels: the offset from its
// device rectangle's origin divided by its ratio (0 or less counts as 1).
// REVIEW: From the rounded device rectangle, so at a fractional ratio a position near the far edge
// can be up to half a device pixel (over the ratio) off the buffer pixel drawn there.
void fsapp_pixels_convert_to_buffer_position(const fsapp_pixels_rectangle_t *device, double ratio,
                                             double x, double y, double *buffer_x,
                                             double *buffer_y);

// Whether a ratio is whole (to 1/120; 0 or less counts as 1), so that a surface is drawn with
// nearest sampling, else linear.
// REVIEW: Nearest at every whole ratio, 1 included (the same pixels as linear at 1).
bool fsapp_pixels_is_whole_ratio(double ratio);

// What a configure carries: for the host window's handler, the host window's size in device pixels
// and the display scale (SDL_GetWindowDisplayScale); for a surface, the host window's size in its
// buffer pixels and its surface scale.
typedef struct fsapp_pixels_configure {
    int width;
    int height;
    float scale;
} fsapp_pixels_configure_t;

// The configure of a surface with the scale support, from the host window's: the host window's size
// in the surface's buffer pixels (fsapp_pixels_convert_to_buffer_length at its ratio) and its
// surface scale, 1 for "1 only" and the display scale as it is for "any".
// REVIEW: The host window's size in the surface's buffer pixels, rounded down, rather than the
// surface's own size, since a client needs the host window's size to place itself.
void fsapp_pixels_get_surface_configure(const fsapp_pixels_configure_t *window,
                                        fsapp_pixels_scale_support_t support,
                                        fsapp_pixels_configure_t *surface);

// The configure last sent: none yet when sent is false.
typedef struct fsapp_pixels_configure_state {
    bool sent;
    fsapp_pixels_configure_t last;
} fsapp_pixels_configure_state_t;

// Whether current is to be sent as a configure: when none has been sent yet, or its size or scale
// differs from the last sent. When it is, it becomes the last sent.
bool fsapp_pixels_take_configure(fsapp_pixels_configure_state_t *state,
                                 const fsapp_pixels_configure_t *current);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSAPP_PIXELS_H */
