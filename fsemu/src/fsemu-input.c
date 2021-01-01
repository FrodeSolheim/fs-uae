#include "fsemu-internal.h"
#include "fsemu-input.h"

#include "fsemu-action.h"
#include "fsemu-controller.h"
#include "fsemu-glib.h"
#include "fsemu-gui.h"
#include "fsemu-inputdevice.h"
#include "fsemu-key.h"
#include "fsemu-mouse.h"
#include "fsemu-mutex.h"
#include "fsemu-option.h"
#include "fsemu-oskeyboard.h"
#include "fsemu-osmenu.h"
#include "fsemu-sdlinput.h"
#include "fsemu-thread.h"

#ifdef FSUAE
#include <fs/ml.h>
#endif

typedef struct {
    int16_t action;
} keyboard_t;

static struct {
    uint16_t action_table[1024 * 1024];  // FIXME
    // Queued input actions pending input to emulator
    GQueue *action_queue;
    // Queued command actions pending input to emulator
    GQueue *command_queue;
    bool initialized;
    keyboard_t keyboard[FSEMU_KEY_NUM_KEYS * FSEMU_KEYBOARD_NUM_MODS];
    int num_ports;
    fsemu_inputport_t *ports[FSEMU_INPUT_MAX_PORTS];
    int num_devices;
    fsemu_inputdevice_t *devices[FSEMU_INPUT_MAX_DEVICES];

    // Emulation thread synchronization for fetching actions
    fsemu_mutex_t *mutex;
} fsemu_input;

int fsemu_input_log_level = 1;

// ----------------------------------------------------------------------------
// FIXME: Move

static void fsemu_keyboard_add_system_device(void)
{
    fsemu_mouse_log(1, "Adding system keyboard device\n");
    fsemu_inputdevice_t *device = fsemu_inputdevice_new();
    fsemu_inputdevice_set_type(device, FSEMU_INPUTDEVICE_TYPE_KEYBOARD);
    fsemu_inputdevice_set_name(device, "Keyboard");
    int error;
    if ((error = fsemu_input_add_device(device)) != 0) {
        printf("input - device could not be registered - error %d\n", error);
        fsemu_inputdevice_unref(device);
        return;
    }
    // fsemu_mouse.system_mouse = device;
}

void fsemu_keyboard_add_devices(void)
{
    fsemu_keyboard_add_system_device();
}

static void fsemu_keyboard_init(void)
{
}

// ----------------------------------------------------------------------------

void fsemu_input_init(void)
{
    if (fsemu_input.initialized) {
        return;
    }
    fsemu_input.initialized = true;

    fsemu_mouse_init();
    fsemu_keyboard_init();

    fsemu_keyboard_add_devices();
    fsemu_mouse_add_devices();

    fsemu_sdlinput_init();

    fsemu_input_log("Init\n");
    fsemu_input.mutex = fsemu_mutex_create();

    fsemu_option_read_int(FSEMU_OPTION_LOG_INPUT, &fsemu_input_log_level);

#ifdef FSUAE_LEGACY
    fs_ml_input_init();
#endif

    fsemu_input.action_queue = g_queue_new();
    fsemu_input.command_queue = g_queue_new();
}

void fsemu_input_add_port(fsemu_inputport_t *port)
{
    if (fsemu_input.num_ports == FSEMU_INPUT_MAX_PORTS) {
        printf("FIXME: WARNING: Max input ports reached\n");
        return;
    }
    port->index = fsemu_input.num_ports;
    fsemu_input.ports[fsemu_input.num_ports] = port;
    fsemu_input.num_ports++;
    printf("[FSEMU] Added input port[%d] %s\n",
           port->index,
           fsemu_inputport_name(port));
}

int fsemu_input_port_count(void)
{
    fsemu_thread_assert_main();

    return fsemu_input.num_ports;
}

fsemu_inputport_t *fsemu_input_port_by_index(int index)
{
    fsemu_thread_assert_main();
    fsemu_assert(index >= 0 && index < fsemu_input.num_ports);

    return fsemu_input.ports[index];
}

