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
#include "fsapp-input.h"
#include "fsapp-navigation.h"
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
    if (surface->state.parent != NULL) {
        fsapp_surface_unref(surface->state.parent);
    }
    if (surface->state.key_target != NULL) {
        fsapp_surface_unref(surface->state.key_target);
    }
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
    // The key target, a descendant, holds a reference to its parent: released here, so that the two
    // do not keep each other alive.
    if (surface->state.key_target != NULL) {
        fsapp_surface_unref(surface->state.key_target);
        surface->state.key_target = NULL;
    }
    if (surface->event_handler != NULL) {
        // REVIEW: Destroyed from C, the GIL may not be held. Left alone when Python has ended.
        if (Py_IsInitialized()) {
            PyGILState_STATE gil = PyGILState_Ensure();
            Py_CLEAR(surface->event_handler);
            PyGILState_Release(gil);
        }
    }
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
    surface->state.band = FSAPP_STACKING_BAND_NORMAL;
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

// Whether a surface with the role has a parent: a child or popup.
static bool has_parent_role(fsapp_stacking_role_t role)
{
    return role == FSAPP_STACKING_ROLE_CHILD || role == FSAPP_STACKING_ROLE_POPUP;
}

void fsapp_surface_set_role(fsapp_surface_t *surface, fsapp_stacking_role_t role,
                            fsapp_surface_t *parent)
{
    bool takes_parent = has_parent_role(role);
    // A parent among the surface's own descendants would make a cycle of references.
    bool descendant = false;
    for (const fsapp_surface_t *p = parent; p != NULL && !descendant; p = p->state.parent) {
        descendant = p == surface;
    }
    if (takes_parent != (parent != NULL) || descendant || (parent != NULL && parent->destroyed)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Surface %p: role %d with parent %p ignored (a child or popup takes a parent, "
                     "a top-level or an overlay none, and not itself, a descendant or a destroyed "
                     "surface)",
                     (void *)surface, (int)role, (void *)parent);
        return;
    }
    if (surface->state.role == role && surface->state.parent == parent) {
        return;
    }
    if (parent != NULL) {
        fsapp_surface_ref(parent);
    }
    if (surface->state.parent != NULL) {
        fsapp_surface_unref(surface->state.parent);
    }
    surface->state.role = role;
    surface->state.parent = parent;
    client_changed(surface);
}

void fsapp_surface_set_key_target(fsapp_surface_t *top_level, fsapp_surface_t *surface)
{
    bool in_tree = surface == NULL;
    for (const fsapp_surface_t *s = surface; s != NULL && !in_tree; s = s->state.parent) {
        in_tree = s == top_level;
    }
    if (top_level->state.role != FSAPP_STACKING_ROLE_TOP_LEVEL || !in_tree ||
        (surface != NULL && surface->destroyed)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Surface %p: key target %p ignored (a top-level names a surface in its tree)",
                     (void *)top_level, (void *)surface);
        return;
    }
    if (surface == top_level) {
        surface = NULL;
    }
    if (top_level->state.key_target == surface || top_level->destroyed) {
        return;
    }
    if (surface != NULL) {
        fsapp_surface_ref(surface);
    }
    if (top_level->state.key_target != NULL) {
        fsapp_surface_unref(top_level->state.key_target);
    }
    top_level->state.key_target = surface;
    client_changed(top_level);
}

void fsapp_surface_set_band(fsapp_surface_t *surface, fsapp_stacking_band_t band)
{
    if (surface->state.band == band) {
        return;
    }
    surface->state.band = band;
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

static bool is_scale_support(int value)
{
    return value == FSAPP_PIXELS_SCALE_ONE || value == FSAPP_PIXELS_SCALE_ANY;
}

void fsapp_surface_set_scale_support(fsapp_surface_t *surface,
                                     fsapp_pixels_scale_support_t scale_support)
{
    if (!is_scale_support(scale_support)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Not a scale support: %d, ignored",
                     (int)scale_support);
        return;
    }
    if (surface->state.scale_support == scale_support) {
        return;
    }
    surface->state.scale_support = scale_support;
    client_changed(surface);
}

fsapp_pixels_scale_support_t fsapp_surface_get_scale_support(const fsapp_surface_t *surface)
{
    return surface->state.scale_support;
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

void fsapp_surface_set_focusable(fsapp_surface_t *surface, bool focusable)
{
    if (surface->state.focusable == focusable) {
        return;
    }
    surface->state.focusable = focusable;
    client_changed(surface);
}

static void request_raise_in_record(fsapp_surface_t *surface);

void fsapp_surface_raise(fsapp_surface_t *surface)
{
    if (surface->destroyed) {
        return;
    }
    request_raise_in_record(surface);
}

static void request_focus_in_record(fsapp_surface_t *surface);

void fsapp_surface_request_focus(fsapp_surface_t *surface)
{
    if (surface->destroyed) {
        return;
    }
    request_focus_in_record(surface);
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
    SDL_FillSurfaceRect(surface->sdl, rect, pixel);
}

PyObject *fsapp_surface_create_python_capsule(fsapp_surface_t *surface)
{
    // The capsule owns a reference, and destroys the surface when it is freed.
    fsapp_surface_ref(surface);
    return PyCapsule_New(surface, "fsapp_surface_t", fsapp_surface_python_destructor);
}

// _fsapp_surface.create(size, special, scale_support=SCALE_ONE).
// REVIEW: The scale support is set only here, with no Python setter for changing it later.
static PyObject *fsapp_surface_python_create(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static const char *keywords[] = {"", "", "scale_support", NULL};
    int width, height, special;
    int scale_support = FSAPP_PIXELS_SCALE_ONE;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "(ii)i|i:create", (char **)keywords, &width,
                                     &height, &special, &scale_support)) {
        return NULL;
    }
    if (!is_scale_support(scale_support)) {
        PyErr_Format(PyExc_ValueError, "Not a scale support: %d", scale_support);
        return NULL;
    }

    fsapp_surface_t *surface = fsapp_surface_new(width, height, special);
    fsapp_surface_set_scale_support(surface, (fsapp_pixels_scale_support_t)scale_support);

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

