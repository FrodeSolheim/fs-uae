#ifdef FSGS

#include "fsgs.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define FSGS_PATH_MAX 4096

// #define USE_GLIB
// #include <fs/emu/video.h>

#include <SDL_opengl.h>
#include <png.h>

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

GLuint openglTexture = 0;
int openglWidth = 1024;
int openglHeight = 576;
double openglScaleX = 1.0;
double openglScaleY = 1.0;
int openglTextureWidth = 1024;
int openglTextureHeight = 1024;
double filter_horiz_offset = 0.0;
double filter_vert_offset = 0.0;
double filter_horiz_zoom = 1.0;
double filter_vert_zoom = 1.0;
double mouse_tx = 0.0;
double mouse_ty = 0.0;
double mouse_sx = 1.0;
double mouse_sy = 1.0;
void updateg_screen(void);

#ifdef WITH_SDL2
#else
static SDL_Surface *g_screen = NULL;
static SDL_Surface *g_surface = NULL;
static int g_surface_depth = 0;

static int g_window_w;
static int g_window_h;
static int g_window_bpp;
static int g_window_center_x;
static int g_window_center_y;
static int g_fullscreen;
static int g_fullscreen_x;
static int g_fullscreen_y;
static int g_fullscreen_w;
static int g_fullscreen_h;
static int g_fullscreen_mode;

static int g_current_w;
static int g_current_h;
static double g_fsgs_scale_x = 1.0;
static double g_fsgs_scale_y = 1.0;
#endif

#define FULLSCREEN_MODE_WINDOW '1'
#define FULLSCREEN_MODE_FULLSCREEN '2'
#define FULLSCREEN_MODE_DESKTOP '3'

static int g_fsgs_screenshot_crop[4] = {};
// static int g_crop[4] = {};
/* Whether to stretch graphics to fill window or not. */
static int g_stretch = 0;
static double g_aspect = 1.0;

static int g_border;
static int g_border_crop[3][4];

struct texture {
    int width;
    int height;
    void *data;
    GLuint texture;
};

static struct texture g_skin_left;
static struct texture g_skin_right;
static struct texture g_skin_left_overlay;
static struct texture g_skin_right_overlay;

static double g_skin_left_pos = -1.0;
// static double g_skin_right_pos = 1.0;

struct WindowOverride {
    int sx;
    int sy;
    int sw;
    int sh;
    int dx;
    int dy;
    int dw;
    int dh;
#if 0
    int ssx;
    int ssy;
    int ssw;
    int ssh;
#endif
    struct WindowOverride *next;
};

static struct WindowOverride *g_window_override[2];
static struct WindowOverride *g_last_window_override[2];

void fsgs_init_window_overrides(void);

void fsgs_init(void)
{
    static int initialized = 0;
    if (initialized) {
        return;
    }
    initialized = 1;
    printf("[FSGS] Init\n");

    if (getenv("FSGS_BORDER")) {
        if (getenv("FSGS_BORDER")[0] == '1') {
            g_border = 1;
        } else if (getenv("FSGS_BORDER")[0] == '2') {
            g_border = 2;
        }
    }
    if (getenv("FSGS_STRETCH")) {
        if (getenv("FSGS_STRETCH")[0] == '1') {
            g_stretch = 1;
        } else if (getenv("FSGS_STRETCH")[0] == '2') {
            g_stretch = 2;
        }
    }
    if (getenv("FSGS_FULLSCREEN_MODE")) {
        g_fullscreen_mode = getenv("FSGS_FULLSCREEN_MODE")[0];
    }
    if (getenv("FSGS_FULLSCREEN_RECT")) {
        int x = 0, y = 0, w = 0, h = 0;
        sscanf(getenv("FSGS_FULLSCREEN_RECT"), "%d,%d,%d,%d", &x, &y, &w, &h);
        g_fullscreen_x = x;
        g_fullscreen_y = y;
        g_fullscreen_w = w;
        g_fullscreen_h = h;
    }
    if (g_fullscreen_w == 0 || g_fullscreen_h == 0) {
        const SDL_VideoInfo *info = SDL_GetVideoInfo();
        g_fullscreen_w = info->current_w;
        g_fullscreen_h = info->current_h;
    }
    if (getenv("FSGS_WINDOW_CENTER")) {
        int x = 0, y = 0;
        sscanf(getenv("FSGS_WINDOW_CENTER"), "%d,%d", &x, &y);
        g_window_center_x = x;
        g_window_center_y = y;
    }

    fsgs_init_window_overrides();
}

