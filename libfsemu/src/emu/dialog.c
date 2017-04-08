/* libfsemu - a library with emulator support functions
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "dialog.h"

#include <fs/emu.h>
#include <fs/glib.h>

#include <stdlib.h>

#ifdef USE_OPENGL
#include <fs/ml/opengl.h>
#endif

#include "font.h"
#include "menu.h"
#include "render.h"

static GList *g_dialog_stack = NULL;
int g_fs_emu_dialog_mode = 0;

#if 0
void fs_emu_dialog_init(void)
{
    fs_log("initialize dialog module\n");
}
#endif

fs_emu_dialog* fs_emu_dialog_create(const char *title,
        const char *affirmative, const char *negative) {
    fs_emu_dialog* dialog = g_malloc0(sizeof(fs_emu_dialog));
    if (title) {
        dialog->title = g_strdup(title);
    }
    if (affirmative) {
        dialog->affirmative = g_strdup(affirmative);
    }
    if (negative) {
        dialog->negative = g_strdup(negative);
    }
    return dialog;
}

void fs_emu_dialog_destroy(fs_emu_dialog *dialog) {
    if (dialog->title) {
        g_free(dialog->title);
    }
    if (dialog->affirmative) {
        g_free(dialog->affirmative);
    }
    if (dialog->negative) {
        g_free(dialog->negative);
    }
    for (int i = 0; i < DIALOG_MAX_LINES; i++) {
        if (dialog->lines[i]) {
            g_free(dialog->lines[i]);
        }
    }
    g_free(dialog);
}

void fs_emu_dialog_add_option(fs_emu_dialog *dialog, const char *option,
        int value) {
    fs_emu_assert_gui_lock();
}

void fs_emu_dialog_set_line(fs_emu_dialog *dialog, int line,
        const char *text) {
    if (line < 0 || line >= DIALOG_MAX_LINES) {
        fs_log("warning: invalid dialog line number");
        return;
    }
    if (dialog->lines[line]) {
        g_free(dialog->lines[line]);
    }
    dialog->lines[line] = g_strdup(text);
}

void fs_emu_dialog_show(fs_emu_dialog *dialog) {
    fs_emu_assert_gui_lock();
    g_dialog_stack = g_list_append(g_dialog_stack, dialog);
    g_fs_emu_dialog_mode = 1;
}

int fs_emu_dialog_result(fs_emu_dialog *dialog) {
    return dialog->result;
}

void fs_emu_dialog_dismiss(fs_emu_dialog *dialog) {
    fs_emu_assert_gui_lock();
    GList* link = g_dialog_stack;
    while (link) {
        if (link->data == dialog) {
            g_dialog_stack = g_list_delete_link(g_dialog_stack, link);
            break;
        }
        link = link->next;
    }
    g_fs_emu_dialog_mode = (g_dialog_stack != NULL);
}

fs_emu_dialog *fs_emu_dialog_get_current() {
    fs_emu_assert_gui_lock();
    fs_emu_dialog *dialog = NULL;
    GList* link = g_dialog_stack;
    while (link) {
        dialog = link->data;
        link = link->next;
    }
    return dialog;
}

void fs_emu_dialog_handle_action(int action, int state) {
    //printf("dialog-action\n");
    if (state == 0) {
        return;
    }
    //printf("dialog-action\n");
    fs_emu_dialog *dialog = fs_emu_dialog_get_current();
    if (!dialog) {
        return;
    }

    //printf("%d %d %d\n", action, ACTION_MENU_PRIMARY, ACTION_MENU_BACK);
    if (dialog->affirmative && action == ACTION_MENU_PRIMARY) {
        //printf("..\n");
        dialog->result = DIALOG_RESULT_AFFIRMATIVE;
    }
    else if (dialog->negative && action == ACTION_MENU_BACK) {
        //printf("...\n");
        dialog->result = DIALOG_RESULT_NEGATIVE;
    }
}

void fs_emu_dialog_render() {
    fs_emu_dialog *dialog = fs_emu_dialog_get_current();
    if (!dialog) {
        return;
    }

    fs_gl_ortho_hd();
    fs_gl_blending(1);
    fs_gl_texturing(0);
    fs_gl_color4f(0.0, 0.0, 0.0, 0.5);
    
#ifdef USE_GLES    
    GLfloat vert[] = {
        0, 0,
        1920, 0,
        1920, 1080,
        0, 1080
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vert);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
#else
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(1920, 0);
    glVertex2f(1920, 1080);
    glVertex2f(0, 1080);
    glEnd();
#endif

    fs_gl_blending(0);

    int width = 1920 / 2;
    int height = 480; //1080 / 2;
    float x1 = (1920 - width) / 2.0;
    float x2 = x1 + width;
    float y1 = (1080 - height) / 2.0;
    float y2 = y1 + height;

#ifdef USE_GLES
    GLfloat color2[] = {
        0.0, 0.4, 0.75, 1.0,
        0.0, 0.4, 0.75, 1.0,
        0.0, 0.2, 0.375, 1.0,
        0.0, 0.2, 0.375, 1.0
    };
    GLfloat vert2[] = {
        x1, y1,
        x2, y1,
        x2, y2,
        x1, y2
    };

    glEnableClientState(GL_COLOR_ARRAY);

    glColorPointer(4, GL_FLOAT, 0, color2);
    glVertexPointer(2, GL_FLOAT, 0, vert2);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
#else
    glBegin(GL_QUADS);
    fs_gl_color4f(0.0, 0.4, 0.75, 1.0);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    fs_gl_color4f(0.0, 0.2, 0.375, 1.0);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
#endif

    fs_emu_font *font = fs_emu_font_get_menu();
    int tx = x1 + 50;
    int ty = y2 - 80;
    if (dialog->title) {
        fs_emu_font_render(font, dialog->title,
                tx, ty, 1.0, 1.0, 1.0, 1.0);
        ty -= 50;
    }

    for (int i = 0; i < DIALOG_MAX_LINES; i++) {
        if (dialog->lines[i]) {
            fs_emu_font_render(font, dialog->lines[i],
                    tx, ty, 1.0, 1.0, 1.0, 1.0);
        }
        ty -= 50;
    }

    tx = x2;
    ty = y1 - 50;
    int tw;
    if (dialog->affirmative) {
        fs_emu_font_measure(font, dialog->affirmative, &tw, NULL);
        tx -= tw;
        fs_emu_font_render(font, dialog->affirmative,
                tx, ty, 1.0, 1.0, 1.0, 1.0);
        tx -= 20;
        fs_emu_font_measure(font, "<OK>", &tw, NULL);
        tx -= tw;
        fs_emu_font_render(font, "<OK>", tx, ty, 1.0, 1.0, 1.0, 1.0);
        //tx -= 40;
    }
    tx = x1;
    if (dialog->negative) {
        //fs_emu_font_measure(font, dialog->negative, &tw, NULL);
        //tx -= tw;
        tx += fs_emu_font_render(font, "<BK>", tx, ty, 1.0, 1.0, 1.0, 1.0);
        tx += 20;
        fs_emu_font_render(font, dialog->negative,
                tx, ty, 1.0, 1.0, 1.0, 1.0);
        //fs_emu_font_measure(font, "<BK>", &tw, NULL);
        //tx -= tw;
        //tx -= 40;
    }
}
