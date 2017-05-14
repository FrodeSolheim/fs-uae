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

#include "hud.h"
#include <fs/emu.h>
#include <fs/emu/options.h>
#include <fs/emu/video.h>
#include <fs/conf.h>
#include <fs/glib.h>
#include <fs/thread.h>
#include "libfsemu.h"
#include "render.h"
#include "menu.h"
#include "font.h"
#include <string.h>
#include <stdlib.h>

#ifdef USE_OPENGL
#include <fs/ml/opengl.h>
#endif
 
#define MAX_VISIBLE_TIME (10 * 1000 * 1000)
#define DEFAULT_DURATION (10 * 1000 * 1000)

static int g_notification_duration = DEFAULT_DURATION;
char g_fs_emu_chat_string[FSE_MAX_CHAT_STRING_LENGTH + 1] = {};
static int g_fs_emu_chat_string_pos = 0;
static int g_fs_emu_chat_mode = 0;
static int64_t g_last_line_time = 0;
int g_fs_emu_hud_mode = 0;

typedef struct message_t {
    int type;
    int64_t time;
    int64_t show_until;
    char *text;
} message_t;

static GQueue *g_console_lines = NULL;
static fs_mutex *g_console_mutex = NULL;

void fse_init_hud_early(void)
{
    FSE_INIT_ONCE();
    g_console_mutex = fs_mutex_create();
    g_console_lines = g_queue_new();
}

void fse_init_hud_after_config(void)
{
    FSE_INIT_ONCE();
    g_notification_duration = fs_config_get_int_clamped(
            OPTION_NOTIFICATION_DURATION, 0, 60 * 1000);
    if (g_notification_duration == FS_CONFIG_NONE) {
        g_notification_duration = DEFAULT_DURATION;
    } else {
        g_notification_duration *= 1000;
    }
}

int fs_emu_hud_in_chat_mode(void)
{
    return g_fs_emu_chat_mode;
}

void fs_emu_hud_enable_chat_mode(void)
{
    g_fs_emu_chat_mode = 1;
    g_fs_emu_hud_mode = 1;
}

void fse_notify(uint32_t type, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    char *buffer = g_strdup_vprintf(format, ap);
    va_end(ap);
    int len = strlen(buffer);
    // strip trailing newline, if any
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    fs_log("%s\n", buffer);

    fs_mutex_lock(g_console_mutex);

    if (type != 0) {
       message_t *line = g_queue_peek_head(g_console_lines);
       if (line && line->type == type) {
           free(line->text);
           line->text = buffer;
           line->time = fs_emu_monotonic_time();
           line->show_until = line->time + g_notification_duration;
           g_last_line_time = MAX(line->show_until, line->show_until);
           fs_mutex_unlock(g_console_mutex);
           return;
       }
    }

    message_t *line = malloc(sizeof(message_t));
    //printf("new console line at %p\n", line);
    line->type = type;
    line->text = buffer;
    line->time = fs_emu_monotonic_time();
    line->show_until = line->time + g_notification_duration;
    g_last_line_time = MAX(line->show_until, line->show_until);

    g_queue_push_head(g_console_lines, line);
    fs_mutex_unlock(g_console_mutex);
}

void fs_emu_hud_add_console_line(const char *text, int flags)
{
    message_t *line = malloc(sizeof(message_t));
    line->type = 0;
    line->text = g_strdup(text);
    line->time = fs_emu_monotonic_time();
    line->show_until = line->time + DEFAULT_DURATION;
    g_last_line_time = MAX(line->show_until, line->show_until);

    // assert(g_console_mutex != NULL);
    fs_mutex_lock(g_console_mutex);
    g_queue_push_head(g_console_lines, line);
    fs_mutex_unlock(g_console_mutex);
}

void fs_emu_hud_add_chat_message(const char *text, const char *player)
{
    char *line;
    if (text[0] == 1) {
        line = g_strdup_printf("* %s taunts: %s", player, text);
    }
    else {
        line = g_strdup_printf("<%s> %s", player, text);
    }
    fs_emu_hud_add_console_line(line, 0);
    free(line);
}

void fs_emu_netplay_say(const char *text)
{
#ifdef WITH_NETPLAY
    fs_emu_send_netplay_message(text);
    //char *line = fs_strdup_printf("<%s> %s",
     //       fs_emu_get_netplay_tag(-1), text);
    //fs_emu_add_console_line(line, 0);
    //g_free(line);
    fs_emu_hud_add_chat_message(text, fs_emu_get_netplay_tag(-1));
#endif
}

static void process_command(const char* text)
{
    if (text[0] == '\0') {
        return;
    }
    fs_log("process_command: %s\n", g_fs_emu_chat_string);
    if (text[0] != '/') {
        fs_emu_netplay_say(text);
    }
}

