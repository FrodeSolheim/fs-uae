/*
 * Surfaces: what the UI draws into, composited into the host window.
 *
 * A surface's positions, sizes, buffer, damage and picture rectangle are in its buffer pixels: for
 * a surface with the scale support "any", pixels of the host window's drawable (device pixels); for
 * one with "1 only" (the default), pixels that cover ratio device pixels each (fsapp-pixels.h). The
 * host window side draws and hit tests each surface at its device rectangle
 * (fsapp_pixels_get_device_rectangle), with nearest sampling when its ratio is whole, else linear,
 * and gives pointer positions to it in its buffer pixels.
 *
 * Each surface's state is split in two, so that no field is shared between threads:
 *
 * - The client state is what its client (the UI thread) has set: its buffer, size, position, role
 *   and parent, band, key target, visibility, translucent flag, fill colour, focusable flag and
 *   scale support, its event handler, and the damage committed since the last UI frame ended. The
 *   setters below write it. Only the UI thread reads and writes it (or the main thread before the
 *   UI thread is started, when the program sets up its surfaces).
 * - The applied state is what the main loop's host window side has applied from the records: the
 *   live surfaces that the flat list is built from. Only the host window side reads and writes it.
 *
 * A UI frame is one run of the UI thread's work (the program's frame function). Everything it
 * changes reaches the compositor through one record: at the end of each UI frame, fsapp collects
 * the surfaces created and changed in it, with their new client state and, for those committed,
 * their content (buffer, size and damaged rectangles), and the raise and focus requests made in it
 * (fsapp_surface_raise, fsapp_surface_request_focus), and hands the record over in a pending slot
 * under a short lock (never held while drawing, uploading or running Python), waking the main
 * loop. A UI frame that changed nothing hands over nothing (except the first UI frame, which always
 * does). Setters that set the value a surface already has change nothing. The UI frame's end is
 * fsapp's: the client calls nothing to end it.
 *
 * At most one record is in flight: the UI thread starts its next UI frame only once its last
 * record has been taken in. The host window side takes the pending record when it writes the
 * frame's snapshot, applies it to the applied state (rebuilding the flat list when the layout
 * changed), and puts its content in the snapshot (fsapp_snapshot_layout_t); the drawing side
 * uploads the content in the UI upload and then marks the record taken in, which lets the UI
 * thread go on. So a committed buffer belongs to the drawing side from the end of the UI frame
 * until it is taken in, without a copy, and everything one UI frame changed shows in the same swap.
 * Nothing drawn in a surface's buffer shows before it is committed, and then only inside the
 * committed damage, which is all the drawing side uploads (with glTexSubImage2D, straight from the
 * buffer). A buffer's new size takes effect with the content committed for it, which is uploaded
 * whole. No frame is drawn before the first record has been taken in.
 *
 * Destroying a surface is a change in the record too: its buffer is freed at once, and the host
 * window side removes it from the applied state with the record of the UI frame that destroyed it,
 * and the drawing side deletes its texture before it uploads, so that it disappears in the same
 * swap as the UI frame's other changes. A surface destroyed outside a UI frame goes with the next
 * record.
 *
 * A surface without a buffer can have streamed content instead (the emulation surface; "Streamed
 * content" below). Its frames do not go through the records: its client hands them to fsapp on the
 * drawing side, uploading each into the back one of two textures and committing it, which makes it
 * the front texture, drawn into the picture rectangle; the rest of the surface's rectangle (the
 * bars) is filled with its fill colour. Its rectangle, visibility, place in the stacking order and
 * fill colour are client state like any surface's.
 *
 * The stacking order (back to front) of the applied surfaces is computed from their roles, parents,
 * bands and raise order (fsapp-stacking.h, which has the rules); the flat list and routing's list
 * follow it. A surface left out of it (a child or popup whose parent is gone) is neither drawn nor
 * routed to, and neither is one that is not shown: a hidden surface hides its descendants.
 *
 * The refcount keeps a surface alive while its client (the Python capsule, or a C client), a child
 * whose parent it is, a top-level whose key target it is (until that top-level is destroyed), a
 * record or the applied state holds it.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSAPP_SURFACE_H
#define FSAPP_SURFACE_H

#include <Python.h>
#include <SDL3/SDL.h>

#include "fsapp-config.h"
#include "fs/log.h"
#include "fsapp-pixels.h"
#include "fsapp-refable.h"
#include "fsapp-routing.h"
#include "fsapp-snapshot.h"
#include "fsapp-stacking.h"

struct fsapp_surface;
typedef struct fsapp_surface fsapp_surface_t;

// The pixel format of streamed content (fsapp_surface_attach_streamed_content).
typedef enum fsapp_surface_stream_format {
    // Bytes B, G, R, A (SDL_PIXELFORMAT_ARGB8888 on little-endian hosts), 4 bytes per pixel. The
    // fourth byte is ignored: the content is opaque.
    FSAPP_SURFACE_STREAM_BGRA,
    // 16-bit pixels with red in the 5 most significant bits, green in the 6 middle bits and blue in
    // the 5 least significant bits (SDL_PIXELFORMAT_RGB565, native byte order), 2 bytes per pixel.
    // Fewer colours, half the bytes to upload: kept for a measurement on the Raspberry Pi.
    FSAPP_SURFACE_STREAM_RGB565,
} fsapp_surface_stream_format_t;

// How streamed content is sampled when it is drawn larger or smaller than its source rectangle.
typedef enum fsapp_surface_stream_filter {
    FSAPP_SURFACE_STREAM_FILTER_LINEAR,
    FSAPP_SURFACE_STREAM_FILTER_NEAREST,
} fsapp_surface_stream_filter_t;

#ifdef __cplusplus
extern "C" {
#endif

void fsapp_surface_init(void);

void fsapp_surface_ref(fsapp_surface_t *font);

void fsapp_surface_unref(fsapp_surface_t *font);

// A surface's state as its client sets it and a record carries it (all of its client state but the
// buffer and the damage), and as the host window side has applied it.
typedef struct fsapp_surface_state {
    bool visible;
    int x;
    int y;
    // The size. In the applied state, for a surface with a buffer, that of its last committed
    // content.
    int width;
    int height;
    // The role, and the parent of a child or popup, else NULL (fsapp_surface_set_role). In the
    // client state, the surface holds a reference to its parent.
    fsapp_stacking_role_t role;
    fsapp_surface_t *parent;
    // A top-level's band (fsapp_surface_set_band).
    fsapp_stacking_band_t band;
    // A top-level's key target, or NULL for itself (fsapp_surface_set_key_target). In the client
    // state, the surface holds a reference to it until the surface is destroyed.
    fsapp_surface_t *key_target;
    // Whether the surface is translucent (fsapp_surface_set_translucent).
    bool translucent;
    // The fill colour of a surface with streamed content (fsapp_surface_set_fill_color).
    SDL_Color fill_color;
    // Whether a top-level's tree may take keyboard focus (fsapp_surface_set_focusable).
    bool focusable;
    // What its client can draw at (fsapp_surface_set_scale_support).
    fsapp_pixels_scale_support_t scale_support;
} fsapp_surface_state_t;

struct fsapp_surface {
    FSAPP_REFABLE;

    // Set when the surface is created, and not changed after.

    int special;
    // Whether a surface without a buffer has streamed content
    // (fsapp_surface_attach_streamed_content), and its pixel format.
    bool streamed;
    fsapp_surface_stream_format_t stream_format;

    // The client state (the UI thread's, see the top of this file).

    // The buffer, in the native format that the compositor uploads without swizzling:
    // premultiplied BGRA (bytes B, G, R, A; SDL_PIXELFORMAT_ARGB8888 on little-endian hosts, and
    // big-endian hosts are not supported), contiguous, width x 4 bytes per row. NULL for a surface
    // without a buffer or with an empty size. Write it with fsapp's drawing functions, which take
    // straight RGBA colours and map them through its pixel format.
    SDL_Surface *sdl;
    void *data;

    // Whether the surface has been destroyed (fsapp_surface_destroy).
    bool destroyed;

    // The Python callable that gets the surface's events (_fsapp_surface.set_handler), or NULL.
    PyObject *event_handler;

    fsapp_surface_state_t state;
    // Whether the surface has been committed in the current UI frame, and the damage of its commits
    // in it (fsapp_surface_commit): the whole surface, or the rectangles (clipped, not empty).
    bool committed;
    fsapp_snapshot_damage_t damage;
    // Whether the rectangles have been merged into their bounding box (damage.rects[0]), in which
    // every further rectangle in the UI frame is merged too.
    bool damage_merged;
    // Whether the buffer has been reallocated (fsapp_surface_resize) since the surface was last
    // committed: its next commit is widened to the whole surface.
    bool resized;
    // Whether the surface is in the current UI frame's list of changed surfaces (which holds a
    // reference to it).
    bool changed;

    // The applied state (the host window side's, see the top of this file).

    struct {
        // Whether the surface is among the applied surfaces (which hold a reference to it).
        bool live;
        // Whether a surface with a buffer has content (committed with a size that is not empty).
        bool has_content;
        fsapp_surface_state_t state;
        // The raise order (fsapp-stacking.h): 0 until it is first raised (fsapp_surface_raise).
        uint64_t raise;
        // Whether it was left out of the last stacking order (logged when it is first left out).
        bool left_out;
    } applied;
};

// Create a surface. A surface without a buffer has no content of its own (the emulation surface,
// whose content is streamed, see fsapp_surface_attach_streamed_content). Call it on the UI thread,
// or on the main thread before the UI thread is started; the surface appears with the record of the
// UI frame that created it (or the first record).
fsapp_surface_t *fsapp_surface_create(int width, int height, bool without_buffer);

// Destroy the surface, letting go of the client's reference (do not use it after). Its buffer is
// freed at once, and it disappears with the record of the UI frame, together with the UI frame's
// other changes. Call it on the UI thread inside a UI frame (_fsapp_surface's destroy does the
// same, and so does freeing its Python capsule). Outside a UI frame (before the UI thread's first
// UI frame or after its last, such as a capsule freed in the exit handler on the main thread), the
// destruction is queued and goes with the next record, and what is still queued when the main loop
// has ended is freed then.
void fsapp_surface_destroy(fsapp_surface_t *surface);

// ----------------------------------------------------------------------------
// Streamed content.
//
// Unstable, like the other calls for the emulation surface: nothing here is published before the
// surface API's first version.
//
// A surface without a buffer can have streamed content: content on its own clock (the emulated
// picture, a video), which its client hands fsapp frame by frame on the main loop's drawing side,
// usually from the pacer's wait hook. fsapp keeps two textures for it: the slices of a frame go
// into the back texture, and the commit makes it the front texture, the one drawn, so a frame drawn
// without new content (a stall, a pause, no pacer) always shows one whole committed frame, never
// half of a new one. fsapp draws the front texture's source rectangle into the picture rectangle,
// and fills the rest of the surface's rectangle (the bars) with its fill colour
// (fsapp_surface_set_fill_color); with no content, the whole rectangle.
//
// The upload region is the part of a frame the client uploads; it lands at texel (0, 0), and the
// slices and the source rectangle are given in its coordinates (its top row first). The picture
// rectangle is in the surface's coordinates (relative to its top left corner, in pixels), so
// moving the surface needs no change; it may extend beyond the surface, and is
// clipped to it.

// Attach streamed content with its pixel format to a surface without a buffer, when it is created,
// on the same thread, before it is handed to the UI. It has no content until the first commit.
void fsapp_surface_attach_streamed_content(fsapp_surface_t *surface,
                                           fsapp_surface_stream_format_t format);

// The size of a pixel of the format in bytes (4 for BGRA, 2 for RGB565). Any thread.
int fsapp_surface_get_stream_pixel_size(fsapp_surface_stream_format_t format);

// The calls below are made on the main loop's drawing side (a pacer's wait hook, a drawing hook).

// Begin a frame with the upload region's size: the next slices go into the back texture. A frame
// begun while another is half uploaded abandons that one (a dropped frame): nothing of it is ever
// shown. Each texture grows with the largest region it has been given (created again, with no
// data, when a larger one comes), never above the GPU's largest texture: a larger region is cut to
// it, with a warning logged once. Only its top left part is then uploaded, and of the source
// rectangle given at the commit only what lies in that part is shown, in the part of the picture
// rectangle it maps to (the scale stays that of the whole source); the rest of the picture
// rectangle shows the fill colour, as the bars do.
void fsapp_surface_begin_streamed_frame(fsapp_surface_t *surface, int width, int height);

// Upload a slice of the frame begun: height rows of the upload region from row y, full width.
// pixels points to the first pixel of row y, and rows are stride bytes apart. The pixels are lent
// for the call only: they are read during it, and fsapp keeps no pointer to them.
void fsapp_surface_upload_streamed_slice(fsapp_surface_t *surface, int y, int height,
                                         const void *pixels, int stride);

// Commit the frame begun: its back texture becomes the front texture, drawn from this frame on
// (when called in the wait, in this frame's swap), with the source rectangle (the part of the
// upload region shown) drawn into the picture rectangle (the surface's coordinates). Every row of
// the region should have been uploaded.
void fsapp_surface_commit_streamed_frame(fsapp_surface_t *surface, const SDL_Rect *source,
                                         const SDL_Rect *picture);

// Set how the content is sampled (linear by default).
void fsapp_surface_set_streamed_filter(fsapp_surface_t *surface,
                                       fsapp_surface_stream_filter_t filter);

// Set the picture rectangle (the surface's coordinates) of the content shown, without a commit: a
// refit when the surface's size changed, also while paused (no pacer). Call it before drawing below
// (in a drawing hook) for it to show in this frame. The next commit gives its own.
void fsapp_surface_set_streamed_picture(fsapp_surface_t *surface, const SDL_Rect *picture);

// Drop the content: from now on (in this frame when called before drawing below) the surface has
// no content, and its fill colour shows over its whole rectangle, until the next commit.
void fsapp_surface_drop_streamed_content(fsapp_surface_t *surface);

// The surface's rectangle (pixels) in the frame being drawn, as the frame's snapshot
// has it, so that a drawing hook can fit the picture without asking the host window side. Returns
// whether the surface is in the frame's flat list (visible and not empty); if not, rect is left as
// it is.
bool fsapp_surface_get_frame_rectangle(fsapp_surface_t *surface, SDL_Rect *rect);

// ----------------------------------------------------------------------------

// Commit the surface's content in the damaged rectangles (surface pixels), or with damage_count 0
// the whole surface: what is drawn in its buffer inside the damage shows with the record of the UI
// frame. Nothing drawn shows before it is committed, and what is drawn outside the damage shows
// only when a later commit's damage covers it (the compositor reads the buffer only inside the
// damage), so commit what you draw. The rectangles are clipped to the buffer and empty ones
// dropped; several commits in one UI frame are combined, above FSAPP_SNAPSHOT_MAX_DAMAGE
// rectangles merged into their bounding box (and every later rectangle in the UI frame into the
// box), and a rectangle covering the whole surface replaces the others. Call it on the UI thread
// inside a UI frame (or before the UI thread is started), after drawing. A surface without a buffer
// has nothing to commit (its changes are layout only). No raw access to the buffer is offered yet.
// When it is, a raw pointer to a surface's buffer is valid for one UI frame only (it may be a
// mapped pixel buffer later).
void fsapp_surface_commit(fsapp_surface_t *surface, const SDL_Rect *damage, int damage_count);

// Resize the surface. A surface with a buffer gets a new buffer of the new size at once (the old
// content is gone: draw it all again), but the new size takes effect only with the surface's next
// commit, which counts as damage to the whole surface whatever damage it names (and a commit made
// before the resize in the same UI frame is dropped, since its buffer is gone). Until then the old
// content is drawn at the old size, never stretched or half drawn. A surface without a buffer (the
// emulation surface) changes size with the UI frame's record, as layout, together with the UI
// frame's other changes. Call it on the UI thread inside a UI frame (or before the UI thread is
// started; _fsapp_surface's resize does the same).
void fsapp_surface_resize(fsapp_surface_t *surface, int width, int height);

// Unstable: the surface's role and, for a child or popup, its parent, which decide its place in the
// stacking order (fsapp-stacking.h). A top-level by default. A child or popup takes a parent, and a
// top-level or an overlay none: a call that breaks this, or names the surface itself, one of its
// descendants or a destroyed surface as the parent, is logged as an error and ignored. The surface
// holds a reference to its parent. A child's or popup's position is in the host window's pixels, as
// any surface's: it does not follow its parent. Like the other setters below, it writes the client
// state, so call it on the UI thread inside a UI frame (or before the UI thread is started); the
// change goes with the UI frame's record (_fsapp_surface's set_role does the same).
// REVIEW: The strictness about parents and roles, and that children do not follow their parent.
void fsapp_surface_set_role(fsapp_surface_t *surface, fsapp_stacking_role_t role,
                            fsapp_surface_t *parent);

// Unstable: the band of a top-level (fsapp-stacking.h; it counts for top-levels only). Normal by
// default. Called as fsapp_surface_set_role (_fsapp_surface's set_band does the same).
void fsapp_surface_set_band(fsapp_surface_t *surface, fsapp_stacking_band_t band);

// Unstable, for the top: names the surface in a top-level's tree that gets the keys while the tree
// has keyboard focus (its key target, fsapp-input.h), or with NULL the top-level itself, as by
// default. The key target stays named while it is hidden or out of the tree, and the top-level gets
// the keys meanwhile. A call for a surface that is not a top-level, or naming a surface outside its
// tree or destroyed, is logged as an error and ignored. Called as fsapp_surface_set_role
// (_fsapp_surface's set_key_target does the same).
void fsapp_surface_set_key_target(fsapp_surface_t *top_level, fsapp_surface_t *surface);

// Unstable: raises the surface in front of the others of its kind (fsapp-stacking.h): a top-level
// in front of the top-levels in its band, an overlay in front of the overlays, and a child or popup
// in front of its siblings. The request goes with the UI frame's record and is applied after the
// record's other changes, in order with the UI frame's other raise requests; it is ignored for a
// surface destroyed by then. Called as fsapp_surface_set_role (_fsapp_surface's raise_surface does
// the same).
// REVIEW: Whether children and popups should be raised among their siblings, or only top-levels and
// overlays raised.
void fsapp_surface_raise(fsapp_surface_t *surface);

// Unstable: whether the surface is translucent (blended with its content's alpha, premultiplied) or
// opaque (its content's alpha ignored). Opaque by default. Called as fsapp_surface_set_role
// (_fsapp_surface's set_translucent does the same).
void fsapp_surface_set_translucent(fsapp_surface_t *surface, bool translucent);

// Unstable: the fill colour of a surface with streamed content (the emulation surface), for fsapp
// to fill the surface's rectangle with before the wait, so that the bars around its content and a
// surface with no content show it. Its alpha is ignored (the fill is opaque). Black by default.
// Called as fsapp_surface_set_role.
void fsapp_surface_set_fill_color(fsapp_surface_t *surface, SDL_Color fill_color);

// The surface's position in its buffer pixels. Called as fsapp_surface_set_role
// (_fsapp_surface's set_position does the same).
void fsapp_surface_set_position(fsapp_surface_t *surface, int x, int y);

// Whether the surface is shown (hidden when created). Called as fsapp_surface_set_role
// (_fsapp_surface's set_visible does the same).
void fsapp_surface_set_visible(fsapp_surface_t *surface, bool visible);

// Unstable: whether a top-level's tree may take keyboard focus (fsapp-input.h; for other surfaces
// it does not count). Not focusable by default. Called as fsapp_surface_set_role (_fsapp_surface's
// set_focusable does the same).
void fsapp_surface_set_focusable(fsapp_surface_t *surface, bool focusable);

// Unstable: the surface's scale support (fsapp-pixels.h): "1 only" (FSAPP_PIXELS_SCALE_ONE) by
// default, or "any" (FSAPP_PIXELS_SCALE_ANY). Its handler's configure follows it: the host
// window's size in its buffer pixels and its surface scale (fsapp_pixels_get_surface_configure).
// An unknown value is logged as an error and ignored. Like the other setters, call it on the UI
// thread inside a UI frame (or before the UI thread is started); the change goes with the UI
// frame's record. From Python it is set only when the surface is created (_fsapp_surface's create
// takes it).
void fsapp_surface_set_scale_support(fsapp_surface_t *surface,
                                     fsapp_pixels_scale_support_t scale_support);

// Unstable: the surface's scale support (fsapp_surface_set_scale_support), from the client state:
// call it on the UI thread (or before the UI thread is started).
fsapp_pixels_scale_support_t fsapp_surface_get_scale_support(const fsapp_surface_t *surface);

// Unstable: asks for keyboard focus for the surface's tree (fsapp-input.h). The request goes with
// the UI frame's record and is applied, in order with the UI frame's other requests, after the
// record's other changes; it is ignored if the tree's top-level is not focusable or not visible
// then. Called as fsapp_surface_set_role (_fsapp_surface's request_focus does the same).
void fsapp_surface_request_focus(fsapp_surface_t *surface);

// Fills a rectangle of the surface's buffer with a colour (straight RGBA), clipped to the buffer's
// clip rectangle, storing it premultiplied in the buffer's format, as fsapp's drawing functions all
// leave the buffer. Call it on the UI thread inside a UI frame (_fsapp_surface's
// draw_filled_rectangle does the same); it shows once committed.
void fsapp_surface_draw_filled_rectangle(fsapp_surface_t *surface, const SDL_Rect *rect,
                                         SDL_Color color);

// The clip rectangle that the Python drawing functions of _fsapp_surface, _fsapp_font and
// _fsapp_image take last: (x, y, width, height) in the buffer's pixels, or None (or left out) for
// the whole buffer.
typedef struct fsapp_surface_clip {
    bool set;
    SDL_Rect rect;
} fsapp_surface_clip_t;

// A PyArg_ParseTuple converter ("O&") from a clip rectangle or None to an fsapp_surface_clip_t.
int fsapp_surface_convert_python_clip(PyObject *object, void *address);

// Makes the clip (intersected with the buffer) the buffer's clip rectangle until
// fsapp_surface_end_clip: SDL's fills and blits draw only inside it, and fsapp's own drawing code
// reads it with SDL_GetSurfaceClipRect.
void fsapp_surface_begin_clip(fsapp_surface_t *surface, const fsapp_surface_clip_t *clip);

// Makes the whole buffer the clip rectangle again.
void fsapp_surface_end_clip(fsapp_surface_t *surface);

// A new Python capsule for the surface, as _fsapp_surface's functions take. The capsule holds a
// reference, and destroys the surface when it is freed. Call with the GIL held.
PyObject *fsapp_surface_create_python_capsule(fsapp_surface_t *surface);

// ----------------------------------------------------------------------------
// The record (see the top of this file), for fsapp's UI thread and main loop.

// The UI thread, at the end of each UI frame: releases the views of the buffers that Python took in
// it (_fsapp_surface.get_buffer; with the GIL held when Python took any), then collects the UI
// frame's record and hands it over, if anything changed (or it is the first UI frame), waking the
// main loop. Returns whether it handed one over.
bool fsapp_surface_end_ui_frame(void);

// The UI thread, at the start of each UI frame (the first too): waits until the last record handed
// over has been taken in (returning at once before the first UI frame, and when the last UI frame
// handed over nothing). From then until fsapp_surface_end_ui_frame, the UI thread is inside a UI
// frame. Returns false when the UI frames are stopped (fsapp_surface_stop_ui_frames).
bool fsapp_surface_wait_for_next_ui_frame(void);

// Any thread: ends the UI thread's wait for good, so that it can end.
void fsapp_surface_stop_ui_frames(void);

// The host window side: whether a record has been handed over and not taken yet (it makes a frame
// needed), and whether the first record has been taken (before then, no frame is drawn).
bool fsapp_surface_has_pending_record(void);
bool fsapp_surface_has_taken_first_record(void);

// The host window side, when it writes the snapshot: takes the pending record, if there is one,
// with the surfaces destroyed since the last one, applies it to the applied state, and writes the
// layout: the flat list, and the record's content and destroyed surfaces (fsapp_snapshot_layout_t).
// The layout's storage is the host window side's, until the next call.
//
// untaken is the layout of the next snapshot when the drawing side has not taken it yet (render
// thread mode, fsapp-mainthread.h), whose record (its content and destroyed surfaces) is then
// carried over into this one, and no new pending record is taken until it has been taken (at most
// one record is in flight anyway: the UI thread hands over the next only after this one was taken
// in). NULL when there is none (always in one-thread mode).
void fsapp_surface_write_snapshot(fsapp_snapshot_layout_t *layout,
                                  const fsapp_snapshot_layout_t *untaken);

// The host window side: the applied surfaces for routing (fsapp-routing.h), in the stacking order
// (back to front), with their device rectangles (fsapp_pixels_get_device_rectangle), visibility,
// focusable flag, whether they take pointer input, their tree's top-level and a top-level's key
// target, as of the last record applied or raise. The storage is the host window side's, until the
// next record is applied or a surface is raised.
const fsapp_routing_surface_t *fsapp_surface_get_routing_surfaces(int *count);

// The host window side: a position in device pixels relative to an applied surface in routing's
// list, in its buffer pixels (fsapp_pixels_convert_to_buffer_position); unchanged for a surface not
// in it.
void fsapp_surface_get_relative_position(const void *surface, double x, double y, float *relative_x,
                                         float *relative_y);

// The host window side, with each configure: the display scale, which the applied surfaces' ratios
// follow (fsapp_pixels_get_ratio). A change rebuilds routing's list at once and the flat list with
// the next frame, which this asks for.
void fsapp_surface_set_display_scale(float display_scale);

// The host window side: raises an applied surface (identified by its pointer, as in routing's list;
// nothing happens for one that is not applied) as fsapp_surface_raise does, at once rather than
// with a record: for a press (fsapp-input.h). Routing's list follows at once, the flat list with
// the next frame, which this asks for.
void fsapp_surface_raise_applied(const void *surface);

// The drawing side, at the end of the UI upload of every frame: marks the layout's record (if it
// carries one) taken in, which lets the UI thread start its next UI frame.
void fsapp_surface_on_uploaded(const fsapp_snapshot_layout_t *layout);

// The host window side, when the main loop has ended: from then on, a surface destroyed outside a
// UI frame is freed at once. Those destroyed before that no record took are left to the UI thread's
// next UI frame, if it runs one.
void fsapp_surface_quit(void);

#ifdef __cplusplus
}
#endif

#endif /* FSAPP_SURFACE_H */
