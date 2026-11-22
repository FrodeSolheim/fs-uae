/*
 * The compositor's GL drawing: small GLSL programs and one static vertex buffer with a unit quad,
 * with which every rectangle is drawn.
 *
 * The programs are GLSL 1.20 (#version 120), in the subset that GL 2.1 and GLES share. One vertex
 * shader places the unit quad: the rectangle is a uniform, and the depth is set with the depth
 * range (glDepthRange(depth, depth), the quad at z = 0). There are no vertex array objects (GL 2.1
 * has none): fsapp_gldraw_begin_program sets the attribute pointer for each step. The programs and
 * the buffer are fsapp's own, for the main loop's drawing side only.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#ifndef FSAPP_GLDRAW_H
#define FSAPP_GLDRAW_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum fsapp_gldraw_program {
    // Draws a texture into a rectangle and writes alpha 1, whatever the content's alpha.
    FSAPP_GLDRAW_PROGRAM_TEXTURE_OPAQUE,
    // Draws a texture into a rectangle with the content's alpha, for the blending that the step
    // sets (premultiplied).
    FSAPP_GLDRAW_PROGRAM_TEXTURE_TRANSLUCENT,
    // Fills a rectangle with one colour (fsapp_gldraw_fill_rectangle).
    FSAPP_GLDRAW_PROGRAM_FILL,
    // Draws part of streamed content's texture into its picture rectangle and writes alpha 1
    // (fsapp_gldraw_draw_texture_part).
    FSAPP_GLDRAW_PROGRAM_STREAMED,
    FSAPP_GLDRAW_PROGRAM_COUNT
} fsapp_gldraw_program_t;

// Make the programs and the vertex buffer, once, on the drawing side when its first frame starts,
// after fsapp_gl_load_functions. Returns NULL when they were made; otherwise the message to stop
// with (static), after logging the shader's or the program's info log.
const char *fsapp_gldraw_init(void);

// Begin drawing with the program: use it, bind the vertex buffer, set and enable the attribute
// pointer, and make texture unit 0 the active one. Rectangles are given in pixels of a drawable of
// pixel_width x pixel_height (the viewport is the step's to set). Only the program's
// state (and the depth range, for each rectangle) is set: blending, the depth test and the masks
// are the step's.
void fsapp_gldraw_begin_program(fsapp_gldraw_program_t program, int pixel_width, int pixel_height);

// Draw the texture into the rectangle (pixels, y down), with a texture program begun.
// The texture's first row is at the top of the rectangle. depth is the window depth (0 to 1), which
// every fragment gets exactly: it sets the depth range to it (glDepthRange(depth, depth)), so a
// depth that is exactly representable in the depth buffer is stored as it is. The texture's filter
// and wrap modes are the caller's: this binds it to texture unit 0 and draws.
void fsapp_gldraw_draw_texture(GLuint texture, const SDL_FRect *rect, double depth);

// Draw part of the texture (in texture coordinates: s and t of its top left corner, then its width
// and height; t grows downwards, from the texture's first row) into the rectangle, as
// fsapp_gldraw_draw_texture does, with the streamed-content program begun. Every fragment samples
// the texture at its coordinates clamped to the clamp rectangle (texture coordinates, as part), so
// that nothing outside it is sampled, also by linear filtering when the clamp rectangle is inset by
// half a texel.
void fsapp_gldraw_draw_texture_part(GLuint texture, const SDL_FRect *rect, double depth,
                                    const SDL_FRect *part, const SDL_FRect *clamp);

// Fill the rectangle (as for fsapp_gldraw_draw_texture) with the colour (components 0 to 1, written
// as given: premultiplied for a translucent fill), with the fill program begun.
void fsapp_gldraw_fill_rectangle(const SDL_FRect *rect, double depth, float r, float g, float b,
                                 float a);

// End drawing with the program begun: set the depth range back to 0 to 1, unuse the program,
// disable the attribute array and unbind the vertex buffer.
void fsapp_gldraw_end_program(void);

// Debug builds (SDL_ASSERT_LEVEL 2 or more): log the GL errors pending, with the name of the step
// after which they were found. Does nothing in other builds.
void fsapp_gldraw_check_errors(const char *step);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSAPP_GLDRAW_H */
