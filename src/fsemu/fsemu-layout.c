#define FSEMU_INTERNAL
#include "fsemu-layout.h"

#include "fsemu-common.h"
#include "fsemu-option.h"
#include "fsemu-options.h"
#include "fsemu-titlebar.h"
// FIXME: Maybe we want to remove this dependency and have the
// window depend on/update the layout when it changes size?
#include "fsemu-window.h"

#include <math.h>

/*
 * FIXME: TOOD: When performance GUI is enabled, the video display area
 * must be shrunkfrom the sides so the performance GUI is visible without
 * overlaying the game display?
 */

static struct {
    int width;
    int height;
    int video_width;
    int video_height;
    // double video_aspect;
    double pixel_aspect;
    bool nonsquare_pixels;
    int stretch_mode;
} fsemu_layout;

static int g_fsemu_client_margins[4];

static void fsemu_layout_read_options(void)
{
    int stretch_mode;
    if (fsemu_option_read_int(FSEMU_OPTION_STRETCH_MODE, &stretch_mode)) {
        fsemu_layout_set_stretch_mode(stretch_mode);
    }
}

void fsemu_layout_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_layout_read_options();
}

void fsemu_layout_set_size(fsemu_size_t *size)
{
    fsemu_layout_set_size_2(size->w, size->h);
}

void fsemu_layout_set_size_2(int width, int height)
{
    fsemu_layout.width = width;
    fsemu_layout.height = height;
}

void fsemu_layout_set_client_margins(int margins[4])
{
    for (int i = 0; i < 4; i++) {
        g_fsemu_client_margins[0] = margins[0];
    }
}

void fsemu_layout_set_video_size(int width, int height)
{
    fsemu_layout.video_width = width;
    fsemu_layout.video_height = height;
}

int fsemu_layout_stretch_mode(void)
{
    return fsemu_layout.stretch_mode;
}

int fsemu_layout_cycle_stretch_mode(void)
{
    if (++fsemu_layout.stretch_mode == FSEMU_STRETCH_MODE_MAX) {
        fsemu_layout.stretch_mode = 0;
    }
    // Temporary
    if (fsemu_layout.stretch_mode == FSEMU_STRETCH_MODE_SQUARE_PIXELS) {
        /*
        if (fsemu_layout.pixel_aspect == 0) {
            fsemu_log("(Unknown pixels aspect ratio)\n");
            fsemu_layout.stretch_mode = 0;
        }
        if (fabs(fsemu_layout.pixel_aspect - 1.0) < 0.01) {
            fsemu_log("(Aspect correct pixels are already square)\n");
            fsemu_layout.stretch_mode = 0;
        }
        */
        if (fsemu_layout.nonsquare_pixels == 0) {
            fsemu_log("Ignoring square pixels mode (using display aspect)\n");
            fsemu_layout.stretch_mode = 0;
        }
    }
    fsemu_log("stretch_mode %d\n", fsemu_layout.stretch_mode);
    return fsemu_layout.stretch_mode;
}

void fsemu_layout_set_stretch_mode(int stretch_mode)
{
    if (stretch_mode < 0 || stretch_mode >= FSEMU_STRETCH_MODE_MAX) {
        fsemu_log("WARNING: Invalid stretch mode\n");
        return;
    }
    fsemu_layout.stretch_mode = stretch_mode;
}

#if 0
void fsemu_layout_set_video_aspect(double aspect)
{
    fsemu_layout.video_aspect = aspect;
}
#endif

void fsemu_layout_set_pixel_aspect(double aspect)
{
    fsemu_layout.pixel_aspect = aspect;
    if (fabs(fsemu_layout.pixel_aspect - 1.0) > 0.01) {
        fsemu_layout.nonsquare_pixels = true;
    } else {
        fsemu_layout.nonsquare_pixels = false;
    }
}

double fsemu_layout_pixel_aspect(void)
{
    if (fsemu_layout.pixel_aspect == 0) {
        return 1.0;
    }
    return fsemu_layout.pixel_aspect;
}

void fsemu_layout_client_rect(fsemu_rect_t *rect)
{
    // fsemu_size size;
    // fsemu_window_size(&size);
    int titlebar = fsemu_titlebar_static_height();

    rect->x = 0;
    rect->y = titlebar;
    rect->w = fsemu_layout.width;
    rect->h = fsemu_layout.height - titlebar;

    // printf("%d %d (layout)\n", fsemu_layout.width, fsemu_layout.height);
}

void fsemu_layout_client_offset_scale(double *ox,
                                      double *oy,
                                      double *sx,
                                      double *sy)
{
    fsemu_rect_t rect;
    fsemu_layout_client_rect(&rect);
    // printf("client rect %d %d %d %d (%d %d)\n", rect.x, rect.y, rect.w,
    // rect.h, fsemu_layout.width, fsemu_layout.height);
    *ox = (double) rect.x / fsemu_layout.width;
    *oy = -(double) rect.y / fsemu_layout.height;
    *sx = (double) rect.w / fsemu_layout.width;
    *sy = (double) rect.h / fsemu_layout.height;
}

void fsemu_layout_video_rect(fsemu_rect_t *rect)
{
    fsemu_rect_t client_rect;
    fsemu_layout_client_rect(&client_rect);
    // printf("layoyt client rect %d %d %d %d\n",
    //        client_rect.x, client_rect.y, client_rect.w, client_rect.h);

    double scale_x = 1.0;
    double scale_y = 1.0;

    int stretch_mode = fsemu_layout_stretch_mode();

    if (stretch_mode == FSEMU_STRETCH_MODE_FILL_SCREEN) {
    } else {
        double pixel_aspect;
        if (stretch_mode == FSEMU_STRETCH_MODE_SQUARE_PIXELS) {
            pixel_aspect = 1.0;
        } else {
            pixel_aspect = fsemu_layout_pixel_aspect();
        }
        double initial_aspect = (double) client_rect.w / client_rect.h;
        double aspect =
            ((double) fsemu_layout.video_width / fsemu_layout.video_height) /
            pixel_aspect;
        if (aspect < initial_aspect) {
            scale_x = aspect / initial_aspect;
        } else {
            scale_y = initial_aspect / aspect;
        }
    }

    rect->w = client_rect.w * scale_x;
    rect->h = client_rect.h * scale_y;
    rect->x = client_rect.x + (client_rect.w - rect->w) / 2;
    rect->y = client_rect.y + (client_rect.h - rect->h) / 2;

    // printf("layout video rect %d %d %d %d\n",
    //        rect->x, rect->y, rect->w, rect->h);

    /*
    fsemu_size size;
    fsemu_window_size(&size);
    rect->x = 0;
    rect->y = 0;
    // rect->w = size.w;
    // rect->h = size.h;
    rect->w = 752;
    rect->h = 572;
    */
}

int fsemu_scale_mode(void)
{
    return FSEMU_MAX_SCALING;
}

bool fsemu_custom_frame(void)
{
    static bool custom_frame = false;
    static int initialized = 0;
    if (!initialized) {
        if (getenv("FSE_CUSTOM_FRAME")) {
            if (getenv("FSE_CUSTOM_FRAME")[0] == '1') {
                custom_frame = 1;
            }
        }
    }
    return custom_frame;
}

int fsemu_stretch_clamp(void)
{
    return 10;
}
