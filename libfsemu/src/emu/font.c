#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "font.h"

#include <fs/emu.h>
#include <fs/glib.h>
#include <fs/i18n.h>

#include <stdlib.h>
#include <string.h>

#ifdef USE_OPENGL
#include <fs/ml/opengl.h>
#endif

#ifdef USE_FREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library library;
#endif

#include "video.h"
#include "render.h"

static int g_texture_width = 2048;
static int g_texture_height = 2048;

// FIXME: little-endian only
#define MASK 0x00ffffff

static int g_initialized = 0;
static GList* g_cache = NULL;
static int g_video_version = 0;
static GLuint g_text_texture = 0;
static uint8_t *g_buffer = NULL;

#define CACHE_SIZE (g_texture_height / 32)

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

static void initialize_cache() {
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache_item *item = malloc(sizeof(cache_item));
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
    void *data = g_malloc0(g_texture_width * g_texture_height * 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g_texture_width, g_texture_height,
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
            free(item->text);
            free(item);
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

#ifdef USE_FREETYPE

static void init_freetype(void) {
    int error = FT_Init_FreeType(&library);
    if (error) {
        fs_emu_warning("Could not initialize freetype");
    }
    else {
        fs_emu_log("freetype initialized\n");
    }
}

#endif

static void initialize() {
    g_texture_width = 2048;
    g_texture_height = 2048;
    int max_texture_size = fs_ml_get_max_texture_size();
    if (max_texture_size > 0) {
        if (max_texture_size < g_texture_width) {
            g_texture_width = max_texture_size;
        }
        if (max_texture_size < g_texture_height) {
            g_texture_height = max_texture_size;
        }
    }
    fs_log("using text cache texture size %dx%d\n", g_texture_width,
            g_texture_height);

    initialize_cache();
    create_text_texture();
    fs_gl_add_context_notification(context_notification_handler, NULL);
    g_buffer = malloc(g_texture_width * 32 * 4);

#ifdef USE_FREETYPE
    init_freetype();
#endif
    g_initialized = 1;
}

void fs_emu_font_measure(fs_emu_font *font, const char *text, int* width,
        int *height) {
    if (font == NULL || font->image == NULL) {
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

//gunichar2 fix_char(fs_emu_font *font, gunichar2 c) {
static int fix_char(fs_emu_font *font, int c) {
    if (c == 0x2019) {
        // replace RIGHT SINGLE QUOTATION MARK with common apostrophe
        return '\'';
    }
    else if (c >= font->chars) {
        // replace with question mark
        return 63;
    }
    else if (c < 0) {
        // replace with question mark
        return 63;
    }
    return c;
}


int fs_emu_font_render(fs_emu_font *font, const char *text, float x, float y,
        float r, float g, float b, float alpha) {
    if (font == NULL || font->image == NULL) {
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

#ifdef USE_GLES
        GLfloat tex[] = {
            item->x1, item->y2,
            item->x2, item->y2,
            item->x2, item->y1,
            item->x1, item->y1
        };
        GLfloat vert[] = {
            x, y,
            x + item->width, y,
            x + item->width, y + item->height,
            x, y + item->height
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
        glTexCoord2f(item->x1, item->y2);
        glVertex2f(x, y);
        glTexCoord2f(item->x2, item->y2);
        glVertex2f(x + item->width, y);
        glTexCoord2f(item->x2, item->y1);
        glVertex2f(x + item->width, y + item->height);
        glTexCoord2f(item->x1, item->y1);
        glVertex2f(x, y + item->height);
        glEnd();
#endif

        g_cache = g_list_prepend(g_cache, item);
        sanity_check();
        return item->width;
    }

    // calculate size of text

    //printf(":: %s\n", text);

    int num_chars = 0;
    int required_width = 0;
    int required_height = font->h;

    //char *base_text = fs_utf8_strup(text, -1);
    const char *base_text = text;

    unsigned const char *c = (unsigned const char*) base_text;
    int continuations = 0;
    int cp = 0;
    while(*c) {
        //if ((*c & 0b10000000) == 0b00000000) {
        if ((*c & 0x80) == 0x0) {
            continuations = 0;
            //cp = *c & 0b01111111;
            cp = *c & 0x7f;
        }
        //else if ((*c & 0b11000000) == 0b10000000) {
        else if ((*c & 0xc0) == 0x80) {
            continuations--;
            cp = cp << 6;
            //cp = cp | (*c & 0b00111111);
            cp = cp | (*c & 0x3f);
        }
        //else if ((*c & 0b11111110) == 0b11111100) { // 1111110x
        else if ((*c & 0xfe) == 0xfc) { // 1111110x
            continuations = 5;
            //cp = *c & 0b00000001;
            cp = *c & 0x1;
        }
        //else if ((*c & 0b11111100) == 0b11111000) { // 111110xx
        else if ((*c & 0xfc) == 0xf8) { // 111110xx
            continuations = 4;
            //cp = *c & 0b00000011;
            cp = *c & 0x3;
        }
        //else if ((*c & 0b11111000) == 0b11110000) { // 11110xxx
        else if ((*c & 0xf8) == 0xf0) { // 11110xxx
            continuations = 3;
            //cp = *c & 0b00000111;
            cp = *c & 0x7;
        }
        //else if ((*c & 0b11110000) == 0b11100000) { // 1110xxxx
        else if ((*c & 0xf0) == 0xe0) { // 1110xxxx
            continuations = 2;
            //cp = *c & 0b00001111;
            cp = *c & 0xf;
        }
        //else if ((*c & 0b11100000) == 0b11000000) { // 110xxxxx
        else if ((*c & 0xe0) == 0xc0) { // 110xxxxx
            continuations = 1;
            //cp = *c & 0b00011111;
            cp = *c & 0x1f;
        }

        ++c;
        if (continuations) {
            continue;
        }

        cp = fix_char(font, cp);
        //printf("%d\n", cp);
        if (required_width + font->w[cp] > g_texture_width) {
            break;
        }
        required_width += font->w[cp];
        num_chars++;
    }

    int dx = 0;
    int dy = 0;
    c = (unsigned const char*) base_text;
    //printf("base_text: %s\n", c);
    continuations = 0;
    cp = 0;
    int k = 0;
    while(*c) {
        //printf("%d\n", *c);
        //if ((*c & 0b10000000) == 0b00000000) {
        if ((*c & 0x80) == 0x0) {
            continuations = 0;
            //cp = *c & 0b01111111;
            cp = *c & 0x7f;
        }
        //else if ((*c & 0b11000000) == 0b10000000) {
        else if ((*c & 0xc0) == 0x80) {
            continuations--;
            cp = cp << 6;
            //cp = cp | (*c & 0b00111111);
            cp = cp | (*c & 0x3f);
        }
        //else if ((*c & 0b11111110) == 0b11111100) { // 1111110x
        else if ((*c & 0xfe) == 0xfc) { // 1111110x
            continuations = 5;
            //cp = *c & 0b00000001;
            cp = *c & 0x1;
        }
        //else if ((*c & 0b11111100) == 0b11111000) { // 111110xx
        else if ((*c & 0xfc) == 0xf8) { // 111110xx
            continuations = 4;
            //cp = *c & 0b00000011;
            cp = *c & 0x3;
        }
        //else if ((*c & 0b11111000) == 0b11110000) { // 11110xxx
        else if ((*c & 0xf8) == 0xf0) { // 11110xxx
            continuations = 3;
            //cp = *c & 0b00000111;
            cp = *c & 0x7;
        }
        //else if ((*c & 0b11110000) == 0b11100000) { // 1110xxxx
        else if ((*c & 0xf0) == 0xe0) { // 1110xxxx
            continuations = 2;
            //cp = *c & 0b00001111;
            cp = *c & 0xf;
        }
        //else if ((*c & 0b11100000) == 0b11000000) { // 110xxxxx
        else if ((*c & 0xe0) == 0xc0) { // 110xxxxx
            continuations = 1;
            //cp = *c & 0b00011111;
            cp = *c & 0x1f;
        }

        ++c;
        if (continuations) {
            continue;
        }

        if (++k > num_chars) {
            // there may be more chars left in the original string,
            // but there is no room for the next char in the texture
            break;
        }

        cp = fix_char(font, cp);

        int sx = font->x[cp];
        int sy = font->y[cp];
        int sw = font->w[cp];
        int sh = font->h;

        //if (c > 256) {
        //    printf("%d %d %d %d %d\n", c, sx, sy, sw, sh);
        //}

        // draw character
        //printf("-----------> %d\n", cp);

        int *sl = ((int *) font->image->data) + font->image->width * sy + sx;
        int ss = font->image->width; // source stride
        //int *sl = sp;
        int *dl = ((int *) g_buffer) + g_texture_width * dy + dx;
        int ds = g_texture_width; // destination stride
        //int *dl = dp;

        for (int y = 0; y < sh; y++) {
            //printf("%d\n", y);
            int *sp = sl;
            int *dp = dl;
            for (int x = 0; x < sw; x++) {
                //printf("%d %d\n", x, y);
                //*dp++ = 0xff0000ff;
                //*dp++ = *sp++;
                int a = *sp;
                sp++;
                *dp = a;
                dp++;
                //int a = *sp++;
            }
            sl += ss;
            dl += ds;
        }

        dx += sw;
    }
    //free(utext);
    //free(base_text);

    GList *last = g_list_last(g_cache);
    cache_item *last_item = (cache_item *) last->data;
    int position = last_item->position;

    fs_gl_bind_texture(g_text_texture);
    int gl_buffer_format = GL_RGBA;
    if (fs_emu_get_video_format() == FS_EMU_VIDEO_FORMAT_BGRA) {
        gl_buffer_format = GL_BGRA;
    }
#ifdef USE_GLES
    /* GLES does not support unpack padding of buffer. we have to update line-wise (or create a new one) */
    uint8_t *buf = g_buffer;
    for(int y=0;y<required_height;y++) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, position * 32 + y, required_width,
                1, gl_buffer_format, GL_UNSIGNED_BYTE, buf);
        buf += g_texture_width * 4;
    }
#else
    fs_gl_unpack_row_length(g_texture_width);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, position * 32, required_width,
            required_height, gl_buffer_format, GL_UNSIGNED_BYTE, g_buffer);
#endif

    cache_item *item = malloc(sizeof(cache_item));
    item->font = font;
    item->text = g_strdup(text);
    item->width = required_width;
    item->height = required_height;
    item->position = position;

    item->x1 = 0;
    item->x2 = required_width / (1.0 * g_texture_width);
    item->y1 = (item->position * 32) / (1.0 * g_texture_height);
    item->y2 = (item->position * 32 + required_height) /
            (1.0 * g_texture_height);
    //item->texture = render_texture;
    g_cache = g_list_prepend(g_cache, item);
    sanity_check();

    if (last_item->text) {
        free(last_item->text);
    }
    /*
    if (last_item->texture) {
        glDeleteTextures(1, &last_item->texture);
    }
    */
    free(last_item);

    g_cache = g_list_delete_link(g_cache, last);
    sanity_check();

    // now the text is in the cache, so call function again
    return fs_emu_font_render(font, text, x, y, r, g, b, alpha);
}

static void convert_to_premultiplied_alpha(fs_image *image)
{
    if (image->format == FS_IMAGE_FORMAT_RGBA) {
        int num_pixels = image->width * image->height;
        unsigned char *pixels = image->data;
        for (int i = 0; i < num_pixels; i++) {
            unsigned char alpha = pixels[3];
            pixels[0] = ((int) pixels[0]) * alpha / 255;
            pixels[1] = ((int) pixels[1]) * alpha / 255;
            pixels[2] = ((int) pixels[2]) * alpha / 255;
            pixels += 4;
        }
    }
}

static fs_image *load_font_from_file(const char *path)
{
    if (path == NULL) {
        fs_emu_warning(_("Could not find font: %s"), path);
        return NULL;
    }
    fs_emu_log("loading image \"%s\"\n", path);
    fs_image *image = fs_image_new_from_file(path);
    if (image == NULL) {
        fs_emu_warning(_("Error loading font: %s"), path);
        return NULL;
    }
    convert_to_premultiplied_alpha(image);
    return image;
}

static fs_image *load_font_from_data(char *data, int size)
{
    fs_image *image = fs_image_new_from_data(data, size);
    if (image == NULL) {
        fs_emu_warning(_("Error loading font from data"));
        return NULL;
    }
    convert_to_premultiplied_alpha(image);
    return image;
}

static void prepare_font(fs_emu_font *font)
{
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
}

fs_emu_font *fs_emu_font_new_from_data(char *data, int size)
{
    fs_emu_log("load font from data %p size %d\n", data, size);
    fs_emu_font *font = g_malloc0(sizeof(fs_emu_font));
    if (data != NULL) {
        font->image = load_font_from_data(data, size);
    }
    prepare_font(font);
    return font;
}

fs_emu_font *fs_emu_font_new_from_file(const char *name)
{
    fs_emu_log("load font %s\n", name);
    fs_emu_font *font = g_malloc0(sizeof(fs_emu_font));
    font->image = load_font_from_file(name);
    prepare_font(font);
    return font;
}