static void fsemu_input_log_port_summary(void)
{
    printf("----------\n");
    for (int i = 0; i < fsemu_input.num_ports; i++) {
        fsemu_inputport_t *port = fsemu_input.ports[i];
        const char *device_name = "";
        if (port->device_index >= 0) {
            device_name = fsemu_inputdevice_name(
                fsemu_input.devices[port->device_index]);
        }
        printf("Input port[%d] (%s) <- %d %s\n",
               i,
               fsemu_inputport_name(port),
               port->device_index,
               device_name);
    }
    printf("----------\n");
}

fsemu_error_t fsemu_input_add_device(fsemu_inputdevice_t *device)
{
#if 0
    if (fsemu_input.num_devices == FSEMU_INPUT_MAX_DEVICES) {
        printf("FIXME: WARNING: Max input devices reached\n");
        return FSEMU_ERROR_GENERIC;
    }
    device->index = fsemu_input.num_devices;
    fsemu_input.devices[fsemu_input.num_devices] = device;
    fsemu_input.num_devices++;
#endif
    int device_index = -1;
    for (int i = 0; i < FSEMU_INPUT_MAX_DEVICES; i++) {
        if (fsemu_input.devices[i] == NULL) {
            printf("[FSEMU] Next free device index %d\n", i);
            device_index = i;
            break;
        }
    }
    device->index = device_index;
    fsemu_input.devices[device_index] = device;

    printf("[FSEMU] Added input device %s\n", fsemu_inputdevice_name(device));
    fsemu_inputdevice_ref(device);

    // FIXME: Maybe postpone to module update
    fsemu_input_autofill_devices();
    fsemu_input_reconfigure();

    return FSEMU_ERROR_NONE;
}

void fsemu_input_remove_device(fsemu_inputdevice_t *device)
{
    printf("ERROR: fsemu_input_remove_device %p (index %d)\n",
           device,
           device->index);
    if (device == NULL) {
        printf("ERROR: input device was null\n");
        return;
    }
    int device_index = -1;
    // FIXME: We don't really need to iterate here since we've got
    // device->index.
    for (int i = 0; i < FSEMU_INPUT_MAX_DEVICES; i++) {
        if (device == fsemu_input.devices[i]) {
            device_index = i;
            break;
            return;
        }
    }

    if (device_index < 0) {
        printf("ERROR: did not find input device to be removed\n");
        return;
    }

    // We have device->port_index, but let's iterate over ports instead, in
    // case of bugs that could be more reliable to make sure that device_index
    // is not used.

    for (int i = 0; i < fsemu_input.num_ports; i++) {
        fsemu_inputport_t *port = fsemu_input.ports[i];
        if (port->device_index == device_index) {
            // FIXME:
            // fsemu_inputport_detach_device();
            // fsemu_input_attach_device_to_port
            port->device_index = -1;
            if (device->port_index != i) {
                printf("ERROR: device->port_index was not set correctly\n");
            }
            device->port_index = -1;
            printf("Removing input device from port[%d]\n", i);
        }
    }

    // if (device->port_index) {
    //     fsemu_inputport_t *port = fsemu_input.ports[device->port_index];
    // }
    fsemu_input.devices[device_index] = NULL;
    fsemu_inputdevice_unref(device);

    // FIXME: Maybe postpone to module update
    fsemu_input_reconfigure();
}

void fsemu_input_remove_device_by_index(int device_index)
{
    fsemu_inputdevice_t *device = fsemu_input.devices[device_index];
    fsemu_input_remove_device(device);
}

void fsemu_input_work(int timeout)
{
    fsemu_sdlinput_work();
}

void fsemu_input_configure_keyboard(fsemu_input_configure_keyboard_t mapping[])
{
    fsemu_input_log("Configure keyboard\n");
    fsemu_input_configure_keyboard_t *item;
    for (item = mapping; item->key; item++) {
        fsemu_input_log("Keyboard key %d%s -> action %d (0x%x)\n",
                        item->key,
                        item->mod ? " (modifier FIXME)" : "",
                        item->action,
                        item->action);
        if (item->key >= FSEMU_KEY_NUM_KEYS) {
            // FIXME: WARNING
            fsemu_input_log("Key (%d) >= FSEMU_KEY_NUM_KEYS (%d); ignoring\n",
                            item->key,
                            FSEMU_KEY_NUM_KEYS);
            continue;
        }
        if (item->mod >= FSEMU_KEYBOARD_NUM_MODS) {
            // FIXME: WARNING
            fsemu_input_log(
                "Modifier (%d) >= FSEMU_KEYBOARD_NUM_MODS (%d); ignoring\n",
                item->mod,
                FSEMU_KEYBOARD_NUM_MODS);
            continue;
        }
        fsemu_input.keyboard[item->mod * FSEMU_KEY_NUM_KEYS + item->key]
            .action = item->action;
    }
}

