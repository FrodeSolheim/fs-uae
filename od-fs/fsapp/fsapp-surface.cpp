#include "fsapp-surface.h"

#include <Python.h>
#include <SDL3/SDL.h>
#include <stdlib.h>

#include "fsemu-color.h"
#include "fsemu-data.h"
#include "fsemu-image.h"
#include "fsemu-sdl.h"
#include "fsemu-util.h"
#include "fsgui-font.h"
#include "fslib-refable.h"

// FIXME: Maybe rename to fsapp-surface / _fsapp_surface

// -------------------------------------------------------------------------------------------------

int fsapp_surface_log_level = FSEMU_LOG_LEVEL_INFO;

#define fsapp_surface_log(format, ...)                                   \
    if (fsemu_likely(fsapp_surface_log_level >= FSEMU_LOG_LEVEL_INFO)) { \
        fsemu_log("[GUI] [SFC] " format, ##__VA_ARGS__);                 \
    }

#define fsapp_surface_log_debug(format, ...)                                \
    if (fsemu_unlikely(fsapp_surface_log_level >= FSEMU_LOG_LEVEL_DEBUG)) { \
        fsemu_log("[GUI] [SFC] " format, ##__VA_ARGS__);                    \
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

void fsapp_surface_ref(fsapp_surface_t* surface) {
    fslib_refable_ref(surface);
}

void fsapp_surface_unref(fsapp_surface_t* surface) {
    fslib_refable_unref(surface);
}

static void fsapp_surface_finalize(void* object) {
#ifdef FSGUI_DEBUG_CLEANUP
    printf("SURFACE FINALIZE\n");
#endif

    printf("SURFACE FINALIZE\n");

    fsapp_surface_t* surface = (fsapp_surface_t*)object;
    fsapp_surface_log_debug("Finalizing surface %p\n", surface);

    // FIXME: Add to OpenGL free list - DONE - Handled differently

    free(surface);
}

// -------------------------------------------------------------------------------------------------

static void fsapp_surface_actually_create(fsapp_surface_t* surface, int width, int height) {
    SDL_Log("fsapp_surface_actually_create %d %d\n", width, height);
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

static void fsapp_surface_do_destroy(fsapp_surface_t* surface) {
#ifdef FSGUI_DEBUG_CLEANUP
    printf("fsapp_surface_python_destructor\n");
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
    fsapp_surface_ref(surface);

    // Remove from surface list and add to surface free list
    g_surface_list = g_list_remove(g_surface_list, surface);
    g_surface_opengl_free_list = g_list_append(g_surface_opengl_free_list, surface);
}

// -------------------------------------------------------------------------------------------------

static void fsapp_surface_python_destructor(PyObject* surface_capsule) {
#ifdef FSGUI_DEBUG_CLEANUP
    printf("fsapp_surface_python_destructor\n");
#endif

    fsapp_surface_t* surface =
        (fsapp_surface_t*)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);

    fsapp_surface_do_destroy(surface);

    fsapp_surface_unref(surface);
}

// -------------------------------------------------------------------------------------------------

static PyObject* fsapp_surface_python_create(PyObject* self, PyObject* args) {
    int width, height, special;
    if (!PyArg_ParseTuple(args, "(ii)i:create", &width, &height, &special)) return NULL;

    fsapp_surface_log_debug("Creating %dx%d surface\n", width, height);

    fsapp_surface_t* surface = (fsapp_surface_t*)malloc(sizeof(fsapp_surface_t));
    memset(surface, 0, sizeof(fsapp_surface_t));
    fslib_refable_init_with_finalizer(surface, fsapp_surface_finalize);

    surface->special = special;
    fsapp_surface_actually_create(surface, width, height);

    SDL_Log("Adding surface %dx%d to surface list", surface->width, surface->height);
    g_surface_list = g_list_append(g_surface_list, surface);

    PyObject* surface_capsule =
        PyCapsule_New(surface, "fsapp_surface_t", fsapp_surface_python_destructor);
    return surface_capsule;
}

// -------------------------------------------------------------------------------------------------

static PyObject* fsapp_surface_python_destroy(PyObject* self, PyObject* args) {
    PyObject* surface_capsule;
    if (!PyArg_ParseTuple(args, "O:destroy", &surface_capsule)) {
        return NULL;
    }

    fsapp_surface_t* surface =
        (fsapp_surface_t*)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);
    fsapp_surface_do_destroy(surface);

    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject* fsapp_surface_python_resize(PyObject* self, PyObject* args) {
    // FIXME: For later - release the GIL while doing work!!

    PyObject* surface_capsule;
    int width, height;
    if (!PyArg_ParseTuple(args, "O(ii):resize_surface", &surface_capsule, &width, &height)) {
        return NULL;
    }

    fsapp_surface_t* surface =
        (fsapp_surface_t*)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);

    // Don't need to re-blit old surface, just draw everything again (maybe)

    // FIXME: Release GIL?
    SDL_DestroySurface(surface->sdl);
    fsapp_surface_actually_create(surface, width, height);

    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject* fsapp_surface_python_set_position(PyObject* self, PyObject* args) {
    PyObject* surface_capsule;
    // float x, y;
    // if (!PyArg_ParseTuple(args, "O(ff):set_position", &surface_capsule, &x, &y)) {
    int x, y;
    if (!PyArg_ParseTuple(args, "O(ii):set_position", &surface_capsule, &x, &y)) {
        return NULL;
    }
    fsapp_surface_t* surface =
        (fsapp_surface_t*)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);
    surface->x = x;
    surface->y = y;
    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject* fsapp_surface_python_set_z_index(PyObject* self, PyObject* args) {
    PyObject* surface_capsule;
    int z_index;
    if (!PyArg_ParseTuple(args, "Oi:set_z_index", &surface_capsule, &z_index)) {
        return NULL;
    }
    fsapp_surface_t* surface =
        (fsapp_surface_t*)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);
    SDL_assert(z_index > -1000 && z_index < 1000);
    surface->z = z_index;
    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject* fsapp_surface_python_set_visible(PyObject* self, PyObject* args) {
    PyObject* surface_capsule;
    int visible;
    if (!PyArg_ParseTuple(args, "Oi:set_visible", &surface_capsule, &visible)) {
        return NULL;
    }
    fsapp_surface_t* surface =
        (fsapp_surface_t*)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);
    surface->visible = visible != 0;
    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject* fsapp_surface_python_draw_filled_rectangle(PyObject* self, PyObject* args) {
    PyObject* surface_capsule;
    SDL_Rect rect;
    int r, g, b, a;
    if (!PyArg_ParseTuple(args, "O(ii)(ii)(iiii):draw_filled_rectangle", &surface_capsule, &rect.x,
                          &rect.y, &rect.w, &rect.h, &r, &g, &b, &a)) {
        return NULL;
    }

    fsapp_surface_t* surface =
        (fsapp_surface_t*)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
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

static PyObject* fsapp_surface_python_draw_line(PyObject* self, PyObject* args) {
    // FIXME: For later - release the GIL while doing work!!

    PyObject* surface_capsule;
    int x1, y1, x2, y2;
    int r, g, b, a, line_width;
    if (!PyArg_ParseTuple(args, "O(ii)(ii)(iiii)i:draw_line", &surface_capsule, &x1, &y1, &x2, &y2,
                          &r, &g, &b, &a, &line_width)) {
        return NULL;
    }

    fsapp_surface_t* surface =
        (fsapp_surface_t*)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);

    // FIXME: Maybe we want to do bounds checking on release as well
    // Note: Removed strict assertions since widgets may calculate coordinates
    // slightly outside bounds, but the actual drawing code handles bounds checking

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

    // Fast paths for width=1 horizontal and vertical lines
    if (line_width == 1) {
        if (y1 == y2) {
            // Horizontal line - handle both directions
            int start_x = (x1 < x2) ? x1 : x2;
            int end_x = (x1 < x2) ? x2 : x1;
            if (y1 >= 0 && y1 < surface->height) {
                // Clip to surface bounds
                if (start_x < 0) start_x = 0;
                if (end_x >= surface->width) end_x = surface->width - 1;

                // Only draw if there's something to draw after clipping
                if (start_x <= end_x) {
                    int32_t* p = pixels + y1 * surface->width + start_x;
                    for (int x = start_x; x <= end_x; x++) {
                        *(p++) = color;
                    }
                }
            }
            Py_RETURN_NONE;
        } else if (x1 == x2) {
            // Vertical line - handle both directions
            int start_y = (y1 < y2) ? y1 : y2;
            int end_y = (y1 < y2) ? y2 : y1;
            if (x1 >= 0 && x1 < surface->width) {
                // Clip to surface bounds
                if (start_y < 0) start_y = 0;
                if (end_y >= surface->height) end_y = surface->height - 1;

                // Only draw if there's something to draw after clipping
                if (start_y <= end_y) {
                    int32_t* p = pixels + start_y * surface->width + x1;
                    for (int y = start_y; y <= end_y; y++) {
                        *p = color;
                        p += surface->width;
                    }
                }
            }
            Py_RETURN_NONE;
        }
    }

    // AI-generated method for non-horizontal/vertical (experiment)

    // General case: thick lines or diagonal lines
    int half_width = line_width / 2;

    // Bresenham's line algorithm with thick brush
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    int x = x1;
    int y = y1;

    while (true) {
        // Draw thick pixel at (x, y) using circular brush
        if (line_width == 1) {
            // Fast path for single pixel (diagonal case)
            if (x >= 0 && x < surface->width && y >= 0 && y < surface->height) {
                pixels[y * surface->width + x] = color;
            }
        } else {
            // Draw circular brush around current pixel
            for (int dy_brush = -half_width; dy_brush <= half_width; dy_brush++) {
                for (int dx_brush = -half_width; dx_brush <= half_width; dx_brush++) {
                    // Use circular brush (distance from center <= radius)
                    if (dx_brush * dx_brush + dy_brush * dy_brush <=
                        half_width * half_width + half_width) {
                        int plot_x = x + dx_brush;
                        int plot_y = y + dy_brush;
                        if (plot_x >= 0 && plot_x < surface->width && plot_y >= 0 &&
                            plot_y < surface->height) {
                            pixels[plot_y * surface->width + plot_x] = color;
                        }
                    }
                }
            }
        }

        // Check if we've reached the end point
        if (x == x2 && y == y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
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

// -------------------------------------------------------------------------------------------------

static PyObject* fsapp_surface_python_draw_circle(PyObject* self, PyObject* args) {
    // // AI-generated method (experiment)
    PyObject* surface_capsule;
    int center_x, center_y, radius;
    int r, g, b, a, line_width;
    if (!PyArg_ParseTuple(args, "O(ii)i(iiii)i:draw_circle", &surface_capsule, &center_x, &center_y,
                          &radius, &r, &g, &b, &a, &line_width)) {
        return NULL;
    }

    fsapp_surface_t* surface =
        (fsapp_surface_t*)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);

    uint32_t color = (a << 24) | (b << 16) | (g << 8) | r;
    int32_t* pixels = (int32_t*)surface->data;
    int half_width = line_width / 2;

    // Midpoint circle algorithm with thick brush
    int x = 0;
    int y = radius;
    int d = 1 - radius;

    while (x <= y) {
        // Draw 8 symmetric points with thick brush
        int points_x[] = {x, -x, x, -x, y, -y, y, -y};
        int points_y[] = {y, y, -y, -y, x, x, -x, -x};

        for (int i = 0; i < 8; i++) {
            int px = center_x + points_x[i];
            int py = center_y + points_y[i];

            if (line_width == 1) {
                // Fast path for single pixel
                if (px >= 0 && px < surface->width && py >= 0 && py < surface->height) {
                    pixels[py * surface->width + px] = color;
                }
            } else {
                // Draw circular brush around each point
                for (int dy_brush = -half_width; dy_brush <= half_width; dy_brush++) {
                    for (int dx_brush = -half_width; dx_brush <= half_width; dx_brush++) {
                        // Use circular brush (distance from center <= radius)
                        if (dx_brush * dx_brush + dy_brush * dy_brush <=
                            half_width * half_width + half_width) {
                            int plot_x = px + dx_brush;
                            int plot_y = py + dy_brush;
                            if (plot_x >= 0 && plot_x < surface->width && plot_y >= 0 &&
                                plot_y < surface->height) {
                                pixels[plot_y * surface->width + plot_x] = color;
                            }
                        }
                    }
                }
            }
        }

        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }

    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject* fsapp_surface_python_draw_filled_circle(PyObject* self, PyObject* args) {
    // // AI-generated method (experiment)
    PyObject* surface_capsule;
    int center_x, center_y, radius;
    int r, g, b, a;
    if (!PyArg_ParseTuple(args, "O(ii)i(iiii):draw_filled_circle", &surface_capsule, &center_x,
                          &center_y, &radius, &r, &g, &b, &a)) {
        return NULL;
    }

    fsapp_surface_t* surface =
        (fsapp_surface_t*)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);

    uint32_t color = (a << 24) | (b << 16) | (g << 8) | r;
    int32_t* pixels = (int32_t*)surface->data;

    // Midpoint circle algorithm adapted for filled circles
    int x = 0;
    int y = radius;
    int d = 1 - radius;

    // Draw initial horizontal line
    if (center_y >= 0 && center_y < surface->height) {
        int start_x = (center_x - radius < 0) ? 0 : center_x - radius;
        int end_x = (center_x + radius >= surface->width) ? surface->width - 1 : center_x + radius;
        int32_t* line_pixels = pixels + center_y * surface->width + start_x;
        for (int i = start_x; i <= end_x; i++) {
            *(line_pixels++) = color;
        }
    }

    while (x < y) {
        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;

        // Draw horizontal lines for this y level
        // Top and bottom lines for x offset
        if (center_y + y >= 0 && center_y + y < surface->height) {
            int start_x = (center_x - x < 0) ? 0 : center_x - x;
            int end_x = (center_x + x >= surface->width) ? surface->width - 1 : center_x + x;
            int32_t* line_pixels = pixels + (center_y + y) * surface->width + start_x;
            for (int i = start_x; i <= end_x; i++) {
                *(line_pixels++) = color;
            }
        }
        if (center_y - y >= 0 && center_y - y < surface->height) {
            int start_x = (center_x - x < 0) ? 0 : center_x - x;
            int end_x = (center_x + x >= surface->width) ? surface->width - 1 : center_x + x;
            int32_t* line_pixels = pixels + (center_y - y) * surface->width + start_x;
            for (int i = start_x; i <= end_x; i++) {
                *(line_pixels++) = color;
            }
        }

        // Top and bottom lines for y offset (if different from x)
        if (x != y) {
            if (center_y + x >= 0 && center_y + x < surface->height) {
                int start_x = (center_x - y < 0) ? 0 : center_x - y;
                int end_x = (center_x + y >= surface->width) ? surface->width - 1 : center_x + y;
                int32_t* line_pixels = pixels + (center_y + x) * surface->width + start_x;
                for (int i = start_x; i <= end_x; i++) {
                    *(line_pixels++) = color;
                }
            }
            if (center_y - x >= 0 && center_y - x < surface->height) {
                int start_x = (center_x - y < 0) ? 0 : center_x - y;
                int end_x = (center_x + y >= surface->width) ? surface->width - 1 : center_x + y;
                int32_t* line_pixels = pixels + (center_y - x) * surface->width + start_x;
                for (int i = start_x; i <= end_x; i++) {
                    *(line_pixels++) = color;
                }
            }
        }
    }

    Py_RETURN_NONE;
}

static PyMethodDef fsapp_surface_python_methods[] = {
    {"create", fsapp_surface_python_create, METH_VARARGS, "..."},
    {"resize", fsapp_surface_python_resize, METH_VARARGS, "..."},
    {"set_position", fsapp_surface_python_set_position, METH_VARARGS, "..."},
    {"set_z_index", fsapp_surface_python_set_z_index, METH_VARARGS, "..."},
    {"set_visible", fsapp_surface_python_set_visible, METH_VARARGS, "..."},
    {"draw_filled_rectangle", fsapp_surface_python_draw_filled_rectangle, METH_VARARGS, "..."},
    {"draw_line", fsapp_surface_python_draw_line, METH_VARARGS, "..."},
    {"draw_circle", fsapp_surface_python_draw_circle, METH_VARARGS, "..."},
    {"draw_filled_circle", fsapp_surface_python_draw_filled_circle, METH_VARARGS, "..."},
    {"destroy", fsapp_surface_python_destroy, METH_VARARGS, "..."},
    {NULL, NULL, 0, NULL}};

static PyModuleDef fsapp_surface_python_module = {PyModuleDef_HEAD_INIT,
                                                  "_fsapp_surface",
                                                  NULL,
                                                  -1,
                                                  fsapp_surface_python_methods,
                                                  NULL,
                                                  NULL,
                                                  NULL,
                                                  NULL};

static PyObject* fsapp_surface_python_initfunc(void) {
    return PyModule_Create(&fsapp_surface_python_module);
}

// -------------------------------------------------------------------------------------------------

void fsapp_surface_init_module(void) {
    static bool initialized;
    if (initialized) {
        return;
    }
    initialized = true;

    SDL_Log("Initializing surface module\n");

    PyImport_AppendInittab("_fsapp_surface", &fsapp_surface_python_initfunc);
}
