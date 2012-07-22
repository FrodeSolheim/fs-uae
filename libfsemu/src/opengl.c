/* libfsml - a media library for video and input
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

#include <fs/fs.h>
#include <fs/glee.h>
#include <fs/glu.h>
#include <fs/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <fs/ml/opengl.h>

//#define TRACE

static int g_texturing_enabled = -1;
static int g_blending_enabled = -1;
static int g_viewport_x = -1;
static int g_viewport_y = -1;
static int g_viewport_w = -1;
static int g_viewport_h = -1;
static int g_current_texture = -1;
static int g_unpack_row_length = -1;
static int g_projection = -1;
static float g_r = -1;
static float g_g = -1;
static float g_b = -1;
static float g_a = -1;

void fs_gl_reset_client_state() {
    g_texturing_enabled = -1;
    g_blending_enabled = -1;
    g_viewport_x = -1;
    g_viewport_y = -1;
    g_viewport_w = -1;
    g_viewport_h = -1;
    g_current_texture = -1;
    g_unpack_row_length = -1;
    g_projection = -1;
}

typedef struct context_notification {
    fs_gl_context_notify_function function;
    void *data;
} context_notification;

static fs_list *g_context_notifications = NULL;

void fs_gl_send_context_notification(int notification) {
    fs_list *link = g_context_notifications;
    while (link) {
        context_notification *cn = link->data;
        cn->function(notification, cn->data);
        link = link->next;
    }
}

void fs_gl_add_context_notification(fs_gl_context_notify_function function,
        void *data) {
    context_notification *cn = fs_new(context_notification, 1);
    cn->function = function;
    cn->data = data;
    // better to prepend than to append, since long-living texture
    // notifications will then end up at the end..
    g_context_notifications = fs_list_prepend(g_context_notifications, cn);
}

void fs_gl_remove_context_notification(fs_gl_context_notify_function function,
        void *data) {
    fs_list *link = g_context_notifications;
    while (link) {
        context_notification *cn = link->data;
        if (cn->function == function && cn->data == data) {
            free(cn);
            g_context_notifications = fs_list_delete_link(
                    g_context_notifications, link);
            return;
        }
        link = link->next;
    }
    fs_log("WARNING: did not find context notification to remove");
}

void fs_gl_finish() {
#ifdef TRACE
    printf("---------- glFinish ----------\n");
#endif
    glFinish();
    CHECK_GL_ERROR();
}

void fs_gl_color4fv(float *c) {
    fs_gl_color4f(c[0], c[1], c[2], c[3]);
}

void fs_gl_color4f(float r, float g, float b, float a) {
    if (g_r == r && g_g == g && g_b == b && g_a == a) {
        return;
    }
#ifdef TRACE
    printf("glColor4f %f %f %f %f\n", r, g, b, a);
#endif
    glColor4f(r, g, b, a);
    g_r = r;
    g_g = g;
    g_b = b;
    g_a = a;
}

void fs_gl_bind_texture(int texture) {
    if (g_current_texture == texture) {
        return;
    }
#ifdef TRACE
    printf("glBindTexture %d\n", texture);
#endif
    glBindTexture(GL_TEXTURE_2D, texture);
    CHECK_GL_ERROR();
    g_current_texture = texture;
}

void fs_gl_unpack_row_length(int length) {
    if (g_unpack_row_length == length) {
        return;
    }
#ifdef TRACE
    printf("glPixelStorei GL_UNPACK_ROW_LENGTH %d\n", length);
#endif
    glPixelStorei(GL_UNPACK_ROW_LENGTH, length);
    CHECK_GL_ERROR();
    g_unpack_row_length = length;
}

void fs_gl_viewport(int x, int y, int w, int h) {
    if (x == g_viewport_x && y == g_viewport_y && w == g_viewport_w
            && h == g_viewport_h) {
        return;
    }
#ifdef TRACE
    printf("glViewport %d %d %d %d\n", x, y, w, h);
#endif
    glViewport(x, y, w, h);
    CHECK_GL_ERROR();
    g_viewport_x = x;
    g_viewport_y = y;
    g_viewport_w = w;
    g_viewport_h = h;
}

void fs_gl_texturing(int enable) {
    if (enable == g_texturing_enabled) {
        return;
    }
#ifdef TRACE
    printf("glEnable GL_TEXTURE_2D %d\n", enable);
#endif
    enable ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
    CHECK_GL_ERROR();
    g_texturing_enabled = enable;
}

void fs_gl_blending(int enable) {
    if (enable == g_blending_enabled) {
        return;
    }
#ifdef TRACE
    printf("glEnable GL_BLEND %d\n", enable);
#endif
    enable ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
    CHECK_GL_ERROR();
    g_blending_enabled = enable;
}

void fs_gl_ortho() {
    if (g_projection == 1) {
        return;
    }
#ifdef TRACE
    printf("gluOrtho2D -1.0 1.0 -1.0 1.0\n");
#endif
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    CHECK_GL_ERROR();
    //glLoadIdentity();
    g_projection = 1;
}

void fs_gl_ortho_hd() {
    if (g_projection == 2) {
        return;
    }
#ifdef TRACE
    printf("gluOrtho2D 0.0 1920.0 0.0 1080.0\n");
#endif
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1920.0, 0.0, 1080.0);
    glMatrixMode(GL_MODELVIEW);
    CHECK_GL_ERROR();
    //glLoadIdentity();
    g_projection = 2;
}

void fs_gl_perspective() {
    if (g_projection == 3) {
        return;
    }
#ifdef TRACE
    printf("gluPerspective 45.0, 16.0 / 9.0 0.1 100.0\n");
#endif
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // FIXME: ASPECT
    gluPerspective(45.0, 16.0 / 9.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    CHECK_GL_ERROR();
    //glLoadIdentity();
    g_projection = 3;
}
