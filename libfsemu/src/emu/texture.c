#include "texture.h"

#include <fs/glee.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <fs/ml.h>

#include "fs/image.h"
#include "video.h"
#include "render.h"
#include "libfsemu.h"

static fs_emu_texture *g_atlas = NULL;
fs_emu_texture *g_fs_emu_overlay_texture = NULL;

typedef struct texture_entry {
    int x;
    int y;
    int w;
    int h;
} texture_entry;

// dividing the 1024x1024 bitmap into a 128x128 cell structure, for
// placement of sub-textures.
static char g_cells[128][128] = {};

static texture_entry g_entries[] = {
    {   0,   0,   0,    0},
    {  96, 512, 512,  512}, // TEXTURE_GLOSS - OK
    { 640, 896, 128,  128}, // TEXTURE_GLOW_LEFT - OK
    { 640, 736, 128,  128}, // TEXTURE_GLOW_TOP_LEFT - OK
    { 800, 736, 128,  128}, // TEXTURE_GLOW_TOP - OK
    {  50,   0,  10, 1020}, // TEXTURE_SIDEBAR - OK
    {   0,   0,  60, 1020}, // TEXTURE_SIDEBAR_EDGE - OK
    { 224,   0,  64,   64}, // TEXTURE_CLOSE - OK
    { 288,   0,  64,   64}, // TEXTURE_VOLUME - OK
    { 352,   0,  64,   64}, // TEXTURE_VOLUME_MUTED - OK
    {  96,   0,  64,   64}, // TEXTURE_ASPECT - OK
    { 160,   0,  64,   64}, // TEXTURE_STRETCH - OK
    {  96, 384, 540,   96}, // TEXTURE_ITEM_BACKGROUND - OK
    { 658, 402,  60,   60}, // TEXTURE_TOP_ITEM_BG - OK
    { 672, 480,  32,   32}, // TEXTURE_HEADING_BG - OK
    { 0, 0, 0, 0}, // TEXTURE_PAUSE
};

void fs_emu_draw_from_atlas(float dx, float dy, float dw, float dh,
        int sx, int sy, int sw, int sh) {
    float tx = sx / 1024.0;
    float ty = sy / 1024.0;
    float tw = sw / 1024.0;
    float th = sh / 1024.0;
    //fs_gl_texturing(1);
    //fs_gl_bind_texture(g_atlas->texture);
    //printf("%f %f %f %f - %f %f %f %f\n", dx, dy, dw, dh, tx, ty, tw, th);

    fs_emu_set_texture(g_atlas);
    glBegin(GL_QUADS);
    glTexCoord2f(tx, ty + th); glVertex2f(dx, dy);
    glTexCoord2f(tx + tw, ty + th); glVertex2f(dx + dw, dy);
    glTexCoord2f(tx + tw, ty); glVertex2f(dx + dw, dy + dh);
    glTexCoord2f(tx, ty); glVertex2f(dx, dy + dh);
    glEnd();
    CHECK_GL_ERROR();
}

void fs_emu_prepare_texture(int entry, float *tx1, float *ty1,
        float *tx2, float *ty2) {
    *tx1 = g_entries[entry].x / 1024.0;
    *ty1 = g_entries[entry].y / 1024.0;
    *tx2 = *tx1 + g_entries[entry].w / 1024.0;
    *ty2 = *ty1 + g_entries[entry].h / 1024.0;
    fs_emu_set_texture(g_atlas);
}

void fs_emu_draw_texture_with_size(int entry, float x, float y, float w,
        float h) {
    fs_emu_draw_from_atlas(x, y, w, h, g_entries[entry].x,
            g_entries[entry].y, g_entries[entry].w, g_entries[entry].h);
}

void fs_emu_draw_texture(int entry, float x, float y) {
    //printf("%d - %f %f\n", entry, x, y);
    fs_emu_draw_texture_with_size(entry, x, y, g_entries[entry].w,
            g_entries[entry].h);
}

static int check_placement(int cx, int cy, int cw, int ch) {
    // just using a naive approach to finding a suitable space. The search
    // space isn't that big in this case..
    for (int y = cy; y < cy + ch; y++) {
        //printf("%d\n", y);
        for (int x = cx; x < cx + cw; x++) {
            if (g_cells[y][x]) {
                return 0;
            }
        }
    }
    return 1;
}

