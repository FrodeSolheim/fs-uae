#define FSEMU_INTERNAL
#include "fsemu-glvideo.h"

#include "fsemu-frame.h"
#include "fsemu-glib.h"
#include "fsemu-gui.h"
#include "fsemu-image.h"
#include "fsemu-layout.h"
#include "fsemu-opengl.h"
#include "fsemu-perfgui.h"
#include "fsemu-sdl.h"
#include "fsemu-sdlwindow.h"
#include "fsemu-time.h"
#include "fsemu-titlebar.h"
#include "fsemu-types.h"
#include "fsemu-util.h"
#include "fsemu-video.h"
#include "fsemu-window.h"

static struct {
    GLuint textures[2];
    int current_texture;
    fsemu_rect_t rect;
    // Drawable size is the real pixel size, which is not necessary the same
    // as screen coordinates with high-DPI windows on some systems.
    fsemu_size_t drawable_size;
} fsemu_glvideo;

void fsemu_glvideo_init(void)
{
    fsemu_return_if_already_initialized();
    // fsemu_opengl_init();
    fsemu_video_log("Initializing OpenGL video renderer\n");

#if 0
    SDL_Window *window = fsemu_sdlwindow_window();
    // renderer = SDL_CreateRenderer(window, -1, renderer_flags);
    // renderer = SDL_CreateRenderer(fsemu_sdlwindow_window(), -1, 0);
    fsemu_video_log("%p\n", window);
    // fsemu_video_log("%p\n", renderer);
#endif

    // fsemu_video_log("Setting clear color to #000000\n");

    int r, g, b;
    fsemu_video_background_color_rgb(&r, &g, &b);
    glClearColor(r / 255.0, g / 255.0, b / 255.0, 1.0);
#if 0
    glClearColor(1.0, 0.0, 0.0, 1.0);
    printf("%d\n", glGetError());
#endif

    glGenTextures(2, fsemu_glvideo.textures);
    printf("%d\n", glGetError());

    uint8_t *data = (uint8_t *) malloc(1024 * 1024 * 4);
#if 0
    memset(data, 0x00, 1024 * 1024 * 4);
#else
    // Setting color to 0xff here to debug "bleed issues" with linear filtering
    memset(data, 0xff, 1024 * 1024 * 4);
#if 0
    for (int i = 0; i < 1024 * 1024 * 4; i+=4) {
        data[i] = 0xff;
    }
#endif
#endif
    // FIXME:
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    for (int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, fsemu_glvideo.textures[i]);
        printf("%d\n", glGetError());
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     1024,
                     1024,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     data);
        printf("%d\n", glGetError());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    free(data);
}

void fsemu_glvideo_set_size_2(int width, int height)
{
    // This function will be called from the UI thread, which is not
    // necessarily the same as the video thread, so we register the new
    // glViewport dimensions here, but defer changing it so the video thread
    // can change it when starting on a new thread.

    // FIXME: Ideally use locking to ensure both dimensions are updated
    // atomatically, probably not that important though since it will be
    // correct one frame later in all cases.

    // FIXME: Make sure this is called initially on Window creation?

    fsemu_video_log("Got resize callback from window subsystem\n");
    int draw_w, draw_h;
    SDL_GL_GetDrawableSize(fsemu_sdlwindow_window(), &draw_w, &draw_h);
    fsemu_video_log("Window size %dx%d : drawable size %dx%d\n",
                    width,
                    height,
                    draw_w,
                    draw_h);
    fsemu_glvideo.drawable_size.w = draw_w;
    fsemu_glvideo.drawable_size.h = draw_h;
}

