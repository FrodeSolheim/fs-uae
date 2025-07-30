#define FSUAE_INTERNAL
#include "fsuae-inputport.h"

#include "fsemu-action.h"
#include "fsemu-input.h"
#include "fsemu-inputport.h"
#include "fsuae-action.h"
#include "fsuae-options.h"

static struct {
    fsemu_inputport_t *joystick_port_1;
    fsemu_inputport_t *joystick_port_0;
    int selected_state_slot;
} fsuae_inputport;

static void add_none_mode(fsemu_inputport_t *port, int num)
{
    fsemu_inputmode_t *mode = fsemu_inputmode_new();
    fsemu_inputmode_set_name(mode, "None");
    fsemu_inputmode_set_title(mode, "None");
    fsemu_inputport_add_mode(port, mode);
}

#define MAP(input, output) \
    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_##input, output)

static void add_mouse_mode(fsemu_inputport_t *port, int num)
{
    fsemu_inputmode_t *mode = fsemu_inputmode_new();
    fsemu_inputmode_set_name(mode, "Mouse");
    fsemu_inputmode_set_title(mode, "Amiga mouse");
    fsemu_inputport_add_mode(port, mode);

    if (num == 1) {
        MAP(DPUP, ACTION_MOUSE1_UP);
        MAP(DPRIGHT, ACTION_MOUSE1_RIGHT);
        MAP(DPDOWN, ACTION_MOUSE1_DOWN);
        MAP(DPLEFT, ACTION_MOUSE1_LEFT);
        MAP(BUTTON_A, ACTION_JOYSTICK1_FIRE);
        MAP(BUTTON_B, ACTION_MOUSE1_UP);
        // MAP(RIGHTSTICK, ACTION_MOUSE0_FIRE);

        MAP(MOUSEYNEG, ACTION_MOUSE1_UP);
        MAP(MOUSEXPOS, ACTION_MOUSE1_RIGHT);
        MAP(MOUSEYPOS, ACTION_MOUSE1_DOWN);
        MAP(MOUSEXNEG, ACTION_MOUSE1_LEFT);
        MAP(LEFTMOUSE, ACTION_JOYSTICK1_FIRE);
        MAP(RIGHTMOUSE, ACTION_JOYSTICK1_BUTTON2);
    } else {
        MAP(DPUP, ACTION_MOUSE0_UP);
        MAP(DPRIGHT, ACTION_MOUSE0_RIGHT);
        MAP(DPDOWN, ACTION_MOUSE0_DOWN);
        MAP(DPLEFT, ACTION_MOUSE0_LEFT);
        MAP(BUTTON_A, ACTION_JOYSTICK0_FIRE);
        MAP(BUTTON_B, ACTION_MOUSE0_UP);

        MAP(MOUSEYNEG, ACTION_MOUSE0_UP);
        MAP(MOUSEXPOS, ACTION_MOUSE0_RIGHT);
        MAP(MOUSEYPOS, ACTION_MOUSE0_DOWN);
        MAP(MOUSEXNEG, ACTION_MOUSE0_LEFT);
        MAP(LEFTMOUSE, ACTION_JOYSTICK0_FIRE);
        MAP(RIGHTMOUSE, ACTION_JOYSTICK0_BUTTON2);
    }
}

