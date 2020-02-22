#define FSEMU_INTERNAL
#include "fsemu-sdlvideo.h"

#ifdef FSEMU_SDL

#include "fsemu-frame.h"
#include "fsemu-glib.h"
#include "fsemu-gui.h"
#include "fsemu-image.h"
#include "fsemu-layout.h"
#include "fsemu-perfgui.h"
#include "fsemu-sdl.h"
#include "fsemu-sdlwindow.h"
#include "fsemu-time.h"
#include "fsemu-titlebar.h"
#include "fsemu-types.h"
#include "fsemu-util.h"
#include "fsemu-video.h"
#include "fsemu-widget.h"
#include "fsemu-window.h"

#define fsemu_sdlvideo_MAX_FRAME_STATS (1 << 8)  // 256

static struct {
    SDL_Renderer *renderer;
    SDL_Texture *textures[2];
    int current_texture;
    fsemu_rect_t rect;
    fsemu_rect_t limits_rect;
} fsemu_sdlvideo;

void fsemu_sdlvideo_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_video_log("Initializing SDL video renderer\n");

    int renderer_flags = SDL_RENDERER_ACCELERATED;
    // renderer_flags |= SDL_RENDERER_PRESENTVSYNC;

    SDL_Window *window = fsemu_sdlwindow_window();
    fsemu_sdlvideo.renderer = SDL_CreateRenderer(window, -1, renderer_flags);
    // renderer = SDL_CreateRenderer(fsemu_sdlwindow_window(), -1, 0);
    fsemu_video_log("%p\n", window);
    fsemu_video_log("%p\n", fsemu_sdlvideo.renderer);

    int r, g, b;
    fsemu_video_background_color_rgb(&r, &g, &b);
    SDL_SetRenderDrawColor(fsemu_sdlvideo.renderer, r, g, b, 0xff);
    SDL_RenderClear(fsemu_sdlvideo.renderer);

    SDL_RenderPresent(fsemu_sdlvideo.renderer);

    SDL_SetRenderDrawColor(fsemu_sdlvideo.renderer, r, g, b, 0xff);
    SDL_RenderClear(fsemu_sdlvideo.renderer);
#if 0
    SDL_Event event;
    while (SDL_PollEvent(&event)) {

    }
#endif

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    fsemu_sdlvideo.textures[0] = SDL_CreateTexture(fsemu_sdlvideo.renderer,
                                                   SDL_PIXELFORMAT_BGRA32,
                                                   SDL_TEXTUREACCESS_STREAMING,
                                                   1024,
                                                   1024);
    fsemu_sdlvideo.textures[1] = SDL_CreateTexture(fsemu_sdlvideo.renderer,
                                                   SDL_PIXELFORMAT_BGRA32,
                                                   SDL_TEXTUREACCESS_STREAMING,
                                                   1024,
                                                   1024);
}

void fsemu_sdlvideo_work(int timeout_us)
{
    fsemu_video_frame_t *frame = fsemu_video_get_frame(timeout_us);
    if (!frame) {
        return;
    }
    fsemu_video_log(" ---------------- draw got frame! (%dx%d) partial? %d\n",
                    frame->width,
                    frame->height,
                    frame->partial);
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = frame->width;
    rect.h = frame->height;
    uint8_t *pixels = (uint8_t *) frame->buffer;

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

    SDL_UpdateTexture(fsemu_sdlvideo.textures[fsemu_sdlvideo.current_texture],
                      &rect,
                      pixels,
                      frame->stride);

#if 1
    static uint8_t *greenline;
    if (greenline == NULL) {
        greenline = (uint8_t *) malloc(2048 * 4);
        for (int i = 0; i < 2048; i++) {
            ((int32_t *) greenline)[i] = FSEMU_RGB(0x00ff00);
        }
    }
    if (fsemu_perfgui_mode() == 2) {
        rect.h = 1;
        SDL_UpdateTexture(
            fsemu_sdlvideo.textures[fsemu_sdlvideo.current_texture],
            &rect,
            greenline,
            2048 * 4);
    }
#endif

    if (frame->partial > 0 && frame->partial != frame->height) {
        return;
    }

    fsemu_sdlvideo.limits_rect = frame->limits;

    fsemu_video_log(" draw ___________ READY______________ \n");

    // FIXME: Locked access for main/video thread separation?
    // fsemu_sdlvideo.rect.x = 0;
    // fsemu_sdlvideo.rect.y = 0;
    fsemu_sdlvideo.rect.w = frame->width;
    fsemu_sdlvideo.rect.h = frame->height;

    fsemu_video_set_ready(true);

    static int64_t last;
    int64_t now = fsemu_time_us();
    fsemu_video_log("_______draw _______diff __________ %lld\n",
                    lld(now - last));
    last = now;

    static int framecount;
    framecount += 1;

    static int64_t framecount_time = 0;
    if (framecount_time == 0) {
        framecount = 0;
        framecount_time = fsemu_time_us();
    }
    if (now - framecount_time > 1000000) {
        // double fps = framecount * 1000000.0 / (now - framecount_time);
        framecount = 0;
        framecount_time = now;
        // g_printerr("FPS: %0.2f\n", fps);
    }

    // FIXME: Free frame if it is a partial frame, otherwise save for render...
}

