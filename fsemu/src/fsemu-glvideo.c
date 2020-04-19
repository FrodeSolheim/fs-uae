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
#include "fsemu-shader.h"
#include "fsemu-time.h"
#include "fsemu-titlebar.h"
#include "fsemu-types.h"
#include "fsemu-util.h"
#include "fsemu-video.h"
#include "fsemu-videothread.h"
#include "fsemu-widget.h"
#include "fsemu-window.h"

// ----------------------------------------------------------------------------

#define FSEMU_OPENGL_SHOW_TEXT_DEBUG_RECTANGLE 0

// ----------------------------------------------------------------------------

static struct {
    GLuint textures[2];
    int current_texture;
    fsemu_rect_t rect;
    fsemu_rect_t limits_rect;
    // Drawable size is the real pixel size, which is not necessary the same
    // as screen coordinates with high-DPI windows on some systems.
    fsemu_size_t drawable_size;
    // Number of frames rendered by the OpenGL video renderer.
    int frame_count;
} fsemu_glvideo;

void fsemu_glvideo_init(void)
{
    fsemu_return_if_already_initialized();
    // fsemu_opengl_init();
    fsemu_video_log("Initializing OpenGL video renderer\n");
    fsemu_shader_module_init();

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
    fsemu_opengl_log_error_maybe();

#if 0
    glClearColor(1.0, 0.0, 0.0, 1.0);
    fsemu_opengl_log_error_maybe();
#endif

    glGenTextures(2, fsemu_glvideo.textures);
    printf("fsemu_glvideo.textures[0] = %d\n", fsemu_glvideo.textures[0]);
    printf("fsemu_glvideo.textures[1] = %d\n", fsemu_glvideo.textures[1]);
    fsemu_opengl_log_error_maybe();

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
    // FIXME: Shouldn't be necessary.
    // FIXME: Make sure to synch opengl state instead?
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    fsemu_opengl_log_error_maybe();

    for (int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, fsemu_glvideo.textures[i]);
        fsemu_opengl_log_error_maybe();
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     1024,
                     1024,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     data);
        fsemu_opengl_log_error_maybe();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        fsemu_opengl_log_error_maybe();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        fsemu_opengl_log_error_maybe();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        fsemu_opengl_log_error_maybe();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        fsemu_opengl_log_error_maybe();
    }
    free(data);
}

// FIXME: Maybe rename to fsemu_glvideo_on_resize or something
void fsemu_glvideo_set_size_2(int width, int height)
{
    // This function will be called from the UI thread, which is not
    // necessarily the same as the video thread, so we register the new
    // glViewport dimensions here, but defer changing it so the video thread
    // can change it when starting on a new thread.

    // FIXME: Ideally use locking to ensure both dimensions are updated
    // atomatically, probably not that important though since it will be
    // correct one frame later in all cases.
    // Updated: Video renderer now uses a copy of drawable size, atomically
    // copied.

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

    fsemu_video_set_drawable_size(&fsemu_glvideo.drawable_size);
}

