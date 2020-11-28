#include "fsemu-internal.h"
#include "fsemu-action.h"

#include <stdio.h>

#include "fsemu-control.h"
#include "fsemu-input.h"
#include "fsemu-oskeyboard.h"
#include "fsemu-osmenu.h"
#include "fsemu-savestate.h"
#include "fsemu-thread.h"

void fsemu_action_post_from_main(uint16_t action)
{
    fsemu_thread_assert_main();

    fsemu_input_process_action(action, 1);
}

void fsemu_action_post_from_main_with_state(uint16_t action, int16_t state)
{
    fsemu_thread_assert_main();

    fsemu_input_process_action(action, state);
}

void fsemu_action_process_non_emu(fsemu_action_t action,
                                  fsemu_action_state_t state)
{
    fsemu_thread_assert_main();

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

#if 0
    if (action >= FSEMU_ACTION_LOADSTATE0 &&
        action <= FSEMU_ACTION_SAVESTATE9) {
        if (state) {
            fsemu_savestate_handle_action(action);
        }
    }
#endif
}

void fsemu_action_process_command_in_main(fsemu_action_t action,
                                          fsemu_action_state_t state)
{
    fsemu_thread_assert_main();

    switch (action) {
        case FSEMU_ACTION_PAUSE:
            fsemu_action_log("[MAIN] FSEMU_ACTION_PAUSE\n");
            fsemu_control_set_paused_internal(state != 0);
            break;

        case FSEMU_ACTION_PAUSE_DISABLE:
            fsemu_action_log("[MAIN] FSEMU_ACTION_PAUSE_DISABLE\n");
            if (state) {
                fsemu_control_set_paused_internal(false);
            }
            break;

        case FSEMU_ACTION_PAUSE_ENABLE:
            fsemu_action_log("[MAIN] FSEMU_ACTION_PAUSE_ENABLE\n");
            if (state) {
                fsemu_control_set_paused_internal(true);
            }
            break;

        case FSEMU_ACTION_PAUSE_TOGGLE:
            // FIXME: This action should perhaps not propagate further, instead
            // post a new event (or rewrite this one) based on previous pause
            // state and make the action explicit (enable or disable).
            // Otherwise we risk emu and main thread getting out of sync.
            fsemu_action_log("[MAIN] FSEMU_ACTION_PAUSE_DISABLE\n");
            fsemu_warning("Pause toggle action not supported yet\n");
            // if (state) {
            //     fsemu_control_set_paused_internal(!fsemu_control_paused());
            // }
            break;

        case FSEMU_ACTION_WARP:
            fsemu_action_log("[MAIN] FSEMU_ACTION_WARP\n");
            fsemu_control_set_warp_internal(state != 0);
            break;

        case FSEMU_ACTION_WARP_DISABLE:
            fsemu_action_log("[MAIN] FSEMU_ACTION_WARP_DISABLE\n");
            if (state) {
                fsemu_control_set_warp_internal(false);
            }
            break;

        case FSEMU_ACTION_WARP_ENABLE:
            fsemu_action_log("[MAIN] FSEMU_ACTION_WARP_ENABLE\n");
            if (state) {
                fsemu_control_set_warp_internal(true);
            }
            break;

        case FSEMU_ACTION_WARP_TOGGLE:
            // FIXME: This action should perhaps not propagate further, instead
            // post a new event (or rewrite this one) based on previous warp
            // state and make the action explicit (enable or disable).
            // Otherwise we risk emu and main thread getting out of sync.
            fsemu_action_log("[MAIN] FSEMU_ACTION_WARP_DISABLE\n");
            fsemu_warning("Warp toggle action not supported yet\n");
            // if (state) {
            //     fsemu_control_set_paused_internal(!fsemu_control_paused());
            // }
            break;
    }
}
