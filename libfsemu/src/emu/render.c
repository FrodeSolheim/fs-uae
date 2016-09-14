#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define FS_EMU_INTERNAL
#include <fs/emu.h>
#include <fs/emu/options.h>
#include <fs/emu/path.h>

#include "render.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fs/conf.h>
#include <fs/filesys.h>
#include <fs/i18n.h>
#include <fs/glib.h>
#include <fs/ml.h>
#include <fs/thread.h>
#include <fs/time.h>
#include <fs/util.h>

#include "libfsemu.h"
#include "audio.h"
#include "dialog.h"
#include "emu_lua.h"
#include "font.h"
#include "hud.h"
#include "menu.h"
#include "scanlines.h"
#include "texture.h"
#include "theme.h"
#include "util.h"
#include "video.h"
#include "video_buffer.h"
#include "xml_shader.h"

#ifdef USE_OPENGL
#include <fs/ml/opengl.h>
#endif

#ifdef USE_GLES
#define glScaled glScalef
#define glTranslated glTranslatef
#define glRotated glRotatef
#define double float
#endif

#define VIDEO_DEBUG_SCALE_TIMES 2.5

// menu transition
static double g_menu_transition = 0.0;
static double g_menu_transition_target = 0.0;

//static fs_emu_texture g_frame_texture = {};
static GLuint g_frame_texture = 0;
static int g_frame_texture_width = 0;
static int g_frame_texture_height = 0;

static int g_texture_filter = GL_LINEAR;

static int g_frame_texture_black_left = 100000;
static int g_frame_texture_black_top = 100000;

// crop coordinates of emulator video frame
static fs_emu_rect g_crop = {};

#define FS_EMU_VIEWPORT_MODE_CROP 0
#define FS_EMU_VIEWPORT_MODE_CENTER 1
static int g_viewport_mode = FS_EMU_VIEWPORT_MODE_CROP;
static int g_effective_viewport_mode = FS_EMU_VIEWPORT_MODE_CROP;

// size of emulator video frame
static int g_frame_width = 0;
static int g_frame_height = 0;

static double g_frame_aspect = 0;

static uint8_t *g_scanline_buffer = NULL;
static int g_scanline_buffer_width = -1;
static int g_scanline_buffer_height = -1;

// scaling and alignment options
static double g_scale_x = -1.0;
static double g_scale_y = -1.0;
static double g_align_x = 0.5;
static double g_align_y = 0.5;

static int g_frame_override = 0;
static int g_frame_override_x = 0;
static int g_frame_override_y = 0;
static int g_frame_override_w = 0;
static int g_frame_override_h = 0;
static int g_l_scale_x = 1920;
static int g_l_scale_y = 1080;

struct overlay_status {
    int state;
    int render_state;
    //int was_enabled;
    //int was_disabled;
    int render_status;
};
static struct overlay_status g_overlay_status[FS_EMU_MAX_OVERLAYS];
static fs_mutex *g_overlay_mutex = NULL;

void fs_emu_set_custom_overlay_state(int overlay, int state) {
    fs_emu_set_overlay_state(FS_EMU_FIRST_CUSTOM_OVERLAY + overlay, state);
}

void fs_emu_set_overlay_state(int overlay, int state) {
    if (overlay < 0 || overlay >= FS_EMU_MAX_OVERLAYS) {
        return;
    }
    // fs_mutex_lock(g_overlay_mutex);
    g_overlay_status[overlay].state = state;

#if 0
    if (state) {
        g_overlay_status[overlay].state = 1;
        g_overlay_status[overlay].was_enabled = 1;
    }
    else {
        g_overlay_status[overlay].state = 0;
        g_overlay_status[overlay].was_disabled = 1;
    }
#endif
    // fs_mutex_unlock(g_overlay_mutex);
}

static void setup_opengl() {
    fs_log("setup_opengl\n");
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    CHECK_GL_ERROR();
}

static void context_notification_handler(int notification, void *data) {
    if (notification == FS_GL_CONTEXT_DESTROY) {
        if (g_frame_texture != 0) {
            glDeleteTextures(1, &g_frame_texture);
            CHECK_GL_ERROR();
            g_frame_texture = 0;
        }
    }
    else if (notification == FS_GL_CONTEXT_CREATE) {
        setup_opengl();
    }
}

void fs_emu_initialize_opengl() {
    setup_opengl();
    fs_emu_initialize_textures();
    fs_gl_add_context_notification(context_notification_handler, NULL);
}

static void get_buffer_format(int *gl_buffer_format, int *gl_buffer_type) {
    int format = fs_emu_get_video_format();
    if (format == FS_EMU_VIDEO_FORMAT_BGRA) {
        *gl_buffer_format = GL_BGRA;
#ifdef WORDS_BIGENDIAN
        *gl_buffer_type = GL_UNSIGNED_INT_8_8_8_8_REV;
#else
        *gl_buffer_type = GL_UNSIGNED_BYTE;
#endif
    }
    else if (format == FS_EMU_VIDEO_FORMAT_RGBA) {
        *gl_buffer_format = GL_RGBA;
#ifdef WORDS_BIGENDIAN
        *gl_buffer_type = GL_UNSIGNED_INT_8_8_8_8_REV;
#else
        *gl_buffer_type = GL_UNSIGNED_BYTE;
#endif
    }
    else if (format == FS_EMU_VIDEO_FORMAT_RGB) {
        *gl_buffer_format = GL_RGB;
#ifdef WORDS_BIGENDIAN
        *gl_buffer_type = GL_UNSIGNED_INT_8_8_8_8_REV;
#else
        *gl_buffer_type = GL_UNSIGNED_BYTE;
#endif
    }
    else if (format == FS_EMU_VIDEO_FORMAT_R5G6B5) {
        *gl_buffer_format = GL_RGB;
        *gl_buffer_type = GL_UNSIGNED_SHORT_5_6_5;
    }
    else if (format == FS_EMU_VIDEO_FORMAT_R5G5B5A1) {
        *gl_buffer_format = GL_RGBA;
        *gl_buffer_type = GL_UNSIGNED_SHORT_5_5_5_1;
    }
}

static void create_texture_if_needed(int width, int height) {
    //g_frame_texture.video_version = g_fs_emu_video_version;
    if (g_frame_texture && g_frame_texture_width >= width &&
            g_frame_texture_height >= height) {
        return;
    }
    fs_gl_bind_texture(0);

    if (g_frame_texture) {
        glDeleteTextures(1, &g_frame_texture);
        CHECK_GL_ERROR();
    }
    g_frame_texture_width = 1;
    while (g_frame_texture_width < width) {
        g_frame_texture_width *= 2;
    }
    g_frame_texture_height = 1;
    while (g_frame_texture_height < height) {
        g_frame_texture_height *= 2;
    }
    glGenTextures(1, &g_frame_texture);
    CHECK_GL_ERROR();
    //g_frame_texture.opengl_context_stamp = g_fs_ml_opengl_context_stamp;
    //fs_emu_set_texture(&g_frame_texture);
    fs_gl_bind_texture(g_frame_texture);
#if 0
    // with high quality borders, there should be no reason to initialize
    // the texture to black
    void *data = g_malloc0(g_frame_texture_width * g_frame_texture_height * 4);
#endif
    void *data = NULL;
    fs_gl_unpack_row_length(0);
    int gl_buffer_format = 0;
    int gl_buffer_type = 0;
    get_buffer_format(&gl_buffer_format, &gl_buffer_type);
    glTexImage2D(GL_TEXTURE_2D, 0, fs_emu_get_texture_format(),
            g_frame_texture_width, g_frame_texture_height, 0,
            gl_buffer_format, gl_buffer_type, data);
    CHECK_GL_ERROR();
#if 0
    if (data) {
        free(data);
    }
#endif
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, g_texture_filter);
    CHECK_GL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, g_texture_filter);
    CHECK_GL_ERROR();
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //CHECK_GL_ERROR();
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //CHECK_GL_ERROR();
#ifdef USE_GLES

#else
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    CHECK_GL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    CHECK_GL_ERROR();
#endif

    // texture contains no data now, so we ensure that the upload routine
    // blanks the border if necessary
    g_frame_texture_black_left = 100000;
    g_frame_texture_black_top = 100000;
}

static void fix_border(fs_emu_video_buffer *buffer, int *upload_x,
        int *upload_y, int *upload_w, int *upload_h) {
    // upload fixed border as well
    int ux = g_crop.x;
    int uy = g_crop.y;
    int uw = g_crop.w;
    int uh = g_crop.h;

    // currently disabled border fixing ... -because this "destroys" the
    // buffer so it cannot be copied properly from when reusing lines
    // for the next frame
#if 0
    int32_t *idata = (int32_t*) buffer->data;
    int32_t *src;
    int32_t *dst;

    if (uy > 1) {
        src = idata + uy * buffer->width + ux;
        uy--;
        dst = idata + uy * buffer->width + ux;
        for (int x = 0; x < uw; x++) {
            *dst++ = *src++;
        }
        uh++;
    }
    //printf("%d %d %d\n", uy, uh, buffer->buffer_height);
    if (uy + uh < buffer->buffer_height) {
        src = idata + (uy + uh - 1) * buffer->width + ux;
        uh++;
        dst = idata + (uy + uh - 1) * buffer->width + ux;
        for (int x = 0; x < uw; x++) {
            *dst++ = *src++;
        }
    }
    if (ux > 1) {
        src = idata + uy * buffer->width + ux;
        ux--;
        dst = idata + uy * buffer->width + ux;
        for (int y = 0; y < uh; y++) {
            *dst = *src;
            src += buffer->width;
            dst += buffer->width;
        }
        uw++;
    }
    if (ux + uw < buffer->buffer_width) {
        src = idata + uy * buffer->width + (ux + uw - 1);
        uw++;
        dst = idata + uy * buffer->width + (ux + uw - 1);
        for (int y = 0; y < uh; y++) {
            *dst = *src;
            src += buffer->width;
            dst += buffer->width;
        }
    }
#endif
    *upload_x = ux;
    *upload_y = uy;
    *upload_w = uw;
    *upload_h = uh;
}

#define R5G6B5_MASK_R 0xf800
#define R5G6B5_MASK_G 0x07e0
#define R5G6B5_MASK_B 0x001f
#define R5G6B5_SHIFT_R 11
#define R5G6B5_SHIFT_G 5
#define R5G6B5_SHIFT_B 0

