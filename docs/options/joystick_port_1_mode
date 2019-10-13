Summary: Joystick Port 1
Default: joystick
Example: joystick

Specify what emulated Amiga device is connected to joystick port 1. If not
specified, this value will be automatically chosen based on the emulated
Amiga model and the host device associated with the joystick port.
Valid values are:

* joystick
* mouse
* cd32 gamepad
* nothing

Code:

    if c.joystick_port_1_mode.explicit:
        value = c.joystick_port_1_mode.explicit
    elif c.amiga_model == "CD32":
        value = "cd32 gamepad"
    else:
        # FIXME: depends on actual device in joystick_port_1...
        value = "joystick"
