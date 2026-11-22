/*
 * Images for the UI: loading, sizing SVG images, tinting, reading pixels and drawing, also from
 * Python (_fsapp_image).
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#include "fsapp-image.h"

#include <Python.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "fsapp-pythonthread.h"
#include "fsapp-refable.h"
#include "fsapp-surface.h"
#include "fsapp-util.h"

// ----------------------------------------------------------------------------

int fsapp_image_log_level = SDL_LOG_PRIORITY_INFO;

#define fsapp_image_log(format, ...)                                                               \
    FS_LOG_INFO(fsapp_image_log_level, "[GUI] [IMG]", format, ##__VA_ARGS__)

#define fsapp_image_log_debug(format, ...)                                                         \
    FS_LOG_DEBUG(fsapp_image_log_level, "[GUI] [IMG]", format, ##__VA_ARGS__)

#define fsapp_image_log_error(format, ...)                                                         \
    FS_LOG_ERROR(fsapp_image_log_level, "[GUI] [IMG]", format, ##__VA_ARGS__)

#define fsapp_image_log_warning(format, ...)                                                       \
    FS_LOG_WARNING(fsapp_image_log_level, "[GUI] [IMG]", format, ##__VA_ARGS__)

// ----------------------------------------------------------------------------

void fsapp_image_ref(fsapp_image_t *image)
{
    fsapp_refable_ref(&image->refable);
}

void fsapp_image_unref(fsapp_image_t *image)
{
    fsapp_refable_unref(&image->refable);
}

static void fsapp_image_finalize(void *object)
{
    fsapp_image_t *image = (fsapp_image_t *)object;
    fsapp_image_log_debug("Finalizing image %p\n", image);

    SDL_DestroySurface(image->surface);
    free(image);
}

// ----------------------------------------------------------------------------

fsapp_image_t *fsapp_image_load(const char *name)
{
    fsapp_image_t *image = FSAPP_UTIL_MALLOC0(fsapp_image_t);
    fsapp_refable_init_with_finalizer(&image->refable, fsapp_image_finalize);

    const char *path = name;
    SDL_Surface *surface = IMG_Load(path);
    if (!surface) {
        fsapp_image_log_error("Could not load image %s: %s", path, SDL_GetError());
        free(image);
        return NULL;
    }
    image->surface = surface;

    if (surface->format == SDL_PIXELFORMAT_RGBA32) {
        // SDL_PIXELFORMAT_ABGR8888 on little endian machines
        fsapp_image_log_debug("Pixel format RGBA32!\n");
        // } else if (surface->format == SDL_PIXELFORMAT_ARGB32) {
        //     printf("ARGB32!\n");
    } else if (surface->format == SDL_PIXELFORMAT_BGRA32) {
        // macOS ARM64?
        fsapp_image_log_debug("Pixel format BGRA32!\n");
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Pixel format (0x%x) is not RGBA32", surface->format);
        fsapp_image_log_warning("Neither ARGB32 nor RGBA32!\n");
    }

    if (image->surface->pitch != image->surface->w * 4) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Pitch is not width * 4");
    }

    fsapp_image_log_debug("Loaded %s\n", name);

    return image;
}

fsapp_image_t *fsapp_image_load_sized_svg(const char *name, int width, int height)
{
    fsapp_image_t *image = FSAPP_UTIL_MALLOC0(fsapp_image_t);
    fsapp_refable_init_with_finalizer(&image->refable, fsapp_image_finalize);

    const char *path = name;

    // Open the file as an IO stream for SDL_image
    SDL_IOStream *io = SDL_IOFromFile(path, "rb");
    if (!io) {
        fsapp_image_log_error("Could not open SVG file %s: %s", path, SDL_GetError());
        free(image);
        return NULL;
    }

    // Use IMG_LoadSizedSVG_IO to load the SVG at the specified size. When SDL_image is built
    // without SVG support, this returns NULL without setting an error, so clear any stale error.
    SDL_ClearError();
    SDL_Surface *surface = IMG_LoadSizedSVG_IO(io, width, height);
    SDL_CloseIO(io);

    if (!surface) {
        fsapp_image_log_error("Could not load sized SVG %s: %s", path, SDL_GetError());
        free(image);
        return NULL;
    }

    image->surface = surface;

    if (surface->format == SDL_PIXELFORMAT_RGBA32) {
        fsapp_image_log_debug("Sized SVG pixel format RGBA32!\n");
    } else if (surface->format == SDL_PIXELFORMAT_BGRA32) {
        fsapp_image_log_debug("Sized SVG pixel format BGRA32!\n");
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Sized SVG pixel format (0x%x) is not RGBA32",
                     surface->format);
        fsapp_image_log_warning("Sized SVG neither ARGB32 nor RGBA32!\n");
    }

    if (image->surface->pitch != image->surface->w * 4) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Sized SVG pitch is not width * 4");
    }

    fsapp_image_log_debug("Loaded sized SVG %s (%dx%d)\n", name, width, height);

    return image;
}

// REVIEW: The tint changes the image itself, not a surface buffer, so its result stays straight
// alpha like the rest of the image: drawing the image blends it into the buffer, which
// premultiplies it (fsapp_image_python_draw). Premultiplying here would multiply by the alpha
// twice. The tint's alpha multiplies each pixel's (255 keeps the image's shape as it is).
void fsapp_image_tint(fsapp_image_t *image, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    if (!image || !image->surface) {
        fsapp_image_log_error("Cannot tint null image or surface\n");
        return;
    }

    SDL_Surface *surface = image->surface;

    // Lock the surface for direct pixel access (if needed)
    bool surface_was_locked = false;
    if (SDL_MUSTLOCK(surface)) {
        if (SDL_LockSurface(surface) != 0) {
            fsapp_image_log_error("Could not lock surface for tinting: %s\n", SDL_GetError());
            return;
        }
        surface_was_locked = true;
        fsapp_image_log("Surface locked for tinting\n");
    } else {
        fsapp_image_log("Surface does not require locking\n");
    }

    int width = surface->w;
    int height = surface->h;
    uint32_t *pixels = (uint32_t *)surface->pixels;

    fsapp_image_log("Tinting %dx%d image with RGBA(%d, %d, %d, %d)\n", width, height, r, g, b, a);

    // Get pixel format details for SDL3 API
    const SDL_PixelFormatDetails *format_details = SDL_GetPixelFormatDetails(surface->format);
    if (!format_details) {
        fsapp_image_log_error("Could not get pixel format details: %s\n", SDL_GetError());
        if (surface_was_locked) {
            SDL_UnlockSurface(surface);
        }
        return;
    }

    // Tint each pixel
    int pixels_processed = 0;
    int non_transparent_pixels = 0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint32_t pixel = pixels[y * width + x];
            uint8_t pixel_r, pixel_g, pixel_b, pixel_a;

            // Extract RGBA components (SDL3 API)
            SDL_GetRGBA(pixel, format_details, NULL, &pixel_r, &pixel_g, &pixel_b, &pixel_a);

            // Debug first few pixels
            if (pixels_processed < 5 && pixel_a > 0) {
                fsapp_image_log("Pixel %d: Original RGBA(%d, %d, %d, %d)\n", pixels_processed,
                                pixel_r, pixel_g, pixel_b, pixel_a);
            }

            // Only tint non-transparent pixels
            if (pixel_a > 0) {
                non_transparent_pixels++;

                // Apply tint color - use replacement tinting for better results
                // For white tint (255,255,255), replace the color but keep the shape
                uint8_t new_r = r;
                uint8_t new_g = g;
                uint8_t new_b = b;
                uint8_t new_a = (uint8_t)((pixel_a * a + 127) / 255);

                // Debug first few pixels after tinting
                if (pixels_processed < 5) {
                    fsapp_image_log("Pixel %d: Tinted RGBA(%d, %d, %d, %d)\n", pixels_processed,
                                    new_r, new_g, new_b, new_a);
                }

                // Create new pixel and set it back
                uint32_t new_pixel = SDL_MapRGBA(format_details, NULL, new_r, new_g, new_b, new_a);
                pixels[y * width + x] = new_pixel;

                pixels_processed++;
            }
        }
    }

    fsapp_image_log("Processed %d non-transparent pixels out of %d total pixels\n",
                    non_transparent_pixels, width * height);

    // Unlock surface only if we locked it
    if (surface_was_locked) {
        SDL_UnlockSurface(surface);
        fsapp_image_log("Surface unlocked after tinting\n");
    }

    fsapp_image_log("Tinted image with color (%d, %d, %d, %d)\n", r, g, b, a);
}

fsapp_image_t *fsapp_image_create_tinted(fsapp_image_t *image, uint8_t r, uint8_t g, uint8_t b,
                                         uint8_t a)
{
    // fsapp_image_tint works on 32-bit pixels with alpha, as images are loaded.
    SDL_PixelFormat format = image->surface->format;
    if (format != SDL_PIXELFORMAT_RGBA32 && format != SDL_PIXELFORMAT_BGRA32) {
        format = SDL_PIXELFORMAT_RGBA32;
    }
    SDL_Surface *surface = SDL_ConvertSurface(image->surface, format);
    if (surface == NULL) {
        return NULL;
    }
    fsapp_image_t *tinted = FSAPP_UTIL_MALLOC0(fsapp_image_t);
    fsapp_refable_init_with_finalizer(&tinted->refable, fsapp_image_finalize);
    tinted->surface = surface;
    fsapp_image_tint(tinted, r, g, b, a);
    return tinted;
}

// ----------------------------------------------------------------------------

// The capsule holds the image's reference from its load, released on the UI thread, where the
// image was loaded and is drawn. Dropped on another thread, the image is left unfreed.
static void fsapp_image_python_destructor(PyObject *image_capsule)
{
    fsapp_image_t *image = (fsapp_image_t *)PyCapsule_GetPointer(image_capsule, "fsapp_image_t");
    if (image == NULL) {
        return;
    }
    if (!fsapp_pythonthread_can_free_ui_objects()) {
        fsapp_image_log_debug("Image %p dropped off the UI thread: not freed\n", image);
        return;
    }
    fsapp_image_unref(image);
}

// ----------------------------------------------------------------------------

static PyObject *fsapp_image_python_load(PyObject *self, PyObject *args)
{
    const char *name;
    if (!PyArg_ParseTuple(args, "s:load", &name)) {
        return NULL;
    }

    fsapp_image_t *image = fsapp_image_load(name);
    if (image == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not load image %s: %s", name, SDL_GetError());
        PyErr_Format(PyExc_RuntimeError, "Could not load image %s: %s", name, SDL_GetError());
        return NULL;
    }

    PyObject *capsule = PyCapsule_New(image, "fsapp_image_t", fsapp_image_python_destructor);
    return capsule;
}

static PyObject *fsapp_image_python_load_sized_svg(PyObject *self, PyObject *args)
{
    const char *name;
    int width, height;
    if (!PyArg_ParseTuple(args, "sii:load_sized_svg", &name, &width, &height)) {
        return NULL;
    }

    fsapp_image_t *image = fsapp_image_load_sized_svg(name, width, height);
    if (image == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not load sized SVG %s: %s", name,
                     SDL_GetError());
        PyErr_Format(PyExc_RuntimeError, "Could not load sized SVG %s: %s", name, SDL_GetError());
        return NULL;
    }

    PyObject *capsule = PyCapsule_New(image, "fsapp_image_t", fsapp_image_python_destructor);
    return capsule;
}

static PyObject *fsapp_image_python_tint(PyObject *self, PyObject *args)
{
    PyObject *image_capsule;
    int r, g, b, a;
    if (!PyArg_ParseTuple(args, "Oiiii:tint", &image_capsule, &r, &g, &b, &a)) {
        return NULL;
    }

    fsapp_image_t *image = (fsapp_image_t *)PyCapsule_GetPointer(image_capsule, "fsapp_image_t");
    if (!image) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Invalid image capsule for tinting");
        return NULL;
    }

    fsapp_image_tint(image, (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a);

    Py_RETURN_NONE;
}

// _fsapp_image.create_tinted(image, (r, g, b, a)): a tinted copy, as a new capsule.
static PyObject *fsapp_image_python_create_tinted(PyObject *self, PyObject *args)
{
    PyObject *image_capsule;
    int r, g, b, a;
    if (!PyArg_ParseTuple(args, "O(iiii):create_tinted", &image_capsule, &r, &g, &b, &a)) {
        return NULL;
    }
    fsapp_image_t *image = (fsapp_image_t *)PyCapsule_GetPointer(image_capsule, "fsapp_image_t");
    if (image == NULL) {
        return NULL;
    }
    fsapp_image_t *tinted =
        fsapp_image_create_tinted(image, (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a);
    if (tinted == NULL) {
        PyErr_Format(PyExc_RuntimeError, "Could not copy the image: %s", SDL_GetError());
        return NULL;
    }
    return PyCapsule_New(tinted, "fsapp_image_t", fsapp_image_python_destructor);
}

// _fsapp_image.get_pixels(image): the image's pixels as bytes, premultiplied, in the surface
// buffers' format (bytes B, G, R, A on little-endian hosts), rows of width * 4 bytes.
static PyObject *fsapp_image_python_get_pixels(PyObject *self, PyObject *args)
{
    PyObject *image_capsule;
    if (!PyArg_ParseTuple(args, "O:get_pixels", &image_capsule)) {
        return NULL;
    }
    fsapp_image_t *image = (fsapp_image_t *)PyCapsule_GetPointer(image_capsule, "fsapp_image_t");
    if (image == NULL) {
        return NULL;
    }
    SDL_Surface *converted = SDL_ConvertSurface(image->surface, SDL_PIXELFORMAT_ARGB8888);
    if (converted == NULL || !SDL_PremultiplySurfaceAlpha(converted, false)) {
        PyErr_Format(PyExc_RuntimeError, "Could not convert the image: %s", SDL_GetError());
        SDL_DestroySurface(converted);
        return NULL;
    }
    int row_size = converted->w * 4;
    PyObject *pixels = PyBytes_FromStringAndSize(NULL, (Py_ssize_t)row_size * converted->h);
    if (pixels != NULL) {
        char *data = PyBytes_AS_STRING(pixels);
        for (int y = 0; y < converted->h; y++) {
            SDL_memcpy(data + (size_t)y * row_size,
                       (Uint8 *)converted->pixels + y * converted->pitch, row_size);
        }
    }
    SDL_DestroySurface(converted);
    return pixels;
}

// ----------------------------------------------------------------------------

static PyObject *fsapp_image_python_get_size(PyObject *self, PyObject *args)
{
    PyObject *image_capsule;
    if (!PyArg_ParseTuple(args, "O:get_size", &image_capsule)) {
        return NULL;
    }

    fsapp_image_t *image = (fsapp_image_t *)PyCapsule_GetPointer(image_capsule, "fsapp_image_t");
    SDL_assert(image);

    return Py_BuildValue("(ii)", image->surface->w, image->surface->h);
}

// ----------------------------------------------------------------------------

static PyObject *fsapp_image_python_draw(PyObject *self, PyObject *args)
{
    PyObject *image_capsule, *surface_capsule;
    int x, y;
    fsapp_surface_clip_t clip = {};
    if (!PyArg_ParseTuple(args, "OO(ii)|O&:draw", &image_capsule, &surface_capsule, &x, &y,
                          fsapp_surface_convert_python_clip, &clip)) {
        return NULL;
    }
    fsapp_image_t *image = (fsapp_image_t *)PyCapsule_GetPointer(image_capsule, "fsapp_image_t");
    SDL_assert(image);

    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface);

    // SDL_Rect src_rect = ;
    SDL_Rect dst_rect = {.x = x, .y = y};
    // printf("%d %d\n", image->surface->width, image->surface->height);
    // If you want a specific width and height, you should use
    // SDL_BlitSurfaceScaled().
    // REVIEW: Keeps the buffer premultiplied. The image is straight alpha (as loaded, also after
    // fsapp_image_tint). With an alpha channel, SDL gives it SDL_BLENDMODE_BLEND (fsapp never
    // changes it), which gives src * srcA + dst * (1 - srcA) for colour and
    // srcA + dstA * (1 - srcA) for alpha: premultiplied over a premultiplied buffer. Without one,
    // the blit copies with alpha 255, which is premultiplied as it is.
    fsapp_surface_begin_clip(surface, &clip);
    SDL_BlitSurface(image->surface, NULL, surface->sdl, &dst_rect);
    fsapp_surface_end_clip(surface);

    Py_RETURN_NONE;
}

// ----------------------------------------------------------------------------

// _fsapp_image.draw_part(image, surface, source, destination, filter, clip=None): the source
// rectangle of the image (x, y, width, height) drawn into the destination rectangle, scaled with
// the filter (FILTER_NEAREST or FILTER_LINEAR) when the sizes differ.
static PyObject *fsapp_image_python_draw_part(PyObject *self, PyObject *args)
{
    PyObject *image_capsule, *surface_capsule;
    SDL_Rect source, destination;
    int filter;
    fsapp_surface_clip_t clip = {};
    if (!PyArg_ParseTuple(args, "OO(iiii)(iiii)i|O&:draw_part", &image_capsule, &surface_capsule,
                          &source.x, &source.y, &source.w, &source.h, &destination.x,
                          &destination.y, &destination.w, &destination.h, &filter,
                          fsapp_surface_convert_python_clip, &clip)) {
        return NULL;
    }
    if (filter != SDL_SCALEMODE_NEAREST && filter != SDL_SCALEMODE_LINEAR) {
        PyErr_Format(PyExc_ValueError, "Not a filter: %d", filter);
        return NULL;
    }
    fsapp_image_t *image = (fsapp_image_t *)PyCapsule_GetPointer(image_capsule, "fsapp_image_t");
    if (image == NULL) {
        return NULL;
    }
    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    if (surface == NULL) {
        return NULL;
    }
    if (source.w <= 0 || source.h <= 0 || destination.w <= 0 || destination.h <= 0) {
        Py_RETURN_NONE;
    }

    // REVIEW: Keeps the buffer premultiplied, as fsapp_image_python_draw: scaled with
    // SDL_BLENDMODE_BLEND, SDL scales into a temporary surface and then blends that with the
    // image's blend mode.
    fsapp_surface_begin_clip(surface, &clip);
    if (source.w == destination.w && source.h == destination.h) {
        SDL_BlitSurface(image->surface, &source, surface->sdl, &destination);
    } else {
        SDL_BlitSurfaceScaled(image->surface, &source, surface->sdl, &destination,
                              (SDL_ScaleMode)filter);
    }
    fsapp_surface_end_clip(surface);

    Py_RETURN_NONE;
}

// ----------------------------------------------------------------------------

static PyMethodDef fsapp_image_python_methods[] = {
    {"draw", fsapp_image_python_draw, METH_VARARGS, "..."},
    {"draw_part", fsapp_image_python_draw_part, METH_VARARGS, "..."},
    {"create_tinted", fsapp_image_python_create_tinted, METH_VARARGS, "..."},
    {"get_pixels", fsapp_image_python_get_pixels, METH_VARARGS, "..."},
    {"load", fsapp_image_python_load, METH_VARARGS, "..."},
    {"load_sized_svg", fsapp_image_python_load_sized_svg, METH_VARARGS, "..."},
    {"tint", fsapp_image_python_tint, METH_VARARGS, "..."},
    {"get_size", fsapp_image_python_get_size, METH_VARARGS, "..."},
    {NULL, NULL, 0, NULL}};

static PyModuleDef fsapp_image_python_module = {PyModuleDef_HEAD_INIT,
                                                "_fsapp_image",
                                                NULL,
                                                -1,
                                                fsapp_image_python_methods,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL};

static PyObject *fsapp_image_python_initfunc(void)
{
    PyObject *module = PyModule_Create(&fsapp_image_python_module);
    if (module == NULL) {
        return NULL;
    }
    // The filters of draw_part.
    if (PyModule_AddIntConstant(module, "FILTER_NEAREST", SDL_SCALEMODE_NEAREST) < 0 ||
        PyModule_AddIntConstant(module, "FILTER_LINEAR", SDL_SCALEMODE_LINEAR) < 0) {
        Py_DECREF(module);
        return NULL;
    }
    return module;
}

// ----------------------------------------------------------------------------

void fsapp_image_init(void)
{
    static bool initialized;
    if (initialized) {
        return;
    }
    initialized = true;

    fsapp_image_log("Initializing image module\n");

    // FIXME: Who inits IMG?

    // if (!IMG_Init()) {
    //     SDL_LogError(
    //         SDL_LOG_CATEGORY_ERROR, "Couldn't initialize SDL_image: %s",
    //         SDL_GetError()
    //     );
    //     // FIXME: Error handling... ?
    //     // return SDL_APP_FAILURE;
    // }

    PyImport_AppendInittab("_fsapp_image", &fsapp_image_python_initfunc);
}