// A PyArg_ParseTuple converter ("O&") from a surface's capsule to its fsapp_surface_t *.
static int convert_surface(PyObject *object, void *address)
{
    fsapp_surface_t *surface = (fsapp_surface_t *)PyCapsule_GetPointer(object, "fsapp_surface_t");
    if (surface == NULL) {
        return 0;
    }
    *(fsapp_surface_t **)address = surface;
    return 1;
}

// As convert_surface, also taking None for NULL.
static int convert_optional_surface(PyObject *object, void *address)
{
    if (object == Py_None) {
        *(fsapp_surface_t **)address = NULL;
        return 1;
    }
    return convert_surface(object, address);
}

// _fsapp_surface.set_role(surface, role, parent=None).
static PyObject *fsapp_surface_python_set_role(PyObject *self, PyObject *args)
{
    fsapp_surface_t *surface;
    int role;
    fsapp_surface_t *parent = NULL;
    if (!PyArg_ParseTuple(args, "O&i|O&:set_role", convert_surface, &surface, &role,
                          convert_optional_surface, &parent)) {
        return NULL;
    }
    if (role < FSAPP_STACKING_ROLE_TOP_LEVEL || role > FSAPP_STACKING_ROLE_OVERLAY) {
        PyErr_Format(PyExc_ValueError, "Not a role: %d", role);
        return NULL;
    }
    fsapp_surface_set_role(surface, (fsapp_stacking_role_t)role, parent);
    Py_RETURN_NONE;
}

static PyObject *fsapp_surface_python_set_band(PyObject *self, PyObject *args)
{
    fsapp_surface_t *surface;
    int band;
    if (!PyArg_ParseTuple(args, "O&i:set_band", convert_surface, &surface, &band)) {
        return NULL;
    }
    if (band < FSAPP_STACKING_BAND_BACKGROUND || band > FSAPP_STACKING_BAND_PANEL) {
        PyErr_Format(PyExc_ValueError, "Not a band: %d", band);
        return NULL;
    }
    fsapp_surface_set_band(surface, (fsapp_stacking_band_t)band);
    Py_RETURN_NONE;
}

static PyObject *fsapp_surface_python_set_key_target(PyObject *self, PyObject *args)
{
    fsapp_surface_t *top_level;
    fsapp_surface_t *surface;
    if (!PyArg_ParseTuple(args, "O&O&:set_key_target", convert_surface, &top_level,
                          convert_optional_surface, &surface)) {
        return NULL;
    }
    fsapp_surface_set_key_target(top_level, surface);
    Py_RETURN_NONE;
}

