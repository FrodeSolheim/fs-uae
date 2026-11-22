/*
 * Stacking: the stacking order of the surfaces, back to front, from their trees, roles, bands and
 * raise order. The compositor's flat list and routing's hit test both follow it.
 *
 * A pure unit with no SDL, clock, thread or other module, like routing (fsapp-routing.h), so that
 * its tests (od-fs/tests/fsapp) compile its source file alone.
 *
 * The rules:
 *
 * - A surface tree is a top-level and its descendants: its children and popups, and theirs. An
 *   overlay has a tree of its own in the same way.
 * - Top-levels stack by band, from back to front: background, normal, panel. Overlays are above
 *   every band.
 * - Within a band, top-levels stack by raise order (raised later is in front), and those with the
 *   same raise order (never raised: 0) by creation order (created later is in front). Overlays
 *   stack among themselves, and siblings among themselves, by the same rule.
 * - A tree stacks together: its top-level, then its children depth first (each above its parent),
 *   then its popups above the rest of the tree, each with its own children and popups above it.
 *   A popup belongs to the tree of the surface it is a popup of.
 * - A child or popup without a parent, whose parent is not in the list (destroyed), or whose
 *   parents never lead to a top-level or an overlay, is left out, and so are its children and
 *   popups. A top-level's or an overlay's parent does not count, nor does the band of a surface
 *   that is not a top-level.
 * - A surface is shown when it and all its ancestors (parents up to its top-level or overlay) are
 *   visible: a hidden surface hides its children and popups, and theirs.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#ifndef FSAPP_STACKING_H
#define FSAPP_STACKING_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// A surface's role.
typedef enum fsapp_stacking_role {
    FSAPP_STACKING_ROLE_TOP_LEVEL,
    FSAPP_STACKING_ROLE_CHILD,
    FSAPP_STACKING_ROLE_POPUP,
    FSAPP_STACKING_ROLE_OVERLAY,
} fsapp_stacking_role_t;

// A top-level's band, from back to front.
typedef enum fsapp_stacking_band {
    FSAPP_STACKING_BAND_BACKGROUND,
    FSAPP_STACKING_BAND_NORMAL,
    FSAPP_STACKING_BAND_PANEL,
} fsapp_stacking_band_t;

typedef struct fsapp_stacking_surface {
    // Identifies the surface (never NULL); stacking never dereferences it.
    const void *id;
    // The parent of a child or popup, else NULL.
    const void *parent;
    fsapp_stacking_role_t role;
    // Only a top-level's band counts.
    fsapp_stacking_band_t band;
    // The raise order: 0 when never raised, else higher for a surface raised later.
    uint64_t raise;
    // Its own visible flag.
    bool visible;
} fsapp_stacking_surface_t;

// A surface in the stacking order.
typedef struct fsapp_stacking_entry {
    // The surface's index in the list given.
    int index;
    // Whether it and all its ancestors are visible.
    bool shown;
} fsapp_stacking_entry_t;

// Computes the stacking order of the surfaces (listed in the order they were created) and writes
// it to order (room for count), from back to front. Returns how many it wrote: the surfaces left
// out are not among them.
int fsapp_stacking_compute_order(const fsapp_stacking_surface_t *surfaces, int count,
                                 fsapp_stacking_entry_t *order);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSAPP_STACKING_H */
