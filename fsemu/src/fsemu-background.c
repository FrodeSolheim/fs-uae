#define FSEMU_INTERNAL 1
#include "fsemu-background.h"

#include <math.h>
#include <stdint.h>

#include "fsemu-color.h"
#include "fsemu-data.h"
#include "fsemu-gui.h"
#include "fsemu-image.h"
#include "fsemu-module.h"
#include "fsemu-widget.h"

#define module fsemu_background

#define FSEMU_WITH_BACKGROUND 1
#ifdef FSEMU_WITH_BACKGROUND

// ----------------------------------------------------------------------------

static struct {
    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    } color;
    bool initialized;
    struct {
        fsemu_widget_t *top;
        fsemu_widget_t *right;
        fsemu_widget_t *right_2;
        fsemu_widget_t *bottom;
        fsemu_widget_t *left;
        fsemu_widget_t *left_2;
        fsemu_widget_t *shadow_n;
        fsemu_widget_t *shadow_ne;
        fsemu_widget_t *shadow_e;
        fsemu_widget_t *shadow_se;
        fsemu_widget_t *shadow_s;
        fsemu_widget_t *shadow_sw;
        fsemu_widget_t *shadow_w;
        fsemu_widget_t *shadow_nw;
    } widgets;
} module;

// #define FSEMU_BACKGROUND_LEVEL 0x08
// #define FSEMU_BACKGROUND_LEVEL_2 0x18

#define FSEMU_BACKGROUND_LEVEL 0x0A
#define FSEMU_BACKGROUND_LEVEL_2 0x20

static void fsemu_background_create_side(fsemu_image_t *image,
                                         int width,
                                         int height,
                                         int overlap)
{
    // Gradients
    // double left = 0x0c / 255.0;
    // double right = 0x2c / 255.0;
    double left = FSEMU_BACKGROUND_LEVEL / 255.0;
    double right = FSEMU_BACKGROUND_LEVEL_2 / 255.0;

    double top = 0.3;

    double xdiff = right - left;
    double ydiff = 1.0 - top;

    // Dept (error diffusion) for Floyd-Steinberg dithering. Using
    // width + 2 to avoid having to deal with edge case.
    double *line_debt = (double *) malloc((width + 2) * sizeof(double));
    memset(line_debt, 0, (width + 2) * sizeof(double));

    double *next_debt = (double *) malloc((width + 2) * sizeof(double));
    memset(next_debt, 0, (width + 2) * sizeof(double));

    // double next_debt = 0;

    uint8_t *data = image->data;
    uint8_t *line = data;
    for (int y = 0; y < height; y++) {
        uint8_t *pixel = line;
        for (int x = 0; x < width; x++) {
            // End gradient a bit before to avoid overlap with drop shadow,
            // for possibly nicer effect.
            double d = (double) x / (width - 10);
            if (d > 1.0) {
                d = 1.0;
            }
            // double gray_d = 255.0 * (left + xdiff * pow(d, 2.2)) *
            //                     (top + ydiff * y / height) +
            //                 line_debt[x + 0 + 1];
            double gray_d = 255.0 * (left + xdiff * pow(d, 2.2)) *
                                (top + ydiff * (1.0 - abs(y - height / 2.0) /
                                                          (height / 2.0))) +
                            line_debt[x + 0 + 1];
            // double gray_d = 255.0 * (left + xdiff * pow(d, 2.2)) *
            //         (top + ydiff * (abs(y - height / 2.0) / (height / 2.0)))
            //         +
            //                 line_debt[x + 0 + 1];

            // gray_d = 255.0 * (0.0 + 0.05 * pow((double) x / width, 2.2))
            //  +line_debt[x + 0 + 1];

            // fsemu_background_log_debug("gray_d = %0.2f, error was %0.2f\n",
            // gray_d, line_debt[x
            // + 0 + 1]);
            uint8_t gray = gray_d + 0.5;
            pixel[0] = gray;  // Red
            pixel[1] = gray;  // Green
            pixel[2] = gray;  // Blue
            pixel[3] = 0xff;  // Alpha
            pixel += 4;

            double error = gray_d - gray;
            line_debt[x + 1 + 1] += error * 7 / 16 * 1;
            next_debt[x - 1 + 1] += error * 3 / 16 * 1;
            next_debt[x + 0 + 1] += error * 5 / 16 * 1;
            next_debt[x + 1 + 1] += error * 1 / 16 * 1;

            // Reset error diffusion for this pixel for next run
            line_debt[x + 0 + 1] = 0;
        }
        line += image->stride;
        double *temp = line_debt;
        line_debt = next_debt;
        next_debt = temp;
    }

    free(line_debt);
    free(next_debt);
}