static inline void copy_pixel(unsigned char **dst, unsigned char **src) {
    unsigned char *sp = *src;
    unsigned char *dp = *dst;
    if (fs_emu_get_video_format() == FS_EMU_VIDEO_FORMAT_BGRA) {
        dp[0] = ((int) sp[2]) * sp[3] / 255;
        dp[1] = ((int) sp[1]) * sp[3] / 255;
        dp[2] = ((int) sp[0]) * sp[3] / 255;
    }
    else {
        dp[0] = ((int) sp[0]) * sp[3] / 255;
        dp[1] = ((int) sp[1]) * sp[3] / 255;
        dp[2] = ((int) sp[2]) * sp[3] / 255;
    }
    dp[3] = sp[3];
    *dst = *dst + 4;
    *src = *src + 4;
}

static void load_atlas_texture(fs_image *atlas_image,
        int texture_id, const char *name) {
    char *path = fs_emu_get_theme_resource(name);
    if (!path) {
        fs_emu_warning("Could not find resource %s\n", name);
        return;
    }

    fs_image *image = fs_image_new_from_file(path);
    if (!image) {
        fs_emu_warning("error loading texture \"%s\"\n", name);
        return;
    }
    fs_emu_log("loaded sub-texture from \"%s\"\n", path);

    // find needed number of cells
    int cw = (image->width + 7) / 8;
    int ch = (image->height + 7) / 8;
    // adding 1 to get some spacing between cells
    if (cw < 128) {
        cw++;
    }
    if (ch < 128) {
        ch++;
    }

    // try to find space for it in the texture atlas using a simple
    // brute-force search
    int cx = 0;
    int cy = 0;
    int ok = 0;
    for (cy = 0; cy <= 128 - ch; cy++) {
        for (cx = 0; cx <= 128 - cw; cx++) {
            if (check_placement(cx, cy, cw, ch)) {
                ok = 1;
                break;
            }
        }
        if (ok) {
            break;
        }
    }
    if (!ok) {
        fs_emu_warning("could not find space for \"%s\"\n", name);
        //printf("%d %d %d %d\n", cx * 8, cy * 8, cw * 8, ch * 8);
        return;
    }
    // mark cells as used
    for (int y = cy; y < cy + ch; y++) {
        for (int x = cx; x < cx + cw; x++) {
            g_cells[y][x] = 1;
        }
    }
    //printf("%d %d %d %d\n", cx * 8, cy * 8, cw * 8, ch * 8);

    // copy sub-texture into texture, also converting to pre-multiplied
    // alpha, and BGRA if needed. Also copy border pixels to pixels outside
    // border to fix unwanted bilinear filtering effects
    int dx = cx * 8;
    int dy = cy * 8;
    int ds = atlas_image->width * 4; // stride
    unsigned char *dst = atlas_image->data + \
            (dy * atlas_image->width + dx) * 4;
    unsigned char *sp = image->data;

    for (int y = 0; y < image->height; y++) {
#if 1
        if (y == 0 && dy > 0) {
            // repeat first line
            unsigned char *dp = dst - ds;
            for (int x = 0; x < image->width; x++) {
                copy_pixel(&dp, &sp);
            }
            sp -= image->width * 4;
        }
#endif
        unsigned char *dp = dst;
        for (int x = 0; x < image->width; x++) {
            copy_pixel(&dp, &sp);
        }
        dst += 1024 * 4; // stride
#if 1
        if (y == image->height - 1 && dy + y - 1 < atlas_image->height - 1) {
            // repeat last line
            sp -= image->width * 4;
            unsigned char *dp = dst;
            for (int x = 0; x < image->width; x++) {
                copy_pixel(&dp, &sp);
            }
        }
#endif
    }

    // repeat left and right borders
    uint32_t *idata = (uint32_t *) atlas_image->data;
    int iwidth = atlas_image->width;
    int y1 = dy;
    int y2 = dy + image->height - 1;
    if (y1 > 0) {
        y1 -= 1;
    }
    if (y2 < atlas_image->height - 1) {
        y2 += 1;
    }
    if (dx > 0) {
        for (int y = y1; y <= y2; y++) {
            idata[y * iwidth + dx - 1] = idata[y * iwidth + dx];
        }
    }
    if (dx + image->width - 1 < atlas_image->width - 1) {
        for (int y = y1; y <= y2; y++) {
            idata[y * iwidth + dx + image->width - 1 + 1] = \
                    idata[y * iwidth + dx + image->width - 1];
        }
    }

    // register texture coordinates
    g_entries[texture_id].x = cx * 8;
    g_entries[texture_id].y = cy * 8;
    g_entries[texture_id].w = image->width;
    g_entries[texture_id].h = image->height;

    // and finally free the sub-texture image
    fs_unref(image);
}