static int fsemu_input_action_table_index_from_key(fsemu_key_t key, int mod)
{
    return mod * FSEMU_KEY_NUM_KEYS + key;
}

static int fsemu_input_action_table_index_from_input(int device_index,
                                                     int input_index)
{
    return device_index * FSEMU_INPUTDEVICE_MAX + input_index;
}

static int fsemu_input_keyboard_navigation(fsemu_key_t key)
{
    if (key == FSEMU_KEY_UP) {
        return FSEMU_GUI_NAVIGATE_UP;
    } else if (key == FSEMU_KEY_RIGHT) {
        return FSEMU_GUI_NAVIGATE_RIGHT;
    } else if (key == FSEMU_KEY_DOWN) {
        return FSEMU_GUI_NAVIGATE_DOWN;
    } else if (key == FSEMU_KEY_LEFT) {
        return FSEMU_GUI_NAVIGATE_LEFT;
    } else if (key == FSEMU_KEY_RETURN) {
        return FSEMU_GUI_NAVIGATE_PRIMARY;
    } else if (key == FSEMU_KEY_SPACE) {
        return FSEMU_GUI_NAVIGATE_SECONDARY;
    } else if (key == FSEMU_KEY_TAB) {  // FIXME: Is tab suitable key?
        return FSEMU_GUI_NAVIGATE_TERTIARY;
    } else if (key == FSEMU_KEY_BACKSPACE) {
        return FSEMU_GUI_NAVIGATE_BACK;
    } else if (key == FSEMU_KEY_F11) {
        // FIXME: Hackish, mapping both F11 and F12 to close oskeyboard and
        // osmenu. Should only close with the toggle key. Fine for now.
        return FSEMU_GUI_NAVIGATE_CLOSE;
    } else if (key == FSEMU_KEY_F12) {
        return FSEMU_GUI_NAVIGATE_CLOSE;
    }
    return FSEMU_GUI_NAVIGATE_NONE;
}

static int fsemu_input_controller_navigation(int slot)
{
    if (slot == FSEMU_CONTROLLER_DPUP) {
        return FSEMU_GUI_NAVIGATE_UP;
    } else if (slot == FSEMU_CONTROLLER_DPRIGHT) {
        return FSEMU_GUI_NAVIGATE_RIGHT;
    } else if (slot == FSEMU_CONTROLLER_DPDOWN) {
        return FSEMU_GUI_NAVIGATE_DOWN;
    } else if (slot == FSEMU_CONTROLLER_DPLEFT) {
        return FSEMU_GUI_NAVIGATE_LEFT;
    } else if (slot == FSEMU_CONTROLLER_A) {
        return FSEMU_GUI_NAVIGATE_PRIMARY;
    } else if (slot == FSEMU_CONTROLLER_X) {
        return FSEMU_GUI_NAVIGATE_SECONDARY;
    } else if (slot == FSEMU_CONTROLLER_Y) {
        return FSEMU_GUI_NAVIGATE_TERTIARY;
    } else if (slot == FSEMU_CONTROLLER_B) {
        return FSEMU_GUI_NAVIGATE_BACK;
    } else if (slot == FSEMU_CONTROLLER_BACK) {
        return FSEMU_GUI_NAVIGATE_CLOSE;
    } else if (slot == FSEMU_CONTROLLER_START) {
        return FSEMU_GUI_NAVIGATE_CLOSE;
    } else if (slot == FSEMU_CONTROLLER_GUIDE) {
        // FIXME: Hackish, mapping both BACK and GUIDE (and START) to close
        // oskeyboard and osmenu. Should only close with the toggle key. Fine
        // for now.
        return FSEMU_GUI_NAVIGATE_CLOSE;
    }
    return FSEMU_GUI_NAVIGATE_NONE;
}