static void fsemu_background_create_top(fsemu_image_t *image,
                                        int width,
                                        int height,
                                        int offset)
{
    // double right = 0x2c / 255.0;
    double right = FSEMU_BACKGROUND_LEVEL_2 / 255.0;
    double top = 0.5;
    double ydiff = 1.0 - top;

    // Dept (error diffusion) for Floyd-Steinberg dithering. Using
    // width + 2 to avoid having to deal with edge case.
    double *line_debt = (double *) malloc((width + 2) * sizeof(double));
    memset(line_debt, 0, (width + 2) * sizeof(double));

    double *next_debt = (double *) malloc((width + 2) * sizeof(double));
    memset(next_debt, 0, (width + 2) * sizeof(double));

    uint8_t *data = image->data;
    uint8_t *line = data;
    for (int y = 0; y < height; y++) {
        uint8_t *pixel = line;
        for (int x = 0; x < width; x++) {
            double gray_d =
                255.0 * right * (top + ydiff * (offset + y) / 1080) +
                line_debt[x + 0 + 1];
            uint8_t gray = gray_d + 0.5;
            pixel[0] = gray;  // Red
            pixel[1] = gray;  // Green
            pixel[2] = gray;  // Blue
            pixel[3] = 0xff;  // Alpha
            pixel += 4;
            double error = gray_d - gray;
            line_debt[x + 1 + 1] += error * 7 / 16 * 1;
            next_debt[x - 1 + 1] += error * 3 / 16 * 1;
            next_debt[x + 0 + 1] += error * 5 / 16 * 1;
            next_debt[x + 1 + 1] += error * 1 / 16 * 1;
            // Reset error diffusion for this pixel for next run
            line_debt[x + 0 + 1] = 0;
        }
        line += image->stride;
        double *temp = line_debt;
        line_debt = next_debt;
        next_debt = temp;
    }

    free(line_debt);
    free(next_debt);
}

static void fsemu_background_create_side_2(fsemu_image_t *image)
{
    double r = fsemu_background.color.r / 255.0;
    double g = fsemu_background.color.g / 255.0;
    double b = fsemu_background.color.b / 255.0;

    double top = 0.5;
    double ydiff = 1.0 - top;

    int h = image->height;

    uint8_t *data = image->data;
    uint8_t *line = data;
    for (int y = 0; y < h; y++) {
        uint8_t *pixel = line;
        for (int x = 0; x < image->width; x++) {
            pixel[0] = 255 * (r * (top + ydiff * y / h)) + 0.5;
            pixel[1] = 255 * (g * (top + ydiff * y / h)) + 0.5;
            pixel[2] = 255 * (b * (top + ydiff * y / h)) + 0.5;
            pixel[3] = 0xff;  // Alpha
            pixel += 4;
        }
        line += image->stride;
    }
}

static void fsemu_background_mirror(fsemu_image_t *in, fsemu_image_t *out)
{
    uint8_t *in_line = in->data;
    uint8_t *out_line = out->data;
    for (int y = 0; y < out->height; y++) {
        uint8_t *in_pixel = in_line + (in->width - 1) * 4;
        uint8_t *out_pixel = out_line;
        for (int x = 0; x < out->width; x++) {
            out_pixel[0] = in_pixel[0];
            out_pixel[1] = in_pixel[1];
            out_pixel[2] = in_pixel[2];
            out_pixel[3] = in_pixel[3];
            out_pixel += 4;
            in_pixel -= 4;
        }
        in_line += in->stride;
        out_line += out->stride;
    }
}