static void load_texture_from_file(struct texture *texture, const char *file)
{
    printf("[FSGS] Load texture from %s\n", file);
    int result = fsgs_read_png_file(
        file, &texture->data, &texture->width, &texture->height);
    if (result != FSGS_NO_ERROR) {
        return;
    }
    glGenTextures(1, &texture->texture);
    glBindTexture(GL_TEXTURE_2D, texture->texture);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 texture->width,
                 texture->height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 texture->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

static int g_fs_opengl_initialized = 0;

void fsgs_init_opengl(void)
{
    if (g_fs_opengl_initialized) {
        return;
    }
    g_fs_opengl_initialized = 1;
    printf("[FSGS] Init OpenGL\n");
    if (getenv("FSGS_BEZEL_LEFT")) {
        load_texture_from_file(&g_skin_left, getenv("FSGS_BEZEL_LEFT"));
    }
    if (getenv("FSGS_BEZEL_RIGHT")) {
        load_texture_from_file(&g_skin_right, getenv("FSGS_BEZEL_RIGHT"));
    }
    if (getenv("FSGS_BEZEL_LEFT_OVERLAY")) {
        load_texture_from_file(&g_skin_left_overlay,
                               getenv("FSGS_BEZEL_LEFT_OVERLAY"));
    }
    if (getenv("FSGS_BEZEL_RIGHT_OVERLAY")) {
        load_texture_from_file(&g_skin_right_overlay,
                               getenv("FSGS_BEZEL_RIGHT_OVERLAY"));
    }
}

//#ifdef _WIN32
//#define fsgs_getenv_path(x) getenv(x "_MBCS")
//#else
#define fsgs_getenv_path getenv
//#endif

const char *fsgs_window_title(const char *fallback)
{
    const char *window_title = NULL;
    if (window_title != NULL) {
        return window_title;
    }
    /* FIXME: UTF-8 / Unicode handling. */
    if (getenv("FSGS_WINDOW_TITLE")) {
        window_title = getenv("FSGS_WINDOW_TITLE");
    } else {
        window_title = fallback;
    }
    return window_title;
}

int fsgs_screenshot_crop(int *x, int *y, int *w, int *h)
{
    *x = g_fsgs_screenshot_crop[0];
    *y = g_fsgs_screenshot_crop[1];
    *w = g_fsgs_screenshot_crop[2];
    *h = g_fsgs_screenshot_crop[3];
    return (g_fsgs_screenshot_crop[2] && g_fsgs_screenshot_crop[3]);
}

void fsgs_set_screenshot_crop(int x, int y, int w, int h)
{
    g_fsgs_screenshot_crop[0] = x;
    g_fsgs_screenshot_crop[1] = y;
    g_fsgs_screenshot_crop[2] = w;
    g_fsgs_screenshot_crop[3] = h;
}

const char *fsgs_screenshots_dir(void)
{
    const char *screenshots_dir = NULL;
    if (screenshots_dir != NULL) {
        return screenshots_dir;
    }
    if (fsgs_getenv_path("FSGS_SCREENSHOTS_DIR")) {
        screenshots_dir = fsgs_getenv_path("FSGS_SCREENSHOTS_DIR");
    } else {
        screenshots_dir = "/tmp";
    }
    return screenshots_dir;
}

const char *fsgs_screenshots_base(void)
{
    const char *screenshots_base = NULL;
    if (screenshots_base != NULL) {
        return screenshots_base;
    }
    if (fsgs_getenv_path("FSGS_SCREENSHOTS_BASE")) {
        screenshots_base = fsgs_getenv_path("FSGS_SCREENSHOTS_BASE");
    } else {
        screenshots_base = "screenshot";
    }
    return screenshots_base;
}

const char *fsgs_screenshot_path(const char *type)
{
    static int counter = 1;
    static char buffer[FSGS_PATH_MAX];
    const char *base = fsgs_screenshots_base();
    const char *dir = fsgs_screenshots_dir();
    time_t tm1;
    time(&tm1);
    struct tm *tm2 = localtime(&tm1);
    static char time_buffer[64];
    strftime(time_buffer, 64 - 1, "%y%m%d%H%M", tm2);
    time_buffer[64 - 1] = '\0';
    while (1) {
        snprintf(buffer,
                 FSGS_PATH_MAX - 1,
                 "%s/%s-%s-%s-%02d.png",
                 dir,
                 base,
                 type,
                 time_buffer,
                 counter);
        counter += 1;
        break;
    }
    buffer[FSGS_PATH_MAX - 1] = '\0';
    printf("[FSGS] Screenshot: %s\n", buffer);
    return buffer;
}

double fsgs_aspect(void)
{
    return g_aspect;
}

void fsgs_set_aspect(double aspect)
{
    printf("[FSGS] Pixel aspect = %0.2f\n", aspect);
    g_aspect = aspect;
}

void fsgs_set_render_scale(double scale_x, double scale_y)
{
    g_fsgs_scale_x = scale_x;
    g_fsgs_scale_y = scale_y;
}

void fsgs_register_window_size(int w, int h)
{
    g_current_w = w;
    g_current_h = h;
}

void fsgs_scale_to_aspect(int w, int h)
{
    double scale_x = 1.0;
    double scale_y = 1.0;
    if (fsgs_stretch() == FSGS_STRETCH_FILL_SCREEN) {
    } else {
        double pixel_aspect = 1.0;
        if (fsgs_stretch() == FSGS_STRETCH_ASPECT) {
            pixel_aspect = fsgs_aspect();
        }
        double initial_aspect = (double) g_current_w / g_current_h;
        double aspect = ((double) w / h) / pixel_aspect;
        if (aspect < initial_aspect) {
            scale_x = aspect / initial_aspect;
        } else {
            scale_y = initial_aspect / aspect;
        }
    }

#if 0
    if (scale_x > 0.95) {
        scale_x = 1.0;
    }
    if (scale_y > 0.95) {
        scale_y = 1.0;
    }
#endif

    if (scale_x != 1.0 || scale_y != 1.0) {
        glScalef(scale_x, scale_y, 1.0);
    }
    fsgs_set_render_scale(scale_x, scale_y);
}

int fsgs_stretch(void)
{
    return g_stretch;
}

void fsgs_set_stretch(int stretch)
{
    printf("[FSGS] Stretch = %d\n", stretch);
    g_stretch = stretch;
}

void fsgs_cycle_stretch(void)
{
    if (fsgs_stretch() == FSGS_NO_STRETCH) {
        if (fsgs_aspect() == 1.0) {
            fsgs_set_stretch(FSGS_STRETCH_FILL_SCREEN);
        } else {
            fsgs_set_stretch(FSGS_STRETCH_ASPECT);
        }
    } else if (fsgs_stretch() == FSGS_STRETCH_ASPECT) {
        fsgs_set_stretch(FSGS_STRETCH_FILL_SCREEN);
    } else {
        fsgs_set_stretch(FSGS_NO_STRETCH);
    }
}

int fsgs_border(void)
{
    return g_border;
}

void fsgs_set_border(int border)
{
    g_border = border;
}

void fsgs_cycle_border(void)
{
    if (fsgs_border() == FSGS_NO_BORDER) {
        fsgs_set_border(FSGS_LARGE_BORDER);
    } else if (fsgs_border() == FSGS_SMALL_BORDER) {
        fsgs_set_border(FSGS_NO_BORDER);
    } else {
        fsgs_set_border(FSGS_SMALL_BORDER);
    }
}

void fsgs_set_crop(int x, int y, int w, int h)
{
    int i;
    /* Sets cropping for all border modes. */
    for (i = 0; i < 2; i++) {
        g_border_crop[i][0] = x;
        g_border_crop[i][1] = y;
        g_border_crop[i][2] = w;
        g_border_crop[i][3] = h;
    }
}

void fsgs_set_border_crop(int mode, int x, int y, int w, int h)
{
    if (mode < 0 || mode > 2) {
        printf("[FSGS] WARNING: Invalid border crop mode\n");
        return;
    }
    g_border_crop[mode][0] = x;
    g_border_crop[mode][1] = y;
    g_border_crop[mode][2] = w;
    g_border_crop[mode][3] = h;
}

#ifndef WITH_SDL2

static void update_video_mode(void)
{
    static char env[42];
    static int first_window_mode = 1;

    int w = g_window_w;
    int h = g_window_h;
    int bpp = g_window_bpp;
    int flags = SDL_OPENGL | SDL_DOUBLEBUF;
    if (g_fullscreen) {
        snprintf(env,
                 42,
                 "SDL_VIDEO_WINDOW_POS=%d,%d",
                 g_fullscreen_x,
                 g_fullscreen_y);
        SDL_putenv(env);
        printf("[FSGS] %s\n", env);
        w = g_fullscreen_w;
        h = g_fullscreen_h;
        if (g_fullscreen_mode == FULLSCREEN_MODE_WINDOW) {
            flags |= SDL_NOFRAME;
        } else {
            flags |= SDL_FULLSCREEN;
        }
    } else {
        if (first_window_mode) {
            first_window_mode = 0;
            if (g_window_center_x && g_window_center_y) {
                int x = g_window_center_x - w / 2;
                int y = g_window_center_y - h / 2;
                snprintf(env, 42, "SDL_VIDEO_WINDOW_POS=%d,%d", x, y);
                SDL_putenv(env);
                printf("[FSGS] %s\n", env);
            }
        }
        flags |= SDL_RESIZABLE;
    }
    g_screen = SDL_SetVideoMode(w, h, bpp, flags);
    fsgs_register_window_size(w, h);
    printf("[FSGS] Size is now %dx%d (wanted %dx%d)\n",
           g_current_w,
           g_current_h,
           w,
           h);
    glViewport(0, 0, g_current_w, g_current_h);

    /* Make sure that subsequent window resizing does not reset the window
     * position. */
    snprintf(env, 42, "SDL_VIDEO_WINDOW_POS=");
    SDL_putenv(env);

    /* Force re-initialization of textures since context is (probably) lost. */
    g_fs_opengl_initialized = 0;
    fsgs_init_opengl();
}

void fsgs_create_window(int fullscreen)
{
    /* Call fsgs_init, in case it has not already been called. */
    fsgs_init();
    // static char env[42];
#if 0
    int fsgs_fullscreen = 0;
    if (getenv("FSGS_FULLSCREEN")) {
        sscanf(getenv("FSGS_FULLSCREEN"), "%d", &fsgs_fullscreen);
    }
#endif
    if (getenv("FSGS_GEOMETRY")) {
        int fsx, fsy, fsw, fsh;
        sscanf(getenv("FSGS_GEOMETRY"), "%d,%d,%d,%d", &fsx, &fsy, &fsw, &fsh);
        g_fullscreen_x = fsx;
        g_fullscreen_y = fsy;
        g_fullscreen_w = fsw;
        g_fullscreen_h = fsh;
    }
#if 0
    int fsgs_sdl_flags = SDL_OPENGL | SDL_DOUBLEBUF;
    fsgs_sdl_flags |= SDL_RESIZABLE;
    if (fsgs_fullscreen) {
        g_fullscreen = 1;
        if (fsgs_fullscreen == 1) {
            snprintf(env, 42, "SDL_VIDEO_WINDOW_POS=%d,%d", fsgs_x, fsgs_y);
            SDL_putenv(env);
            fsgs_sdl_flags |= SDL_NOFRAME;
            fsgs_sdl_flags &= ~SDL_RESIZABLE;
        } else {
            fsgs_sdl_flags |= SDL_FULLSCREEN;
        }
    } else {
        fsgs_w = 960;
        fsgs_h = 540;
    }
    g_window_w = fsgs_w;
    g_window_h = fsgs_h;
#endif
    printf("[FSGS] Create window fullscreen=%d\n", fullscreen);
    g_fullscreen = fullscreen;
    g_window_w = 960;
    g_window_h = 540;

    int vsync = 0;
    if (getenv("FSGS_VSYNC")) {
        if (getenv("FSGS_VSYNC")[0] == '1') {
            vsync = 1;
        }
    }
    if (vsync) {
        printf("[FSGS] Setting OpenGL swap interval to 1\n");
        SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
    } else {
        /* Important to set it to 0 here, some OS/drivers may default
         * to some kind of V-Sync / frame limiting. E.g. DOSBox (70Hz)
         * on Windows 10 with nVIDIA is fixed with this. */
        printf("[FSGS] Setting OpenGL swap interval to 0\n");
        SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 0);
    }
    printf("[FSGS] Setting OpenGL double-buffer mode\n");
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    update_video_mode();

#if 0
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, fsgs_w, fsgs_h, 0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
#endif
    glGenTextures(1, &openglTexture);
    glBindTexture(GL_TEXTURE_2D, openglTexture);
    int tw = openglTextureWidth, th = openglTextureHeight;
    void *data = malloc(tw * th * 4);
    memset(data, 0x0, tw * th * 4);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    free(data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);
#if 0
    if (getenv("FILTER_VERT_OFFSET")) {
            sscanf(getenv("FILTER_VERT_OFFSET"), "%lf",
                    &filter_vert_offset);
    }
    if (getenv("FILTER_HORIZ_OFFSET")) {
            sscanf(getenv("FILTER_HORIZ_OFFSET"), "%lf",
                    &filter_horiz_offset);
    }
    if (getenv("FILTER_VERT_ZOOM")) {
            sscanf(getenv("FILTER_VERT_ZOOM"), "%lf",
                    &filter_vert_zoom);
    }
    if (getenv("FILTER_HORIZ_ZOOM")) {
            sscanf(getenv("FILTER_HORIZ_ZOOM"), "%lf",
                    &filter_horiz_zoom);
    }
#endif
    // FIXME: MOUSE EVENT COORDINATES MUST BE UPDATED
    // WITH ZOOM / OFFSET COMPENSATION
    openglWidth = g_current_w;
    openglHeight = g_current_h;

    /* Make sure that subsequent window resizing does not reset the window
     * position. */
    // snprintf(env, 42, "SDL_VIDEO_WINDOW_POS=");
    // SDL_putenv(env);

    fsgs_init_opengl();
}