static PyObject *fsapp_surface_python_raise_surface(PyObject *self, PyObject *args)
{
    fsapp_surface_t *surface;
    if (!PyArg_ParseTuple(args, "O&:raise_surface", convert_surface, &surface)) {
        return NULL;
    }
    fsapp_surface_raise(surface);
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

static PyObject *fsapp_surface_python_set_focusable(PyObject *self, PyObject *args)
{
    PyObject *surface_capsule;
    int focusable;
    if (!PyArg_ParseTuple(args, "Op:set_focusable", &surface_capsule, &focusable)) {
        return NULL;
    }
    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    if (surface == NULL) {
        return NULL;
    }
    fsapp_surface_set_focusable(surface, focusable != 0);
    Py_RETURN_NONE;
}

static PyObject *fsapp_surface_python_request_focus(PyObject *self, PyObject *args)
{
    PyObject *surface_capsule;
    if (!PyArg_ParseTuple(args, "O:request_focus", &surface_capsule)) {
        return NULL;
    }
    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    if (surface == NULL) {
        return NULL;
    }
    fsapp_surface_request_focus(surface);
    Py_RETURN_NONE;
}

// _fsapp_surface.set_handler(surface, handler): the callable that gets the surface's events (or
// None for none), kept until it is replaced or the surface is destroyed.
static PyObject *fsapp_surface_python_set_handler(PyObject *self, PyObject *args)
{
    PyObject *surface_capsule;
    PyObject *handler;
    if (!PyArg_ParseTuple(args, "OO:set_handler", &surface_capsule, &handler)) {
        return NULL;
    }
    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    if (surface == NULL) {
        return NULL;
    }
    if (handler != Py_None && !PyCallable_Check(handler)) {
        PyErr_SetString(PyExc_TypeError, "The handler is not callable");
        return NULL;
    }
    if (surface->destroyed) {
        PyErr_SetString(PyExc_ValueError, "The surface is destroyed");
        return NULL;
    }
    PyObject *old = surface->event_handler;
    surface->event_handler = handler != Py_None ? Py_NewRef(handler) : NULL;
    Py_XDECREF(old);
    if (surface->event_handler != NULL) {
        fsapp_input_on_handler_set(surface);
    }
    Py_RETURN_NONE;
}

// _fsapp_surface.get_id(surface): the surface's id (its address), as surface events name it.
static PyObject *fsapp_surface_python_get_id(PyObject *self, PyObject *args)
{
    PyObject *surface_capsule;
    if (!PyArg_ParseTuple(args, "O:get_id", &surface_capsule)) {
        return NULL;
    }
    void *surface = PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    if (surface == NULL) {
        return NULL;
    }
    return PyLong_FromVoidPtr(surface);
}

// -------------------------------------------------------------------------------------------------

int fsapp_surface_convert_python_clip(PyObject *object, void *address)
{
    fsapp_surface_clip_t *clip = (fsapp_surface_clip_t *)address;
    if (object == Py_None) {
        clip->set = false;
        return 1;
    }
    if (!PyArg_ParseTuple(object, "iiii:clip", &clip->rect.x, &clip->rect.y, &clip->rect.w,
                          &clip->rect.h)) {
        return 0;
    }
    clip->set = true;
    return 1;
}

void fsapp_surface_begin_clip(fsapp_surface_t *surface, const fsapp_surface_clip_t *clip)
{
    if (surface->sdl != NULL && clip->set) {
        SDL_SetSurfaceClipRect(surface->sdl, &clip->rect);
    }
}

void fsapp_surface_end_clip(fsapp_surface_t *surface)
{
    if (surface->sdl != NULL) {
        SDL_SetSurfaceClipRect(surface->sdl, NULL);
    }
}

namespace
{

// Stores a colour in the pixels of a surface's buffer that are inside the clip (the whole buffer
// when it is not set), for the shapes that SDL does not draw.
struct plotter {
    int32_t *pixels;
    int width;
    // Empty without a buffer.
    SDL_Rect clip = {};
    uint32_t color;

    plotter(fsapp_surface_t *surface, const fsapp_surface_clip_t *surface_clip, uint32_t color)
        : pixels((int32_t *)surface->data), width(surface->state.width), color(color)
    {
        fsapp_surface_begin_clip(surface, surface_clip);
        SDL_GetSurfaceClipRect(surface->sdl, &clip);
        fsapp_surface_end_clip(surface);
    }

    void plot(int x, int y) const
    {
        if (x >= clip.x && x < clip.x + clip.w && y >= clip.y && y < clip.y + clip.h) {
            pixels[y * width + x] = color;
        }
    }

    // The pixels from x1 to x2 (inclusive) in row y.
    void plot_row(int y, int x1, int x2) const
    {
        if (y < clip.y || y >= clip.y + clip.h) {
            return;
        }
        x1 = SDL_max(x1, clip.x);
        x2 = SDL_min(x2, clip.x + clip.w - 1);
        int32_t *p = pixels + y * width;
        for (int x = x1; x <= x2; x++) {
            p[x] = color;
        }
    }

    // The pixels from y1 to y2 (inclusive) in column x.
    void plot_column(int x, int y1, int y2) const
    {
        if (x < clip.x || x >= clip.x + clip.w) {
            return;
        }
        y1 = SDL_max(y1, clip.y);
        y2 = SDL_min(y2, clip.y + clip.h - 1);
        for (int y = y1; y <= y2; y++) {
            pixels[y * width + x] = color;
        }
    }

    // A round brush of the width centred on x, y.
    void plot_brush(int x, int y, int line_width) const
    {
        if (line_width == 1) {
            plot(x, y);
            return;
        }
        int half_width = line_width / 2;
        for (int dy = -half_width; dy <= half_width; dy++) {
            for (int dx = -half_width; dx <= half_width; dx++) {
                if (dx * dx + dy * dy <= half_width * half_width + half_width) {
                    plot(x + dx, y + dy);
                }
            }
        }
    }
};

} // namespace

// -------------------------------------------------------------------------------------------------

static PyObject *fsapp_surface_python_draw_filled_rectangle(PyObject *self, PyObject *args)
{
    PyObject *surface_capsule;
    SDL_Rect rect;
    int r, g, b, a;
    fsapp_surface_clip_t clip = {};
    if (!PyArg_ParseTuple(args, "O(ii)(ii)(iiii)|O&:draw_filled_rectangle", &surface_capsule,
                          &rect.x, &rect.y, &rect.w, &rect.h, &r, &g, &b, &a,
                          fsapp_surface_convert_python_clip, &clip)) {
        return NULL;
    }

    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);

    // FIXME: ASSERT COLOR BYTES.. (0 - 255) - not masking
    // FIXME: For later - release the GIL while doing work?
    SDL_Color color = {(Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a};
    fsapp_surface_begin_clip(surface, &clip);
    fsapp_surface_draw_filled_rectangle(surface, &rect, color);
    fsapp_surface_end_clip(surface);

    Py_RETURN_NONE;
}

// _fsapp_surface.draw_rectangle(surface, (x, y), (w, h), colour, width, clip=None): the outline
// inside the rectangle, width pixels wide (the whole rectangle when it is that narrow).
static PyObject *fsapp_surface_python_draw_rectangle(PyObject *self, PyObject *args)
{
    PyObject *surface_capsule;
    int x, y, w, h;
    int r, g, b, a, line_width;
    fsapp_surface_clip_t clip = {};
    if (!PyArg_ParseTuple(args, "O(ii)(ii)(iiii)i|O&:draw_rectangle", &surface_capsule, &x, &y, &w,
                          &h, &r, &g, &b, &a, &line_width, fsapp_surface_convert_python_clip,
                          &clip)) {
        return NULL;
    }

    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);

    if (w <= 0 || h <= 0 || line_width <= 0) {
        Py_RETURN_NONE;
    }
    uint32_t pixel = premultiplied_pixel(surface, r, g, b, a);
    fsapp_surface_begin_clip(surface, &clip);
    if (2 * line_width >= w || 2 * line_width >= h) {
        SDL_Rect rect = {x, y, w, h};
        SDL_FillSurfaceRect(surface->sdl, &rect, pixel);
    } else {
        int inner_h = h - 2 * line_width;
        SDL_Rect sides[4] = {
            {x, y, w, line_width},
            {x, y + h - line_width, w, line_width},
            {x, y + line_width, line_width, inner_h},
            {x + w - line_width, y + line_width, line_width, inner_h},
        };
        SDL_FillSurfaceRects(surface->sdl, sides, 4, pixel);
    }
    fsapp_surface_end_clip(surface);

    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------

static PyObject *fsapp_surface_python_draw_line(PyObject *self, PyObject *args)
{
    // FIXME: For later - release the GIL while doing work!!

    PyObject *surface_capsule;
    int x1, y1, x2, y2;
    int r, g, b, a, line_width;
    fsapp_surface_clip_t clip = {};
    if (!PyArg_ParseTuple(args, "O(ii)(ii)(iiii)i|O&:draw_line", &surface_capsule, &x1, &y1, &x2,
                          &y2, &r, &g, &b, &a, &line_width, fsapp_surface_convert_python_clip,
                          &clip)) {
        return NULL;
    }

    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);

    // FIXME: ASSERT COLOR BYTES.. (0 - 255) - not masking

    plotter pen(surface, &clip, premultiplied_pixel(surface, r, g, b, a));

    // Fast paths for width=1 horizontal and vertical lines
    if (line_width == 1) {
        if (y1 == y2) {
            pen.plot_row(y1, SDL_min(x1, x2), SDL_max(x1, x2));
            Py_RETURN_NONE;
        } else if (x1 == x2) {
            pen.plot_column(x1, SDL_min(y1, y2), SDL_max(y1, y2));
            Py_RETURN_NONE;
        }
    }

    // AI-generated method for non-horizontal/vertical (experiment)

    // General case: thick lines or diagonal lines, Bresenham's line algorithm with a round brush.
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    int x = x1;
    int y = y1;

    while (true) {
        pen.plot_brush(x, y, line_width);

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
    fsapp_surface_clip_t clip = {};
    if (!PyArg_ParseTuple(args, "O(ii)i(iiii)i|O&:draw_circle", &surface_capsule, &center_x,
                          &center_y, &radius, &r, &g, &b, &a, &line_width,
                          fsapp_surface_convert_python_clip, &clip)) {
        return NULL;
    }

    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);

    plotter pen(surface, &clip, premultiplied_pixel(surface, r, g, b, a));

    // Midpoint circle algorithm with thick brush
    int x = 0;
    int y = radius;
    int d = 1 - radius;

    while (x <= y) {
        // Draw 8 symmetric points with thick brush
        int points_x[] = {x, -x, x, -x, y, -y, y, -y};
        int points_y[] = {y, y, -y, -y, x, x, -x, -x};

        for (int i = 0; i < 8; i++) {
            pen.plot_brush(center_x + points_x[i], center_y + points_y[i], line_width);
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
    fsapp_surface_clip_t clip = {};
    if (!PyArg_ParseTuple(args, "O(ii)i(iiii)|O&:draw_filled_circle", &surface_capsule, &center_x,
                          &center_y, &radius, &r, &g, &b, &a, fsapp_surface_convert_python_clip,
                          &clip)) {
        return NULL;
    }

    fsapp_surface_t *surface =
        (fsapp_surface_t *)PyCapsule_GetPointer(surface_capsule, "fsapp_surface_t");
    SDL_assert(surface != NULL);

    plotter pen(surface, &clip, premultiplied_pixel(surface, r, g, b, a));

    // Midpoint circle algorithm adapted for filled circles
    int x = 0;
    int y = radius;
    int d = 1 - radius;

    pen.plot_row(center_y, center_x - radius, center_x + radius);

    while (x < y) {
        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;

        // The rows y away from the centre, and (if different) x away.
        pen.plot_row(center_y + y, center_x - x, center_x + x);
        pen.plot_row(center_y - y, center_x - x, center_x + x);
        if (x != y) {
            pen.plot_row(center_y + x, center_x - y, center_x + y);
            pen.plot_row(center_y - x, center_x - y, center_x + y);
        }
    }

    Py_RETURN_NONE;
}

// -------------------------------------------------------------------------------------------------
// Raw access to a surface's buffer (_fsapp_surface.get_buffer).

namespace
{

// What a buffer's memoryview is made from. It holds a reference to the buffer's SDL_Surface, so
// that the memory stays valid if a resize or destroy frees the surface's buffer while a view (or a
// slice of one) is still alive. Released at the end of the UI frame, it exports no more views.
struct buffer_object {
    PyObject ob_base;
    SDL_Surface *sdl;
    bool released;
};

} // namespace

static PyTypeObject *g_buffer_type;

// A memoryview handed out by get_buffer in the current UI frame and what it was made from (a
// reference each), released at its end.
struct handed_buffer {
    PyObject *view;
    buffer_object *object;
};

static std::vector<handed_buffer> g_buffers;

static int buffer_object_get_buffer(PyObject *self, Py_buffer *view, int flags)
{
    buffer_object *object = (buffer_object *)self;
    if (object->sdl == NULL || object->released) {
        PyErr_SetString(PyExc_ValueError,
                        "The surface buffer was released at the end of a UI frame");
        view->obj = NULL;
        return -1;
    }
    SDL_Surface *sdl = object->sdl;
    return PyBuffer_FillInfo(view, self, sdl->pixels, (Py_ssize_t)sdl->pitch * sdl->h, 0, flags);
}

// The SDL_Surface's reference count is not atomic: it is changed only with the GIL held (here,
// get_buffer, and freeing a surface's buffer on the UI thread or from Python), or where no Python
// runs.
static void buffer_object_dealloc(PyObject *self)
{
    SDL_DestroySurface(((buffer_object *)self)->sdl);
    PyTypeObject *type = Py_TYPE(self);
    type->tp_free(self);
    Py_DECREF(type);
}

static PyType_Slot buffer_object_slots[] = {
    {Py_bf_getbuffer, (void *)buffer_object_get_buffer},
    {Py_tp_dealloc, (void *)buffer_object_dealloc},
    {0, NULL},
};

static PyType_Spec buffer_object_spec = {"_fsapp_surface.Buffer", sizeof(buffer_object), 0,
                                         Py_TPFLAGS_DEFAULT | Py_TPFLAGS_DISALLOW_INSTANTIATION,
                                         buffer_object_slots};

// _fsapp_surface.get_buffer(surface): a writable memoryview of the surface's buffer (premultiplied,
// bytes B, G, R, A on little-endian hosts, rows of width * 4 bytes), released at the end of the UI
// frame, before the record hands the buffer to the main thread, with what it was made from, which
// then exports no more views.
// REVIEW: A slice or cast of the view shares its export and is not released with it: writing
// through one after the UI frame writes into a buffer that the main thread may be reading (or into
// a buffer already replaced, which the export keeps alive, so never into freed memory). Revoking
// those too would take a buffer of our own per UI frame, copied into the surface's at its end.
static PyObject *fsapp_surface_python_get_buffer(PyObject *self, PyObject *args)
{
    fsapp_surface_t *surface;
    if (!PyArg_ParseTuple(args, "O&:get_buffer", convert_surface, &surface)) {
        return NULL;
    }
    if (surface->sdl == NULL) {
        PyErr_SetString(PyExc_ValueError, "The surface has no buffer");
        return NULL;
    }
    buffer_object *object = PyObject_New(buffer_object, g_buffer_type);
    if (object == NULL) {
        return NULL;
    }
    object->sdl = surface->sdl;
    object->sdl->refcount++;
    object->released = false;
    PyObject *view = PyMemoryView_FromObject((PyObject *)object);
    if (view == NULL) {
        Py_DECREF(object);
        return NULL;
    }
    // The list takes over the reference to the object.
    g_buffers.push_back({Py_NewRef(view), object});
    return view;
}

// Releases the memoryviews from get_buffer and what they were made from, so that using them, or
// making a new view from them, raises ValueError. Called with the GIL held when there are any (the
// Python UI thread holds it through its UI frames).
static void release_python_buffers(void)
{
    for (const handed_buffer &buffer : g_buffers) {
        buffer.object->released = true;
        PyObject *result = PyObject_CallMethod(buffer.view, "release", NULL);
        if (result == NULL) {
            // A view that something was made from (an export) cannot be released.
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Could not release a surface buffer's view");
            PyErr_Clear();
        }
        Py_XDECREF(result);
        Py_DECREF(buffer.view);
        Py_DECREF((PyObject *)buffer.object);
    }
    g_buffers.clear();
}

static PyMethodDef fsapp_surface_python_methods[] = {
    {"create", (PyCFunction)(void (*)(void))fsapp_surface_python_create,
     METH_VARARGS | METH_KEYWORDS, "..."},
    {"resize", fsapp_surface_python_resize, METH_VARARGS, "..."},
    {"commit", fsapp_surface_python_commit, METH_VARARGS, "..."},
    {"set_position", fsapp_surface_python_set_position, METH_VARARGS, "..."},
    {"set_role", fsapp_surface_python_set_role, METH_VARARGS, "..."},
    {"set_band", fsapp_surface_python_set_band, METH_VARARGS, "..."},
    {"set_key_target", fsapp_surface_python_set_key_target, METH_VARARGS, "..."},
    {"raise_surface", fsapp_surface_python_raise_surface, METH_VARARGS, "..."},
    {"set_translucent", fsapp_surface_python_set_translucent, METH_VARARGS, "..."},
    {"set_visible", fsapp_surface_python_set_visible, METH_VARARGS, "..."},
    {"set_focusable", fsapp_surface_python_set_focusable, METH_VARARGS, "..."},
    {"request_focus", fsapp_surface_python_request_focus, METH_VARARGS, "..."},
    {"set_handler", fsapp_surface_python_set_handler, METH_VARARGS, "..."},
    {"get_id", fsapp_surface_python_get_id, METH_VARARGS, "..."},
    {"take_events", fsapp_input_python_take_events, METH_VARARGS, "..."},
    {"set_window_handler", fsapp_input_python_set_window_handler, METH_VARARGS, "..."},
    {"take_configures", fsapp_input_python_take_configures, METH_VARARGS, "..."},
    {"take_window_state", fsapp_input_python_take_window_state, METH_VARARGS, "..."},
    {"draw_filled_rectangle", fsapp_surface_python_draw_filled_rectangle, METH_VARARGS, "..."},
    {"draw_rectangle", fsapp_surface_python_draw_rectangle, METH_VARARGS, "..."},
    {"draw_line", fsapp_surface_python_draw_line, METH_VARARGS, "..."},
    {"draw_circle", fsapp_surface_python_draw_circle, METH_VARARGS, "..."},
    {"draw_filled_circle", fsapp_surface_python_draw_filled_circle, METH_VARARGS, "..."},
    {"get_buffer", fsapp_surface_python_get_buffer, METH_VARARGS, "..."},
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
    PyObject *module = PyModule_Create(&fsapp_surface_python_module);
    if (module == NULL) {
        return NULL;
    }
    if (g_buffer_type == NULL) {
        g_buffer_type = (PyTypeObject *)PyType_FromSpec(&buffer_object_spec);
        if (g_buffer_type == NULL) {
            Py_DECREF(module);
            return NULL;
        }
    }
    // The kinds of surface events in take_events(), and of their devices.
    static const struct {
        const char *name;
        int kind;
    } kinds[] = {
        {"EVENT_KEY_DOWN", FSAPP_INPUT_EVENT_KEY_DOWN},
        {"EVENT_KEY_UP", FSAPP_INPUT_EVENT_KEY_UP},
        {"EVENT_FOCUS_IN", FSAPP_INPUT_EVENT_FOCUS_IN},
        {"EVENT_FOCUS_OUT", FSAPP_INPUT_EVENT_FOCUS_OUT},
        {"EVENT_NAVIGATION", FSAPP_INPUT_EVENT_NAVIGATION},
        {"EVENT_POINTER_ENTER", FSAPP_INPUT_EVENT_POINTER_ENTER},
        {"EVENT_POINTER_LEAVE", FSAPP_INPUT_EVENT_POINTER_LEAVE},
        {"EVENT_POINTER_MOTION", FSAPP_INPUT_EVENT_POINTER_MOTION},
        {"EVENT_POINTER_BUTTON", FSAPP_INPUT_EVENT_POINTER_BUTTON},
        {"EVENT_POINTER_WHEEL", FSAPP_INPUT_EVENT_POINTER_WHEEL},
        {"EVENT_OUTSIDE_CLICK", FSAPP_INPUT_EVENT_OUTSIDE_CLICK},
        // The kinds of input devices of navigation events.
        {"DEVICE_KEYBOARD", FSAPP_NAVIGATION_KEYBOARD},
        {"DEVICE_GAMEPAD", FSAPP_NAVIGATION_GAMEPAD},
        // Roles and bands (set_role, set_band).
        {"ROLE_TOP_LEVEL", FSAPP_STACKING_ROLE_TOP_LEVEL},
        {"ROLE_CHILD", FSAPP_STACKING_ROLE_CHILD},
        {"ROLE_POPUP", FSAPP_STACKING_ROLE_POPUP},
        {"ROLE_OVERLAY", FSAPP_STACKING_ROLE_OVERLAY},
        {"BAND_BACKGROUND", FSAPP_STACKING_BAND_BACKGROUND},
        {"BAND_NORMAL", FSAPP_STACKING_BAND_NORMAL},
        {"BAND_PANEL", FSAPP_STACKING_BAND_PANEL},
        // Scale supports (create).
        {"SCALE_ONE", FSAPP_PIXELS_SCALE_ONE},
        {"SCALE_ANY", FSAPP_PIXELS_SCALE_ANY},
    };
    for (const auto &entry : kinds) {
        if (PyModule_AddIntConstant(module, entry.name, entry.kind) < 0) {
            Py_DECREF(module);
            return NULL;
        }
    }
    return module;
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
    return a->visible == b->visible && a->x == b->x && a->y == b->y && a->role == b->role &&
           a->parent == b->parent && a->band == b->band && a->key_target == b->key_target &&
           a->translucent == b->translucent && same_color(a->fill_color, b->fill_color) &&
           a->focusable == b->focusable && a->scale_support == b->scale_support;
}

} // namespace

// The UI thread's side of the record.
static struct {
    // The record being collected (its storage is kept from UI frame to UI frame): the surfaces
    // created or changed, and the surfaces destroyed (the current UI frame's go in at once), each
    // holding a reference until the host window side has applied it.
    std::vector<record_entry> record;
    std::vector<fsapp_surface_t *> destroyed;
    // The raise and focus requests of the current UI frame, in order, each holding a reference.
    std::vector<fsapp_surface_t *> raise_requests;
    std::vector<fsapp_surface_t *> focus_requests;
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
    std::vector<fsapp_surface_t *> raise_requests;
    std::vector<fsapp_surface_t *> focus_requests;
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
    std::vector<fsapp_surface_t *> raise_requests;
    std::vector<fsapp_surface_t *> focus_requests;
    // The applied surfaces, in the order they were created (in the records), each holding a
    // reference.
    std::vector<fsapp_surface_t *> surfaces;
    // The last raise order given (fsapp_surface_raise).
    uint64_t raise_count;
    // Whether a record changed the layout since the flat list was built.
    bool layout_changed;
    bool taken_first;
    // The stacking unit's input and output (fsapp-stacking.h), and the flat list.
    std::vector<fsapp_stacking_surface_t> stacking;
    std::vector<fsapp_stacking_entry_t> order;
    std::vector<bool> stacked;
    std::vector<fsapp_snapshot_surface_t> flat;
    // The applied surfaces for routing, from back to front (fsapp_surface_get_routing_surfaces).
    std::vector<fsapp_routing_surface_t> routing;
    // The record's content and destroyed surfaces, for the snapshot of the frame that takes it.
    std::vector<fsapp_snapshot_content_t> contents;
    std::vector<const void *> destroyed_ids;
    // The display scale of the last configure (fsapp_surface_set_display_scale), which the applied
    // surfaces' ratios follow.
    float display_scale = 1.0f;
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

// Puts a raise request in the current UI frame's record. Outside a UI frame (before the UI thread
// is started), it goes with the first record.
static void request_raise_in_record(fsapp_surface_t *surface)
{
    fsapp_surface_ref(surface);
    g_ui.raise_requests.push_back(surface);
}

// Puts a focus request in the current UI frame's record. Outside a UI frame (before the UI thread
// is started), it goes with the first record.
static void request_focus_in_record(fsapp_surface_t *surface)
{
    fsapp_surface_ref(surface);
    g_ui.focus_requests.push_back(surface);
}

bool fsapp_surface_end_ui_frame(void)
{
    release_python_buffers();

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
    std::vector<fsapp_surface_t *> &raise_requests = g_ui.raise_requests;
    std::vector<fsapp_surface_t *> &focus_requests = g_ui.focus_requests;
    bool hand_over = !record.empty() || !destroyed.empty() || !raise_requests.empty() ||
                     !focus_requests.empty() || !g_ui.handed_first;
    SDL_LockMutex(g_slot.mutex);
    g_slot.in_ui_frame = false;
    if (hand_over) {
        // At most one record is in flight: the UI thread waited for the last to be taken in.
        SDL_assert(!g_slot.pending);
        g_slot.record.swap(record);
        g_slot.destroyed.swap(destroyed);
        g_slot.raise_requests.swap(raise_requests);
        g_slot.focus_requests.swap(focus_requests);
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

// The top-level (or overlay) of the applied surface's tree, following the parents of children and
// popups (only called for surfaces in the stacking order, whose parents lead to one).
static const fsapp_surface_t *find_applied_top_level(const fsapp_surface_t *surface)
{
    while (surface->applied.state.parent != NULL && has_parent_role(surface->applied.state.role)) {
        surface = surface->applied.state.parent;
    }
    return surface;
}

// The applied surface's ratio: how many device pixels one of its buffer pixels covers.
static double get_applied_ratio(const fsapp_surface_t *surface)
{
    return fsapp_pixels_get_ratio(surface->applied.state.scale_support, g_host.display_scale);
}

// The applied surface's device rectangle: its rectangle in buffer pixels at its ratio.
static fsapp_pixels_rectangle_t get_applied_device_rectangle(const fsapp_surface_t *surface)
{
    const auto &state = surface->applied.state;
    fsapp_pixels_rectangle_t rectangle = {state.x, state.y, state.width, state.height};
    fsapp_pixels_rectangle_t device;
    fsapp_pixels_get_device_rectangle(&rectangle, get_applied_ratio(surface), &device);
    return device;
}

// Computes the stacking order of the applied state and builds routing's list in it (every surface
// in the stacking order).
static void build_routing_list(void)
{
    const std::vector<fsapp_surface_t *> &surfaces = g_host.surfaces;
    std::vector<fsapp_stacking_surface_t> &stacking = g_host.stacking;
    stacking.clear();
    for (const fsapp_surface_t *surface : surfaces) {
        const auto &state = surface->applied.state;
        stacking.push_back(
            {surface, state.parent, state.role, state.band, surface->applied.raise, state.visible});
    }
    std::vector<fsapp_stacking_entry_t> &order = g_host.order;
    order.resize(surfaces.size());
    order.resize(fsapp_stacking_compute_order(stacking.data(), (int)stacking.size(), order.data()));

    std::vector<bool> &stacked = g_host.stacked;
    stacked.assign(surfaces.size(), false);
    for (const fsapp_stacking_entry_t &entry : order) {
        stacked[entry.index] = true;
    }
    for (size_t i = 0; i < surfaces.size(); i++) {
        fsapp_surface_t *surface = surfaces[i];
        // REVIEW: Destroying a parent destroys nothing else: its children and popups are left out
        // until they are given another parent or destroyed.
        if (!stacked[i] && !surface->applied.left_out) {
            SDL_Log("Surface %p left out of the stacking order: its parent %p is not there",
                    (void *)surface, (void *)surface->applied.state.parent);
        }
        surface->applied.left_out = !stacked[i];
    }

    g_host.routing.clear();
    for (const fsapp_stacking_entry_t &entry : order) {
        fsapp_surface_t *surface = surfaces[entry.index];
        const auto &applied = surface->applied;
        fsapp_routing_surface_t routing = {};
        routing.id = surface;
        // A surface with a buffer and no content yet has an empty rectangle, and takes no click.
        fsapp_pixels_rectangle_t device = get_applied_device_rectangle(surface);
        routing.rectangle = {device.x, device.y, device.width, device.height};
        routing.visible = entry.shown;
        routing.focusable = applied.state.focusable;
        routing.takes_pointer = applied.state.role != FSAPP_STACKING_ROLE_OVERLAY;
        routing.top_level = find_applied_top_level(surface);
        // Compared with the list's ids only: routing checks that it is in the tree.
        routing.key_target = applied.state.key_target;
        routing.popup = applied.state.role == FSAPP_STACKING_ROLE_POPUP;
        g_host.routing.push_back(routing);
    }
}

// Builds routing's list and the flat list from the applied state, in the stacking order: the flat
// list has the surfaces of routing's list with content (or streamed content) that are shown and
// not empty.
static void build_flat_list(void)
{
    build_routing_list();
    g_host.flat.clear();
    for (const fsapp_stacking_entry_t &entry : g_host.order) {
        fsapp_surface_t *surface = g_host.surfaces[entry.index];
        const auto &applied = surface->applied;
        if (!entry.shown || applied.state.width <= 0 || applied.state.height <= 0) {
            continue;
        }
        // A surface without a buffer has content to draw only with streamed content (the emulation
        // surface, drawn in the phase of drawing the paced surface; with no content yet, its fill).
        if (!surface->streamed && (surface->special || !applied.has_content)) {
            continue;
        }
        fsapp_snapshot_surface_t copy = {};
        copy.id = surface;
        fsapp_pixels_rectangle_t device = get_applied_device_rectangle(surface);
        copy.x = device.x;
        copy.y = device.y;
        copy.width = device.width;
        copy.height = device.height;
        copy.nearest = fsapp_pixels_is_whole_ratio(get_applied_ratio(surface));
        copy.visible = entry.shown;
        copy.translucent = applied.state.translucent;
        if (surface->streamed) {
            copy.streamed = true;
            copy.fill_color = applied.state.fill_color;
        }
        g_host.flat.push_back(copy);
    }
}

// Gives the applied surface the next raise order.
static void raise_applied(fsapp_surface_t *surface)
{
    g_host.raise_count += 1;
    surface->applied.raise = g_host.raise_count;
    g_host.layout_changed = true;
}

void fsapp_surface_raise_applied(const void *surface)
{
    for (fsapp_surface_t *applied : g_host.surfaces) {
        if (applied == surface) {
            raise_applied(applied);
            // Routing's list now, so that the next event is hit tested in the new order; the flat
            // list, which the snapshot written for the next frame may still be using, with the next
            // frame.
            build_routing_list();
            fsapp_frame_request();
            return;
        }
    }
}

const fsapp_routing_surface_t *fsapp_surface_get_routing_surfaces(int *count)
{
    *count = (int)g_host.routing.size();
    return g_host.routing.data();
}

void fsapp_surface_get_relative_position(const void *surface, double x, double y, float *relative_x,
                                         float *relative_y)
{
    *relative_x = (float)x;
    *relative_y = (float)y;
    for (const fsapp_routing_surface_t &routing : g_host.routing) {
        if (routing.id == surface) {
            const fsapp_routing_rectangle_t &rectangle = routing.rectangle;
            fsapp_pixels_rectangle_t device = {rectangle.x, rectangle.y, rectangle.width,
                                               rectangle.height};
            double buffer_x, buffer_y;
            fsapp_pixels_convert_to_buffer_position(
                &device, get_applied_ratio((const fsapp_surface_t *)surface), x, y, &buffer_x,
                &buffer_y);
            *relative_x = (float)buffer_x;
            *relative_y = (float)buffer_y;
            return;
        }
    }
}

void fsapp_surface_set_display_scale(float display_scale)
{
    if (display_scale == g_host.display_scale) {
        return;
    }
    g_host.display_scale = display_scale;
    // Every "1 only" surface's device rectangle changes: routing's list now, the flat list with
    // the next frame, as for a raise.
    g_host.layout_changed = true;
    build_routing_list();
    fsapp_frame_request();
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
        g_host.raise_requests.swap(g_slot.raise_requests);
        g_host.focus_requests.swap(g_slot.focus_requests);
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
        for (fsapp_surface_t *surface : g_host.raise_requests) {
            if (surface->applied.live) {
                raise_applied(surface);
            }
            // The record's reference.
            fsapp_surface_unref(surface);
        }
        g_host.raise_requests.clear();
        g_host.taken_first = true;
    }
    if (g_host.layout_changed) {
        build_flat_list();
        g_host.layout_changed = false;
    }
    if (taken) {
        // Keyboard focus follows the record: its fallback, then the focus requests in order.
        fsapp_input_on_record_applied(g_host.focus_requests.data(),
                                      (int)g_host.focus_requests.size());
        for (fsapp_surface_t *surface : g_host.focus_requests) {
            // The record's reference.
            fsapp_surface_unref(surface);
        }
        g_host.focus_requests.clear();
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

// The surfaces destroyed outside a UI frame that no record took are left in the queue: freeing
// their buffers here, on the main thread, could race with the UI thread, which may be running a UI
// frame and freeing a buffer view that shares a buffer (buffer_object_dealloc). The UI thread frees
// them if it ends another UI frame; otherwise they are left to the program's exit.
void fsapp_surface_quit(void)
{
    SDL_LockMutex(g_slot.mutex);
    g_slot.ended = true;
    int queued = (int)g_slot.queued.size();
    SDL_UnlockMutex(g_slot.mutex);
    if (queued > 0) {
        SDL_Log("%d destroyed surface(s) left when the main loop ended, for the UI thread", queued);
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