#define R5G5B5A1_MASK_R 0xf800
#define R5G5B5A1_MASK_G 0x07c0
#define R5G5B5A1_MASK_B 0x003e
#define R5G5B5A1_SHIFT_R 11
#define R5G5B5A1_SHIFT_G 6
#define R5G5B5A1_SHIFT_B 1

static void save_screenshot(const char *path, int cx, int cy, int cw, int ch,
        int count, uint8_t *frame, int frame_width, int frame_height,
        int frame_bpp) {
    fs_log("writing screenshot to %s\n", path);

    uint8_t *out_data = malloc(cw * ch * 3);
    int row_len = cw * frame_bpp;
    int frame_format = fs_emu_get_video_format();

    for (int y = 0; y < ch; y++) {
        uint8_t *ip = frame + ((cy + y) * frame_width + cx) * frame_bpp;
        uint8_t *op = out_data + y * cw * 3;
        if (frame_format == FS_EMU_VIDEO_FORMAT_BGRA) {
            for (int x = 0; x < row_len; x += frame_bpp) {
#ifdef WORDS_BIGENDIAN
                *op++ = ip[x + 1];
                *op++ = ip[x + 2];
                *op++ = ip[x + 3];
#else
                *op++ = ip[x + 2];
                *op++ = ip[x + 1];
                *op++ = ip[x + 0];
#endif
            }
        }
        else if (frame_format == FS_EMU_VIDEO_FORMAT_RGBA) {
            for (int x = 0; x < row_len; x += frame_bpp) {
                *op++ = ip[x + 0];
                *op++ = ip[x + 1];
                *op++ = ip[x + 2];
            }
        }
        else if (frame_format == FS_EMU_VIDEO_FORMAT_R5G6B5) {
            for (int x = 0; x < row_len; x += frame_bpp) {
                unsigned short *p = (unsigned short *) (ip + x);
                unsigned char c;
                c = (*p & R5G6B5_MASK_R) >> R5G6B5_SHIFT_R;
                *op++ = (c << 3) | (c >> 2);
                c = (*p & R5G6B5_MASK_G) >> R5G6B5_SHIFT_G;
                *op++ = (c << 2) | (c >> 4);
                c = (*p & R5G6B5_MASK_B) >> R5G6B5_SHIFT_B;
                *op++ = (c << 3) | (c >> 2);
            }
        }
        else if (frame_format == FS_EMU_VIDEO_FORMAT_R5G5B5A1) {
            for (int x = 0; x < row_len; x += frame_bpp) {
                unsigned short *p = (unsigned short *) (ip + x);
                unsigned char c;
                c = (*p & R5G5B5A1_MASK_R) >> R5G5B5A1_SHIFT_R;
                *op++ = (c << 3) | (c >> 2);
                c = (*p & R5G5B5A1_MASK_G) >> R5G5B5A1_SHIFT_G;
                *op++ = (c << 3) | (c >> 2);
                c = (*p & R5G5B5A1_MASK_B) >> R5G5B5A1_SHIFT_B;
                *op++ = (c << 3) | (c >> 2);
            }
        }
    }

    int result = fs_image_save_data(path, out_data, cw, ch, 3);
    if (result) {
        fs_log("saved screenshot\n");
    }
    else {
        fs_log("error saving screenshot\n");
    }
    free(out_data);
#if 0
    free(name);
    free(path);
#endif
}

static int update_texture() {
    fs_emu_video_buffer *buffer = fs_emu_video_buffer_lock();
#ifdef DEBUG_VIDEO_SYNC
    printf("u %p\n", buffer);
#endif
    // unlocked in fs_emu_video_after_update

    if (buffer->seq == 0) {
        // we haven't received a video frame from the emulator yet
        return -1;
    }

    uint8_t *frame = buffer->data;
    if (frame == NULL) {
        return -1;
    }
    int is_new_frame = 1;
    static int last_seq_no = -1;
    if (buffer->seq == last_seq_no + 1) {
        // normal
    }
    else if (buffer->seq == last_seq_no) {
        //fs_log("WARNING: repeated frame %d\n", info->seq_no);
        g_fs_emu_repeated_frames++;
        if (g_fs_emu_repeated_frames > 9999) {
            g_fs_emu_repeated_frames = 9999;
        }
        g_fs_emu_repeated_frame_time = fs_get_monotonic_time();
        is_new_frame = 0;
    }
    else {
        int lost_frame_count = buffer->seq - last_seq_no - 1;
        g_fs_emu_lost_frames += lost_frame_count;
        g_fs_emu_lost_frame_time = fs_get_monotonic_time();
        //fs_log("lost %d frame(s)\n", lost_frame_count);
    }
    last_seq_no = buffer->seq;

    is_new_frame = 1;

    int width = buffer->width;
    int height = buffer->height;
    int bpp = buffer->bpp;

    // check for cropping before screenshot, so we can also save cropped
    // screenshot

    if (g_fs_emu_video_crop_mode) {
        g_crop = buffer->crop;
        if (g_crop.w == 0) {
            g_crop.w = width;
        }
        if (g_crop.h == 0) {
            g_crop.h = height;
        }
    }
    else {
        g_crop.x = 0;
        g_crop.y = 0;
        g_crop.w = width;
        g_crop.h = height;
    }

    // g_fs_emu_screenshot is set to 1 when screenshot command is executed
    // (keyboard shortcut), but screenshot is saved here, as soon as possible.

    if (g_fs_emu_screenshot > 0) {
        static char* screenshots_dir = NULL;
        static char* screenshots_prefix = NULL;
        static int screenshots_mask = 7;
        if (screenshots_dir == NULL) {
            char *path = fs_config_get_string(OPTION_SCREENSHOTS_OUTPUT_DIR);
            if (path) {
                path = fs_emu_path_expand_and_free(path);
                if (fs_path_exists(path)) {
                    screenshots_dir = path;
                }
                else {
                    fs_emu_warning("Directory does not exist: %s", path);
                    g_free(path);
                }
            }
            if (!screenshots_dir) {
                screenshots_dir = g_strdup(fs_get_desktop_dir());
            }
            screenshots_mask = fs_config_get_int("screenshots_output_mask");
            if (screenshots_mask == FS_CONFIG_NONE) {
                screenshots_mask = 7;
            }

            screenshots_prefix = fs_config_get_string(
                    "screenshots_output_prefix");
            if (!screenshots_prefix) {
                screenshots_prefix = g_strdup("fs-uae");
            }
        }

        static int total_count = 0;
        if (g_fs_emu_screenshot == 1) {

            char *name, *path;
            time_t t = time(NULL);
            struct tm tm_struct;
            struct tm *tm_p = &tm_struct;
            fs_localtime_r(&t, tm_p);

            static int count = 0;
            static char laststrbuf[20] = {};

            char strbuf[20];
            strftime(strbuf, 20, "%y%m%d%H%M", tm_p);
            if (strcmp(strbuf, laststrbuf) != 0) {
                count = 0;
                memcpy(laststrbuf, strbuf, 20);
            }

            count += 1;
            total_count += 1;

            if (screenshots_mask & 1) {
                name = g_strdup_printf("%s-%s-%s-%02d.png",
                        screenshots_prefix, "full", strbuf, count);
                path = g_build_filename(screenshots_dir, name, NULL);
                save_screenshot(path, 0, 0, width, height, count, frame,
                        width, height, bpp);
                g_free(path);
                g_free(name);
            }
            if (screenshots_mask & 2) {
                name = g_strdup_printf("%s-%s-%s-%02d.png",
                        screenshots_prefix, "crop", strbuf, count);
                path = g_build_filename(screenshots_dir, name, NULL);
                save_screenshot(path, g_crop.x, g_crop.y, g_crop.w, g_crop.h,
                        count, frame, width, height, bpp);
                g_free(path);
                g_free(name);
            }
            if (screenshots_mask & 4) {
                name = g_strdup_printf("%s-%s-%s-%02d.png",
                        screenshots_prefix, "real", strbuf, count);
                path = g_build_filename(screenshots_dir, name, NULL);
                fs_ml_video_screenshot(path);
                g_free(path);
                g_free(name);
            }
            g_fs_emu_screenshot++;
        }
        else if (g_fs_emu_screenshot > 4) {
            // we wait a bit until printing the notification, so the OpenGL
            // screenshot can be captured before the notification is shown
            g_fs_emu_screenshot = 0;
            fs_emu_notification(81830444,
                    _("Saved screenshot %d"), total_count);
        }
        else {
            g_fs_emu_screenshot++;
        }
    }

    int upload_x, upload_y, upload_w, upload_h;
    g_effective_viewport_mode = g_viewport_mode;
    if (buffer->flags & FS_EMU_FORCE_VIEWPORT_CROP_FLAG) {
        g_effective_viewport_mode = FS_EMU_VIEWPORT_MODE_CROP;
    }

    if (g_effective_viewport_mode == FS_EMU_VIEWPORT_MODE_CROP) {
        fix_border(buffer, &upload_x, &upload_y, &upload_w, &upload_h);
    }
    else {
        upload_x = 0;
        upload_y = 0;
        upload_w = buffer->width;
        upload_h = buffer->height;
    }

    int filter = 0;
    if (0) {
        filter = 1;
    }
    else if (g_fs_emu_scanlines &&
            (buffer->flags & FS_EMU_NO_SCANLINES_FLAG) == 0) {
        filter = 2;
    }

    if (filter) {
        if (is_new_frame) {
            if (g_scanline_buffer_width != buffer->width ||
                    g_scanline_buffer_height != buffer->height) {
                if (g_scanline_buffer) {
                    free(g_scanline_buffer);
                }
                g_scanline_buffer = malloc(buffer->width * buffer->height * bpp);
                g_scanline_buffer_width = buffer->width;
                g_scanline_buffer_height = buffer->height;
            }
            if (filter == 2) {
                fs_emu_scanline_filter(g_scanline_buffer, buffer,
                        upload_x, upload_y, upload_w, upload_h,
                        g_fs_emu_scanlines_dark, g_fs_emu_scanlines_light);
            }
            else {
                fs_emu_2xcolor_filter(g_scanline_buffer, buffer,
                        upload_x, upload_y, upload_w, upload_h,
                        g_fs_emu_scanlines_dark, g_fs_emu_scanlines_light);                
            }
        }
        if (g_scanline_buffer) {
            frame = g_scanline_buffer;
        }
    }

    //fs_log("%d %d %d %d %d %d %d\n", width, height, bpp,
    //        g_crop.x, g_crop.y, g_crop.w, g_crop.h);
    g_frame_width = width;
    g_frame_height = height;
    g_frame_aspect = buffer->aspect;

    int gl_buffer_format = 0;
    int gl_buffer_type = 0;
    get_buffer_format(&gl_buffer_format, &gl_buffer_type);

    create_texture_if_needed(width, height);
    fs_gl_bind_texture(g_frame_texture);

    uint8_t *gl_buffer_start = frame + ((upload_y * width) + upload_x) * bpp;

#ifdef USE_GLES
    /* we don't have unpack padding in GLES. uploading full width lines instead */
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, upload_h,
            gl_buffer_format, gl_buffer_type, gl_buffer_start);
