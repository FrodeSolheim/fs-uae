#include "fsuae-action.h"
#include "fsuae.h"

static struct {
    fsemu_inputport_t *joystick_port_1;
    fsemu_inputport_t *joystick_port_0;
    int selected_state_slot;
} fsuae_inputport;

static void configure_joystick_port(fsemu_inputport_t *port, int num)
{
    if (num == 1) {
        fsemu_inputport_set_name(port, "Joystick Port 1");
        // base = ACTION_JOYSTICK1_BEFOREFIRST;
    } else {
        fsemu_inputport_set_name(port, "Joystick Port 0");
        // base = ACTION_JOYSTICK2_BEFOREFIRST;
    }

    fsemu_inputmode_t *mode;
    // int base;

    // fsemu_inputmode_map_f map = fsemu_inputmode_map;
    // int key = 1;
    // int ctl = 2;

    mode = fsemu_inputmode_new();
    fsemu_inputmode_set_name(mode, "Joystick");
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

#define MAP(input, output) \
    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_##input, output)

    if (num == 1) {
        MAP(DPUP, ACTION_JOYSTICK1_UP);
        MAP(DPRIGHT, ACTION_JOYSTICK1_RIGHT);
        MAP(DPDOWN, ACTION_JOYSTICK1_DOWN);
        MAP(DPLEFT, ACTION_JOYSTICK1_LEFT);
        MAP(SOUTH, ACTION_JOYSTICK1_FIRE);
        MAP(EAST, ACTION_JOYSTICK1_UP);
        MAP(RIGHTSTICK, ACTION_JOYSTICK0_FIRE);
    } else {
        MAP(DPUP, ACTION_JOYSTICK0_UP);
        MAP(DPRIGHT, ACTION_JOYSTICK0_RIGHT);
        MAP(DPDOWN, ACTION_JOYSTICK0_DOWN);
        MAP(DPLEFT, ACTION_JOYSTICK0_LEFT);
        MAP(SOUTH, ACTION_JOYSTICK0_FIRE);
        MAP(EAST, ACTION_JOYSTICK0_UP);
    }

    MAP(GUIDE, FSEMU_ACTION_OSMENU);
    MAP(START, FSEMU_ACTION_OSMENU);
    MAP(BACK, FSEMU_ACTION_OSKEYBOARD);

    fsemu_input_add_port(port);

    mode = fsemu_inputmode_new();
    fsemu_inputmode_set_name(mode, "Mouse");
    fsemu_inputport_add_mode(port, mode);

    if (num == 1) {
        MAP(DPUP, ACTION_MOUSE1_UP);
        MAP(DPRIGHT, ACTION_MOUSE1_RIGHT);
        MAP(DPDOWN, ACTION_MOUSE1_DOWN);
        MAP(DPLEFT, ACTION_MOUSE1_LEFT);
        MAP(SOUTH, ACTION_JOYSTICK1_FIRE);
        MAP(EAST, ACTION_MOUSE1_UP);
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
        MAP(SOUTH, ACTION_JOYSTICK0_FIRE);
        MAP(EAST, ACTION_MOUSE0_UP);

        MAP(MOUSEYNEG, ACTION_MOUSE0_UP);
        MAP(MOUSEXPOS, ACTION_MOUSE0_RIGHT);
        MAP(MOUSEYPOS, ACTION_MOUSE0_DOWN);
        MAP(MOUSEXNEG, ACTION_MOUSE0_LEFT);
        MAP(LEFTMOUSE, ACTION_JOYSTICK0_FIRE);
        MAP(RIGHTMOUSE, ACTION_JOYSTICK0_BUTTON2);
    }
}

#define FSUAE_JOYSTICK_PORT_MODE_MOUSE 1

void fsuae_inputport_init(void)
{
    fsuae_action_init();

    fsuae_inputport.joystick_port_1 = fsemu_inputport_new();
    configure_joystick_port(fsuae_inputport.joystick_port_1, 1);
    fsuae_inputport.joystick_port_0 = fsemu_inputport_new();
    configure_joystick_port(fsuae_inputport.joystick_port_0, 0);

    fsemu_inputport_set_mode_index(fsuae_inputport.joystick_port_0,
                                   FSUAE_JOYSTICK_PORT_MODE_MOUSE);

    // int action = INPUTEVENT_AMIGA_JOYPORT_MODE_0_NONE + modes * i +
    //                 port->new_mode;
    // fs_emu_queue_action(ACTION_PORT0_MOUSEMODE, 1);
    // fs_emu_queue_action(ACTION_PORT0_MOUSEMODE, 0);
    // fsemu_action_queue(ACTION_PORT0_MOUSEMODE, 1)
    fsemu_input_process_action(ACTION_PORT0_MOUSEMODE, 1);
}