void fsemu_sdlvideo_render(void)
{
    fsemu_video_log("--- render --- [draw]\n");
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = fsemu_sdlvideo.rect.w;
    src.h = fsemu_sdlvideo.rect.h;
    // src.w = 692;
    // src.h = 540;

    src.x = fsemu_sdlvideo.limits_rect.x;
    src.y = fsemu_sdlvideo.limits_rect.y;
    src.w = fsemu_sdlvideo.limits_rect.w;
    src.h = fsemu_sdlvideo.limits_rect.h;

    fsemu_layout_set_video_size(src.w, src.h);
    fsemu_layout_set_pixel_aspect(((double) src.w / src.h) / (4.0 / 3.0));

    SDL_Rect dst;
    fsemu_layout_video_rect(&dst);
    SDL_RenderCopy(fsemu_sdlvideo.renderer,
                   fsemu_sdlvideo.textures[fsemu_sdlvideo.current_texture],
                   &src,
                   &dst);
    // SDL_RenderDrawRect(fsemu_sdlvideo.renderer, &dst);

    // FIXME:...
    static int framecount;
    fsemu_video_set_frame_rendered_at(framecount, fsemu_time_us());
    framecount += 1;
}

void fsemu_sdlvideo_display(void)
{
    fsemu_video_log("--- display --- [draw]\n");
    // SDL_Window *window = fsemu_sdlwindow_window();
    // SDL_GL_SwapWindow(window);
    // return;

    SDL_RenderPresent(fsemu_sdlvideo.renderer);

    int r, g, b;
    fsemu_video_background_color_rgb(&r, &g, &b);
    SDL_SetRenderDrawColor(fsemu_sdlvideo.renderer, r, g, b, 0xff);
    SDL_RenderClear(fsemu_sdlvideo.renderer);
}

static void fsemu_sdlvideo_convert_coordinates(SDL_Rect *out,
                                               fsemu_rect_t *in,
                                               int coordinates)
{
    // FIXME: Use copy of window size? Layout?
    fsemu_size_t window_size;
    fsemu_window_size(&window_size);

    int titlebar = fsemu_titlebar_static_height();
    fsemu_size_t client_size;
    client_size.w = window_size.w;
    client_size.h = window_size.h - titlebar;
    double yoff = titlebar;
    // printf("%d %f\n", titlebar, yoff);

    if (coordinates == FSEMU_COORD_REAL) {
        out->w = in->w;
        out->h = in->h;
        out->x = in->x;
        out->y = in->y;
    } else if (coordinates == FSEMU_COORD_1080P) {
        double scale_x = client_size.w / 1920.0;
        double scale_y = client_size.h / 1080.0;

        out->w = in->w * scale_x;
        out->h = in->h * scale_y;
        out->x = in->x * scale_x;
        out->y = in->y * scale_y + yoff;

    } else if (coordinates == FSEMU_COORD_1080P_LEFT) {
        double scale = client_size.h / 1080.0;

        out->w = in->w * scale;
        out->h = in->h * scale;
        out->x = in->x * scale;
        out->y = in->y * scale + yoff;

    } else if (coordinates == FSEMU_COORD_1080P_RIGHT) {
        double scale = client_size.h / 1080.0;

        out->w = in->w * scale;
        out->h = in->h * scale;
        out->x = window_size.w - (1920 - in->x) * scale;
        out->y = in->y * scale + yoff;
    }
}

