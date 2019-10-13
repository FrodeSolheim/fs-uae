#define FSEMU_INTERNAL
#include "fsemu-font.h"

#include "fsemu-data.h"
#include "fsemu-sdl.h"

struct fsemu_font {
    TTF_Font *font;
};

void fsemu_font_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_data_init();
    fsemu_font_log("Initialize\n");

    if (!TTF_WasInit() && TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(1);
    }
}

fsemu_font_t *fsemu_font_load(const char *name, int size)
{
    // FIXME: Implement shared fonts
    fsemu_font_t *font = (fsemu_font_t *) malloc(sizeof(fsemu_font_t));

    void *data;
    int data_size;
    fsemu_data_load(name, &data, &data_size);
    if (data == NULL) {
        return NULL;
    }

    fsemu_font_log("Loaded font %s size %d\n", name, data_size);

    font->font = TTF_OpenFontRW(SDL_RWFromMem(data, data_size), 0, size);
    if (!font->font) {
        fsemu_font_log("TTF_OpenFontRW: %s\n", TTF_GetError());
        free(font);
        return NULL;
    }

    return font;
}

fsemu_image_t *fsemu_font_render_text_to_image(fsemu_font_t *font,
                                               const char *text,
                                               uint32_t color)
{
    uint8_t r = color & 0xff;
    uint8_t g = (color & 0xff00) >> 8;
    uint8_t b = (color & 0xff0000) >> 16;
    uint8_t a = (color & 0xff000000) >> 24;
    SDL_Color sdl_color = {r, g, b, a};

    SDL_Surface *surface;
    if (!(surface = TTF_RenderUTF8_Blended(font->font, text, sdl_color))) {
        // handle error here, perhaps print TTF_GetError at least
        fsemu_font_log("Error rendering text: %s\n", TTF_GetError());
        return NULL;
    }

    fsemu_image_t *image = malloc(sizeof(fsemu_image_t));

    image->width = surface->w;
    image->height = surface->h;
    image->data = surface->pixels;
    image->stride = surface->pitch;

    // Do not delete the surface here (or rather, the image data) since
    // we reference and return this.
    // SDL_FreeSurface(text_surface);

    return image;
}