SDL_Surface *fsgs_create_surface(int width, int height, int depth)
{
    openglScaleX = (double) g_current_w / (double) width;
    openglScaleY = (double) g_current_h / (double) height;

    if (g_surface) {
        SDL_FreeSurface(g_surface);
    }
    g_surface =
        SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, depth, 0, 0, 0, 0);
    g_surface_depth = depth;
    return g_surface;
}

void fsgs_render_texture(void)
{
    // printf("render surface\n");
    static GLdouble matrix[16];
#if 0
    int ax, ay;
    for (ay = 0; ay < g_surface->h; ay++) {
        for (ax = 0; ax < g_surface->w; ax++) {
            int16_t *pixels = g_surface->pixels;
            *(pixels + ay * g_surface->w + ax) = 0xffff;
        }
    }
#endif
    if (g_surface_depth == 32) {
        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        0,
                        0,
                        g_surface->w,
                        g_surface->h,
                        GL_RGBA,
                        GL_UNSIGNED_BYTE,
                        g_surface->pixels);
    } else if (g_surface_depth == 16) {
        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        0,
                        0,
                        g_surface->w,
                        g_surface->h,
                        GL_RGB,
                        GL_UNSIGNED_SHORT_5_6_5,
                        g_surface->pixels);
    } else {
        printf("Unsupported surface depth: %d\n", g_surface_depth);
    }

    /*
    int crop_x = g_crop[0];
    int crop_y = g_crop[1];
    int crop_w = g_crop[2];
    int crop_h = g_crop[3];
    */
    int crop_x = g_border_crop[g_border][0];
    int crop_y = g_border_crop[g_border][1];
    int crop_w = g_border_crop[g_border][2];
    int crop_h = g_border_crop[g_border][3];

    if (crop_w == 0 && crop_h == 0) {
        crop_x = 0;
        crop_y = 0;
        crop_w = g_surface->w;
        crop_h = g_surface->h;
    }