static void add_joystick_mode(fsemu_inputport_t *port, int num)
{
    fsemu_inputmode_t *mode = fsemu_inputmode_new();

    mode = fsemu_inputmode_new();
    fsemu_inputmode_set_name(mode, "Joystick");
    fsemu_inputmode_set_title(mode, "Amiga joystick");
    fsemu_inputport_add_mode(port, mode);

#if 0
    map(mode, ctl, FSEMU_CONTROLLER_DPUP, base + OFFSET_UP);
    map(mode, ctl, FSEMU_CONTROLLER_DPRIGHT, base + OFFSET_RIGHT);
    map(mode, ctl, FSEMU_CONTROLLER_DPDOWN, base + OFFSET_DOWN);
    map(mode, ctl, FSEMU_CONTROLLER_DPLEFT, base + OFFSET_LEFT);
    map(mode, ctl, FSEMU_CONTROLLER_LEFTYNEG, base + OFFSET_UP);
    map(mode, ctl, FSEMU_CONTROLLER_LEFTXPOS, base + OFFSET_RIGHT);
    map(mode, ctl, FSEMU_CONTROLLER_LEFTYPOS, base + OFFSET_DOWN);
    map(mode, ctl, FSEMU_CONTROLLER_LEFTXNEG, base + OFFSET_LEFT);
    map(mode, ctl, FSEMU_CONTROLLER_A, base + OFFSET_FIRE);
    map(mode, ctl, FSEMU_CONTROLLER_B, base + OFFSET_UP);

    map(mode, key, FSEMU_KEY_UP, base + OFFSET_UP);
    map(mode, key, FSEMU_KEY_RIGHT, base + OFFSET_RIGHT);
    map(mode, key, FSEMU_KEY_DOWN, base + OFFSET_DOWN);
    map(mode, key, FSEMU_KEY_LEFT, base + OFFSET_LEFT);
    map(mode, key, FSEMU_KEY_RCTRL, base + OFFSET_FIRE);
    map(mode, key, FSEMU_KEY_RALT, base + OFFSET_FIRE);
#endif

#if 0
    fsemu_inputmode_map(mode, FSEMU_CONTROLLER_DPUP, base + OFFSET_UP);
    fsemu_inputmode_map(mode, FSEMU_CONTROLLER_DPRIGHT, base + OFFSET_RIGHT);
    fsemu_inputmode_map(mode, FSEMU_CONTROLLER_DPDOWN, base + OFFSET_DOWN);
    fsemu_inputmode_map(mode, FSEMU_CONTROLLER_DPLEFT, base + OFFSET_LEFT);
    fsemu_inputmode_map(mode, FSEMU_CONTROLLER_LEFTYNEG, base + OFFSET_UP);
    fsemu_inputmode_map(mode, FSEMU_CONTROLLER_LEFTXPOS, base + OFFSET_RIGHT);
    fsemu_inputmode_map(mode, FSEMU_CONTROLLER_LEFTYPOS, base + OFFSET_DOWN);
    fsemu_inputmode_map(mode, FSEMU_CONTROLLER_LEFTXNEG, base + OFFSET_LEFT);
    fsemu_inputmode_map(mode, FSEMU_CONTROLLER_A, base + OFFSET_FIRE);
    fsemu_inputmode_map(mode, FSEMU_CONTROLLER_B, base + OFFSET_UP);

    fsemu_inputmode_map(mode, FSEMU_KEYBOARD_UP, base + OFFSET_UP);
    fsemu_inputmode_map(mode, FSEMU_KEYBOARD_RIGHT, base + OFFSET_RIGHT);
    fsemu_inputmode_map(mode, FSEMU_KEYBOARD_DOWN, base + OFFSET_DOWN);
    fsemu_inputmode_map(mode, FSEMU_KEYBOARD_LEFT, base + OFFSET_LEFT);
    fsemu_inputmode_map(mode, FSEMU_KEYBOARD_RCTRL, base + OFFSET_FIRE);
    fsemu_inputmode_map(mode, FSEMU_KEYBOARD_RALT, base + OFFSET_FIRE);

    fsemu_inputmode_map(mode, FSEMU_MOUSE_LEFT, base + OFFSET_FIRE);
#endif

#if 0
    fsemu_inputmode_map(mode, FSEMU_AXIS_LEFTYNEG, base + OFFSET_UP);
    fsemu_inputmode_map(mode, FSEMU_AXIS_LEFTXPOS, base + OFFSET_RIGHT);
    fsemu_inputmode_map(mode, FSEMU_AXIS_LEFTYPOS, base + OFFSET_DOWN);
    fsemu_inputmode_map(mode, FSEMU_AXIS_LEFTXNEG, base + OFFSET_LEFT);

    fsemu_inputmode_map(mode, FSEMU_BUTTON_DPUP, base + OFFSET_UP);
    fsemu_inputmode_map(mode, FSEMU_BUTTON_DPRIGHT, base + OFFSET_RIGHT);
    fsemu_inputmode_map(mode, FSEMU_BUTTON_DPDOWN, base + OFFSET_DOWN);
    fsemu_inputmode_map(mode, FSEMU_BUTTON_DPLEFT, base + OFFSET_LEFT);
    fsemu_inputmode_map(mode, FSEMU_BUTTON_A, base + OFFSET_FIRE);
    fsemu_inputmode_map(mode, FSEMU_BUTTON_B, base + OFFSET_UP);

    fsemu_inputmode_map(mode, FSEMU_KEY_UP, base + OFFSET_UP);
    fsemu_inputmode_map(mode, FSEMU_KEY_RIGHT, base + OFFSET_RIGHT);
    fsemu_inputmode_map(mode, FSEMU_KEY_DOWN, base + OFFSET_DOWN);
    fsemu_inputmode_map(mode, FSEMU_KEY_LEFT, base + OFFSET_LEFT);
    fsemu_inputmode_map(mode, FSEMU_KEY_RCTRL, base + OFFSET_FIRE);
    fsemu_inputmode_map(mode, FSEMU_KEY_RALT, base + OFFSET_FIRE);

    fsemu_inputmode_map(mode, FSEMU_BUTTON_LEFTMOUSE, base + OFFSET_FIRE);
#endif

#if 0
    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_LEFTYNEG, base + OFFSET_UP);
    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_LEFTXPOS, base + OFFSET_RIGHT);
    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_LEFTYPOS, base + OFFSET_DOWN);
    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_LEFTXNEG, base + OFFSET_LEFT);

    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_DPUP, base + OFFSET_UP);
    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_DPRIGHT, base + OFFSET_RIGHT);
    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_DPDOWN, base + OFFSET_DOWN);
    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_DPLEFT, base + OFFSET_LEFT);
    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_SOUTH, base + OFFSET_FIRE);
    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_EAST, base + OFFSET_UP);

    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_WEST, ACTION_KEY_SPACE);

    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_BACK, FSEMU_ACTION_OSKEYBOARD);

    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_UP, base + OFFSET_UP);
    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_RIGHT, base + OFFSET_RIGHT);
    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_DOWN, base + OFFSET_DOWN);
    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_LEFT, base + OFFSET_LEFT);
    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_RCTRL, base + OFFSET_FIRE);
    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_RALT, base + OFFSET_FIRE);

    // fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_MOUSELEFT, base +
    // OFFSET_FIRE);
