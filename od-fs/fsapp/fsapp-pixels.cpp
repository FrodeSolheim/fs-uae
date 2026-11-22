/*
 * The conversions between SDL's window coordinates and pixels (fsapp-pixels.h).
 *
 * Copyright (c) 2026 Frode Solheim
 */

#include "fsapp-pixels.h"

#include <cmath>

static double get_axis_density(int length, int pixel_length)
{
    if (length <= 0 || pixel_length <= 0) {
        return 1.0;
    }
    return (double)pixel_length / length;
}

void fsapp_pixels_convert_from_window_position(const fsapp_pixels_sizes_t *sizes, float x, float y,
                                               float *pixel_x, float *pixel_y)
{
    *pixel_x = (float)(x * get_axis_density(sizes->width, sizes->pixel_width));
    *pixel_y = (float)(y * get_axis_density(sizes->height, sizes->pixel_height));
}

// REVIEW: Rounded to the nearest. With a density measured from rounded sizes (Wayland's 1001/667
// for 1.5), a length other than the window's own can come back one window coordinate off from the
// one SDL would turn into that many pixels. Check on GNOME Wayland at 125 % and 150 % that the
// window opens at the size asked for: the log's SDL_CreateWindow pixels against Host window's.
int fsapp_pixels_convert_to_window_length(int pixels, double density)
{
    if (density <= 0.0) {
        density = 1.0;
    }
    int length = (int)std::lround(pixels / density);
    if (length == 0 && pixels > 0) {
        return 1;
    }
    return length;
}

// A display scale or a ratio in 120ths (Wayland's unit for fractional scales), at least 1; 120 for
// a value of 0 or less. The conversions compute in 120ths, so that an exact result is not off by
// one from the value's binary fraction.
static long get_120ths(double value)
{
    if (value <= 0.0) {
        return 120;
    }
    long count = std::lround(value * 120.0);
    return count < 1 ? 1 : count;
}

int fsapp_pixels_scale_length(int length, float display_scale)
{
    double scale = get_120ths(display_scale) / 120.0;
    int pixels = (int)std::lround(length * scale);
    if (pixels == 0 && length > 0) {
        return 1;
    }
    return pixels;
}

double fsapp_pixels_get_ratio(fsapp_pixels_scale_support_t support, float display_scale)
{
    if (support == FSAPP_PIXELS_SCALE_ANY) {
        return 1.0;
    }
    return get_120ths(display_scale) / 120.0;
}

int fsapp_pixels_convert_to_buffer_length(int device_length, double ratio)
{
    return (int)((long long)device_length * 120 / get_120ths(ratio));
}

// A buffer pixel edge in device pixels at a ratio in 120ths, rounded to the nearest, halves away
// from zero.
static int to_device_edge(int edge, long ratio_120ths)
{
    long long product = (long long)edge * ratio_120ths;
    long long rounded = (2 * (product < 0 ? -product : product) + 120) / 240;
    return (int)(product < 0 ? -rounded : rounded);
}

void fsapp_pixels_get_device_rectangle(const fsapp_pixels_rectangle_t *rectangle, double ratio,
                                       fsapp_pixels_rectangle_t *device)
{
    long ratio_120ths = get_120ths(ratio);
    int left = to_device_edge(rectangle->x, ratio_120ths);
    int top = to_device_edge(rectangle->y, ratio_120ths);
    device->x = left;
    device->y = top;
    device->width = to_device_edge(rectangle->x + rectangle->width, ratio_120ths) - left;
    device->height = to_device_edge(rectangle->y + rectangle->height, ratio_120ths) - top;
}

// REVIEW: The zones are scaled by the display scale of the last configure, which fsgui's title bar
// follows only from its next UI frame, and the edges (4) and corners (8) were sized at scale 1.
// Check on Windows at 150 % and on a Retina Mac that the title bar drags only between the free
// square at the left and the buttons, and that the edges resize within 4 design units of them, also
// just after the window has moved to a monitor with another scale.
fsapp_pixels_hit_t fsapp_pixels_hit_test(int pixel_width, int pixel_height, float x, float y,
                                         float display_scale)
{
    float title_bar =
        (float)fsapp_pixels_scale_length(FSAPP_PIXELS_TITLE_BAR_HEIGHT, display_scale);
    float edge = (float)fsapp_pixels_scale_length(4, display_scale);
    float corner = (float)fsapp_pixels_scale_length(8, display_scale);
    float w = (float)pixel_width;
    float h = (float)pixel_height;

    if (y >= h - edge) {
        if (x < corner) {
            return FSAPP_PIXELS_HIT_RESIZE_BOTTOM_LEFT;
        }
        if (x >= w - corner) {
            return FSAPP_PIXELS_HIT_RESIZE_BOTTOM_RIGHT;
        }
        return FSAPP_PIXELS_HIT_RESIZE_BOTTOM;
    }
    if (y < title_bar) {
        if (x < title_bar || x >= w - title_bar * 3) {
            return FSAPP_PIXELS_HIT_NORMAL;
        }
        return FSAPP_PIXELS_HIT_DRAGGABLE;
    }
    if (x < edge) {
        return FSAPP_PIXELS_HIT_RESIZE_LEFT;
    }
    if (x >= w - edge) {
        return FSAPP_PIXELS_HIT_RESIZE_RIGHT;
    }
    return FSAPP_PIXELS_HIT_NORMAL;
}

void fsapp_pixels_get_surface_configure(const fsapp_pixels_configure_t *window,
                                        fsapp_pixels_scale_support_t support,
                                        fsapp_pixels_configure_t *surface)
{
    double ratio = fsapp_pixels_get_ratio(support, window->scale);
    surface->width = fsapp_pixels_convert_to_buffer_length(window->width, ratio);
    surface->height = fsapp_pixels_convert_to_buffer_length(window->height, ratio);
    surface->scale = support == FSAPP_PIXELS_SCALE_ANY ? window->scale : 1.0f;
}

bool fsapp_pixels_take_configure(fsapp_pixels_configure_state_t *state,
                                 const fsapp_pixels_configure_t *current)
{
    if (state->sent && current->width == state->last.width &&
        current->height == state->last.height && current->scale == state->last.scale) {
        return false;
    }
    state->sent = true;
    state->last = *current;
    return true;
}

void fsapp_pixels_convert_to_buffer_position(const fsapp_pixels_rectangle_t *device, double ratio,
                                             double x, double y, double *buffer_x, double *buffer_y)
{
    double rounded_ratio = get_120ths(ratio) / 120.0;
    *buffer_x = (x - device->x) / rounded_ratio;
    *buffer_y = (y - device->y) / rounded_ratio;
}

bool fsapp_pixels_is_whole_ratio(double ratio)
{
    return get_120ths(ratio) % 120 == 0;
}
