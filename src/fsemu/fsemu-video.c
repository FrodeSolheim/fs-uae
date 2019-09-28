#define FSEMU_INTERNAL
#include "fsemu/fsemu-video.h"

#include "fsemu/fsemu-frame.h"
#include "fsemu/fsemu-gui.h"
#include "fsemu/fsemu-image.h"
#include "fsemu/fsemu-layout.h"
#include "fsemu/fsemu-sdlwindow.h"
#include "fsemu/fsemu-time.h"
#include "fsemu/fsemu-types.h"
#include "fsemu/fsemu-util.h"
#include "fsemu/fsemu-window.h"

#include <glib.h>

#define FSEMU_VIDEO_MAX_FRAME_STATS (1 << 8)  // 256

static struct {
    fsemu_video_frame_stats_t stats[FSEMU_VIDEO_MAX_FRAME_STATS];
    fsemu_rect_t rect;
    int ready;
    int emu_us_avg;
} fsemu_video;

static GAsyncQueue *fsemu_video_frame_queue;
static SDL_Renderer *renderer;
static SDL_Texture *textures[2];
static int current_texture;

void fsemu_video_init(int flags)
{
    fsemu_return_if_already_initialized();

    fsemu_frame_init();

    fsemu_video_frame_queue = g_async_queue_new();

    int renderer_flags = SDL_RENDERER_ACCELERATED;
    // renderer_flags |= SDL_RENDERER_PRESENTVSYNC;

    SDL_Window *window = fsemu_sdlwindow_window();
    renderer = SDL_CreateRenderer(window, -1, renderer_flags);
    // renderer = SDL_CreateRenderer(fsemu_sdlwindow_window(), -1, 0);
    fsemu_video_log("%p\n", window);
    fsemu_video_log("%p\n", renderer);

    // SDL_SetRenderDrawColor(renderer, 0x10, 0x10, 0x10, 0xff);
    // SDL_SetRenderDrawColor(renderer, 0x0a, 0x0a, 0x0a, 0xff);

    SDL_SetRenderDrawColor(renderer, 0x0c, 0x0c, 0x0c, 0xff);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);

    SDL_SetRenderDrawColor(renderer, 0x0c, 0x0c, 0x0c, 0xff);
    SDL_RenderClear(renderer);
#if 0
    SDL_Event event;
    while (SDL_PollEvent(&event)) {

    }
#endif

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
}

void fsemu_video_work(int timeout_us)
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
    // fsemu_video.rect.x = 0;
    // fsemu_video.rect.y = 0;
    fsemu_video.rect.w = frame->width;
    fsemu_video.rect.h = frame->height;

    fsemu_video.ready = true;

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
}

bool fsemu_video_ready(void)
{
    return fsemu_video.ready;
}

void fsemu_video_render(void)
{
    fsemu_video_log("--- render --- [draw]\n");
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = fsemu_video.rect.w;
    src.h = fsemu_video.rect.h;
    // src.w = 692;
    // src.h = 540;

    fsemu_layout_set_video_size(src.w, src.h);
    fsemu_layout_set_pixel_aspect(((double) src.w / src.h) / (4.0 / 3.0));

    SDL_Rect dst;
    fsemu_layout_video_rect(&dst);
    SDL_RenderCopy(renderer, textures[current_texture], &src, &dst);
    fsemu_video.ready = false;
    // SDL_RenderDrawRect(renderer, &dst);
}

void fsemu_video_display(void)
{
    fsemu_video_log("--- display --- [draw]\n");
    // SDL_RenderClear(renderer);

    // SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0x00);

    SDL_RenderPresent(renderer);

    SDL_SetRenderDrawColor(renderer, 0x0c, 0x0c, 0x0c, 0xff);
    SDL_RenderClear(renderer);

    // SDL_Delay(1000);
}

void fsemu_video_post_frame(fsemu_video_frame_t *frame)
{
    g_async_queue_push(fsemu_video_frame_queue, frame);
}

fsemu_video_frame_t *fsemu_video_get_frame(int timeout_us)
{
    if (timeout_us == 0) {
        // Probably not necessary, but did not test timeout 0
        return (fsemu_video_frame_t *) g_async_queue_try_pop(
            fsemu_video_frame_queue);
    }
    return (fsemu_video_frame_t *) g_async_queue_timeout_pop(
        fsemu_video_frame_queue, timeout_us);
}

// FIXME: COPY WINDOW / LAYOUT SIZE on main <-> video sync ??

