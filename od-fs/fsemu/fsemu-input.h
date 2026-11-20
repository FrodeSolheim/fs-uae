#ifndef FSEMU_INPUT_H
#define FSEMU_INPUT_H

#include "fsemu-action.h"
#include "fsemu-common.h"
#include "fsemu-inputdevice.h"
#include "fsemu-inputport.h"
#include "fsemu-key.h"
// #include "fsemu-scancodes.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Message types that fsemu's Python side (fsemu/constants.py) posts on fsapp's main channel
// (g_fsapp_main_channel), next to fsapp's own (fsapp.h).
enum {
    FSEMU_MESSAGE_BEFORE_FIRST = 0x20000,
    // Data "<port instance id>:<device type>:<device instance id>": connect the device to the port.
    FSEMU_MESSAGE_SET_PORT_DEVICE = 0x20001,
};

#define FSEMU_INPUT_MAX_PORTS 4

// FSEMU_INPUT_MAX_DEVICES cannot be more than 128, since an int8_t is
// sometimes used to hold device_index.
#define FSEMU_INPUT_MAX_DEVICES 20

typedef struct {
    int key;
    int mod;
    int action;
} fsemu_input_configure_keyboard_t;

typedef struct {
    const char *name;
    uint16_t value;
    int flags;
} fsemu_input_action_t;

void fsemu_input_add_action(fsemu_input_action_t *action);

void fsemu_input_add_actions(fsemu_input_action_t actions[]);

fsemu_error_t fsemu_input_add_device(fsemu_inputdevice_t *device);

fsemu_inputdevice_t *fsemu_input_get_device(int index);
fsemu_inputdevice_t *fsemu_input_find_device_by_type_and_instance_id(int type, int instance_id);

void fsemu_input_remove_device(fsemu_inputdevice_t *device);

void fsemu_input_remove_device_by_index(int device_index);

void fsemu_input_add_port(fsemu_inputport_t *port);

int fsemu_input_port_count(void);

fsemu_inputport_t *fsemu_input_port_by_index(int index);
fsemu_inputport_t *fsemu_input_find_port_by_instance_id(int instance_id);

// FIXME: Possibly temporary function
// void fsemu_input_autofill_devices(void);

void fsemu_input_configure_keyboard(fsemu_input_configure_keyboard_t mapping[]);

void fsemu_input_handle_controller(int device_index, int slot, int16_t state);
void fsemu_input_handle_keyboard(fsemu_key_t scancode, bool pressed);
void fsemu_input_handle_mouse(int device_index, int slot, int16_t state);

void fsemu_input_init(void);
void fsemu_input_init_module(void);

// Whether input actions are queued for the emulation thread, without locking: checked before
// fsemu_input_next_action, which the emulation thread calls at every scanline. May be stale for a
// moment (see fsemu-input.cpp).
bool fsemu_input_has_actions(void);
bool fsemu_input_next_action(uint16_t *action, int16_t *state);
bool fsemu_input_next_command(uint16_t *action, int16_t *state);

fsemu_action_and_state_t fsemu_input_pack_action_state(uint16_t action, int16_t state);

// Internal function used by fsemu_sdlinput.
// Processes the action and decides whether to process further immediately on
// the main thread, or push to the emulation input queue, or emulation
// command queue. Later, the event will also be pushed via the netplay system
// if necessary.
void fsemu_input_process_action(uint16_t action, int16_t state);

void fsemu_input_reconfigure(void);

void fsemu_input_unpack_action_state(fsemu_action_and_state_t action_and_state, uint16_t *action,
                                     int16_t *state);

void fsemu_input_work(int timeout);

extern bool fsemu_input_reconfigure_needed;

// ----------------------------------------------------------------------------
// Logging
// ----------------------------------------------------------------------------

extern int fsemu_input_log_level;

#define fsemu_input_log(format, ...)                                                               \
    FS_LOG_INFO(fsemu_input_log_level, "[FSE] [INP]", format, ##__VA_ARGS__)

#define fsemu_input_log_debug(format, ...)                                                         \
    FS_LOG_DEBUG(fsemu_input_log_level, "[FSE] [INP]", format, ##__VA_ARGS__)

#define fsemu_input_log_error(format, ...)                                                         \
    FS_LOG_ERROR(fsemu_input_log_level, "[FSE] [INP]", format, ##__VA_ARGS__)

#define fsemu_input_log_info(format, ...)                                                          \
    FS_LOG_INFO(fsemu_input_log_level, "[FSE] [INP]", format, ##__VA_ARGS__)

#define fsemu_input_log_trace(format, ...)                                                         \
    FS_LOG_TRACE(fsemu_input_log_level, "[FSE] [INP]", format, ##__VA_ARGS__)

#define fsemu_input_log_warning(format, ...)                                                       \
    FS_LOG_WARNING(fsemu_input_log_level, "[FSE] [INP]", format, ##__VA_ARGS__)

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* FSEMU_INPUT_H */
