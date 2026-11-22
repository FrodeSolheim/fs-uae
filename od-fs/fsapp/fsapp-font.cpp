/*
 * Fonts: loading them and drawing text, also from Python (_fsapp_font).
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#include "fsapp-font.h"

#include <Python.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "fsapp-pythonthread.h"
#include "fsapp-refable.h"
#include "fsapp-surface.h"
#include "fsapp-util.h"

// ----------------------------------------------------------------------------

struct fsapp_font {
    FSAPP_REFABLE;

    TTF_Font *font;
    TTF_TextEngine *engine;
    TTF_Text *text;
    // SDL_IOStream *rwops;
    // void *data;
};

// ----------------------------------------------------------------------------

static TTF_TextEngine *fsapp_font_engine;

// ----------------------------------------------------------------------------

int fsapp_font_log_level = SDL_LOG_PRIORITY_INFO;

#define fsapp_font_log(format, ...)                                                                \
    FS_LOG_INFO(fsapp_font_log_level, "[GUI] [FNT]", format, ##__VA_ARGS__)

#define fsapp_font_log_debug(format, ...)                                                          \
    FS_LOG_DEBUG(fsapp_font_log_level, "[GUI] [FNT]", format, ##__VA_ARGS__)

#define fsapp_font_log_error(format, ...)                                                          \
    FS_LOG_ERROR(fsapp_font_log_level, "[GUI] [FNT]", format, ##__VA_ARGS__)

// ----------------------------------------------------------------------------

void fsapp_font_ref(fsapp_font_t *font)
{
    fsapp_refable_ref(&font->refable);
}

void fsapp_font_unref(fsapp_font_t *font)
{
    fsapp_refable_unref(&font->refable);
}

static void fsapp_font_finalize(void *object)
{
    fsapp_font_t *font = (fsapp_font_t *)object;
    fsapp_font_log_debug("Finalizing font %p\n", font);
    if (font->text) {
        TTF_DestroyText(font->text);
    }
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

fsapp_font_t *fsapp_font_load(const char *name, int size)
{
    // fsapp_font_init();

    fsapp_font_t *font = FSAPP_UTIL_MALLOC0(fsapp_font_t);
    // printf("LEAK malloc %p\n", font);

    // FIXME: fsapp_refable?
    fsapp_refable_init_with_finalizer(&font->refable, fsapp_font_finalize);

    // void* data;
    // int data_size;
    // fsapp_data_load(name, &data, &data_size);
    // if (data == NULL) {
    //     free(font);
    //     return NULL;
    // }
    // fsapp_font_log_debug("Read %s (%d bytes)\n", name, data_size);
    // font->data = data;
    // font->rwops = SDL_IOFromMem(data, data_size);
    // font->font = TTF_OpenFontIO(font->rwops, 0, size);

    font->font = TTF_OpenFont(name, size);
    if (!font->font) {
        fsapp_font_log_error("TTF_OpenFont: %s\n", SDL_GetError());
        fsapp_font_unref(font);
        return NULL;
    }
    font->engine = fsapp_font_engine;
    SDL_assert(font->engine);
    font->text = TTF_CreateText(font->engine, font->font, "", 0);
    SDL_assert(font->text);

    // TTF_SetFontHinting(font->font, TTF_HINTING_LIGHT_SUBPIXEL);
    // TTF_SetFontHinting(font->font, TTF_HINTING_LIGHT);

    fsapp_font_log("Loaded %s (%d)\n", name, size);

    // FIXME: Never return null? Instead return a font which cannot render
    // anything? With a function to check if the font is OK...

    return font;
}

// ----------------------------------------------------------------------------

// The capsule holds the font's reference from fsapp_font_load, released on the UI thread, where the
// font was loaded and is drawn with. Dropped on another thread, the font is left unfreed.
// REVIEW: Nothing draws with a freed font: every drawing call takes the capsule, which a handle
// (fsapp.surfaceapi.v1.Font, fsgui's font cache) keeps alive while it may draw.
static void fsapp_font_python_destructor(PyObject *font_capsule)
{
    fsapp_font_t *font = (fsapp_font_t *)PyCapsule_GetPointer(font_capsule, "fsapp_font_t");
    if (font == NULL) {
        return;
    }
    if (!fsapp_pythonthread_can_free_ui_objects()) {
        fsapp_font_log_debug("Font %p dropped off the UI thread: not freed\n", font);
        return;
    }
    fsapp_font_unref(font);
}

// ----------------------------------------------------------------------------

static PyObject *fsapp_font_python_load(PyObject *self, PyObject *args)
{
    const char *name;
    int size;
    if (!PyArg_ParseTuple(args, "si:load", &name, &size)) {
        return NULL;
    }

    fsapp_font_t *font = fsapp_font_load(name, size);
    if (font == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not load font %s at size %d: %s", name, size,
                     SDL_GetError());
        PyErr_Format(PyExc_RuntimeError, "Could not load font %s at size %d: %s", name, size,
                     SDL_GetError());
        return NULL;
    }

    PyObject *capsule = PyCapsule_New(font, "fsapp_font_t", fsapp_font_python_destructor);
    return capsule;
}

// ----------------------------------------------------------------------------

static PyObject *fsapp_font_python_measure_text(PyObject *self, PyObject *args)
{
    PyObject *font_capsule;
    const char *string;
    if (!PyArg_ParseTuple(args, "Os:measure_text", &font_capsule, &string)) {
        return NULL;
    }
    fsapp_font_t *font = (fsapp_font_t *)PyCapsule_GetPointer(font_capsule, "fsapp_font_t");
    SDL_assert(font != NULL);
    int w, h;
    Py_BEGIN_ALLOW_THREADS;

    if (!TTF_GetStringSize(font->font, string, 0, &w, &h)) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "TTF_GetStringSize failed: %s", SDL_GetError());
    }

    Py_END_ALLOW_THREADS;
    return Py_BuildValue("(ii)", w, h);
}

// ----------------------------------------------------------------------------

static PyObject *fsapp_font_python_measure_text_wrapped(PyObject *self, PyObject *args)
{
    PyObject *font_capsule;
    const char *string;
    int wrap_width;
    if (!PyArg_ParseTuple(args, "Osi:measure_text_wrapped", &font_capsule, &string, &wrap_width)) {
        return NULL;
    }
    fsapp_font_t *font = (fsapp_font_t *)PyCapsule_GetPointer(font_capsule, "fsapp_font_t");
    SDL_assert(font != NULL);
    int w, h;

    Py_BEGIN_ALLOW_THREADS;

    if (!TTF_GetStringSizeWrapped(font->font, string, 0, wrap_width, &w, &h)) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "TTF_GetStringSizeWrapped failed: %s",
                    SDL_GetError());
    }

    Py_END_ALLOW_THREADS;
    return Py_BuildValue("(ii)", w, h);
}

// ----------------------------------------------------------------------------

static PyObject *fsapp_font_python_draw_text(PyObject *self, PyObject *args)
{
    PyObject *font_capsule, *surface_capsule;
    const char *string;
    int x, y;
    int r, g, b, a;
    fsapp_surface_clip_t clip = {};
    // FIXME: It seems like the text should not be freed (true)?
    if (!PyArg_ParseTuple(args, "OOs(ii)(iiii)|O&:draw_text", &font_capsule, &surface_capsule,
                          &string, &x, &y, &r, &g, &b, &a, fsapp_surface_convert_python_clip,
                          &clip)) {
        return NULL;
    }

    // printf("Render text: %s\n", string);

    // FIXME: Create/associate SDL_Surface directly with surface...?
    // FIXME: Maybe just use SDL_Surface always (surface->surface)

    fsapp_font_t *font = (fsapp_font_t *)PyCapsule_GetPointer(font_capsule, "fsapp_font_t");
    if (font == NULL) {
        return NULL;
    }

    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    if (surface == NULL) {
        return NULL;
    }

    Py_BEGIN_ALLOW_THREADS;

    int string_length = 0; // 0 for null terminated text
    TTF_SetTextString(font->text, string, string_length);
    TTF_SetTextColor(font->text, r, g, b, a);
    // REVIEW: Keeps the buffer premultiplied: SDL_ttf's surface text engine blits each glyph
    // (ARGB8888, white with the coverage as alpha, colour and alpha as SDL's colour and alpha
    // modulation) with SDL_BLENDMODE_BLEND, which gives src * srcA + dst * (1 - srcA) for colour
    // and srcA + dstA * (1 - srcA) for alpha. The engine's fill operations (underline and
    // strikethrough) would write the straight colour with SDL_FillSurfaceRect, but fsapp sets no
    // font style.
    fsapp_surface_begin_clip(surface, &clip);
    TTF_DrawSurfaceText(font->text, x, y, surface->sdl);
    fsapp_surface_end_clip(surface);

    Py_END_ALLOW_THREADS;
    Py_RETURN_NONE;
}

// ----------------------------------------------------------------------------

static PyObject *fsapp_font_python_draw_text_wrapped(PyObject *self, PyObject *args)
{
    PyObject *font_capsule, *surface_capsule;
    const char *string;
    int x, y, wrap_width;
    int r = 0, g = 0, b = 0, a = 255;
    fsapp_surface_clip_t clip = {};
    if (!PyArg_ParseTuple(args, "OOs(ii)i|(iiii)O&:draw_text_wrapped", &font_capsule,
                          &surface_capsule, &string, &x, &y, &wrap_width, &r, &g, &b, &a,
                          fsapp_surface_convert_python_clip, &clip)) {
        return NULL;
    }

    fsapp_font_t *font = (fsapp_font_t *)PyCapsule_GetPointer(font_capsule, "fsapp_font_t");
    if (font == NULL) {
        return NULL;
    }

    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    if (surface == NULL) {
        return NULL;
    }

    Py_BEGIN_ALLOW_THREADS;

    // FIXME: SDL3_ttf does not seem to have a function to draw wrapped text
    // directly to an existing surface, so we have to blit the temp surface...
    // (Check if there is another way?)
    SDL_Color color = {(Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a};

    SDL_Surface *temp_surface =
        TTF_RenderText_Blended_Wrapped(font->font, string, 0, color, wrap_width);

    SDL_Rect dest = {.x = x, .y = y};
    // REVIEW: Keeps the buffer premultiplied: SDL_ttf gives blended text a straight-alpha surface
    // with SDL_BLENDMODE_BLEND, which blends it as in fsapp_font_python_draw_text.
    fsapp_surface_begin_clip(surface, &clip);
    SDL_BlitSurface(temp_surface, NULL, surface->sdl, &dest);
    fsapp_surface_end_clip(surface);
    SDL_DestroySurface(temp_surface);

    Py_END_ALLOW_THREADS;
    Py_RETURN_NONE;
}

// ----------------------------------------------------------------------------

static PyMethodDef fsapp_font_python_methods[] = {
    {"load", fsapp_font_python_load, METH_VARARGS, "..."},
    {"measure_text", fsapp_font_python_measure_text, METH_VARARGS, "..."},
    {"measure_text_wrapped", fsapp_font_python_measure_text_wrapped, METH_VARARGS, "..."},
    {"draw_text", fsapp_font_python_draw_text, METH_VARARGS, "..."},
    {"draw_text_wrapped", fsapp_font_python_draw_text_wrapped, METH_VARARGS, "..."},
    {NULL, NULL, 0, NULL}};

static PyModuleDef fsapp_font_python_module = {PyModuleDef_HEAD_INIT,
                                               "_fsapp_font",
                                               NULL,
                                               -1,
                                               fsapp_font_python_methods,
                                               NULL,
                                               NULL,
                                               NULL,
                                               NULL};

static PyObject *fsapp_font_python_initfunc(void)
{
    return PyModule_Create(&fsapp_font_python_module);
}

// ----------------------------------------------------------------------------

void fsapp_font_init(void)
{
    static bool initialized;
    if (initialized) {
        return;
    }
    initialized = true;

    fsapp_font_log("Initializing font module\n");

    if (!TTF_Init()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't initialize SDL_ttf: %s", SDL_GetError());
        // FIXME: Error handling... ?
        // return SDL_APP_FAILURE;
    }

    fsapp_font_engine = TTF_CreateSurfaceTextEngine();

    PyImport_AppendInittab("_fsapp_font", &fsapp_font_python_initfunc);
}
