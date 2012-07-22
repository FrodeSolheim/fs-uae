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

#include <fs/emu.h>
#include <glib.h>
#include "actions.h"

/*
void fs_emu_taunt_action(int state) {

}
*/

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

static void taunt() {
    char *text = fs_strdup_printf("%c%s\n", 1,
            g_taunts[g_random_int_range(0, g_num_taunts)]);
    fs_emu_say(text);
    free(text);
}

void fs_emu_handle_libfsemu_action(int action, int state) {
    printf("internal action %d %d\n", action, state);
    if (action == FS_EMU_ACTION_TAUNT) {
        taunt();
    }
}
