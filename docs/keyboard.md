# Keyboard

This document explains how to use keyboard shortcuts in FS-UAE, how FS-UAE
and system keyboard shortcuts can interfere, and how the emulated Amiga
keyboard works.

**FIXME** Keyboard joystick emulation doc

**FIXME** Write about custom keyboard joystick emulation devices

**FIXME** Keyboard CD32 emulation doc

**FIXME** Reference option full_keyboard

## Keyboard shortcuts

[Keyboard shortcuts](keyboard-shortcuts.md) are listed in a separated file. You
can also read about shortcut modifier keys, how to alter those, and find
information specific to your operating system.

## Keyboard joystick emulation

If you don't have a joystick or gamepad attached to your computer, then you can
use [keyboard joystick emulation](keyboard-joystick-emulation.md) instead.

## Input grab

Grabbing the input means that FS-UAE asks the operating system to lock
the mouse to the FS-UAE window, and to send as many key presses as possible
directly to FS-UAE. This is necessary in order to:

- Allow the Amiga mouse to work properly, because relative mouse motion is
  needed.
- Allow full use of the emulated Amiga keyboard.

So, when input is grabbed, most key pressed are sent directly to FS-UAE and
is not intercepted by the operating system. This is especially true when
running FS-UAE in full-screen mode, but it also depends on the operating
system.

> **Note:** If you do _not_ want to grab as many keyboard keys as possible you
> can disable the option [keyboard_input_grab](options/keyboard_input_grab.md).
> But please note that you may be prevented from using some Amiga keys.

### Multimedia keys and desktop shortcuts

Since most key presses are sent directly to FS-UAE when input is grabbed,
most desktop shortcuts and multimedia keys will not work. This means that you
will not be able to (for example) use the volume controls on your keyboard
-- if you have those -- while FS-UAE has grabbed the input.

In order to use such keys or keyboard shortcuts, you can do one of the
following:

- Enter the FS-UAE menu. The input grab is released while the menu is open.
  You can then use the keyboard shortcuts you want, and close the FS-UAE menu
  afterwards.
- Release the input grab using <kbd>Mod</kbd>+<kbd>G</kbd>. You can re-acquire
  the input grab with the same key combination.
- Release the input grab by pressing the middle mouse button. You can
  re-acquire the input grab by pressing once more.
- Disable [keyboard grabbing](options/keyboard_input_grab.md) altogether.

### Alt-tab behavior (window switching)

When keyboard input is grabbed, on some operating systems (Linux, BSD),
Alt-Tab while generally not work, since the key presses are sent directly to
FS-UAE. In order to still allow Alt-Tab to work on these platforms, FS-UAE
does the following:

- When <kbd>Mod</kbd> is pressed, the input grab is temporarily released.
- When <kbd>Mod</kbd> is released, the input grab is taken again, unless
  another Window has been switched to in the meantime.

Since the input grab is released when you press <kbd>Mod</kbd>, the operating
system will be able to intercept <kbd>Alt</kbd>+<kbd>Tab</kbd> when you
subsequently press <kbd>Tab</kbd> while still holding <kbd>Alt</kbd> as long
as <kbd>Alt</kbd> is the modifier key in FS-UAE. If the system uses another
modifier key for window switching, this workaround will not work unless FS-UAE
is also configured to use the same modifier key.

> **Note:** On Windows and OS X, the operating system always intercepts
> <kbd>Alt</kbd>+<kbd>Tab</kbd> and <kbd>Cmd</kbd>+<kbd>Tab</kbd>
> respectively, so window switching should always works regardless of input
> grab in FS-UAE.

### Host quit keys interfering with Alt+F4 and Cmd+Q

The SDL library used by FS-UAE intercepts <kbd>Alt</kbd>+<kbd>F4</kbd> and
<kbd>Cmd</kbd>+<kbd>Q</kbd> and uses this to close the application. This means
that even with full keyboard emulation enabled, you are prevented from using
Amiga+F4 or Amiga+Q.

This will continue to be the case until SDL supports
`SDL_HINT_HANDLE_OS_HOTKEYS = 0`.

FIXME: Might be outdated information by now! Double-check

## Amiga keyboard mapping

Information about how the [keyboard is mapping](keyboard-mapping.md) is moved
to a separate file. You can also read about custom keyboard mapping there.

## Additional shortcuts

The following shortcuts work, but you should be aware that the
<kbd>Ctrl</kbd> key press will be sent to the emulated Amiga before you have
completed the shortcut combination, so the key press may have side effects
on the emulated Amiga:

**FIXME** Remove items from this list which is listed elsewhere:

- <kbd>Alt</kbd>+<kbd>Tab</kbd> (or <kbd>Cmd</kbd>+<kbd>Tab</kbd>) – Switch
  to another window / release input grab.
- <kbd>Alt</kbd>+<kbd>F4</kbd> – Quit the emulator.
- <kbd>Ctrl</kbd>+<kbd>F10</kbd> – Toggle performance graphs / FPS display
  (There are two FPS numbers shown, SYS: actual video output frame rate.
  EMU: emulated video frame rate).

### Additional Shortcuts in Net Play Mode

The following shortcuts will only have an special effect while in net play
mode:

- Press <kbd>Tab</kbd> to toggle chat function.

## Other

### Custom shortcuts and emulation

If you create a custom shortcut, for example:

    keyboard_key_ctrl_p = action_pause

Then you should be aware that the emulated Amiga will see the
<kbd>Ctrl</kbd> key press before you have pressed <kbd>P</kbd>. The Amiga
will not see the <kbd>P</kbd> key press however, since this triggers the
shortcut instead.

This may or may not be a problem for you. But it isn't possible to avoid
automatically because FS-UAE cannot know if you are going to press a key
combination or not. In this case, if you do not want the Amiga to see the
<kbd>Ctrl</kbd> key presses, you can for example also add:

    keyboard_key_ctrl = action_none

But then of course, you'll have no way to press the Amiga "Ctrl" key - unless
you also map another host key (combination) to Amiga "Ctrl", for example:

    keyboard_key_ctrl_c = action_key_ctrl

## Examples

### Remapping the Extra Key on the Number Row

The number row of the Amiga has one more key than on PC keyboards. By
default, this key is moved to the key left of <kbd>Return</kbd>
(two rows down), and this key again is moved to the <kb>Insert</kbd> key.
If you want to, you can change the mapping so the extra key is moved directly
to the Insert key, while leaving the one next to Return alone:

    keyboard_key_insert = action_key_backslash
    keyboard_key_backslash = action_key_2b

### Disabling the F12 Menu Key

You can disable the F12 menu key with the following option, but please note
that it will still function as alternative modifier key:

    keyboard_key_f12 = action_none
