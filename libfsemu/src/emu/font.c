#include "font.h"

#include <fs/glee.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <glib.h>

#include "video.h"
#include "render.h"

#define TEXTURE_WIDTH 1024
#define TEXTURE_HEIGHT 1024

// FIXME: little-endian only
#define MASK 0x00ffffff

static int g_initialized = 0;
static GList* g_cache = NULL;
static int g_video_version = 0;
static GLuint g_text_texture = 0;
static uint8_t *g_buffer = NULL;

#define CACHE_SIZE (TEXTURE_HEIGHT / 32)

//static char g_positions[64] = {};

typedef struct _cache_item {
    fs_emu_font *font;
    char *text;
    //unsigned int texture;
    int position;
    int width;
    int height;
    float x1;
    float x2;
    float y1;
    float y2;
} cache_item;

static void sanity_check() {
}

void initialize_cache() {
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache_item *item = g_malloc(sizeof(cache_item));
        item->font = NULL;
        item->text = NULL;
        item->position = i;
        g_cache = g_list_append(g_cache, item);
    }
    sanity_check();
    // FIXME: REMOVE
    g_video_version = g_fs_ml_opengl_context_stamp;
}

static void create_text_texture() {
    glGenTextures(1, &g_text_texture);
    fs_gl_bind_texture(g_text_texture);
    // want to clear data to color (0, 0, 0, 0), probably a better
    // way to to this...
    void *data = g_malloc0(TEXTURE_WIDTH * TEXTURE_HEIGHT * 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_WIDTH, TEXTURE_HEIGHT,
            0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    g_free(data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

static void context_notification_handler(int notification, void *data) {
    if (notification == FS_GL_CONTEXT_DESTROY) {
        if (g_text_texture != 0) {
            glDeleteTextures(1, &g_text_texture);
            g_text_texture = 0;
        }

        // FIXME: clear text cache..
        //printf("FIXME: clear text cache\n");

        GList* list = g_cache;
        while (list) {
            cache_item *item = (cache_item *) list->data;
            g_free(item->text);
            g_free(item);
            list = list->next;
        }
        g_list_free(g_cache);
        g_cache = NULL;
        initialize_cache();
    }
    else if (notification == FS_GL_CONTEXT_CREATE) {
        create_text_texture();
    }
}

void initialize() {
    initialize_cache();
    create_text_texture();
    fs_gl_add_context_notification(context_notification_handler, NULL);
    g_buffer = g_malloc(TEXTURE_WIDTH * 32 * 4);
    g_initialized = 1;
}

void fs_emu_font_measure(fs_emu_font *font, const char *text, int* width,
        int *height) {
    if (font->image == NULL) {
        if (width) {
            *width = 0;
        }
        if (height) {
            *height = 0;
        }
        return;
    }
    int required_width = 0;
    int required_height = font->h;
    unsigned char *cp = (unsigned char *) text;
    for(; *cp; cp++) {
        unsigned char c = *cp;
        required_width += font->w[c];
    }
    //fs_log("width: %d, height: %d\n", required_width, required_height);
    if (width) {
        *width = required_width;
    }
    if (height) {
        *height = required_height;
    }
}

int fs_emu_font_render_with_outline(fs_emu_font *font, const char *text,
        float x, float y, float r, float g, float b, float a,
        float o_r, float o_g, float o_b, float o_a, float o_w) {
    fs_emu_font_render(font, text, x - o_w, y - o_w, o_r, o_g, o_b, o_a);
    fs_emu_font_render(font, text, x + o_w, y + o_w, o_r, o_g, o_b, o_a);
    fs_emu_font_render(font, text, x + o_w, y - o_w, o_r, o_g, o_b, o_a);
    fs_emu_font_render(font, text, x - o_w, y + o_w, o_r, o_g, o_b, o_a);
    return fs_emu_font_render(font, text, x, y, r, g, b, a);
}

gunichar2 fix_char(fs_emu_font *font, gunichar2 c) {
    if (c == 0x2019) {
        // replace RIGHT SINGLE QUOTATION MARK with common apostrophe
        return '\'';
    }
    else if (c >= font->chars) {
        // replace with question mark
        return 63;
    }
    return c;
}


int fs_emu_font_render(fs_emu_font *font, const char *text, float x, float y,
        float r, float g, float b, float alpha) {
    if (font->image == NULL) {
        return 0;
    }
    if (text == NULL || *text == '\0') {
        return 0 ;
    }
    if (!g_initialized) {
        initialize();
    }

    // find cached text entry, if any
    GList* list = g_cache;
    while (list) {
        cache_item *item = (cache_item *) list->data;
        if (item->font == font && strcmp(item->text, text) == 0) {
            break;
        }
        list = list->next;
    }
    if (list) {
        cache_item *item = (cache_item *) list->data;
        g_cache = g_list_delete_link(g_cache, list);
        sanity_check();
        fs_gl_blending(1);
        fs_gl_texturing(1);
        //fs_emu_ortho();
        //fs_emu_set_texture(NULL);
        fs_gl_bind_texture(g_text_texture);
        //glColor4f(1.0, 1.0, 1.0, alpha);
        //printf("rendering %f %f %f %f...\n", item->x1, item->x2, item->y1, item->y2);
        fs_gl_color4f(r, g, b, alpha);
        //glColor4f(r * alpha, g * alpha, b * alpha, alpha);
        glBegin(GL_QUADS);
        glTexCoord2f(item->x1, item->y2);
        glVertex2f(x, y);
        glTexCoord2f(item->x2, item->y2);
        glVertex2f(x + item->width, y);
        glTexCoord2f(item->x2, item->y1);
        glVertex2f(x + item->width, y + item->height);
        glTexCoord2f(item->x1, item->y1);
        glVertex2f(x, y + item->height);
        glEnd();

        g_cache = g_list_prepend(g_cache, item);
        sanity_check();
        return item->width;
    }

    // calculate size of text

    //printf(":: %s\n", text);

    int chars = 0;
    int required_width = 0;
    int required_height = font->h;

    gunichar2 *utext = g_utf8_to_utf16(text, -1, NULL, NULL, NULL);

    //unsigned char *cp = (unsigned char *) text;
    gunichar2 *cp = (gunichar2 *) utext;

    for(; *cp; cp++) {
        gunichar2 c = fix_char(font, *cp);
        //printf("  %p\n", font);
        //printf("    %d\n", c);
        //printf("    %d\n", font->w[c]);
        if (required_width + font->w[c] > TEXTURE_WIDTH) {
            break;
        }
        required_width += font->w[c];
        chars++;
    }

    // FIXME: clear g_buffer
    //cp = (unsigned char *) text;
    cp = (gunichar2 *) utext;

    int dx = 0;
    int dy = 0;

    for(int i = 0; i < chars; i++) {
        //unsigned char c = *cp++;
        gunichar2 c = fix_char(font, *cp++);
        //unsigned char = (unsigned char) text[i];
        int sx = font->x[c];
        int sy = font->y[c];
        int sw = font->w[c];
        int sh = font->h;

        //if (c > 256) {
        //    printf("%d %d %d %d %d\n", c, sx, sy, sw, sh);
        //}

        // draw character

        int *sl = ((int *) font->image->data) + font->image->width * sy + sx;
        int ss = font->image->width; // source stride
        //int *sl = sp;
        int *dl = ((int *) g_buffer) + TEXTURE_WIDTH * dy + dx;
        int ds = TEXTURE_WIDTH; // destination stride
        //int *dl = dp;

        for (int y = 0; y < sh; y++) {
            //printf("%d\n", y);
            int *sp = sl;
            int *dp = dl;
            for (int x = 0; x < sw; x++) {
                //printf("%d %d\n", x, y);
                //*dp++ = 0xff0000ff;
                *dp++ = *sp++;
                //int a = *sp++;
            }
            sl += ss;
            dl += ds;
        }

        dx += sw;
    }
    g_free(utext);

    GList *last = g_list_last(g_cache);
    cache_item *last_item = (cache_item *) last->data;
    int position = last_item->position;

    fs_gl_bind_texture(g_text_texture);
    fs_gl_unpack_row_length(TEXTURE_WIDTH);
    int gl_buffer_format = GL_RGBA;
    if (fs_emu_get_video_format() == FS_EMU_VIDEO_FORMAT_BGRA) {
        gl_buffer_format = GL_BGRA;
    }
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, position * 32, required_width,
            required_height, gl_buffer_format, GL_UNSIGNED_BYTE, g_buffer);

    cache_item *item = g_malloc(sizeof(cache_item));
    item->font = font;
    item->text = g_strdup(text);
    item->width = required_width;
    item->height = required_height;
    item->position = position;

    item->x1 = 0;
    item->x2 = required_width / (1.0 * TEXTURE_WIDTH);
    item->y1 = (item->position * 32) / (1.0 * TEXTURE_HEIGHT);
    item->y2 = (item->position * 32 + required_height) /
            (1.0 * TEXTURE_HEIGHT);
    //item->texture = render_texture;
    g_cache = g_list_prepend(g_cache, item);
    sanity_check();

    if (last_item->text) {
        g_free(last_item->text);
    }
    /*
    if (last_item->texture) {
        glDeleteTextures(1, &last_item->texture);
    }
    */
    g_free(last_item);

    g_cache = g_list_delete_link(g_cache, last);
    sanity_check();

    // now the text is in the cache, so call function again
    return fs_emu_font_render(font, text, x, y, r, g, b, alpha);
}

static fs_image *load_font_from_file(const char *path) {
    //char *full_name = g_strconcat(name, ".png", NULL);
    //char *path = g_build_filename(fs_emu_get_share_dir(), full_name, NULL);
    //char *path = fs_get_program_data_file(full_name);
    if (path == NULL) {
        fs_emu_warning("Could not find font");
        return NULL;
    }
    fs_emu_log("loading image \"%s\"\n", path);
    fs_image *image = fs_image_new_from_file(path);
    if (image == NULL) {
        fs_emu_warning("Error loading font");
        return NULL;
    }

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
    return image;
}

fs_emu_font *fs_emu_font_new_from_file(const char *name) {
    fs_emu_log("load font %s\n", name);
    fs_emu_font *font = g_malloc0(sizeof(fs_emu_font));
    font->image = load_font_from_file(name);

    if (font->image) {
        unsigned char *data = font->image->data;
        uint32_t *idata = (uint32_t *) data;
        uint32_t *line = idata;
        int width = font->image->width;
        int height = font->image->height;

        int x = 1;
        int y = 1;
        int h = 0;
        //uint32_t blank = line[0] & MASK;
        uint32_t blank = line[0];

        for (int i = 0; i < height; i++) {
            //if (i > 0 && h == 0 && (idata[i * width] & MASK) == blank) {
            if (i > 0 && h == 0 && idata[i * width] == blank) {
                h = i - 1;
            }
            idata[i * width] = 0x00000000;
        }
        fs_emu_log("font height: %d pixels\n", h);
        font->h = h;

        //unsigned char c = 31; // first actual character is 32
        int c = -1;
        int in_character = 0;
        while (1) {
            if (in_character) {
                //if ((line[x] & MASK) == blank) {
                if (line[x] == blank) {
                    //fs_log("blank at %d %d\n", x, y);
                    in_character = 0;
                    font->w[c] = x - font->x[c];
                }
            }
            else {
                //if ((line[x] & MASK) != blank) {
                if ((line[x]) != blank) {
                    if (c == FS_EMU_FONT_MAX_CHARS - 1) {
                        fs_emu_log("FS_EMU_FONT_MAX_CHARS reached\n");
                        break;
                    }
                    c++;
                    //fs_log("char %d at %d %d\n", c, x, y);
                    font->x[c] = x;
                    font->y[c] = y;
                    in_character = 1;
                }
            }
            //fs_log("%d %x\n", x, idata[x]);
            line[x] = 0x00000000;
            x++;
            if (x == width) {
                x = 1;
                y = y + (h + 1);
                if (y >= height) {
                    break;
                }
                line = line + width * (h + 1);
                //exit(1);
            }
        }
        font->chars = c + 1;
        fs_emu_log("%d characters\n", font->chars);
    }

    return font;
}
