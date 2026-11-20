/*
 * Surfaces: what the UI draws into, composited into the host window.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#include "fsapp-surface.h"

#include <Python.h>
#include <SDL3/SDL.h>
#include <stdlib.h>

#include <algorithm>
#include <vector>

#include "fsapp-frame.h"
#include "fsapp-refable.h"

// FIXME: Maybe rename to fsapp-surface / _fsapp_surface

// -------------------------------------------------------------------------------------------------

int fsapp_surface_log_level = SDL_LOG_PRIORITY_INFO;

#define fsapp_surface_log(format, ...)                                                             \
    FS_LOG_INFO(fsapp_surface_log_level, "[GUI] [SFC]", format, ##__VA_ARGS__)

#define fsapp_surface_log_debug(format, ...)                                                       \
    FS_LOG_DEBUG(fsapp_surface_log_level, "[GUI] [SFC]", format, ##__VA_ARGS__)

// The surfaces changed in the current UI frame (the client state, the UI thread's), in the order
// they first changed in it, each holding a reference. Created surfaces are among them.
static std::vector<fsapp_surface_t *> g_changed;

// Adds the surface to the current UI frame's changed surfaces, if it is not there yet.
static void client_changed(fsapp_surface_t *surface)
{
    SDL_assert(!surface->destroyed);
    if (surface->changed) {
        return;
    }
    surface->changed = true;
    fsapp_surface_ref(surface);
    g_changed.push_back(surface);
}

// -------------------------------------------------------------------------------------------------

void fsapp_surface_ref(fsapp_surface_t *surface)
{
    fsapp_refable_ref(&surface->refable);
}

void fsapp_surface_unref(fsapp_surface_t *surface)
{
    fsapp_refable_unref(&surface->refable);
}

static void fsapp_surface_finalize(void *object)
{
    fsapp_surface_t *surface = (fsapp_surface_t *)object;
    fsapp_surface_log_debug("Finalizing surface %p\n", surface);
    free(surface);
}

// -------------------------------------------------------------------------------------------------

static void fsapp_surface_actually_create(fsapp_surface_t *surface, int width, int height)
{
    SDL_Log("fsapp_surface_actually_create %d %d\n", width, height);
    surface->state.width = width;
    surface->state.height = height;

    if (surface->special > 0) {
        // Do not allocate data for special surfaces
        // (FIXME: Should maybe have flags also/instead)
    } else if (surface->state.width == 0 || surface->state.height == 0) {
        //
    } else if (surface->state.width < 0 || surface->state.height < 0) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Negative surface size %dx%d", width, height);
    } else {
        // The native format, which the compositor uploads without swizzling (GL_BGRA with
        // GL_UNSIGNED_INT_8_8_8_8_REV, the same packed 32-bit pixel): premultiplied BGRA, bytes
        // B, G, R, A on little-endian hosts (big-endian hosts are not supported).
        surface->sdl = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_ARGB8888);
        // Other code assume that the pixel array is contiguous, so we check
        // for that already here.
        SDL_assert_release(surface->sdl->pitch == width * 4);
        surface->data = surface->sdl->pixels;
    }
}

// -------------------------------------------------------------------------------------------------

static void free_buffer(fsapp_surface_t *surface)
{
    if (surface->sdl != NULL) {
        SDL_DestroySurface(surface->sdl);
        surface->sdl = NULL;
        surface->data = NULL;
    }
}

static void destroy_in_record(fsapp_surface_t *surface);

// Destroys the surface (see fsapp_surface_destroy), keeping the client's reference. Does nothing
// when it is destroyed already (_fsapp_surface's destroy, then the capsule's destructor).
static void fsapp_surface_do_destroy(fsapp_surface_t *surface)
{
    SDL_assert(surface != NULL);
    if (surface->destroyed) {
        return;
    }
    surface->destroyed = true;
    destroy_in_record(surface);
}

// -------------------------------------------------------------------------------------------------

static void fsapp_surface_python_destructor(PyObject *surface_capsule)
{
#ifdef FSAPP_DEBUG_CLEANUP
    printf("fsapp_surface_python_destructor\n");
#endif

    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);

    fsapp_surface_do_destroy(surface);

    fsapp_surface_unref(surface);
}

// -------------------------------------------------------------------------------------------------

static fsapp_surface_t *fsapp_surface_new(int width, int height, int special)
{
    fsapp_surface_log_debug("Creating %dx%d surface\n", width, height);

    fsapp_surface_t *surface = (fsapp_surface_t *)malloc(sizeof(fsapp_surface_t));
    memset(surface, 0, sizeof(fsapp_surface_t));
    fsapp_refable_init_with_finalizer(&surface->refable, fsapp_surface_finalize);

    surface->special = special;
    surface->state.fill_color = {0, 0, 0, 255};
    fsapp_surface_actually_create(surface, width, height);

    SDL_Log("Adding surface %dx%d", surface->state.width, surface->state.height);
    // Created in this UI frame: the surface goes with its record.
    client_changed(surface);
    return surface;
}

fsapp_surface_t *fsapp_surface_create(int width, int height, bool without_buffer)
{
    return fsapp_surface_new(width, height, without_buffer ? 1 : 0);
}

void fsapp_surface_destroy(fsapp_surface_t *surface)
{
    fsapp_surface_do_destroy(surface);
    // The client's reference.
    fsapp_surface_unref(surface);
}

void fsapp_surface_attach_streamed_content(fsapp_surface_t *surface,
                                           fsapp_surface_stream_format_t format)
{
    SDL_assert(surface->special);
    surface->streamed = true;
    surface->stream_format = format;
}

static bool same_color(SDL_Color a, SDL_Color b)
{
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

// Adds a damaged rectangle to the surface's damage in the current UI frame, by the damage rules
// (fsapp_surface_commit).
static void add_damage(fsapp_surface_t *surface, const SDL_Rect *rect)
{
    fsapp_snapshot_damage_t *damage = &surface->damage;
    if (damage->whole) {
        return;
    }
    const SDL_Rect bounds = {0, 0, surface->state.width, surface->state.height};
    SDL_Rect clipped;
    if (!SDL_GetRectIntersection(rect, &bounds, &clipped)) {
        // Empty, or outside the buffer.
        return;
    }
    if (surface->damage_merged) {
        // Merged into their bounding box already: every further rectangle in the UI frame is
        // merged into the box too.
        SDL_GetRectUnion(&clipped, &damage->rects[0], &clipped);
        damage->count = 0;
    } else if (damage->count == FSAPP_SNAPSHOT_MAX_DAMAGE) {
        // Too many: merged into their bounding box (REVIEW at FSAPP_SNAPSHOT_MAX_DAMAGE).
        for (int i = 0; i < damage->count; i++) {
            SDL_GetRectUnion(&clipped, &damage->rects[i], &clipped);
        }
        damage->count = 0;
        surface->damage_merged = true;
    }
    if (clipped.w == bounds.w && clipped.h == bounds.h) {
        damage->whole = true;
        damage->count = 0;
        return;
    }
    damage->rects[damage->count++] = clipped;
}

// Clears the surface's damage in the current UI frame (none, not merged).
static void clear_damage(fsapp_surface_t *surface)
{
    SDL_zero(surface->damage);
    surface->damage_merged = false;
}

void fsapp_surface_commit(fsapp_surface_t *surface, const SDL_Rect *damage, int damage_count)
{
    if (surface->special) {
        // No buffer: nothing to commit.
        return;
    }
    if (damage_count == 0 || surface->resized) {
        // A commit after a resize brings content for the whole new buffer, whatever damage it
        // names, so that a rectangle left out cannot show what the new buffer held before.
        surface->resized = false;
        clear_damage(surface);
        surface->damage.whole = true;
    }
    for (int i = 0; i < damage_count; i++) {
        add_damage(surface, &damage[i]);
    }
    surface->committed = true;
    client_changed(surface);
}

void fsapp_surface_set_z_index(fsapp_surface_t *surface, int z_index)
{
    if (surface->state.z == z_index) {
        return;
    }
    surface->state.z = z_index;
    client_changed(surface);
}

void fsapp_surface_set_translucent(fsapp_surface_t *surface, bool translucent)
{
    if (surface->state.translucent == translucent) {
        return;
    }
    surface->state.translucent = translucent;
    client_changed(surface);
}

void fsapp_surface_set_fill_color(fsapp_surface_t *surface, SDL_Color fill_color)
{
    if (same_color(surface->state.fill_color, fill_color)) {
        return;
    }
    surface->state.fill_color = fill_color;
    client_changed(surface);
}

void fsapp_surface_set_position(fsapp_surface_t *surface, int x, int y)
{
    if (surface->state.x == x && surface->state.y == y) {
        return;
    }
    surface->state.x = x;
    surface->state.y = y;
    client_changed(surface);
}

void fsapp_surface_set_visible(fsapp_surface_t *surface, bool visible)
{
    if (surface->state.visible == visible) {
        return;
    }
    surface->state.visible = visible;
    client_changed(surface);
}

void fsapp_surface_resize(fsapp_surface_t *surface, int width, int height)
{
    if (surface->state.width == width && surface->state.height == height) {
        return;
    }
    free_buffer(surface);
    fsapp_surface_actually_create(surface, width, height);
    if (!surface->special) {
        // A commit made before the resize in this UI frame named the buffer just freed: it is
        // dropped, and the old content stays at the old size until the next commit.
        surface->committed = false;
        clear_damage(surface);
        surface->resized = true;
    }
    client_changed(surface);
}

// The pixel that the shapes store in the surface's buffer for a straight RGBA colour: the colour
// multiplied by its alpha (rounded), so that the buffer stays premultiplied, in the buffer's pixel
// format.
static uint32_t premultiplied_pixel(fsapp_surface_t *surface, int r, int g, int b, int a)
{
    Uint8 pr = (Uint8)((r * a + 127) / 255);
    Uint8 pg = (Uint8)((g * a + 127) / 255);
    Uint8 pb = (Uint8)((b * a + 127) / 255);
    return SDL_MapSurfaceRGBA(surface->sdl, pr, pg, pb, (Uint8)a);
}

void fsapp_surface_draw_filled_rectangle(fsapp_surface_t *surface, const SDL_Rect *rect,
                                         SDL_Color color)
{
    uint32_t pixel = premultiplied_pixel(surface, color.r, color.g, color.b, color.a);
    // Supports SDL_SetSurfaceClipRect() !!!
    SDL_FillSurfaceRect(surface->sdl, rect, pixel);
}

PyObject *fsapp_surface_create_python_capsule(fsapp_surface_t *surface)
{
    // The capsule owns a reference, and destroys the surface when it is freed.
    fsapp_surface_ref(surface);
    return PyCapsule_New(surface, "fsapp_surface_t", fsapp_surface_python_destructor);
}

static PyObject *fsapp_surface_python_create(PyObject *self, PyObject *args)
{
    int width, height, special;
    if (!PyArg_ParseTuple(args, "(ii)i:create", &width, &height, &special)) {
        return NULL;
    }

    fsapp_surface_t *surface = fsapp_surface_new(width, height, special);

    PyObject *surface_capsule =
        PyCapsule_New(surface, "fsapp_surface_t", fsapp_surface_python_destructor);
    return surface_capsule;
}

// -------------------------------------------------------------------------------------------------

static PyObject *fsapp_surface_python_destroy(PyObject *self, PyObject *args)
{
    PyObject *surface_capsule;
    if (!PyArg_ParseTuple(args, "O:destroy", &surface_capsule)) {
        return NULL;
    }

    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);
    fsapp_surface_do_destroy(surface);

    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject *fsapp_surface_python_resize(PyObject *self, PyObject *args)
{
    // FIXME: For later - release the GIL while doing work!!

    PyObject *surface_capsule;
    int width, height;
    if (!PyArg_ParseTuple(args, "O(ii):resize_surface", &surface_capsule, &width, &height)) {
        return NULL;
    }

    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);

    // Don't need to re-blit old surface, just draw everything again (maybe)

    // FIXME: Release GIL?
    fsapp_surface_resize(surface, width, height);

    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject *fsapp_surface_python_commit(PyObject *self, PyObject *args)
{
    PyObject *surface_capsule;
    PyObject *damage_object = Py_None;
    if (!PyArg_ParseTuple(args, "O|O:commit", &surface_capsule, &damage_object)) {
        return NULL;
    }
    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);
    // The damaged rectangles, (x, y, width, height) each; none (or an empty sequence) for the
    // whole surface.
    std::vector<SDL_Rect> damage;
    if (damage_object != Py_None) {
        PyObject *items = PySequence_Fast(damage_object, "commit: damage must be a sequence");
        if (items == NULL) {
            return NULL;
        }
        Py_ssize_t count = PySequence_Fast_GET_SIZE(items);
        for (Py_ssize_t i = 0; i < count; i++) {
            SDL_Rect rect;
            if (!PyArg_ParseTuple(PySequence_Fast_GET_ITEM(items, i), "iiii:commit", &rect.x,
                                  &rect.y, &rect.w, &rect.h)) {
                Py_DECREF(items);
                return NULL;
            }
            damage.push_back(rect);
        }
        Py_DECREF(items);
    }
    fsapp_surface_commit(surface, damage.data(), (int)damage.size());
    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject *fsapp_surface_python_set_position(PyObject *self, PyObject *args)
{
    PyObject *surface_capsule;
    // float x, y;
    // if (!PyArg_ParseTuple(args, "O(ff):set_position", &surface_capsule, &x, &y)) {
    int x, y;
    if (!PyArg_ParseTuple(args, "O(ii):set_position", &surface_capsule, &x, &y)) {
        return NULL;
    }
    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);
    fsapp_surface_set_position(surface, x, y);
    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject *fsapp_surface_python_set_z_index(PyObject *self, PyObject *args)
{
    PyObject *surface_capsule;
    int z_index;
    if (!PyArg_ParseTuple(args, "Oi:set_z_index", &surface_capsule, &z_index)) {
        return NULL;
    }
    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);
    fsapp_surface_set_z_index(surface, z_index);
    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject *fsapp_surface_python_set_translucent(PyObject *self, PyObject *args)
{
    PyObject *surface_capsule;
    int translucent;
    if (!PyArg_ParseTuple(args, "Op:set_translucent", &surface_capsule, &translucent)) {
        return NULL;
    }
    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);
    fsapp_surface_set_translucent(surface, translucent != 0);
    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject *fsapp_surface_python_set_visible(PyObject *self, PyObject *args)
{
    PyObject *surface_capsule;
    int visible;
    if (!PyArg_ParseTuple(args, "Oi:set_visible", &surface_capsule, &visible)) {
        return NULL;
    }
    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);
    fsapp_surface_set_visible(surface, visible != 0);
    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject *fsapp_surface_python_draw_filled_rectangle(PyObject *self, PyObject *args)
{
    PyObject *surface_capsule;
    SDL_Rect rect;
    int r, g, b, a;
    if (!PyArg_ParseTuple(args, "O(ii)(ii)(iiii):draw_filled_rectangle", &surface_capsule, &rect.x,
                          &rect.y, &rect.w, &rect.h, &r, &g, &b, &a)) {
        return NULL;
    }

    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);

    // FIXME: ASSERT COLOR BYTES.. (0 - 255) - not masking
    // FIXME: For later - release the GIL while doing work?
    SDL_Color color = {(Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a};
    fsapp_surface_draw_filled_rectangle(surface, &rect, color);

    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject *fsapp_surface_python_draw_line(PyObject *self, PyObject *args)
{
    // FIXME: For later - release the GIL while doing work!!

    PyObject *surface_capsule;
    int x1, y1, x2, y2;
    int r, g, b, a, line_width;
    if (!PyArg_ParseTuple(args, "O(ii)(ii)(iiii)i:draw_line", &surface_capsule, &x1, &y1, &x2, &y2,
                          &r, &g, &b, &a, &line_width)) {
        return NULL;
    }

    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);

    // FIXME: Maybe we want to do bounds checking on release as well
    // Note: Removed strict assertions since widgets may calculate coordinates
    // slightly outside bounds, but the actual drawing code handles bounds checking

    // FIXME: ASSERT COLOR BYTES.. (0 - 255) - not masking

    uint32_t color = premultiplied_pixel(surface, r, g, b, a);

    // FIXME: Check that x2 > x1
    // FIXME: Check that y2 > y1
    // FIXME: Out of bounds checking!
    // FIXME: Clip rect support

    int32_t *pixels = (int32_t *)surface->data;

    // Fast paths for width=1 horizontal and vertical lines
    if (line_width == 1) {
        if (y1 == y2) {
            // Horizontal line - handle both directions
            int start_x = (x1 < x2) ? x1 : x2;
            int end_x = (x1 < x2) ? x2 : x1;
            if (y1 >= 0 && y1 < surface->state.height) {
                // Clip to surface bounds
                if (start_x < 0) {
                    start_x = 0;
                }
                if (end_x >= surface->state.width) {
                    end_x = surface->state.width - 1;
                }

                // Only draw if there's something to draw after clipping
                if (start_x <= end_x) {
                    int32_t *p = pixels + y1 * surface->state.width + start_x;
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
            if (x1 >= 0 && x1 < surface->state.width) {
                // Clip to surface bounds
                if (start_y < 0) {
                    start_y = 0;
                }
                if (end_y >= surface->state.height) {
                    end_y = surface->state.height - 1;
                }

                // Only draw if there's something to draw after clipping
                if (start_y <= end_y) {
                    int32_t *p = pixels + start_y * surface->state.width + x1;
                    for (int y = start_y; y <= end_y; y++) {
                        *p = color;
                        p += surface->state.width;
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
            if (x >= 0 && x < surface->state.width && y >= 0 && y < surface->state.height) {
                pixels[y * surface->state.width + x] = color;
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
                        if (plot_x >= 0 && plot_x < surface->state.width && plot_y >= 0 &&
                            plot_y < surface->state.height) {
                            pixels[plot_y * surface->state.width + plot_x] = color;
                        }
                    }
                }
            }
        }

        // Check if we've reached the end point
        if (x == x2 && y == y2) {
            break;
        }

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

    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject *fsapp_surface_python_draw_circle(PyObject *self, PyObject *args)
{
    // // AI-generated method (experiment)
    PyObject *surface_capsule;
    int center_x, center_y, radius;
    int r, g, b, a, line_width;
    if (!PyArg_ParseTuple(args, "O(ii)i(iiii)i:draw_circle", &surface_capsule, &center_x, &center_y,
                          &radius, &r, &g, &b, &a, &line_width)) {
        return NULL;
    }

    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);

    uint32_t color = premultiplied_pixel(surface, r, g, b, a);
    int32_t *pixels = (int32_t *)surface->data;
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
                if (px >= 0 && px < surface->state.width && py >= 0 && py < surface->state.height) {
                    pixels[py * surface->state.width + px] = color;
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
                            if (plot_x >= 0 && plot_x < surface->state.width && plot_y >= 0 &&
                                plot_y < surface->state.height) {
                                pixels[plot_y * surface->state.width + plot_x] = color;
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

static PyObject *fsapp_surface_python_draw_filled_circle(PyObject *self, PyObject *args)
{
    // // AI-generated method (experiment)
    PyObject *surface_capsule;
    int center_x, center_y, radius;
    int r, g, b, a;
    if (!PyArg_ParseTuple(args, "O(ii)i(iiii):draw_filled_circle", &surface_capsule, &center_x,
                          &center_y, &radius, &r, &g, &b, &a)) {
        return NULL;
    }

    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);

    uint32_t color = premultiplied_pixel(surface, r, g, b, a);
    int32_t *pixels = (int32_t *)surface->data;

    // Midpoint circle algorithm adapted for filled circles
    int x = 0;
    int y = radius;
    int d = 1 - radius;

    // Draw initial horizontal line
    if (center_y >= 0 && center_y < surface->state.height) {
        int start_x = (center_x - radius < 0) ? 0 : center_x - radius;
        int end_x = (center_x + radius >= surface->state.width) ? surface->state.width - 1
                                                                : center_x + radius;
        int32_t *line_pixels = pixels + center_y * surface->state.width + start_x;
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
        if (center_y + y >= 0 && center_y + y < surface->state.height) {
            int start_x = (center_x - x < 0) ? 0 : center_x - x;
            int end_x =
                (center_x + x >= surface->state.width) ? surface->state.width - 1 : center_x + x;
            int32_t *line_pixels = pixels + (center_y + y) * surface->state.width + start_x;
            for (int i = start_x; i <= end_x; i++) {
                *(line_pixels++) = color;
            }
        }
        if (center_y - y >= 0 && center_y - y < surface->state.height) {
            int start_x = (center_x - x < 0) ? 0 : center_x - x;
            int end_x =
                (center_x + x >= surface->state.width) ? surface->state.width - 1 : center_x + x;
            int32_t *line_pixels = pixels + (center_y - y) * surface->state.width + start_x;
            for (int i = start_x; i <= end_x; i++) {
                *(line_pixels++) = color;
            }
        }

        // Top and bottom lines for y offset (if different from x)
        if (x != y) {
            if (center_y + x >= 0 && center_y + x < surface->state.height) {
                int start_x = (center_x - y < 0) ? 0 : center_x - y;
                int end_x = (center_x + y >= surface->state.width) ? surface->state.width - 1
                                                                   : center_x + y;
                int32_t *line_pixels = pixels + (center_y + x) * surface->state.width + start_x;
                for (int i = start_x; i <= end_x; i++) {
                    *(line_pixels++) = color;
                }
            }
            if (center_y - x >= 0 && center_y - x < surface->state.height) {
                int start_x = (center_x - y < 0) ? 0 : center_x - y;
                int end_x = (center_x + y >= surface->state.width) ? surface->state.width - 1
                                                                   : center_x + y;
                int32_t *line_pixels = pixels + (center_y - x) * surface->state.width + start_x;
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
    {"commit", fsapp_surface_python_commit, METH_VARARGS, "..."},
    {"set_position", fsapp_surface_python_set_position, METH_VARARGS, "..."},
    {"set_z_index", fsapp_surface_python_set_z_index, METH_VARARGS, "..."},
    {"set_translucent", fsapp_surface_python_set_translucent, METH_VARARGS, "..."},
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

static PyObject *fsapp_surface_python_initfunc(void)
{
    return PyModule_Create(&fsapp_surface_python_module);
}

// -------------------------------------------------------------------------------------------------
// The record (see fsapp-surface.h).

namespace
{

// A surface created or changed in a UI frame: its new client state, and its content when it was
// committed. It holds a reference to the surface until the host window side has applied it.
struct record_entry {
    fsapp_surface_t *surface;
    fsapp_surface_state_t state;
    // Committed in the UI frame: buffer (NULL when the size is empty) and the state's width and
    // height are its content, and the damage what changed in it.
    bool committed;
    const void *buffer;
    fsapp_snapshot_damage_t damage;
};

// Whether two states have the same layout: all but the size.
bool same_layout(const fsapp_surface_state_t *a, const fsapp_surface_state_t *b)
{
    return a->visible == b->visible && a->x == b->x && a->y == b->y && a->z == b->z &&
           a->translucent == b->translucent && same_color(a->fill_color, b->fill_color);
}

} // namespace

// The UI thread's side of the record.
static struct {
    // The record being collected (its storage is kept from UI frame to UI frame): the surfaces
    // created or changed, and the surfaces destroyed (the current UI frame's go in at once), each
    // holding a reference until the host window side has applied it.
    std::vector<record_entry> record;
    std::vector<fsapp_surface_t *> destroyed;
    // Whether the first record has been handed over.
    bool handed_first;
    // Whether the UI thread waits, before its next UI frame, for its last record to be taken in
    // (not before the first UI frame, nor after one that handed over nothing), and the count of
    // g_slot's records taken in that it waits for.
    bool waits_for_taken_in;
    uint64_t wait_for;
} g_ui;

// The pending slot, and what the UI thread waits for, under the record's lock. The lock is held
// only to put a record in the slot or take it out and to count, never while drawing, uploading or
// running Python.
static struct {
    SDL_Mutex *mutex;
    SDL_Condition *condition;
    // The pending record (when pending).
    std::vector<record_entry> record;
    std::vector<fsapp_surface_t *> destroyed;
    bool pending;
    // Whether a UI frame runs (from fsapp_surface_wait_for_next_ui_frame to
    // fsapp_surface_end_ui_frame, the UI thread's sleep after the wait included), and on which
    // thread.
    bool in_ui_frame;
    SDL_ThreadID ui_frame_thread;
    // The surfaces destroyed outside a UI frame, for the next record, each holding a reference
    // (their buffers are freed when they are taken), and whether the main loop has ended (then a
    // surface destroyed outside a UI frame is freed at once).
    std::vector<fsapp_surface_t *> queued;
    bool ended;
    // The records handed over and taken in so far.
    uint64_t handed;
    uint64_t taken_in;
    // Set by fsapp_surface_stop_ui_frames.
    bool stopped;
} g_slot;

// The host window side's applied state, and the layout it writes into the snapshot.
static struct {
    // The record being applied (its storage is kept from record to record).
    std::vector<record_entry> record;
    std::vector<fsapp_surface_t *> destroyed;
    // The applied surfaces, in the order they were created (in the records), each holding a
    // reference.
    std::vector<fsapp_surface_t *> surfaces;
    // Whether a record changed the layout since the flat list was built.
    bool layout_changed;
    bool taken_first;
    // The flat list, and the surfaces in it while they are put in stacking order.
    std::vector<fsapp_surface_t *> order;
    std::vector<fsapp_snapshot_surface_t> flat;
    // The record's content and destroyed surfaces, for the snapshot of the frame that takes it.
    std::vector<fsapp_snapshot_content_t> contents;
    std::vector<const void *> destroyed_ids;
} g_host;

// Puts the destruction of a surface (just marked destroyed) in a record. Inside a UI frame, on its
// thread, the buffer is freed at once (the drawing side never reads it while a UI frame runs) and
// the destruction goes in the UI frame's record. Outside a UI frame (a Python capsule freed in the
// exit handler on the main thread), it is queued under the record's lock for the next record, and
// its buffer is freed when the UI thread takes it; once the main loop has ended, it is freed at
// once.
// REVIEW: A surface destroyed outside a UI frame must not be used by a UI frame running at the same
// time (its client state is the UI thread's): today that happens only before the UI thread's first
// UI frame (start-up) and after its last (the exit handler).
static void destroy_in_record(fsapp_surface_t *surface)
{
    SDL_LockMutex(g_slot.mutex);
    bool in_ui_frame = g_slot.in_ui_frame && g_slot.ui_frame_thread == SDL_GetCurrentThreadID();
    bool ended = g_slot.ended;
    if (!in_ui_frame && !ended) {
        fsapp_surface_ref(surface);
        g_slot.queued.push_back(surface);
    }
    SDL_UnlockMutex(g_slot.mutex);
    if (!in_ui_frame) {
        SDL_Log("Surface %p destroyed outside a UI frame%s", (void *)surface,
                ended ? ", after the main loop ended: freed at once"
                      : ": queued for the next record");
    }
    if (in_ui_frame) {
        free_buffer(surface);
        fsapp_surface_ref(surface);
        g_ui.destroyed.push_back(surface);
    } else if (ended) {
        // No frame is drawn any more.
        free_buffer(surface);
    }
}

bool fsapp_surface_end_ui_frame(void)
{
    std::vector<record_entry> &record = g_ui.record;
    record.clear();
    for (fsapp_surface_t *surface : g_changed) {
        surface->changed = false;
        if (surface->destroyed) {
            // Destroyed: it goes with the destroyed surfaces instead.
            fsapp_surface_unref(surface);
            continue;
        }
        // The entry takes over the reference of the changed surfaces.
        record_entry entry = {};
        entry.surface = surface;
        entry.state = surface->state;
        if (surface->committed) {
            surface->committed = false;
            entry.committed = true;
            entry.buffer = surface->data;
            entry.damage = surface->damage;
            clear_damage(surface);
        }
        record.push_back(entry);
    }
    g_changed.clear();

    // The surfaces destroyed outside a UI frame since the last record go with this one. No record
    // naming their buffers is in flight any more, so their buffers are freed here.
    std::vector<fsapp_surface_t *> &destroyed = g_ui.destroyed;
    size_t queued_from = destroyed.size();
    SDL_LockMutex(g_slot.mutex);
    destroyed.insert(destroyed.end(), g_slot.queued.begin(), g_slot.queued.end());
    g_slot.queued.clear();
    SDL_UnlockMutex(g_slot.mutex);
    for (size_t i = queued_from; i < destroyed.size(); i++) {
        free_buffer(destroyed[i]);
    }

    // The first record is handed over even when it is empty (no surfaces), so that a program
    // without surfaces is drawn at all (no frame is drawn before the first record).
    bool hand_over = !record.empty() || !destroyed.empty() || !g_ui.handed_first;
    SDL_LockMutex(g_slot.mutex);
    g_slot.in_ui_frame = false;
    if (hand_over) {
        // At most one record is in flight: the UI thread waited for the last to be taken in.
        SDL_assert(!g_slot.pending);
        g_slot.record.swap(record);
        g_slot.destroyed.swap(destroyed);
        g_slot.pending = true;
        g_slot.handed += 1;
        g_ui.waits_for_taken_in = true;
        g_ui.wait_for = g_slot.handed;
    }
    SDL_UnlockMutex(g_slot.mutex);
    if (!hand_over) {
        return false;
    }
    g_ui.handed_first = true;
    // A record makes a frame needed.
    fsapp_frame_wake();
    return true;
}

bool fsapp_surface_wait_for_next_ui_frame(void)
{
    SDL_LockMutex(g_slot.mutex);
    if (g_ui.waits_for_taken_in) {
        while (!g_slot.stopped && g_slot.taken_in < g_ui.wait_for) {
            SDL_WaitCondition(g_slot.condition, g_slot.mutex);
        }
    }
    g_ui.waits_for_taken_in = false;
    bool stopped = g_slot.stopped;
    if (!stopped) {
        g_slot.in_ui_frame = true;
        g_slot.ui_frame_thread = SDL_GetCurrentThreadID();
    }
    SDL_UnlockMutex(g_slot.mutex);
    return !stopped;
}

void fsapp_surface_stop_ui_frames(void)
{
    SDL_LockMutex(g_slot.mutex);
    g_slot.stopped = true;
    SDL_BroadcastCondition(g_slot.condition);
    SDL_UnlockMutex(g_slot.mutex);
}

bool fsapp_surface_has_pending_record(void)
{
    SDL_LockMutex(g_slot.mutex);
    bool pending = g_slot.pending;
    SDL_UnlockMutex(g_slot.mutex);
    return pending;
}

bool fsapp_surface_has_taken_first_record(void)
{
    return g_host.taken_first;
}

// A surface destroyed in a record: it leaves the applied state, and its address goes in the
// snapshot, for the drawing side to delete its texture (one destroyed before a record applied it
// has none).
static void remove_destroyed_surface(fsapp_surface_t *surface)
{
    SDL_assert(surface->destroyed);
    if (surface->applied.live) {
        g_host.destroyed_ids.push_back(surface);
        surface->applied.live = false;
        auto found = std::find(g_host.surfaces.begin(), g_host.surfaces.end(), surface);
        SDL_assert(found != g_host.surfaces.end());
        g_host.surfaces.erase(found);
        g_host.layout_changed = true;
        // The applied state's reference.
        fsapp_surface_unref(surface);
    }
}

// Applies a record's entry to the surface's applied state, and adds its content (if committed) to
// the snapshot's.
static void apply_entry(const record_entry *entry)
{
    fsapp_surface_t *surface = entry->surface;
    auto &applied = surface->applied;
    if (!applied.live) {
        // Created: it is applied from now on, in the order of creation.
        applied.live = true;
        fsapp_surface_ref(surface);
        g_host.surfaces.push_back(surface);
        g_host.layout_changed = true;
    }
    const fsapp_surface_state_t *state = &entry->state;
    if (!same_layout(&applied.state, state)) {
        // All but the size, which follows.
        int width = applied.state.width;
        int height = applied.state.height;
        applied.state = *state;
        applied.state.width = width;
        applied.state.height = height;
        g_host.layout_changed = true;
    }
    int width = applied.state.width;
    int height = applied.state.height;
    if (surface->special) {
        // Without a buffer, the size is layout.
        width = state->width;
        height = state->height;
    } else if (entry->committed) {
        // With a buffer, the size takes effect with the content committed for it.
        bool has_content = entry->buffer != NULL && state->width > 0 && state->height > 0;
        if (has_content) {
            fsapp_snapshot_content_t content = {};
            content.id = surface;
            content.buffer = entry->buffer;
            content.width = state->width;
            content.height = state->height;
            // A new texture, or one whose size changes, is uploaded whole; else only the damage.
            content.size_changed = !applied.has_content || state->width != applied.state.width ||
                                   state->height != applied.state.height;
            content.damage = entry->damage;
            // A commit whose damage was all empty (or outside the buffer) uploads nothing.
            if (content.size_changed || content.damage.whole || content.damage.count > 0) {
                g_host.contents.push_back(content);
            }
        }
        if (has_content != applied.has_content) {
            applied.has_content = has_content;
            g_host.layout_changed = true;
        }
        width = has_content ? state->width : 0;
        height = has_content ? state->height : 0;
    }
    if (width != applied.state.width || height != applied.state.height) {
        applied.state.width = width;
        applied.state.height = height;
        g_host.layout_changed = true;
    }
}

// Builds the flat list from the applied state: the surfaces with content (or streamed content) that
// are visible and not empty, in the order they were created, put in stacking order from back to
// front (by z index, keeping the order of creation for the same z index).
static void build_flat_list(void)
{
    std::vector<fsapp_surface_t *> &order = g_host.order;
    order.clear();
    for (fsapp_surface_t *surface : g_host.surfaces) {
        const auto &applied = surface->applied;
        if (!applied.state.visible || applied.state.width <= 0 || applied.state.height <= 0) {
            continue;
        }
        // A surface without a buffer has content to draw only with streamed content (the emulation
        // surface, drawn in the phase of drawing the paced surface; with no content yet, its fill).
        if (!surface->streamed && (surface->special || !applied.has_content)) {
            continue;
        }
        order.push_back(surface);
    }
    std::stable_sort(order.begin(), order.end(),
                     [](const fsapp_surface_t *a, const fsapp_surface_t *b) {
                         return a->applied.state.z < b->applied.state.z;
                     });
    g_host.flat.clear();
    for (const fsapp_surface_t *surface : order) {
        const auto &applied = surface->applied;
        fsapp_snapshot_surface_t copy = {};
        copy.id = surface;
        copy.x = applied.state.x;
        copy.y = applied.state.y;
        copy.width = applied.state.width;
        copy.height = applied.state.height;
        copy.visible = applied.state.visible;
        copy.translucent = applied.state.translucent;
        if (surface->streamed) {
            copy.streamed = true;
            copy.fill_color = applied.state.fill_color;
        }
        g_host.flat.push_back(copy);
    }
}

void fsapp_surface_write_snapshot(fsapp_snapshot_layout_t *layout,
                                  const fsapp_snapshot_layout_t *untaken)
{
    g_host.contents.clear();
    g_host.destroyed_ids.clear();
    // A record in a snapshot that the drawing side has not taken yet (render thread mode) is
    // carried over into this one, with its content and destroyed surfaces, so that it is taken in
    // once, and whole. Its layout has been applied already (the flat list below has it), and
    // while it is in flight no new pending record can come: the UI thread hands over its next
    // record only once this one has been taken in. Should one come all the same, it waits in the
    // pending slot for the next snapshot (and keeps a frame needed).
    bool carried = untaken != NULL && untaken->takes_record;
    if (carried) {
        g_host.contents.assign(untaken->contents, untaken->contents + untaken->content_count);
        g_host.destroyed_ids.assign(untaken->destroyed,
                                    untaken->destroyed + untaken->destroyed_count);
    }
    bool taken = false;
    SDL_LockMutex(g_slot.mutex);
    if (g_slot.pending && !carried) {
        g_host.record.swap(g_slot.record);
        g_host.destroyed.swap(g_slot.destroyed);
        g_slot.pending = false;
        taken = true;
    }
    SDL_UnlockMutex(g_slot.mutex);

    if (taken) {
        // The destroyed surfaces are handed over once, with the record (carried over with it
        // while the drawing side has not taken it, above).
        for (fsapp_surface_t *surface : g_host.destroyed) {
            remove_destroyed_surface(surface);
            // The record's reference.
            fsapp_surface_unref(surface);
        }
        g_host.destroyed.clear();
        for (const record_entry &entry : g_host.record) {
            apply_entry(&entry);
            // The record's reference (the applied state holds one of its own).
            fsapp_surface_unref(entry.surface);
        }
        g_host.record.clear();
        g_host.taken_first = true;
    }
    if (g_host.layout_changed) {
        build_flat_list();
        g_host.layout_changed = false;
    }

    layout->takes_record = taken || carried;
    layout->surfaces = g_host.flat.data();
    layout->surface_count = (int)g_host.flat.size();
    layout->contents = g_host.contents.data();
    layout->content_count = (int)g_host.contents.size();
    layout->destroyed = g_host.destroyed_ids.data();
    layout->destroyed_count = (int)g_host.destroyed_ids.size();
}

void fsapp_surface_on_uploaded(const fsapp_snapshot_layout_t *layout)
{
    SDL_LockMutex(g_slot.mutex);
    if (layout->takes_record) {
        g_slot.taken_in += 1;
        SDL_BroadcastCondition(g_slot.condition);
    }
    SDL_UnlockMutex(g_slot.mutex);
}

void fsapp_surface_quit(void)
{
    std::vector<fsapp_surface_t *> queued;
    SDL_LockMutex(g_slot.mutex);
    g_slot.ended = true;
    queued.swap(g_slot.queued);
    SDL_UnlockMutex(g_slot.mutex);
    for (fsapp_surface_t *surface : queued) {
        free_buffer(surface);
        // The queue's reference.
        fsapp_surface_unref(surface);
    }
    if (!queued.empty()) {
        SDL_Log("Freed %d destroyed surface(s) left when the main loop ended", (int)queued.size());
    }
}

// -------------------------------------------------------------------------------------------------

void fsapp_surface_init(void)
{
    static bool initialized;
    if (initialized) {
        return;
    }
    initialized = true;

    SDL_Log("Initializing surface module\n");

    g_slot.mutex = SDL_CreateMutex();
    g_slot.condition = SDL_CreateCondition();

    PyImport_AppendInittab("_fsapp_surface", &fsapp_surface_python_initfunc);
}