#if 0
    crop_x -= 10;
    crop_y -= 10;
    crop_w += 20;
    crop_h += 20;
#endif

#if 1
    double tx0 = (double) crop_x / (double) openglTextureWidth;
    double ty0 = (double) crop_y / (double) openglTextureHeight;
    double tx1 = tx0 + (double) crop_w / (double) openglTextureWidth;
    double ty1 = ty0 + (double) crop_h / (double) openglTextureHeight;
#else
    double tx0 = 0.0;
    double ty0 = 0.0;
    double tx1 = (double) g_surface->w / (double) openglTextureWidth;
    double ty1 = (double) g_surface->h / (double) openglTextureHeight;
#endif

    // tx1 = 1.0;
    // ty1 = 1.0;

    double ratio = MIN(((double) openglWidth / (double) g_surface->w),
                       ((double) openglHeight / (double) g_surface->h));
    float w = (int) (g_surface->w * ratio + 0.5);
    float h = (int) (g_surface->h * ratio + 0.5);
    float x = (openglWidth - w) / 2;
    float y = (openglHeight - h) / 2;

    x = 0;
    y = 0;
    w = openglWidth;
    h = openglHeight;

    glPushMatrix();
    fsgs_scale_to_aspect(crop_w, crop_h);
    /*
        double scale_x = 1.0;
        double scale_y = 1.0;
        if (fsgs_stretch() == FSGS_STRETCH_FILL_SCREEN) {

        } else {
            double pixel_aspect = 1.0;
            if (fsgs_stretch() == FSGS_STRETCH_ASPECT) {
                pixel_aspect = fsgs_aspect();
            }
            double initial_aspect = (double) g_current_w / g_current_h;
            double aspect = pixel_aspect * (double) crop_w / crop_h;
            if (aspect < initial_aspect) {
                scale_x = aspect / initial_aspect;
            } else {
                scale_y = initial_aspect / aspect;
            }
        }

        if (scale_x != 1.0 || scale_y != 1.0) {
            glScalef(scale_x, scale_y, 1.0);
        }
    */
    // glTranslatef(0 + 960 / 2.0f, 0 + 540 / 2.0f, 0.0f);
    // glScalef(950 / 2.0f, 530 / 2.0f, 1.0f);

    // glTranslatef(x + w / 2.0f, y + h / 2.0f, 0.0f);
    // glScalef(w / 2.0f, h / 2.0f, 1.0f);
