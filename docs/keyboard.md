# Keyboard

This document explains how to use keyboard shortcuts in FS-UAE, how FS-UAE
and system keyboard shortcuts can interfere, and how the emulated Amiga
keyboard works.

**FIXME** Keyboard joystick emulation doc

**FIXME** Write about custom keyboard joystick emulation devices

**FIXME** Keyboard CD32 emulation doc

**FIXME** Reference full_keyboard

## Keyboard Shortcuts

The following keyboard shortcuts are available by default. <kbd>Mod</kbd>
means <kbd>Cmd</kbd> on OS X and <kbd>Alt</kbd> on other platforms, but it
can be re-configured as another key. See the "modifier key" section for more
information.

> **Note:** The shortcuts are mapped against keys on a English keyboard,
> so on international keyboards, the key (symbol) you have to press may
> in some cases be different (but same physical key).

* Use <kbd>F12</kbd> to enter the FS-UAE on-screen menu.
* Use <kbd>Mod</kbd>+<kbd>F</kbd> or <kbd>Mod</kbd>+<kbd>Return</kbd> to
  toggle full-screen mode.
* Use <kbd>Mod</kbd>+<kbd>Z</kbd> or <kbd>F11</kbd> to cycle through zoom modes.
* Use <kbd>Mod</kbd>+<kbd>B</kbd> to toggle extra visible border.
* Use <kbd>Mod</kbd>+<kbd>S</kbd> or <kbd>Print Scrn</kbd> to save a screenshot
  of the current Amiga frame to the desktop (or chosen screenshots location).
* Use <kbd>Mod</kbd>+<kbd>Q</kbd> or <kbd>Mod</kbd>+<kbd>F4</kbd> to quit.
* Use <kbd>Mod</kbd>+<kbd>D</kbd> to activate (console) debugger.

Input control:

* Use <kbd>Mod</kbd>+<kbd>G</kbd> to release or acquire the input grab. You
  can also use the middle mouse button, or enter the menu.
* Use <kbd>Mod</kbd>+<kbd>K</kbd> to enable full keyboard emulation. This
  disables the modifier key to make it usable in emulation. In order to
  disable full keyboard emulation you need to press
  <kbd>AltMod</kbd>+<kbd>K</kbd>. This will usually be
  <kbd>F12</kbd>+<kbd>K</kbd>. Please see the "alternative modifier keys"
  section.
* Use <kbd>Mod</kbd>+<kbd>Up</kbd> when you need to press Amiga cursor
  up and joystick emulation is enabled. Similar for
  <kbd>Mod</kbd>+<kbd>Down</kbd>, <kbd>Mod</kbd>+<kbd>Left</kbd> and
  <kbd>Mod</kbd>+<kbd>Right</kbd>.

Emulation control:

* Use <kbd>Mod</kbd>+<kbd>W</kbd> to toggle warp mode (no frame limit,
  no audio). This does not affect the emulation accuracy, it only removes the
  pauses between generating frames.
* Use <kbd>Mod</kbd>+<kbd>R</kbd> to perform a soft reset.
* Use <kbd>Mod</kbd>+<kbd>T</kbd> to perform a hard reset.
* Use <kbd>Mod</kbd>+<kbd>P</kbd> or <kbd>Pause</kbd> to toggle pause mode.
* Use <kbd>Mod</kbd>+<kbd>A</kbd> to activate cartridge (Action Replay, ...).

Volume control:

* Use <kbd>Mod</kbd>+<kbd>,</kbd> to decrease FS-UAE volume.
* Use <kbd>Mod</kbd>+<kbd>.</kbd> to increase FS-UAE volume.
* Use <kbd>Mod</kbd>+<kbd>M</kbd> to mute FS-UAE (press again to unmute).
* Use <kbd>Mod</kbd>+<kbd>N</kbd> to mute floppy sounds (press again to unmute).

Disk swapper:

* Use <kbd>Prev</kbd> (multimedia key) to go backward through the swap list.
* Use <kbd>Next</kbd> (multimedia key) to go forward through the swap list.
* Use <kbd>Play</kbd> (multimedia key) to insert the currently selected
  swap list entry into `DF0`.

FIXME:

* Tab – toggle chat function.
* Ctrl+F10 – Toggle performance graphs / FPS display (There are two FPS numbers shown, SYS: actual video output frame rate. EMU: emulated video frame rate).
* Alt+Tab (or Cmd+Tab) – Switch to another window / release input grab.

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
overridden the modifier key -- you will not be able to use this key alone
to trigger the Amiga "Left Alt" key. The solution to this is to enable full
keyboard emulation when you need it by pressing <kbd>Mod</kbd>+<kbd>K</kbd>.

> **Note:** On OS X you'll have problems with the "Left Amiga" key instead.

### Alternative Modifier Keys

The <kbd>F12</kbd> and <kbd>F11</kbd> are always available as modifier keys.
This is useful when full keyboard emulation is enabled since the normal
modifier keys is then disabled. The Amiga does not have either F11 or F12, so
this does not interfere with the emulation.

> **Note:** On many laptops the function keys can only be used while pressing
> <kbd>Fn</kbd>. If this is the case on your keyboard, you need to hold this
> key as well when you are asked to use a function key.

### Function Keys in OS X

On OS X, using a function key (if necessary in combination with
<kbd>Fn</kbd>) may trigger a desktop function such as the "Dashboard"
instead. So in order to be able to use for example <kbd>F12</kbd> you may
need to add another modifier key such as <kbd>Cmd</kbd> into the mix so OS X
will not recognize the key combination. For example
<kbd>Cmd</kbd>+<kbd>F12</kbd> will not trigger the "Dashboard", but will still
open the FS-UAE menu, since FS-UAE allows <kbd>F12</kbd> to be used with any
modifier.

Depending on your keyboard setup, you may even have to press
<kbd>Fn</kbd>+<kbd>Cmd</kbd>+<kbd>F12</kbd>. This is getting a bit ridiculous,
so what you can do instead is one or more of the following:

* Avoid the use of function keys where possible. You can use
  <kbd>Cmd</kbd>-based shortcuts instead for most purposes.
* Alter OS X keyboard settings so function keys can be used without
  <kbd>Fn</kbd>.
* Alter OS X preferences and disable the desktop shortcut for the function key
  you want to use, for example <kbd>F12</kbd>.
* Use a gamepad to open/close the FS-UAE menu.
* If you do not want to alter OS X settings, but still want to be able to
  open/close the FS-UAE menu using a single key press, you can use
  [input mapping](input-mapping.md) to assign `action_menu` to a keyboard key.
  You should choose a key which you do not need to use for Amiga emulation.

Input Grab
----------

Grabbing the input means that FS-UAE asks the operating system to lock
the mouse to the FS-UAE window, and to send as many key presses as possible
directly to FS-UAE. This is necessary in order to:

* Allow the Amiga mouse to work properly, because relative mouse motion is
  needed.
* Allow full use of the emulated Amiga keyboard.

So, when input is grabbed, most key pressed are sent directly to FS-UAE and
is not intercepted by the operating system. This is especially true when
running FS-UAE in full-screen mode, but it also depends on the operating
system.

> **Note:** If you do *not* want to grab as many keyboard keys as possible you
> can disable the option [keyboard_input_grab](options/keyboard_input_grab.md).
> But please note that you may be prevented from using some Amiga keys.

### Multimedia Keys and Desktop Shortcuts

Since most key presses are sent directly to FS-UAE when input is grabbed,
most desktop shortcuts and multimedia keys will not work. This means that you
will not be able to (for example) use the volume controls on your keyboard
-- if you have those -- while FS-UAE has grabbed the input.

In order to use such keys or keyboard shortcuts, you can do one of the
following:

* Enter the FS-UAE menu. The input grab is released while the menu is open.
  You can then use the keyboard shortcuts you want, and close the FS-UAE menu
  afterwards.
* Release the input grab using <kbd>Mod</kbd>+<kbd>G</kbd>. You can re-acquire
  the input grab with the same key combination.
* Release the input grab by pressing the middle mouse button. You can
  re-acquire the input grab by pressing once more.
* Disable [keyboard grabbing](options/keyboard_input_grab.md) altogether.

### Alt-Tab Behavior (Window Switching)

When keyboard input is grabbed, on some operating systems (Linux, BSD),
Alt-Tab while generally not work, since the key presses are sent directly to
FS-UAE. In order to still allow Alt-Tab to work on these platforms, FS-UAE
does the following:

* When <kbd>Mod</kbd> is pressed, the input grab is temporarily released.
* When <kbd>Mod</kbd> is released, the input grab is taken again, unless
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

## Quit Keys

The SDL library used by FS-UAE intercepts <kbd>Alt</kbd>+<kbd>F4</kbd> and
<kbd>Cmd</kbd>+<kbd>Q</kbd> and uses this to close the application. This means
that even with full keyboard emulation enabled, you are prevented from using
Amiga+F4 or Amiga+Q.

This will continue to be the case until SDL supports
`SDL_HINT_HANDLE_OS_HOTKEYS = 0`.

## Amiga Keyboard Mapping

Keys in FS-UAE are (generally) mapped based on physical location and not by
symbol. Trying to map symbols would impossible without hack-ish workarounds
anyway, since one Amiga key can have its symbols spread over two or more host
keys, depending on keyboard layout.

To to understand how to use the emulated Amiga keyboard, it is useful to
visualize that you have printed stickers for Amiga keys, and pasted them
on top of your keyboard keys, in the corresponding physical location.

Please note the following **important information**:

* Amiga key names are based on what the *physical* key location is called
  on an US/UK Amiga keyboard.
* Similarly, host key names are based on what the *physical* key location
  is called on an US/UK Amiga keyboard.

If you have a non-English keyboard, you need to compare your keyboard
to an English one to find out what a key in a given physical location is
called. Likewise, you must use an English Amiga keyboard as reference to
understand what a specific Amiga key is named.

> **Note:** Since the Amiga keys are listed in row order, you can also
> use this order to figure out the correct name for non-English keys. For
> host keys, you can refer to the separate table in the "Host Key names"
> section.

The Amiga keys are listed in row order, top row first. The listed action
names in the table are for use with custom [input mapping](input-mapping.md).
The host key name is printed in **bold letters** when the physical location
of the host key differs from the Amiga key.

