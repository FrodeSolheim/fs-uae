# Mouse

## Middle Mouse Button

## Mouse Wheel

## Multiple Mice

FS-UAE supports multiple mice on many platforms through the ManyMouse library
by Ryan C. Gordon.

The old `Mouse` host device is still present and works like before - this
is basically the system cursor, and most likely, all connected mice will
affect this host device in FS-UAE.

If the support for multiple mice works on your system, you will have one
_additional_ specific host device for each mouse connected. So with two
Microsoft Intellimouse devices connected, you might see these devices:

- `Mouse` - This is controllable by both the physical mice
- `Microsoft Microsoft 5-Button Mouse with IntelliEye(TM)`
- `Microsoft Microsoft 5-Button Mouse with IntelliEye(TM)`

There might be some specific things you should be aware of, depending on your
operating system. Read on for more information!

## Mouse Speed and Acceleration

The old `Mouse` device is subject to acceleration settings, while
direct hardware access to specific mouse devices is not. You may therefore
want to use the new _specific mouse devices_ for more pleasant mouse
emulation regardless of the number of mice you have. You may even find that
there is less input lag with the new mouse devices.

## Windows and OS X

Support for multiple mice seems to work just fine on Windows out of the box.

## OS X

Support for multiple mice seems to work just fine on OS X out of the box.

### Mouse Buttons on OS X

## Linux

You may have to change permissions on some device nodes before starting
FS-UAE (depending on your Linux distribution). If FS-UAE does not have
permission to read from the devices, you will only get access to the
old/single `Mouse` host device.

You can run this simple command to easily enable FS-UAE to open the mouse
devices (_after_ the mice are connected), though remember
that this can have security implications if other people have remote access
to the computer:

    sudo chmod a+r /dev/input/*

A much better method is to tell the system to specifically make mouse event
devices readable when they are plugged in:

### Creating udev Rules for Readable Mice

Note that the following works perfectly for me on Ubuntu, and it is possible
you must do it slightly differently depending on your Linux distribution.
Hopefully the instructions apply to you regardless of your Linux
distribution :)

We need to create a config file with a udev rule to set read permissions
for all mice which are connected. From a terminal, type:

    sudo nano -w /etc/udev/rules.d/99-readable-mice.rules

Paste the following (on one line) in this file, and save it:

    ENV{ID_INPUT_MOUSE}=="?*", MODE="644"

On my system, this was all that was needed - just unplug the mouse/mice and
reconnect them for the changes to work.

To verify that it works, reconnect your mouse/mice if you haven't already
and type:

    ls -l /dev/input/mouse*

All files listed should now have read permissions for all users, something
like this:

    crw-r--r-- 1 root root 13, 32 aug.  20 17:30 /dev/input/mouse0
    crw-r--r-- 1 root root 13, 33 aug.  20 17:30 /dev/input/mouse1

If it did not work, you have to do additional steps to activate the rules.
Perhaps a command to reinitialize udev, such as as `sudo udevstart`,
`sudo /etc/init.d/udev restart`, `sudo udevcontrol reload_rules` or
`sudo <em>udevadm trigger --subsystem-match=input` and/or just reboot.

Try to google information about udev rules on your distribution if you
have problems.

### Evdev vs XInput2

The ManyMouse library really supports two drivers for Linux: XInput2 and
Linux evdev interface. Normally, ManyMouse tries XInput2 first, and if that
works, it will not use Linux evdev. In FS-UAE, I have reversed this order,
so it will always try evdev first (and thus probably never use XInput2).

The reason for this is that when XInput2 is used and mouse input is grabbed
by FS-UAE, no mouse events at all reach the ManyMouse library. It works fine
when input grab is released. With the evdev driver, it always works fine.

## FreeBSD / OpenBSD

Presumably, XInput2 should work well, with the caveat that input from
specific mice probably only works when FS-UAE does not have input grabbed
(See the Linux section for more information). You can use the option
[automatic_input_grab](options/automatic_input_grab.md) to prevent FS-UAE from
grabbing the input.

If you are using FreeBSD / OpenBSD and have tested this feature, please leave
feedback on fs-uae.net!