static void fsemu_background_flip(fsemu_image_t *in, fsemu_image_t *out)
{
    uint8_t *in_line = in->data + (in->height - 1) * in->stride;
    uint8_t *out_line = out->data;
    for (int y = 0; y < out->height; y++) {
        uint8_t *in_pixel = in_line;
        uint8_t *out_pixel = out_line;
        for (int x = 0; x < out->width; x++) {
            out_pixel[0] = in_pixel[0];
            out_pixel[1] = in_pixel[1];
            out_pixel[2] = in_pixel[2];
            out_pixel[3] = in_pixel[3];
            out_pixel += 4;
            in_pixel += 4;
        }
        in_line -= in->stride;
        out_line += out->stride;
    }
}

static void fsemu_background_transpose(fsemu_image_t *in, fsemu_image_t *out)
{
    uint8_t *in_column = in->data;
    uint8_t *out_line = out->data;
    for (int y = 0; y < out->height; y++) {
        uint8_t *in_pixel = in_column;
        uint8_t *out_pixel = out_line;
        for (int x = 0; x < out->width; x++) {
            out_pixel[0] = in_pixel[0];
            out_pixel[1] = in_pixel[1];
            out_pixel[2] = in_pixel[2];
            out_pixel[3] = in_pixel[3];
            out_pixel += 4;
            in_pixel += in->stride;
        }
        in_column += 4;
        out_line += out->stride;
    }
}

#if 0
static void fsemu_background_stripe(fsemu_image_t *in,
                                    int x_offset,
                                    int y_offset,
                                    fsemu_image_t *out)
{
    uint8_t *in_data = in->data;
    uint8_t *out_data = out->data;
    uint8_t *in_line = in_data + y_offset * in->stride;
    uint8_t *out_line = out_data;
    for (int y = 0; y < out->height; y++) {
        uint8_t *in_pixel = in_line + x_offset * 4;
        uint8_t *out_pixel = out_line;
        for (int x = 0; x < out->width; x++) {
            out_pixel[0] = in_pixel[0];
            out_pixel[1] = in_pixel[1];
            out_pixel[2] = in_pixel[2];
            out_pixel[3] = in_pixel[3];
            out_pixel += 4;
        }
        in_line += in->stride;
        out_line += out->stride;
    }
}
#endif

static void fsemu_background_setup_shadow_2(fsemu_widget_t **widget_p,
                                            fsemu_image_t *image,
                                            int anchor_left,
                                            double offset_left,
                                            int anchor_top,
                                            double offset_top,
                                            int anchor_right,
                                            double offset_right,
                                            int anchor_bottom,
                                            double offset_bottom)
{
    *widget_p = fsemu_widget_new_with_name("fsemu:background:shadow");
    fsemu_widget_t *widget = *widget_p;
    fsemu_widget_set_image(widget, image, false);
    // fsemu_widget_set_coordinates(
    //     widget, 320 - 32, 60 - 32, 64, 64, FSEMU_WIDGET_1080_LEFT);
    fsemu_widget_set_coordinates(widget,
                                 anchor_left,
                                 offset_left,
                                 anchor_top,
                                 offset_top,
                                 anchor_right,
                                 offset_right,
                                 anchor_bottom,
                                 offset_bottom);
    fsemu_widget_set_z_index(widget, -9990);
    fsemu_widget_set_color(widget, FSEMU_COLOR_RGB_A(0xffffff, 0x80));
    fsemu_widget_set_visible(widget, true);
    fsemu_gui_add_item(widget);
}