#else
    fs_gl_unpack_row_length(width);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, upload_w, upload_h,
            gl_buffer_format, gl_buffer_type, gl_buffer_start);
#endif
    CHECK_GL_ERROR();

    int update_black_border = 1;
    if (update_black_border) {

        int black_left = upload_w;
        int black_right = g_frame_texture_width;
        //printf("%d %d\n", black_left, black_right);
        if (g_frame_texture_black_left < black_right) {
            black_right = g_frame_texture_black_left;
            //printf("set g_frame_texture_black_left to %d\n", black_left);
        }
        //printf("%d %d\n", black_left, black_right);
        int black_width = black_right - black_left;
        if (black_width > 0) {
            void *black_data = g_malloc0(
                    black_width * g_frame_texture_height * bpp);
            //memset(black_data, 0xff, black_width * g_frame_texture_height * bpp);
            //printf("black1 w %d h %d\n", black_width, g_frame_texture_height);
            fs_gl_unpack_row_length(0);
            glTexSubImage2D(GL_TEXTURE_2D, 0, black_left, 0,
                    black_width, g_frame_texture_height,
                    gl_buffer_format, gl_buffer_type, black_data);
            free(black_data);
            CHECK_GL_ERROR();
        }
        g_frame_texture_black_left = black_left;

        int black_top = upload_h;
        int black_bottom = g_frame_texture_height;
        if (g_frame_texture_black_top < black_bottom) {
            black_bottom = g_frame_texture_black_top;
        }
        int black_height = black_bottom - black_top;
        //printf("---- %d %d\n", black_top, black_bottom);
        if (black_height > 0) {
            void *black_data = g_malloc0(
                    upload_w * black_height * bpp);
            //memset(black_data, 0xff, upload_w * black_height * bpp);
            //printf("black2 w %d h %d\n", upload_w, black_height);
            fs_gl_unpack_row_length(0);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, black_top,
                    upload_w, black_height,
                    gl_buffer_format, gl_buffer_type, black_data);
            free(black_data);
            CHECK_GL_ERROR();
        }
        g_frame_texture_black_top = black_top;
    }
    //g_frame_texture_black_left = 100000;
    //g_frame_texture_black_top = 100000;

#if 0
    int hq_border = 2;
    if (hq_border >= 1) {
        /*
        if (upload_y > 0) {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, upload_y - 1, width, 1,
                    format, GL_UNSIGNED_BYTE,
                    frame + (upload_y) * width * bpp);
            CHECK_GL_ERROR();
        }
        if (upload_y + upload_h < g_frame_texture_height) {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, upload_y + upload_h,
                    width, 1, format, GL_UNSIGNED_BYTE,
                    frame + (upload_y + upload_h - 1) * width * bpp);
            CHECK_GL_ERROR();
        }        */

        if (upload_h < g_frame_texture_height) {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, upload_h,
                    width, 1, format, GL_UNSIGNED_BYTE,
                    frame + (upload_y + upload_h - 1) * width * bpp);
            CHECK_GL_ERROR();
        }

        /*
        if (upload_x > 0) {
            glTexSubImage2D(GL_TEXTURE_2D, 0, upload_x - 1, upload_y,
                    1, upload_h, format, GL_UNSIGNED_BYTE, frame +
                    ((upload_y) * width + upload_x) * bpp);
            CHECK_GL_ERROR();
        }
        if (upload_x + upload_w < g_frame_texture_width) {
            glTexSubImage2D(GL_TEXTURE_2D, 0, upload_x + upload_w,
                    upload_y, 1, upload_h, format,
                    GL_UNSIGNED_BYTE, frame +
                    ((upload_y) * width + upload_x + upload_w - 1) * bpp);
            CHECK_GL_ERROR();
        }
        */

        if (upload_w < g_frame_texture_width) {
            glTexSubImage2D(GL_TEXTURE_2D, 0, upload_w,
                    upload_y, 1, upload_h, format,
                    GL_UNSIGNED_BYTE, frame +
                    ((upload_y) * width + upload_x + upload_w - 1) * bpp);
            CHECK_GL_ERROR();
        }
    }
#if 0
    if (hq_border >= 2) {
        if (upload_y + upload_h < g_frame_texture.height - 1) {
            //printf("1\n");
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, upload_y + upload_h + 1,
                    width, 1, format, GL_UNSIGNED_BYTE,
                    frame + (upload_y + upload_h - 1) * width * bpp);
                    //frame + (upload_y + upload_h - 2) * width * bpp);
        }

        if (upload_x + upload_w < g_frame_texture.width - 1) {
            glTexSubImage2D(GL_TEXTURE_2D, 0, upload_x + upload_w + 1,
                    upload_y, 1, upload_h, format,
                    GL_UNSIGNED_BYTE, frame +
                    ((upload_y) * width + upload_x + upload_w - 1) * bpp);
        }
    }
#endif
#endif
    return last_seq_no;
}

static void render_gloss(double alpha) {
    //fs_emu_set_texture(g_tex_screen_gloss);
    fs_gl_blending(1);
    //fs_emu_blending(0);
    //fs_emu_texturing(0);
    fs_gl_color4f(alpha, alpha, alpha, alpha);
    //fs_ml_color4f(alpha, 0.0, 0.0, alpha);
    //glBegin(GL_QUADS);
    //glTexCoord2f(0.0, 1.0); glVertex2f(-1.0, -1.0);
    //glTexCoord2f(1.0, 1.0); glVertex2f( 1.0, -1.0);
    //glTexCoord2f(1.0, 0.0); glVertex2f( 1.0,  1.0);
    //glTexCoord2f(0.0, 0.0); glVertex2f(-1.0,  1.0);
    fs_emu_draw_texture_with_size(TEXTURE_GLOSS, -1.0, -1.0, 2.0, 2.0);
    //glEnd();
}

static void render_quad(float x1, float y1, float x2, float y2, float s1,
        float t1, float s2, float t2) {
#ifdef USE_GLES
    GLfloat tex[] = {
        s1, t2,
        s2, t2,
        s2, t1,
        s1, t1
    };
    GLfloat vert[] = {
        x1, y1,
        x2, y1,
        x2, y2,
        x1, y2
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, vert);
    glTexCoordPointer(2, GL_FLOAT, 0, tex);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#else
    glBegin(GL_QUADS);
    glTexCoord2d(s1, t2);
    glVertex2f(x1, y1);
    glTexCoord2d(s2, t2);
    glVertex2f(x2, y1);
    glTexCoord2d(s2, t1);
    glVertex2f(x2, y2);
    glTexCoord2d(s1, t1);
    glVertex2f(x1, y2);
    glEnd();
#endif
    CHECK_GL_ERROR();
}

static void render_overlays(void) {
    fs_mutex_lock(g_overlay_mutex);
    for (int i = 0; i < FS_EMU_MAX_OVERLAYS; i++) {
        int state = g_overlay_status[i].state;
#if 0
        if (g_overlay_status[i].render_status &&
                g_overlay_status[i].was_disabled) {
            show = 0;
        }
        else if (!g_overlay_status[i].render_status &&
                g_overlay_status[i].was_enabled) {
            show = 1;
        }
        g_overlay_status[i].was_enabled = 0;
        g_overlay_status[i].was_disabled = 0;
#endif
        g_overlay_status[i].render_state = state;
        if (state) {
            g_overlay_status[i].render_status++;
        }
        else {
            g_overlay_status[i].render_status = 0;
        }
    }
    fs_mutex_unlock(g_overlay_mutex);

    for (int i = 0; i < FS_EMU_MAX_OVERLAYS; i++) {
        //if (g_overlay_status[i].render_status < 1) {
        //    continue;
        //}
        int state = g_overlay_status[i].render_state;
        if (state < 0) state = 0;
        if (state > FS_EMU_MAX_OVERLAY_STATES - 1) {
            state = FS_EMU_MAX_OVERLAY_STATES - 1;
        }

        fs_emu_texture *overlay_texture = \
                g_fs_emu_theme.overlays[i].textures[state];
        if (!overlay_texture) {
            continue;
        }

        float w = g_fs_emu_theme.overlays[i].w;
        float h = g_fs_emu_theme.overlays[i].h;
        float x1 = g_fs_emu_theme.overlays[i].x;
        if (g_fs_emu_theme.overlays[i].anchor & FS_EMU_ANCHOR_RIGHT_BIT) {
            x1 = x1 + 1.0 - w;
        }
        float y1 = g_fs_emu_theme.overlays[i].y;
        if (g_fs_emu_theme.overlays[i].anchor & FS_EMU_ANCHOR_BOTTOM_BIT) {
            y1 = y1 + 1.0 - h;
        }

        float x2 = x1 + w;
        float y2 = y1 + h;

        x1 = -1.0 + 2.0 * x1;
        x2 = -1.0 + 2.0 * x2;
        y1 =  1.0 - 2.0 * y1;
        y2 =  1.0 - 2.0 * y2;

        fs_gl_blending(1);
        fs_gl_texturing(1);
        fs_gl_color4f(1.0, 1.0, 1.0, 1.0);
        fs_emu_set_texture(overlay_texture);

#ifdef USE_GLES
        GLfloat tex[] = {
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0
        };
        GLfloat vert[] = {
            x1, y1,
            x2, y1,
            x2, y2,
            x1, y2
        };

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(2, GL_FLOAT, 0, vert);
        glTexCoordPointer(2, GL_FLOAT, 0, tex);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#else
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex2f(x1, y1);
        glTexCoord2f(1.0, 0.0); glVertex2f(x2, y1);
        glTexCoord2f(1.0, 1.0); glVertex2f(x2, y2);
        glTexCoord2f(0.0, 1.0); glVertex2f(x1, y2);
        glEnd();
#endif
    }
}

