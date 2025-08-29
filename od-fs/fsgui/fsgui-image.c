// #define FSEMU_INTERNAL
#include "fsgui-image.h"

#include <Python.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <glib.h>

#include "fsapp-surface.h"
#include "fsemu-util.h"
#include "fslib-refable.h"

// ----------------------------------------------------------------------------

int fsgui_image_log_level = FSEMU_LOG_LEVEL_INFO;

#define fsgui_image_log(format, ...)                                   \
    if (fsemu_likely(fsgui_image_log_level >= FSEMU_LOG_LEVEL_INFO)) { \
        fsemu_log("[GUI] [IMG] " format, ##__VA_ARGS__);               \
    }

#define fsgui_image_log_debug(format, ...)                                \
    if (fsemu_unlikely(fsgui_image_log_level >= FSEMU_LOG_LEVEL_DEBUG)) { \
        fsemu_log("[GUI] [IMG] " format, ##__VA_ARGS__);                  \
    }

#define fsgui_image_log_error(format, ...)                              \
    if (fsemu_likely(fsgui_image_log_level >= FSEMU_LOG_LEVEL_ERROR)) { \
        fsemu_log("[GUI] [IMG] " format, ##__VA_ARGS__);                \
    }

#define fsgui_image_log_warning(format, ...)                              \
    if (fsemu_likely(fsgui_image_log_level >= FSEMU_LOG_LEVEL_WARNING)) { \
        fsemu_log("[GUI] [IMG] " format, ##__VA_ARGS__);                  \
    }

// ----------------------------------------------------------------------------

void fsgui_image_ref(fsgui_image_t* image) {
    fslib_refable_ref(image);
}

void fsgui_image_unref(fsgui_image_t* image) {
    fslib_refable_unref(image);
}

static void fsgui_image_finalize(void* object) {
    fsgui_image_t* image = (fsgui_image_t*)object;
    fsgui_image_log_debug("Finalizing image %p\n", image);

    SDL_DestroySurface(image->surface);
    free(image);
}

// ----------------------------------------------------------------------------

static void fsgui_image_init_9patch(fsgui_image_t* image) {
    uint32_t black = SDL_MapSurfaceRGBA(image->surface, 0, 0, 0, 255);
    int width = image->surface->w;
    int height = image->surface->h;

    image->left = 0;
    image->top = 0;
    image->bottom = 0;
    image->right = 0;

    uint32_t* pixels = (uint32_t*)image->surface->pixels;
    for (int x = 1; x < width - 1; x++) {
        uint32_t pixel = pixels[x];
        if (image->left == 0) {
            if (pixel == black) {
                // Left border size is same as position of first black pixel
                // minus 1 due to the transparent border.
                image->left = x - 1;
            }
        } else {
            if (pixel != black) {
                // Determine right border size
                image->right = width - x - 1;
                break;
            }
        }
    }

    int offset = width;
    for (int y = 1; y < image->surface->h - 1; y++) {
        uint32_t pixel = pixels[offset];
        if (image->top == 0) {
            if (pixel == black) {
                image->top = y - 1;
            }
        } else {
            if (pixel != black) {
                image->bottom = height - y - 1;
                break;
            }
        }
        offset += width;
    }

    fsgui_image_log_debug("9 patch image border left %d top %d right %d bottom %d\n", image->left,
                          image->top, image->right, image->bottom);
}

// ----------------------------------------------------------------------------

fsgui_image_t* fsgui_image_load(const char* name) {
    fsgui_image_t* image = FSEMU_UTIL_MALLOC0(fsgui_image_t);
    fslib_refable_init_with_finalizer(image, fsgui_image_finalize);

    const char* path = name;
    SDL_Surface* surface = IMG_Load(path);
    image->surface = surface;

    if (surface->format == SDL_PIXELFORMAT_RGBA32) {
        // SDL_PIXELFORMAT_ABGR8888 on little endian machines
        fsgui_image_log_debug("Pixel format RGBA32!\n");
        // } else if (surface->format == SDL_PIXELFORMAT_ARGB32) {
        //     printf("ARGB32!\n");
    } else if (surface->format == SDL_PIXELFORMAT_BGRA32) {
        // macOS ARM64?
        fsgui_image_log_debug("Pixel format BGRA32!\n");
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Pixel format (0x%x) is not RGBA32", surface->format);
        fsgui_image_log_warning("Neither ARGB32 nor RGBA32!\n");
    }

    if (image->surface->pitch != image->surface->w * 4) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Pitch is not width * 4");
    }

    if (g_str_has_suffix(path, ".9p.png")) {
        fsgui_image_init_9patch(image);
    }

    fsgui_image_log_debug("Loaded %s\n", name);

    return image;
}

// ----------------------------------------------------------------------------

static void fsgui_image_python_destructor(PyObject* image_capsule) {
#ifdef FSGUI_DEBUG_CLEANUP
    fsgui_image_log_warning("fsgui_image_python_destructor FIXME: Not implemented\n");
#endif
}

// ----------------------------------------------------------------------------

static PyObject* fsgui_image_python_load(PyObject* self, PyObject* args) {
    const char* name;
    if (!PyArg_ParseTuple(args, "s:load", &name)) {
        return NULL;
    }

    fsgui_image_t* image = fsgui_image_load(name);
    if (image == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not load image %s: %s", name, SDL_GetError());
        return NULL;
    }

    PyObject* capsule = PyCapsule_New(image, "fsgui_image_t", fsgui_image_python_destructor);
    return capsule;
}

// ----------------------------------------------------------------------------

static PyObject* fsgui_image_python_get_size(PyObject* self, PyObject* args) {
    PyObject* image_capsule;
    if (!PyArg_ParseTuple(args, "O:get_size", &image_capsule)) {
        return NULL;
    }

    fsgui_image_t* image = (fsgui_image_t*)PyCapsule_GetPointer(image_capsule, "fsgui_image_t");
    SDL_assert(image);

    // FIXME: Subtract left/right/top/bottom if a 9patch image?
    return Py_BuildValue("(ii)", image->surface->w, image->surface->h);
}

// ----------------------------------------------------------------------------

static PyObject* fsgui_image_python_draw(PyObject* self, PyObject* args) {
    PyObject *image_capsule, *surface_capsule;
    int x, y;
    if (!PyArg_ParseTuple(args, "OO(ii):draw", &image_capsule, &surface_capsule, &x, &y)) {
        return NULL;
    }
    fsgui_image_t* image = (fsgui_image_t*)PyCapsule_GetPointer(image_capsule, "fsgui_image_t");
    SDL_assert(image);

    fsapp_surface_t* surface =
        (fsapp_surface_t*)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface);

    // SDL_Rect src_rect = ;
    SDL_Rect dst_rect = {.x = x, .y = y};
    // printf("%d %d\n", image->surface->width, image->surface->height);
    // If you want a specific width and height, you should use
    // SDL_BlitSurfaceScaled().
    SDL_BlitSurface(image->surface, NULL, surface->sdl, &dst_rect);

    Py_RETURN_NONE;
}

// ----------------------------------------------------------------------------

static PyObject* fsgui_image_python_draw_stretched(PyObject* self, PyObject* args) {
    PyObject *image_capsule, *surface_capsule;
    int x, y, w, h;
    if (!PyArg_ParseTuple(args, "OO(ii)(ii):draw_stretched", &image_capsule, &surface_capsule, &x,
                          &y, &w, &h)) {
        return NULL;
    }
    fsgui_image_t* image = (fsgui_image_t*)PyCapsule_GetPointer(image_capsule, "fsgui_image_t");
    SDL_assert(image);

    fsapp_surface_t* surface =
        (fsapp_surface_t*)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface);

    // SDL_Rect src_rect = ;
    SDL_Rect dst_rect = {.x = x, .y = y, .w = w, .h = h};

    if (image->left) {
        SDL_Rect src_rect = {
            .x = 1, .y = 1, .w = image->surface->w - 2, .h = image->surface->h - 2};
        SDL_BlitSurface9Grid(image->surface, &src_rect, image->left, image->right, image->top,
                             image->bottom, 1.0, SDL_SCALEMODE_LINEAR, surface->sdl, &dst_rect);

    } else {
        SDL_BlitSurfaceScaled(image->surface, NULL, surface->sdl, &dst_rect, SDL_SCALEMODE_LINEAR);
    }

    Py_RETURN_NONE;
}

// ----------------------------------------------------------------------------

static PyMethodDef fsgui_image_python_methods[] = {
    {"draw", fsgui_image_python_draw, METH_VARARGS, "..."},
    {"draw_stretched", fsgui_image_python_draw_stretched, METH_VARARGS, "..."},
    {"load", fsgui_image_python_load, METH_VARARGS, "..."},
    {"get_size", fsgui_image_python_get_size, METH_VARARGS, "..."},
    {NULL, NULL, 0, NULL}};

static PyModuleDef fsgui_image_python_module = {PyModuleDef_HEAD_INIT,
                                                "fsgui_image",
                                                NULL,
                                                -1,
                                                fsgui_image_python_methods,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL};

static PyObject* fsgui_image_python_initfunc(void) {
    return PyModule_Create(&fsgui_image_python_module);
}

// ----------------------------------------------------------------------------

void fsgui_image_init_module(void) {
    static bool initialized;
    if (initialized) {
        return;
    }
    initialized = true;

    fsgui_image_log("Initializing image module\n");

    // FIXME: Who inits IMG?

    // if (!IMG_Init()) {
    //     SDL_LogError(
    //         SDL_LOG_CATEGORY_ERROR, "Couldn't initialize SDL_image: %s",
    //         SDL_GetError()
    //     );
    //     // FIXME: Error handling... ?
    //     // return SDL_APP_FAILURE;
    // }

    PyImport_AppendInittab("fsgui_image", &fsgui_image_python_initfunc);
}
