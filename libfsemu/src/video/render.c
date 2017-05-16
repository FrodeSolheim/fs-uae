#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define FSE_INTERNAL_API
#include <fs/emu/log.h>
#include <fs/emu/options.h>
#include <fs/emu/render.h>
#include <fs/conf.h>
#include <assert.h>
#include <string.h>

fse_render_t fse_render = {};

static double g_scale_x = -1.0;
static double g_scale_y = -1.0;
static double g_align_x = 0.5;
static double g_align_y = 0.5;
static bool g_render_frame = true;

int fse_stretch_mode(void)
{
    return fse_render.stretch_mode;
}

void fse_set_stretch_mode(int stretch_mode)
{
    if (stretch_mode != fse_render.stretch_mode) {
        fse_log("[RENDER] Stretch mode = %d\n", stretch_mode);
    }
    fse_render.stretch_mode = stretch_mode;
}

void fse_cycle_stretch_mode(void)
{
    if (fse_stretch_mode() == FSE_STRETCH_NONE) {
#if 1
        if (fse_pixel_aspect() == 1.0) {
            fse_set_stretch_mode(FSE_STRETCH_FILL_SCREEN);
        } else {
            fse_set_stretch_mode(FSE_STRETCH_ASPECT);
        }
#else
        fse_set_stretch_mode(FSE_STRETCH_ASPECT);
#endif
    } else if (fse_stretch_mode() == FSE_STRETCH_ASPECT) {
        fse_set_stretch_mode(FSE_STRETCH_FILL_SCREEN);
    } else {
        fse_set_stretch_mode(FSE_STRETCH_NONE);
    }
}

int fse_scale_mode(void)
{
    return fse_render.scale_mode;
}

void fse_set_scale_mode(int scale_mode)
{
    if (scale_mode != fse_render.scale_mode) {
        fse_log("[RENDER] Scale mode = %d\n", scale_mode);
    }
    fse_render.scale_mode = scale_mode;
}

void fse_set_render_size(int w, int h)
{
    fse_render.width = w;
    fse_render.height = h;
}

double fse_pixel_aspect(void)
{
    if (fse_render.pixel_aspect == 0) {
        return 1.0;
    }
    return fse_render.pixel_aspect;
}

void fse_set_pixel_aspect(double aspect)
{
    fse_log("[RENDER] Pixel aspect = %0.2f\n", aspect);
    fse_render.pixel_aspect = aspect;
}

void fse_calculate_and_set_view_rectangle(int w, int h)
{
    fse_calculate_video_rectangle(
                fse_render.width, fse_render.height, w, h,
                &fse_render.view_x, &fse_render.view_y,
                &fse_render.view_w, &fse_render.view_h, NULL, NULL);
}

void fse_set_view_rectangle(int x, int y, int w, int h)
{
    fse_render.view_x = x;
    fse_render.view_y = y;
    fse_render.view_w = w;
    fse_render.view_h = h;
}

void fse_calculate_video_rectangle(
        int render_w, int render_h, int video_w, int video_h,
        int *x, int *y, int *w, int *h, double *sx, double *sy)
{
    double scale_x = 1.0;
    double scale_y = 1.0;
    int scale_mode = fse_scale_mode();
    int stretch_mode = fse_stretch_mode();

    if (scale_mode == FSE_SCALE_MAX) {
        if (stretch_mode == FSE_STRETCH_FILL_SCREEN) {
            /* No scaling change needed. */
        } else {
            double pixel_aspect = 1.0;
            if (stretch_mode == FSE_STRETCH_ASPECT) {
                pixel_aspect = fse_pixel_aspect();
            }
            double render_aspect = (double) render_w / render_h;
            double aspect = ((double) video_w / video_h) / pixel_aspect;
            if (aspect < render_aspect) {
                scale_x = aspect / render_aspect;
            } else {
                scale_y = render_aspect / aspect;
            }
        }
    } else if (scale_mode == FSE_SCALE_INTEGER) {
        scale_x = (double) video_w / render_w;
        scale_y = (double) video_h / render_h;
        int factor_x = 1.0 / scale_x;
        int factor_y = 1.0 / scale_y;
        factor_x = factor_x < factor_y ? factor_x : factor_y;
        if (factor_x < 1) {
            factor_x = 1;
        }
        factor_y = factor_x;
        scale_x *= factor_x;
        scale_y *= factor_y;
    } else if (scale_mode == FSE_SCALE_NONE) {
        scale_x = (double) video_w / render_w;
        scale_y = (double) video_h / render_h;
    } else if (scale_mode == FSE_SCALE_LEGACY) {
        assert(g_scale_x != 0);
        assert(g_scale_y != 0);
        if (g_scale_x < 0.0) {
            scale_x *= -g_scale_x;
        } else {
            scale_x = g_scale_x * video_w / render_w;
        }
        if (g_scale_y < 0.0) {
            scale_y *= -g_scale_y;
        } else {
            scale_y = g_scale_y * video_h / render_h;
        }
    }

    if (scale_mode != FSE_SCALE_INTEGER && scale_mode != FSE_SCALE_LEGACY) {
        if (scale_x > 0.98) {
            scale_x = 1.0;
        }
        if (scale_y > 0.98) {
            scale_y = 1.0;
        }
    }

#if 0
    if (scale_x != 1.0 || scale_y != 1.0) {
        glScalef(scale_x, scale_y, 1.0);
    }
    fsgs_set_render_scale(scale_x, scale_y);
#endif
    int dest_w = (render_w + 0.5) * scale_x;
    int dest_h = (render_h + 0.5) * scale_y;
    int dest_x = (render_w - dest_w) * g_align_x;
    int dest_y = (render_h - dest_h) * (1.0 - g_align_y);

    if (x) {
        *x = dest_x;
    }
    if (y) {
        *y = dest_y;
    }
    if (w) {
        *w = dest_w;
    }
    if (h) {
        *h = dest_h;
    }
    if (sx) {
        *sx * scale_x;
    }
    if (sy) {
        *sy * scale_y;
    }
}

