#define FSEMU_INTERNAL
#include "fsemu-layout.h"

#include "fsemu-common.h"
#include "fsemu-option.h"
#include "fsemu-options.h"
#include "fsemu-osmenu.h"
#include "fsemu-theme.h"
#include "fsemu-titlebar.h"
// FIXME: Maybe we want to remove this dependency and have the
// window depend on/update the layout when it changes size?
#include <math.h>

#include "fsemu-window.h"

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
    int zoom_mode;
    // FIXME: Not currently used
    int client_margins[4];
} fsemu_layout;

enum {
    FSEMU_ZOOM_MODE_NONE,
    FSEMU_ZOOM_MODE_1,
    FSEMU_ZOOM_MODE_2,
    FSEMU_ZOOM_MODE_3,
    FSEMU_ZOOM_MODE_MAX,
};

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
    fsemu_theme_module_init();
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
        fsemu_layout.client_margins[0] = margins[0];
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

int fsemu_layout_zoom_mode(void)
{
    return fsemu_layout.zoom_mode;
}

int fsemu_layout_cycle_zoom_mode(void)
{
    if (++fsemu_layout.zoom_mode == FSEMU_ZOOM_MODE_MAX) {
        fsemu_layout.zoom_mode = 0;
    }
    fsemu_log("zoom_mode %d\n", fsemu_layout.zoom_mode);
    return fsemu_layout.zoom_mode;
}