#if 0
    glScaled(filter_horiz_zoom, filter_vert_zoom, 1.0);
    glTranslated(filter_horiz_offset, filter_vert_offset, 1.0);
    glGetDoublev(GL_MODELVIEW_MATRIX, matrix);
    mouse_sx = ratio * filter_horiz_zoom;
    mouse_sy = ratio * filter_vert_zoom;
    mouse_tx = -1.0 * matrix[0] + matrix[12];
    mouse_ty = -1.0 * matrix[5] + matrix[13];
#endif
    // glClearColor(1.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glTexCoord2f(tx0, ty1);
    glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(tx1, ty1);
    glVertex2f(1.0f, -1.0f);
    glTexCoord2f(tx1, ty0);
    glVertex2f(1.0f, 1.0f);
    glTexCoord2f(tx0, ty0);
    glVertex2f(-1.0f, 1.0f);
#if 0
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f (0, 0);
    glVertex2f (100, 0);
    glVertex2f (100, 100);
    glVertex2f (0, 100);
#endif
    glEnd();

    glPopMatrix();
}

void fsgs_render_surface(void)
{
    // printf("render surface\n");
    // static GLdouble matrix[16];
#if 0
    int ax, ay;
    for (ay = 0; ay < g_surface->h; ay++) {
        for (ax = 0; ax < g_surface->w; ax++) {
            int16_t *pixels = g_surface->pixels;
            *(pixels + ay * g_surface->w + ax) = 0xffff;
        }
    }
#endif
    if (g_surface_depth == 32) {
        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        0,
                        0,
                        g_surface->w,
                        g_surface->h,
                        GL_RGBA,
                        GL_UNSIGNED_BYTE,
                        g_surface->pixels);
    } else if (g_surface_depth == 16) {
        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        0,
                        0,
                        g_surface->w,
                        g_surface->h,
                        GL_RGB,
                        GL_UNSIGNED_SHORT_5_6_5,
                        g_surface->pixels);
    } else {
        printf("Unsupported surface depth: %d\n", g_surface_depth);
    }
    fsgs_render_texture();
}

void render_texture_with_size(
    struct texture *texture, int x, int y, int w, int h)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture->texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0);
    glVertex2f(x, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex2f(x + w, 0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex2f(x + w, h);
    glTexCoord2f(0.0, 0.0);
    glVertex2f(x, h);
    glEnd();
}

void render_texture(struct texture *texture, int x, int y)
{
    render_texture_with_size(texture, x, y, texture->width, texture->height);
}

static int g_fsgs_attribs_level;
void fsgs_push_attribs(void)
{
    if (g_fsgs_attribs_level == 0) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
    }

    g_fsgs_attribs_level += 1;
}
void fsgs_pop_attribs(void)
{
    if (g_fsgs_attribs_level == 0) {
        printf("[FSGS] WARNING: Attribs levels is already 0\n");
    }
    if (g_fsgs_attribs_level == 1) {
        glPopAttrib();
    }
    g_fsgs_attribs_level -= 1;
}

