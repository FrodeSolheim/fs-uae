Summary: Joystick Port 0
Default: mouse
Example: joystick

Specify what emulated Amiga device is connected to joystick port 0. If not
specified, this value will be automatically chosen based on the emulated
Amiga model and the host device associated with the joystick port.
Valid values are:

* joystick
* mouse
* cd32 gamepad
* nothing

See also [joystick_port_0_mode].

Code:

    if c.joystick_port_0_mode.explicit:
        value = c.joystick_port_0_mode.explicit
    elif c.amiga_model == "CD32":
        value = "cd32 gamepad"
    else:
        # FIXME: depends on actual device in joystick_port_0...
        value = "mouse"
