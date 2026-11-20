/*
 * Adornments: rectangles of content that fsapp draws in the host window, taking no input.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#include "fsapp-adornment.h"

#include <vector>

#include "fsapp-frame.h"
#include "fsapp-mainthread.h"
#include "fsapp-snapshot.h"
#include "fsapp-time.h"
#include "fs/trace.h"

struct fsapp_adornment {
    // All fields are behind g_adornments_lock.
    fsapp_adornment_place_t place;
    // The surface it is attached in front of, for FSAPP_ADORNMENT_IN_FRONT_OF_SURFACE.
    const void *surface;
    SDL_Rect rect;
    bool opaque;
    fsapp_adornment_filter_t filter;
    // Destroyed by its owner: left out of the next snapshot, and handed to the drawing side as
    // destroyed with it.
    bool destroyed;
    // Content committed since the previous snapshot, else NULL, and the damaged rows of all the
    // commits since then combined into one span (pending_damage_height 0: none).
    const void *pending;
    int pending_width;
    int pending_height;
    int pending_stride;
    int pending_damage_y;
    int pending_damage_height;
    // The commits counted, and the last one taken in (uploaded).
    uint64_t committed;
    uint64_t taken;
};

// The adornments, guarded by the lock: created and not yet handed over as destroyed, in the order
// they were created, and whether one changed since the previous snapshot. The calls come from any
// thread and hold the lock only briefly.
static SDL_SpinLock g_adornments_lock;
static std::vector<fsapp_adornment_t *> g_adornments;
static bool g_adornments_changed;

// The storage of the snapshot's arrays (host window side).
static std::vector<fsapp_snapshot_adornment_t> g_snapshot_adornments;
static std::vector<fsapp_adornment_t *> g_snapshot_destroyed;

// Releases the lock taken by a call. When the call changed something, the change is marked, so the
// next snapshot reports it and a frame is needed, and the main loop is woken (with the lock
// released) to take it in.
static void unlock_after(bool changed)
{
    if (changed) {
        g_adornments_changed = true;
    }
    SDL_UnlockSpinlock(&g_adornments_lock);
    if (changed) {
        fsapp_frame_wake();
    }
}

static fsapp_adornment_t *create(fsapp_adornment_place_t place, const void *surface)
{
    // A plain struct: SDL_calloc zeroes it, and fsapp_adornment_on_uploaded frees it with SDL_free.
    fsapp_adornment_t *adornment = (fsapp_adornment_t *)SDL_calloc(1, sizeof(fsapp_adornment_t));
    adornment->place = place;
    adornment->surface = surface;
    adornment->opaque = true;
    adornment->filter = FSAPP_ADORNMENT_FILTER_LINEAR;
    SDL_LockSpinlock(&g_adornments_lock);
    g_adornments.push_back(adornment);
    unlock_after(true);
    return adornment;
}

fsapp_adornment_t *fsapp_adornment_create_in_front(void)
{
    return create(FSAPP_ADORNMENT_IN_FRONT_OF_EVERYTHING, NULL);
}

fsapp_adornment_t *fsapp_adornment_create_in_front_of(struct fsapp_surface *surface)
{
    SDL_assert(surface);
    // The layout identifies a surface by its address (fsapp_snapshot_surface_t.id).
    return create(FSAPP_ADORNMENT_IN_FRONT_OF_SURFACE, surface);
}

void fsapp_adornment_destroy(fsapp_adornment_t *adornment)
{
    if (adornment == NULL) {
        return;
    }
    SDL_LockSpinlock(&g_adornments_lock);
    SDL_assert(!adornment->destroyed);
    adornment->destroyed = true;
    adornment->pending = NULL;
    unlock_after(true);
}

void fsapp_adornment_set_rect(fsapp_adornment_t *adornment, int x, int y, int width, int height)
{
    SDL_LockSpinlock(&g_adornments_lock);
    SDL_Rect rect = {x, y, width, height};
    bool changed = SDL_memcmp(&rect, &adornment->rect, sizeof(rect)) != 0;
    if (changed) {
        adornment->rect = rect;
    }
    unlock_after(changed);
}

void fsapp_adornment_set_opaque(fsapp_adornment_t *adornment, bool opaque)
{
    SDL_LockSpinlock(&g_adornments_lock);
    bool changed = adornment->opaque != opaque;
    if (changed) {
        adornment->opaque = opaque;
    }
    unlock_after(changed);
}

void fsapp_adornment_set_filter(fsapp_adornment_t *adornment, fsapp_adornment_filter_t filter)
{
    SDL_LockSpinlock(&g_adornments_lock);
    bool changed = adornment->filter != filter;
    if (changed) {
        adornment->filter = filter;
    }
    unlock_after(changed);
}

// Combines the damaged rows of older content, not yet taken in, into those of newer content
// (width x height) that replaces it: when the older had another size, its damaged rows do not say
// what changed in the newer, so all of it counts as damaged; else the damaged rows of both are
// combined into one span, from the first damaged row to the last.
static void combine_damage(int older_width, int older_height, int older_damage_y,
                           int older_damage_height, int width, int height, int *damage_y,
                           int *damage_height)
{
    if (older_width != width || older_height != height) {
        *damage_y = 0;
        *damage_height = height;
    } else if (older_damage_height > 0) {
        if (*damage_height == 0) {
            *damage_y = older_damage_y;
            *damage_height = older_damage_height;
        } else {
            int top = SDL_min(*damage_y, older_damage_y);
            int bottom = SDL_max(*damage_y + *damage_height, older_damage_y + older_damage_height);
            *damage_y = top;
            *damage_height = bottom - top;
        }
    }
}

void fsapp_adornment_commit(fsapp_adornment_t *adornment, const void *pixels, int width, int height,
                            int stride, int damage_y, int damage_height)
{
    SDL_assert(pixels && width > 0 && height > 0);
    SDL_assert(stride >= width * 4 && stride % 4 == 0);
    SDL_assert(damage_y >= 0 && damage_height >= 0 && damage_y + damage_height <= height);
    SDL_LockSpinlock(&g_adornments_lock);
    if (adornment->pending) {
        // Several commits before a snapshot takes them: the newest content, with the damaged rows
        // of all of them.
        combine_damage(adornment->pending_width, adornment->pending_height,
                       adornment->pending_damage_y, adornment->pending_damage_height, width, height,
                       &damage_y, &damage_height);
    }
    adornment->pending = pixels;
    adornment->pending_width = width;
    adornment->pending_height = height;
    adornment->pending_stride = stride;
    adornment->pending_damage_y = damage_y;
    adornment->pending_damage_height = damage_height;
    adornment->committed++;
    unlock_after(true);
}

bool fsapp_adornment_is_content_taken(fsapp_adornment_t *adornment)
{
    SDL_LockSpinlock(&g_adornments_lock);
    bool taken = adornment->taken == adornment->committed;
    SDL_UnlockSpinlock(&g_adornments_lock);
    return taken;
}

// ----------------------------------------------------------------------------

bool fsapp_adornment_has_changed(void)
{
    SDL_LockSpinlock(&g_adornments_lock);
    bool changed = g_adornments_changed;
    SDL_UnlockSpinlock(&g_adornments_lock);
    return changed;
}

// The new content of the adornment in an untaken snapshot's adornments (see
// fsapp_adornment_write_snapshot), or NULL when it has none there.
static const fsapp_snapshot_adornment_t *
find_untaken_content(const fsapp_snapshot_adornments_t *untaken, const fsapp_adornment_t *adornment)
{
    for (int i = 0; untaken != NULL && i < untaken->count; i++) {
        const fsapp_snapshot_adornment_t *entry = &untaken->adornments[i];
        if (entry->id == adornment) {
            return entry->content ? entry : NULL;
        }
    }
    return NULL;
}

// Carries the new content of an adornment in an untaken snapshot over into its entry in the one
// being written (copy): kept when the entry has none newer, else combined with it (combine_damage).
static void carry_over_content(fsapp_snapshot_adornment_t *copy,
                               const fsapp_snapshot_adornment_t *older)
{
    if (copy->content == NULL) {
        copy->content = older->content;
        copy->content_width = older->content_width;
        copy->content_height = older->content_height;
        copy->content_stride = older->content_stride;
        copy->content_damage_y = older->content_damage_y;
        copy->content_damage_height = older->content_damage_height;
        copy->content_commit = older->content_commit;
        return;
    }
    combine_damage(older->content_width, older->content_height, older->content_damage_y,
                   older->content_damage_height, copy->content_width, copy->content_height,
                   &copy->content_damage_y, &copy->content_damage_height);
}

void fsapp_adornment_write_snapshot(fsapp_snapshot_adornments_t *adornments,
                                    const fsapp_snapshot_adornments_t *untaken)
{
    FSAPP_MAINTHREAD_ASSERT_HOST_WINDOW_SIDE();
    g_snapshot_adornments.clear();
    g_snapshot_destroyed.clear();
    // What an untaken snapshot hands over only once is carried over into this one (render thread
    // mode): its destroyed adornments first (each is handed over, and freed, once), and below its
    // new content and its change. This module owns these rules, as it owns combining commits.
    if (untaken != NULL) {
        g_snapshot_destroyed.assign(untaken->destroyed,
                                    untaken->destroyed + untaken->destroyed_count);
    }
    SDL_LockSpinlock(&g_adornments_lock);
    size_t kept = 0;
    for (fsapp_adornment_t *adornment : g_adornments) {
        if (adornment->destroyed) {
            // The drawing side deletes its texture and frees it (fsapp_adornment_on_uploaded).
            g_snapshot_destroyed.push_back(adornment);
            continue;
        }
        g_adornments[kept++] = adornment;
        fsapp_snapshot_adornment_t copy = {};
        copy.id = adornment;
        copy.place = adornment->place;
        copy.surface = adornment->surface;
        copy.x = adornment->rect.x;
        copy.y = adornment->rect.y;
        copy.width = adornment->rect.w;
        copy.height = adornment->rect.h;
        copy.opaque = adornment->opaque;
        copy.filter = adornment->filter;
        if (adornment->pending) {
            copy.content = adornment->pending;
            copy.content_width = adornment->pending_width;
            copy.content_height = adornment->pending_height;
            copy.content_stride = adornment->pending_stride;
            copy.content_damage_y = adornment->pending_damage_y;
            copy.content_damage_height = adornment->pending_damage_height;
            copy.content_commit = adornment->committed;
            adornment->pending = NULL;
        }
        const fsapp_snapshot_adornment_t *older = find_untaken_content(untaken, adornment);
        if (older != NULL) {
            carry_over_content(&copy, older);
        }
        g_snapshot_adornments.push_back(copy);
    }
    g_adornments.resize(kept);
    adornments->changed = g_adornments_changed || (untaken != NULL && untaken->changed);
    g_adornments_changed = false;
    SDL_UnlockSpinlock(&g_adornments_lock);

    adornments->adornments = g_snapshot_adornments.data();
    adornments->count = (int)g_snapshot_adornments.size();
    adornments->destroyed = g_snapshot_destroyed.data();
    adornments->destroyed_count = (int)g_snapshot_destroyed.size();
}

void fsapp_adornment_on_uploaded(const fsapp_snapshot_adornments_t *adornments)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    int64_t now_ns = FS_IsTraceEnabled() ? (int64_t)SDL_GetTicksNS() : 0;
    SDL_LockSpinlock(&g_adornments_lock);
    // An adornment destroyed since this snapshot is still alive (it is freed only after a later
    // snapshot has handed it over as destroyed), so its content can still be marked taken. The
    // commit is the snapshot's, not the adornment's latest: with a render thread, a newer commit
    // may already be in the next snapshot, not taken in yet.
    for (int i = 0; i < adornments->count; i++) {
        const fsapp_snapshot_adornment_t *adornment = &adornments->adornments[i];
        if (adornment->content && adornment->content_commit > adornment->id->taken) {
            adornment->id->taken = adornment->content_commit;
        }
        if (adornment->content && now_ns != 0) {
            FS_TraceValue values[] = {{"commit", (int64_t)adornment->content_commit},
                                      {"rows", adornment->content_damage_height}};
            FS_AddTraceInstantWithValues("adornment content taken in", now_ns, FS_TRACE_NO_FRAME,
                                         values, 2);
        }
    }
    SDL_UnlockSpinlock(&g_adornments_lock);
    // The destroyed adornments are handed over once, with the snapshot of the frame that follows
    // (carried over by fsapp_adornment_write_snapshot while the drawing side has not taken it).
    for (int i = 0; i < adornments->destroyed_count; i++) {
        SDL_free(adornments->destroyed[i]);
    }
}

void fsapp_adornment_quit(const fsapp_snapshot_adornments_t *untaken)
{
    FSAPP_MAINTHREAD_ASSERT_HOST_WINDOW_SIDE();
    // REVIEW: The owners keep their pointers to the adornments freed here (fsemu's performance
    // graph does, until its next frame hook, which never comes); nothing uses them after the main
    // loop has ended today. An owner that outlives the loop would need to be told.
    // The ones handed over as destroyed with a snapshot that the drawing side took have been freed
    // by it (fsapp_adornment_on_uploaded); those in a snapshot it never took (render thread mode)
    // are freed here.
    int untaken_count = untaken != NULL ? untaken->destroyed_count : 0;
    for (int i = 0; i < untaken_count; i++) {
        SDL_free(untaken->destroyed[i]);
    }
    g_snapshot_adornments.clear();
    g_snapshot_destroyed.clear();
    SDL_LockSpinlock(&g_adornments_lock);
    int count = (int)g_adornments.size();
    for (fsapp_adornment_t *adornment : g_adornments) {
        SDL_free(adornment);
    }
    g_adornments.clear();
    g_adornments_changed = false;
    SDL_UnlockSpinlock(&g_adornments_lock);
    if (count + untaken_count > 0) {
        SDL_Log("Freed %d adornment(s) left when the main loop ended", count + untaken_count);
    }
}