static int g_fsgs_screen_matrix_level;
void fsgs_push_screen_matrix(void)
{
    if (g_fsgs_screen_matrix_level == 0) {
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(-1.0, -1.0, 0.0);
        glScalef(2.0 / g_current_w, 2.0 / g_current_h, 1.0);
    }

    g_fsgs_screen_matrix_level += 1;
}
void fsgs_pop_screen_matrix(void)
{
    if (g_fsgs_screen_matrix_level == 0) {
        printf("[FSGS] WARNING: Screen matrix level is already 0\n");
    }
    if (g_fsgs_screen_matrix_level == 1) {
        glPopMatrix();
    }
    g_fsgs_screen_matrix_level -= 1;
}

void fsgs_render_overlays(void)
{
    fsgs_init_opengl();

    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    fsgs_push_attribs();
    fsgs_push_screen_matrix();

    glDisable(GL_DEPTH_TEST);

    /*
    g_current_w = 1920;
    g_current_h = 1080;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    */

    /*
     * g_current_w, g_current_h is the pixel size of the window.
     * g_fsgs_scale_x [0.0, 1.0] tells us how wide the game output is relative
     * to the window width.
     */

    /*
    g_current_w = 1920;
    g_current_h = 1080;
    g_fsgs_scale_x = 0.5;
    printf("g_current_w %d g_current_h %d\n", g_current_w, g_current_h);
    */
#if 0
    glLoadIdentity();
    glTranslatef(-1.0, -1.0, 0.0);
    glScalef(2.0 / g_current_w, 2.0 / g_current_h, 1.0);
#endif

    double want_left_pos = -(2.0 * g_fsgs_scale_x) / 2.0;
#if 0
    if (want_left_pos > g_skin_left_pos) {
        //g_skin_left_pos = min(want_left_pos, g_skin_left_pos + 0.01);
        g_skin_left_pos = g_skin_left_pos + 0.005;
        if (want_left_pos < g_skin_left_pos) {
            g_skin_left_pos = want_left_pos;
        }
    }
    else if (want_left_pos < g_skin_left_pos) {
        //g_skin_left_pos = min(want_left_pos, g_skin_left_pos + 0.01);
        g_skin_left_pos = g_skin_left_pos - 0.005;
        if (want_left_pos > g_skin_left_pos) {
            g_skin_left_pos = want_left_pos;
        }
    }
#else
    g_skin_left_pos = want_left_pos;
#endif

    double left_border = g_current_w * (g_skin_left_pos + 1.0) / 2;
    double right_border = g_current_w - left_border;
    // printf("%0.2f  %d\n", g_skin_left_pos, left_border);

    if (left_border != g_skin_left_pos * g_current_w) {
    }

    double scale = g_current_h / 1080.0;
    int x, y, w, h;
    y = 0;
    h = g_current_h;

    if (g_skin_left.texture && left_border > 0) {
        w = g_skin_left.width * scale + 0.5;
        x = left_border - w;
        render_texture_with_size(&g_skin_left, x, y, w, h);
        if (g_skin_right.texture) {
            w = g_skin_right.width * scale + 0.5;
            x = right_border;
            render_texture_with_size(&g_skin_right, x, y, w, h);
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (g_skin_left_overlay.texture) {
            w = g_skin_left_overlay.width * scale + 0.5;
            x = left_border;
            render_texture_with_size(&g_skin_left_overlay, x, y, w, h);
        }
        if (g_skin_right_overlay.texture) {
            w = g_skin_right_overlay.width * scale + 0.5;
            x = right_border - w;
            render_texture(&g_skin_right_overlay,
                           right_border - g_skin_right_overlay.width,
                           0);
        }
        // FIXME
        glDisable(GL_BLEND);
    }
#if 0
    static int opacity = 0;
    if (opacity != 255) {
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0, 0.0, 0.0, (255 - opacity) / 255.0);
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(1920, 0);
        glVertex2f(1920, 1080);
        glVertex2f(0, 1080);
        glEnd();
    }
    if (opacity < 255) {
        opacity += 5;
    }
#endif
#if 0
    glDisable(GL_TEXTURE_2D);
    static int x = 0, y = 0;
    glColor3f(1.0, 1.0, 1.0);
    for (int i = -1; i < g_current_h / 40; i++) {
        glBegin(GL_QUADS);
        glVertex2f(x, i * 40 + y);
        glVertex2f(x + 20, i * 40 + y);
        glVertex2f(x + 20, i * 40 + y + 20);
        glVertex2f(x, i * 40 + y + 20);
        glEnd();
    }
    if (--y <= 0) {
        y = 39;
    }
#endif

    // fsemu_render_hud();

    // glPopMatrix();
    // glPopAttrib();
    fsgs_pop_screen_matrix();
    fsgs_pop_attribs();
}

