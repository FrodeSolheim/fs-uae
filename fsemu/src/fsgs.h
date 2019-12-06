#ifndef FSGS_H_
#define FSGS_H_

#ifdef FSGS

#ifdef WITH_SDL2

#else
#include <SDL.h>
#ifdef __APPLE__
/* KMOD_LMETA */
//#define FSGS_MOD 0x0400
#define FSGS_MOD_MOD KMOD_LMETA
#else
/* KMOD_LALT */
//#define FSGS_MOD 0x0100
#define FSGS_MOD_MOD KMOD_LALT
#endif
#define FSGS_KEY_MENU SDLK_F12
#define FSGS_KEY_PAUSE SDLK_p
#define FSGS_KEY_SCREENSHOT SDLK_s
#define FSGS_KEY_QUIT SDLK_q
#define FSGS_KEY_STRETCH SDLK_a /* Aspect */
#define FSGS_KEY_BORDER SDLK_b
#define FSGS_KEY_ZOOM SDLK_z
#define FSGS_KEY_GRAB SDLK_g
#define FSGS_KEY_WARP SDLK_w
#endif
/*
  KMOD_NONE  = 0x0000,
  KMOD_LSHIFT= 0x0001,
  KMOD_RSHIFT= 0x0002,
  KMOD_LCTRL = 0x0040,
  KMOD_RCTRL = 0x0080,
  KMOD_LALT  = 0x0100,
  KMOD_RALT  = 0x0200,
  KMOD_LMETA = 0x0400,
  KMOD_RMETA = 0x0800,
  KMOD_NUM   = 0x1000,
  KMOD_CAPS  = 0x2000,
  KMOD_MODE  = 0x4000,
#endif
*/

#include <stdbool.h>

void fsgs_init(void);
void fsgs_init_opengl(void);

void fsgs_create_window(int fullscreen);
const char *fsgs_window_title(const char *fallback);

void fsgs_register_window_size(int w, int h);

SDL_Surface *fsgs_create_surface(int width, int height, int depth);
//#ifdef WITH_SDL
#include "SDL.h"
bool fsgs_check_mod(SDL_Event *event);
int fsgs_handle_sdl_event(SDL_Event *event);
//#else

double fsgs_aspect(void);
void fsgs_set_aspect(double aspect);
void fsgs_scale_to_aspect(int w, int h);

enum { FSGS_NO_STRETCH, FSGS_STRETCH_FILL_SCREEN, FSGS_STRETCH_ASPECT };

int fsgs_stretch(void);
void fsgs_set_stretch(int stretch);
void fsgs_cycle_stretch(void);

enum { FSGS_NO_BORDER, FSGS_SMALL_BORDER, FSGS_LARGE_BORDER };

int fsgs_border(void);
void fsgs_set_border(int border);
void fsgs_cycle_border(void);
void fsgs_set_border_crop(int mode, int x, int y, int w, int h);
void fsgs_set_crop(int x, int y, int w, int h);

int fsgs_fullscreen(void);
void fsgs_set_fullscreen(int fullscreen);

void fsgs_render_texture(void);
void fsgs_render_surface(void);
void fsgs_render_overlays(void);
void fsgs_render_fade(void);
void fsgs_set_render_scale(double scale_x, double scale_y);
void fsgs_swap_buffers(void);

const char *fsgs_screenshot_path(const char *type);
int fsgs_screenshot_crop(int *x, int *y, int *w, int *h);
void fsgs_set_screenshot_crop(int x, int y, int w, int h);

int64_t fsgs_micros(void);

int fsgs_read_png_file(const char *path,
                       void **out_data,
                       int *out_width,
                       int *out_height);

#define FSGS_NO_ERROR 0

#endif  // FSGS

#endif  // FSGS_H_