Amiga Key    | Input Mapping Action        | Default Host Key(s)
------------ | --------------------------- | -------------------
ESC          | `action_key_esc`            | ESCAPE
F1           | `action_key_f1`             | F1
F2           | `action_key_f2`             | F2
F3           | `action_key_f3`             | F3
F4           | `action_key_f4`             | F4
F5           | `action_key_f5`             | F5
F6           | `action_key_f6`             | F6
F7           | `action_key_f7`             | F7
F8           | `action_key_f8`             | F8
F9           | `action_key_f9`             | F9
F10          | `action_key_f10`            | F10
-            |                             |
BACKQUOTE    | `action_key_backquote`      | BACKQUOTE
1            | `action_key_1`              | 1
2            | `action_key_2`              | 2
3            | `action_key_3`              | 3
4            | `action_key_4`              | 4
5            | `action_key_5`              | 5
6            | `action_key_6`              | 6
7            | `action_key_7`              | 7
8            | `action_key_8`              | 8
9            | `action_key_9`              | 9
0            | `action_key_0`              | 0
SUB          | `action_key_sub`            | MINUS
EQUALS       | `action_key_equals`         | EQUALS
BACKSLASH    | `action_key_backslash`      | **BACKSLASH**
BACKSPACE    | `action_key_backspace`      | BACKSPACE
DEL          | `action_key_del`            | DELETE
HELP         | `action_key_help`           | **END**
LPAREN       | `action_key_lparen`         | **HOME**
RPAREN       | `action_key_rparen`         | **PAGEUP**
NP_DIV       | `action_key_div`            | KP_DIVIDE
NP_MUL       | `action_key_mul`            | KP_MULTIPLY
-            |                             |
TAB          | `action_key_tab`            | TAB
Q            | `action_key_q`              | Q
W            | `action_key_w`              | W
E            | `action_key_e`              | E
R            | `action_key_r`              | R
T            | `action_key_t`              | T
Y            | `action_key_y`              | Y
U            | `action_key_u`              | U
I            | `action_key_i`              | I
O            | `action_key_o`              | O
P            | `action_key_p`              | P
LEFTBRACKET  | `action_key_leftbracket`    | LEFTBRACKET
RIGHTBRACKET | `action_key_rightbracket`   | RIGHTBRACKET
RETURN       | `action_key_return`         | RETURN
NP_7         | `action_key_np_7`           | KP7
NP_8         | `action_key_np_8`           | KP8
NP_9         | `action_key_np_9`           | KP9
NP_SUB       | `action_key_sub`            | KP_MINUS
-            |                             |
CTRL         | `action_key_ctrl`           | LCTRL
CAPS_LOCK    | `action_key_caps_lock`      | CAPSLOCK
A            | `action_key_a`              | A
S            | `action_key_s`              | S
D            | `action_key_d`              | D
F            | `action_key_f`              | F
G            | `action_key_g`              | G
H            | `action_key_h`              | H
J            | `action_key_j`              | J
K            | `action_key_k`              | K
L            | `action_key_l`              | L
SEMICOLON    | `action_key_semicolon`      | SEMICOLON
SINGLEQUOTE  | `action_key_singlequote`    | QUOTE
2B           | `action_key_2b`             | **INSERT**
CURSOR_UP    | `action_key_cursor_up`      | UP
NP_4         | `action_key_np_4`           | KP4
NP_5         | `action_key_np_5`           | KP5
NP_6         | `action_key_np_6`           | KP6
NP_ADD       | `action_key_add`            | KP_PLUS
-            |                             |
SHIFT_LEFT   | `action_key_shift_left`     | LSHFT
30           | `action_key_30`             | LESS
Z            | `action_key_z`              | Z
X            | `action_key_x`              | X
C            | `action_key_c`              | C
V            | `action_key_v`              | V
B            | `action_key_b`              | B
N            | `action_key_n`              | N
M            | `action_key_m`              | M
COMMA        | `action_key_comma`          | COMMA
PERIOD       | `action_key_period`         | PERIOD
DIV          | `action_key_div`            | SLASH
SHIFT_RIGHT  | `action_key_shift_right`    | RSHIFT
CURSOR_LEFT  | `action_key_cursor_left`    | LEFT
CURSOR_DOWN  | `action_key_cursor_down`    | DOWN
CURSOR_RIGHT | `action_key_cursor_right`   | RIGHT
NP_1         | `action_key_np_1`           | KP1
NP_2         | `action_key_np_2`           | KP2
NP_3         | `action_key_np_3`           | KP3
NP_ENTER     | `action_key_enter`          | KP_ENTER
ALT_LEFT     | `action_key_alt_left`       | **LALT**
AMIGA_LEFT   | `action_key_amiga_left`     | **LSUPER**
SPACE        | `action_key_space`          | SPACE
AMIGA_RIGHT  | `action_key_amiga_right`    | **RSUPER**, **PAGEDOWN**, **MENU**
ALT_RIGHT    | `action_key_alt_right`      | **RALT**
NP_0         | `action_key_np_0`           | KP0
NP_PERIOD    | `action_key_period`         | KP_PERIOD

* **FIXME** map MENU to ALT_LEFT instead?
* **FIXME** Alt and Amiga mappings