int fs_emu_hud_handle_chat_input(fs_emu_event *event)
{
    fs_mutex_lock(g_console_mutex);
    if (event->type == FS_ML_KEYDOWN) {
        int key_code = event->key.keysym.sym;
        if (key_code == FS_ML_KEY_RETURN) {
            fs_mutex_unlock(g_console_mutex);
            process_command(g_fs_emu_chat_string);
            fs_mutex_lock(g_console_mutex);
            g_fs_emu_chat_string_pos = 0;
            g_fs_emu_chat_string[g_fs_emu_chat_string_pos] = 0;
        } else if (key_code == FS_ML_KEY_ESCAPE) {
            g_fs_emu_chat_mode = 0;
            g_fs_emu_hud_mode = 0;
            g_fs_emu_chat_string_pos = 0;
            g_fs_emu_chat_string[g_fs_emu_chat_string_pos] = 0;
        } else if (key_code == FS_ML_KEY_TAB) {
            g_fs_emu_chat_mode = 0;
            g_fs_emu_hud_mode = 0;
            // hide all messages when dismissing the hud
            //g_last_line_time = 0;
        } else if (key_code == FS_ML_KEY_BACKSPACE) {
            if (g_fs_emu_chat_string_pos > 0) {
                g_fs_emu_chat_string_pos--;
                g_fs_emu_chat_string[g_fs_emu_chat_string_pos] = 0;
            }
        }
    }
    if (event->type == FS_ML_TEXTINPUT) {
        if (g_fs_emu_chat_string_pos < FSE_MAX_CHAT_STRING_LENGTH - 1 &&
                event->text.text[0] >= 32 &&
                ((unsigned char) event->text.text[0]) < 128) {
            g_fs_emu_chat_string[g_fs_emu_chat_string_pos] = \
                event->text.text[0];
            g_fs_emu_chat_string_pos++;
            g_fs_emu_chat_string[g_fs_emu_chat_string_pos] = '\0';
        }
    }
#ifdef USE_SDL2
        //printf("FIXME: no hud text input with SDL 2 yet");
#else
        //printf("%d\n", event->key.keysym.unicode);
        /*
        if (g_fs_emu_chat_string_pos < FSE_MAX_CHAT_STRING_LENGTH - 1 &&
                event->key.keysym.unicode >= 32 &&
                event->key.keysym.unicode < 128) {
            g_fs_emu_chat_string[g_fs_emu_chat_string_pos] =
                    (char) event->key.keysym.unicode;
            g_fs_emu_chat_string_pos++;
            g_fs_emu_chat_string[g_fs_emu_chat_string_pos] = '\0';
        }
        */
#endif
    //}
    //fs_log("chat: %s\n", g_fs_emu_chat_string);
    fs_mutex_unlock(g_console_mutex);
    return 1;
}

#define MAX_VISIBLE_LINES 12

void fse_render_hud_chat(void)
{
    GList *link;
    int k;

    //fs_emu_assert_gui_lock();
    fs_mutex_lock(g_console_mutex);
    int64_t now = fs_emu_monotonic_time();

    int64_t time_diff = now - g_last_line_time;
    //if (time_diff > MAX_VISIBLE_TIME && !g_fs_emu_chat_mode) {
    if (time_diff >= 0 && !g_fs_emu_chat_mode) {
        fs_mutex_unlock(g_console_mutex);
        return;
    }

    // FIXME: IMPLEMENT A MAX NUMBER OF LINES
    // FIXME: IMPLEMENT SCROLLING / PAGE_UP / PAGE DOWN

    /*
    int total_height = 0;
    GList *link = g_queue_peek_head_link(g_console_lines);
    int k = 0;
    while (link) {
        message_t *line = (message_t *) link->data;
        total_height += 40;

        GList* link2 = link;
        link = link->next;
        k++;
        //g_queue_delete_link(g_console_lines, link2);
        if (k == MAX_VISIBLE_LINES) {
            break;
        }
    }
    */

    /*
    if (total_height == 0 && !g_fs_emu_chat_mode) {
        return;
    }
    */

    fs_gl_ortho_hd();
#if 0
    fs_gl_blending(1);
    fs_gl_texturing(0);

    if (total_height > 0) {
        fs_gl_color4f(0.0, 0.0, 0.0, 0.5);
        glBegin(GL_QUADS);
        glVertex2f(0, 60);
        glVertex2f(1920, 60);
        glVertex2f(1920, 60 + total_height);
        glVertex2f(0, 60 + total_height);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2f(0, 60 + total_height);
        glVertex2f(1920, 60 + total_height);
        fs_gl_color4f(0.0, 0.0, 0.0, 0.0);
        glVertex2f(1920, 60 + total_height + 50);
        glVertex2f(0, 60 + total_height + 50);
        glEnd();
    }

    fs_gl_color4f(0.0, 0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(1920, 0);
    fs_gl_color4f(0.0, 0.0, 0.0, 0.5);
    glVertex2f(1920, 60);
    glVertex2f(0, 60);
    glEnd();
#endif

    if (g_fs_emu_chat_mode) {
        fs_gl_blending(1);
        fs_gl_texturing(0);
        fs_gl_color4f(0.0, 0.3, 0.5, 0.75);

#ifdef USE_GLES
        GLfloat vert[] = {
            0, 0,
            1920, 0,
            1920, 60,
            0, 60
        };

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vert);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
#else
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(1920, 0);
        glVertex2f(1920, 60);
        glVertex2f(0, 60);
        glEnd();
#endif
    }

    fs_emu_font *font = fs_emu_font_get_menu();
    int tx;
    int ty;
    int tw;
    if (g_fs_emu_chat_mode) {
        tx = 65;
        ty = 13;
        tw = fs_emu_font_render(font, "Input:", tx, ty, 1.0, 1.0, 1.0, 1.0);
        tx += tw + 20;
        fs_emu_font_render(font, g_fs_emu_chat_string,
                tx, ty, 1.0, 1.0, 1.0, 1.0);
    }

    tx = 65;
    ty = 65;

    link = g_queue_peek_head_link(g_console_lines);
    k = 0;
    while (link) {
        message_t *line = (message_t *) link->data;
        if (!g_fs_emu_chat_mode && now - line->time > MAX_VISIBLE_TIME) {
            // when not in chat mode, only show lines for a brief period
            // of time
            break;
        }
        fs_emu_font_render_with_outline(font, line->text,
                tx, ty, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.5, 2.0);
        ty += 40;
        link = link->next;
        k++;
        if (k == MAX_VISIBLE_LINES) {
            break;
        }
    }
    fs_mutex_unlock(g_console_mutex);
}

void fse_render_hud(void)
{
    fs_gl_ortho_hd();
    fse_render_hud_chat();
}