#include <fs/emu/hacks.h>

double fs_emu_video_scale_x = 0.0;
double fs_emu_video_scale_y = 0.0;
double fs_emu_video_offset_x = 1.0;
double fs_emu_video_offset_y = 1.0;

static void render_frame(double alpha, int perspective) {
#ifdef WITH_LUA
    fs_emu_lua_run_handler("on_fs_emu_render_frame");
#endif

#ifdef WITH_LUA

#if 0
    fs_emu_acquire_lua();
    lua_getglobal(fs_emu_lua_state, "on_fs_emu_render_frame");
    if (lua_isnil(fs_emu_lua_state, -1)) {
        lua_pop(fs_emu_lua_state, 1);
    }
    else if (lua_pcall(fs_emu_lua_state, 0, 0, 0) != 0) {
        fs_emu_lua_log_error("error calling on_render_frame");
        lua_pop(fs_emu_lua_state, 1);
    }
    //else if (!lua_isnumber(fs_emu_lua_state, -1)) {
    //    fs_log("fs_emu_on_render_frame must return a number\n");
    //}
    else {
    //    //int handled = lua_tonumber(fs_emu_lua_state, -1);
    //    //lua_pop(fs_emu_lua_state, 1);
    //    //if (handled) {
    //    //    return;
    //    //}
        //fs_emu_release_lua();
        //return;
    }
    fs_emu_release_lua();
#endif

#endif

    int fox = g_fs_emu_theme.display_x;
    int foy = g_fs_emu_theme.display_y;
    int fow = g_fs_emu_theme.display_w;
    int foh = g_fs_emu_theme.display_h;
    int frame_override = fow != g_fs_emu_theme.width ||
            foh != g_fs_emu_theme.height;
    if (g_frame_override) {
        fox = g_frame_override_x;
        foy = g_frame_override_y;
        fow = g_frame_override_w;
        foh = g_frame_override_h;
        frame_override = 1;
    }

    if (frame_override) {
        float color = 1.0;
        if (g_frame_texture == 0) {
            // texture has not been created yet
            color = 0.0;
            fs_gl_texturing(0);
        }
        else {
            fs_gl_texturing(1);
            fs_gl_bind_texture(g_frame_texture);
        }
        fs_gl_blending(0);
        fs_gl_color4f(color, color, color, 1.0);

        float x1 = -1.0 + fox * 2.0 / g_l_scale_x;
        float x2 = -1.0 + (fox + fow) * 2.0 / g_l_scale_x;
        float y2 = 1.0 - foy * 2.0 / g_l_scale_y;
        float y1 = 1.0 - (foy + foh) * 2.0 / g_l_scale_y;

        double s1 = 0.0;
        double t1 = 0.0;
        double s2 = (double) g_crop.w / g_frame_texture_width;
        double t2 = (double) g_crop.h / g_frame_texture_height;

        int shader_result = 0;
#ifdef WITH_XML_SHADER
        if (g_frame_texture) {
            // only try to render with shader passes if we have a valid texture

            int screen_w = fs_ml_video_width();
            int screen_h = fs_ml_video_height();
            double doutput_w = screen_w;
            double doutput_h = screen_h;
            doutput_w = doutput_w * g_frame_override_w / (double) g_l_scale_x;
            doutput_h = doutput_h * g_frame_override_h / (double) g_l_scale_y;
            int output_w = doutput_w + 0.5;
            int output_h = doutput_h + 0.5;

            shader_result = fs_emu_xml_shader_render(g_frame_texture,
                    g_frame_texture_width, g_frame_texture_height,
                    g_crop.w, g_crop.h, output_w, output_h,
                    x1, y1, x2, y2, 0, 1.0);
        }
#endif
        if (!shader_result) {
            render_quad(x1, y1, x2, y2, s1, t1, s2, t2);
        }

        render_overlays();
        return;
    }

    int input_w = g_crop.w;
    int input_h = g_crop.h;
    double emu_aspect = (double) input_w / (double) input_h;
    emu_aspect *= g_frame_aspect;

    int screen_w = fs_ml_video_width();
    int screen_h = fs_ml_video_height();
    double screen_aspect = (double) screen_w / (double) screen_h;

    double doutput_w = screen_w;
    double doutput_h = screen_h;

    if (fs_emu_video_get_aspect_correction()) {
        if (emu_aspect > screen_aspect) {
            // emu video is wider than screen, typical for 4:3 or 5:4 monitors
            double h = screen_aspect / emu_aspect;
            doutput_h *= h;
        }
        else {
            // typical scenario for wide-screen monitors
            double w = emu_aspect / screen_aspect;
            doutput_w *= w;
        }
    }

    if (g_scale_x < 0.0) {
        doutput_w *= -g_scale_x;
    }
    else {
        doutput_w = input_w * g_scale_x;
    }
    if (g_scale_y < 0.0) {
        doutput_h *= -g_scale_y;
    }
    else {
        doutput_h = input_h * g_scale_y;
    }

    // round to nearest integer
    int output_w = doutput_w + 0.5;
    int output_h = doutput_h + 0.5;
    //printf("output size: %d %d\n", output_w, output_h);

    int offset_x = (screen_w - output_w) * g_align_x;
    int offset_y = (screen_h - output_h) * (1.0 - g_align_y);

    //printf("w %d h %d x %d y %d\n", output_w, output_h, offset_x, offset_y);

    fs_emu_video_scale_x = (double) input_w / output_w;
    fs_emu_video_scale_y = (double) input_h / output_h;

    fs_emu_video_offset_x = offset_x - g_crop.x / fs_emu_video_scale_x;
    fs_emu_video_offset_y = offset_y - g_crop.y / fs_emu_video_scale_y;

    double s1 = 0.0;
    double t1 = 0.0;
    double s2 = (double) g_crop.w / g_frame_texture_width;
    double t2 = (double) g_crop.h / g_frame_texture_height;

    double x1 = -1.0 + 2.0 * (double) offset_x / screen_w;
    double y1 = -1.0 + 2.0 * (double) offset_y / screen_h;
    double x2 = -1.0 + 2.0 * (double) (offset_x + output_w) / screen_w;
    double y2 = -1.0 + 2.0 * (double) (offset_y + output_h) / screen_h;

#if 0
    double emu_aspect;
    if (g_fs_emu_video_crop_mode) {
        //if (g_viewport_mode == FS_EMU_VIEWPORT_MODE_CROP) {
        /*
        s1 = (double) g_crop.x / g_frame_texture_width;
        s2 = (double) (g_crop.x + g_crop.w) / g_frame_texture_width;
        t1 = (double) g_crop.y / g_frame_texture_height;
        t2 = (double) (g_crop.y + g_crop.h) / g_frame_texture_height;
        */

        s1 = 0.0;
        s2 = (double) (g_crop.w) / g_frame_texture_width;
        t1 = 0.0;
        t2 = (double) (g_crop.h) / g_frame_texture_height;

        emu_aspect = (double) g_crop.w / (double) g_crop.h;
    }
    else {
        s1 = 0.0;
        s2 = (double) g_frame_width / g_frame_texture_width;
        t1 = 0.0;
        t2 = (double)  g_frame_height / g_frame_texture_height;
        emu_aspect = (double) g_frame_width / (double) g_frame_height;
    }

    emu_aspect *= g_frame_aspect;

    double x1 = -1.0;
    double x2 =  1.0;
    double y1 = -1.0;
    double y2 =  1.0;

#endif

    double repeat_right_border = 0;
    //int repeat_bottom_border = 0;

#if 0
    if (fs_emu_video_get_aspect_correction()) {
        double screen_aspect = (double) fs_ml_video_width() /
                (double) fs_ml_video_height();
        if (emu_aspect > screen_aspect) {
            // emu video is wider than screen
            double h = screen_aspect / emu_aspect;
            //double padding = (2.0 - 2.0 * h) / 2.0;
            double padding = 1.0 - h;
            if (g_effective_viewport_mode == FS_EMU_VIEWPORT_MODE_CROP) {
                y1 += padding;
                y2 -= padding;
            }
            else { // FS_EMU_VIEWPORT_MODE_CENTER
                t1 -= padding / 2.0;
                t2 += padding / 2.0;
                //y2 -= padding;
            }
        }
        else {
            double w = emu_aspect / screen_aspect;
            //double padding = (2.0 - 2.0 * w) / 2.0;
            double padding = 1.0 - w;
            if (g_effective_viewport_mode == FS_EMU_VIEWPORT_MODE_CROP) {
                x1 += padding;
                x2 -= padding;
            }
            else { // FS_EMU_VIEWPORT_MODE_CENTER
                //s1 -= padding / 4.0;

                // FIXME: THIS IS WRONG
                s1 -= padding / 2.0;
                //s2 += padding / 2.0;
                x2 -= padding;
                //repeat_right_border = x2;
                repeat_right_border = 1.0 - padding;
            }
        }
    }
#endif

    // if video is not stretched, we render black rectangles to cover
    // the rest of the screen
    if (x1 > -1.0 || x2 < 1.0 || y1 > -1.0 || y2 < 1.0) {
        fs_gl_texturing(0);
        if (alpha < 1.0) {
            fs_gl_blending(1);
            fs_gl_color4f(0.0, 0.0, 0.0, alpha);
        }
        else {
            fs_gl_blending(0);
            fs_gl_color4f(0.0, 0.0, 0.0, 1.0);
        }

        if (x1 > -1.0) {
#ifdef USE_GLES
            GLfloat vert[] = {
                   -1.0, -1.0,
                   x1, -1.0,
                   x1,  1.0,
                   -1.0, 1.0
            };
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 0, vert);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glDisableClientState(GL_VERTEX_ARRAY);
#else
            glBegin(GL_QUADS);
            glVertex2f(-1.0, -1.0);
            glVertex2f(  x1, -1.0);
            glVertex2f(  x1,  1.0);
            glVertex2f(-1.0,  1.0);
            glEnd();
#endif
            CHECK_GL_ERROR();
        }
        if (x2 < 1.0) {
#ifdef USE_GLES
            GLfloat vert[] = {
                    x2, -1.0,
                   1.0, -1.0,
                   1.0,  1.0,
                    x2,  1.0
            };
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 0, vert);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glDisableClientState(GL_VERTEX_ARRAY);
#else
            glBegin(GL_QUADS);
            glVertex2f(  x2, -1.0);
            glVertex2f( 1.0, -1.0);
            glVertex2f( 1.0,  1.0);
            glVertex2f(  x2,  1.0);
            glEnd();
#endif
            CHECK_GL_ERROR();
        }
        if (y1 > -1.0) {
#ifdef USE_GLES
            GLfloat vert[] = {
                   x1, -1.0,
                   x2, -1.0,
                   x2,   y1,
                   x1,   y1
            };
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 0, vert);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glDisableClientState(GL_VERTEX_ARRAY);
#else
            glBegin(GL_QUADS);
            glVertex2f(x1, -1.0);
            glVertex2f(x2, -1.0);
            glVertex2f(x2,   y1);
            glVertex2f(x1,   y1);
            glEnd();
