/*
 * Images for the UI: loading, sizing SVG images and tinting, also from Python (_fsapp_image).
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#include "fsapp-image.h"

#include <Python.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <glib.h>

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

static void fsapp_image_init_9patch(fsapp_image_t *image)
{
    uint32_t black = SDL_MapSurfaceRGBA(image->surface, 0, 0, 0, 255);
    int width = image->surface->w;
    int height = image->surface->h;

    image->left = 0;
    image->top = 0;
    image->bottom = 0;
    image->right = 0;

    uint32_t *pixels = (uint32_t *)image->surface->pixels;
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

    fsapp_image_log_debug("9 patch image border left %d top %d right %d bottom %d\n", image->left,
                          image->top, image->right, image->bottom);
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

    if (g_str_has_suffix(path, ".9p.png")) {
        fsapp_image_init_9patch(image);
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

    // SVG files don't use 9-patch format
    image->left = 0;
    image->top = 0;
    image->bottom = 0;
    image->right = 0;

    fsapp_image_log_debug("Loaded sized SVG %s (%dx%d)\n", name, width, height);

    return image;
}

// REVIEW: The tint changes the image itself, not a surface buffer, so its result stays straight
// alpha like the rest of the image: drawing the image blends it into the buffer, which
// premultiplies it (fsapp_image_python_draw). Premultiplying here would multiply by the alpha
// twice.
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
                uint8_t new_a = pixel_a; // Keep original alpha for shape

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

// ----------------------------------------------------------------------------

static void fsapp_image_python_destructor(PyObject *image_capsule)
{
#ifdef FSAPP_DEBUG_CLEANUP
    fsapp_image_log_warning("fsapp_image_python_destructor FIXME: Not implemented\n");
#endif
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

// ----------------------------------------------------------------------------

static PyObject *fsapp_image_python_get_size(PyObject *self, PyObject *args)
{
    PyObject *image_capsule;
    if (!PyArg_ParseTuple(args, "O:get_size", &image_capsule)) {
        return NULL;
    }

    fsapp_image_t *image = (fsapp_image_t *)PyCapsule_GetPointer(image_capsule, "fsapp_image_t");
    SDL_assert(image);

    // FIXME: Subtract left/right/top/bottom if a 9patch image?
    return Py_BuildValue("(ii)", image->surface->w, image->surface->h);
}

// ----------------------------------------------------------------------------

static PyObject *fsapp_image_python_draw(PyObject *self, PyObject *args)
{
    PyObject *image_capsule, *surface_capsule;
    int x, y;
    if (!PyArg_ParseTuple(args, "OO(ii):draw", &image_capsule, &surface_capsule, &x, &y)) {
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
    SDL_BlitSurface(image->surface, NULL, surface->sdl, &dst_rect);

    Py_RETURN_NONE;
}

// ----------------------------------------------------------------------------

static PyObject *fsapp_image_python_draw_stretched(PyObject *self, PyObject *args)
{
    PyObject *image_capsule, *surface_capsule;
    int x, y, w, h;
    if (!PyArg_ParseTuple(args, "OO(ii)(ii):draw_stretched", &image_capsule, &surface_capsule, &x,
                          &y, &w, &h)) {
        return NULL;
    }
    fsapp_image_t *image = (fsapp_image_t *)PyCapsule_GetPointer(image_capsule, "fsapp_image_t");
    SDL_assert(image);

    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface);

    // SDL_Rect src_rect = ;
    SDL_Rect dst_rect = {.x = x, .y = y, .w = w, .h = h};

    // REVIEW: Keeps the buffer premultiplied, as fsapp_image_python_draw: with SDL_BLENDMODE_BLEND,
    // SDL scales linearly into a temporary surface and then blends that with the image's blend mode
    // (the 9-grid blit scales each part the same way).
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

static PyMethodDef fsapp_image_python_methods[] = {
    {"draw", fsapp_image_python_draw, METH_VARARGS, "..."},
    {"draw_stretched", fsapp_image_python_draw_stretched, METH_VARARGS, "..."},
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
    return PyModule_Create(&fsapp_image_python_module);
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
