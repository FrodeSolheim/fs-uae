#include "fsemu-internal.h"
#include "fsemu-mouse.h"

#include "fsemu-config.h"
#include "fsemu-input.h"
#include "fsemu-inputdevice.h"
#include "fsemu-log.h"
#include "fsemu-manymouse.h"
#include "fsemu-option.h"
#include "fsemu-osmenu.h"
#include "fsemu-thread.h"
#include "fsemu-titlebar.h"

// ---------------------------------------------------------------------------

static struct fsemu_mouse {
    // True if the mouse cursor is (or will be) captured.
    bool captured;
    // True if emulation should grab the mouse on startup
    bool initial_grab;
    // True if emulation should grab the mouse on mouse click
    bool automatic_grab;
    // True if middle clicking should grab/ungrab input
    bool middle_click_ungrab;
    fsemu_inputdevice_t *system_mouse;
} fsemu_mouse;

int fsemu_mouse_log_level = 1;

// ---------------------------------------------------------------------------

static void fsemu_mouse_add_system_device(void)
{
    fsemu_mouse_log(1, "Adding system mouse device\n");
    fsemu_inputdevice_t *device = fsemu_inputdevice_new();
    fsemu_inputdevice_set_type(device, FSEMU_INPUTDEVICE_TYPE_MOUSE);
    fsemu_inputdevice_set_name(device, "Mouse");
    int error;
    if ((error = fsemu_input_add_device(device)) != 0) {
        printf("input - device could not be registered - error %d\n", error);
        fsemu_inputdevice_unref(device);
        return;
    }
    fsemu_mouse.system_mouse = device;
}

// ---------------------------------------------------------------------------

bool fsemu_mouse_captured(void)
{
    return fsemu_mouse.captured;
}

// ---------------------------------------------------------------------------

void fsemu_mouse_set_captured(bool captured)
{
    fsemu_mouse.captured = captured;
}

void fsemu_mouse_toggle_captured(void)
{
    fsemu_mouse_set_captured(!fsemu_mouse_captured());
}

// ---------------------------------------------------------------------------
#if 0
void fsemu_mouse_handle_position(int window_x, int window_y)
{
    fsemu_thread_assert_main();

    fsemu_mouse_log("%d,%d\n", window_x, window_y);
}

// ---------------------------------------------------------------------------

void fsemu_mouse_handle_click(int button,
                              int state,
                              int window_x,
                              int window_y)
{
    fsemu_thread_assert_main();

    fsemu_mouse_log("Button %d clicked (%d) at %d,%d\n",
                    button,
                    state,
                    window_x,
                    window_y);

    if (state) {
        if (button == 2) {
            fsemu_mouse_set_captured(!fsemu_mouse_captured());
        } else {
            if (fsemu_mouse.automatic_grab && !fsemu_mouse_captured()) {
                fsemu_mouse_set_captured(true);
            }
        }
    }
}
#endif
// ---------------------------------------------------------------------------

static int g_fs_emu_mouse_speed = 100;

#define FSEMU_MOUSE_MAX_MICE 2
#define FSEMU_MOUSE_MAX_MICE_AXES 2

// Slow down the host mouse by a factor of two by default. Modern
// high-resolution mice makes the mouse too sensitive for emulation (well,
// subjective opinion).
#define FSEMU_MOUSE_SLOWDOWN 2.0

static void fsemu_mouse_adjust_movement(int mouse, int axis, int *movement)
{
    // FIXME: Maybe these fractional values should decay over time? So that
    // the mouse does not stay at the "edge" of triggering movement for long.
    static double fract[FSEMU_MOUSE_MAX_MICE][FSEMU_MOUSE_MAX_MICE_AXES];
    double d =
        *movement * g_fs_emu_mouse_speed / (100.0 * FSEMU_MOUSE_SLOWDOWN);
    int v = (int) d;
    fract[mouse][axis] += d - v;
    int diff = (int) fract[mouse][axis];
    v += diff;
    fract[mouse][axis] -= diff;
    *movement = v;
}

// ---------------------------------------------------------------------------