void fsemu_input_handle_controller(int device_index, int slot, int16_t state)
{
    printf(
        "fsemu_input_handle_controller %d %d %d\n", device_index, slot, state);

    // FIXME: Temporary hack to redirect input
    if (fsemu_osmenu_open()) {
        // FIXME: Analog to digital needed to support left analog stick
        fsemu_osmenu_navigate(fsemu_input_controller_navigation(slot), state);
        return;
    }
    if (fsemu_oskeyboard_open()) {
        // FIXME: Analog to digital needed to support left analog stick
        fsemu_oskeyboard_navigate(fsemu_input_controller_navigation(slot),
                                  state);
        return;
    }

    // FIXME: Verify
    int input_index = FSEMU_INPUTDEVICE_CONTROLLER_OFFSET + slot;
    int action_table_index =
        fsemu_input_action_table_index_from_input(device_index, input_index);
    // FIXME: action_table
    int action = fsemu_input.action_table[action_table_index];  // .action;
    printf(" input -> action table index %d action %d\n",
           action_table_index,
           action);

    fsemu_input_process_action(action, state);
}

void fsemu_input_handle_keyboard(fsemu_key_t key, bool pressed)
{
    // FIXME: modifier!
    int mod = 0;
    int32_t state = pressed ? FSEMU_ACTION_STATE_MAX : 0;

    // FIXME: Increase log level
    // fsemu_input_log("Handle keyboard scancode=%d pressed=%d\n", key,
    // pressed);

    if (key >= FSEMU_KEY_NUM_KEYS) {
        // FIXME: WARNING
        fsemu_input_log("Key (%d) >= FSEMU_KEY_NUM_KEYS (%d); ignoring\n",
                        key,
                        FSEMU_KEY_NUM_KEYS);
        return;
    }

    // FIXME: Temporary hack to redirect input
    if (fsemu_osmenu_open()) {
        fsemu_osmenu_navigate(fsemu_input_keyboard_navigation(key), state);
        return;
    }
    if (fsemu_oskeyboard_open()) {
        fsemu_oskeyboard_navigate(fsemu_input_keyboard_navigation(key), state);
        return;
    }

    int action_table_index = fsemu_input_action_table_index_from_key(key, mod);
    // FIXME: action_table
    // printf("keyboard input action_table_index %d\n", action_table_index);
    // int action = fsemu_input.keyboard[action_table_index].action;

    int action = fsemu_input.action_table[action_table_index];
    fsemu_input_process_action(action, state);
}

void fsemu_input_handle_mouse(int device_index, int slot, int16_t state)
{
    // printf("fsemu_input_handle_mouse %d %d %d\n", device_index, slot, state);

    // FIXME: Temporary hack to redirect input
    if (fsemu_osmenu_open()) {
        return;
    }
    if (fsemu_oskeyboard_open()) {
        return;
    }

    // FIXME: Verify
    int input_index = FSEMU_INPUTDEVICE_MOUSE_OFFSET + slot;
    int action_table_index =
        fsemu_input_action_table_index_from_input(device_index, input_index);
    // FIXME: action_table
    int action = fsemu_input.action_table[action_table_index];  // .action;
    // printf(" input -> action table index %d action %d\n",
    //        action_table_index,
    //        action);

    fsemu_input_process_action(action, state);
}

fsemu_action_and_state_t fsemu_input_pack_action_state(uint16_t action,
                                                       int16_t state)
{
    return (state & 0xffff) << 16 | (action & 0xffff);
}

void fsemu_input_unpack_action_state(fsemu_action_and_state_t action_and_state,
                                     uint16_t *action,
                                     int16_t *state)
{
    *action = (action_and_state & 0xffff);
    *state = (int16_t)((action_and_state & 0xffff0000) >> 16);
}

