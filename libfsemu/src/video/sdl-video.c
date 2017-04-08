#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define FSE_INTERNAL_API
#include <fs/emu/video.h>
#include <fs/emu/buffer.h>
#include <fs/log.h>

#include "SDL.h"

#define MAX_BUFFERS 3

static SDL_Window* g_window;
static SDL_Renderer* g_renderer;
static SDL_Texture *g_textures[MAX_BUFFERS];

typedef struct sdl_buffer {
    fs_emu_buffer buffer;
    int index;
    bool streaming;
} sdl_buffer;

static sdl_buffer g_buffers[MAX_BUFFERS];
static int g_buffer_width;
static int g_buffer_height;
static int g_buffer_bpp;

static void sdl_buffer_configure(int width, int height)
{
    fs_log("[VIDEO] sdl_buffer_configure width=%d height=%d\n", width, height);
    g_buffer_width = width;
    g_buffer_height = height;
    g_buffer_bpp = 4;

    int num_buffers = MAX_BUFFERS;
    uint32_t format = SDL_PIXELFORMAT_ARGB8888;
    // format = SDL_PIXELFORMAT_RGBA8888;
    int access = SDL_TEXTUREACCESS_STREAMING;
    for (int i = 0; i < num_buffers; i++) {
        fs_log("- creating buffer width=%d height=%d format=%x access=%x\n",
               width, height, format, access);
        g_textures[i] = SDL_CreateTexture(g_renderer, format, access,
                                          width, height);

        g_buffers[i].index = i;
        g_buffers[i].streaming = false;

        g_buffers[i].buffer.width = g_buffer_width;
        g_buffers[i].buffer.height = g_buffer_height;
        g_buffers[i].buffer.bpp = g_buffer_bpp;
        g_buffers[i].buffer.size = g_buffer_width * g_buffer_height * g_buffer_bpp;
        //memset(g_video_buffers[i].data, 0, g_video_buffers[i].size);
        g_buffers[i].buffer.aspect = 1.0;
        //g_video_buffers[i].buffer_width = width;
        //g_video_buffers[i].buffer_height = height;

        if (g_buffers[i].streaming) {
            SDL_LockTexture(g_textures[i], NULL, &(g_buffers[i].buffer.data),
                            &(g_buffers[i].buffer.stride));
            printf("  - stride is %d\n", g_buffers[i].buffer.stride);
        } else {
            g_buffers[i].buffer.data = g_malloc0(g_buffers[i].buffer.size);
            g_buffers[i].buffer.stride = g_buffer_width * g_buffer_bpp;
        }
    }
}

static fs_emu_buffer *sdl_buffer_get(void)
{
#if 0
    int i = 0;
    g_buffers[i].width = g_buffer_width;
    g_buffers[i].height = g_buffer_height;
    g_buffers[i].bpp = g_buffer_bpp;
    g_buffers[i].size = g_buffer_width * g_buffer_height * g_buffer_bpp;
    g_buffers[i].data = g_malloc0(g_buffers[i].size);
    //memset(g_video_buffers[i].data, 0, g_video_buffers[i].size);
    g_buffers[i].aspect = 1.0;
    //g_video_buffers[i].buffer_width = width;
    //g_video_buffers[i].buffer_height = height;
#endif
    int index = fs_emu_buffer_next();
    sdl_buffer *buffer = g_buffers + index;
#if 0
    // memset(buffer->data, 0x00, buffer->size);
    memset(buffer->buffer.data, 0xff, buffer->buffer.size);
#endif
    return (fs_emu_buffer *) buffer;
}

static void sdl_buffer_finish(fs_emu_buffer *buffer)
{
    sdl_buffer *b = (sdl_buffer *) buffer;
    // memset(buffer->data, 0xff, buffer->size);
    fs_emu_buffer_set_current(b->index);
}

static void sdl_video_create_window(int width, int height)
{
    fs_log("[VIDEO] sdl_video_create_window %d %d\n", width, height);

    int window_flags = 0;
    window_flags |= SDL_WINDOW_RESIZABLE;
    g_window = SDL_CreateWindow(
                "FS-UAE SDL Test Driver", SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED, width, height, window_flags);
#ifdef VIDEO_DRIVER_SDL_SOFTWARE
    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_SOFTWARE);
#else
    g_renderer = SDL_CreateRenderer(g_window, -1, 0);
#endif

    SDL_RendererInfo info;
    if (SDL_GetRendererInfo(g_renderer, &info) == 0) {
        fs_log("SDL renderer: %s\n", info.name);
        fs_log("SDL max texture width: %d\n", info.max_texture_width);
        fs_log("SDL max texture height: %d\n", info.max_texture_height);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(g_renderer, width, height);

    SDL_RenderClear(g_renderer);
    SDL_RenderPresent(g_renderer);
}

static void sdl_video_render(void)
{
    int index = fs_emu_buffer_lock();

    sdl_buffer *buffer = g_buffers + index;
    SDL_Texture *texture = g_textures[index];
    // printf("size %d %d\n", buffer->buffer.width, buffer->buffer.height);
    // printf("crop %d %d %d %d\n", buffer->buffer.crop.x, buffer->buffer.crop.y,
    //        buffer->buffer.crop.w, buffer->buffer.crop.h);
    // printf("updating texture (size %d) from buffer %d\n", buffer->size, index);
    SDL_Rect src_rect;
#if 1
    src_rect.x = buffer->buffer.crop.x;
    src_rect.y = buffer->buffer.crop.y;
    src_rect.w = buffer->buffer.crop.w;
    src_rect.h = buffer->buffer.crop.h;
#else
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = buffer->buffer.width;
    src_rect.h = buffer->buffer.height;
#endif
    SDL_Rect dst_rect;
    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.w = 752;
    dst_rect.h = 572;

    if (buffer->streaming) {
        SDL_UnlockTexture(texture);
    } else {
        SDL_UpdateTexture(texture, &src_rect,
                          buffer->buffer.data + buffer->buffer.crop.y * \
                          buffer->buffer.stride + buffer->buffer.crop.x * \
                          buffer->buffer.bpp, buffer->buffer.stride);
        fs_emu_buffer_unlock();
    }

    SDL_RenderClear(g_renderer);
    SDL_RenderCopy(g_renderer, texture, &src_rect, &dst_rect);
    SDL_RenderPresent(g_renderer);

    if (buffer->streaming) {
        SDL_LockTexture(texture, NULL, &(buffer->buffer.data),
                        &(buffer->buffer.stride));
        fs_emu_buffer_unlock();
    }
}

static void register_functions(void)
{
    fse_video.create_window = sdl_video_create_window;
    fse_video.render = sdl_video_render;
    fse_video.configure_buffer = sdl_buffer_configure;
    fse_video.get_buffer = sdl_buffer_get;
    fse_video.finish_buffer = sdl_buffer_finish;
}

#ifdef VIDEO_DRIVER_SDL_SOFTWARE
void fs_emu_video_sdl_software_init(void)
{
    fs_log("[VIDEO] Initialize SDL video (software renderer)\n");
#else
void fs_emu_video_sdl_init(void)
{
    fs_log("[VIDEO] Initialize SDL video\n");
#endif
    register_functions();
}