void fsgs_render_fade(void)
{
    fsgs_init_opengl();

    fsgs_push_attribs();
    fsgs_push_screen_matrix();

    glDisable(GL_DEPTH_TEST);
    static int opacity = 0;
    if (opacity != 255) {
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0, 0.0, 0.0, (255 - opacity) / 255.0);
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(g_current_w, 0);
        glVertex2f(g_current_w, g_current_h);
        glVertex2f(0, g_current_h);
        glEnd();
    }
    if (opacity < 255) {
        opacity += 5;
    }

    fsgs_pop_screen_matrix();
    fsgs_pop_attribs();
}

void fsgs_swap_buffers(void)
{
    fsgs_push_attribs();
    fsgs_push_screen_matrix();

    fsgs_render_overlays();
    fsgs_render_fade();

    static int last_time = 0;
    int ticks = SDL_GetTicks();
    if (last_time > 0) {
        int dt = ticks - last_time;
#if 0
        printf("%d\n", dt);
#endif
    };
    last_time = ticks;
    SDL_GL_SwapBuffers();
    glFlush();

    fsgs_pop_screen_matrix();
    fsgs_pop_attribs();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

#endif

bool fsgs_check_mod(SDL_Event *event)
{
    int mod = event->key.keysym.mod;
    mod &= ~(KMOD_NUM | KMOD_CAPS);
    return mod == FSGS_MOD_MOD;
}

int fsgs_handle_sdl_event(SDL_Event *event)
{
    if (event->type == SDL_VIDEORESIZE) {
        int w = event->resize.w;
        int h = event->resize.h;
        // int bpp = 32;
        // int flags = SDL_OPENGL | SDL_DOUBLEBUF;
        // flags |= SDL_RESIZABLE;
#if 0
        if (fsgs_fullscreen) {
            if (fsgs_fullscreen == 1) {
                static char env[42];
                snprintf(env, 42, "SDL_VIDEO_WINDOW_POS=%d,%d", fsgs_x, fsgs_y);
                SDL_putenv(env);
                fsgs_sdl_flags |= SDL_NOFRAME;
            } else {
                fsgs_sdl_flags |= SDL_FULLSCREEN;
#endif
        printf("[FSGS] Set Video Mode %d %d (SDL_VIDEORESIZE)\n", w, h);
        g_window_w = w;
        g_window_h = h;
        // g_window_bpp = flags;
        // g_window_flags = flags;
        // SDL_SetVideoMode(w, h, bpp, flags);
        update_video_mode();
    }
    return 0;
}

int fsgs_fullscreen(void)
{
    return g_fullscreen;
}

void fsgs_set_fullscreen(int fullscreen)
{
    if (fullscreen == g_fullscreen) {
        return;
    }
    printf("[FSGS] Fullscreen = %d\n", fullscreen);
    g_fullscreen = fullscreen;
    update_video_mode();
}

int64_t fsgs_micros(void)
{
}

int fsgs_read_png_file(const char *path,
                       void **out_data,
                       int *out_width,
                       int *out_height)
{
    int y;
    int width, height;
    png_byte color_type;
    png_byte bit_depth;
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep *row_pointers;
    unsigned char header[8];
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        return 1;
    }
    if (fread(header, 1, 8, fp) != 8) {
        fclose(fp);
        return 2;
    }
    if (png_sig_cmp(header, 0, 8)) {
        fclose(fp);
        return 3;
    }
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 4;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fclose(fp);
        return 5;
    }
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);
    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_set_interlace_handling(png_ptr);
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
    }
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_ptr);
    }
    if (bit_depth < 8) {
        png_set_packing(png_ptr);
    }
    if (color_type == PNG_COLOR_TYPE_RGB) {
        png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
    }
    png_read_update_info(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    int channels = png_get_channels(png_ptr, info_ptr);
    unsigned char *data = (unsigned char *) malloc(width * height * channels);
    row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * height);
    for (y = 0; y < height; y++) {
        row_pointers[y] = data + width * y * channels;
    }
    png_read_image(png_ptr, row_pointers);
    fclose(fp);
    *out_data = data;
    *out_width = width;
    *out_height = height;
    return 0;
}

static int read_window_override_int(const char *s, int *pos, int *out)
{
    char temp[4];
    int read = 0;
    while (s[*pos] == ' ') ++(*pos);
    while (read < 3) {
        temp[read] = s[*pos];
        ++(*pos);
        ++read;
        char c = s[*pos];
        if (c >= '0' && c <= '9') {
            continue;
        }
        temp[read] = '\0';
        if (read == 1) {
            if (temp[0] == '*') {
                *out = -1;
                return 1;
            }
        }
        *out = atoi(temp);
        return 1;
    }
    /* read failed */
    return 0;
}

