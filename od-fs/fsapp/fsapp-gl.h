/*
 * fsapp's OpenGL: the context it asks for, what it logs about the context it got, and the loader of
 * the GL functions it uses beyond GL 1.1.
 *
 * The table of functions is fsapp's own: no other module uses it (the graphics calls are to stay in
 * fsapp). GL 1.1 functions are called directly.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#ifndef FSAPP_GL_H
#define FSAPP_GL_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Ask SDL for the context that fsapp draws in: OpenGL 2.1 with no profile (a compatibility
// context), double buffered, with a 16-bit depth buffer. The alpha size is left at SDL's default
// of 8 bits. Call on the main thread before the host window is created.
void fsapp_gl_set_context_attributes(void);

// Log the OpenGL information of the current context: vendor, renderer, version, shading language
// version, extensions, the maximum texture size, and the depth and alpha sizes obtained. Call with
// the context current (the main loop's drawing side).
void fsapp_gl_log_information(void);

// Check that the current context's OpenGL version (the major.minor at the start of GL_VERSION) is
// at least 2.1, the version fsapp draws with. Returns NULL when it is, otherwise the message to
// stop with, which the caller frees with SDL_free. Call with the context current (the main loop's
// drawing side), before fsapp_gl_load_functions: on some platforms (Mesa) every function is found
// whatever the version, so the version is what tells a context that is too old.
char *fsapp_gl_check_version(void);

// The GL functions beyond GL 1.1 that fsapp uses, with their types from SDL's GL extension header
// (SDL_opengl_glext.h), as X(type, name), the name without its gl prefix. To use another one, add
// it here (in alphabetical order); it is then loaded with the others and called as
// fsapp_gl.Name(...).
#define FSAPP_GL_FUNCTIONS(X)                                                                      \
    X(PFNGLACTIVETEXTUREPROC, ActiveTexture)                                                       \
    X(PFNGLATTACHSHADERPROC, AttachShader)                                                         \
    X(PFNGLBINDATTRIBLOCATIONPROC, BindAttribLocation)                                             \
    X(PFNGLBINDBUFFERPROC, BindBuffer)                                                             \
    X(PFNGLBLENDFUNCSEPARATEPROC, BlendFuncSeparate)                                               \
    X(PFNGLBUFFERDATAPROC, BufferData)                                                             \
    X(PFNGLCOMPILESHADERPROC, CompileShader)                                                       \
    X(PFNGLCREATEPROGRAMPROC, CreateProgram)                                                       \
    X(PFNGLCREATESHADERPROC, CreateShader)                                                         \
    X(PFNGLDELETEBUFFERSPROC, DeleteBuffers)                                                       \
    X(PFNGLDELETEPROGRAMPROC, DeleteProgram)                                                       \
    X(PFNGLDELETESHADERPROC, DeleteShader)                                                         \
    X(PFNGLDISABLEVERTEXATTRIBARRAYPROC, DisableVertexAttribArray)                                 \
    X(PFNGLENABLEVERTEXATTRIBARRAYPROC, EnableVertexAttribArray)                                   \
    X(PFNGLGENBUFFERSPROC, GenBuffers)                                                             \
    X(PFNGLGETPROGRAMINFOLOGPROC, GetProgramInfoLog)                                               \
    X(PFNGLGETPROGRAMIVPROC, GetProgramiv)                                                         \
    X(PFNGLGETSHADERINFOLOGPROC, GetShaderInfoLog)                                                 \
    X(PFNGLGETSHADERIVPROC, GetShaderiv)                                                           \
    X(PFNGLGETUNIFORMLOCATIONPROC, GetUniformLocation)                                             \
    X(PFNGLLINKPROGRAMPROC, LinkProgram)                                                           \
    X(PFNGLSHADERSOURCEPROC, ShaderSource)                                                         \
    X(PFNGLUNIFORM1IPROC, Uniform1i)                                                               \
    X(PFNGLUNIFORM4FPROC, Uniform4f)                                                               \
    X(PFNGLUSEPROGRAMPROC, UseProgram)                                                             \
    X(PFNGLVERTEXATTRIBPOINTERPROC, VertexAttribPointer)

typedef struct fsapp_gl_functions {
#define FSAPP_GL_DECLARE_FUNCTION(type, name) type name;
    FSAPP_GL_FUNCTIONS(FSAPP_GL_DECLARE_FUNCTION)
#undef FSAPP_GL_DECLARE_FUNCTION
} fsapp_gl_functions_t;

// The loaded functions (NULL until fsapp_gl_load_functions has loaded them). Only for the main
// loop's drawing side, which loads them when its first frame starts.
extern fsapp_gl_functions_t fsapp_gl;

// Load the functions in the table with SDL_GL_GetProcAddress (on every platform), with the context
// current. Returns NULL when all were found, otherwise the name of the first that is missing (with
// its gl prefix), and the table is left as it was.
const char *fsapp_gl_load_functions(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSAPP_GL_H */
