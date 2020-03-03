#define FSEMU_INTERNAL
#include "fsemu-font.h"

#include "fsemu-color.h"
#include "fsemu-data.h"
#include "fsemu-image.h"
#include "fsemu-refable.h"
#include "fsemu-sdl.h"
#include "fsemu-util.h"

struct fsemu_font {
    FSEMU_REFABLE;
    TTF_Font *font;
    SDL_RWops *rwops;
    void *data;
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

void fsemu_font_ref(fsemu_font_t *font)
{
    fsemu_refable_ref(font);
}

void fsemu_font_unref(fsemu_font_t *font)
{
    fsemu_refable_unref(font);
}

static void fsemu_font_finalize(void *object)
{
    fsemu_font_t *font = (fsemu_font_t *) object;
    fsemu_font_log("Finalizing font %p\n", font);
    if (font->font) {
        TTF_CloseFont(font->font);
    }
    if (font->rwops) {
        SDL_RWclose(font->rwops);
    }
    if (font->data) {
        free(font->data);
    }
    // printf("LEAK free %p\n", font);
    free(font);
}

fsemu_font_t *fsemu_font_load(const char *name, int size)
{
    fsemu_font_init();

    fsemu_font_t *font = FSEMU_UTIL_MALLOC0(fsemu_font_t);
    // printf("LEAK malloc %p\n", font);
    fsemu_refable_init_with_finalizer(font, fsemu_font_finalize);

    void *data;
    int data_size;
    fsemu_data_load(name, &data, &data_size);
    if (data == NULL) {
        free(font);
        return NULL;
    }

    fsemu_font_log("Loaded font %s size %d\n", name, data_size);

    font->data = data;
    font->rwops = SDL_RWFromMem(data, data_size);
    font->font = TTF_OpenFontRW(font->rwops, 0, size);

    // FIXME: Must data remain valid?
    // free(data);

    if (!font->font) {
        fsemu_font_log("TTF_OpenFontRW: %s\n", TTF_GetError());
        fsemu_font_unref(font);
        return NULL;
    }

    // FIXME: Never return null? Instead return a font which cannot render
    // anything? With a function to check if the font is OK...

    return font;
}

fsemu_image_t *fsemu_font_render_text_to_image(fsemu_font_t *font,
                                               const char *text,
                                               uint32_t color)
{
    SDL_Color sdl_color;
    fsemu_color_decode(
        color, &sdl_color.r, &sdl_color.g, &sdl_color.b, &sdl_color.a);
#if 0
    sdl_color.r = 255;
    sdl_color.g = 255;
    sdl_color.b = 255;
    sdl_color.a = 255;
#endif

    // Reversing colors to (effectively) get BGRA
    uint8_t temp = sdl_color.r;
    sdl_color.r = sdl_color.b;
    sdl_color.b = temp;

    // printf("color %d %d %d %d\n", sdl_color.r, sdl_color.g, sdl_color.b,
    // sdl_color.a);
    SDL_Surface *surface;
    if (!(surface = TTF_RenderUTF8_Blended(font->font, text, sdl_color))) {
        // handle error here, perhaps print TTF_GetError at least
        fsemu_font_log("Error rendering text: %s\n", TTF_GetError());
        fsemu_font_log("Text was: '%s'\n", text);
        fsemu_image_t *image = fsemu_image_from_size(1, 1);
        // FIXME: Make transparent pixel
        return image;
        // return NULL;
    }

    fsemu_image_t *image = fsemu_image_new();
    image->bpp = 4;
    image->depth = 32;
    image->height = surface->h;
    image->stride = surface->pitch;
    image->width = surface->w;

#if 1
    image->data = (uint8_t *) surface->pixels;
    image->free_function = (void (*)(void *)) SDL_FreeSurface;
    image->free_param = surface;
#else
    // It seems we cannot call free on surface->pixels. So right now we
    // allocate a new memory area and copy the pixels into it so we can
    // safely free it later. Alternatively, we could add a free callback
    // (free_function = SDL_FreeSurface, free_data = surface).

    image->data = (uint8_t *) malloc(surface->pitch * surface->h);
    memcpy(image->data, surface->pixels, surface->pitch * surface->h);
    SDL_FreeSurface(surface);
#endif

    // Do not delete the surface here (or rather, the image data) since
    // we reference and return this.
    // SDL_FreeSurface(text_surface);

    return image;
}
