#include "fsgui-surface.h"

#include <Python.h>
#include <SDL3/SDL.h>

#include "fsemu-color.h"
#include "fsemu-data.h"
#include "fsemu-image.h"
#include "fsemu-sdl.h"
#include "fsemu-util.h"
#include "fsgui-font.h"
#include "fslib-refable.h"

// FIXME: Maybe rename to fsapp-surface / _fsapp_surface

// -------------------------------------------------------------------------------------------------

int fsgui_surface_log_level = FSEMU_LOG_LEVEL_INFO;

#define fsgui_surface_log(format, ...)                                   \
    if (fsemu_likely(fsgui_surface_log_level >= FSEMU_LOG_LEVEL_INFO)) { \
        fsemu_log("[GUI] [SFC] " format, ##__VA_ARGS__);                 \
    }

#define fsgui_surface_log_debug(format, ...)                                \
    if (fsemu_unlikely(fsgui_surface_log_level >= FSEMU_LOG_LEVEL_DEBUG)) { \
        fsemu_log("[GUI] [SFC] " format, ##__VA_ARGS__);                    \
    }

#define fsgui_surface_log_error(format, ...)                              \
    if (fsemu_likely(fsgui_surface_log_level >= FSEMU_LOG_LEVEL_ERROR)) { \
        fsemu_log("[GUI] [SFC] " format, ##__VA_ARGS__);                  \
    }

static GList* g_surface_list;
static GList* g_surface_opengl_free_list;

GList* fsapp_surface_get_list() {
    return g_surface_list;
}

GList* fsapp_surface_get_opengl_free_list() {
    GList* list = g_surface_opengl_free_list;
    g_surface_opengl_free_list = NULL;
    return list;
}

// -------------------------------------------------------------------------------------------------

void fsgui_surface_ref(fsgui_surface_t* surface) {
    fslib_refable_ref(surface);
}

void fsgui_surface_unref(fsgui_surface_t* surface) {
    fslib_refable_unref(surface);
}

static void fsgui_surface_finalize(void* object) {
#ifdef FSGUI_DEBUG_CLEANUP
    printf("SURFACE FINALIZE\n");
#endif

    printf("SURFACE FINALIZE\n");

    fsgui_surface_t* surface = (fsgui_surface_t*)object;
    fsgui_surface_log_debug("Finalizing surface %p\n", surface);

    // FIXME: Add to OpenGL free list - DONE - Handled differently

    free(surface);
}

// -------------------------------------------------------------------------------------------------

static void fsgui_surface_actually_create(fsgui_surface_t* surface, int width, int height) {
    SDL_Log("fsgui_surface_actually_create %d %d\n", width, height);
    surface->width = width;
    surface->height = height;

    if (surface->special > 0) {
        // Do not allocate data for special surfaces
        // (FIXME: Should maybe have flags also/instead)
    } else if (surface->width == 0 || surface->height == 0) {
        //
    } else if (surface->width < 0 || surface->height < 0) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Negative surface size %dx%d", width, height);
    } else {
        // Always 32-bit surfaces (RGBA), at least for now
        surface->sdl = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);
        // Other code assume that the pixel array is contiguous, so we check
        // for that already here.
        SDL_assert_release(surface->sdl->pitch == width * 4);
        surface->data = surface->sdl->pixels;
    }
}

// -------------------------------------------------------------------------------------------------

static void fsgui_surface_do_destroy(fsgui_surface_t* surface) {
#ifdef FSGUI_DEBUG_CLEANUP
    printf("fsgui_surface_python_destructor\n");
#endif
    SDL_assert(surface != NULL);

    if (surface->deleted) {
        // SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Surface already marked for deletion");
        //  Probably ran destroy explicitly
        return;
    }

    // So the capsule destructor does not try to double delete
    surface->deleted = true;

    if (surface->sdl != NULL) {
        SDL_DestroySurface(surface->sdl);
        surface->sdl = NULL;
        surface->data = NULL;
    }
    // Part of SDL surface
    // if (surface->data != NULL) {
    //     free(surface->data);
    //     surface->data = NULL;
    // }

    // Make sure the main thread gets to own a reference until it is done freeing OpenGL memory
    fsgui_surface_ref(surface);

    // Remove from surface list and add to surface free list
    g_surface_list = g_list_remove(g_surface_list, surface);
    g_surface_opengl_free_list = g_list_append(g_surface_opengl_free_list, surface);
}

// -------------------------------------------------------------------------------------------------

static void fsgui_surface_python_destructor(PyObject* surface_capsule) {
#ifdef FSGUI_DEBUG_CLEANUP
    printf("fsgui_surface_python_destructor\n");
#endif

    fsgui_surface_t* surface =
        (fsgui_surface_t*)PyCapsule_GetPointer(surface_capsule, "fsgui_surface_t");
    SDL_assert(surface != NULL);

    fsgui_surface_do_destroy(surface);

    fsgui_surface_unref(surface);
}

// -------------------------------------------------------------------------------------------------

static PyObject* fsgui_surface_python_create(PyObject* self, PyObject* args) {
    int width, height, special;
    if (!PyArg_ParseTuple(args, "(ii)i:create", &width, &height, &special)) return NULL;

    fsgui_surface_log_debug("Creating %dx%d surface\n", width, height);

    fsgui_surface_t* surface = (fsgui_surface_t*)malloc(sizeof(fsgui_surface_t));
    memset(surface, 0, sizeof(fsgui_surface_t));
    fslib_refable_init_with_finalizer(surface, fsgui_surface_finalize);

    surface->special = special;
    fsgui_surface_actually_create(surface, width, height);

    SDL_Log("Adding surface %dx%d to surface list", surface->width, surface->height);
    g_surface_list = g_list_append(g_surface_list, surface);

    PyObject* surface_capsule =
        PyCapsule_New(surface, "fsgui_surface_t", fsgui_surface_python_destructor);
    return surface_capsule;
}

// -------------------------------------------------------------------------------------------------

static PyObject* fsgui_surface_python_destroy(PyObject* self, PyObject* args) {
    PyObject* surface_capsule;
    if (!PyArg_ParseTuple(args, "O:destroy", &surface_capsule)) {
        return NULL;
    }

    fsgui_surface_t* surface =
        (fsgui_surface_t*)PyCapsule_GetPointer(surface_capsule, "fsgui_surface_t");
    SDL_assert(surface != NULL);
    fsgui_surface_do_destroy(surface);

    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject* fsgui_surface_python_resize(PyObject* self, PyObject* args) {
    // FIXME: For later - release the GIL while doing work!!

    PyObject* surface_capsule;
    int width, height;
    if (!PyArg_ParseTuple(args, "O(ii):resize_surface", &surface_capsule, &width, &height)) {
        return NULL;
    }

    fsgui_surface_t* surface =
        (fsgui_surface_t*)PyCapsule_GetPointer(surface_capsule, "fsgui_surface_t");
    SDL_assert(surface != NULL);

    // Don't need to re-blit old surface, just draw everything again (maybe)

    // FIXME: Release GIL?
    SDL_DestroySurface(surface->sdl);
    fsgui_surface_actually_create(surface, width, height);

    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject* fsgui_surface_python_set_position(PyObject* self, PyObject* args) {
    PyObject* surface_capsule;
    // float x, y;
    // if (!PyArg_ParseTuple(args, "O(ff):set_position", &surface_capsule, &x, &y)) {
    int x, y;
    if (!PyArg_ParseTuple(args, "O(ii):set_position", &surface_capsule, &x, &y)) {
        return NULL;
    }
    fsgui_surface_t* surface =
        (fsgui_surface_t*)PyCapsule_GetPointer(surface_capsule, "fsgui_surface_t");
    SDL_assert(surface != NULL);
    surface->x = x;
    surface->y = y;
    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject* fsgui_surface_python_set_z_index(PyObject* self, PyObject* args) {
    PyObject* surface_capsule;
    int z_index;
    if (!PyArg_ParseTuple(args, "Oi:set_z_index", &surface_capsule, &z_index)) {
        return NULL;
    }
    fsgui_surface_t* surface =
        (fsgui_surface_t*)PyCapsule_GetPointer(surface_capsule, "fsgui_surface_t");
    SDL_assert(surface != NULL);
    SDL_assert(z_index > -1000 && z_index < 1000);
    surface->z = z_index;
    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject* fsgui_surface_python_set_visible(PyObject* self, PyObject* args) {
    PyObject* surface_capsule;
    int visible;
    if (!PyArg_ParseTuple(args, "Oi:set_visible", &surface_capsule, &visible)) {
        return NULL;
    }
    fsgui_surface_t* surface =
        (fsgui_surface_t*)PyCapsule_GetPointer(surface_capsule, "fsgui_surface_t");
    SDL_assert(surface != NULL);
    surface->visible = visible != 0;
    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject* fsgui_surface_python_draw_filled_rectangle(PyObject* self, PyObject* args) {
    PyObject* surface_capsule;
    SDL_Rect rect;
    int r, g, b, a;
    if (!PyArg_ParseTuple(args, "O(ii)(ii)(iiii):draw_filled_rectangle", &surface_capsule, &rect.x,
                          &rect.y, &rect.w, &rect.h, &r, &g, &b, &a)) {
        return NULL;
    }

    fsgui_surface_t* surface =
        (fsgui_surface_t*)PyCapsule_GetPointer(surface_capsule, "fsgui_surface_t");
    SDL_assert(surface != NULL);

    // FIXME: ASSERT COLOR BYTES.. (0 - 255) - not masking
    // uint32_t color = (r << 24) | (g << 16) | (b << 8) | a;
    // FIXME: Support big-endian!
    // Using ABGR for little-endian machines

    uint32_t color = (a << 24) | (b << 16) | (g << 8) | r;
    // uint32_t color = SDL_MapRGBA(SDL_PIXELFORMAT_BGR48_FLOAT)

    // Supports SDL_SetSurfaceClipRect() !!!

    // FIXME: For later - release the GIL while doing work?

    SDL_FillSurfaceRect(surface->sdl, &rect, color);

    /*
    int32_t color = (a << 24) | (b << 16) | (g << 8) | r;
    // FIXME: Bounds checking!
    int32_t* pixels = (int32_t*)surface->data;
    int32_t* line_start = pixels + y * surface->width + x;

    for (int i = 0; i < h; i++) {
        int32_t* p = line_start;
        for (int j = 0; j < w; j++) {
            *(p++) = color;
        }
        line_start += surface->width;
    }
    */

    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject* fsgui_surface_python_draw_line(PyObject* self, PyObject* args) {
    // FIXME: For later - release the GIL while doing work!!

    PyObject* surface_capsule;
    int x1, y1, x2, y2;
    int r, g, b, a;
    if (!PyArg_ParseTuple(args, "O(ii)(ii)(iiii):draw_line", &surface_capsule, &x1, &y1, &x2, &y2,
                          &r, &g, &b, &a)) {
        return NULL;
    }

    fsgui_surface_t* surface =
        (fsgui_surface_t*)PyCapsule_GetPointer(surface_capsule, "fsgui_surface_t");
    SDL_assert(surface != NULL);

    // FIXME: Maybe we want to do bounds checking on release as well

    SDL_assert(x1 >= 0 && x2 < surface->width);
    SDL_assert(y1 >= 0 && y2 < surface->height);

    // FIXME: ASSERT COLOR BYTES.. (0 - 255) - not masking

    // Using ABGR for little-endian machines

    // uint32_t color = (a << 24) | (b << 16) | (g << 8) | r;

    // uint32_t color = (r << 24) | (g << 16) | (b << 8) | a;

    // FIXME: Support big-endian architectures?
    uint32_t color = (a << 24) | (b << 16) | (g << 8) | r;

    // FIXME: Check that x2 > x1
    // FIXME: Check that y2 > y1
    // FIXME: Out of bounds checking!
    // FIXME: Clip rect support

    int32_t* pixels = (int32_t*)surface->data;
    int32_t* p = pixels + y1 * surface->width + x1;
    if (y1 == y2) {
        for (int x = x1; x <= x2; x++) {
            // printf(">%d<\n", x);
            *(p++) = color;
        }
    } else if (x1 == x2) {
        for (int y = y1; y <= y2; y++) {
            *p = color;
            p += surface->width;
        }
    } else {
        // FIXME: WARNING: Unsupported case (not doing diagonal lines yet)
    }

    /*
    int32_t color = (a << 24) | (b << 16) | (g << 8) | r;
    // FIXME: Bounds checking!
    int32_t* pixels = (int32_t*)surface->data;
    int32_t* line_start = pixels + y * surface->width + x;

    for (int i = 0; i < h; i++) {
        int32_t* p = line_start;
        for (int j = 0; j < w; j++) {
            *(p++) = color;
        }
        line_start += surface->width;
    }
    */

    Py_RETURN_NONE;
}

static PyMethodDef fsgui_surface_python_methods[] = {
    {"create", fsgui_surface_python_create, METH_VARARGS, "..."},
    {"resize", fsgui_surface_python_resize, METH_VARARGS, "..."},
    {"set_position", fsgui_surface_python_set_position, METH_VARARGS, "..."},
    {"set_z_index", fsgui_surface_python_set_z_index, METH_VARARGS, "..."},
    {"set_visible", fsgui_surface_python_set_visible, METH_VARARGS, "..."},
    {"draw_filled_rectangle", fsgui_surface_python_draw_filled_rectangle, METH_VARARGS, "..."},
    {"draw_line", fsgui_surface_python_draw_line, METH_VARARGS, "..."},
    {"destroy", fsgui_surface_python_destroy, METH_VARARGS, "..."},
    {NULL, NULL, 0, NULL}};

static PyModuleDef fsgui_surface_python_module = {PyModuleDef_HEAD_INIT,
                                                  "fsgui_surface",
                                                  NULL,
                                                  -1,
                                                  fsgui_surface_python_methods,
                                                  NULL,
                                                  NULL,
                                                  NULL,
                                                  NULL};

static PyObject* fsgui_surface_python_initfunc(void) {
    return PyModule_Create(&fsgui_surface_python_module);
}

// -------------------------------------------------------------------------------------------------

void fsgui_surface_init_module(void) {
    static bool initialized;
    if (initialized) {
        return;
    }
    initialized = true;

    fsgui_surface_log("Initializing surface module\n");

    PyImport_AppendInittab("fsgui_surface", &fsgui_surface_python_initfunc);
}
