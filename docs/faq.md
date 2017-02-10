# FAQ

This page contains answers to some frequently asked questions.

## Can I donate towards FS-UAE development?

You do not have to, but I have put up a donation page if you want to:
[https://fs-uae.net/donate](https://fs-uae.net/donate)  :)

## Do joysticks work with FS-UAE on Fedora Linux?

Yes they do, but from Fedora 17 and onwards, you need to install an extra
package to get joysticks working (at all, this is not specific to FS-UAE):

    sudo yum install kernel-modules-extra

## Can FS-UAE on MacOS launch on a separate fullscreen space?

Yes, you can force FS-UAE to open in its own fullscreen space by adding this
to advanced settings in the Launcher:

    SDL_VIDEO_MAC_FULLSCREEN_SPACES = 1

> **Note:** There be may usability issues when FS-UAE quits and tries to
> return to the Launcher/Arcade when fullscreen spaces are used.

## Can I use more than one keyboard-emulated joystick?

Yes, it is possible, but a bit awkward to setup. See [keyboard](keyboard.md)
for more information.

**FIXME: There is a new, better way to add extra keyboard-emulated joysticks.
Needs documentation!**

## Why does FS-UAE crash or disappear on startup?

This is usually caused by a driver problem or having conflicting software
installed. Please see [windows.md](Windows Notes) for solutions to some
problems which has been reported, your problem might be mentioned there!

## Why are the cursor keys not working?

If you start with the default settings and without a (supported) joystick
connected, the cursor keys will be used to emulate an Amiga joystick and
will therefore not function as Amiga cursor keys. You can either connect
a joystick to your computer, or disable the joystick emulation:

    joystick_port_1 = nothing

You can also do this from the GUI menu (F12) and in FS-UAE Launcher.

## Why don’t I get the normal kickstart when starting FS-UAE?

This happens when FS-UAE is unable to find the find the proper kickstart for
the Amiga model your trying to emulate and defaults back to the built-in AROS
kickstart. Please verify that your configuration correctly points to a valid
kickstart file.

## Why isn’t my gamepad or joystick working?

Make sure it’s plugged in and connected before starting FS-UAE. If it’s still
not working, read the documentation on on how to configure your controller,
you may have to create a configuration file for it. This is easily done for
you via FS-UAE Launcher -> Settings -> Joysticks & Gamepads. Just
double-click on your device to start configuring it.

## What can I do if I cannot get F12 to work for opening the menu?

If you've tried combining <kbd>F12</kbd> with <kbd>Fn</kbd> and/or
<kbd>Cmd</kbd>, and it still does not work, you can either:

* Bind the menu action to another key combination, or
* Toggle the menu with a controller (menu button or start + select together).

To bind the menu action to a key combination, you can add the following
to advanced settings:

    keyboard_key_mod_m = action_menu

Then you should be able to use <kbd>Mod+M</kbd> to open/close the menu.
<kbd>Mod</kbd> means <kbd>Cmd</kbd> on Macs and <kbd>Alt</kbd> on other
computers.

## In what directory must screenshots for FS-UAE Launcher be saved?

See the documentation page for FS-UAE Launcher.

## Will FS-UAE be ported to … ?

I have received several questions regarding whether I will port FS-UAE to
other systems, including:

* Android
* iOS
* AROS / MorphOs / AmigaOS
* Windows RT (Microsoft Surface)
* Pandora

I am sure porting to many of these systems could be an interesting challenge,
but in the end, it comes down to this: I can choose to spend (much) time on
such ports, or I can spend time improving FS-UAE for the already supported
platforms (Windows, macOS and Linux). I have many features I still want to
implemented, so I think my time is better spent improving FS-UAE and
developing related software for these platforms.

But because I have Android devices myself, and will likely continue to use
Android in years to come, a future Android port is not unlikely :)

I am not interested in developing FS-UAE for closed systems such as iOS and
Windows RT, where the only way to install software is via the official
application store, and where Apple and Microsoft, respectively, unilaterally
decides whether to allow software distribution or not.

## Why is Zorro III RAM not recognized?

The Zorro III memory size setting is only respected when emulating a CPU
with a 32-bit address bus, for instance by using the A1200/020 or the
A4000/040 model.

    amiga_model = A1200/020

Or you can override the CPU only, for example with:

    cpu = 68020

## How do I use Action Replay with FS-UAE?

** FIXME: THIS INFORMATION IS OUTDATED **

To use action replay with FS-UAE, you need an action replay rom file and you
need to configure a key or button to activate the cartridge. Example:

uae_cart_file = full/path/to/rom/file
The default shortcut to invoke the cartridge is Alt/Cmd+A (or F12+A).