static void fsemu_glvideo_handle_frame(fsemu_video_frame_t *frame)
{
    fsemu_video_log(" ---------------- draw got frame! (%dx%d) partial? %d\n",
                    frame->width,
                    frame->height,
                    frame->partial);
#if 0
    printf(
        " ---------------- draw got frame! (%dx%d) partial? %d - %d %d %d "
        "%d\n",
        frame->width,
        frame->height,
        frame->partial,
        frame->limits.x,
        frame->limits.y,
        frame->limits.w,
        frame->limits.h

    );
#endif
    // FIXME: 4
    if (frame->partial) {
        // Need to check frame->height before modifying it
        if (frame->partial == frame->height) {
            frame->partial = frame->limits.h;
        } else {
            frame->partial -= frame->limits.y;
            if (frame->partial < 0) {
                return;
            }
        }
    }
    frame->buffer += (frame->limits.y * frame->stride) + frame->limits.x * 4;
    frame->width = frame->limits.w;
    frame->height = frame->limits.h;

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
    fsemu_opengl_log_error_maybe();

    // SDL_UpdateTexture(fsemu_glvideo.textures[fsemu_glvideo.current_texture],
    //                   &rect,
    //                   pixels,
    //                   frame->stride);

    int row_length = frame->stride / 4;  // FIXME: 4 -> bpp

    // fsemu_opengl_unpack_row_length(frame->width == row_length ? 0 :
    // row_length); fsemu_opengl_unpack_row_length(0);
    fsemu_opengl_unpack_row_length(row_length);

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

    // printf("b x=%d y=%d size=%dx%d\n", rect.x, rect.y, rect.w, rect.h);

    glTexSubImage2D(GL_TEXTURE_2D,
                    0,
                    rect.x,
                    rect.y,
                    rect.w,
                    rect.h,
                    format,
                    type,
                    pixels);
    // FIXME: fsemu_opengl_log_error_maybe();
    fsemu_opengl_log_error_maybe();

    // Duplicate right (and later, bottom) edge to remove bleed effect from
    // unused pixels in the texture when doing bilinear filtering.
    if (rect.w < tw) {
        // FIXME: Wrapper call via fsemu-opengl ?
        // fsemu_opengl_unpack_row_length(row_length);
        // fsemu_opengl_log_error_maybe();
        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        rect.w,
                        rect.y,
                        1,
                        rect.h,
                        format,
                        type,
                        // pixels + (rect.w * 4 * rect.y) + (rect.w - 1) * 4);
                        pixels + (rect.w - 1) * 4);
        fsemu_opengl_log_error_maybe();
        // FIXME: Remove
        // fsemu_opengl_unpack_row_length(0);
        // fsemu_opengl_log_error_maybe();
    }

    static uint8_t *greenline;
    if (greenline == NULL) {
        greenline = (uint8_t *) malloc(2048 * 4);
        for (int i = 0; i < 2048; i++) {
            ((int32_t *) greenline)[i] = FSEMU_RGB(0x00ff00);
        }
    }
    if (fsemu_perfgui_mode() == 2) {
        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        rect.x,
                        rect.y,
                        rect.w,
                        1,
                        GL_RGBA,
                        GL_UNSIGNED_BYTE,
                        greenline);
        fsemu_opengl_log_error_maybe();
    }

    if (frame->partial > 0 && frame->partial != frame->height) {
        return;
    }

    fsemu_glvideo.limits_rect = frame->limits;

    // Only draw bottom border duplicate line for last slice, and only if
    // there is space for it in the texture
    if (rect.y + rect.h < th) {
        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        0,
                        rect.y + rect.h,
                        rect.w,
                        1,
                        format,
                        type,
                        pixels + (rect.w * 4 * (rect.h - 1)));
        fsemu_opengl_log_error_maybe();
        // Draw corner pixel if room for it
        if (rect.w < tw) {
            glTexSubImage2D(GL_TEXTURE_2D,
                            0,
                            rect.w,
                            rect.y + rect.h,
                            1,
                            1,
                            format,
                            type,
                            pixels + (rect.w * rect.h - 1) * 4);
            fsemu_opengl_log_error_maybe();
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
}

