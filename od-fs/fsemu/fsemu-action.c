#define FSEMU_INTERNAL
#include "fsemu-action.h"

#include <stdio.h>

#include "fsemu-control.h"
#include "fsemu-glib.h"
#include "fsemu-input.h"
#include "fsemu-module.h"
#include "fsemu-oskeyboard.h"
#include "fsemu-osmenu.h"
#include "fsemu-savestate.h"
#include "fsemu-thread.h"

int fsemu_action_log_level = FSEMU_LOG_LEVEL_INFO;

static struct {
    bool initialized;
    GAsyncQueue *posted_to_main;
    // To be able to handle multiple events in the same update call and only
    // reconfigure once.
    bool reconfigure_input;
    GHashTable *action_names;
} fsemu_action;

// ----------------------------------------------------------------------------

void fsemu_action_register(const char *name, uint16_t action)
{
    // printf("Register %s -> %d\n", name, action);
    g_hash_table_insert(
        fsemu_action.action_names, strdup(name), GINT_TO_POINTER(action));
}

uint16_t fsemu_action_from_name(const char *name)
{
    uint16_t action = (uint16_t) GPOINTER_TO_INT(
        g_hash_table_lookup(fsemu_action.action_names, name));
    return action;
}

// ----------------------------------------------------------------------------

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

    fsemu_action_log_debug(
        "fsemu_action_process_non_emu %04x %04x\n", action, state);

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

    fsemu_inputport_t *port;

    switch (action) {
        case FSEMU_ACTION_PAUSE:
            fsemu_action_log_debug("FSEMU_ACTION_PAUSE\n");
            fsemu_control_set_paused_internal(state != 0);
            break;

        case FSEMU_ACTION_PAUSE_DISABLE:
            fsemu_action_log_debug("FSEMU_ACTION_PAUSE_DISABLE\n");
            if (state) {
                fsemu_control_set_paused_internal(false);
            }
            break;

        case FSEMU_ACTION_PAUSE_ENABLE:
            fsemu_action_log_debug("FSEMU_ACTION_PAUSE_ENABLE\n");
            if (state) {
                fsemu_control_set_paused_internal(true);
            }
            break;

        case FSEMU_ACTION_PAUSE_TOGGLE:
            // FIXME: This action should perhaps not propagate further, instead
            // post a new event (or rewrite this one) based on previous pause
            // state and make the action explicit (enable or disable).
            // Otherwise we risk emu and main thread getting out of sync.
            fsemu_action_log_debug("FSEMU_ACTION_PAUSE_DISABLE\n");
            fsemu_warning("Pause toggle action not supported yet\n");
            // if (state) {
            //     fsemu_control_set_paused_internal(!fsemu_control_paused());
            // }
            break;

        case FSEMU_ACTION_WARP:
            fsemu_action_log_debug("FSEMU_ACTION_WARP\n");
            fsemu_control_set_warp_internal(state != 0);
            break;

        case FSEMU_ACTION_WARP_DISABLE:
            fsemu_action_log_debug("FSEMU_ACTION_WARP_DISABLE\n");
            if (state) {
                fsemu_control_set_warp_internal(false);
            }
            break;

        case FSEMU_ACTION_WARP_ENABLE:
            fsemu_action_log_debug("FSEMU_ACTION_WARP_ENABLE\n");
            if (state) {
                fsemu_control_set_warp_internal(true);
            }
            break;

        case FSEMU_ACTION_WARP_TOGGLE:
            // FIXME: This action should perhaps not propagate further, instead
            // post a new event (or rewrite this one) based on previous warp
            // state and make the action explicit (enable or disable).
            // Otherwise we risk emu and main thread getting out of sync.
            fsemu_action_log_debug("FSEMU_ACTION_WARP_DISABLE\n");
            fsemu_warning("Warp toggle action not supported yet\n");
            // if (state) {
            //     fsemu_control_set_paused_internal(!fsemu_control_paused());
            // }
            break;

        // FIXME: from fsemu_action_update
        case FSEMU_ACTION_PORT0TYPE0:
        case FSEMU_ACTION_PORT0TYPE1:
        case FSEMU_ACTION_PORT0TYPE2:
        case FSEMU_ACTION_PORT0TYPE3:
        case FSEMU_ACTION_PORT0TYPE4:
        case FSEMU_ACTION_PORT0TYPE5:
        case FSEMU_ACTION_PORT0TYPE6:
        case FSEMU_ACTION_PORT0TYPE7:
        case FSEMU_ACTION_PORT0TYPE8:
        case FSEMU_ACTION_PORT0TYPE9:
            printf("FSEMU - Switch port %d to mode %d\n",
                   0,
                   action - FSEMU_ACTION_PORT0TYPE0);
            // FIXME: Update FS-UAE to support this method of event
            // passing?
            port = fsemu_input_port_by_index(0);
            fsemu_inputport_set_mode_by_index(
                port, action - FSEMU_ACTION_PORT0TYPE0);
            fsemu_input_reconfigure();
            fsemu_action.reconfigure_input = true;
            break;

        // FIXME: from fsemu_action_update
        case FSEMU_ACTION_PORT1TYPE0:
        case FSEMU_ACTION_PORT1TYPE1:
        case FSEMU_ACTION_PORT1TYPE2:
        case FSEMU_ACTION_PORT1TYPE3:
        case FSEMU_ACTION_PORT1TYPE4:
        case FSEMU_ACTION_PORT1TYPE5:
        case FSEMU_ACTION_PORT1TYPE6:
        case FSEMU_ACTION_PORT1TYPE7:
        case FSEMU_ACTION_PORT1TYPE8:
        case FSEMU_ACTION_PORT1TYPE9:
            printf("FSEMU - Switch port %d to mode %d\n",
                   1,
                   action - FSEMU_ACTION_PORT1TYPE0);
            // FIXME: Update FS-UAE to support this method of event passing?
            port = fsemu_input_port_by_index(1);
            fsemu_inputport_set_mode_by_index(
                port, action - FSEMU_ACTION_PORT1TYPE0);
            fsemu_action.reconfigure_input = true;
            break;
    }
}

void fsemu_action_post_to_main(int action)
{
    fsemu_thread_assert_emu();
    g_async_queue_push(fsemu_action.posted_to_main, GINT_TO_POINTER(action));
}

void fsemu_action_update(void)
{
    if (!fsemu_action.initialized) {
        return;
    }
    void *data;
    while ((data = g_async_queue_try_pop(fsemu_action.posted_to_main))) {
        int action = GPOINTER_TO_INT(data);
        // Maybe process in a different function? Kind of mixing two types of
        // processing here, so maybe not a good idea?
        fsemu_action_process_command_in_main(action, 1);
    }
    if (fsemu_action.reconfigure_input) {
        fsemu_input_reconfigure();
        fsemu_action.reconfigure_input = false;
    }
}

static void fsemu_action_quit(void)
{
    g_async_queue_unref(fsemu_action.posted_to_main);
    fsemu_action.posted_to_main = NULL;
}

void fsemu_action_init(void)
{
    if (FSEMU_MODULE_INIT(action)) {
        return;
    }
    fsemu_action.posted_to_main = g_async_queue_new();
    fsemu_action.action_names =
        g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
}