void fsemu_input_process_action(uint16_t action, int16_t state)
{
    if (action == FSEMU_ACTION_NONE) {
        // No action - not considered an error, it's convenient to allow code
        // to just look up an action in a table and process it without
        // checking for "null action".
        return;
    }
#if 0
    if (action < 0 || action > 0xffff) {
        // FIXME: WARNING
        fsemu_log("action < 0 || action > 0xffff; ignoring\n");
        return;
    }
    if (state < FSEMU_ACTION_STATE_MIN || state > FSEMU_ACTION_STATE_MAX) {
        // FIXME: WARNING
        fsemu_log(
            "state < FSEMU_ACTION_STATE_MIN || state > "
            "FSEMU_ACTION_STATE_MAX; ignoring\n");
        return;
    }
#endif

    // FIXME: If we get a command such as (toggle) pause or warp mode, we
    // should consider doing some preprocessing here, updating the main thread
    // state so we know if toggling it again from UI should enable/disable it
    // (the current command might come from an input device)
    // So basically "commands" such be processed in two steps.
    // - Firstly in main (immediate)
    // - Secondly in emu thread (frame start)

    // printf(".... %04x %04x\n", action, state);

    fsemu_action_and_state_t action_state =
        fsemu_input_pack_action_state(action, state);

    if (action & FSEMU_ACTION_COMMAND_FLAG) {
        fsemu_action_process_command_in_main(action, state);

        fsemu_mutex_lock(fsemu_input.mutex);
        g_queue_push_tail(fsemu_input.command_queue,
                          GUINT_TO_POINTER(action_state));
        fsemu_mutex_unlock(fsemu_input.mutex);
        // printf("(command) pushed %04x %04x\n", action, state);
    } else if (action & FSEMU_ACTION_NONEMU_FLAG) {
        fsemu_action_process_non_emu(action, state);
    } else {
        fsemu_mutex_lock(fsemu_input.mutex);
        g_queue_push_tail(fsemu_input.action_queue,
                          GUINT_TO_POINTER(action_state));
        fsemu_mutex_unlock(fsemu_input.mutex);
        // printf("(input) pushed %04x %04x\n", action, state);
    }
}

static bool fsemu_input_next_from_queue(GQueue *queue,
                                        uint16_t *action,
                                        int16_t *state,
                                        bool command)
{
    fsemu_assert(fsemu_input.initialized);

    // FIXME: Double-locking?
    fsemu_mutex_lock(fsemu_input.mutex);
    gpointer action_state_p = g_queue_pop_head(queue);
    fsemu_mutex_unlock(fsemu_input.mutex);
    if (action_state_p == NULL) {
        *action = 0;
        *state = 0;
        return false;
    }
    guint action_state = GPOINTER_TO_UINT(action_state_p);
    // *action = (action_state & 0xffff0000) >> 16;
    // *state = (int16_t)(action_state & 0xffff);
    // *action = (action_state & 0xffff);
    // *state = (int16_t)((action_state & 0xffff0000) >> 16);
    fsemu_input_unpack_action_state(action_state, action, state);
#if 0
    if (command) {
        printf("fsemu_input_next_command %04x %04x\n", *action, *state);
    } else {
        printf("fsemu_input_next_action %04x %04x\n", *action, *state);
    }
#endif
    return true;
}

bool fsemu_input_next_action(uint16_t *action, int16_t *state)
{
    return fsemu_input_next_from_queue(
        fsemu_input.action_queue, action, state, false);
}

bool fsemu_input_next_command(uint16_t *action, int16_t *state)
{
    return fsemu_input_next_from_queue(
        fsemu_input.command_queue, action, state, true);
}

void fsemu_input_add_action(fsemu_input_action_t *action)
{
    fsemu_input_log("FIXME: TODO: Register action %s = %d (0x%x)\n",
                    action->name,
                    action->value,
                    action->value);
}

void fsemu_input_add_actions(fsemu_input_action_t actions[])
{
    fsemu_input_action_t *action = actions;
    while (action->name) {
        fsemu_input_add_action(action);
        action++;
    }
}

fsemu_inputdevice_t *fsemu_input_get_device(int index)
{
    fsemu_thread_assert_main();
    fsemu_assert(index >= 0 && index < FSEMU_INPUT_MAX_DEVICES);

    return fsemu_input.devices[index];
}