#endif

    if (num == 1) {
        MAP(DPUP, ACTION_JOYSTICK1_UP);
        MAP(DPRIGHT, ACTION_JOYSTICK1_RIGHT);
        MAP(DPDOWN, ACTION_JOYSTICK1_DOWN);
        MAP(DPLEFT, ACTION_JOYSTICK1_LEFT);
        MAP(BUTTON_A, ACTION_JOYSTICK1_FIRE);
        MAP(BUTTON_B, ACTION_JOYSTICK1_UP);
        MAP(RIGHTSTICK, ACTION_JOYSTICK0_FIRE);
    } else {
        MAP(DPUP, ACTION_JOYSTICK0_UP);
        MAP(DPRIGHT, ACTION_JOYSTICK0_RIGHT);
        MAP(DPDOWN, ACTION_JOYSTICK0_DOWN);
        MAP(DPLEFT, ACTION_JOYSTICK0_LEFT);
        MAP(BUTTON_A, ACTION_JOYSTICK0_FIRE);
        MAP(BUTTON_B, ACTION_JOYSTICK0_UP);
    }

    if (num == 1) {
        MAP(UP, ACTION_JOYSTICK1_UP);
        MAP(RIGHT, ACTION_JOYSTICK1_RIGHT);
        MAP(DOWN, ACTION_JOYSTICK1_DOWN);
        MAP(LEFT, ACTION_JOYSTICK1_LEFT);
        MAP(RCTRL, ACTION_JOYSTICK1_FIRE);
        MAP(RALT, ACTION_JOYSTICK1_FIRE);
    } else {
        MAP(UP, ACTION_JOYSTICK0_UP);
        MAP(RIGHT, ACTION_JOYSTICK0_RIGHT);
        MAP(DOWN, ACTION_JOYSTICK0_DOWN);
        MAP(LEFT, ACTION_JOYSTICK0_LEFT);
        MAP(RCTRL, ACTION_JOYSTICK0_FIRE);
        MAP(RALT, ACTION_JOYSTICK0_FIRE);
    }

    // FIXME: These should maybe be pre-initialized by FSEMU, unless overridden?
    MAP(GUIDE, FSEMU_ACTION_OSMENU);
    MAP(START, FSEMU_ACTION_OSMENU);
    MAP(BACK, FSEMU_ACTION_OSKEYBOARD);
}