static void fsemu_background_multiply_alpha(fsemu_image_t *in1,
                                            fsemu_image_t *in2,
                                            fsemu_image_t *out)
{
    uint8_t *in1_line = in1->data;
    uint8_t *in2_line = in2->data;
    uint8_t *out_line = out->data;
    for (int y = 0; y < out->height; y++) {
        uint8_t *in1_pixel = in1_line;
        uint8_t *in2_pixel = in2_line;
        uint8_t *out_pixel = out_line;
        for (int x = 0; x < out->width; x++) {
            out_pixel[0] = 0x0;
            out_pixel[1] = 0x0;
            out_pixel[2] = 0x0;
            // out_pixel[3] = 255 * (in1_pixel[3] / 255.0) * (in2_pixel[3] /
            // 255.0) + 0.5;
            out_pixel[3] = 255 * in1_pixel[3] * in2_pixel[3] / 65536;
            in1_pixel += 4;
            in2_pixel += 4;
            out_pixel += 4;
        }
        in1_line += in1->stride;
        in2_line += in2->stride;
        out_line += out->stride;
    }
}

#if 0
static void fsemu_background_setup_shadow(fsemu_widget_t *widget,
                                          const char *name,
                                          int anchor_left,
                                          double offset_left,
                                          int anchor_top,
                                          double offset_top,
                                          int anchor_right,
                                          double offset_right,
                                          int anchor_bottom,
                                          double offset_bottom)
{
    fsemu_image_t *image =
        fsemu_image_from_stream(fsemu_data_stream(name), true);
    fsemu_background_setup_shadow_2(widget,
                                    image,
                                    anchor_left,
                                    offset_left,
                                    anchor_top,
                                    offset_top,
                                    anchor_right,
                                    offset_right,
                                    anchor_bottom,
                                    offset_bottom);
    fsemu_refable_unref(image);
}
#endif

#define FSEMU_BACKGROUND_SHADOW_POW 3.0

static void fsemu_background_create_side_shadow(fsemu_image_t *image)
{
    // FIXME: Optimize: Can render one line and then duplicate downwards

    double dw = image->width / 2.0;

    uint8_t *data = image->data;
    uint8_t *line = data;
    for (int y = 0; y < image->height; y++) {
        uint8_t *pixel = line;
        for (int x = 0; x < image->width; x++) {
            double d = (dw - x) / dw;
            if (d < 0) {
                d = 0;
            }
            double a = pow(1.0 - d, FSEMU_BACKGROUND_SHADOW_POW);
            /*
            if (a > 1.0) {
                a = 1.0;
            }
            */
            pixel[0] = 0x00;  // Red
            pixel[1] = 0x00;  // Green
            pixel[2] = 0x00;  // Blue
            pixel[3] = 255 * a + 0.5;
            pixel += 4;
        }
        line += image->stride;
    }
}

#if 0
static void fsemu_background_create_corner_shadow(fsemu_image_t *image)
{
    // FIXME: Optimize: Can use reflection to halve calculations

    double dw = image->width / 2.0;

    uint8_t *data = image->data;
    uint8_t *line = data;
    for (int y = 0; y < image->height; y++) {
        uint8_t *pixel = line;
        for (int x = 0; x < image->width; x++) {
            double dx = (dw - x) / dw;
            if (dx < 0) {
                dx = 0;
            }
            double dy = (dw - y) / dw;
            if (dy < 0) {
                dy = 0;
            }

            double d = sqrt(pow(dx, 2) + pow(dy, 2));
            if (d > 1.0) {
                d = 1.0;
            }
            double a = pow(1.0 - d, FSEMU_BACKGROUND_SHADOW_POW);

            pixel[0] = 0x00;  // Red
            pixel[1] = 0x00;  // Green
            pixel[2] = 0x00;  // Blue
            pixel[3] = 255 * a + 0.5;
            pixel += 4;
        }
        line += image->stride;
    }
}
#endif