#endif
            // left side (3D)
#ifdef USE_GLES
            GLfloat vert2[] = {
                   -1.0, -1.0, -0.1,
                   -1.0, -1.0,  0.0,
                   -1.0,   y1,  0.0,
                   -1.0,   y1, -0.1
            };
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, vert2);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);            
            glDisableClientState(GL_VERTEX_ARRAY);
#else
            glBegin(GL_QUADS);
            glVertex3f(-1.0, -1.0, -0.1);
            glVertex3f(-1.0, -1.0,  0.0);
            glVertex3f(-1.0,   y1,  0.0);
            glVertex3f(-1.0,   y1, -0.1);
            glEnd();
#endif
            CHECK_GL_ERROR();
        }
        if (y2 < 1.0) {
#ifdef USE_GLES
            GLfloat vert[] = {
                x1, y2,
                x2, y2,
                x2, 1.0,
                x1, 1.0
            };
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 0, vert);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);            
            glDisableClientState(GL_VERTEX_ARRAY);
#else
            glBegin(GL_QUADS);
            glVertex2f(x1,   y2);
            glVertex2f(x2,   y2);
            glVertex2f(x2,  1.0);
            glVertex2f(x1,  1.0);
            glEnd();
#endif
            // left side (3D)
#ifdef USE_GLES
            GLfloat vert2[] = {
                   -1.0,   y2, -0.1,
                   -1.0,   y2,  0.0,
                   -1.0,  1.0,  0.0,
                   -1.0,  1.0, -0.1
            };
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, vert2);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);            
            glDisableClientState(GL_VERTEX_ARRAY);
#else
            glBegin(GL_QUADS);
            glVertex3f(-1.0,  y2, -0.1);
            glVertex3f(-1.0,  y2,  0.0);
            glVertex3f(-1.0, 1.0,  0.0);
            glVertex3f(-1.0, 1.0, -0.1);
            glEnd();
#endif
            CHECK_GL_ERROR();
        }
    }
    //printf("--- render frame done ---\n");

    int render_textured_side = 0;
    if (perspective) {
        // render left side in 3d mode
        fs_gl_blending(0);
        if (x1 > -1.0) {
            // emu screen does not reach screen edge - side will be black
            fs_gl_texturing(0);
            fs_gl_color4f(0.0, 0.0, 0.0, alpha);
        }
        /*
        else if (fs_emu_using_shader()) {
            // for simplicity, render black side when using shaders (the
            // problem is that shaders can render directly to the framebuffer,
            // and not necessarily to a texture. This can be fixed, but
            // side rendering should be copied to fs_emu_render_with_shader
            // in this case, so the side can be rendered with the shader
            // too...
            //fs_gl_texturing(0);
            //fs_gl_color4f(0.0, 0.0, 0.0, alpha);
        }
        */
        else {
            render_textured_side = 1;
            fs_gl_texturing(1);
            fs_gl_bind_texture(g_frame_texture);
            fs_gl_color4f(0.33 * alpha, 0.33 * alpha, 0.33 * alpha, alpha);
        }

        if (render_textured_side == 0
#ifdef WITH_XML_SHADER
                || !fs_emu_xml_shader_is_enabled()
#endif
        ) {
#ifdef USE_GLES
            GLfloat tex[] = {
                s1, t2,
                s1, t2,
                s1, t1,
                s1, t1
            };
            GLfloat vert[] = {
                -1.0, y1, -0.1,
                -1.0, y1, 0.0,
                -1.0, y2, 0.0,
                -1.0, y2, -0.1
            };
    
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
            glVertexPointer(3, GL_FLOAT, 0, vert);
            glTexCoordPointer(2, GL_FLOAT, 0, tex);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);        
#else
            glBegin(GL_QUADS);
            glTexCoord2d(s1, t2); glVertex3f(-1.0, y1, -0.1);
            glTexCoord2d(s1, t2); glVertex3f(-1.0, y1,  0.0);
            glTexCoord2d(s1, t1); glVertex3f(-1.0, y2,  0.0);
            glTexCoord2d(s1, t1); glVertex3f(-1.0, y2, -0.1);
            glEnd();
#endif
            CHECK_GL_ERROR();
        }
    }

    float color = 1.0;
    if (g_frame_texture == 0) {
        // texture has not been created yet
        color = 0.0;
        fs_gl_texturing(0);
    }
    else {
        fs_gl_texturing(1);
        fs_gl_bind_texture(g_frame_texture);
    }
    if (alpha < 1.0) {
        fs_gl_blending(1);
        fs_gl_color4f(color * alpha, color * alpha, color * alpha, alpha);
    }
    else {
        fs_gl_blending(0);
        fs_gl_color4f(color, color, color, 1.0);
    }

    int shader_result = 0;
#ifdef WITH_XML_SHADER
    if (g_frame_texture) {
        // only try to render with shader passes if we have a valid texture
        shader_result = fs_emu_xml_shader_render(g_frame_texture,
                g_frame_texture_width, g_frame_texture_height,
                g_crop.w, g_crop.h, output_w, output_h,
                x1, y1, x2, y2, render_textured_side, alpha);
    }
#endif
    if (!shader_result) {
        render_quad(x1, y1, x2, y2, s1, t1, s2, t2);
    }

    //repeat_right_border = 0;
    if (repeat_right_border > 0.0) {
        s1 = s2 = (double) (g_frame_width - 1) / g_frame_texture_width;

#ifdef USE_GLES
        GLfloat tex[] = {
            s1, t2,
            s2, t2,
            s2, t1,
            s1, t1
        };
        GLfloat vert[] = {
            repeat_right_border, y1,
            1.0, y1,
            1.0, y2,
            repeat_right_border, y2
        };
    
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
        glVertexPointer(2, GL_FLOAT, 0, vert);
        glTexCoordPointer(2, GL_FLOAT, 0, tex);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);        
#else
        glBegin(GL_QUADS);
        glTexCoord2d(s1, t2); glVertex2f(repeat_right_border, y1);
        glTexCoord2d(s2, t2); glVertex2f(1.0, y1);
        glTexCoord2d(s2, t1); glVertex2f(1.0, y2);
        glTexCoord2d(s1, t1); glVertex2f(repeat_right_border, y2);
        glEnd();
#endif
        CHECK_GL_ERROR();

        // so the following code does not render black rectangle over
        // the right border
        x2 = 1.0;
    }

    if (g_fs_emu_overlay_texture) {
        fs_gl_blending(1);
        fs_gl_texturing(1);
        fs_gl_color4f(alpha, alpha, alpha, alpha);
        //fs_gl_bind_texture(g_fs_emu_overlay_texture->texture);
        fs_emu_set_texture(g_fs_emu_overlay_texture);
        
#ifdef USE_GLES
        GLfloat tex[] = {
            0.0, 1.0,
            1.0, 1.0,
            1.0, 0.0,
            0.0, 0.0
        };
        GLfloat vert[] = {
            -1.0, -1.0,
             1.0, -1.0,
             1.0, 1.0,
            -1.0, 1.0
        };
    
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
        glVertexPointer(2, GL_FLOAT, 0, vert);
        glTexCoordPointer(2, GL_FLOAT, 0, tex);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);        
#else        
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1.0); glVertex2f(-1.0, -1.0);
        glTexCoord2f(1.0, 1.0); glVertex2f( 1.0, -1.0);
        glTexCoord2f(1.0, 0.0); glVertex2f( 1.0,  1.0);
        glTexCoord2f(0.0, 0.0); glVertex2f(-1.0,  1.0);
        glEnd();
#endif
    }
    render_overlays();
}

