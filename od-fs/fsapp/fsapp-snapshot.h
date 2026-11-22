/*
 * The snapshot: the per-frame record that the main loop's host window side writes and its drawing
 * side takes when its frame starts. It is all of the host window side's state that the drawing side
 * reads (see fsapp-mainthread.h), so that the drawing side can run on a render thread without new
 * locks.
 *
 * In one-thread mode, the drawing side takes each snapshot right after it is written. In render
 * thread mode, the host window side writes the next snapshot into one slot under a short lock,
 * with its own copies of the arrays, and the render thread takes the newest when its frame starts
 * (else it draws from the one it has). A snapshot is written over while not taken yet (the normal
 * case with a pacer): what is state (the window, the flat list, the adornments' places and flags,
 * the pacer, the phases) is then the newest, and what is handed over only once is carried over
 * into it by the module that owns it, so that nothing is lost or handed over twice: a record with
 * its content and destroyed surfaces (fsapp_surface_write_snapshot, which takes no new record
 * meanwhile), destroyed adornments, the adornments' change and their new content, newest content
 * with the damaged rows combined (fsapp_adornment_write_snapshot). A frame drawn again from a
 * snapshot already taken has what is handed over only once cleared.
 *
 * Streamed content (the emulation surface's) is not in the snapshot: its client hands it to the
 * drawing side itself (fsapp-surface.h, "Streamed content"), which keeps it by the surface's id
 * (fsapp-streamed.h). The snapshot only says which surfaces have it, with their fill colour, which
 * fills the bars around the picture (or the whole surface without content).
 *
 * Copyright (c) 2026 Frode Solheim
 */

#ifndef FSAPP_SNAPSHOT_H
#define FSAPP_SNAPSHOT_H

#include <SDL3/SDL.h>
#include <stdbool.h>

#include "fsapp-adornment.h"
#include "fsapp-pacer.h"