static void initialize_atlas(fs_image *image) {
    load_atlas_texture(image, TEXTURE_SIDEBAR_EDGE, "sidebar_edge.png");
    load_atlas_texture(image, TEXTURE_SIDEBAR, "sidebar.png");
    load_atlas_texture(image, TEXTURE_GLOSS, "gloss.png");
    load_atlas_texture(image, TEXTURE_GLOW_LEFT, "glow_left.png");
    load_atlas_texture(image, TEXTURE_GLOW_TOP, "glow_top.png");
    load_atlas_texture(image, TEXTURE_GLOW_TOP_LEFT, "glow_top_left.png");
    load_atlas_texture(image, TEXTURE_CLOSE, "close.png");
    load_atlas_texture(image, TEXTURE_VOLUME, "volume.png");
    load_atlas_texture(image, TEXTURE_VOLUME_MUTED, "volume_muted.png");
    load_atlas_texture(image, TEXTURE_ASPECT, "aspect.png");
    load_atlas_texture(image, TEXTURE_STRETCH, "stretch.png");

    load_atlas_texture(image, TEXTURE_ITEM_BACKGROUND, "item_background.png");
    load_atlas_texture(image, TEXTURE_TOP_ITEM_BG, "top_item_background.png");
    load_atlas_texture(image, TEXTURE_HEADING_BG, "heading_strip.png");
    load_atlas_texture(image, TEXTURE_PAUSE, "pause_indicator.png");

    //fs_image_save_data("atlas-output-test.png", image->data,
    //        image->width, image->height, 4);
}

void load_texture(fs_emu_texture *texture) {
    fs_image *image = texture->image;
    //printf("loading texture from image %p\n", image);
    if (!image) {
        return;
    }
    unsigned int opengl_texture;
    glGenTextures(1, &opengl_texture);
    //texture->opengl_context_stamp = g_fs_ml_opengl_context_stamp;
    fs_gl_bind_texture(opengl_texture);
    fs_gl_unpack_row_length(0);
    int gl_buffer_format = GL_RGBA;
    if (fs_emu_get_video_format() == FS_EMU_VIDEO_FORMAT_BGRA) {
        gl_buffer_format = GL_BGRA;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height,
            0, gl_buffer_format, GL_UNSIGNED_BYTE, image->data);
    CHECK_GL_ERROR();
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
    //        GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
    //        GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    CHECK_GL_ERROR();
    //glGenerateMipmapEXT(GL_TEXTURE_2D);
    texture->texture = opengl_texture;
}

static void context_notification_handler(int notification, void *data) {
    fs_emu_texture *texture = (fs_emu_texture *) data;
    if (notification == FS_GL_CONTEXT_DESTROY) {
        //printf("context_notification_handler DESTROY %d\n", texture->texture);
        if (texture->texture) {
            glDeleteTextures(1, &texture->texture);
            CHECK_GL_ERROR();
            texture->texture = 0;
        }
    }
    else if (notification == FS_GL_CONTEXT_CREATE) {
        // loading the texture right now is not strictly necessary
        load_texture(texture);
    }
}

void fs_emu_initialize_textures() {
    //g_atlas = fs_emu_texture_new_from_file("atlas");
    fs_image *image = fs_image_new();
    image->width = 1024;
    image->height = 1024;
    image->format = FS_IMAGE_FORMAT_RGBA;
    image->data = fs_malloc0(1024 * 1024 * 4);
    initialize_atlas(image);

    g_atlas = fs_new(fs_emu_texture, 1);
    g_atlas->width = image->width;
    g_atlas->height = image->height;
    g_atlas->image = image;
    load_texture(g_atlas);
    fs_emu_set_texture(g_atlas);
    fs_gl_add_context_notification(context_notification_handler, g_atlas);

    if (g_fs_emu_theme.overlay_image[0]) {
        char *path = fs_emu_get_theme_resource(g_fs_emu_theme.overlay_image);
        fs_log("g_fs_emu_theme.overlay_image %s => %s\n",
                g_fs_emu_theme.overlay_image, path);
        if (path) {
            g_fs_emu_overlay_texture = fs_emu_texture_new_from_file(path);
        }
    }

    for (int i = 0; i < MAX_CUSTOM_OVERLAYS; i++) {
        char *name;
        name = fs_strdup_printf("custom_%d.png", i);
        char *path = fs_emu_get_theme_resource(name);
        if (path) {
            //printf("loading %s\n", path);
            g_fs_emu_theme.overlay_textures[i] =
                    fs_emu_texture_new_from_file(path);
            free(path);
        }
        free(name);
    }
}

