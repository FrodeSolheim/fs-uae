#define FSEMU_INTERNAL
#include "fsemu-glvideo.h"

#include "fsemu-frame.h"
#include "fsemu-glib.h"
#include "fsemu-gui.h"
#include "fsemu-image.h"
#include "fsemu-layout.h"
#include "fsemu-opengl.h"
#include "fsemu-sdl.h"
#include "fsemu-sdlwindow.h"
#include "fsemu-time.h"
#include "fsemu-types.h"
#include "fsemu-util.h"
#include "fsemu-video.h"
#include "fsemu-window.h"

/*
static struct {
    fsemu_rect_t rect;
} fsemu_glvideo;

static int current_texture;
*/

void fsemu_glvideo_init(void)
{
    fsemu_return_if_already_initialized();
    // fsemu_opengl_init();
    fsemu_video_log("Initializing OpenGL video renderer\n");

    SDL_Window *window = fsemu_sdlwindow_window();
    // renderer = SDL_CreateRenderer(window, -1, renderer_flags);
    // renderer = SDL_CreateRenderer(fsemu_sdlwindow_window(), -1, 0);
    fsemu_video_log("%p\n", window);
    // fsemu_video_log("%p\n", renderer);

    int r, g, b;
    fsemu_video_background_color_rgb(&r, &g, &b);
#if 0
    SDL_SetRenderDrawColor(renderer, r, g, b, 0xff);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);

    SDL_SetRenderDrawColor(renderer, r, g, b, 0xff);
    SDL_RenderClear(renderer);
#endif

#if 0
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    textures[0] = SDL_CreateTexture(renderer,
                                    SDL_PIXELFORMAT_BGRA32,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    1024,
                                    1024);
    textures[1] = SDL_CreateTexture(renderer,
                                    SDL_PIXELFORMAT_BGRA32,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    1024,
                                    1024);
#endif
}

void fsemu_glvideo_work(int timeout_us)
{
    fsemu_video_frame_t *frame = fsemu_video_get_frame(timeout_us);
    if (!frame) {
        return;
    }
#if 0
    fsemu_video_log(" ---------------- draw got frame! (%dx%d) partial? %d\n",
                    frame->width,
                    frame->height,
                    frame->partial);
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = frame->width;
    rect.h = frame->height;
    uint8_t *pixels = frame->buffer;

    // FIXME: support for partial
    int y = 0;

    static int last_y;
    // static int last_x;
    if (frame->partial) {
        if (frame->partial <= last_y) {
            y = 0;
            // last_x = 0;
        } else {
            y = last_y;
        }
        rect.y = y;
        rect.h = frame->partial - y;
        pixels += y * frame->stride;

        // rect.x = last_x;
        // if (last_x == 0) {
        //     last_x = 32;
        // } else {
        //     last_x = 0;
        // }

        last_y = frame->partial;
    }

    // rect.y = y;
    // rect.h = rect.h - y;

    fsemu_video_log(" draw ___________ y %d -> %d %d %d %d \n",
                    y,
                    rect.x,
                    rect.y,
                    rect.w,
                    rect.h);

    SDL_UpdateTexture(textures[current_texture], &rect, pixels, frame->stride);

#if 1
    static uint8_t *greenline;
    if (greenline == NULL) {
        greenline = (uint8_t *) malloc(2048 * 4);
        for (int i = 0; i < 2048; i++) {
            ((int32_t *) greenline)[i] = FSEMU_RGB(0x00ff00);
        }
    }

    rect.h = 1;
    SDL_UpdateTexture(textures[current_texture], &rect, greenline, 2048 * 4);
#endif

    if (frame->partial > 0 && frame->partial != frame->height) {
        return;
    }

    fsemu_video_log(" draw ___________ READY______________ \n");

    // FIXME: Locked access for main/video thread separation?
    // fsemu_glvideo.rect.x = 0;
    // fsemu_glvideo.rect.y = 0;
    fsemu_glvideo.rect.w = frame->width;
    fsemu_glvideo.rect.h = frame->height;

    fsemu_video_set_ready(true);

    static int64_t last;
    int64_t now = fsemu_time_us();
    fsemu_video_log("_______draw _______diff __________ %lld\n", lld(now - last));
    last = now;

    static int framecount;
    framecount += 1;

    static int64_t framecount_time = 0;
    if (framecount_time == 0) {
        framecount = 0;
        framecount_time = fsemu_time_us();
    }
    if (now - framecount_time > 1000000) {
        double fps = framecount * 1000000.0 / (now - framecount_time);
        framecount = 0;
        framecount_time = now;
        // g_printerr("FPS: %0.2f\n", fps);
    }

    // FIXME: Free frame if it is a partial frame, otherwise save for render...
#endif
}