// void fsemu_mouse_handle_mouse(int x, int y, int button, int state)
bool fsemu_mouse_handle_mouse(fsemu_mouse_event_t *event)
{
    fsemu_thread_assert_main();

    if (fsemu_titlebar_handle_mouse(event)) {
        return true;
    }

    if (event->button > 0) {
        fsemu_mouse_log(2,
                        "Button %d clicked (%d) at %d,%d\n",
                        event->button,
                        event->state,
                        event->x,
                        event->y);

        if (event->state) {
            // if (fsemu_osmenu_open()) {
            //     return true;
            // } else
            if (event->button == 2) {
                if (fsemu_mouse_captured()) {
                    fsemu_mouse_set_captured(false);
                } else if (fsemu_osmenu_open()) {
                    // Don't want to grab while menu is open
                } else {
                    fsemu_mouse_set_captured(true);
                }
                return true;
            } else {
                if (fsemu_mouse.automatic_grab && !fsemu_mouse_captured() &&
                    !fsemu_osmenu_open()) {
                    fsemu_mouse_set_captured(true);
                    return true;
                }
            }
        }
    } else {
        // fsemu_mouse_log("%d,%d\n", event->x, event->y);
#if 0
        if (y == 0) { // FIXME: CHECK GRAB?
            fsemu_titlebar_handle_mouse(x, y, button, state);
        }
#endif
    }

    // FIXME: Hardcoded for now.. :-/
    int device_index = 1;
    int slot = 0;
    int state = 0;

    if (event->button > 0) {
        if (event->button > 3) {
            printf("Mouse button %d not supported yet\n", event->button);
        } else {
            slot = FSEMU_MOUSE_BEFORE_FIRST_BUTTON + event->button;
            state = event->state ? FSEMU_ACTION_STATE_MAX : 0;
        }
        fsemu_input_handle_mouse(device_index, slot, state);
    } else {
        // printf("%d,%d\n", event->rel_x, event->rel_y);
        int x = event->rel_x;
        int y = event->rel_y;

        fsemu_mouse_adjust_movement(0, 0, &x);
        fsemu_mouse_adjust_movement(0, 1, &y);

        if (y > 0) {
            fsemu_input_handle_mouse(device_index, FSEMU_MOUSE_YPOS, y);
            fsemu_input_handle_mouse(device_index, FSEMU_MOUSE_YPOS, 0);
        } else if (y < 0) {
            fsemu_input_handle_mouse(device_index, FSEMU_MOUSE_YNEG, -y);
            fsemu_input_handle_mouse(device_index, FSEMU_MOUSE_YNEG, 0);
        }
        if (x > 0) {
            fsemu_input_handle_mouse(device_index, FSEMU_MOUSE_XPOS, x);
            fsemu_input_handle_mouse(device_index, FSEMU_MOUSE_XPOS, 0);
        } else if (x < 0) {
            fsemu_input_handle_mouse(device_index, FSEMU_MOUSE_XNEG, -x);
            fsemu_input_handle_mouse(device_index, FSEMU_MOUSE_XNEG, 0);
        }
    }

    return false;
}

void fsemu_mouse_add_devices(void)
{
    fsemu_mouse_add_system_device();
}

// ---------------------------------------------------------------------------

void fsemu_mouse_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_thread_init();
    fsemu_titlebar_init();

    fsemu_mouse_log(1, "Init\n");

#ifdef FSEMU_MANYMOUSE
    // FIXME: Or many put the dependency the other way around, and call
    // fsemu_manymouse_init from somewhere else.
    fsemu_manymouse_init();
#endif

    fsemu_option_read_int(FSEMU_OPTION_LOG_MOUSE, &fsemu_mouse_log_level);
#if 0
    fsemu_mouse.initial_grab = true;
    fsemu_config_read_bool(
        FSEMU_OPTION_AUTOMATIC_INPUT_GRAB, &fsemu_mouse.initial_grab);
    fsemu_mouse.automatic_grab = true;
    fsemu_config_read_bool(
        FSEMU_OPTION_AUTOMATIC_INPUT_GRAB, &fsemu_mouse.automatic_grab);
#endif

    fsemu_config_read_bool_default(
        FSEMU_OPTION_AUTOMATIC_INPUT_GRAB, &fsemu_mouse.automatic_grab, true);
    fsemu_config_read_bool_default(FSEMU_OPTION_INITIAL_INPUT_GRAB,
                                   &fsemu_mouse.initial_grab,
                                   fsemu_mouse.automatic_grab);

    // FIXME: For now, inital grab is always off. TODO: Default to true when
    // emulator needs the mouse. Perhaps have tri-state option:
    // on / off / on-demand.
    fsemu_mouse.initial_grab = false;

    fsemu_config_read_bool_default(FSEMU_OPTION_MIDDLE_CLICK_UNGRAB,
                                   &fsemu_mouse.middle_click_ungrab,
                                   true);

    fsemu_mouse_log(1, "Initial grab:        %d\n", fsemu_mouse.initial_grab);
    fsemu_mouse_log(
        1, "Automatic grab:      %d\n", fsemu_mouse.automatic_grab);
    fsemu_mouse_log(
        1, "Middle-click ungrab: %d\n", fsemu_mouse.middle_click_ungrab);

    // Set the desired state of mouse capture (to be handled by window update
    // or creation function
    fsemu_mouse.captured = fsemu_mouse.initial_grab;
}