static fsemu_inputdevice_t *fsemu_input_find_available_device(bool mouse)
{
    // FIXME: If device_index is going to be persistent when other devices
    // are unplugged, then we get holes, and using num_devices to iterate
    // does not make any sense...

    // for (int i = 0; i < fsemu_input.num_devices; i++) {
    for (int i = 0; i < FSEMU_INPUT_MAX_DEVICES; i++) {
        fsemu_inputdevice_t *device = fsemu_input_get_device(i);
        if (device == NULL) {
            continue;
        }
        if (mouse && device->type != FSEMU_INPUTDEVICE_TYPE_MOUSE) {
            continue;
        }
        if (!mouse && device->type == FSEMU_INPUTDEVICE_TYPE_MOUSE) {
            continue;
        }
        if (device->type == FSEMU_INPUTDEVICE_TYPE_KEYBOARD) {
            // Ignore keyboard the first time through
            // FIXME: Might want to only ignore the system keyboard device;
            // devices such as X-Arcade "controllers" should perhaps be
            // prioritized here.
            continue;
        }
        if (device->port_index == -1) {
            return device;
        }
    }

    if (mouse) {
        return NULL;
    }

    // This time also allow for keyboard devices.
    for (int i = 0; i < FSEMU_INPUT_MAX_DEVICES; i++) {
        fsemu_inputdevice_t *device = fsemu_input_get_device(i);
        if (device && device->port_index == -1) {
            return device;
        }
    }
    return NULL;
}

void fsemu_input_autofill_devices(void)
{
    fsemu_input_log("Autofill devices\n");
    for (int i = 0; i < fsemu_input.num_ports; i++) {
        fsemu_inputport_t *port = fsemu_input.ports[i];
        fsemu_input_log(
            "Input port %d: (%s)\n", i, fsemu_inputport_name(port));
        if (port->device_index >= 0) {
            // Already has device inserted
            continue;
        }

        bool mouse = false;
        // printf("a\n");
        // printf("%s\n", fsemu_inputport_mode_name(port));
        // printf("b\n");
        if (strcmp(fsemu_inputport_mode_name(port), "Mouse") == 0) {
            mouse = true;
        }

        fsemu_inputdevice_t *device = fsemu_input_find_available_device(mouse);
        if (device) {
            fsemu_input_log("Found input device %d: (%s)\n",
                            device->index,
                            fsemu_inputdevice_name(device));
            port->device_index = device->index;
            device->port_index = port->index;
        } else {
        }
    }
}

void fsemu_input_reconfigure(void)
{
    printf("fsemu_input_reconfigure\n");

    // FIXME: Do we need to clear the action table? Maybe...
    memset(fsemu_input.action_table, 0, sizeof(fsemu_input.action_table));

    printf("Add keyboard actions first\n");
    printf("FIXME: Include mods as well (check!)\n");
    // for (int i = 0; i > FSEMU_KEY_NUM_KEYS * FSEMU_KEYBOARD_NUM_MODS; i++) {
    for (int i = 0; i < FSEMU_KEY_NUM_KEYS; i++) {
        int action = fsemu_input.keyboard[i].action;
        fsemu_input.action_table[i] = action;
    }

    for (int i = 0; i < fsemu_input.num_ports; i++) {
        fsemu_inputport_t *port = fsemu_input.ports[i];
        printf("Input port %d: (%s)\n", i, fsemu_inputport_name(port));
        if (port->device_index == -1) {
            // No device in port
            printf(" - no input device\n");
            continue;
        }
        fsemu_inputdevice_t *device = fsemu_input.devices[port->device_index];
        // if (device == NULL) {
        //     continue;
        // }
        printf(" - input device %d: (%s) with mode index %d\n",
               device->index,
               fsemu_inputdevice_name(device),
               port->mode_index);
        // if (device) {
        //     port->device_index = device->index;
        //     device->port_index = port->index;
        // } else {
        // }

        fsemu_inputmode_t *mode = port->modes[port->mode_index];
        printf(" - input mode: %s\n", fsemu_inputmode_name(mode));

        for (int j = 0; j < FSEMU_INPUTDEVICE_MAX; j++) {
            int action = mode->mapping[j];
            if (action) {
                int action_table_index =
                    fsemu_input_action_table_index_from_input(device->index,
                                                              j);
                printf("[INPUT] - action table index: %d = %04x\n",
                       action_table_index,
                       action);
                fsemu_input.action_table[action_table_index] = action;
            }
        }
    }

    fsemu_input_log_port_summary();
}
