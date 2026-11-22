#define FSEMU_INTERNAL
#include "fsemu-layout.h"

#include <SDL3/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "fsemu-common.h"
#include "fsemu-option.h"

static int fsemu_layout_log_level = SDL_LOG_PRIORITY_INFO;

#define fsemu_layout_log_debug(format, ...)                                                        \
    FS_LOG_DEBUG(fsemu_layout_log_level, "[FSE] [LAY]", format, ##__VA_ARGS__)

#define fsemu_layout_log_warning(format, ...)                                                      \
    FS_LOG_WARNING(fsemu_layout_log_level, "[FSE] [LAY]", format, ##__VA_ARGS__)

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
    // Whether the emulated display is NTSC: set by the emulation thread with each frame, read by
    // the drawing side's layout (the picture's aspect), so atomic. The other layout state here is
    // the drawing side's (fsemu_video_fit_picture), but for what the options set on the main
    // thread before the drawing side's first frame.
    SDL_AtomicInt ntsc_mode;
    // The picture policies (fsemu_layout_crop_policy_t, fsemu_layout_fit_policy_t,
    // fsemu_layout_filter_t), set from any thread (fsemu's shortcuts: Mod+S, Mod+C) and read by
    // the emulation thread (the crop) and the drawing side (the fit and the filter), so they are
    // atomic.
    SDL_AtomicInt crop_policy;
    SDL_AtomicInt fit_policy;
    SDL_AtomicInt filter;
    // FIXME: Not currently used
    int client_margins[4];
    // The emulation surface's rectangle in the host window (pixels).
    fsemu_rect_t client_area;
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

    // Default to zoom mode 1 for now
    fsemu_layout.zoom_mode = 1;
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
    if (width == 0 || height == 0) {
        fsemu_layout_log_warning("Zero layout video size %dx%d\n", width, height);
        width = 4;
        height = 3;
    }
    // fsapp_assert(width > 0);
    // fsapp_assert(height > 0);

    fsemu_layout.video_width = width;
    fsemu_layout.video_height = height;
}

int fsemu_layout_stretch_mode(void)
{
    if (fsemu_layout_get_fit_policy() == FSEMU_LAYOUT_FIT_POLICY_STRETCH) {
        return FSEMU_STRETCH_MODE_FILL_SCREEN;
    }
    return fsemu_layout.stretch_mode;
}

void fsemu_layout_set_stretch_mode(int stretch_mode)
{
    if (stretch_mode < 0 || stretch_mode >= FSEMU_STRETCH_MODE_MAX) {
        fsemu_layout_log_warning("Invalid stretch mode");
        return;
    }
    if (stretch_mode == FSEMU_STRETCH_MODE_FILL_SCREEN) {
        fsemu_layout_set_fit_policy(FSEMU_LAYOUT_FIT_POLICY_STRETCH);
        fsemu_layout.stretch_mode = FSEMU_STRETCH_MODE_CORRECT_ASPECT;
    } else {
        if (fsemu_layout_get_fit_policy() == FSEMU_LAYOUT_FIT_POLICY_STRETCH) {
            fsemu_layout_set_fit_policy(FSEMU_LAYOUT_FIT_POLICY_FIT);
        }
        fsemu_layout.stretch_mode = stretch_mode;
    }
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
    SDL_Log("zoom_mode %d\n", fsemu_layout.zoom_mode);
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

bool fsemu_layout_ntsc_mode(void)
{
    return SDL_GetAtomicInt(&fsemu_layout.ntsc_mode) != 0;
}

void fsemu_layout_set_ntsc_mode(bool ntsc_mode)
{
    SDL_SetAtomicInt(&fsemu_layout.ntsc_mode, ntsc_mode ? 1 : 0);
}

fsemu_layout_crop_policy_t fsemu_layout_get_crop_policy(void)
{
    return (fsemu_layout_crop_policy_t)SDL_GetAtomicInt(&fsemu_layout.crop_policy);
}

void fsemu_layout_set_crop_policy(fsemu_layout_crop_policy_t policy)
{
    SDL_SetAtomicInt(&fsemu_layout.crop_policy, policy);
}

fsemu_layout_fit_policy_t fsemu_layout_get_fit_policy(void)
{
    return (fsemu_layout_fit_policy_t)SDL_GetAtomicInt(&fsemu_layout.fit_policy);
}

void fsemu_layout_set_fit_policy(fsemu_layout_fit_policy_t policy)
{
    SDL_SetAtomicInt(&fsemu_layout.fit_policy, policy);
}

fsemu_layout_filter_t fsemu_layout_get_filter(void)
{
    return (fsemu_layout_filter_t)SDL_GetAtomicInt(&fsemu_layout.filter);
}

void fsemu_layout_set_filter(fsemu_layout_filter_t filter)
{
    SDL_SetAtomicInt(&fsemu_layout.filter, filter);
}

void fsemu_layout_set_client_area(const fsemu_rect_t *rect)
{
    fsemu_layout.client_area = *rect;
}

void fsemu_layout_client_area(fsemu_rect_t *rect)
{
    *rect = fsemu_layout.client_area;
}

void fsemu_layout_client_offset_scale(double *ox, double *oy, double *sx, double *sy)
{
    fsemu_rect_t rect;
    fsemu_layout_client_rect(&rect);
    // printf("client rect %d %d %d %d (%d %d)\n", rect.x, rect.y, rect.w,
    // rect.h, fsemu_layout.width, fsemu_layout.height);
    *ox = (double)rect.x / fsemu_layout.width;
    *oy = -(double)rect.y / fsemu_layout.height;
    *sx = (double)rect.w / fsemu_layout.width;
    *sy = (double)rect.h / fsemu_layout.height;
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
        // The margins of the removed fsemu theme module, in 1080p coordinates: scaled by the area's
        // height in pixels.
        ttop = 60;
        tright = 60;
        tbottom = 60;
        tleft = 60;
    } else {
        ttop = 0;
        tright = 0;
        tbottom = 0;
        tleft = 0;
    }

    // double margin_scale = video_area.h / 1080.0;
    double margin_scale = rect->h / 1080.0;

    ttop = ttop * margin_scale + 0.5;
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

static void dst_rect_from_dst_crop(fsemu_rect_t *src_rect, fsemu_rect_t *src_crop,
                                   fsemu_rect_t *dst_crop, fsemu_rect_t *dst_rect)
{
    if (src_rect->w == 0 || src_rect->h == 0) {
        fsemu_layout_log_debug("src rect is 0\n");
        *dst_rect = *dst_crop;
        return;
    }
    if (src_crop->w == 0 || src_crop->h == 0) {
        fsemu_layout_log_debug("src crop is 0\n");
        *dst_rect = *dst_crop;
        return;
    }

    double scale_x = (double)src_rect->w / src_crop->w;
    double scale_y = (double)src_rect->h / src_crop->h;
    fsemu_layout_log_debug("src_rect->w %d / src_crop->w %d = scale_x %0.2f\n", src_rect->w,
                           src_crop->w, scale_x);
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

    int input_w = fsemu_layout.video_width;
    int input_h = fsemu_layout.video_height;

    if (input_w == 0 || input_h == 0) {
        // FIXME:
        // printf("WARNING: Layout video size not set\n");
        input_w = 4;
        input_h = 3;
    }

    // fsapp_assert(video_w > 0);
    // fsapp_assert(video_h > 0);

    // printf("--------------- %d %d\n", video_w, video_h);

    fsemu_rect_t src_crop;
    // C64
    src_crop.x = (768 - 640) / 2;
    src_crop.y = 70; // not (544 - 400) / 2;
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

    double video_aspect;
    // if (fsemu_likely(fsemu_layout.video_height)) {
    // video_a =
    //     (double) fsemu_layout.video_width / fsemu_layout.video_height;
    video_aspect = (double)input_w / input_h;
    // printf("video_a = %0.2f\n", video_a);
    // } else {
    //     video_a = 1.0;
    // }

    if (fsemu_layout.zoom_mode >= 2) {
        video_aspect = (double)src_crop.w / src_crop.h;
    }

    double scale_x = 1.0;
    double scale_y = 1.0;

    // Read once, so that the whole layout follows one policy when another thread changes it.
    fsemu_layout_fit_policy_t fit_policy = fsemu_layout_get_fit_policy();
    int stretch_mode = fsemu_layout.stretch_mode;

    if (fit_policy == FSEMU_LAYOUT_FIT_POLICY_STRETCH) {
    } else {
        // printf("-.-.--.-.-.\n");
        double pixel_aspect;
        if (stretch_mode == FSEMU_STRETCH_MODE_SQUARE_PIXELS) {
            pixel_aspect = 1.0;
        } else {
            pixel_aspect = fsemu_layout_pixel_aspect();
        }

        // FIXME: Overriding
        if (fsemu_layout_ntsc_mode()) {
            pixel_aspect = 1.2;
        } else {
            pixel_aspect = 1.0;
        }

        double initial_aspect;
        // if (fsemu_likely(video_area.h)) {
        initial_aspect = (double)video_area.w / video_area.h;
        // } else {
        //     initial_aspect = 1.0;
        // }
        double aspect = video_aspect / pixel_aspect;
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

    // printf("%d %f, %d %f\n", video_area.w, scale_x, video_area.h, scale_y);

    int output_w = video_area.w * scale_x;
    int output_h = video_area.h * scale_y;

    bool ntsc_aspect_ratio = fsemu_layout_ntsc_mode();

    // if (ntsc_aspect_ratio) {
    //     scale_y = max_y_iscale < (int) max_x_iscale ? max_y_iscale : max_x_iscale;
    //     scale_x = scale_y;
    // } else {
    //     scale_y = max_y_iscale < max_x_iscale ? max_y_iscale : max_x_iscale;
    //     scale_x = scale_y;
    // }

    int area_w = video_area.w;
    int area_h = video_area.h;

    // ---------------------------------------------------------------------------------------------
    // FIXME: For now overriding with simple integer scaling

    if (fit_policy == FSEMU_LAYOUT_FIT_POLICY_INTEGER) {
        // Assuming low-res / non-interlaced for calculation. This will effectively give integer
        // scaling for low-res / non-interlaced, and half-integer scaling for high-res / interlaced
        // graphics modes.

        int max_x_iscale = area_w / (input_w / 2);
        int max_y_iscale = area_h / (input_h / 2);

        if (ntsc_aspect_ratio) {
            // Account for ~ 6:5 pixel aspect ratio while having integer / half-integer scaling.
            // This will effectively leave scale_x the same as scale_y when scale_y <= 3; scale_x
            // will be one less then scale_y when scale_y <= 9, (etc).
            scale_y = max_y_iscale;
            scale_x = round(scale_y / 1.2);
            // Looping here, while not the most efficient approach perhaps, might avoid some edge
            // cases due to the relationship between scale_x and scale_y at different sizes.
            while (scale_x > max_x_iscale) {
                scale_y -= 1;
                scale_x = round(scale_y / 1.2);
            }
        } else {
            scale_y = max_y_iscale < max_x_iscale ? max_y_iscale : max_x_iscale;
            scale_x = scale_y;
        }

        scale_x /= 2.0; // Correction for low-res in calculation while pixel doubling
        scale_y /= 2.0;
        output_w = input_w * scale_x;
        output_h = input_h * scale_y;
    }

    // ---------------------------------------------------------------------------------------------

    // printf("ntsc? %d, scale_x %0.1f scale_y %0.1f scaled_w %d scaled_h %d\n", ntsc_aspect_ratio,
    //        scale_x * 2, scale_y * 2, output_w, output_h);

    // In pixels, like the emulation surface's rectangle, so the integer fit's steps are whole
    // pixels.
    rect->w = output_w;
    rect->h = output_h;
    rect->x = video_area.x + (video_area.w - output_w) / 2;
    rect->y = video_area.y + (video_area.h - output_h) / 2;

    if (fsemu_layout.zoom_mode >= 2) {
        fsemu_rect_t src_rect;
        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.w = input_w;
        src_rect.h = input_h;
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

    // printf("%d %d\n", x_border, y_border);
    // printf("%d %d %d %d\n", rect->x, rect->y, rect->w, rect->h);
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