static int read_window_override(int type, const char *s, int *pos)
{
    while (s[*pos] == ' ') ++(*pos);
    int sx, sy, sw, sh;
    int dx, dy, dw, dh;
    int ssx = 0, ssy = 0, ssw = 0, ssh = 0;
    if (!read_window_override_int(s, pos, &sx))
        return 0;
    if (!read_window_override_int(s, pos, &sy))
        return 0;
    if (!read_window_override_int(s, pos, &sw))
        return 0;
    if (!read_window_override_int(s, pos, &sh))
        return 0;
    while (s[*pos] == ' ') ++(*pos);
#if 0
    if (s[(*pos)] == '[') {
        ++(*pos);
        if (!read_window_override_int(s, pos, &ssx)) return 0;
        if (!read_window_override_int(s, pos, &ssy)) return 0;
        if (!read_window_override_int(s, pos, &ssw)) return 0;
        if (!read_window_override_int(s, pos, &ssh)) return 0;
        while(s[*pos] == ' ') ++(*pos);
        if (s[(*pos)++] != ']') return 0;
        while(s[*pos] == ' ') ++(*pos);
    }
#endif
    if (s[(*pos)++] != '=')
        return 0;
    if (s[(*pos)++] == '>')
        ++(*pos);
    if (!read_window_override_int(s, pos, &dx))
        return 0;
    if (!read_window_override_int(s, pos, &dy))
        return 0;
    if (!read_window_override_int(s, pos, &dw))
        return 0;
    if (!read_window_override_int(s, pos, &dh))
        return 0;
    printf(
        "[FSGS] Viewport transformation: %3d %3d %3d %3d "
        "=> %3d %3d %3d %3d\n",
        sx,
        sy,
        sw,
        sh,
        dx,
        dy,
        dw,
        dh);
    struct WindowOverride *wo =
        (struct WindowOverride *) malloc(sizeof(struct WindowOverride));
    wo->sx = sx;
    wo->sy = sy;
    wo->sw = sw;
    wo->sh = sh;
    wo->dx = dx;
    wo->dy = dy;
    wo->dw = dw;
    wo->dh = dh;
#if 0
    wo->ssx = ssx;
    wo->ssy = ssy;
    wo->ssw = ssw;
    wo->ssh = ssh;
#endif
    wo->next = NULL;

    if (g_last_window_override[type] == NULL) {
        g_window_override[type] = wo;
    } else {
        g_last_window_override[type]->next = wo;
    }
    g_last_window_override[type] = wo;
    return 1;
}

static void init_window_overrides_2(int type, const char *s)
{
    int pos = 0;
    while (1) {
        int result = read_window_override(type, s, &pos);
        if (!result) {
            printf("[FSGS] Error parsing wiewport transformation\n");
        }
        while (s[pos] == ' ') {
            ++(pos);
        }
        int c = s[(pos)++];
        if (c == ';') {
            continue;
        } else if (c == ',') {
            continue;
        } else if (c == '\0') {
            break;
        } else {
            printf(
                "[FSGS] Unexpected byte (%d) while parsing "
                "viewport option\n",
                c);
            return;
        }
    }
}

void fsgs_init_window_overrides(void)
{
    const char *s;
    s = getenv("FSGS_VIEWPORT");
    if (s) {
        init_window_overrides_2(0, s);
    }
}

#endif /* FSGS */

#ifdef FSEMU

#ifdef __linux__
#define LINUX 1
#define HAVE_STRUCT_STAT_ST_BLOCKS 1
#define HAVE_STRUCT_STAT_ST_MTIM_TV_NSEC 1
#define HAVE_ENDIAN_H 1
#endif

#ifdef __APLE__
#define MACOS 1
#define MACOSX 1
#define HAVE_STRUCT_STAT_ST_BLOCKS 1
#define HAVE_STRUCT_STAT_ST_MTIMESPEC_TV_NSEC 1
#define HAVE_STRUCT_STAT_ST_RDEV 1
#define HAVE_LIBKERN_OSBYTEORDER_H 1
#endif

#ifdef _WIN32
#define WINDOWS 1
#endif

#define USE_GLIB 1
#define USE_OPENGL 1
#define USE_PNG 1
#define USE_SDL 1

#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline void fs_emu_warning(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    char *buffer = g_strdup_vprintf(format, ap);
    va_end(ap);
    int len = strlen(buffer);
    // strip trailing newline, of any
    if (len > 0 && buffer[len] == '\n') {
        buffer[len] = '\0';
    }
    // fs_log("WARNING: %s\n", buffer);
    printf("WARNING: %s\n", buffer);
    g_free(buffer);
}

void fs_emu_fatal(const char *msg)
{
    // fsemu_log("FATAL: %s\n", msg);
    printf("FATAL: %s\n", msg);
    exit(1);
}

int fs_emu_get_video_format()
{
    return 0;  // FS_EMU_VIDEO_FORMAT_BGRA
}

#include "../fs-uae/libfsemu/src/base.c"
#include "../fs-uae/libfsemu/src/conf.c"
#include "../fs-uae/libfsemu/src/data.c"
#include "../fs-uae/libfsemu/src/emu/texture.c"
#include "../fs-uae/libfsemu/src/emu/theme.c"
#include "../fs-uae/libfsemu/src/filesys.c"
#include "../fs-uae/libfsemu/src/image.c"
//#include "../fs-uae/libfsemu/src/inifile.c"
#include "../fs-uae/libfsemu/src/log.c"
#include "../fs-uae/libfsemu/src/opengl.c"
#include "../fs-uae/libfsemu/src/path.c"
#include "../fs-uae/libfsemu/src/ref.c"
#include "../fs-uae/libfsemu/src/thread.c"
#include "../fs-uae/libfsemu/src/time.c"
#include "../fs-uae/libfsemu/src/video/opengl-render.c"
#include "../fs-uae/libfsemu/src/video/render.c"

#ifdef __cplusplus
}
#endif

#endif
