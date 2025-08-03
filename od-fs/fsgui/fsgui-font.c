#include "fsgui-font.h"

#include "fsemu-util.h"
#include "fsgui-surface.h"
#include "fslib-refable.h"

#include <Python.h>
#include <SDL3_ttf/SDL_ttf.h>

// ----------------------------------------------------------------------------

struct fsgui_font {
    FSLIB_REFABLE;

    TTF_Font* font;
    TTF_TextEngine* engine;
    TTF_Text* text;
    // SDL_IOStream *rwops;
    // void *data;
};

// ----------------------------------------------------------------------------

static TTF_TextEngine* fsgui_font_engine;

// ----------------------------------------------------------------------------

int fsgui_font_log_level = FSEMU_LOG_LEVEL_INFO;

#define fsgui_font_log(format, ...)                                           \
    if (fsemu_likely(fsgui_font_log_level >= FSEMU_LOG_LEVEL_INFO)) {         \
        fsemu_log("[GUI] [FNT] " format, ##__VA_ARGS__);                      \
    }

#define fsgui_font_log_debug(format, ...)                                     \
    if (fsemu_unlikely(fsgui_font_log_level >= FSEMU_LOG_LEVEL_DEBUG)) {      \
        fsemu_log("[GUI] [FNT] " format, ##__VA_ARGS__);                      \
    }

#define fsgui_font_log_error(format, ...)                                     \
    if (fsemu_likely(fsgui_font_log_level >= FSEMU_LOG_LEVEL_ERROR)) {        \
        fsemu_log("[GUI] [FNT] " format, ##__VA_ARGS__);                      \
    }

// ----------------------------------------------------------------------------

void fsgui_font_ref(fsgui_font_t* font)
{
    fslib_refable_ref(font);
}

void fsgui_font_unref(fsgui_font_t* font)
{
    fslib_refable_unref(font);
}

static void fsgui_font_finalize(void* object)
{
    fsgui_font_t* font = (fsgui_font_t*)object;
    fsgui_font_log_debug("Finalizing font %p\n", font);
    if (font->font) {
        TTF_CloseFont(font->font);
    }
    // if (font->rwops) {
    //     SDL_CloseIO(font->rwops);
    // }
    // if (font->data) {
    //     free(font->data);
    // }
    // printf("LEAK free %p\n", font);
    free(font);
}

// ----------------------------------------------------------------------------

fsgui_font_t* fsgui_font_load(const char* name, int size)
{
    // fsgui_font_init();

    fsgui_font_t* font = FSEMU_UTIL_MALLOC0(fsgui_font_t);
    // printf("LEAK malloc %p\n", font);

    // FIXME: fslib_refable?
    fslib_refable_init_with_finalizer(font, fsgui_font_finalize);

    // void* data;
    // int data_size;
    // fsemu_data_load(name, &data, &data_size);
    // if (data == NULL) {
    //     free(font);
    //     return NULL;
    // }
    // fsgui_font_log_debug("Read %s (%d bytes)\n", name, data_size);
    // font->data = data;
    // font->rwops = SDL_IOFromMem(data, data_size);
    // font->font = TTF_OpenFontIO(font->rwops, 0, size);

    font->font = TTF_OpenFont(name, size);
    SDL_assert(font->font);
    font->engine = fsgui_font_engine;
    SDL_assert(font->engine);
    font->text = TTF_CreateText(font->engine, font->font, "", 0);
    SDL_assert(font->text);

    // TTF_SetFontHinting(font->font, TTF_HINTING_LIGHT_SUBPIXEL);
    // TTF_SetFontHinting(font->font, TTF_HINTING_LIGHT);

    // FIXME: Must data remain valid?
    // free(data);

    if (!font->font) {
        fsgui_font_log_error("TTF_OpenFontRW: %s\n", SDL_GetError());
        fsgui_font_unref(font);
        return NULL;
    }

    fsgui_font_log("Loaded %s (%d)\n", name, size);

    // FIXME: Never return null? Instead return a font which cannot render
    // anything? With a function to check if the font is OK...

    return font;
}

// ----------------------------------------------------------------------------

static void fsgui_font_python_destructor(PyObject* font_capsule)
{
#ifdef FSGUI_DEBUG_CLEANUP
    printf("fsgui_font_python_destructor FIXME: Not implemented\n");
#endif
}

// ----------------------------------------------------------------------------

static PyObject* fsgui_font_python_load(PyObject* self, PyObject* args)
{
    const char* name;
    int size;
    if (!PyArg_ParseTuple(args, "si:load", &name, &size)) {
        return NULL;
    }

    fsgui_font_t* font = fsgui_font_load(name, size);
    if (font == NULL) {
        SDL_LogError(
            SDL_LOG_CATEGORY_ERROR, "Could not load font %s at size %d: %s",
            name, size, SDL_GetError()
        );
        return NULL;
    }

    PyObject* capsule
        = PyCapsule_New(font, "fsgui_font_t", fsgui_font_python_destructor);
    return capsule;
}

// ----------------------------------------------------------------------------

static PyObject* fsgui_font_python_measure_text(PyObject* self, PyObject* args)
{
    PyObject* font_capsule;
    const char* string;
    if (!PyArg_ParseTuple(args, "Os:measure_text", &font_capsule, &string)) {
        return NULL;
    }
    fsgui_font_t* font
        = (fsgui_font_t*)PyCapsule_GetPointer(font_capsule, "fsgui_font_t");
    SDL_assert(font != NULL);
    int w, h;
    Py_BEGIN_ALLOW_THREADS;

    if (!TTF_GetStringSize(font->font, string, 0, &w, &h)) {
        SDL_LogWarn(
            SDL_LOG_CATEGORY_APPLICATION, "TTF_GetStringSize failed: %s",
            SDL_GetError()
        );
    }

    Py_END_ALLOW_THREADS;
    return Py_BuildValue("(ii)", w, h);
}

// ----------------------------------------------------------------------------

static PyObject*
fsgui_font_python_measure_text_wrapped(PyObject* self, PyObject* args)
{
    PyObject* font_capsule;
    const char* string;
    int wrap_width;
    if (!PyArg_ParseTuple(
            args, "Osi:measure_text_wrapped", &font_capsule, &string,
            &wrap_width
        )) {
        return NULL;
    }
    fsgui_font_t* font
        = (fsgui_font_t*)PyCapsule_GetPointer(font_capsule, "fsgui_font_t");
    SDL_assert(font != NULL);
    int w, h;

    Py_BEGIN_ALLOW_THREADS;

    if (!TTF_GetStringSizeWrapped(font->font, string, 0, wrap_width, &w, &h)) {
        SDL_LogWarn(
            SDL_LOG_CATEGORY_APPLICATION,
            "TTF_GetStringSizeWrapped failed: %s", SDL_GetError()
        );
    }

    Py_END_ALLOW_THREADS;
    return Py_BuildValue("(ii)", w, h);
}

// ----------------------------------------------------------------------------

static PyObject* fsgui_font_python_draw_text(PyObject* self, PyObject* args)
{
    PyObject *font_capsule, *surface_capsule;
    const char* string;
    int x, y;
    int r, g, b, a;
    // FIXME: It seems like the text should not be freed (true)?
    if (!PyArg_ParseTuple(
            args, "OOs(ii)(iiii):draw_text", &font_capsule, &surface_capsule,
            &string, &x, &y, &r, &g, &b, &a
        )) {
        return NULL;
    }

    // printf("Render text: %s\n", string);

    // FIXME: Create/associate SDL_Surface directly with surface...?
    // FIXME: Maybe just use SDL_Surface always (surface->surface)

    fsgui_font_t* font
        = (fsgui_font_t*)PyCapsule_GetPointer(font_capsule, "fsgui_font_t");
    if (font == NULL) {
        return NULL;
    }

    fsgui_surface_t* surface = (fsgui_surface_t*)PyCapsule_GetPointer(
        surface_capsule, "fsgui_surface_t"
    );
    if (surface == NULL) {
        return NULL;
    }

    Py_BEGIN_ALLOW_THREADS;

    int string_length = 0; // 0 for null terminated text
    TTF_SetTextString(font->text, string, string_length);
    TTF_SetTextColor(font->text, r, g, b, a);
    TTF_DrawSurfaceText(font->text, x, y, surface->sdl);

    Py_END_ALLOW_THREADS;
    Py_RETURN_NONE;
}

// ----------------------------------------------------------------------------

static PyObject*
fsgui_font_python_draw_text_wrapped(PyObject* self, PyObject* args)
{
    PyObject *font_capsule, *surface_capsule;
    const char* string;
    int x, y, wrap_width;
    if (!PyArg_ParseTuple(
            args, "OOs(ii)i:draw_text", &font_capsule, &surface_capsule,
            &string, &x, &y, &wrap_width
        )) {
        return NULL;
    }

    fsgui_font_t* font
        = (fsgui_font_t*)PyCapsule_GetPointer(font_capsule, "fsgui_font_t");
    if (font == NULL) {
        return NULL;
    }

    fsgui_surface_t* surface = (fsgui_surface_t*)PyCapsule_GetPointer(
        surface_capsule, "fsgui_surface_t"
    );
    if (surface == NULL) {
        return NULL;
    }

    Py_BEGIN_ALLOW_THREADS;

    // FIXME: SDL3_ttf does not seem to have a function to draw wrapped text
    // directly to an existing surface, so we have to blit the temp surface...
    // (Check if there is another way?)
    SDL_Color color = { .r = 0, .g = 0, .b = 0, .a = 255 };

    SDL_Surface* temp_surface = TTF_RenderText_Blended_Wrapped(
        font->font, string, 0, color, wrap_width
    );

    SDL_Rect dest = { .x = x, .y = y };
    SDL_BlitSurface(temp_surface, NULL, surface->sdl, &dest);
    SDL_DestroySurface(temp_surface);

    Py_END_ALLOW_THREADS;
    Py_RETURN_NONE;
}

// ----------------------------------------------------------------------------

static PyMethodDef fsgui_font_python_methods[]
    = { { "load", fsgui_font_python_load, METH_VARARGS, "..." },
        { "measure_text", fsgui_font_python_measure_text, METH_VARARGS,
          "..." },
        { "measure_text_wrapped", fsgui_font_python_measure_text_wrapped,
          METH_VARARGS, "..." },
        { "draw_text", fsgui_font_python_draw_text, METH_VARARGS, "..." },
        { "draw_text_wrapped", fsgui_font_python_draw_text_wrapped,
          METH_VARARGS, "..." },
        { NULL, NULL, 0, NULL } };

static PyModuleDef fsgui_font_python_module = { PyModuleDef_HEAD_INIT,
                                                "fsgui_font",
                                                NULL,
                                                -1,
                                                fsgui_font_python_methods,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL };

static PyObject* fsgui_font_python_initfunc(void)
{
    return PyModule_Create(&fsgui_font_python_module);
}

// ----------------------------------------------------------------------------

void fsgui_font_init_module(void)
{
    static bool initialized;
    if (initialized) {
        return;
    }
    initialized = true;

    fsgui_font_log("Initializing font module\n");

    if (!TTF_Init()) {
        SDL_LogError(
            SDL_LOG_CATEGORY_ERROR, "Couldn't initialize SDL_ttf: %s",
            SDL_GetError()
        );
        // FIXME: Error handling... ?
        // return SDL_APP_FAILURE;
    }

    fsgui_font_engine = TTF_CreateSurfaceTextEngine();

    PyImport_AppendInittab("fsgui_font", &fsgui_font_python_initfunc);
}