void fsemu_layout_set_zoom_mode(int zoom_mode)
{
    if (zoom_mode < 0 || zoom_mode >= FSEMU_ZOOM_MODE_MAX) {
        fsemu_error("Invalid zoom mode in %s\n", __func__);
        return;
    }
    fsemu_layout.zoom_mode = zoom_mode;
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

void fsemu_layout_client_area(fsemu_rect_t *rect)
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

void fsemu_layout_video_area(fsemu_rect_t *rect)
{
    // fsemu_rect_t video_area;
    fsemu_layout_client_area(rect);

    // fsemu_drect_t temp_drect;
    // temp_drect.x = client_area.x;
    // temp_drect.y = client_area.y;
    // temp_drect.w = client_area.w;
    // temp_drect.h = client_area.h;

    int ttop, tright, tbottom, tleft;
    if (fsemu_layout.zoom_mode == 0) {
        fsemu_theme_margins(&ttop, &tright, &tbottom, &tleft);
    } else {
        ttop = 0;
        tright = 0;
        tbottom = 0;
        tleft = 0;
    }

    // double margin_scale = video_area.h / 1080.0;
    double margin_scale = rect->h / 1080.0;

    ttop = tbottom * margin_scale + 0.5;
    tright = tright * margin_scale + 0.5;
    tbottom = tbottom * margin_scale + 0.5;
    tleft = tleft * margin_scale + 0.5;

    // video_area.x += tleft;
    // video_area.w -= tleft + tright;
    // video_area.y += ttop;
    // video_area.h -= ttop + tbottom;
    rect->x += tleft;
    rect->w -= tleft + tright;
    rect->y += ttop;
    rect->h -= ttop + tbottom;

    if (fsemu_osmenu_open()) {
        // Displace the video rectangle when the menu is open, so we get both
        // the entire video display and menu visible, with 4:3 video on a
        // 16:9 display.
        rect->x -= 240 * rect->h / 1080.0;
    }

    // // FIXME: 1080 coordinates
    // // temp_drect.x += tleft; // FIXME
    // temp_drect.y += ttop * client_rect.h / 1080.0;
    // // temp_drect.w -= tleft + tright;  // FIXME
    // temp_drect.h -= (ttop + tbottom) * client_rect.h / 1080.0;

    // temp_drect.x += tleft * client_area.w / 1080.0;
    // temp_drect.w += (tleft + tright) * client_area.w / 1080.0;
#if 0
    printf("------ layoyt video area %d %d %d %d\n",
           rect->x,
           rect->y,
           rect->w,
           rect->h);
#endif
    // printf("temp drect %f %f %f %f\n",
    //        temp_drect.x,
    //        temp_drect.y,
    //        temp_drect.w,
    //        temp_drect.h);
}

static void dst_rect_from_dst_crop(fsemu_rect_t *src_rect,
                                   fsemu_rect_t *src_crop,
                                   fsemu_rect_t *dst_crop,
                                   fsemu_rect_t *dst_rect)
{
    if (src_rect->w == 0 || src_rect->h == 0) {
        printf("src rect is 0\n");
        *dst_rect = *dst_crop;
        return;
    }
    if (src_crop->w == 0 || src_crop->h == 0) {
        printf("src crop is 0\n");
        *dst_rect = *dst_crop;
        return;
    }

    double scale_x = (double) src_rect->w / src_crop->w;
    double scale_y = (double) src_rect->h / src_crop->h;
    printf("src_rect->w %d / src_crop->w %d = scale_x %0.2f\n",
           src_rect->w,
           src_crop->w,
           scale_x);
    // printf("src_rect->h %d / src_crop->h %d = scale_y %0.2f\n",
    //        src_rect->h,
    //        src_crop->h,
    //        scale_y);

    dst_rect->w = dst_crop->w * scale_x;
    dst_rect->h = dst_crop->h * scale_y;
    dst_rect->x = dst_crop->x - dst_rect->w * src_crop->x / src_rect->w;
    dst_rect->y = dst_crop->y - dst_rect->h * src_crop->y / src_rect->h;

    // printf(
    //     "dst_crop->h = %d --> dst_rect->h = %d\n",
    //     dst_crop->h, dst_rect->h);
}

void fsemu_layout_video_rect(fsemu_rect_t *rect)
{
    fsemu_rect_t video_area;
    fsemu_layout_video_area(&video_area);

#if 0
    fsemu_rect_t client_rect;
    fsemu_layout_client_area(&client_rect);

    fsemu_drect_t temp_drect;
    temp_drect.x = client_rect.x;
    temp_drect.y = client_rect.y;
    temp_drect.w = client_rect.w;
    temp_drect.h = client_rect.h;

    // FIXME: SCALING

    int ttop, tright, tbottom, tleft;
    fsemu_theme_margins(&ttop, &tright, &tbottom, &tleft);

    // FIXME: 1080 coordinates
    // temp_drect.x += tleft; // FIXME
    temp_drect.y += ttop * client_rect.h / 1080.0;
    // temp_drect.w -= tleft + tright;  // FIXME
    temp_drect.h -= (ttop + tbottom) * client_rect.h / 1080.0;

    printf("layoyt client rect %d %d %d %d\n",
           client_rect.x,
           client_rect.y,
           client_rect.w,
           client_rect.h);
    printf("temp drect %f %f %f %f\n",
           temp_drect.x,
           temp_drect.y,
           temp_drect.w,
           temp_drect.h);
#endif

    int video_w = fsemu_layout.video_width;
    int video_h = fsemu_layout.video_height;

    fsemu_rect_t src_crop;
    // C64
    src_crop.x = (768 - 640) / 2;
    src_crop.y = 70;  // not (544 - 400) / 2;
    src_crop.w = 640;
    src_crop.h = 400;
    if (fsemu_layout.zoom_mode == 2) {
        src_crop.x -= 16;
        src_crop.y -= 16;
        src_crop.w += 16 * 2;
        src_crop.h += 16 * 2;
    }
    // FIXME: Check aspect

    // Commodore 64 zoom
    // fsemu_layout.zoom_mode = 2;
    // video_w = 320;
    // video_h = 200;

    double video_a =
        (double) fsemu_layout.video_width / fsemu_layout.video_height;

    if (fsemu_layout.zoom_mode >= 2) {
        video_a = (double) src_crop.w / src_crop.h;
    }

    double scale_x = 1.0;
    double scale_y = 1.0;

    int stretch_mode = fsemu_layout_stretch_mode();

    if (stretch_mode == FSEMU_STRETCH_MODE_FILL_SCREEN) {
    } else {
        // printf("-.-.--.-.-.\n");
        double pixel_aspect;
        if (stretch_mode == FSEMU_STRETCH_MODE_SQUARE_PIXELS) {
            pixel_aspect = 1.0;
        } else {
            pixel_aspect = fsemu_layout_pixel_aspect();
        }
        double initial_aspect = (double) video_area.w / video_area.h;
        double aspect = video_a / pixel_aspect;
        if (aspect < initial_aspect) {
            scale_x = aspect / initial_aspect;
        } else {
            scale_y = initial_aspect / aspect;
        }
        // printf("W/H %d/%d\n", video_area.w, video_area.h);
    }

    // FIXME: We might want to ensure that margins and video area and video
    // rect use multiples of 2. So w / 2 is is integer, etc. Also need to
    // consider rounding errors

    // Example:
    // layoyt video area 30 70 900 480
    // layoyt video area 30 70 900 480
    // scale x 0,711111 y 1,000000
    // layout video rect 160 70 639 480
    // ---> 639 should be 640.

    int scaled_w = video_area.w * scale_x;
    int scaled_h = video_area.h * scale_y;

    // Calculating borders first to get consistent rounding (down) and use
    // borders to recalculate width to get equal borders on opposite sides.

    int x_border = (video_area.w - scaled_w) / 2;
    int y_border = (video_area.h - scaled_h) / 2;

    // rect->w = w;
    // rect->h = h;
    // rect->x = video_area.x + (video_area.w - w) / 2;
    // rect->y = video_area.y + (video_area.h - h) / 2;

    // We should now have a nice video rect with integer size and position.

    // FIXME: For high-DPI windows where drawable size != window size, and
    // drawable size is not an integer multiple of window size, we might get
    // a problem where video rect is _not_ actually of integer size and
    // position on the underlying drawable, and video shaders assuming
    // 1:1 pixel mapping will not work correctly. So we might have to re-do
    // the layoyut both for window size and drawable size (for video renderer)
    // or always use the real drawable size instead.
    // FIXME: We probably want to (always) use real drawable size when
    // possible, since for example, when positioning shadows around a video
    // rect, we want to do the positioning in real pixels (same coordinate
    // system as video rect).

    rect->w = video_area.w - 2 * x_border;
    rect->h = video_area.h - 2 * y_border;
    rect->x = video_area.x + x_border;
    rect->y = video_area.y + y_border;

    if (fsemu_layout.zoom_mode >= 2) {
        fsemu_rect_t src_rect;
        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.w = video_w;
        src_rect.h = video_h;
        fsemu_rect_t dst_rect;
        dst_rect_from_dst_crop(&src_rect, &src_crop, rect, &dst_rect);

        *rect = dst_rect;

        // FIXME: We probably want to output two different rects here:
        // - a video rect which is a sub-rectangle of video_area
        // - a crop rect which specifies what part of the emulation video
        //   to render
        // Right now we just increase the video rect so that the crop is
        // sized and positioned correctly, but that means that the video
        // can go outside the video_area, and also that we get bleed
        // effects from "border" pixels.
    }
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