void fsemu_glvideo_work(int timeout_us)
{
    fsemu_video_frame_t *frame = fsemu_video_get_frame(timeout_us);
    if (!frame) {
        return;
    }
#if 1
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

    if (rect.w == 0 || rect.h == 0) {
        fsemu_video_log("Invalid rect\n");
        return;
    }

    // fsemu_opengl_texture_2d(true);
    glBindTexture(GL_TEXTURE_2D,
                  fsemu_glvideo.textures[fsemu_glvideo.current_texture]);
    // printf("glGetError %d\n", glGetError());
    // SDL_UpdateTexture(fsemu_glvideo.textures[fsemu_glvideo.current_texture],
    //                   &rect,
    //                   pixels,
    //                   frame->stride);

    // FIXME:
    // glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // FIXME: stride!!

    // for (int i = 0; i < rect.w * rect.h * 4; i++) {
    //     pixels[i] = i % 256;
    // }
    // pixels = (void *) 0xffff0000;

    // FIXME: Replace constants
    int tw = 1024;
    int th = 1024;
    // fsemu_video_fix_right_edge(pixels, rect.y, rect.w, rect.h, 1024, 1024);

    // FIXME: Internal format vs format vs type, make sure to use efficient
    // combos!
    GLenum format = GL_BGRA;
    GLenum type = GL_UNSIGNED_BYTE;

    glTexSubImage2D(GL_TEXTURE_2D,
                    0,
                    rect.x,
                    rect.y,
                    rect.w,
                    rect.h,
                    format,
                    type,
                    pixels);

    // Duplicate right (and later, bottom) edge to remove bleed effect from
    // unused pixels in the texture when doing bilinear filtering.
    if (rect.w < tw) {
        // FIXME: Wrapper call via fsemu-opengl ?
        glPixelStorei(GL_UNPACK_ROW_LENGTH, rect.w);
        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        rect.w,
                        rect.y,
                        1,
                        rect.h,
                        format,
                        type,
                        pixels + (rect.w * 4 * rect.y) + (rect.w - 1) * 4);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    }

    // printf("glGetError %d\n", glGetError());

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
        /*
        SDL_UpdateTexture(
            fsemu_sdlvideo.textures[fsemu_sdlvideo.current_texture],
            &rect,
            greenline,
            2048 * 4);
        */
        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        rect.x,
                        rect.y,
                        rect.w,
                        rect.h,
                        GL_RGBA,
                        GL_UNSIGNED_BYTE,
                        greenline);
    }
#endif

    if (frame->partial > 0 && frame->partial != frame->height) {
        return;
    }

    // Only draw bottom border duplicate line for last slice, and only if
    // there is space for it in the texture
    if (rect.h < th) {
        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        0,
                        rect.h,
                        rect.w,
                        1,
                        format,
                        type,
                        pixels + (rect.w * 4 * (rect.h - 1)));
        // Draw corner pixel if room for it
        if (rect.w < tw) {
            glTexSubImage2D(GL_TEXTURE_2D,
                            0,
                            rect.w,
                            rect.h,
                            1,
                            1,
                            format,
                            type,
                            pixels + (rect.w * rect.h - 1) * 4);
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        }
    }

    fsemu_video_log(" draw ___________ READY______________ \n");

    // FIXME: Locked access for main/video thread separation?
    // fsemu_sdlvideo.rect.x = 0;
    // fsemu_sdlvideo.rect.y = 0;
    fsemu_glvideo.rect.w = frame->width;
    fsemu_glvideo.rect.h = frame->height;

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

#endif
}

static void fsemu_glvideo_convert_coordinates(fsemu_drect_t *out,
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
    double yoff = 2.0 * titlebar / window_size.h;

    if (coordinates == FSEMU_COORD_REAL) {
        out->w = 2.0 * in->w / window_size.w;
        out->h = 2.0 * in->h / window_size.h;
        out->x = -1.0 + 2.0 * in->x / window_size.w;
        out->y = 1.0 - 2.0 * in->y / window_size.h - out->h;

    } else if (coordinates == FSEMU_COORD_1080P) {
        double scale_x = client_size.w / 1920.0;
        double scale_y = client_size.h / 1080.0;

        out->w = 2.0 * (in->w * scale_x) / window_size.w;
        out->h = 2.0 * (in->h * scale_y) / window_size.h;
        out->x = -1.0 + 2.0 * (in->x * scale_y) / window_size.w;
        out->y = 1.0 - 2.0 * (in->y * scale_y) / window_size.h - out->h - yoff;

    } else if (coordinates == FSEMU_COORD_1080P_LEFT) {
        double scale = client_size.h / 1080.0;

        out->w = 2.0 * (in->w * scale) / window_size.w;
        out->h = 2.0 * (in->h * scale) / window_size.h;
        out->x = -1.0 + 2.0 * (in->x * scale) / window_size.w;
        out->y = 1.0 - 2.0 * (in->y * scale) / window_size.h - out->h - yoff;

    } else if (coordinates == FSEMU_COORD_1080P_RIGHT) {
        double scale = client_size.h / 1080.0;

        out->w = 2.0 * (in->w * scale) / window_size.w;
        out->h = 2.0 * (in->h * scale) / window_size.h;
        out->x = -1.0 + 2.0 * (window_size.w - (1920 - in->x) * scale) /
                            window_size.w;
        out->y = 1.0 - 2.0 * (in->y * scale) / window_size.h - out->h - yoff;
    }
}