static void fsemu_background_init_widgets(void)
{
    int margin = 60;
    int height = 1080;
    int overlap = 0;
    int video_width = (height - 2 * margin) * 4 / 3;
    int left_width = (1920 - video_width) / 2 + overlap;
    fsemu_background_log("Side width: %d\n", left_width);

    int middle_width = video_width;
    int middle_height = margin;

    fsemu_image_t *left_image = fsemu_image_from_size(left_width, height);
    fsemu_image_t *left_2_image = fsemu_image_from_size(1, height);
    fsemu_image_t *top_image =
        fsemu_image_from_size(middle_width, middle_height);
    fsemu_image_t *right_image = fsemu_image_from_size(left_width, height);
    fsemu_image_t *bottom_image =
        fsemu_image_from_size(middle_width, middle_height);

    // FIXME: Ideally, for best image quality, dithering diffusion should
    // happen across edges between left/right and top and bottom edge images.

    // FIXME: left_2_image / right_2_image is not dithered right now...
    // FIXME: If dithered, they should not be stretched, but tiled instead.
    // (That's also true for the top/bottom images...)

    // Or, just move background rendering to the GPU and compute dithering in
    // a shader instead...

    fsemu_background_create_side(
        left_image, left_width - overlap, height, overlap);
    fsemu_background_create_side_2(left_2_image);
    // fsemu_background_stripe(left_image, left_width - 1, 0, top_image);
    fsemu_background_create_top(top_image, middle_width, 60, 0);
    fsemu_background_mirror(left_image, right_image);
    fsemu_background_create_top(bottom_image, middle_width, 60, height - 60);
    // fsemu_background_stripe(
    //     left_image, left_width - 1, 1080 - middle_height - 1, bottom_image);

    fsemu_widget_t *widget;

    fsemu_background.widgets.left_2 =
        fsemu_widget_new_with_name("fsemu:background:left2");
    widget = fsemu_background.widgets.left_2;
    fsemu_widget_set_image(widget, left_2_image, false);
    // fsemu_widget_set_coords(
    //     widget, 0, 0, left_width, height, FSEMU_WIDGET_1080_LEFT);
    fsemu_widget_set_coordinates(widget,
                                 FSEMU_WIDGET_SCREEN_LEFT,
                                 0,
                                 FSEMU_WIDGET_SCREEN_TOP,
                                 0,
                                 FSEMU_WIDGET_VIDEO_LEFT,
                                 -left_width,
                                 FSEMU_WIDGET_SCREEN_BOTTOM,
                                 0);
    fsemu_widget_set_z_index(widget, -9999);
    fsemu_widget_set_visible(widget, true);
    fsemu_gui_add_item(widget);

    fsemu_background.widgets.left =
        fsemu_widget_new_with_name("fsemu:background:left");
    widget = fsemu_background.widgets.left;
    fsemu_widget_set_image(widget, left_image, false);
    // fsemu_widget_set_coords(
    //     widget, 0, 0, left_width, height, FSEMU_WIDGET_1080_LEFT);
    fsemu_widget_set_coordinates(widget,
                                 FSEMU_WIDGET_VIDEO_LEFT,
                                 -left_width,
                                 FSEMU_WIDGET_SCREEN_TOP,
                                 0,
                                 FSEMU_WIDGET_VIDEO_LEFT,
                                 0,
                                 FSEMU_WIDGET_SCREEN_BOTTOM,
                                 0);
    fsemu_widget_set_z_index(widget, -9999);
    fsemu_widget_set_visible(widget, true);
    fsemu_gui_add_item(widget);

    fsemu_background.widgets.top =
        fsemu_widget_new_with_name("fsemu:background:top");
    widget = fsemu_background.widgets.top;
    fsemu_widget_set_image(widget, top_image, false);
    // FIXME: Hmmm, problem with coords here...
    // Maybe draw below sides, across the screen instead
    // fsemu_widget_set_coords(
    //     widget, left_width, 0, middle_width, middle_height,
    //     FSEMU_WIDGET_1080);
    fsemu_widget_set_coordinates(widget,
                                 FSEMU_WIDGET_VIDEO_LEFT,
                                 0,
                                 FSEMU_WIDGET_SCREEN_TOP,
                                 0,
                                 FSEMU_WIDGET_VIDEO_RIGHT,
                                 0,
                                 FSEMU_WIDGET_VIDEO_TOP,
                                 0);
    fsemu_widget_set_z_index(widget, -9999);
    fsemu_widget_set_visible(widget, true);
    fsemu_gui_add_item(widget);

    fsemu_background.widgets.right =
        fsemu_widget_new_with_name("fsemu:background:right");
    widget = fsemu_background.widgets.right;
    fsemu_widget_set_image(widget, right_image, false);
    // fsemu_widget_set_coords(widget,
    //                         1920 - left_width,
    //                         0,
    //                         left_width,
    //                         height,
    //                         FSEMU_WIDGET_1080_RIGHT);
    fsemu_widget_set_coordinates(widget,
                                 FSEMU_WIDGET_VIDEO_RIGHT,
                                 0,
                                 FSEMU_WIDGET_SCREEN_TOP,
                                 0,
                                 FSEMU_WIDGET_VIDEO_RIGHT,
                                 left_width,
                                 FSEMU_WIDGET_SCREEN_BOTTOM,
                                 0);
    fsemu_widget_set_z_index(widget, -9999);
    fsemu_widget_set_visible(widget, true);
    fsemu_gui_add_item(widget);

    fsemu_background.widgets.right_2 =
        fsemu_widget_new_with_name("fsemu:background:right2");
    widget = fsemu_background.widgets.right_2;
    fsemu_widget_set_image(widget, left_2_image, false);
    fsemu_widget_set_coordinates(widget,
                                 FSEMU_WIDGET_VIDEO_RIGHT,
                                 left_width,
                                 FSEMU_WIDGET_SCREEN_TOP,
                                 0,
                                 FSEMU_WIDGET_SCREEN_RIGHT,
                                 0,
                                 FSEMU_WIDGET_SCREEN_BOTTOM,
                                 0);
    fsemu_widget_set_z_index(widget, -9999);
    fsemu_widget_set_visible(widget, true);
    fsemu_gui_add_item(widget);

    fsemu_background.widgets.bottom =
        fsemu_widget_new_with_name("fsemu:background:bottom");
    widget = fsemu_background.widgets.bottom;
    fsemu_widget_set_image(widget, bottom_image, false);
    // FIXME: Hmmm, problem with coords here...
    // Maybe draw below sides, across the screen instead
    // fsemu_widget_set_coords(widget,
    //                         left_width,
    //                         1080 - middle_height,
    //                         middle_width,
    //                         middle_height,
    //                         FSEMU_WIDGET_1080);
    fsemu_widget_set_coordinates(widget,
                                 FSEMU_WIDGET_VIDEO_LEFT,
                                 0,
                                 FSEMU_WIDGET_VIDEO_BOTTOM,
                                 0,
                                 FSEMU_WIDGET_VIDEO_RIGHT,
                                 0,
                                 FSEMU_WIDGET_SCREEN_BOTTOM,
                                 0);
    fsemu_widget_set_z_index(widget, -9999);
    fsemu_widget_set_visible(widget, true);
    fsemu_gui_add_item(widget);

    // FIXME: unref images? check image ownership

    // FIXME: Add noise to images to reduce banding? Dithering?

#if 0
    fsemu_background_setup_shadow(&fsemu_background.widgets.shadow_w,
                                  "DropShadowW.png",
                                  /* VIDEO_LEFT */ 0,
                                  -32,
                                  -32,
                                  /* VIDEO_TOP */ 0,
                                  32,
                                  32);
    fsemu_background_setup_shadow(&fsemu_background.widgets.shadow_nw,
                                  "DropShadowNW.png",
                                  /* VIDEO_LEFT */ 0,
                                  -32,
                                  -32,
                                  /* VIDEO_TOP */ 0,
                                  32,
                                  32);
fsemu_background_setup_shadow(&fsemu_background.widgets.shadow_n,
                              "DropShadowN.png",
                              /* VIDEO_LEFT */ 0,
                              32,
                              132,
                              /* VIDEO_TOP */ 0,
                              32,
                              32);
#endif

    fsemu_image_t *shadow_w = fsemu_image_from_size(32, 32);
    fsemu_background_create_side_shadow(shadow_w);

    fsemu_background_setup_shadow_2(&fsemu_background.widgets.shadow_w,
                                    shadow_w,
                                    FSEMU_WIDGET_VIDEO_LEFT,
                                    -32,
                                    FSEMU_WIDGET_VIDEO_TOP,
                                    32,
                                    FSEMU_WIDGET_VIDEO_LEFT,
                                    32,
                                    FSEMU_WIDGET_VIDEO_BOTTOM,
                                    -32);

#if 0
    fsemu_background_setup_shadow(&fsemu_background.widgets.shadow_w,
                                  "DropShadowW.png",
                                  FSEMU_WIDGET_VIDEO_LEFT,
                                  -32,
                                  FSEMU_WIDGET_VIDEO_TOP,
                                  32,
                                  FSEMU_WIDGET_VIDEO_LEFT,
                                  32,
                                  FSEMU_WIDGET_VIDEO_BOTTOM,
                                  -32);
#endif

    fsemu_image_t *shadow_n = fsemu_image_from_size(32, 32);
    fsemu_background_transpose(shadow_w, shadow_n);

    fsemu_background_setup_shadow_2(&fsemu_background.widgets.shadow_n,
                                    shadow_n,
                                    FSEMU_WIDGET_VIDEO_LEFT,
                                    32,
                                    FSEMU_WIDGET_VIDEO_TOP,
                                    -32,
                                    FSEMU_WIDGET_VIDEO_RIGHT,
                                    -32,
                                    FSEMU_WIDGET_VIDEO_TOP,
                                    32);

#if 1
    fsemu_image_t *shadow_nw = fsemu_image_from_size(32, 32);
    fsemu_background_multiply_alpha(shadow_w, shadow_n, shadow_nw);

    fsemu_background_setup_shadow_2(&fsemu_background.widgets.shadow_nw,
                                    shadow_nw,
                                    FSEMU_WIDGET_VIDEO_LEFT,
                                    -32,
                                    FSEMU_WIDGET_VIDEO_TOP,
                                    -32,
                                    FSEMU_WIDGET_VIDEO_LEFT,
                                    32,
                                    FSEMU_WIDGET_VIDEO_TOP,
                                    32);
#endif

#if 1

    fsemu_image_t *shadow_ne = fsemu_image_from_size(32, 32);
    fsemu_background_mirror(shadow_nw, shadow_ne);

    fsemu_background_setup_shadow_2(&fsemu_background.widgets.shadow_ne,
                                    shadow_ne,
                                    FSEMU_WIDGET_VIDEO_RIGHT,
                                    -32,
                                    FSEMU_WIDGET_VIDEO_TOP,
                                    -32,
                                    FSEMU_WIDGET_VIDEO_RIGHT,
                                    32,
                                    FSEMU_WIDGET_VIDEO_TOP,
                                    32);
#endif
#if 1

    fsemu_image_t *shadow_e = fsemu_image_from_size(32, 32);
    fsemu_background_mirror(shadow_w, shadow_e);

    fsemu_background_setup_shadow_2(&fsemu_background.widgets.shadow_e,
                                    shadow_e,
                                    FSEMU_WIDGET_VIDEO_RIGHT,
                                    -32,
                                    FSEMU_WIDGET_VIDEO_TOP,
                                    32,
                                    FSEMU_WIDGET_VIDEO_RIGHT,
                                    32,
                                    FSEMU_WIDGET_VIDEO_BOTTOM,
                                    -32);
#endif
#if 1

    fsemu_image_t *shadow_se = fsemu_image_from_size(32, 32);
    fsemu_background_flip(shadow_ne, shadow_se);

    fsemu_background_setup_shadow_2(&fsemu_background.widgets.shadow_se,
                                    shadow_se,
                                    FSEMU_WIDGET_VIDEO_RIGHT,
                                    -32,
                                    FSEMU_WIDGET_VIDEO_BOTTOM,
                                    -32,
                                    FSEMU_WIDGET_VIDEO_RIGHT,
                                    32,
                                    FSEMU_WIDGET_VIDEO_BOTTOM,
                                    32);
#endif

    fsemu_image_t *shadow_s = fsemu_image_from_size(32, 32);
    fsemu_background_flip(shadow_n, shadow_s);

    fsemu_background_setup_shadow_2(&fsemu_background.widgets.shadow_s,
                                    shadow_s,
                                    FSEMU_WIDGET_VIDEO_LEFT,
                                    32,
                                    FSEMU_WIDGET_VIDEO_BOTTOM,
                                    -32,
                                    FSEMU_WIDGET_VIDEO_RIGHT,
                                    -32,
                                    FSEMU_WIDGET_VIDEO_BOTTOM,
                                    32);
#if 1

    fsemu_image_t *shadow_sw = fsemu_image_from_size(32, 32);
    fsemu_background_mirror(shadow_se, shadow_sw);

    fsemu_background_setup_shadow_2(&fsemu_background.widgets.shadow_sw,
                                    shadow_sw,
                                    FSEMU_WIDGET_VIDEO_LEFT,
                                    -32,
                                    FSEMU_WIDGET_VIDEO_BOTTOM,
                                    -32,
                                    FSEMU_WIDGET_VIDEO_LEFT,
                                    32,
                                    FSEMU_WIDGET_VIDEO_BOTTOM,
                                    32);
#endif

    // The widgets have references to these images now.
    fsemu_image_unref(top_image);
    fsemu_image_unref(left_image);
    fsemu_image_unref(left_2_image);
    fsemu_image_unref(bottom_image);
    fsemu_image_unref(right_image);
    fsemu_image_unref(shadow_n);
    fsemu_image_unref(shadow_ne);
    fsemu_image_unref(shadow_e);
    fsemu_image_unref(shadow_se);
    fsemu_image_unref(shadow_s);
    fsemu_image_unref(shadow_sw);
    fsemu_image_unref(shadow_w);
    fsemu_image_unref(shadow_nw);
}