static void fsemu_video_convert_coordinates(SDL_Rect *out,
                                            fsemu_rect *in,
                                            int coordinates)
{
    // FIXME: Use copy of window size? Layout?
    fsemu_size_t window_size;
    fsemu_window_size(&window_size);

    if (coordinates == FSEMU_COORD_REAL) {
        out->w = in->w;
        out->h = in->h;
        out->x = in->x;
        out->y = in->y;
    } else if (coordinates == FSEMU_COORD_1080P_LEFT) {
        double scale = window_size.h / 1080.0;

        out->w = in->w * scale;
        out->h = in->h * scale;
        out->x = in->x * scale;
        out->y = in->y * scale;

    } else if (coordinates == FSEMU_COORD_1080P_RIGHT) {
        double scale = window_size.h / 1080.0;

        out->w = in->w * scale;
        out->h = in->h * scale;
        out->x = window_size.w - (1920 - in->x) * scale;
        out->y = in->y * scale;
    }
}

static void fsemu_video_render_image(fsemu_gui_item_t *item)
{
    SDL_Rect dst_rect;
    fsemu_video_convert_coordinates(&dst_rect, &item->rect, item->coordinates);

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
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect src_rect;
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = image->width;
    src_rect.h = image->height;

    int flip = 0;
    if (item->flags & FSEMU_GUI_FLAG_FLIP_X) {
        flip |= SDL_FLIP_HORIZONTAL;
    }

    // SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);
    SDL_RenderCopyEx(renderer, texture, &src_rect, &dst_rect, 0, NULL, flip);

    fsemu_video_log("Rendering image to %d,%d %dx%d\n",
                    dst_rect.x,
                    dst_rect.y,
                    dst_rect.w,
                    dst_rect.h);
    SDL_DestroyTexture(texture);
}

static void fsemu_video_render_rectangle(fsemu_gui_item_t *item)
{
    SDL_Rect dst_rect = item->rect;

    // uint8_t b = item->color & 0xff;
    // uint8_t g = (item->color & 0xff00) >> 8;
    // uint8_t r = (item->color & 0xff0000) >> 16;
    // uint8_t a = 255 - ((item->color & 0xff000000) >> 24);

    uint8_t r = item->color & 0xff;
    uint8_t g = (item->color & 0xff00) >> 8;
    uint8_t b = (item->color & 0xff0000) >> 16;
    uint8_t a = (item->color & 0xff000000) >> 24;

    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderFillRect(renderer, &dst_rect);
}

static void fsemu_video_render_item(fsemu_gui_item_t *item)
{
    if (item->visible) {
        if (item->image) {
            fsemu_video_render_image(item);
        } else {
            fsemu_video_render_rectangle(item);
        }
    }
}

void fsemu_video_render_gui_early(fsemu_gui_item_t *items)
{
    fsemu_video_log("render_gui_early\n");
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    fsemu_gui_item_t *item = items;
    while (item) {
        fsemu_video_render_item(item);
        item = item->next;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void fsemu_video_render_gui(fsemu_gui_item_t *items)
{
    fsemu_video_log("render_gui\n");
#if 0
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    fsemu_gui_item_t *item = items;
    while (item) {
        fsemu_video_render_item(item);
        item = item->next;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
#endif
}

static void fsemu_video_update_stats(void)
{
    static int64_t last;
    int64_t now = fsemu_time_us();
#if 0
    if (last == 0) {  // if (fsemu_frame_emu_duration > 100000) {
        fsemu_video_log("FIXME: first timing is off...\n");
        last = now;
        return;
    }
#endif

    fsemu_video_frame_stats_t *stats =
        &fsemu_video
             .stats[fsemu_frame_counter_mod(FSEMU_VIDEO_MAX_FRAME_STATS)];
    stats->wait_us = fsemu_frame_wait_duration;
    stats->emu_us = fsemu_frame_emu_duration;
    stats->sleep_us = fsemu_frame_sleep_duration;
    stats->extra_us = fsemu_frame_extra_duration;
    if (last != 0) {
        stats->other_us = (now - last) - stats->wait_us - stats->emu_us -
                          stats->sleep_us - stats->extra_us;
    }

    static fsemu_mavgi_t emu_us_mavgi;
    static int emu_us_mavgi_values[FSEMU_VIDEO_MAX_FRAME_STATS];
    // stats->emu_us_mavg = fsemu_mavgi(
    //     &emu_us_mavgi, emu_us_mavgi_values, 60, stats->emu_us);
    fsemu_video.emu_us_avg = fsemu_mavgi(&emu_us_mavgi,
                                         emu_us_mavgi_values,
                                         FSEMU_VIDEO_MAX_FRAME_STATS,
                                         stats->emu_us);

    last = now;
}

// FIXME: Should probably be fsemu_frame_emutime_avg_us
// FIXME: move
int fsemu_frame_emutime_avg_us(void)
{
    return fsemu_video.emu_us_avg;
}

void fsemu_video_end_frame(void)
{
    fsemu_video_update_stats();
}

void fsemu_video_frame_stats(int frame, fsemu_video_frame_stats_t *stats)
{
    memcpy(stats,
           &fsemu_video.stats[frame % FSEMU_VIDEO_MAX_FRAME_STATS],
           sizeof(fsemu_video_frame_stats_t));
}