void fsemu_glvideo_render(void)
{
    fsemu_video_log("--- render --- [draw]\n");
#if 0
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = fsemu_glvideo.rect.w;
    src.h = fsemu_glvideo.rect.h;
    // src.w = 692;
    // src.h = 540;

    fsemu_layout_set_video_size(src.w, src.h);
    fsemu_layout_set_pixel_aspect(((double) src.w / src.h) / (4.0 / 3.0));

    SDL_Rect dst;
    fsemu_layout_video_rect(&dst);
    SDL_RenderCopy(renderer, textures[current_texture], &src, &dst);
    // SDL_RenderDrawRect(renderer, &dst);
#endif
}

static int64_t fsemu_glvideo_wait_for_swap(void)
{
    // printf("wait for swap\n");
    // FIXME: IMPROVE!
    glFinish();
    int64_t vsync_time = fsemu_time_us();
    return vsync_time;
}

void fsemu_glvideo_display(void)
{
    // fsemu_video_log("--- display --- [draw]\n");
    SDL_Window *window = fsemu_sdlwindow_window();
    SDL_GL_SwapWindow(window);

    if (fsemu_video_vsync()) {
        static int64_t last;
        int64_t vsync_time = fsemu_glvideo_wait_for_swap();
        fsemu_video_set_vsync_time(vsync_time);
        if (last != 0) {
            // printf("dt %d\n", (int) (vsync_time - last));
        }
        last = vsync_time;
    }

    // int r, g, b;
    // fsemu_video_background_color_rgb(&r, &g, &b);
    // glClearColorf

    glClear(GL_COLOR_BUFFER_BIT);
}

static void fsemu_glvideo_convert_coordinates(fsemu_drect_t *out,
                                              fsemu_rect_t *in,
                                              int coordinates)
{
    // FIXME: Use copy of window size? Layout?
    fsemu_size_t window_size;
    fsemu_window_size(&window_size);

    if (coordinates == FSEMU_COORD_REAL) {
        out->w = 2.0 * in->w / window_size.w;
        out->h = 2.0 * in->h / window_size.h;
        out->x = -1.0 + 2.0 * in->x / window_size.w;
        out->y = 1.0 - 2.0 * in->y / window_size.h - out->h;

    } else if (coordinates == FSEMU_COORD_1080P) {
        double scale_x = window_size.h / 1080.0;
        double scale_y = window_size.h / 1080.0;

        out->w = 2.0 * (in->w * scale_x) / window_size.w;
        out->h = 2.0 * (in->h * scale_y) / window_size.h;
        out->x = -1.0 + 2.0 * (in->x * scale_y) / window_size.w;
        out->y = 1.0 - 2.0 * (in->y * scale_y) / window_size.h - out->h;

    } else if (coordinates == FSEMU_COORD_1080P_LEFT) {
        double scale = window_size.h / 1080.0;

        out->w = 2.0 * (in->w * scale) / window_size.w;
        out->h = 2.0 * (in->h * scale) / window_size.h;
        out->x = -1.0 + 2.0 * (in->x * scale) / window_size.w;
        out->y = 1.0 - 2.0 * (in->y * scale) / window_size.h - out->h;

    } else if (coordinates == FSEMU_COORD_1080P_RIGHT) {
        double scale = window_size.h / 1080.0;

        out->w = 2.0 * (in->w * scale) / window_size.w;
        out->h = 2.0 * (in->h * scale) / window_size.h;
        out->x = -1.0 + 2.0 * (window_size.w - (1920 - in->x) * scale) /
                            window_size.w;
        out->y = 1.0 - 2.0 * (in->y * scale) / window_size.h - out->h;
    }
}