The keys named "2B and "30" in this list is for international Amiga keyboards
which has these physical keys:

* Amiga key code `0x2B` is the key between SINGLEQUOTE and RETURN.
* Amiga key code `0x30` is the key between LSHIFT and Z.

> **Note:** It is often possible to change the keyboard layout in Workbench
> (or other programs) running in the emulated Amiga. This will *not* affect
> the names of the keys used in input mapping. In fact, FS-UAE will be unaware
> of such a setting, and it will work just like it would with a real Amiga
> and a real Amiga keyboard. I.e. if you change keyboard layout in Amiga
> emulation you can get a different symbol when you press a given key.

## Host Key Names

Host Key     | Input Mapping Option        | Comment
------------ | --------------------------- | -------
-            |                             | **Function key row**
ESCAPE       | `keyboard_key_escape`       |
F1           | `keyboard_key_f1`           |
F2           | `keyboard_key_f2`           |
F3           | `keyboard_key_f3`           |
F4           | `keyboard_key_f4`           |
F5           | `keyboard_key_f5`           |
F6           | `keyboard_key_f6`           |
F7           | `keyboard_key_f7`           |
F8           | `keyboard_key_f8`           |
F9           | `keyboard_key_f9`           |
F10          | `keyboard_key_f10`          |
F11          | `keyboard_key_f11`          |
F12          | `keyboard_key_f12`          |
PRINT        | `keyboard_key_print`        |
SCROLLOCK    | `keyboard_key_scrollock`    |
PAUSE        | `keyboard_key_print`        |
-            |                             | **Number row**
BACKQUOTE    | `keyboard_key_backquote`    |
1            | `keyboard_key_1`            |
2            | `keyboard_key_2`            |
3            | `keyboard_key_3`            |
4            | `keyboard_key_4`            |
5            | `keyboard_key_5`            |
6            | `keyboard_key_6`            |
7            | `keyboard_key_7`            |
8            | `keyboard_key_8`            |
9            | `keyboard_key_9`            |
0            | `keyboard_key_0`            |
MINUS        | `keyboard_key_minus`        |
EQUALS       | `keyboard_key_equals`       |
BACKSPACE    | `keyboard_key_backspace`    |
INSERT       | `keyboard_key_insert`       |
HOME         | `keyboard_key_home`         |
PAGEUP       | `keyboard_key_pageup`       |
NUMLOCK      | `keyboard_key_numlock`      |
KP_DIVIDE    | `keyboard_key_kp_divide`    |
KP_MULTIPLY  | `keyboard_key_kp_multiply`  |
KP_MINUS     | `keyboard_key_kp_minus`     |
-            |                             | **Top letter row**
TAB          | `keyboard_key_tab`          |
Q            | `keyboard_key_q`            |
W            | `keyboard_key_w`            |
E            | `keyboard_key_e`            |
R            | `keyboard_key_r`            |
T            | `keyboard_key_t`            |
Y            | `keyboard_key_y`            |
U            | `keyboard_key_u`            |
I            | `keyboard_key_i`            |
O            | `keyboard_key_o`            |
P            | `keyboard_key_p`            |
LEFTBRACKET  | `keyboard_key_rightbracket` |
RIGHTBRACKET | `keyboard_key_rightbracket` |
RETURN       | `keyboard_key_return`       |
DELETE       | `keyboard_key_delete`       |
END          | `keyboard_key_end`          |
PAGEDOWN     | `keyboard_key_pagedown`     |
KP7          | `keyboard_key_kp7`          |
KP8          | `keyboard_key_kp8`          |
KP9          | `keyboard_key_kp9`          |
KP_PLUS      | `keyboard_key_kp_plus`      |
-            |                             | **Middle letter row**
CAPSLOCK     | `keyboard_key_capslock`     |
A            | `keyboard_key_a`            |
S            | `keyboard_key_s`            |
D            | `keyboard_key_d`            |
F            | `keyboard_key_f`            |
G            | `keyboard_key_g`            |
H            | `keyboard_key_h`            |
J            | `keyboard_key_j`            |
K            | `keyboard_key_k`            |
L            | `keyboard_key_l`            |
SEMICOLON    | `keyboard_key_semicolon`    |
QUOTE        | `keyboard_key_quote`        |
BACKSLASH    | `keyboard_key_backslash`    |
KP4          | `keyboard_key_kp4`          |
KP5          | `keyboard_key_kp5`          |
KP6          | `keyboard_key_kp6`          |
-            |                             | **Bottom letter row**
LSHIFT       | `keyboard_key_lshift`       |
LESS         | `keyboard_key_less`         |
Z            | `keyboard_key_z`            |
X            | `keyboard_key_x`            |
C            | `keyboard_key_c`            |
V            | `keyboard_key_v`            |
B            | `keyboard_key_b`            |
N            | `keyboard_key_n`            |
M            | `keyboard_key_m`            |
COMMA        | `keyboard_key_comma`        |
PERIOD       | `keyboard_key_period`       |
SLASH        | `keyboard_key_slash`        |
RSHIFT       | `keyboard_key_rshift`       |
UP           | `keyboard_key_up`           |
KP1          | `keyboard_key_kp1`          |
KP2          | `keyboard_key_kp2`          |
KP3          | `keyboard_key_kp3`          |
KP_ENTER     | `keyboard_key_kp_enter`     |
-            |                             | **Bottom row**
LCTRL        | `keyboard_key_kp_lctrl`     |
LSUPER       | `keyboard_key_kp_lsuper`    | Left "Windows" key or "Cmd"
LALT         | `keyboard_key_kp_lalt`      | Different physical position on Mac
SPACE        | `keyboard_key_kp_space`     |
RALT         | `keyboard_key_kp_ralt`      | Different physical position on Mac
RSUPER       | `keyboard_key_kp_rsuper`    | Right "Windows" key or "Cmd"
MENU         | `keyboard_key_kp_menu`      |
RCTRL        | `keyboard_key_kp_rctrl`     |
LEFT         | `keyboard_key_left`         |
DOWN         | `keyboard_key_down`         |
RIGHT        | `keyboard_key_right`        |
KP0          | `keyboard_key_kp0`          |
KP_PERIOD    | `keyboard_key_kp_period`    |
-            |                             | **Special keys**
VOLUMEUP     | `keyboard_key_volumeup`     |
VOLUMEDOWN   | `keyboard_key_volumedown`   |
MUTE         | `keyboard_key_mute`         |
AUDIONEXT    | `keyboard_key_audionext`    |
AUDIOPREV    | `keyboard_key_audioprev`    |
AUDIOSTOP    | `keyboard_key_audiostop`    |
AUDIOPLAY    | `keyboard_key_audioplay`    |

## Additional Shortcuts

The following shortcuts work, but you should be aware that the
<kbd>Ctrl</kbd> key press will be sent to the emulated Amiga before you have
completed the shortcut combination, so the key press may have side effects
on the emulated Amiga:

**FIXME** Remove items from this list which is listed elsewhere:

* <kbd>Alt</kbd>+<kbd>Tab</kbd> (or <kbd>Cmd</kbd>+<kbd>Tab</kbd>) – Switch
  to another window / release input grab.
* <kbd>Alt</kbd>+<kbd>F4</kbd> – Quit the emulator.
* <kbd>Ctrl</kbd>+<kbd>F10</kbd> – Toggle performance graphs / FPS display
  (There are two FPS numbers shown, SYS: actual video output frame rate.
  EMU: emulated video frame rate).

### Additional Shortcuts in Net Play Mode

The following shortcuts will only have an special effect while in net play
mode:

* Press <kbd>Tab</kbd> to toggle chat function.

## Other

### Custom Shortcuts and Emulation

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
