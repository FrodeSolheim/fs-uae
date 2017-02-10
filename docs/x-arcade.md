# X-Arcade Controller

If you have an X-Arcade controller, you need to install some special config
files in order for it to be recognized. This is because the X-Arcade device
appears as a keyboard, and not a joystick device.

Copy the three files from docs/x-arcade into `FS-UAE/Data/Devs/Keyboards`
in order to:

* Tell FS-UAE about the controllers, and how the keys are mapped.
* Disable the default Keyboard device, as this will otherwise conflict with
  the X-Arcade controllers.

If you do not have the dual X-Arcade controller, you should only copy
X-Arcade.ini and Keyboard.disabled.

Please note that one of the buttons is mapped to the <kbd>Alt</kbd> key,
which can interfere with emulators.
Using the alternative X-Arcade layout from docs/x-arcade-fs is recommended!
This requires you to remap the X-Arcade controller (see X-Arcade
documentation for information about this).
Alternatively, you can enable full keyboard emulation, or use the
modifier_key to specify a key *not* in use by X-Arcade as a modifier key.
But note that the <kbd>Alt</kbd> key may still be used by some other
emulators if you use the multi-platform support in FS-UAE Launcher/Arcade.