#ifdef __cplusplus
extern "C" {
#endif

// The host window's state.
typedef struct fsapp_snapshot_window {
    // The window to swap. The drawing side uses it only for SDL_GL_SwapWindow.
    SDL_Window *sdl_window;
    // The size in window coordinates, SDL's unit, for host window operations only.
    int width;
    int height;
    // The size in pixels (of the drawable), which the surfaces' positions and sizes are in. It
    // differs from the size in window coordinates with high pixel density.
    int pixel_width;
    int pixel_height;
    // Pixels per window coordinate (SDL_GetWindowPixelDensity).
    float scale;
    // The colour the window is cleared with, set by the UI.
    SDL_Color background_color;
    // The host refresh rate in Hz (60 when SDL cannot tell), and whether every swap waits for a
    // host vertical blank, whatever the swap interval (see fsapp_mainthread_display_hook_t).
    double refresh_rate_hz;
    bool every_swap_waits;
    // Whether SDL told the host refresh rate (else refresh_rate_hz is the assumed 60 Hz).
    bool refresh_rate_known;
    // Whether the return of a glFinish() after the swap is taken as the observed host vertical
    // blank on this platform (see fsapp_frame_host_vertical_blank_t), else as an estimate.
    bool host_vertical_blank_observed;
} fsapp_snapshot_window_t;

// A surface in the flat list, from the host window side's applied state (fsapp-surface.h).
typedef struct fsapp_snapshot_surface {
    // Identifies the surface (and the drawing side's texture for it) from frame to frame. The
    // drawing side never dereferences it.
    const void *id;
    // The device rectangle (fsapp_pixels_get_device_rectangle): for a surface with a buffer, its
    // last committed content (its texture's size, in buffer pixels) at its position, at its ratio.
    // The whole texture is drawn into it.
    int x;
    int y;
    int width;
    int height;
    // Whether its texture is drawn with nearest sampling (its ratio is whole), else linear.
    bool nearest;
    bool visible;
    // Translucent (blended with its content's alpha, premultiplied) or opaque (its content's alpha
    // ignored), as the client set it (fsapp_surface_set_translucent).
    bool translucent;
    // Whether a surface without a buffer has streamed content instead (the emulation surface,
    // fsapp_surface_attach_streamed_content), which the drawing side keeps by the surface's id.
    bool streamed;
    // The fill colour of a surface with streamed content (fsapp_surface_set_fill_color).
    SDL_Color fill_color;
} fsapp_snapshot_surface_t;

// The most damaged rectangles a surface has in one UI frame's record: above this, they are merged
// into their bounding box (fsapp_surface_commit).
// REVIEW: The count is a guess; measure the UI upload's time before changing it.
#define FSAPP_SNAPSHOT_MAX_DAMAGE 8

// The damage of a surface's content: what changed in it, the whole content or the rectangles
// (within it, not empty; none: nothing).
typedef struct fsapp_snapshot_damage {
    bool whole;
    SDL_Rect rects[FSAPP_SNAPSHOT_MAX_DAMAGE];
    int count;
} fsapp_snapshot_damage_t;

// Content committed by the UI, carried by the record that the frame takes in: the drawing side
// uploads it to the surface's texture in the UI upload.
typedef struct fsapp_snapshot_content {
    // The surface (fsapp_snapshot_surface_t.id).
    const void *id;
    // The surface's buffer (premultiplied BGRA, width x height pixels, contiguous). It is the
    // drawing side's until the record is taken in (fsapp_surface_on_uploaded): the UI thread does
    // not start its next UI frame before then. The drawing side reads it only inside the damage.
    const void *buffer;
    int width;
    int height;
    // Whether the surface's texture is new or changes size, so that it is (re)allocated from the
    // whole buffer; else the damage is uploaded into the texture it has.
    bool size_changed;
    // What to upload, unless size_changed: the whole buffer, or the damaged rectangles.
    fsapp_snapshot_damage_t damage;
} fsapp_snapshot_content_t;

// The surface layout: the flat list, from the host window side's applied state in every frame, and
// what the record that this frame takes in brought (fsapp-surface.h).
typedef struct fsapp_snapshot_layout {
    // Whether this frame takes in a record: the drawing side deletes the textures of its destroyed
    // surfaces and uploads its content in the UI upload, and then marks it taken in
    // (fsapp_surface_on_uploaded), which lets the UI thread start its next UI frame.
    bool takes_record;
    // The flat list: the surfaces with content (a buffer committed at least once) or streamed
    // content that are shown (they and their ancestors visible) and not empty, from back to front
    // in the stacking order, which the host window side computes (fsapp-stacking.h).
    const fsapp_snapshot_surface_t *surfaces;
    int surface_count;
    // The record's content: the surfaces committed in it.
    const fsapp_snapshot_content_t *contents;
    int content_count;
    // The surfaces destroyed with the record, whose textures the drawing side deletes before it
    // uploads the record's content.
    const void *const *destroyed;
    int destroyed_count;
} fsapp_snapshot_layout_t;

// An adornment (fsapp-adornment.h).
typedef struct fsapp_snapshot_adornment {
    // Identifies the adornment (and the drawing side's texture for it) from frame to frame; the
    // drawing side hands it back to fsapp_adornment_on_uploaded.
    struct fsapp_adornment *id;
    fsapp_adornment_place_t place;
    // The surface it is attached in front of (its id in the layout, fsapp_snapshot_surface_t.id)
    // for FSAPP_ADORNMENT_IN_FRONT_OF_SURFACE, else NULL. The drawing side never dereferences it.
    const void *surface;
    // The rectangle in pixels.
    int x;
    int y;
    int width;
    int height;
    bool opaque;
    fsapp_adornment_filter_t filter;
    // Content committed since the previous snapshot (RGBA, premultiplied when translucent), to be
    // uploaded in the UI upload, else NULL. The owner keeps it unchanged until it is taken in.
    const void *content;
    int content_width;
    int content_height;
    int content_stride;
    // The rows of the content that changed: the damaged rows of all the commits since the previous
    // snapshot, combined into one span from the first to the last (content_damage_height 0: none).
    // The drawing side uploads only these, unless its texture is new or of another size.
    int content_damage_y;
    int content_damage_height;
    // Which of the adornment's commits the content is (counted by its owner's commits), which the
    // drawing side hands back (fsapp_adornment_on_uploaded) to mark that content taken in.
    uint64_t content_commit;
} fsapp_snapshot_adornment_t;

// The adornments, in the order they were created.
typedef struct fsapp_snapshot_adornments {
    const fsapp_snapshot_adornment_t *adornments;
    int count;
    // The adornments destroyed since the previous snapshot, whose textures the drawing side
    // deletes before it uploads this snapshot's content.
    struct fsapp_adornment *const *destroyed;
    int destroyed_count;
    // Whether an adornment changed since the previous snapshot (created, destroyed, its place,
    // rectangle, flags or content), which makes a frame needed.
    bool changed;
} fsapp_snapshot_adornments_t;

// The pacer, as set or cleared on the host window side (fsapp_mainthread_set_pacer).
typedef struct fsapp_snapshot_pacer {
    bool set;
    fsapp_pacer_t pacer;
} fsapp_snapshot_pacer_t;

// When the host window side's phases of the frame started (on SDL's clock, in nanoseconds), for the
// frame record.
typedef struct fsapp_snapshot_phases {
    int64_t events_ns;
    int64_t frame_work_ns;
} fsapp_snapshot_phases_t;

typedef struct fsapp_snapshot {
    fsapp_snapshot_window_t window;
    fsapp_snapshot_layout_t layout;
    fsapp_snapshot_adornments_t adornments;
    fsapp_snapshot_pacer_t pacer;
    fsapp_snapshot_phases_t phases;
} fsapp_snapshot_t;

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSAPP_SNAPSHOT_H */