static void render_glow(double opacity) {
    //printf("--- render glow ---\n");
    float tx1, ty1, tx2, ty2;
    //const double dx = 0.1 * 9.0 / 16.0;
    const double dx = 0.15;
    const double dy = 0.15 * 16.0 / 9.0;
    const double z = 0.0;
    const double s = 0.65 * opacity;
    fs_gl_color4f(s, s, s, s);
    fs_gl_blending(1);
    //fs_emu_set_texture(g_tex_glow_top);
    fs_emu_prepare_texture(TEXTURE_GLOW_TOP, &tx1, &ty1, &tx2, &ty2);
    // render top edge
#ifdef USE_GLES
    GLfloat tex[] = {
        tx1, ty2,
        tx2, ty2,
        tx2, ty1,
        tx1, ty1
    };
    GLfloat vert[] = {
        -1.0 + dx, 1.0 - dy, z,
        1.0 - dx, 1.0 - dy, z,
        1.0 - dx, 1.0 + dy, z,
        -1.0 + dx, 1.0 + dy, z
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vert);
    glTexCoordPointer(2, GL_FLOAT, 0, tex);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#else
    glBegin(GL_QUADS);
    glTexCoord2f(tx1, ty2); glVertex3f(-1.0 + dx, 1.0 - dy, z);
    glTexCoord2f(tx2, ty2); glVertex3f( 1.0 - dx, 1.0 - dy, z);
    glTexCoord2f(tx2, ty1); glVertex3f( 1.0 - dx, 1.0 + dy, z);
    glTexCoord2f(tx1, ty1); glVertex3f(-1.0 + dx, 1.0 + dy, z);
    glEnd();
#endif
    CHECK_GL_ERROR();
    // render corners
    fs_emu_prepare_texture(TEXTURE_GLOW_TOP_LEFT, &tx1, &ty1, &tx2, &ty2);
#ifdef USE_GLES
    GLfloat tex2[] = {
        // top left corner
        tx1, ty2,
        tx2, ty2,
        tx2, ty1,
        tx1, ty1,
        // top right corner
        tx2, ty2,
        tx1, ty2,
        tx1, ty1,
        tx2, ty1
    };
    GLfloat vert2[] = {
        // top left corner
        -1.0 - dx, 1.0 - dy, z,
        -1.0 + dx, 1.0 - dy, z,
        -1.0 + dx, 1.0 + dy, z,
        -1.0 - dx, 1.0 + dy, z,
        // top right corner
        1.0 - dx, 1.0 - dy, z,
        1.0 + dx, 1.0 - dy, z,
        1.0 + dx, 1.0 + dy, z,
        1.0 - dx, 1.0 + dy, z
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vert2);
    glTexCoordPointer(2, GL_FLOAT, 0, tex2);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#else
    glBegin(GL_QUADS);
    // top left corner
    glTexCoord2f(tx1, ty2); glVertex3f(-1.0 - dx, 1.0 - dy, z);
    glTexCoord2f(tx2, ty2); glVertex3f(-1.0 + dx, 1.0 - dy, z);
    glTexCoord2f(tx2, ty1); glVertex3f(-1.0 + dx, 1.0 + dy, z);
    glTexCoord2f(tx1, ty1); glVertex3f(-1.0 - dx, 1.0 + dy, z);
    // top right corner
    glTexCoord2f(tx2, ty2); glVertex3f( 1.0 - dx, 1.0 - dy, z);
    glTexCoord2f(tx1, ty2); glVertex3f( 1.0 + dx, 1.0 - dy, z);
    glTexCoord2f(tx1, ty1); glVertex3f( 1.0 + dx, 1.0 + dy, z);
    glTexCoord2f(tx2, ty1); glVertex3f( 1.0 - dx, 1.0 + dy, z);
    glEnd();
#endif
    CHECK_GL_ERROR();
    // render left and right edge
    fs_emu_prepare_texture(TEXTURE_GLOW_LEFT, &tx1, &ty1, &tx2, &ty2);
    //fs_emu_set_texture(g_tex_glow_left);
#ifdef USE_GLES
    GLfloat color3[] = {
        // left edge
        s, s, s, s,
        s, s, s, s,
        s, s, s, s,
        s, s, s, s,
        // right edge
        s, s, s, s,
        s, s, s, s,
        s, s, s, s,
        s, s, s, s,
        // left edge bottom
        0, 0, 0, 0,
        0, 0, 0, 0,
        s, s, s, s,
        s, s, s, s,
        // right edge bottom
        0, 0, 0, 0,
        0, 0, 0, 0,
        s, s, s, s,
        s, s, s, s,
    };
    GLfloat tex3[] = {
        // left edge
        tx1, ty2,
        tx2, ty2,
        tx2, ty1,
        tx1, ty1,
        // right edge
        tx2, ty2,
        tx1, ty2,
        tx1, ty1,
        tx2, ty1,
        // left edge bottom
        tx1, ty2,
        tx2, ty2,
        tx2, ty1,
        tx1, ty1,
        // right edge bottom
        tx2, ty2,
        tx1, ty2,
        tx1, ty1,
        tx2, ty1
    };
    GLfloat vert3[] = {
        // left edge
        -1.0 - dx, -0.5, z,
        -1.0 + dx, -0.5, z,
        -1.0 + dx, 1.0 - dy, z,
        -1.0 - dx, 1.0 - dy, z,
        // right edge
        1.0 - dx, -0.5, z,
        1.0 + dx, -0.5, z,
        1.0 + dx, 1.0 - dy, z,
        1.0 - dx, 1.0 - dy, z,
        // left edge bottom
        -1.0 - dx, -1.0, z,
        -1.0 + dx, -1.0, z,
        -1.0 + dx, -0.5, z,
        -1.0 - dx, -0.5, z,
        // right edge bottom
        1.0 - dx, -1.0, z,
        1.0 + dx, -1.0, z,
        1.0 + dx, -0.5, z,
        1.0 - dx, -0.5, z
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glColorPointer(4, GL_FLOAT, 0, color3);
    glVertexPointer(3, GL_FLOAT, 0, vert3);
    glTexCoordPointer(2, GL_FLOAT, 0, tex3);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
#else
    glBegin(GL_QUADS);
    // left edge
    glTexCoord2f(tx1, ty2); glVertex3f(-1.0 - dx, -0.5, z);
    glTexCoord2f(tx2, ty2); glVertex3f(-1.0 + dx, -0.5, z);
    glTexCoord2f(tx2, ty1); glVertex3f(-1.0 + dx, 1.0 - dy, z);
    glTexCoord2f(tx1, ty1); glVertex3f(-1.0 - dx, 1.0 - dy, z);
    // right edge
    glTexCoord2f(tx2, ty2); glVertex3f( 1.0 - dx, -0.5, z);
    glTexCoord2f(tx1, ty2); glVertex3f( 1.0 + dx, -0.5, z);
    glTexCoord2f(tx1, ty1); glVertex3f( 1.0 + dx, 1.0 - dy, z);
    glTexCoord2f(tx2, ty1); glVertex3f( 1.0 - dx, 1.0 - dy, z);
    // left edge bottom
    fs_gl_color4f(0.0, 0.0, 0.0, 0.0);
    glTexCoord2f(tx1, ty2); glVertex3f(-1.0 - dx, -1.0, z);
    glTexCoord2f(tx2, ty2); glVertex3f(-1.0 + dx, -1.0, z);
    fs_gl_color4f(s, s, s, s);
    glTexCoord2f(tx2, ty1); glVertex3f(-1.0 + dx, -0.5, z);
    glTexCoord2f(tx1, ty1); glVertex3f(-1.0 - dx, -0.5, z);
    // right edge bottom
    fs_gl_color4f(0.0, 0.0, 0.0, 0.0);
    glTexCoord2f(tx2, ty2); glVertex3f( 1.0 - dx, -1.0, z);
    glTexCoord2f(tx1, ty2); glVertex3f( 1.0 + dx, -1.0, z);
    fs_gl_color4f(s, s, s, s);
    glTexCoord2f(tx1, ty1); glVertex3f( 1.0 + dx, -0.5, z);
    glTexCoord2f(tx2, ty1); glVertex3f( 1.0 - dx, -0.5, z);
    glEnd();
#endif
    CHECK_GL_ERROR();
    //printf("--- render glow done ---\n");
}

static void render_fade_overlay(double alpha) {
    // draw fading effect
    fs_gl_viewport(0, 0, fs_ml_video_width(), fs_ml_video_height());
    fs_gl_ortho_hd();
    fs_gl_blending(1);
    fs_gl_texturing(0);
    fs_gl_color4f(g_fs_emu_theme.fade_color[0] * alpha,
            g_fs_emu_theme.fade_color[1] * alpha,
            g_fs_emu_theme.fade_color[2] * alpha, alpha);

#ifdef USE_GLES
    GLfloat vert[] = {
        0, 0,
        1920, 0,
        1920, 1080,
        0, 1080
    };

    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, vert);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
#else
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(1920, 0);
    glVertex2f(1920, 1080);
    glVertex2f(0, 1080);
    glEnd();
#endif

    CHECK_GL_ERROR();
}

/**
 * This function is called at the end of the frame rendering function
 */
static void handle_quit_sequence() {
    int fade_time = fs_config_get_int_clamped("fade_out_duration", 0, 10000);
    if (fade_time == FS_CONFIG_NONE) {
        // fade out over 250ms
        fade_time = 250;
    }
    fade_time = fade_time * 1000;

    int64_t dt = fs_emu_monotonic_time() - g_fs_emu_quit_time;
    if (dt > fade_time && g_fs_emu_emulation_thread_stopped) {
        fs_emu_log("calling fs_ml_stop because emu thread is done\n");
        fs_ml_stop();

    }
    else if (dt > 5 * 1000 * 1000) {
        // 5 seconds has passed after shutdown was requested
        fs_emu_log("calling fs_ml_stop because emu does not stop\n");
        // FIXME: FORCE STOP
        fs_ml_stop();
        fs_emu_log("force-closing the emulator\n");
        exit(1);
    }


    if (fade_time > 0) {
        float fade = (1.0 * dt) / fade_time;
        if (fade > 1.0) {
            fade = 1.0;
        }
        render_fade_overlay(fade);
    }
}

int fs_emu_video_update_function() {
    return update_texture();
}

