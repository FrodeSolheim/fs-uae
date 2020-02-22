#define FSEMU_INTERNAL
#include "fsemu-action.h"

#include <stdio.h>

#include "fsemu-oskeyboard.h"
#include "fsemu-osmenu.h"

void fsemu_action_process_non_emu(fsemu_action_t action,
                                  fsemu_action_state_t state)
{
    printf("fsemu_action_process_non_emu %04x %04x\n", action, state);

    if (action == FSEMU_ACTION_OSKEYBOARD) {
        if (state) {
            fsemu_oskeyboard_toggle_open();
        }
    } else if (action == FSEMU_ACTION_OSMENU) {
        if (state) {
            fsemu_osmenu_toggle_open();
        }
    }
}
