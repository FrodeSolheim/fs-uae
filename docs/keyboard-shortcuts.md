# Keyboard shorcuts

The following keyboard shortcuts are available by default. <kbd>Mod</kbd> means
<kbd>Cmd</kbd> on OS X and <kbd>Alt</kbd> on other platforms, but it can be
re-configured as another key. See the "modifier key" section for more
information.

> **Note:** The shortcuts are mapped against keys on a English keyboard, so on
> international keyboards, the key (symbol) you have to press may in some cases
> be different (but same physical key).

- Use <kbd>F12</kbd> to enter the FS-UAE on-screen menu.
- Use <kbd>Mod</kbd>+<kbd>F</kbd> or <kbd>Mod</kbd>+<kbd>Return</kbd> to toggle
  full-screen mode.
- Use <kbd>Mod</kbd>+<kbd>Z</kbd> or <kbd>F11</kbd> to cycle through zoom
  modes.
- Use <kbd>Mod</kbd>+<kbd>B</kbd> to toggle extra visible border.
- Use <kbd>Mod</kbd>+<kbd>S</kbd> or <kbd>Print Scrn</kbd> to save a screenshot
  of the current Amiga frame to the desktop (or chosen screenshots location).
- Use <kbd>Mod</kbd>+<kbd>Q</kbd> or <kbd>Mod</kbd>+<kbd>F4</kbd> to quit.
- Use <kbd>Mod</kbd>+<kbd>D</kbd> to activate (console) debugger.

Input control:

- Use <kbd>Mod</kbd>+<kbd>G</kbd> to release or acquire the input grab. You can
  also use the middle mouse button, or enter the menu.
- Use <kbd>Mod</kbd>+<kbd>K</kbd> to enable full keyboard emulation. This
  disables the modifier key to make it usable in emulation. In order to disable
  full keyboard emulation you need to press <kbd>AltMod</kbd>+<kbd>K</kbd>.
  This will usually be <kbd>F12</kbd>+<kbd>K</kbd>. Please see the "alternative
  modifier keys" section.
- Use <kbd>Mod</kbd>+<kbd>Up</kbd> when you need to press Amiga cursor up and
  joystick emulation is enabled. Similar for <kbd>Mod</kbd>+<kbd>Down</kbd>,
  <kbd>Mod</kbd>+<kbd>Left</kbd> and <kbd>Mod</kbd>+<kbd>Right</kbd>.

Emulation control:

- Use <kbd>Mod</kbd>+<kbd>W</kbd> to toggle warp mode (no frame limit, no
  audio). This does not affect the emulation accuracy, it only removes the
  pauses between generating frames.
- Use <kbd>Mod</kbd>+<kbd>R</kbd> to perform a soft reset.
- Use <kbd>Mod</kbd>+<kbd>T</kbd> to perform a hard reset.
- Use <kbd>Mod</kbd>+<kbd>P</kbd> or <kbd>Pause</kbd> to toggle pause mode.
- Use <kbd>Mod</kbd>+<kbd>C</kbd> to activate cartridge (Action Replay, ...).

Volume control:

- Use <kbd>Mod</kbd>+<kbd>,</kbd> to decrease FS-UAE volume.
- Use <kbd>Mod</kbd>+<kbd>.</kbd> to increase FS-UAE volume.
- Use <kbd>Mod</kbd>+<kbd>M</kbd> to mute FS-UAE (press again to unmute).
- Use <kbd>Mod</kbd>+<kbd>N</kbd> to mute floppy sounds (press again to
  unmute).

Disk swapper:

- Use <kbd>Prev</kbd> (multimedia key) to go backward through the swap list.
- Use <kbd>Next</kbd> (multimedia key) to go forward through the swap list.
- Use <kbd>Play</kbd> (multimedia key) to insert the currently selected swap
  list entry into `DF0`.

RTG graphics control (shortcuts are subject to change)

- Use <kbd>Mod</kbd>+<kbd>F9</kbd> to toggle chipset/RTG display. ~~\* Use
  <kbd>Mod</kbd>+<kbd>1</kbd> to switch to chipset display.~~ ~~\* Use
  <kbd>Mod</kbd>+<kbd>2</kbd> to switch to RTG display (if any).~~

FIXME:

- Tab – toggle chat function.
- Ctrl+F10 – Toggle performance graphs / FPS display (There are two FPS numbers
  shown, SYS: actual video output frame rate. EMU: emulated video frame rate).
- Alt+Tab (or Cmd+Tab) – Switch to another window / release input grab.

> **Note:** It is possible to customize the keyboard shortcuts. See
> [input mapping](input-mapping.md).

## Modifier Key

The modifier key is the key used to trigger special keyboard shortcuts. The
default modifier key is <kbd>Cmd</kbd> on OS X and <kbd>Alt</kbd> on all other
operating systems. From now on, we will refer to the modifier key as
<kbd>Mod</kbd>.

> **Note:** You can specify another key with the
> [modifier_key](options/modifier_key.md) option.

Since <kbd>Mod</kbd> is used to trigger special actions, this key is not
directly usable for Amiga emulation. This means that -- unless you have
overridden the modifier key -- you will not be able to use this key alone to
trigger the Amiga "Left Alt" key. The solution to this is to enable full
keyboard emulation when you need it by pressing <kbd>Mod</kbd>+<kbd>K</kbd>.

> **Note:** On OS X you'll have problems with the "Left Amiga" key instead.

## Alternative modifier keys

The <kbd>F12</kbd> and <kbd>F11</kbd> are always available as modifier keys.
This is useful when full keyboard emulation is enabled since the normal
modifier keys is then disabled. The Amiga does not have either F11 or F12, so
this does not interfere with the emulation.

> **Note:** On many laptops the function keys can only be used while pressing
> <kbd>Fn</kbd>. If this is the case on your keyboard, you need to hold this
> key as well when you are asked to use a function key.

## Function keys in OS X

On OS X, using a function key (if necessary in combination with <kbd>Fn</kbd>)
may trigger a desktop function such as the "Dashboard" instead. So in order to
be able to use for example <kbd>F12</kbd> you may need to add another modifier
key such as <kbd>Cmd</kbd> into the mix so OS X will not recognize the key
combination. For example <kbd>Cmd</kbd>+<kbd>F12</kbd> will not trigger the
"Dashboard", but will still open the FS-UAE menu, since FS-UAE allows
<kbd>F12</kbd> to be used with any modifier.

Depending on your keyboard setup, you may even have to press
<kbd>Fn</kbd>+<kbd>Cmd</kbd>+<kbd>F12</kbd>. This is getting a bit ridiculous,
so what you can do instead is one or more of the following:

- Avoid the use of function keys where possible. You can use
  <kbd>Cmd</kbd>-based shortcuts instead for most purposes.
- Alter OS X keyboard settings so function keys can be used without
  <kbd>Fn</kbd>.
- Alter OS X preferences and disable the desktop shortcut for the function key
  you want to use, for example <kbd>F12</kbd>.
- Use a gamepad to open/close the FS-UAE menu.
- If you do not want to alter OS X settings, but still want to be able to
  open/close the FS-UAE menu using a single key press, you can use
  [input mapping](input-mapping.md) to assign `action_menu` to a keyboard key.
  You should choose a key which you do not need to use for Amiga emulation.
