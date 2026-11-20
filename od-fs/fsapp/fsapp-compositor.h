/*
 * The compositor: draws the surfaces, the emulation surface and the adornments into the host window
 * with OpenGL, from the frame plan (fsapp-frame-plan.h, which has the rules of the order and the
 * steps). All of it belongs to the main loop's drawing side (see fsapp-mainthread.h), and reads the
 * host window's state only from the frame's snapshot.
 *
 * Each frame, in the main loop's phases:
 *
 * - The UI upload: the window is cleared, its colour to the background colour with alpha 1
 *   (whatever the background's alpha: the window is always opaque) and its depth to the far value
 *   (1.0); then, when the frame takes in a UI frame's record (fsapp-surface.h), the textures of its
 *   destroyed surfaces are deleted and its content is uploaded into each committed surface's one
 *   texture, straight from the client's buffer: the whole buffer with glTexImage2D when the
 *   texture is new or its size changes, else each damaged rectangle with glTexSubImage2D (the
 *   buffer's width as GL_UNPACK_ROW_LENGTH, the rectangle's first pixel as the source, nothing
 *   repacked, the unpack state set in full before each upload); then the
 *   adornments' content. The buffers are in their native format (premultiplied BGRA, as GL_BGRA
 *   with GL_UNSIGNED_INT_8_8_8_8_REV), the adornments' content RGBA bytes (GL_RGBA with
 *   GL_UNSIGNED_BYTE), both into textures with the unsized internal format GL_RGBA. One texture
 *   per surface is enough while the glFinish() before the swap stays: the GPU has finished reading
 *   every texture before the next frame's UI upload.
 * - Drawing below: the frame plan is worked out from the snapshot (the flat list, leaving out a
 *   surface whose buffer has never been uploaded; the adornments with content; the pacer), then
 *   - step 1, the opaque entries, front to back: blending off, the depth test on with GL_LESS,
 *     depth writes on; the opaque texture program (alpha 1, whatever the content's alpha) for
 *     surfaces and adornments, and the fill program for the emulation surface's fill (its whole
 *     rectangle in its fill colour, alpha 1; with streamed content that has content, only the bars
 *     around its picture, and the depth-only entry over the picture: its depth with colour writes
 *     off);
 *   - step 2, the translucent entries behind the emulation surface (all of them when there is
 *     none), back to front: premultiplied blending with
 *     glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE), so the window's alpha
 *     stays 1; the depth test on with GL_LESS and depth writes off, so the opaque entries in front
 *     hide them and they hide nothing; the translucent texture program.
 *   Nothing is flushed between drawing below and the wait.
 * - The wait: the pacer's wait hook; the compositor draws nothing. The pacer may upload and commit
 *   streamed content in it (fsapp-streamed.h), which sets the GL state it needs itself.
 * - Drawing the paced surface: step 4, the emulation surface. With streamed content, its front
 *   texture (after a commit in the wait, the new content), drawn into the picture rectangle clipped
 *   to the surface with the streamed-content program at the surface's depth: the depth test on
 *   with GL_LEQUAL, depth writes, blending and alpha writes off; nothing without content. When
 *   step 1 left a picture unfilled (its depth-only entry) that is not the one drawn now (a commit
 *   in the wait changed the picture rectangle, or there is no content), the surface's whole
 *   rectangle is first filled again with the fill program, in the same state, so that no old
 *   picture is left in the new bars; only in such frames.
 * - Drawing above: step 5, the translucent entries in front of the emulation surface, back to
 *   front, drawn as in step 2.
 *
 * Depth: every entry is drawn at the window depth place / 65535 of its place in the plan (0 is the
 * frontmost, the nearest), which is exactly representable in the 16-bit depth buffer and below the
 * far value. Each rectangle is placed there with glDepthRange(depth, depth), so the fill and the
 * streamed content store the same value and step 4's GL_LEQUAL passes.
 *
 * Every step sets the GL state it relies on and assumes nothing from an earlier step or the UI
 * upload; the unpack state is set for each upload. No GL code but fsapp's runs in the frame, and
 * fsapp never turns on the scissor test, face culling or the alpha and stencil tests (GL starts
 * with them off). Debug builds check for GL errors after each step (fsapp_gldraw_check_errors).
 * fsapp draws with GLSL programs and a vertex buffer (fsapp-gldraw.h), with no immediate mode and
 * no glPushAttrib.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#ifndef FSAPP_COMPOSITOR_H
#define FSAPP_COMPOSITOR_H

#include <SDL3/SDL.h>

#include "fsapp-config.h"
#include "fsapp-snapshot.h"

#ifdef __cplusplus
extern "C" {
#endif

// Clear the host window (its pixels, with the background colour with alpha 1, and the depth) and
// set the viewport to it.
void fsapp_compositor_clear_window(const fsapp_snapshot_t *snapshot);

// The UI upload: delete the textures of the destroyed surfaces of the record the frame takes in (if
// any), then upload its content to the surfaces' textures. The record must not be marked taken in
// (fsapp_surface_on_uploaded) until this has returned.
void fsapp_compositor_update_textures(const fsapp_snapshot_t *snapshot);

// The adornments' part of the UI upload: delete the textures of the snapshot's destroyed
// adornments, then upload the content committed since the previous snapshot to textures: only its
// damaged rows (glTexSubImage2D), or the whole content into a texture that is new or whose content
// size changes.
void fsapp_compositor_update_adornment_textures(const fsapp_snapshot_t *snapshot);

// Drawing below: work out the frame's plan from the snapshot (after the UI upload, so that the
// textures exist), and draw its steps 1 and 2. The plan is kept for the frame's later steps.
void fsapp_compositor_draw_below(const fsapp_snapshot_t *snapshot);

// Drawing the paced surface: step 4 of the frame's plan, the emulation surface (the surface the
// pacer names while one is set, else the frontmost surface with streamed content, so its old
// content is drawn again), drawn from its streamed content at its depth, when it is in the plan.
void fsapp_compositor_draw_paced_surface(const fsapp_snapshot_t *snapshot);

// Drawing above: step 5 of the frame's plan, the translucent entries in front of the emulation
// surface.
void fsapp_compositor_draw_above(const fsapp_snapshot_t *snapshot);

#ifdef __cplusplus
}
#endif

#endif /* FSAPP_COMPOSITOR_H */