static void fsemu_sdlvideo_render_image(fsemu_gui_item_t *item)
{
    SDL_Rect dr;
    fsemu_sdlvideo_convert_coordinates(&dr, &item->rect, item->coordinates);

    fsemu_image_t *image = item->image;
    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(image->data,
                                                    image->width,
                                                    image->height,
                                                    image->depth,
                                                    image->stride,
                                                    0x000000ff,
                                                    0x0000ff00,
                                                    0x00ff0000,
                                                    0xff000000);
    if (surface == NULL) {
        fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Texture *texture =
        SDL_CreateTextureFromSurface(fsemu_sdlvideo.renderer, surface);
    if (texture == NULL) {
        fprintf(
            stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_FreeSurface(surface);

    SDL_Rect src_rect;
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = image->width;
    src_rect.h = image->height;

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (item->flags & FSEMU_WIDGET_FLAG_FLIPX) {
        flip = (SDL_RendererFlip)(flip | SDL_FLIP_HORIZONTAL);
    }

    // SDL_RenderCopy(fsemu_sdlvideo.renderer, texture, &src_rect, &dr);
    SDL_RenderCopyEx(
        fsemu_sdlvideo.renderer, texture, &src_rect, &dr, 0, NULL, flip);

    fsemu_video_log("Render image to %d,%d %dx%d\n", dr.x, dr.y, dr.w, dr.h);
    SDL_DestroyTexture(texture);
}

static void fsemu_sdlvideo_render_rectangle(fsemu_gui_item_t *item)
{
    SDL_Rect dr;
    fsemu_sdlvideo_convert_coordinates(&dr, &item->rect, item->coordinates);

    // uint8_t b = item->color & 0xff;
    // uint8_t g = (item->color & 0xff00) >> 8;
    // uint8_t r = (item->color & 0xff0000) >> 16;
    // uint8_t a = 255 - ((item->color & 0xff000000) >> 24);

    uint8_t r = item->color & 0xff;
    uint8_t g = (item->color & 0xff00) >> 8;
    uint8_t b = (item->color & 0xff0000) >> 16;
    uint8_t a = (item->color & 0xff000000) >> 24;

    SDL_SetRenderDrawColor(fsemu_sdlvideo.renderer, r, g, b, a);
    SDL_RenderFillRect(fsemu_sdlvideo.renderer, &dr);
    fsemu_video_log("Render rect to %d,%d %dx%d\n", dr.x, dr.y, dr.w, dr.h);
}

static void fsemu_sdlvideo_render_item(fsemu_gui_item_t *item)
{
    if (item->visible) {
        if (item->image) {
            // item->color = 0xff0000ff;
            // fsemu_sdlvideo_render_rectangle(item);
            fsemu_sdlvideo_render_image(item);
        } else {
            fsemu_sdlvideo_render_rectangle(item);
        }
    }
}

void fsemu_sdlvideo_render_gui_early(fsemu_gui_item_t *items)
{
    fsemu_video_log("fsemu_sdlvideo_render_gui_early\n");
#if 1
    SDL_SetRenderDrawBlendMode(fsemu_sdlvideo.renderer, SDL_BLENDMODE_BLEND);
    fsemu_gui_item_t *item = items;
    while (item) {
        if (item->z_index < 0) {
            fsemu_sdlvideo_render_item(item);
        }
        item = item->next;
    }
    SDL_SetRenderDrawBlendMode(fsemu_sdlvideo.renderer, SDL_BLENDMODE_NONE);
#endif
}

void fsemu_sdlvideo_render_gui(fsemu_gui_item_t *items)
{
    fsemu_video_log("fsemu_sdlvideo_render_gui\n");
#if 1
    SDL_SetRenderDrawBlendMode(fsemu_sdlvideo.renderer, SDL_BLENDMODE_BLEND);
    fsemu_gui_item_t *item = items;
    while (item) {
        if (item->z_index >= 0) {
            fsemu_sdlvideo_render_item(item);
        }
        item = item->next;
    }
    SDL_SetRenderDrawBlendMode(fsemu_sdlvideo.renderer, SDL_BLENDMODE_NONE);
#endif
}

#endif  // FSEMU_SDL
