#ifndef FSE_RENDER_H
#define FSE_RENDER_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void fse_init_render(void);
void fse_set_render_size(int w, int h);
void fse_render_hud(void);

double fse_pixel_aspect(void);
void fse_set_pixel_aspect(double aspect);

enum {
    FSE_STRETCH_NONE,
    FSE_STRETCH_FILL_SCREEN,
    FSE_STRETCH_ASPECT
};
int fse_stretch_mode(void);
void fse_set_stretch_mode(int stretch);
void fse_cycle_stretch_mode(void);

enum {
    FSE_SCALE_NONE,
    FSE_SCALE_MAX,
    FSE_SCALE_INTEGER,
    FSE_SCALE_LEGACY
};
int fse_scale_mode(void);
void fse_set_scale_mode(int scale);

void fse_calculate_video_rectangle(
        int render_w, int render_h, int video_w, int video_h,
        int *x, int *y, int *w, int *h, double *sx, double *sy);
void fse_calculate_and_set_view_rectangle(int w, int h);
void fse_set_view_rectangle(int x, int y, int w, int h);
void fse_render_frame(void);

//void fse_render_texture(fse_texture_t *texture, int x, int y);
//void fs_emu_render_texture_with_size(
//        fse_texture_t *texture, int x, int y, int w, int h)

void fse_image_size(int image, int *w, int *h);
void fse_render_image(int image, int x, int y);
void fse_render_image_with_size(int image, int x, int y, int w, int h);
//void fse_render_image_with_size_and_alpha(
//        int image, int x, int y, int w, int h, int alpha);


#ifdef FSE_INTERNAL_API

typedef struct fse_render {
    int width;
    int height;
    int stretch_mode;
    int scale_mode;
    double pixel_aspect;
    int view_x;
    int view_y;
    int view_w;
    int view_h;
} fse_render_t;

extern fse_render_t fse_render;

#endif /* FSE_INTERNAL_API */

#ifdef __cplusplus
}
#endif

#endif /* FSE_RENDER_H */
