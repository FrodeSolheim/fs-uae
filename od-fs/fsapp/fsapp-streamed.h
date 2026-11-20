/*
 * Streamed content on the drawing side: the two textures of each surface with streamed content,
 * the uploads into them and the commit that switches them (the client's calls are in
 * fsapp-surface.h, "Streamed content"), and what the compositor reads to draw it.
 *
 * Everything here belongs to the main loop's drawing side. The state is kept by the surface's id
 * (fsapp_snapshot_surface_t.id), from the first call for the surface until the compositor deletes
 * it with the surface's other textures, when the surface is destroyed.
 *
 * GL in the pacer's wait keeps no state: each upload binds the back texture on texture unit 0 (the
 * one fsapp draws with) and sets the whole unpack state before glTexSubImage2D, and assumes nothing
 * kept from an earlier call or across fsapp_frame_wait. Nothing is restored after it: the
 * compositor's steps set the state they use.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#ifndef FSAPP_STREAMED_H
#define FSAPP_STREAMED_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <stdbool.h>

#include "fsapp-surface.h"

#ifdef __cplusplus
extern "C" {
#endif

// The content of a surface with streamed content, as the compositor draws it.
typedef struct fsapp_streamed_content {
    // Whether it has content (a commit), else nothing below is set.
    bool has_content;
    // The front texture, and its size in texels.
    GLuint texture;
    int texture_width;
    int texture_height;
    // The source rectangle (in the upload region, which is at texel (0, 0)) and the picture
    // rectangle (in the surface's coordinates) of the last commit, as the client gave them.
    SDL_Rect source;
    SDL_Rect picture;
    // The size of the last commit's upload region as uploaded: smaller than the client's region
    // when that was larger than the GPU's largest texture. A source rectangle reaching beyond it
    // shows only the part inside it (fsapp_surface_begin_streamed_frame).
    int region_width;
    int region_height;
    fsapp_surface_stream_filter_t filter;
} fsapp_streamed_content_t;

// The streamed content of the surface with that id as it is now (after a commit made in the wait,
// the new content). Drawing side.
void fsapp_streamed_get_content(const void *id, fsapp_streamed_content_t *content);

// Destroys the streamed content of the surface with that id (a destroyed surface), if it has any:
// deletes its textures and forgets its state. Drawing side.
void fsapp_streamed_destroy(const void *id);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSAPP_STREAMED_H */