void fsemu_glvideo_render(void)
{
    fsemu_video_log("--- render --- [draw]\n");

    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = fsemu_glvideo.rect.w;
    src.h = fsemu_glvideo.rect.h;
    // src.w = 692;
    // src.h = 540;

#if 0
    // To debug the duplicate right & bottom edges
    src.w += 1;
    src.h += 1;
#endif

    fsemu_layout_set_video_size(src.w, src.h);
    fsemu_layout_set_pixel_aspect(((double) src.w / src.h) / (4.0 / 3.0));

    SDL_Rect dst;
    fsemu_layout_video_rect(&dst);

    // dst.x = 0;
    // dst.y = 0;
    // dst.w = 800;
    // dst.h = 500;

    fsemu_drect_t dr;
    fsemu_glvideo_convert_coordinates(&dr, &dst, FSEMU_COORD_REAL);

    // printf("%d %d %d %d -> %0.2f %0.2f %0.2f %0.2f\n",
    //        dst.x,
    //        dst.y,
    //        dst.w,
    //        dst.h,
    //        dr.x,
    //        dr.y,
    //        dr.w,
    //        dr.h);

    // SDL_RenderCopy(fsemu_sdlvideo.renderer,
    //                fsemu_sdlvideo.textures[fsemu_sdlvideo.current_texture],
    //                &src,
    //                &dst);
    // SDL_RenderDrawRect(fsemu_sdlvideo.renderer, &dst);

    // FIXME: Remove 1024 constants
    float tx1 = 0.0f, tx2 = src.w / 1024.0;
    float ty1 = src.h / 1024.0, ty2 = 0.0f;

    fsemu_opengl_blend(false);
    fsemu_opengl_color4f(1.0f, 1.0f, 1.0f, 1.0f);
    fsemu_opengl_depth_test(false);
    fsemu_opengl_texture_2d(true);

    glBindTexture(GL_TEXTURE_2D,
                  fsemu_glvideo.textures[fsemu_glvideo.current_texture]);

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

    // FIXME:...
    static int framecount;
    fsemu_video_set_frame_rendered_at(framecount, fsemu_time_us());
    framecount += 1;
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
    // printf("swap\n");
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

#if 1
    // On my Linux laptop (Intel graphics) at least, it is not (always)
    // sufficient to run glViewport in response to resize event. When
    // opening in fullscreen window, the viewport becomes slightly smaller
    // than fullscreen, even though the last glViewport in response the the
    // last resize event used correct coordinates.
    // Workaround here is to run glViewport before every frame...

    // fsemu_size_t window_size;
    // fsemu_window_size(&window_size);
    // printf("glViewport 0, 0, %d, %d\n", window_size.w, window_size.h);
    // fflush(stdout);
    // int flags = SDL_GetWindowFlags(fsemu_sdlwindow_window());
    // printf("glViewport? flags %d\n", flags & SDL_WINDOW_ALLOW_HIGHDPI);
    // glViewport(0, 0, window_size.w, window_size.h);

    if (fsemu_glvideo.drawable_size.w > 0 &&
        fsemu_glvideo.drawable_size.h > 0) {
        glViewport(0,
                   0,
                   fsemu_glvideo.drawable_size.w,
                   fsemu_glvideo.drawable_size.h);
    } else {
        // FIXME: log_warning
        fsemu_video_log("Not a valid drawable size for glViewport\n");
    }

#endif

    // glClearColor(0.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
#ifdef FSEMU_WINDOWS_XXX
    fsemu_opengl_blend(false);
    fsemu_opengl_texture_2d(false);
    glBegin(GL_QUADS);
    fsemu_opengl_color3f(0.0, 1.0, 0.0);
    glVertex2f(-1.0, -1.0);
    glVertex2f(1.0, -1.0);
    glVertex2f(1.0, 1.0);
    glVertex2f(-1.0, 1.0);
    glEnd();
#endif
}

static void fsemu_glvideo_render_image(fsemu_gui_item_t *item)
{
    fsemu_drect_t dr;
    fsemu_glvideo_convert_coordinates(&dr, &item->rect, item->coordinates);

    uint8_t r = item->color & 0xff;
    uint8_t g = (item->color & 0xff00) >> 8;
    uint8_t b = (item->color & 0xff0000) >> 16;
    uint8_t a = (item->color & 0xff000000) >> 24;

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
    // fsemu_opengl_color4f(1.0f, 1.0f, 1.0f, 1.0f);
    fsemu_opengl_color4f(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
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
#if 1
    fsemu_gui_item_t *item = items;
    while (item) {
        if (item->z_index < 0) {
            fsemu_glvideo_render_item(item);
        }
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
        if (item->z_index >= 0) {
            fsemu_glvideo_render_item(item);
        }
        item = item->next;
    }
#endif
}
