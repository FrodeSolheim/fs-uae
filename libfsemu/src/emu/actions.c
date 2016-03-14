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

#include <fs/emu.h>
#include <fs/emu/actions.h>
#include <fs/emu/input.h>
#include <fs/lazyness.h>
#include <fs/glib.h>
#include "video.h"
#include "libfsemu.h"
#include "input.h"
#include <stdlib.h>

static const char* g_taunts[] = {
    "You play like a dairy farmer!",
    "No one will ever catch ME playing as badly as you do!",
    "I once owned a dog that played better then you!",
    "Never before have I faced someone so sissified.",
    "You're no match for my skills, you poor fool.",
    "Go home and be a family man!",
    "It's good to be the king.",
    "Your mother was a hamster and your father smelt of elderberries",
    NULL,
};
static int g_num_taunts = 8;

static void taunt()
{
    char *text = g_strdup_printf(
        "%c%s\n", 1, g_taunts[g_random_int_range(0, g_num_taunts)]);
    fs_emu_netplay_say(text);
    g_free(text);
}

#if 0
void fsuae_ppc_pause(int pause);
#endif

static void special_function(void)
{
    fs_log("special action\n");
#if 0
    static int paused = 0;
    fsuae_ppc_pause(!paused);
    paused = !paused;
#endif
}

static void switch_window(void)
{
    if (fs_ml_input_grab()) {
        fs_log("Switch window\n");
        fs_ml_set_input_grab(false);
        fs_ml_set_input_grab_on_activate(true);
        fs_ml_activate_window_switcher();
    }
}

void fs_emu_handle_libfsemu_action(int action, int state)
{
    if (g_fs_log_input)
        fs_log("fs_emu_handle_libfsemu_action %d %d\n", action, state);

    switch (action) {
    case FS_EMU_ACTION_FULL_KEYBOARD:
        if (state) {
            fs_emu_set_full_keyboard_emulation(
                !fs_emu_full_keyboard_emulation(), true);
        }
        break;
    case FS_EMU_ACTION_FULLSCREEN:
        if (state)
            fs_emu_toggle_fullscreen();
        break;
    case FS_EMU_ACTION_GRAB_INPUT:
        if (state) {
            if (g_fs_emu_grab_input_on_mod_release) {
                /* We had grapped the input, but is holding the modifier
                 * key. So we just don't ungrab on release. */
                g_fs_emu_grab_input_on_mod_release = false;
                fs_emu_show_cursor_msec(FS_EMU_MOUSE_DEFAULT_DURATION);
            } else {
                fs_emu_set_input_grab(!fs_emu_input_grab());
            }
        }
        break;
    case FS_EMU_ACTION_MENU_ALT:
        if (state)
            fs_emu_menu_toggle();
        break;
    case FS_EMU_ACTION_PAUSE:
        if (state)
            fs_emu_pause(!fs_emu_is_paused());
        break;
    case FS_EMU_ACTION_QUIT:
        if (state)
            fs_emu_quit();
        break;
    case FS_EMU_ACTION_SCREENSHOT:
        if (state)
            g_fs_emu_screenshot = 1;
        break;
    case FS_EMU_ACTION_SPECIAL:
        if (state)
            special_function();
        break;
    case FS_EMU_ACTION_SWITCH_WINDOW:
        if (state)
            switch_window();
        break;
    case FS_EMU_ACTION_TAUNT:
        if (state)
            taunt();
        break;
    case FS_EMU_ACTION_VOLUME_DOWN:
        if (state)
            fs_emu_volume_control(-2);
        break;
    case FS_EMU_ACTION_VOLUME_MUTE:
        if (state)
            fs_emu_volume_control(-1);
        break;
    case FS_EMU_ACTION_VOLUME_UP:
        if (state)
            fs_emu_volume_control(-3);
        break;
    case FS_EMU_ACTION_WARP:
        if (state) {
            /* FIXME: UAE-specific hot key hack for warp function,
             * should be moved out of libfsemu. */
            fs_emu_queue_input_event(0x00010000 | 275);
        }
        break;
    case FS_EMU_ACTION_ZOOM:
        if (state)
            fs_emu_toggle_zoom(0);
        break;
    case FS_EMU_ACTION_ZOOM_BORDER:
        if (state)
            fs_emu_toggle_zoom(1);
        break;
    }
}
