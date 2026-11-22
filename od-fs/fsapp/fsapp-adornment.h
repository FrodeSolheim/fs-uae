/*
 * Adornments: rectangles of content that fsapp draws in the host window, attached in front of a
 * surface or in front of every surface, taking no input. They are never hit-tested or focused, and
 * are not part of the UI's stacking. The performance graph is drawn as adornments.
 *
 * The calls can be made from any thread. Their changes are taken in when the next frame of the
 * host window starts: the place, the rectangle and the flags through the frame's snapshot
 * (fsapp-snapshot.h), the content in the frame's UI upload (before the wait). The UI upload uploads
 * only the rows that a commit names as damaged, and the whole content when the adornment's texture
 * is new or its content size changes. When there are several commits before a frame takes them in,
 * the newest content is taken, with the damaged rows of all of them combined into one span (from
 * the first damaged row to the last). The content stays RGBA in byte order (uploaded as GL_RGBA).
 * fsapp draws the adornments from their content every frame, with no hook called, so they stay
 * visible also without a pacer.
 *
 * They are drawn by the frame plan's rules (fsapp-frame-plan.h), as the surfaces are: one attached
 * in front of a surface just in front of it and behind the surfaces in front of it, one in front
 * of everything in front of every surface, later ones in front on the same place. Opaque ones are
 * drawn before the wait, translucent ones (premultiplied) before the wait when they are behind the
 * emulation surface and after it (drawing above) when they are in front of it.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#ifndef FSAPP_ADORNMENT_H
#define FSAPP_ADORNMENT_H

#include <SDL3/SDL.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct fsapp_adornment;
typedef struct fsapp_adornment fsapp_adornment_t;

struct fsapp_surface;
struct fsapp_snapshot_adornments;

// Where an adornment is drawn.
typedef enum fsapp_adornment_place {
    // In front of every surface, the paced surface included (fsapp_adornment_create_in_front).
    FSAPP_ADORNMENT_IN_FRONT_OF_EVERYTHING,
    // Just in front of one surface, behind the surfaces in front of it, and drawn only while that
    // surface is in the layout (fsapp_adornment_create_in_front_of).
    FSAPP_ADORNMENT_IN_FRONT_OF_SURFACE,
} fsapp_adornment_place_t;

// How an adornment's content is filtered when it is stretched to its rectangle.
typedef enum fsapp_adornment_filter {
    FSAPP_ADORNMENT_FILTER_LINEAR,
    FSAPP_ADORNMENT_FILTER_NEAREST,
} fsapp_adornment_filter_t;

// Creates an adornment in front of everything (all surfaces, the paced surface included). It is
// opaque, filtered linearly, with an empty rectangle and no content, so nothing is drawn until both
// are set.
fsapp_adornment_t *fsapp_adornment_create_in_front(void);

// Creates an adornment attached just in front of the surface: in front of its content and behind
// the surfaces in front of it. It is drawn only while the surface is visible, and opaque content
// only so far. Otherwise as fsapp_adornment_create_in_front. The surface must outlive the
// adornment.
fsapp_adornment_t *fsapp_adornment_create_in_front_of(struct fsapp_surface *surface);

// Destroys the adornment; its owner must not use it afterwards. Content committed and not yet taken
// in (fsapp_adornment_is_content_taken) may still be read until the next frame's UI upload is done.
// Adornments still alive when the main loop ends are freed then (fsapp_adornment_quit), so their
// owners must not use them after fsapp_mainthread_run has returned either.
void fsapp_adornment_destroy(fsapp_adornment_t *adornment);

// The rectangle in pixels, which the content is stretched to.
void fsapp_adornment_set_rect(fsapp_adornment_t *adornment, int x, int y, int width, int height);

// Opaque (the default) or translucent. Translucent content is premultiplied by its alpha.
void fsapp_adornment_set_opaque(fsapp_adornment_t *adornment, bool opaque);

// The filter used when the content is stretched to the rectangle: linear (the default) or nearest.
void fsapp_adornment_set_filter(fsapp_adornment_t *adornment, fsapp_adornment_filter_t filter);

// Commits new content: width x height RGBA pixels (bytes R, G, B, A), rows stride bytes apart
// (a multiple of 4), of which the rows from damage_y and damage_height down changed since the
// previous commit; only those are uploaded (all of it for the first commit, or when the size
// changes). The content is handed to fsapp, which reads it in the next frame's UI upload: it must
// stay valid and unchanged until fsapp_adornment_is_content_taken returns true (fsapp makes no
// copy). A commit before the previous one is taken in replaces it, and the damaged rows of both
// are uploaded, so the newest content must hold every row that either of them changed.
void fsapp_adornment_commit(fsapp_adornment_t *adornment, const void *pixels, int width, int height,
                            int stride, int damage_y, int damage_height);

// True when the last committed content has been taken in (uploaded), so the owner may change it
// again; also true before the first commit.
bool fsapp_adornment_is_content_taken(fsapp_adornment_t *adornment);

// ----------------------------------------------------------------------------
// For fsapp's main loop.

// True when an adornment has changed since the last snapshot, which makes a frame needed (also
// without a pacer). Any thread.
bool fsapp_adornment_has_changed(void);

// Writes the adornments into the snapshot (host window side): the live ones with their place,
// rectangle and flags, and content newly committed since the previous snapshot, the ones destroyed
// since the previous snapshot, and whether any of this changed. The arrays are kept until the next
// call.
//
// untaken is the adornments of the next snapshot when the drawing side has not taken it yet (render
// thread mode, fsapp-mainthread.h), else NULL (always in one-thread mode). What it hands over only
// once is then carried over into this one: its destroyed adornments are added to those destroyed
// since, its change to this one's, and its new content is kept for an adornment that has none
// newer, or else replaced by the newer content with the damaged rows of both combined (as commits
// between two snapshots are combined, fsapp_adornment_commit).
void fsapp_adornment_write_snapshot(struct fsapp_snapshot_adornments *adornments,
                                    const struct fsapp_snapshot_adornments *untaken);

// After the drawing side's UI upload of the snapshot's adornments: marks their new content taken,
// and frees the destroyed adornments (drawing side).
void fsapp_adornment_on_uploaded(const struct fsapp_snapshot_adornments *adornments);

// When the main loop has ended (host window side): frees the adornments that are left, alive or
// destroyed and not yet handed over, since no frame will hand them over any more. untaken is the
// adornments of a next snapshot the drawing side never took (render thread mode), whose destroyed
// adornments are freed too, else NULL.
void fsapp_adornment_quit(const struct fsapp_snapshot_adornments *untaken);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSAPP_ADORNMENT_H */