void fs_emu_set_texture(fs_emu_texture *texture) {
    fs_gl_texturing(1);
    /*
    if (texture && texture->opengl_context_stamp && \
            g_fs_ml_opengl_context_stamp != texture->opengl_context_stamp) {
        // OpenGL context has been recreated. load_texture also binds
        load_texture(texture);
    }
    */
    if (!texture) {
        fs_gl_bind_texture(0);
        return;
    }
    if (texture->texture) {
        fs_gl_bind_texture(texture->texture);
    }
    else {
        // texture was not loaded, perhaps due to context recreation
        load_texture(texture);
    }
}

fs_emu_texture *fs_emu_texture_new_from_file(const char *name) {
    char *full_name;
    char *path;
    if (fs_path_exists(name)) {
        full_name = fs_strdup(name);
        path = fs_strdup(name);
    }
    else {
        full_name = fs_strconcat(name, ".png", NULL);
        path = fs_get_program_data_file(full_name);
        if (path == NULL) {
            fs_emu_warning("Could not find texture %s\n", full_name);
            return NULL;
        }
    }
    fs_image *image = fs_image_new_from_file(path);
    fs_emu_log("loading texture \"%s\"\n", path);
    free(path);
    if (image == NULL) {
        fs_emu_warning("Could not load texture from %s\n", full_name);
        free(full_name);
        return NULL;
    }
    free(full_name);

    if (fs_emu_get_video_format() == FS_EMU_VIDEO_FORMAT_BGRA) {
        // convert to premultiplied alpha
        if (image->format == FS_IMAGE_FORMAT_RGBA) {
            int num_pixels = image->width * image->height;
            unsigned char *pixels = image->data;
            for (int i = 0; i < num_pixels; i++) {
                unsigned char alpha = pixels[3];
                unsigned char temp = pixels[2];
                pixels[2] = ((int) pixels[0]) * alpha / 255;
                pixels[1] = ((int) pixels[1]) * alpha / 255;
                pixels[0] = ((int) temp) * alpha / 255;
                pixels += 4;
            }
        }
        else {
            // FIXME: should swap R and B here...
        }
    }
    else {
        // convert to premultiplied alpha
        if (image->format == FS_IMAGE_FORMAT_RGBA) {
            int num_pixels = image->width * image->height;
            unsigned char *pixels = image->data;
            for (int i = 0; i < num_pixels; i++) {
                unsigned char alpha = pixels[3];
                // should really divide by 255, but 256 is faster...
                //pixels[0] = ((int) pixels[0]) * alpha / 256;
                //pixels[1] = ((int) pixels[1]) * alpha / 256;
                //pixels[2] = ((int) pixels[2]) * alpha / 256;
                pixels[0] = ((int) pixels[0]) * alpha / 255;
                pixels[1] = ((int) pixels[1]) * alpha / 255;
                pixels[2] = ((int) pixels[2]) * alpha / 255;
                //pixels[0] = (unsigned char) ((pixels[0] * alpha + 0.5) / 255.0);
                //pixels[1] = (unsigned char) ((pixels[1] * alpha + 0.5) / 255.0);
                //pixels[2] = (unsigned char) ((pixels[2] * alpha + 0.5) / 255.0);
                pixels += 4;
            }
        }
    }

    fs_emu_texture *texture = fs_new(fs_emu_texture, 1);
    texture->width = image->width;
    texture->height = image->height;
    texture->image = image;
    load_texture(texture);
    fs_emu_set_texture(texture);

    fs_gl_add_context_notification(context_notification_handler, texture);

    return texture;
}

void fs_emu_texture_render(fs_emu_texture *texture, int x, int y) {
    fs_emu_render_texture_with_size(texture, x, y, texture->width,
            texture->height);
}

void fs_emu_render_texture_with_size(fs_emu_texture *texture, int x, int y,
        int w, int h) {
    fs_emu_set_texture(texture);
    fs_gl_blending(1);
    //fs_emu_texturing(0);
    //return;
    //fs_log("%d %d %d %d\n", x, y, x + w , y + h);
    glBegin(GL_QUADS);
    fs_gl_color4f(1.0, 1.0, 1.0, 1.0);
    glTexCoord2f(0.0, 1.0);
    glVertex2f(x, y);
    glTexCoord2f(1.0, 1.0);
    glVertex2f(x + w, y);
    glTexCoord2f(1.0, 0.0);
    glVertex2f(x + w, y + h);
    glTexCoord2f(0.0, 0.0);
    glVertex2f(x, y + h);
    glEnd();
    CHECK_GL_ERROR();
}