#include "../emu/texture.h"

void fse_render_frame(void)
{
    if (!g_render_frame) {
        return;
    }
    if (fse_render.view_w == 0 || fse_render.view_h == 0) {
        /* Not rendering surrounding frame when view size is not set. */
        return;
    }
    /* Frame is designed for a 1080p resolution, scaled based on height. */
    int frame_width = 256 * fse_render.height / 1080;
    if (fse_render.view_x > 0) {
        fse_render_image_with_size(
                TEXTURE_FRAME_LEFT, fse_render.view_x - frame_width,
                    0, frame_width, fse_render.height);
        fse_render_image_with_size(
                TEXTURE_FRAME_RIGHT, fse_render.view_x + fse_render.view_w,
                    0, frame_width, fse_render.height);
    }
}

void fse_init_render(void)
{
    fse_init_conf();

    int stretch_mode = -1;
    const char *stretch = fs_config_get_const_string(OPTION_STRETCH);
    if (stretch) {
        if (strcmp(stretch, "1") == 0) {
            stretch_mode = FSE_STRETCH_FILL_SCREEN;
        } else if (strcmp(stretch, "0") == 0) {
            stretch_mode = FSE_STRETCH_NONE;
        } else if (strcmp(stretch, "aspect") == 0) {
            stretch_mode = FSE_STRETCH_ASPECT;
        }
    }
    if (stretch_mode == -1) {
        stretch_mode = FSE_STRETCH_ASPECT;
    }
    fse_set_stretch_mode(stretch_mode);

    int scale_mode = -1;
    const char *scale = fs_config_get_const_string(OPTION_SCALE);
    if (scale) {
        if (strcmp(scale, "1") == 0) {
            scale_mode = FSE_SCALE_MAX;
        } else if (strcmp(scale, "0") == 0) {
            scale_mode = FSE_SCALE_NONE;
        } else if (strcmp(scale, "integer") == 0) {
            scale_mode = FSE_SCALE_INTEGER;
        } else if (strcmp(scale, "legacy") == 0) {
            scale_mode = FSE_SCALE_LEGACY;
        }
    }
    if (scale_mode == -1) {
        scale_mode = FSE_SCALE_MAX;
        g_scale_x = fs_config_get_double("scale_x");
        if (g_scale_x == FS_CONFIG_NONE) {
            g_scale_x = -1.0;
        } else {
            scale_mode = FSE_SCALE_LEGACY;
        }
        g_scale_y = fs_config_get_double("scale_y");
        if (g_scale_y == FS_CONFIG_NONE) {
            g_scale_y = -1.0;
        } else {
            scale_mode = FSE_SCALE_LEGACY;
        }
        g_align_x = fs_config_get_double("align_x");
        if (g_align_x == FS_CONFIG_NONE) {
            g_align_x = 0.5;
        } else {
            scale_mode = FSE_SCALE_LEGACY;
        }
        g_align_y = fs_config_get_double("align_y");
        if (g_align_y == FS_CONFIG_NONE) {
            g_align_y = 0.5;
        } else {
            scale_mode = FSE_SCALE_LEGACY;
        }
    }
    fse_set_scale_mode(scale_mode);

    if (fs_config_false(OPTION_FRAME)) {
        g_render_frame = false;
    }
}