void fs_emu_video_render_function() {
    static int initialized_menu = 0;
    if (!initialized_menu) {
        // render menu once (without really showing it, so all menu
        // resources are initialized and loaded, -prevents flickering
        // when really opening the menu later
        fs_emu_menu_render(g_menu_transition);
        initialized_menu = 1;
    }

    if (g_fs_emu_video_debug) {
        int quarter_height = fs_ml_video_height() / 4;
        fs_gl_viewport(0, quarter_height, fs_ml_video_width(),
                fs_ml_video_height() - quarter_height);
    }
    else {
        fs_gl_viewport(0, 0, fs_ml_video_width(), fs_ml_video_height());
    }

    // FIXME: can perhaps remove this soon..
    fs_emu_video_render_mutex_lock();


    int in_menu = fs_emu_menu_mode();
    if (in_menu && g_menu_transition_target < 1.0) {
        g_menu_transition_target = 1.0;
    }
    if (!in_menu && g_menu_transition_target > 0.0) {
        g_menu_transition_target = 0.0;
    }

    float menu_transition_speed = 0.10;
    int allow_perspective = 1;

#ifdef WITH_LUA
    //fs_emu_acquire_lua();
    //lua_getglobal(fs_emu_lua_state, "on_render_frame");
    //if (lua_isnil(fs_emu_lua_state, -1)) {
    //    lua_pop(fs_emu_lua_state, 1);
    //}

    if (g_frame_override) {
        // don't allow perspective with the new scripted rendering system
        allow_perspective = 0;
        menu_transition_speed = 1.0 / 7.0;
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    //fs_emu_release_lua();
#endif

    // FIXME: ideally, we would use time-based animation - for now, we use a
    // simple frame-based animation
    if (g_menu_transition < g_menu_transition_target) {
        if (g_menu_transition_target == 1.0) {
            g_menu_transition += menu_transition_speed;
        }
    }
    if (g_menu_transition > g_menu_transition_target) {
        if (g_menu_transition_target == 0.0) {
            g_menu_transition -= menu_transition_speed;
        }
    }
    if (g_menu_transition > 1.0) {
        g_menu_transition = 1.0;
    }
    else if (g_menu_transition < 0.0) {
        g_menu_transition = 0.0;
    }

    int matrix_pushed = 0;

    double t0_x = 0.0;
    double t0_y = 0.0;
    double t0_z = -2.42;
    double r0_a = 0.0;

    double t1_x = -0.31;
    //double t1_y = -0.04;
    double t1_y = 0.0;
    double t1_z = -3.7;
    double r1_a = 30.0;

    int perspective = 0;
    if (g_menu_transition == 0.0 || allow_perspective == 0) {
        perspective = 0;
        fs_gl_ortho();
        //glTranslated(1920.0 / 2.0, 1080.0 / 2.0, 0.0);
        //glScaled(1920.0 / 2.0, 1080.0 / 2.0, 1.0);
    }
    else {
        perspective = 1;

        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_DEPTH_BUFFER_BIT);
        CHECK_GL_ERROR();
        fs_gl_ortho_hd();

        // transition y-coordinate between floor and wall
        int splt = g_fs_emu_theme.floor_height;
        fs_gl_blending(FALSE);
        fs_gl_texturing(FALSE);

#ifdef USE_GLES
        GLfloat vert[] = {
            // q1
            0, splt, -0.9,
            1920, splt, -0.9,
            1920, 1020, -0.9,
            0, 1020, -0.9,
            // q2
            0, 1020, -0.9,
            1920, 1020, -0.9,
            1920, 1080, -0.9,
            0, 1080, -0.9,
            // q3
            0, 0, -0.9,
            1920, 0, -0.9,
            1920, splt, -0.9,
            0, splt, -0.9
        };
        GLfloat *wc1 = g_fs_emu_theme.wall_color_1;
        GLfloat *wc2 = g_fs_emu_theme.wall_color_2;
        GLfloat color[] = {
            // q1
            wc2[0], wc2[1], wc2[2], wc2[3],
            wc2[0], wc2[1], wc2[2], wc2[3],
            wc1[0], wc1[1], wc1[2], wc1[3],
            wc1[0], wc1[1], wc1[2], wc1[3],
            // q2
            wc1[0], wc1[1], wc1[2], wc1[3],
            wc1[0], wc1[1], wc1[2], wc1[3],
            wc1[0], wc1[1], wc1[2], wc1[3],
            wc1[0], wc1[1], wc1[2], wc1[3],
            // q3
            wc2[0], wc2[1], wc2[2], wc2[3],
            wc2[0], wc2[1], wc2[2], wc2[3],
            wc1[0], wc1[1], wc1[2], wc1[3],
            wc1[0], wc1[1], wc1[2], wc1[3],    
        };
    
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
    
        glVertexPointer(3, GL_FLOAT, 0, vert);
        glColorPointer(4, GL_FLOAT, 0, color);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
        glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
    
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
#else
        glBegin(GL_QUADS);

        fs_gl_color4fv(g_fs_emu_theme.wall_color_2);
        glVertex3f(   0, splt, -0.9);
        glVertex3f(1920, splt, -0.9);
        fs_gl_color4fv(g_fs_emu_theme.wall_color_1);
        glVertex3f(1920, 1020, -0.9);
        glVertex3f(   0, 1020, -0.9);

        glVertex3f(   0, 1020, -0.9);
        glVertex3f(1920, 1020, -0.9);
        //fs_gl_color4f(0.0, 0.0, 0.0, 1.0);
        glVertex3f(1920, 1080, -0.9);
        glVertex3f(   0, 1080, -0.9);

        fs_gl_color4fv(g_fs_emu_theme.floor_color_2);
        glVertex3f(   0,    0, -0.9);
        glVertex3f(1920,    0, -0.9);
        fs_gl_color4fv(g_fs_emu_theme.floor_color_1);
        glVertex3f(1920, splt, -0.9);
        glVertex3f(   0, splt, -0.9);

        glEnd();
#endif
        CHECK_GL_ERROR();

        fs_gl_perspective();
        double t_x = t0_x + (t1_x - t0_x) * g_menu_transition;
        double t_y = t0_y + (t1_y - t0_y) * g_menu_transition;
        double t_z = t0_z + (t1_z - t0_z) * g_menu_transition;
        double r_a = r0_a + (r1_a - r0_a) * g_menu_transition;

        glPushMatrix();
        matrix_pushed = 1;

        glScaled(16.0 / 9.0, 1.0, 1.0);
        glTranslated(t_x, t_y, t_z);
        glRotated(r_a, 0.0, 1.0, 0.0);
        CHECK_GL_ERROR();
    }

    if (perspective) {
        render_glow(g_menu_transition);
    }
    if (perspective) {
        glPushMatrix();
        glTranslatef(0.0, -2.0, 0.0);
        //glTranslatef(0.0, -1.0, 0.0);
        //glScalef(1.0, -1.0, 1.0);
        glScalef(1.0, -0.5, 1.0);
        glTranslatef(0.0, -1.0, 0.0);
        CHECK_GL_ERROR();
        render_frame(0.33, perspective);
        CHECK_GL_ERROR();
        render_gloss(g_menu_transition * 0.66);
        CHECK_GL_ERROR();
        glPopMatrix();
        CHECK_GL_ERROR();
    }
    render_frame(1.0, perspective);
    if (perspective) {
        render_gloss(g_menu_transition);
    }
#if 0
    if (fs_emu_is_paused()) {
        fs_gl_blending(1);
        fs_gl_texturing(1);
        fs_emu_draw_texture_with_size(TEXTURE_PAUSE, 0.0, 0.0, 0.1, 0.1);
    }
#endif

    if (matrix_pushed) {
        glPopMatrix();
        CHECK_GL_ERROR();
        matrix_pushed = 0;
    }

    fs_emu_acquire_gui_lock();
    fs_emu_hud_render_chat();

    //if (fs_emu_menu_is_active()) {
    if (g_menu_transition > 0.0) {
        fs_emu_menu_render(g_menu_transition);
    }

    fs_emu_dialog_render();
    fs_emu_release_gui_lock();

#ifdef WITH_NETPLAY
    if (g_fs_emu_hud_mode && fs_emu_netplay_enabled()) {
        fs_gl_ortho_hd();
        fs_gl_texturing(0);
        fs_gl_blending(1);
        fs_gl_color4f(0.0, 0.0, 0.0, 0.5);
#ifdef USE_GLES
        GLfloat vert[] = {
            0, 1030,
            1920, 1030,
            1920, 1080,
            0, 1080
        };
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vert);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
#else
        glBegin(GL_QUADS);
        glVertex2f(0, 1030);
        glVertex2f(1920, 1030);
        glVertex2f(1920, 1080);
        glVertex2f(0, 1080);
        glEnd();
#endif
        CHECK_GL_ERROR();
#if 0
        glBegin(GL_QUADS);
        glVertex2f(0, 1030);
        glVertex2f(1920, 1030);
        fs_gl_color4f(0.0, 0.0, 0.0, 0.0);
        glVertex2f(1920, 1030 - 50);
        glVertex2f(0, 1030 - 50);
        glEnd();
#endif
        fs_emu_font *menu_font = fs_emu_font_get_menu();
        char *str;

        for (int i = 0; i < MAX_PLAYERS; i++) {
            fs_emu_player *player = g_fs_emu_players + i;
            int x = i * 1920 / 6 + 20;
            int y = 1038;

            int rendered_tag = 0;
            if (player->tag[0]) {
                str = g_strdup_printf("%s", player->tag);
                fs_emu_font_render(menu_font, str, x, y,
                        1.0, 1.0, 1.0, 1.0);
                free(str);
                rendered_tag = 1;
            }
            if (rendered_tag || player->ping) {
                str = g_strdup_printf("%03d", player->ping);
                fs_emu_font_render(menu_font, str, x + 100, y,
                        1.0, 1.0, 1.0, 1.0);
                free(str);
            }
            /*
            if (rendered_tag || player->lag) {
                str = g_strdup_printf("%03d", player->lag);
                fs_emu_font_render(menu_font, str, x + 200, y,
                        1.0, 1.0, 1.0, 1.0);
                free(str);
            }
            */
        }
    }
#endif

#if 1
    if (fs_emu_is_paused()) {
        fs_gl_ortho_hd();
        fs_gl_blending(1);
        fs_gl_texturing(1);
        fs_emu_draw_texture(TEXTURE_PAUSE, 160, 1080 - 160 - 64);
    }
#endif

    if (g_fs_emu_video_debug) {
        int quarter_height = fs_ml_video_height() / 4;
        fs_gl_viewport(0, 0, fs_ml_video_width(), quarter_height);
        CHECK_GL_ERROR();

        fs_emu_set_texture(NULL);
        CHECK_GL_ERROR();
        static GLuint debug_texture = 0;
        static uint32_t *debug_texture_data = NULL;
        if (debug_texture == 0) {
            debug_texture_data = g_malloc0(256 * 256 * 4);
            glGenTextures(1, &debug_texture);
            CHECK_GL_ERROR();
            fs_gl_bind_texture(debug_texture);
            fs_gl_unpack_row_length(0);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0,
                    GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            CHECK_GL_ERROR();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                    GL_CLAMP_TO_EDGE);
            CHECK_GL_ERROR();
        }
        else {
            fs_gl_bind_texture(debug_texture);
            CHECK_GL_ERROR();
        }

        memset(debug_texture_data, 0x00, 256 * 256 * 4);
        CHECK_GL_ERROR();
        fs_emu_video_render_debug_info(debug_texture_data);
        CHECK_GL_ERROR();
        fs_emu_audio_render_debug_info(debug_texture_data);
        CHECK_GL_ERROR();

        fs_gl_unpack_row_length(0);
        CHECK_GL_ERROR();
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 256,
                GL_RGBA, GL_UNSIGNED_BYTE, debug_texture_data);
        CHECK_GL_ERROR();
        fs_gl_ortho_hd();
        fs_gl_texturing(1);
        fs_gl_blending(0);
        fs_gl_color4f(1.0, 1.0, 1.0, 1.0);