static void add_cd32_mode(fsemu_inputport_t *port, int num)
{
    fsemu_inputmode_t *mode = fsemu_inputmode_new();
    fsemu_inputmode_set_name(mode, "CD32");
    fsemu_inputmode_set_title(mode, "CD32 controller");
    fsemu_inputport_add_mode(port, mode);
}

static void configure_joystick_port(fsemu_inputport_t *port, int num)
{
    if (num == 1) {
        fsemu_inputport_set_name(port, "Joystick Port 1");
        fsemu_inputport_set_config_name(port, OPTION_JOYSTICK_PORT_1);
        // base = ACTION_JOYSTICK1_BEFOREFIRST;
    } else {
        fsemu_inputport_set_name(port, "Joystick Port 0");
        fsemu_inputport_set_config_name(port, OPTION_JOYSTICK_PORT_0);
        // base = ACTION_JOYSTICK2_BEFOREFIRST;
    }

    // These must be in the same order as FSUAE_JOYSTICK_PORT_MODE_NONE, ...
    add_none_mode(port, num);
    add_mouse_mode(port, num);
    add_joystick_mode(port, num);
    add_cd32_mode(port, num);

    fsemu_input_add_port(port);
}

enum {
    FSUAE_JOYSTICK_PORT_MODE_NONE,
    FSUAE_JOYSTICK_PORT_MODE_MOUSE,
    FSUAE_JOYSTICK_PORT_MODE_JOYSTICK,
    FSUAE_JOYSTICK_PORT_MODE_CD32,
};

void fsuae_inputport_init(void)
{
    fsuae_action_init();

    fsuae_inputport.joystick_port_1 = fsemu_inputport_new();
    configure_joystick_port(fsuae_inputport.joystick_port_1, 1);
    fsuae_inputport.joystick_port_0 = fsemu_inputport_new();
    configure_joystick_port(fsuae_inputport.joystick_port_0, 0);

    fsemu_inputport_set_mode_index(fsuae_inputport.joystick_port_1,
                                   FSUAE_JOYSTICK_PORT_MODE_JOYSTICK);

    fsemu_inputport_set_mode_index(fsuae_inputport.joystick_port_0,
                                   FSUAE_JOYSTICK_PORT_MODE_MOUSE);

    // int action = INPUTEVENT_AMIGA_JOYPORT_MODE_0_NONE + modes * i +
    //                 port->new_mode;
    // fs_emu_queue_action(ACTION_PORT0_MOUSEMODE, 1);
    // fs_emu_queue_action(ACTION_PORT0_MOUSEMODE, 0);
    // fsemu_action_queue(ACTION_PORT0_MOUSEMODE, 1)

    // FSEMU port 0 is Amiga port 1 - set to joystick mode (2) by default.
    fsemu_input_process_action(FSEMU_ACTION_PORT0TYPE2, 1);
    // FSEMU port 1 is Amiga port 0 - set to mouse mode (1) by default.
    fsemu_input_process_action(FSEMU_ACTION_PORT1TYPE1, 1);

    // FIXME: Remove
    // fsemu_input_process_action(ACTION_PORT0_MOUSEMODE, 1);
}