void fsemu_glvideo_work(int timeout_us)
{
    fsemu_video_frame_t *frame = fsemu_video_get_frame(timeout_us);
    if (frame) {
        fsemu_glvideo_handle_frame(frame);
        fsemu_video_free_frame(frame);
        return;
    }
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

// FIXME
static struct {
    double left;
    double top;
    double right;
    double bottom;
} fsemu_glvideo_rect_temp;

static struct {
    fsemu_drect_t video_rect;
} fsemu_glvideo_temp;

void fsemu_glvideo_set_rect_temp(double left,
                                 double top,
                                 double right,
                                 double bottom)
{
    fsemu_glvideo_rect_temp.left = left;
    fsemu_glvideo_rect_temp.top = top;
    fsemu_glvideo_rect_temp.right = right;
    fsemu_glvideo_rect_temp.bottom = bottom;

    fsemu_glvideo_temp.video_rect.x = left;
    fsemu_glvideo_temp.video_rect.y = top;
    fsemu_glvideo_temp.video_rect.w = right - left;
    fsemu_glvideo_temp.video_rect.h = bottom - top;
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

    src.x = fsemu_glvideo.limits_rect.x;
    src.y = fsemu_glvideo.limits_rect.y;
    src.w = fsemu_glvideo.limits_rect.w;
    src.h = fsemu_glvideo.limits_rect.h;

    // printf("------------------------ glvideo -------------------\n");

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

    // printf("tx2 %f ty1 %f\n", tx2, ty1);
#if 0
    fsemu_opengl_blend(true);
    fsemu_opengl_color4f(0.0f, 0.0f, 0.0f, 0.0f);
    fsemu_opengl_depth_test(true);
    fsemu_opengl_texture_2d(false);
#endif

    fsemu_opengl_blend(false);
    fsemu_opengl_color4f(1.0f, 1.0f, 1.0f, 1.0f);
    fsemu_opengl_depth_test(false);
    fsemu_opengl_texture_2d(true);

    glBindTexture(GL_TEXTURE_2D,
                  fsemu_glvideo.textures[fsemu_glvideo.current_texture]);
    fsemu_opengl_log_error_maybe();

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
    fsemu_opengl_log_error_maybe();

    // FIXME: REMOVE
    fsemu_glvideo_rect_temp.left = dr.x;
    fsemu_glvideo_rect_temp.top = dr.y + dr.h;
    fsemu_glvideo_rect_temp.right = dr.x + dr.w;
    fsemu_glvideo_rect_temp.bottom = dr.y;

    fsemu_glvideo_temp.video_rect.x = 0.5 + dr.x / 2.0;
    fsemu_glvideo_temp.video_rect.y = 1.0 - (0.5 + (dr.y + dr.h) / 2.0);
    fsemu_glvideo_temp.video_rect.w = dr.w / 2.0;
    fsemu_glvideo_temp.video_rect.h = dr.h / 2.0;

#if 0
    printf("video rect %f %f %f %f\n",
           fsemu_glvideo_temp.video_rect.x,
           fsemu_glvideo_temp.video_rect.y,
           fsemu_glvideo_temp.video_rect.w,
           fsemu_glvideo_temp.video_rect.h);
#endif

    fsemu_video_set_frame_rendered_at(fsemu_glvideo.frame_count,
                                      fsemu_time_us());
    fsemu_glvideo.frame_count += 1;
}

void fsemu_glvideo_set_frame_rendered_externally(void)
{
    fsemu_rect_t video_rect;
    fsemu_videothread_video_rect(&video_rect);

    fsemu_size_t drawable_size;
    fsemu_videothread_drawable_size(&drawable_size);

    // FIXME: This should not be necessary to do here, should be handled
    // internally by FSEMU instead since FSEMU know's the video rect.
    // FIXME2: Now moved to FSEMU, but perhaps do otherwise
    double left = (double) video_rect.x / drawable_size.w;
    double right = (double) (video_rect.x + video_rect.w) / drawable_size.w;
    double top = (double) video_rect.y / drawable_size.h;
    double bottom = (double) (video_rect.y + video_rect.h) / drawable_size.h;
    fsemu_glvideo_set_rect_temp(left, top, right, bottom);

    fsemu_video_set_frame_rendered_at(fsemu_glvideo.frame_count,
                                      fsemu_time_us());
    fsemu_glvideo.frame_count += 1;
}

static int64_t fsemu_glvideo_wait_for_swap(void)
{
    // printf("wait for swap\n");
    // FIXME: IMPROVE!
    glFinish();
    fsemu_opengl_log_error_maybe();

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
        fsemu_opengl_log_error_maybe();
    } else {
        // FIXME: log_warning
        fsemu_video_log("Not a valid drawable size for glViewport\n");
    }

#endif

    // glClearColor(0.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    fsemu_opengl_log_error_maybe();
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

static void fsemu_glvideo_convert_coordinate_2(double *out,
                                               fsemu_gui_coord_t *coord,
                                               fsemu_widget_t *widget)
{
#if 0
    printf("coordinates %d %0.2f\n", coord->anchor, coord->offset);
#endif
    double anchor;
    double scale;

    // int client_top = 80;
    // double offset_scale_x = 1920;
    // double offset_scale_y = 1080 + client_top;

    // FIXME: Do this calculation once for each pass through the render
    // tree, not for every item...
    fsemu_size_t window_size;
    fsemu_window_size(&window_size);

    int titlebar = fsemu_titlebar_static_height();
    fsemu_size_t client_size;
    client_size.w = window_size.w;
    client_size.h = window_size.h - titlebar;
    double yoff = 1.0 * titlebar / window_size.h;

    double scale_x =
        (1920.0 / 1080.0) / ((double) client_size.w / client_size.h) / 1920.0;
    double scale_y = (double) client_size.h / window_size.h / 1080.0;

    if (coord->anchor == FSEMU_WIDGET_VIDEO_TOP) {
        anchor = fsemu_glvideo_temp.video_rect.y;
        scale = scale_y;
    } else if (coord->anchor == FSEMU_WIDGET_VIDEO_RIGHT) {
        anchor =
            fsemu_glvideo_temp.video_rect.x + fsemu_glvideo_temp.video_rect.w;
        scale = scale_x;
    } else if (coord->anchor == FSEMU_WIDGET_VIDEO_BOTTOM) {
        anchor =
            fsemu_glvideo_temp.video_rect.y + fsemu_glvideo_temp.video_rect.h;
        scale = scale_y;
    } else if (coord->anchor == FSEMU_WIDGET_VIDEO_LEFT) {
        anchor = fsemu_glvideo_temp.video_rect.x;
        scale = scale_x;
    } else if (coord->anchor == FSEMU_WIDGET_SCREEN_TOP) {
        anchor = yoff;
        scale = scale_y;
    } else if (coord->anchor == FSEMU_WIDGET_SCREEN_RIGHT) {
        anchor = 1.0;
        scale = scale_x;
    } else if (coord->anchor == FSEMU_WIDGET_SCREEN_BOTTOM) {
        anchor = 1.0;
        scale = scale_y;
    } else if (coord->anchor == FSEMU_WIDGET_SCREEN_LEFT) {
        anchor = 0.0;
        scale = scale_x;

    } else if (coord->anchor == FSEMU_WIDGET_PARENT_TOP) {
        if (widget->parent) {
            anchor = widget->parent->render_rect.y;
            scale = scale_y;
            // printf("anchor %f + offset %f * scale %f\n",
            //        anchor,
            //        coord->offset,
            //        scale);
        } else {
            // FIXME: Use FSEMU_WIDGET_SCREEN_TOP fallback
            anchor = 0.0;
            scale = 1;
        }
    } else if (coord->anchor == FSEMU_WIDGET_PARENT_RIGHT) {
        if (widget->parent) {
            anchor =
                widget->parent->render_rect.x + widget->parent->render_rect.w;
            scale = scale_x;
        } else {
            // FIXME: Use fallback
            anchor = 0.0;
            scale = 1;
        }
    } else if (coord->anchor == FSEMU_WIDGET_PARENT_BOTTOM) {
        if (widget->parent) {
            anchor =
                widget->parent->render_rect.y + widget->parent->render_rect.h;
            scale = scale_y;
        } else {
            // FIXME: Use fallback
            anchor = 0.0;
            scale = 1;
        }
    } else if (coord->anchor == FSEMU_WIDGET_PARENT_LEFT) {
        if (widget->parent) {
            anchor = widget->parent->render_rect.x;
            scale = scale_x;
        } else {
            // FIXME: Use fallback
            anchor = 0.0;
            scale = 1;
        }
    } else {
        anchor = 0.0;
        scale = 1;
    }
    *out = anchor + coord->offset * scale;
}

static void fsemu_glvideo_convert_coordinates_2(fsemu_drect_t *out,
                                                fsemu_widget_t *widget)
{
    double x2, y2;
    fsemu_glvideo_convert_coordinate_2(&out->x, &widget->left, widget);
    fsemu_glvideo_convert_coordinate_2(&out->y, &widget->top, widget);
    fsemu_glvideo_convert_coordinate_2(&x2, &widget->right, widget);
    fsemu_glvideo_convert_coordinate_2(&y2, &widget->bottom, widget);

#if 0
    printf("-> %f %f %f (x2) %f (y2)\n", out->x, out->y, x2, y2);
#endif
    out->w = x2 - out->x;
    out->h = y2 - out->y;
#if 0
    printf("-> -> %f %f %f %f\n", out->x, out->y, out->w, out->h);
#endif
}

static void fsemu_glvideo_render_rect(fsemu_drect_t *dr,
                                      fsemu_widget_t *widget)
{
    // New style coordinates
    dr->x = -1.0 + 2.0 * widget->render_rect.x;
    dr->y = 1.0 - 2.0 * (widget->render_rect.y + widget->render_rect.h);
    dr->w = 2.0 * widget->render_rect.w;
    dr->h = 2.0 * widget->render_rect.h;

#if 0
    if (widget->left.anchor) {
        printf("%f %f %f %f -> %f %f %f %f\n",
               widget->render_rect.x,
               widget->render_rect.y,
               widget->render_rect.w,
               widget->render_rect.h,
               dr->x,
               dr->y,
               dr->w,
               dr->h);
    }
#endif

    if (!widget->left.anchor) {
#if 0
        // New style coordinates
        dr->x = -1.0 + 2.0 * widget->render_rect.x;
        dr->y = 1.0 - 2.0 * widget->render_rect.y;
        dr->w = 2.0 * widget->render_rect.w;
        dr->h = 2.0 * widget->render_rect.h;
#if 0
        fsemu_glvideo_convert_coordinates_2(dr, widget);

        printf("Render image to %0.2f, %0.2f %0.2f x %0.2f\n",
               dr->x,
               dr->y,
               dr->w,
               dr->h);
#endif
    } else {
#endif
        // Old style coordinates
        fsemu_glvideo_convert_coordinates(
            dr, &widget->rect, widget->coordinates);
    }
}

static void fsemu_glvideo_render_text(fsemu_gui_item_t *widget)
{
    fsemu_drect_t dr;
    fsemu_glvideo_render_rect(&dr, widget);
    uint8_t r, g, b, a;
    fsemu_color_decode(widget->color, &r, &g, &b, &a);

#if 1
    // FIXME: Inefficient, temporary
    fsemu_size_t window_size;
    fsemu_window_size(&window_size);
    int titlebar = fsemu_titlebar_static_height();
    fsemu_size_t client_size;
    client_size.w = window_size.w;
    client_size.h = window_size.h - titlebar;
#endif

    // printf("dr.h = %0.1f\n", dr.h);
    // Vertical text alignment
    double text_h = 2.0 * widget->textimage->height / 1080.0;
    double text_w = 2.0 * widget->textimage->width * client_size.h /
                    client_size.w / 1080.0;
    dr.y = dr.y + (dr.h - text_h) * (1.0 - widget->text_valign);
    dr.h = text_h;
    // FIXME: Clamp with to original dr.w plus adjust tx to achieve clipping?
    dr.w = text_w;
    // printf("dr.h = %0.1f\n", dr.h);
    dr.w = text_w;

    // FIXME: Inefficient
    GLuint texture;
    glGenTextures(1, &texture);
    fsemu_opengl_log_error_maybe();
    glBindTexture(GL_TEXTURE_2D, texture);
    fsemu_opengl_log_error_maybe();

    fsemu_image_t *image = widget->textimage;
    // image->depth,
    // image->stride,

    fsemu_opengl_unpack_row_length(0);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 image->width,
                 image->height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 image->data);
    fsemu_opengl_log_error_maybe();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    fsemu_opengl_log_error_maybe();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    fsemu_opengl_log_error_maybe();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    fsemu_opengl_log_error_maybe();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    fsemu_opengl_log_error_maybe();

    float tx1 = 0.0f, tx2 = 1.0f;
    float ty1 = 1.0f, ty2 = 0.0f;
    if (widget->flags & FSEMU_WIDGET_FLAG_FLIPX) {
        tx1 = 1.0f;
        tx2 = 0.0f;
    }

    fsemu_opengl_blend(true);
    // fsemu_opengl_color4f(1.0f, 1.0f, 1.0f, 1.0f);
    fsemu_opengl_depth_test(false);
#if FSEMU_OPENGL_SHOW_TEXT_DEBUG_RECTANGLE
    fsemu_opengl_texture_2d(false);
    fsemu_opengl_color4f(1.0, 0.0, 0.0, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(dr.x, dr.y, 0);
    glVertex3f(dr.x + dr.w, dr.y, 0);
    glVertex3f(dr.x + dr.w, dr.y + dr.h, 0);
    glVertex3f(dr.x, dr.y + dr.h, 0);
    glEnd();
#endif
    fsemu_opengl_color4f(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
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
    fsemu_opengl_log_error_maybe();

    // fsemu_video_log("Render image to %0.2f,%0.2f %0.2fx%0.2f\n",
    //                 dr.x, dr.y, dr.w, dr.h);
    glDeleteTextures(1, &texture);
    fsemu_opengl_log_error_maybe();
}

static void fsemu_glvideo_render_image(fsemu_gui_item_t *widget)
{
    fsemu_drect_t dr;
#if 0
    if (item->left.anchor) {
        fsemu_glvideo_convert_coordinates_2(&dr, item);

        printf("Render image to %0.2f, %0.2f %0.2f x %0.2f\n",
               dr.x,
               dr.y,
               dr.w,
               dr.h);
    } else {
        fsemu_glvideo_convert_coordinates(&dr, &item->rect, item->coordinates);
    }
#endif
    fsemu_glvideo_render_rect(&dr, widget);
    uint8_t r, g, b, a;
    fsemu_color_decode(widget->color, &r, &g, &b, &a);

    // FIXME: Inefficient
    GLuint texture;
    glGenTextures(1, &texture);
    fsemu_opengl_log_error_maybe();
    glBindTexture(GL_TEXTURE_2D, texture);
    fsemu_opengl_log_error_maybe();

    fsemu_image_t *image = widget->image;
    // image->depth,
    // image->stride,

    fsemu_opengl_unpack_row_length(0);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 image->width,
                 image->height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 image->data);
    fsemu_opengl_log_error_maybe();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    fsemu_opengl_log_error_maybe();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    fsemu_opengl_log_error_maybe();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    fsemu_opengl_log_error_maybe();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    fsemu_opengl_log_error_maybe();

    float tx1 = 0.0f, tx2 = 1.0f;
    float ty1 = 1.0f, ty2 = 0.0f;
    if (widget->flags & FSEMU_WIDGET_FLAG_FLIPX) {
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
    fsemu_opengl_log_error_maybe();

    // fsemu_video_log("Render image to %0.2f,%0.2f %0.2fx%0.2f\n",
    //                 dr.x, dr.y, dr.w, dr.h);
    glDeleteTextures(1, &texture);
    fsemu_opengl_log_error_maybe();
}

static void fsemu_glvideo_render_rectangle(fsemu_gui_item_t *widget)
{
    fsemu_drect_t dr;
    fsemu_glvideo_render_rect(&dr, widget);
    uint8_t r, g, b, a;
    fsemu_color_decode(widget->color, &r, &g, &b, &a);

    if (a == 0) {
        // Optimization: No need to render fully transparent rectangle.
        return;
    }

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
    fsemu_opengl_log_error_maybe();
}

// void fsemu_glvideo_render_item(fsemu_gui_item_t *item);

static void fsemu_glvideo_update_render_rect(fsemu_widget_t *widget)
{
    fsemu_glvideo_convert_coordinates_2(&widget->render_rect, widget);
#if 0
    if (widget->flags & FSEMU_WIDGET_FLAG_DEBUG) {
        fsemu_drect_t *r = &widget->render_rect;
        printf("Widget %f %f %f %f\n",
               widget->top.offset,
               widget->right.offset,
               widget->bottom.offset,
               widget->left.offset);
        printf("Widget %f %f %f %f\n", r->x, r->y, r->w, r->h);
    }
#endif
}

static void fsemu_glvideo_render_item(fsemu_gui_item_t *item)
{
    if (item->visible) {
        if (item->textimage) {
            fsemu_glvideo_render_text(item);
        } else if (item->image) {
            fsemu_glvideo_render_image(item);
        } else {
            fsemu_glvideo_render_rectangle(item);
        }

        // if (item->first_child) {
        //     printf("render rect: %f %f %f %f\n",
        //            item->render_rect.x,
        //            item->render_rect.y,
        //            item->render_rect.w,
        //            item->render_rect.h);
        // }
        for (fsemu_widget_t *child = item->first_child; child;
             child = child->next) {
            if (child->parent != item) {
                fsemu_error(
                    "Incorrect parent relationship in %s. Child %p (%s) "
                    "parent %p (%s) (Expected parent: %p (%s))\n",
                    __func__,
                    child,
                    fsemu_widget_name(child),
                    child->parent,
                    fsemu_widget_name(child->parent),
                    item,
                    fsemu_widget_name(item));
            }
            // printf("child->parent render rect: %f %f %f %f\n",
            //        child->parent->render_rect.x,
            //        child->parent->render_rect.y,
            //        child->parent->render_rect.w,
            //        child->parent->render_rect.h);
            fsemu_glvideo_update_render_rect(child);
            fsemu_glvideo_render_item(child);
        }
    }
}

static void fsemu_glvideo_render_gui_2(fsemu_gui_item_t *items, bool early)
{
    for (fsemu_gui_item_t *item = items; item; item = item->next) {
        if (early && item->z_index >= 0) {
            continue;
        }
        if (!early && item->z_index < 0) {
            continue;
        }
        if (!item->visible) {
            continue;
        }
        if (item->left.anchor) {
            fsemu_glvideo_update_render_rect(item);
        }
        fsemu_glvideo_render_item(item);
    }
}

void fsemu_glvideo_render_gui_early(fsemu_gui_item_t *items)
{
    fsemu_glvideo_render_gui_2(items, true);
}

void fsemu_glvideo_render_gui(fsemu_gui_item_t *items)
{
    fsemu_glvideo_render_gui_2(items, false);
}
