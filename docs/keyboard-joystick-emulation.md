# Keyboard joystick emulation

FIXME: This document must be updated with more information
FIXME: This document must updated with markdown formatting


When you emulate an Amiga joystick with your keyboard, the following keys are used to control the joystick:

    Cursor keys controls the joystick stick.
    Right Ctrl or Right Alt controls the fire button.

When the keys are used to emulate the joystick, they will not activate the corresponding Amiga keys while you use this feature. So to use all Amiga features at the same time, a connected real joystick or gamepad is recommended, though it is usually not a problem. Most joystick-controlled games do not require the keyboard cursor keys to be used also…

When playing CD32 games with the keyboard emulating a gamepad, additional keys are used to control the addition buttons on the gamepad:

    Cursor Keys – Controls the directional pad.
    C – Red
    X – Blue
    D – Green
    S – Yellow
    Return – Play
    Z – Rewind
    A – Forward
    Right Ctrl or Right Alt also controls the red button.

## Emulating a second joystick with the keyboard

FIXME: Missing documentation

## Disabling the keyboard joystick device

To disable the built-in joystick keyboard emulation, make sure the following
(empty) file exists: `FS-UAE/Data/Devs/Keyboard.fs-device-disabled`. You might
want to do this if you create other custom joystick keyboard emulation such as 
[x-arcade](x-arcade.md).
