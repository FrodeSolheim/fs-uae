/*
 * The frame plan: what the compositor draws in each step of a frame, in which order and at which
 * depth, worked out from the flat list of surfaces and the adornments.
 *
 * A pure function with no SDL, GL or other module, so that its tests (od-fs/tests/fsapp) can
 * compile its source file alone. The drawing side builds the input from the snapshot each frame,
 * leaving out what it has nothing to draw for (a surface whose content has never been uploaded, an
 * adornment with no content yet), and draws the output in the phases of the main loop.
 *
 * The rules:
 *
 * - The emulation surface is the surface the pacer names while a pacer is set, else the frontmost
 *   surface with streamed content, as long as it is in the list and has streamed content; otherwise
 *   there is none. It is opaque whatever its translucent flag. Other surfaces with streamed content
 *   are left out.
 * - Its picture rectangle is clipped to its rectangle (fsapp_frame_plan_clip_picture, which the
 *   compositor also uses for a picture that came with the commit after the wait). The bars are its
 *   rectangle minus the clipped picture: a band above, a band below, and the parts left and right
 *   of the picture between them.
 * - Adornments are inserted into the list: one attached to a surface directly in front of that
 *   surface and behind every surface in front of it, one in front of everything in front of every
 *   surface. Several on the same place keep the order they were created in, later ones in front.
 *   One attached to a surface that is not in the list (or is left out) is left out.
 * - Entries with an empty rectangle (a width or height of 0 or less) are left out.
 * - Every entry left gets a place of its own, counted from the front: the frontmost has place 0,
 *   the one behind it place 1 and so on. The drawing side turns the place into a depth value that
 *   is exactly representable in the 16-bit depth buffer, the frontmost the nearest. A list longer
 *   than FSAPP_FRAME_PLAN_PLACE_COUNT drops the entries at the back (steps.dropped_count).
 *
 * The steps (the order in which the main loop draws a frame, see fsapp-compositor.h; step 3 is the
 * wait for the emulated frame, which has no entries):
 *
 * 1. The opaque entries, front to back. At the emulation surface's place, all at its place: with
 *    content and a clipped picture that is not empty, fill entries over the bars (each left out
 *    when empty) and a depth-only entry over the clipped picture, so that nothing behind it is
 *    drawn where the picture will be; otherwise (no content, or an empty clipped picture) one fill
 *    entry over its whole rectangle. Drawn before the wait, with depth writes, so that the depth
 *    test skips what lies under them.
 * 2. The translucent entries behind the emulation surface, back to front, blended. Drawn before
 *    the wait. With no emulation surface, every translucent entry is here.
 * 4. The emulation surface (its whole rectangle, at its place), after the wait; or none. Drawn from
 *    its streamed content into its clipped picture.
 * 5. The translucent entries in front of the emulation surface, back to front, blended over the
 *    current emulated frame after the wait. Empty when there is no emulation surface, so a front
 *    end alone draws nothing after a wait.
 *
 * A plan keeps its storage from frame to frame and allocates nothing once it has seen a frame as
 * large as the current one.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#ifndef FSAPP_FRAME_PLAN_H
#define FSAPP_FRAME_PLAN_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// How many places there are: the values of a 16-bit depth buffer below the far value (65535),
// which the depth buffer is cleared to.
#define FSAPP_FRAME_PLAN_PLACE_COUNT 65535

typedef struct fsapp_frame_plan fsapp_frame_plan_t;

// A rectangle in pixels. Empty when its width or height is 0 or less.
typedef struct fsapp_frame_plan_rectangle {
    int x;
    int y;
    int width;
    int height;
} fsapp_frame_plan_rectangle_t;

// A source rectangle: the part of streamed content shown, in texels of its upload region (texel
// (0, 0) is its top left corner). Fractional once clipped.
typedef struct fsapp_frame_plan_source {
    double x;
    double y;
    double width;
    double height;
} fsapp_frame_plan_source_t;

// A surface in the flat list.
typedef struct fsapp_frame_plan_surface {
    // Identifies the surface (never NULL); the plan never dereferences it.
    const void *id;
    fsapp_frame_plan_rectangle_t rectangle;
    // Blended with its content's alpha (premultiplied), else opaque.
    bool translucent;
    // Has streamed content (the emulation surface's) rather than a buffer.
    bool streamed;
    // For a surface with streamed content: whether it has content (a commit since the content was
    // attached or dropped), and the picture rectangle it is drawn into, in pixels. Only
    // the emulation surface's are used.
    bool has_content;
    fsapp_frame_plan_rectangle_t picture;
} fsapp_frame_plan_surface_t;

// An adornment.
typedef struct fsapp_frame_plan_adornment {
    // Identifies the adornment; the plan never dereferences it.
    const void *id;
    // The id of the surface it is attached in front of, or NULL for in front of everything.
    const void *surface;
    fsapp_frame_plan_rectangle_t rectangle;
    bool translucent;
} fsapp_frame_plan_adornment_t;

typedef struct fsapp_frame_plan_input {
    // The flat list, from back to front.
    const fsapp_frame_plan_surface_t *surfaces;
    int surface_count;
    // The adornments, in the order they were created.
    const fsapp_frame_plan_adornment_t *adornments;
    int adornment_count;
    // Whether a pacer is set, and the id of the surface it names (the paced surface; NULL for
    // none).
    bool pacer_set;
    const void *paced_surface;
} fsapp_frame_plan_input_t;

typedef enum fsapp_frame_plan_entry_kind {
    // A surface: from its buffer in steps 1, 2 and 5; the emulation surface from its streamed
    // content in step 4.
    FSAPP_FRAME_PLAN_SURFACE,
    // An adornment, from its content.
    FSAPP_FRAME_PLAN_ADORNMENT,
    // The emulation surface's fill: its rectangle, or one of its bars, in its fill colour (step 1
    // only).
    FSAPP_FRAME_PLAN_FILL,
    // The emulation surface's depth over its clipped picture, with colour writes off (step 1 only).
    FSAPP_FRAME_PLAN_DEPTH_ONLY,
} fsapp_frame_plan_entry_kind_t;

// Something to draw.
typedef struct fsapp_frame_plan_entry {
    fsapp_frame_plan_entry_kind_t kind;
    // The index in the input's surfaces (for a surface or a fill) or adornments, and its id.
    int index;
    const void *id;
    fsapp_frame_plan_rectangle_t rectangle;
    // The place, counted from the front (0 is the frontmost), below FSAPP_FRAME_PLAN_PLACE_COUNT.
    int place;
} fsapp_frame_plan_entry_t;

// The output, one list per step. The arrays belong to the plan and stay valid until its next
// update or its destruction.
typedef struct fsapp_frame_plan_steps {
    // Step 1: the opaque entries, front to back.
    const fsapp_frame_plan_entry_t *opaque;
    int opaque_count;
    // Step 2: the translucent entries behind the emulation surface (or all of them when there is
    // none), back to front.
    const fsapp_frame_plan_entry_t *translucent_below;
    int translucent_below_count;
    // Step 4: the emulation surface, or NULL.
    const fsapp_frame_plan_entry_t *emulation;
    // Step 5: the translucent entries in front of the emulation surface, back to front.
    const fsapp_frame_plan_entry_t *translucent_above;
    int translucent_above_count;
    // How many entries at the back were dropped because the list was longer than
    // FSAPP_FRAME_PLAN_PLACE_COUNT (0 in practice). The caller logs it.
    int dropped_count;
} fsapp_frame_plan_steps_t;

// Creates a plan, with no storage until its first update.
fsapp_frame_plan_t *fsapp_frame_plan_create(void);

void fsapp_frame_plan_destroy(fsapp_frame_plan_t *plan);

// Works out the steps for a frame from the input, which is only read during the call, and writes
// them to steps.
void fsapp_frame_plan_update(fsapp_frame_plan_t *plan, const fsapp_frame_plan_input_t *input,
                             fsapp_frame_plan_steps_t *steps);

// Cuts a picture rectangle to the surface's rectangle (both in pixels), and the source
// rectangle shown in it in proportion (fractional source coordinates), with no scissor: the clipped
// picture shows exactly the part of the source that the unclipped picture shows there. Writes the
// clipped picture and, when source is not NULL, the clipped source. Returns false when the clipped
// picture is empty (an empty picture or surface, or no overlap): nothing of it is drawn, and the
// outputs are left as they are. The plan uses it for the bars before the wait, and the compositor
// for a picture that came with the commit after it, so that both use the same arithmetic.
bool fsapp_frame_plan_clip_picture(const fsapp_frame_plan_rectangle_t *surface,
                                   const fsapp_frame_plan_rectangle_t *picture,
                                   const fsapp_frame_plan_source_t *source,
                                   fsapp_frame_plan_rectangle_t *clipped_picture,
                                   fsapp_frame_plan_source_t *clipped_source);

// A size in pixels or texels.
typedef struct fsapp_frame_plan_size {
    int width;
    int height;
} fsapp_frame_plan_size_t;

// What a texture of streamed content is sized to: the largest upload region it has been given,
// rounded up to a multiple of 64 in each direction, grown only (each direction on its own), never
// above max_size (the GPU's GL_MAX_TEXTURE_SIZE). Not the frame plan's, but GPU-free arithmetic of
// the drawing side's, tested with it.
//
// texture is the texture's size so far (0 by 0 before the first region), and is updated. region is
// the upload region, and is cut to max_size in each direction (the caller compares it with what it
// passed to see whether it was cut, and logs that). Returns true when the texture's size changed,
// so that the texture must be created again.
bool fsapp_frame_plan_grow_texture(fsapp_frame_plan_size_t *texture,
                                   fsapp_frame_plan_size_t *region, int max_size);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSAPP_FRAME_PLAN_H */