static void fsemu_glvideo_render_image(fsemu_gui_item_t *item)
{
    fsemu_drect_t dr;
    fsemu_glvideo_convert_coordinates(&dr, &item->rect, item->coordinates);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    fsemu_image_t *image = item->image;
    // image->depth,
    // image->stride,

    // FIXME:
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 image->width,
                 image->height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 image->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    float tx1 = 0.0f, tx2 = 1.0f;
    float ty1 = 1.0f, ty2 = 0.0f;
    if (item->flags & FSEMU_GUI_FLAG_FLIP_X) {
        tx1 = 1.0f;
        tx2 = 0.0f;
    }

    fsemu_opengl_blend(true);
    fsemu_opengl_color4f(1.0f, 1.0f, 1.0f, 1.0f);
    fsemu_opengl_depth_test(false);
    fsemu_opengl_texture_2d(true);

    glBegin(GL_QUADS);
    glTexCoord2f(tx1, ty1);
    glVertex3f(dr.x, dr.y, 0);
    glTexCoord2f(tx2, ty1);
    glVertex3f(dr.x + dr.w, dr.y, 0);
    glTexCoord2f(tx2, ty2);
    glVertex3f(dr.x + dr.w, dr.y + dr.h, 0);
    glTexCoord2f(tx1, ty2);
    glVertex3f(dr.x, dr.y + dr.h, 0);
    glEnd();

    // fsemu_video_log("Render image to %0.2f,%0.2f %0.2fx%0.2f\n",
    //                 dr.x, dr.y, dr.w, dr.h);
    glDeleteTextures(1, &texture);
}

static void fsemu_glvideo_render_rectangle(fsemu_gui_item_t *item)
{
    fsemu_drect_t dr;
    fsemu_glvideo_convert_coordinates(&dr, &item->rect, item->coordinates);

    uint8_t r = item->color & 0xff;
    uint8_t g = (item->color & 0xff00) >> 8;
    uint8_t b = (item->color & 0xff0000) >> 16;
    uint8_t a = (item->color & 0xff000000) >> 24;

    // printf("%0.2f %0.2f   %0.2fx%0.2f\n", dr.x, dr.y, dr.w, dr.h);

    fsemu_opengl_blend(a < 255);
    fsemu_opengl_depth_test(false);
    fsemu_opengl_texture_2d(false);

    fsemu_opengl_color4f(r / 255.0, g / 255.0, b / 255.0, a / 255.0);

    glBegin(GL_QUADS);
    glVertex3f(dr.x, dr.y, 0);
    glVertex3f(dr.x + dr.w, dr.y, 0);
    glVertex3f(dr.x + dr.w, dr.y + dr.h, 0);
    glVertex3f(dr.x, dr.y + dr.h, 0);
    glEnd();
}

static void fsemu_glvideo_render_item(fsemu_gui_item_t *item)
{
    if (item->visible) {
        if (item->image) {
            fsemu_glvideo_render_image(item);
        } else {
            fsemu_glvideo_render_rectangle(item);
        }
    }
}

void fsemu_glvideo_render_gui_early(fsemu_gui_item_t *items)
{
    // fsemu_video_log("fsemu_glvideo_render_gui_early\n");
    // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
#if 0
    fsemu_gui_item_t *item = items;
    while (item) {
        fsemu_glvideo_render_item(item);
        item = item->next;
    }
#endif
    // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void fsemu_glvideo_render_gui(fsemu_gui_item_t *items)
{
    // fsemu_video_log("fsemu_glvideo_render_gui\n");
#if 1
    fsemu_gui_item_t *item = items;
    while (item) {
        fsemu_glvideo_render_item(item);
        item = item->next;
    }
#endif
}