#ifdef USE_GLES
        GLfloat tex[] = {
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0
        };
        GLfloat vert[] = {
            0, 0,
            1920, 0,
            1920, 1080,
            0, 1080
        };

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glTexCoordPointer(2, GL_FLOAT, 0, tex);
        glVertexPointer(2, GL_FLOAT, 0, vert);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#else
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex2f(0, 0);
        glTexCoord2f(1.0, 0.0);
        glVertex2f(1920, 0);
        glTexCoord2f(1.0, 1.0);
        glVertex2f(1920, 1080);
        glTexCoord2f(0.0, 1.0);
        glVertex2f(0, 1080);
        glEnd();
#endif
        CHECK_GL_ERROR();

        glPushMatrix();
        glScalef(1.0, 4.0, 1.0);

        fs_emu_font *menu_font = fs_emu_font_get_menu();
        char *str;

        /*
        str = g_strdup_printf("%d", fs_emu_get_audio_frequency());
        fs_emu_font_render(menu_font, str, 1920 / 2 + 20, 3,
                1.0, 1.0, 1.0, 1.0);
        free(str);
        */

        static double avg_emu = 0.0;
        static double avg_sys = 0.0;
        static double avg_fill = 0.0;
        static int avg_throttle = 0;
        if (avg_throttle == 0) {
            avg_emu = fs_emu_get_average_emu_fps();
            avg_sys = fs_emu_get_average_sys_fps();
            avg_fill = fs_emu_audio_get_measured_avg_buffer_fill(0);
            avg_throttle = 10;
        }
        avg_throttle -= 1;

        str = g_strdup_printf("%0.1f", avg_fill / 1000.0);
        fs_emu_font_render(menu_font, str, 1920 / 2 + 220, 3,
                1.0, 1.0, 1.0, 1.0);
        free(str);
        str = g_strdup_printf("%d", g_fs_emu_audio_buffer_underruns);
        fs_emu_font_render(menu_font, str, 1920 / 2 + 420, 3,
                1.0, 1.0, 1.0, 1.0);
        free(str);

        fs_emu_font_render(menu_font, "EMU", 20, 3, 1.0, 1.0, 1.0, 1.0);
        str = g_strdup_printf("%0.2f", avg_emu);
        fs_emu_font_render(menu_font, str, 220, 3, 1.0, 1.0, 1.0, 1.0);
        free(str);
        str = g_strdup_printf("%d", g_fs_emu_lost_frames);
        fs_emu_font_render(menu_font, str, 420, 3, 1.0, 1.0, 1.0, 1.0);
        free(str);
        str = g_strdup_printf("%d", g_fs_emu_repeated_frames);
        fs_emu_font_render(menu_font, str, 620, 3, 1.0, 1.0, 1.0, 1.0);
        free(str);

        fs_emu_font_render(menu_font, "SYS", 20, 140, 1.0, 1.0, 1.0, 1.0);
        str = g_strdup_printf("%0.2f", avg_sys);
        fs_emu_font_render(menu_font, str, 220, 140, 1.0, 1.0, 1.0, 1.0);
        free(str);
        str = g_strdup_printf("%d", g_fs_emu_lost_vblanks);
        fs_emu_font_render(menu_font, str, 420, 140, 1.0, 1.0, 1.0, 1.0);
        free(str);

        glPopMatrix();
        CHECK_GL_ERROR();
    }

    if (fs_emu_is_quitting()) {
        handle_quit_sequence();
    }

    static int64_t start_time = -1;
    if (start_time == -1) {
        start_time = fs_emu_monotonic_time();
    }
    static int fade_time = FS_CONFIG_NONE;
    if (fade_time == FS_CONFIG_NONE) {
        fade_time = fs_config_get_int_clamped("fade_in_duration", 0, 10000);
        if (fade_time == FS_CONFIG_NONE) {
            fade_time = 0;
        }
        fade_time = fade_time * 1000;
    }

    uint64_t dt = fs_emu_monotonic_time() - start_time;
    if (dt < fade_time) {
        float fade = 1.0 - (1.0 * dt) / fade_time;
        if (fade < 0.0) {
            fade = 0.0;
        }
        render_fade_overlay(fade);
    }

    fs_emu_video_render_mutex_unlock();
}

void fs_emu_video_render_debug_info(uint32_t *texture) {
    //return;
    int x;
    //, y;
    int y1;
    GList *link;
    uint32_t color = 0x80404080;
    fs_gl_ortho_hd();
    fs_gl_blending(TRUE);
    fs_gl_texturing(FALSE);

    // render debug triangles, these are for visually debugging vblank
    // synchronization

    y1 = 0;
    color = 0x80800080;
    static int start_add = 0;
    int add = start_add;
    for (int x = 0; x < 256; x++) {
        int y2 = add % 20;
        for (int y = y1; y < y2; y++) {
            *(texture + (((255 - y) * 256) + x)) = color;
        }
        add += 2;
    }
    start_add += 2;

    x = 127;
    y1 = 128;
    color = 0x80404080;
    link = g_queue_peek_head_link(g_fs_emu_sys_frame_times.queue);
    while (link) {
        int val = FS_POINTER_TO_INT(link->data);
        //int x2 = x - 8;
        int y2 = y1 + val * VIDEO_DEBUG_SCALE_TIMES;
        if (y2 > 256) {
            y2 = 256;
        }
        for (int y = y1; y < y2; y++) {
            *(texture + ((y * 256) + x)) = color;
        }
        if (--x < 0) {
            break;
        }
        link = link->next;
    }

    x = 127;
    y1 = 0;
    color = 0x80205080;
    link = g_queue_peek_head_link(g_fs_emu_emu_frame_times.queue);
    while (link) {
        int val = FS_POINTER_TO_INT(link->data);
        //int x2 = x - 8;
        int y2 = y1 + val * VIDEO_DEBUG_SCALE_TIMES;
        if (y2 > 256) {
            y2 = 256;
        }
        for (int y = y1; y < y2; y++) {
            *(texture + ((y * 256) + x)) = color;
        }
        if (--x < 0) {
            break;
        }
        link = link->next;
    }

    x = 127;
    y1 = 0;
    color = 0x80008080;
    link = g_queue_peek_head_link(g_fs_emu_emu2_frame_times.queue);
    while (link) {
        int val = FS_POINTER_TO_INT(link->data);
        //int x2 = x - 8;
        int y2 = y1 + val * VIDEO_DEBUG_SCALE_TIMES;
        if (y2 > 256) {
            y2 = 256;
        }
        for (int y = y1; y < y2; y++) {
            *(texture + ((y * 256) + x)) = color;
        }
        if (--x < 0) {
            break;
        }
        link = link->next;
    }
}

#ifdef WITH_LUA

static int l_fs_emu_set_scale(lua_State *L) {
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    // make sure we don't divide by zero later, zero scale not allowed
    if (x == 0) {
        x = 1;
    }
    if (y == 0) {
        y = 1;
    }
    g_l_scale_x = x;
    g_l_scale_y = y;

    return 0;
}

/*
static int l_fs_emu_render_frame(lua_State *L) {
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);

    float color = 1.0;
    if (g_frame_texture == 0) {
        // texture has not been created yet
        color = 0.0;
        fs_gl_texturing(0);
    }
    else {
        fs_gl_texturing(1);
        fs_gl_bind_texture(g_frame_texture);
    }
    fs_gl_blending(0);
    fs_gl_color4f(color, color, color, 1.0);

    float x1 = -1.0 + x * 2.0 / g_l_scale_x;
    float x2 = -1.0 + (x + w) * 2.0 / g_l_scale_x;
    float y2 = 1.0 - y * 2.0 / g_l_scale_y;
    float y1 = 1.0 - (y + h) * 2.0 / g_l_scale_y;

    double s1 = 0.0;
    double t1 = 0.0;
    double s2 = (double) g_crop.w / g_frame_texture_width;
    double t2 = (double) g_crop.h / g_frame_texture_height;

    render_quad(x1, y1, x2, y2, s1, t1, s2, t2);

    return 0;
}
*/

static int l_fs_emu_set_frame_position_and_size(lua_State *L) {
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);

    g_frame_override_x = x;
    g_frame_override_y = y;
    g_frame_override_w = w;
    g_frame_override_h = h;

    g_frame_override = g_frame_override_w > 0 || g_frame_override_h > 0 ||
            g_frame_override_x > 0 || g_frame_override_y > 0;

    return 0;
}

void fs_emu_render_init_lua(void) {
    fs_log("fs_emu_render_init_lua\n");

    //lua_register(fs_emu_lua_state, "fs_emu_render_frame",
    //        l_fs_emu_render_frame);
    lua_register(fs_emu_lua_state, "fs_emu_set_scale",
            l_fs_emu_set_scale);
    lua_register(fs_emu_lua_state, "fs_emu_set_frame_position_and_size",
            l_fs_emu_set_frame_position_and_size);
}

#endif

void fs_emu_init_render() {
    fs_log("fs_emu_init_render\n");

    g_scale_x = fs_config_get_double("scale_x");
    if (g_scale_x == FS_CONFIG_NONE) {
        g_scale_x = -1.0;
    }
    g_scale_y = fs_config_get_double("scale_y");
    if (g_scale_y == FS_CONFIG_NONE) {
        g_scale_y = -1.0;
    }
    g_align_x = fs_config_get_double("align_x");
    if (g_align_x == FS_CONFIG_NONE) {
        g_align_x = 0.5;
    }
    g_align_y = fs_config_get_double("align_y");
    if (g_align_y == FS_CONFIG_NONE) {
        g_align_y = 0.5;
    }

    const char *ccstr = fs_config_get_const_string("texture_filter");
    if (ccstr && g_ascii_strcasecmp(ccstr, "nearest") == 0) {
        g_texture_filter = GL_NEAREST;
    }

    fs_log("scale: %0.2f %0.2f align: %0.2f %0.2f\n", g_scale_x, g_scale_y,
            g_align_x, g_align_y);

    g_overlay_mutex = fs_mutex_create();

#ifdef WITH_LUA
    fs_emu_render_init_lua();
#endif
}
