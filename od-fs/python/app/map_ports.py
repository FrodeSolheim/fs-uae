import logging
from typing import Any

import _fsemu_input  # type: ignore
import _fsemu_inputport  # type: ignore
import fsemu
from fsemu.inputport import InputPort
from fsuae.servicecontainer import ServiceContainer
from uae.inputevent import InputEvent

logger = logging.getLogger(__name__)

joystick_port_1: Any = None
joystick_port_1_none: Any = None
joystick_port_1_mouse: Any = None
joystick_port_1_joystick: Any = None
joystick_port_0: Any = None
joystick_port_0_none: Any = None
joystick_port_0_mouse: Any = None
joystick_port_0_joystick: Any = None

joystick_port_2: Any = None
joystick_port_2_none: Any = None
joystick_port_3: Any = None
joystick_port_3_none: Any = None


def map_ports(services: ServiceContainer):
    # import uae

    # import fsuae

    # import fsemu.constants

    global joystick_port_1
    global joystick_port_1_none
    global joystick_port_1_mouse
    global joystick_port_1_joystick

    global joystick_port_0
    global joystick_port_0_none
    global joystick_port_0_mouse
    global joystick_port_0_joystick

    global joystick_port_2
    global joystick_port_2_none

    global joystick_port_3
    global joystick_port_3_none

    # The main thread will push input events to the inputport system, while the python
    # thread here will/might change settings. We might need mutexes to protect the
    # ports, or make sure that the actual changes are performed on the main thread...
    # This is not a big problem if the ports are set up during init (only) and not
    # subsequently modified...

    # Implement a (synchronous) run_in_main which waits until the main thread is
    # available, runs a (short) function there and continues? This should not slow
    # down the main thread significantly since the main thread does not have to wait
    # for the GIL...

    joystick_port_1 = InputPort("Joystick Port 1")
    joystick_port_1_none = joystick_port_1.create_mode("None")
    joystick_port_1_joystick = joystick_port_1.create_mode("Joystick")
    joystick_port_1_mouse = joystick_port_1.create_mode("Mouse")
    services.input_ports.add_port(joystick_port_1)

    joystick_port_0 = InputPort("Joystick Port 0")
    joystick_port_0_none = joystick_port_0.create_mode("None")
    joystick_port_0_joystick = joystick_port_0.create_mode("Joystick")
    joystick_port_0_mouse = joystick_port_0.create_mode("Mouse")
    services.input_ports.add_port(joystick_port_0)

    joystick_port_2 = InputPort("Joystick Port 2")
    joystick_port_2_none = joystick_port_2.create_mode("None")
    services.input_ports.add_port(joystick_port_2)

    joystick_port_3 = InputPort("Joystick Port 3")
    joystick_port_3_none = joystick_port_3.create_mode("None")
    services.input_ports.add_port(joystick_port_3)

    # joystick_port_1 = fsemu_inputport.new()
    # fsemu_inputport.set_name(joystick_port_1, "Joystick Port 1")
    # fsemu_input.add_port(joystick_port_1)

    # # FIXME: Crash if these two are removed (out of bounds - index check missing?)
    # joystick_port_1_none = fsemu_c.inputmode_new()
    # fsemu_c.inputmode_set_name(joystick_port_1_none, "None")
    # fsemu_inputport.add_mode(joystick_port_1, joystick_port_1_none)

    # joystick_port_1_joystick = fsemu_c.inputmode_new()
    # fsemu_c.inputmode_set_name(joystick_port_1_joystick, "Joystick")
    # fsemu_inputport.add_mode(joystick_port_1, joystick_port_1_joystick)

    # joystick_port_1_mouse = fsemu_c.inputmode_new()
    # fsemu_c.inputmode_set_name(joystick_port_1_mouse, "Mouse")
    # fsemu_inputport.add_mode(joystick_port_1, joystick_port_1_mouse)

    # joystick_port_0 = fsemu_inputport.new()
    # fsemu_inputport.set_name(joystick_port_0, "Joystick Port 0")
    # fsemu_input.add_port(joystick_port_0)

    # joystick_port_0_none = fsemu_c.inputmode_new()
    # fsemu_c.inputmode_set_name(joystick_port_0_none, "None")
    # fsemu_inputport.add_mode(joystick_port_0, joystick_port_0_none)

    # joystick_port_0_joystick = fsemu_c.inputmode_new()
    # fsemu_c.inputmode_set_name(joystick_port_0_joystick, "Joystick")
    # fsemu_inputport.add_mode(joystick_port_0, joystick_port_0_joystick)

    # joystick_port_0_mouse = fsemu_c.inputmode_new()
    # fsemu_c.inputmode_set_name(joystick_port_0_mouse, "Mouse")
    # fsemu_inputport.add_mode(joystick_port_0, joystick_port_0_mouse)

    for trigger, action in [
        (fsemu.INPUTDEVICE_MOUSEYNEG, InputEvent.JOYPORT0_MOUSEYNEG),
        (fsemu.INPUTDEVICE_MOUSEXPOS, InputEvent.JOYPORT0_MOUSEXPOS),
        (fsemu.INPUTDEVICE_MOUSEYPOS, InputEvent.JOYPORT0_MOUSEYPOS),
        (fsemu.INPUTDEVICE_MOUSEXNEG, InputEvent.JOYPORT0_MOUSEXNEG),
        (fsemu.INPUTDEVICE_LEFTMOUSE, InputEvent.JOY1_FIRE_BUTTON),
        (fsemu.INPUTDEVICE_RIGHTMOUSE, InputEvent.JOY1_2ND_BUTTON),
    ]:
        # fsemu_c.inputmode_map(joystick_port_0_mouse, trigger, action)
        joystick_port_0_mouse.map(trigger, action)

    for trigger, action in [
        # MAP(UP, ACTION_JOYSTICK1_UP);
        # MAP(RIGHT, ACTION_JOYSTICK1_RIGHT);
        # MAP(DOWN, ACTION_JOYSTICK1_DOWN);
        # MAP(LEFT, ACTION_JOYSTICK1_LEFT);
        # MAP(RCTRL, ACTION_JOYSTICK1_FIRE);
        # MAP(RALT, ACTION_JOYSTICK1_FIRE);
        (fsemu.INPUTDEVICE_KEY_UP, InputEvent.JOY2_UP),
        (fsemu.INPUTDEVICE_KEY_RIGHT, InputEvent.JOY2_RIGHT),
        (fsemu.INPUTDEVICE_KEY_DOWN, InputEvent.JOY2_DOWN),
        (fsemu.INPUTDEVICE_KEY_LEFT, InputEvent.JOY2_LEFT),
        (fsemu.INPUTDEVICE_KEY_RCTRL, InputEvent.JOY2_FIRE_BUTTON),
        # (fsemu.INPUTDEVICE_KEY_RALT, InputEvent.JOY2_2ND_BUTTON),
        # mac hasn't got right ctrl - FIXME...
        (fsemu.INPUTDEVICE_KEY_RALT, InputEvent.JOY2_FIRE_BUTTON),
        (fsemu.INPUTDEVICE_KEY_RSHIFT, InputEvent.JOY2_2ND_BUTTON),
        (fsemu.INPUTDEVICE_BUTTON_DPAD_LEFT, InputEvent.JOY2_LEFT),
        (fsemu.INPUTDEVICE_BUTTON_DPAD_RIGHT, InputEvent.JOY2_RIGHT),
        (fsemu.INPUTDEVICE_BUTTON_DPAD_UP, InputEvent.JOY2_UP),
        (fsemu.INPUTDEVICE_BUTTON_DPAD_DOWN, InputEvent.JOY2_DOWN),
        (fsemu.INPUTDEVICE_LEFTXNEG, InputEvent.JOY2_LEFT),
        (fsemu.INPUTDEVICE_LEFTXPOS, InputEvent.JOY2_RIGHT),
        (fsemu.INPUTDEVICE_LEFTYNEG, InputEvent.JOY2_UP),
        (fsemu.INPUTDEVICE_LEFTYPOS, InputEvent.JOY2_DOWN),
        (fsemu.INPUTDEVICE_BUTTON_SOUTH, InputEvent.JOY2_FIRE_BUTTON),
        (fsemu.INPUTDEVICE_BUTTON_EAST, InputEvent.JOY2_UP),
        # ??? Should be right trigger or right bumper?
        # (fsemu.INPUTDEVICE_BUTTON_RIGHTSTICK, InputEvent.JOY2_FIRE_BUTTON),
    ]:
        # fsemu_c.inputmode_map(joystick_port_1_joystick, trigger, action)
        joystick_port_1_joystick.map(trigger, action)

    # if (num == 1) {
    #     MAP(DPUP, ACTION_MOUSE1_UP);
    #     MAP(DPRIGHT, ACTION_MOUSE1_RIGHT);
    #     MAP(DPDOWN, ACTION_MOUSE1_DOWN);
    #     MAP(DPLEFT, ACTION_MOUSE1_LEFT);
    #     MAP(BUTTON_A, ACTION_JOYSTICK1_FIRE);
    #     MAP(BUTTON_B, ACTION_MOUSE1_UP);
    #     // MAP(RIGHTSTICK, ACTION_MOUSE0_FIRE);

    #     MAP(MOUSEYNEG, ACTION_MOUSE1_UP);
    #     MAP(MOUSEXPOS, ACTION_MOUSE1_RIGHT);
    #     MAP(MOUSEYPOS, ACTION_MOUSE1_DOWN);
    #     MAP(MOUSEXNEG, ACTION_MOUSE1_LEFT);
    #     MAP(LEFTMOUSE, ACTION_JOYSTICK1_FIRE);
    #     MAP(RIGHTMOUSE, ACTION_JOYSTICK1_BUTTON2);
    # } else {
    #     MAP(DPUP, ACTION_MOUSE0_UP);
    #     MAP(DPRIGHT, ACTION_MOUSE0_RIGHT);
    #     MAP(DPDOWN, ACTION_MOUSE0_DOWN);
    #     MAP(DPLEFT, ACTION_MOUSE0_LEFT);
    #     MAP(BUTTON_A, ACTION_JOYSTICK0_FIRE);
    #     MAP(BUTTON_B, ACTION_MOUSE0_UP);

    #     MAP(MOUSEYNEG, ACTION_MOUSE0_UP);
    #     MAP(MOUSEXPOS, ACTION_MOUSE0_RIGHT);
    #     MAP(MOUSEYPOS, ACTION_MOUSE0_DOWN);
    #     MAP(MOUSEXNEG, ACTION_MOUSE0_LEFT);
    #     MAP(LEFTMOUSE, ACTION_JOYSTICK0_FIRE);
    #     MAP(RIGHTMOUSE, ACTION_JOYSTICK0_BUTTON2);
    # }

    # import fsemu_inputdevice  # type: ignore

    # global keyboard_device
    # global mouse_device

    # By convention, the default keyboard is always the first device (index 0)
    # keyboard_device = fsemu_inputdevice.create()
    # fsemu_inputdevice.set_name(keyboard_device, "Keyboard")
    # fsemu_inputdevice.set_type(keyboard_device, fsemu.INPUTDEVICE_TYPE_KEYBOARD)
    # fsemu_input.add_device(keyboard_device)

    # By convention, the default mouse is always the second device (index 1)
    # mouse_device = fsemu_inputdevice.create()
    # fsemu_inputdevice.set_name(mouse_device, "Mouse")
    # fsemu_inputdevice.set_type(mouse_device, fsemu.INPUTDEVICE_TYPE_MOUSE)
    # fsemu_input.add_device(mouse_device)

    # fsuae_inputport.joystick_port_1 = fsemu_inputport_new();
    # configure_joystick_port(fsuae_inputport.joystick_port_1, 1);
    # fsuae_inputport.joystick_port_0 = fsemu_inputport_new();
    # configure_joystick_port(fsuae_inputport.joystick_port_0, 0);

    # fsemu_inputport_set_mode_by_index(fsuae_inputport.joystick_port_1,
    #                                FSUAE_JOYSTICK_PORT_MODE_JOYSTICK);

    # fsemu_inputport_set_mode_by_index(fsuae_inputport.joystick_port_0,
    #                                FSUAE_JOYSTICK_PORT_MODE_MOUSE);

    # Setting mode index does not reconfigure input?

    # FIXME: Check functionality of set_mode_by_index vs ACTION_PORTXTYPEY
    # FIXME: Mouse mode or joystick mode first in the list of modes??
    # Maybe joystick mode

    logger.warning("NOT SETTING PORT MODES...!!!")
    _fsemu_inputport.set_mode_by_index(joystick_port_1._port, 1)  # type: ignore
    _fsemu_inputport.set_mode_by_index(joystick_port_0._port, 2)  # type: ignore

    # # # FSEMU port 0 is Amiga port 1 - set to joystick mode (2) by default
    # fsemu.post(fsemu.ACTION_PORT0TYPE2)
    # # # FSEMU port 1 is Amiga port 0 - set to mouse mode (1) by default
    # fsemu.post(fsemu.ACTION_PORT1TYPE1)

    # fsemu_inputport.set_device(joystick_port_1, keyboard_device)
    # fsemu_inputport.set_device(joystick_port_0, mouse_device)

    # fsemu_inputport.set_device_by_index(joystick_port_1, 0)

    # Handled by InputDeviceRoboService instead
    # _fsemu_inputport.set_device_by_index(joystick_port_1._port, 0)
    # _fsemu_inputport.set_device_by_index(joystick_port_0._port, 1)

    keyboard_mapping: list[tuple[int, int]] = [
        (fsemu.INPUTDEVICE_KEY_ESCAPE, InputEvent.KEY_ESC),
        (fsemu.INPUTDEVICE_KEY_F1, InputEvent.KEY_F1),
        (fsemu.INPUTDEVICE_KEY_F2, InputEvent.KEY_F2),
        (fsemu.INPUTDEVICE_KEY_F3, InputEvent.KEY_F3),
        (fsemu.INPUTDEVICE_KEY_F4, InputEvent.KEY_F4),
        (fsemu.INPUTDEVICE_KEY_F5, InputEvent.KEY_F5),
        (fsemu.INPUTDEVICE_KEY_F6, InputEvent.KEY_F6),
        (fsemu.INPUTDEVICE_KEY_F7, InputEvent.KEY_F7),
        (fsemu.INPUTDEVICE_KEY_F8, InputEvent.KEY_F8),
        (fsemu.INPUTDEVICE_KEY_F9, InputEvent.KEY_F9),
        (fsemu.INPUTDEVICE_KEY_F10, InputEvent.KEY_F10),
        # F11
        # F12
        # Print screen
        # Scroll lock
        # Pause/break
        # ------------------------------------------------------------------------------------------
        (fsemu.INPUTDEVICE_KEY_GRAVE, InputEvent.KEY_BACKQUOTE ),
        (fsemu.INPUTDEVICE_KEY_1, InputEvent.KEY_1),
        (fsemu.INPUTDEVICE_KEY_2, InputEvent.KEY_2),
        (fsemu.INPUTDEVICE_KEY_3, InputEvent.KEY_3),
        (fsemu.INPUTDEVICE_KEY_4, InputEvent.KEY_4),
        (fsemu.INPUTDEVICE_KEY_5, InputEvent.KEY_5),
        (fsemu.INPUTDEVICE_KEY_6, InputEvent.KEY_6),
        (fsemu.INPUTDEVICE_KEY_7, InputEvent.KEY_7),
        (fsemu.INPUTDEVICE_KEY_8, InputEvent.KEY_8),
        (fsemu.INPUTDEVICE_KEY_9, InputEvent.KEY_9),
        (fsemu.INPUTDEVICE_KEY_0, InputEvent.KEY_0),
        (fsemu.INPUTDEVICE_KEY_MINUS, InputEvent.KEY_SUB),
        (fsemu.INPUTDEVICE_KEY_EQUALS, InputEvent.KEY_EQUALS),
        (fsemu.INPUTDEVICE_KEY_BACKSPACE, InputEvent.KEY_BACKSPACE),
        (fsemu.INPUTDEVICE_KEY_INSERT, InputEvent.KEY_2B),
        (fsemu.INPUTDEVICE_KEY_HOME, InputEvent.KEY_NP_LPAREN),
        (fsemu.INPUTDEVICE_KEY_PAGEUP, InputEvent.KEY_NP_RPAREN),
        # Num lock
        (fsemu.INPUTDEVICE_KEY_KP_DIVIDE, InputEvent.KEY_NP_DIV),
        (fsemu.INPUTDEVICE_KEY_KP_MULTIPLY, InputEvent.KEY_NP_MUL),
        (fsemu.INPUTDEVICE_KEY_KP_MINUS, InputEvent.KEY_NP_SUB),
        # ------------------------------------------------------------------------------------------
        (fsemu.INPUTDEVICE_KEY_TAB, InputEvent.KEY_TAB),
        (fsemu.INPUTDEVICE_KEY_Q, InputEvent.KEY_Q),
        (fsemu.INPUTDEVICE_KEY_W, InputEvent.KEY_W),
        (fsemu.INPUTDEVICE_KEY_E, InputEvent.KEY_E),
        (fsemu.INPUTDEVICE_KEY_R, InputEvent.KEY_R),
        (fsemu.INPUTDEVICE_KEY_T, InputEvent.KEY_T),
        (fsemu.INPUTDEVICE_KEY_Y, InputEvent.KEY_Y),
        (fsemu.INPUTDEVICE_KEY_U, InputEvent.KEY_U),
        (fsemu.INPUTDEVICE_KEY_I, InputEvent.KEY_I),
        (fsemu.INPUTDEVICE_KEY_O, InputEvent.KEY_O),
        (fsemu.INPUTDEVICE_KEY_P, InputEvent.KEY_P),
        (fsemu.INPUTDEVICE_KEY_LEFTBRACKET, InputEvent.KEY_LEFTBRACKET),
        (fsemu.INPUTDEVICE_KEY_RIGHTBRACKET, InputEvent.KEY_RIGHTBRACKET),
        (fsemu.INPUTDEVICE_KEY_RETURN, InputEvent.KEY_RETURN),
        (fsemu.INPUTDEVICE_KEY_DELETE, InputEvent.KEY_DEL),
        (fsemu.INPUTDEVICE_KEY_END, InputEvent.KEY_HELP),
        (fsemu.INPUTDEVICE_KEY_PAGEDOWN, InputEvent.KEY_AMIGA_RIGHT),
        (fsemu.INPUTDEVICE_KEY_KP_7, InputEvent.KEY_NP_7),
        (fsemu.INPUTDEVICE_KEY_KP_8, InputEvent.KEY_NP_8),
        (fsemu.INPUTDEVICE_KEY_KP_9, InputEvent.KEY_NP_9),
        (fsemu.INPUTDEVICE_KEY_KP_PLUS, InputEvent.KEY_NP_ADD),
        # ------------------------------------------------------------------------------------------
        # FIXME: ID_FLAG_TOGGLE
        (fsemu.INPUTDEVICE_KEY_CAPSLOCK, InputEvent.KEY_CAPS_LOCK),
        (fsemu.INPUTDEVICE_KEY_A, InputEvent.KEY_A),
        (fsemu.INPUTDEVICE_KEY_S, InputEvent.KEY_S),
        (fsemu.INPUTDEVICE_KEY_D, InputEvent.KEY_D),
        (fsemu.INPUTDEVICE_KEY_F, InputEvent.KEY_F),
        (fsemu.INPUTDEVICE_KEY_G, InputEvent.KEY_G),
        (fsemu.INPUTDEVICE_KEY_H, InputEvent.KEY_H),
        (fsemu.INPUTDEVICE_KEY_J, InputEvent.KEY_J),
        (fsemu.INPUTDEVICE_KEY_K, InputEvent.KEY_K),
        (fsemu.INPUTDEVICE_KEY_L, InputEvent.KEY_L),
        (fsemu.INPUTDEVICE_KEY_SEMICOLON, InputEvent.KEY_SEMICOLON),
        (fsemu.INPUTDEVICE_KEY_APOSTROPHE, InputEvent.KEY_SINGLEQUOTE),
        (fsemu.INPUTDEVICE_KEY_BACKSLASH, InputEvent.KEY_BACKSLASH),
        (fsemu.INPUTDEVICE_KEY_KP_4, InputEvent.KEY_NP_4),
        (fsemu.INPUTDEVICE_KEY_KP_5, InputEvent.KEY_NP_5),
        (fsemu.INPUTDEVICE_KEY_KP_6, InputEvent.KEY_NP_6),
        # ------------------------------------------------------------------------------------------
        (fsemu.INPUTDEVICE_KEY_LSHIFT, InputEvent.KEY_SHIFT_LEFT),
        (fsemu.INPUTDEVICE_KEY_NONUSBACKSLASH, InputEvent.KEY_30 ),
        (fsemu.INPUTDEVICE_KEY_Z, InputEvent.KEY_Z),
        (fsemu.INPUTDEVICE_KEY_X, InputEvent.KEY_X),
        (fsemu.INPUTDEVICE_KEY_C, InputEvent.KEY_C),
        (fsemu.INPUTDEVICE_KEY_V, InputEvent.KEY_V),
        (fsemu.INPUTDEVICE_KEY_B, InputEvent.KEY_B),
        (fsemu.INPUTDEVICE_KEY_N, InputEvent.KEY_N),
        (fsemu.INPUTDEVICE_KEY_M, InputEvent.KEY_M),
        (fsemu.INPUTDEVICE_KEY_COMMA, InputEvent.KEY_COMMA),
        (fsemu.INPUTDEVICE_KEY_PERIOD, InputEvent.KEY_PERIOD),
        (fsemu.INPUTDEVICE_KEY_SLASH, InputEvent.KEY_DIV),
        (fsemu.INPUTDEVICE_KEY_RSHIFT, InputEvent.KEY_SHIFT_RIGHT),
        (fsemu.INPUTDEVICE_KEY_UP, InputEvent.KEY_CURSOR_UP),
        (fsemu.INPUTDEVICE_KEY_KP_1, InputEvent.KEY_NP_1),
        (fsemu.INPUTDEVICE_KEY_KP_2, InputEvent.KEY_NP_2),
        (fsemu.INPUTDEVICE_KEY_KP_3, InputEvent.KEY_NP_3),
        (fsemu.INPUTDEVICE_KEY_KP_ENTER, InputEvent.KEY_ENTER),
        # ------------------------------------------------------------------------------------------
        (fsemu.INPUTDEVICE_KEY_LCTRL, InputEvent.KEY_CTRL),
        (fsemu.INPUTDEVICE_KEY_LGUI, InputEvent.KEY_AMIGA_LEFT),
        (fsemu.INPUTDEVICE_KEY_LALT, InputEvent.KEY_ALT_LEFT),
        (fsemu.INPUTDEVICE_KEY_SPACE, InputEvent.KEY_SPACE),
        (fsemu.INPUTDEVICE_KEY_RALT, InputEvent.KEY_ALT_RIGHT),
        (fsemu.INPUTDEVICE_KEY_RGUI, InputEvent.KEY_AMIGA_RIGHT),
        (fsemu.INPUTDEVICE_KEY_APPLICATION, InputEvent.KEY_AMIGA_RIGHT),
        # Many keyboards lack right super/gui/menu whatever, but have right ctrl, so it makes
        # sense to map that to amiga right.
        (fsemu.INPUTDEVICE_KEY_RCTRL, InputEvent.KEY_AMIGA_RIGHT),
        (fsemu.INPUTDEVICE_KEY_LEFT, InputEvent.KEY_CURSOR_LEFT),
        (fsemu.INPUTDEVICE_KEY_DOWN, InputEvent.KEY_CURSOR_DOWN),
        (fsemu.INPUTDEVICE_KEY_RIGHT, InputEvent.KEY_CURSOR_RIGHT),
        (fsemu.INPUTDEVICE_KEY_KP_0, InputEvent.KEY_NP_0),
        (fsemu.INPUTDEVICE_KEY_KP_PERIOD, InputEvent.KEY_NP_PERIOD),
        # ------------------------------------------------------------------------------------------
        # (     { FSEMU_KEYBOARD_APOSTROPH),
        #       ALL,
        #       ACTION_KEY_HASH },  // FIXME: ACTION_KEY_SINGLEQUOTE ?
        #     /* FIXME */
        # #if 0
        #     { FS_ML_KEY_LMETA,        ALL, INPUTEVENT_KEY_AMIGA_LEFT },
        # #endif
        #     /* FIXME */
        # #if 0
        #     { FS_ML_KEY_RMETA,        ALL, INPUTEVENT_KEY_AMIGA_RIGHT },
        # #endif
        # #if 0
        # #endif
        # #if 1
        #     // 0x2B is the number (hash) key on some international keyboards.
        # #else
        #     { FSEMU_KEYBOARD_BACKSLASH, ALL, ACTION_KEY_2B },
        # #endif
        #     // 0x30 is the key to the left of Z.
        #     { FSEMU_KEYBOARD_NONUSBACKSLASH, ALL, ACTION_KEY_30 },
        #     {
        #         0,
        #         0,
        #         0,
        #     },
    ]
    _fsemu_input.map_keyboard(keyboard_mapping)  # type: ignore
