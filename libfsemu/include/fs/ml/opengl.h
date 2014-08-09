/* libfsgl - utility and wrapper functions for OpenGL
 * Copyright (C) 2011 Frode Solheim <frode-code@fengestad.no>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef LIBFSGL_OPENGL_H_
#define LIBFSGL_OPENGL_H_

#if defined(USE_GLES) && !defined(FAKE_GLES)
#include <GLES/gl.h>
#define GL_BGRA 0x80e1
#else
#include <GLee.h>
#endif

#ifndef APIENTRY
    #define APIENTRY
#endif
#ifndef APIENTRYP
    #define APIENTRYP APIENTRY *
#endif

void fs_gl_finish();
void fs_gl_texturing(int enable);
void fs_gl_bind_texture(int texture);
void fs_gl_unpack_row_length(int length);

void fs_gl_blending(int enable);
void fs_gl_color4fv(float *c);
void fs_gl_color4f(float r, float g, float b, float a);

void fs_gl_ortho();
void fs_gl_ortho_hd();
void fs_gl_perspective();
void fs_gl_viewport(int x, int y, int w, int h);

void fs_gl_reset_client_state();

#define FS_GL_CONTEXT_CREATE 1
#define FS_GL_CONTEXT_DESTROY 2

typedef void (*fs_gl_context_notify_function)(int notification, void *data);

void fs_gl_send_context_notification(int notification);

void fs_gl_add_context_notification(fs_gl_context_notify_function function,
        void *data);
void fs_gl_remove_context_notification(fs_gl_context_notify_function function,
        void *data);

//#define FS_DEBUG_OPENGL

#ifdef FS_DEBUG_OPENGL
#define CHECK_GL_ERROR() { \
    int error = glGetError(); \
    while (error) { \
        printf("GL ERROR %d (%s %s:%d)\n", error, __FUNCTION__, __FILE__, \
                __LINE__); \
        error = glGetError(); \
    } \
}
#else
#define CHECK_GL_ERROR()
#endif

#endif // LIBFSGL_OPENGL_H_