static void fsemu_background_quit(void)
{
    fsemu_background_log("fsemu_background_quit\n");
    fsemu_widget_unref(fsemu_background.widgets.top);
    fsemu_widget_unref(fsemu_background.widgets.right);
    fsemu_widget_unref(fsemu_background.widgets.right_2);
    fsemu_widget_unref(fsemu_background.widgets.bottom);
    fsemu_widget_unref(fsemu_background.widgets.left);
    fsemu_widget_unref(fsemu_background.widgets.left_2);
    fsemu_widget_unref(fsemu_background.widgets.shadow_n);
    fsemu_widget_unref(fsemu_background.widgets.shadow_ne);
    fsemu_widget_unref(fsemu_background.widgets.shadow_e);
    fsemu_widget_unref(fsemu_background.widgets.shadow_se);
    fsemu_widget_unref(fsemu_background.widgets.shadow_s);
    fsemu_widget_unref(fsemu_background.widgets.shadow_sw);
    fsemu_widget_unref(fsemu_background.widgets.shadow_w);
    fsemu_widget_unref(fsemu_background.widgets.shadow_nw);
}

#endif  // FSEMU_WITH_BACKGROUND

void fsemu_background_init(void)
{
#ifdef FSEMU_WITH_BACKGROUND
    if (module.initialized) {
        return;
    }
    module.initialized = true;
#ifdef FSEMU_LINUX_ARM
    fsemu_background_log("Not using background module (performance)\n");
    return;
#endif
    fsemu_background_log("Initializing background module\n");
    fsemu_module_on_quit(fsemu_background_quit);

    fsemu_background.color.r = FSEMU_BACKGROUND_LEVEL;
    fsemu_background.color.g = FSEMU_BACKGROUND_LEVEL;
    fsemu_background.color.b = FSEMU_BACKGROUND_LEVEL;

    fsemu_background_init_widgets();
#endif
}

// ----------------------------------------------------------------------------

#undef module
