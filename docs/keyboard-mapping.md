# Keyboard mapping

Not all Amiga keys are present on modern computer keyboards, and vice versa.
Most keys are mapped like you would expect (F1 on your keyboard activates F1 on
the emulated Amiga keyboard, etc).

Here are some notable exceptions:
* Amiga Help = End (Mac laptops: Fn + Right)
* Amiga Delete = Delete (Mac laptops: Fn + Backspace)
* Amiga Numpad Left Paren = Home (Mac laptops: Fn + Left)
* Amiga Numpad Right Paren = Page Up (Mac laptops: Fn + Up)
* Amiga Left A = Page Down or Right Menu / Right Windows key.
* Amiga Right A = Left Menu / Left Windows key.
* Amiga Ctrl = Left Ctrl
* Amiga key to the left of Backspace = Insert

Emulator [keyboard shortcuts](keyboard-shortcuts.md) are documented on a
separate page.

Keys in FS-UAE are (generally) mapped based on physical location and not by
symbol. Trying to map symbols would require hack-ish workarounds, since one
Amiga key can have its symbols spread over two or more host keys, depending
on keyboard layout.

To to understand how to use the emulated Amiga keyboard, it is useful to
visualize that you have printed stickers for Amiga keys, and pasted them
on top of your keyboard keys, in the corresponding physical location.

Please note the following **important information**:

- Amiga key names are based on what the _physical_ key location is called
  on an US/UK Amiga keyboard.
- Similarly, host key names are based on what the _physical_ key location
  is called on an US/UK Amiga keyboard.

If you have a non-English keyboard, you need to compare your keyboard
to an English one to find out what a key in a given physical location is
called. Likewise, you must use an English Amiga keyboard as reference to
understand what a specific Amiga key is named.

> **Note:** Since the Amiga keys are listed in row order, you can also
> use this order to figure out the correct name for non-English keys. For
> host keys, you can refer to the separate table in the "Host Key names"
> section.

## Amiga keys

The Amiga keys are listed in row order, top row first. The listed action
names in the table are for use with custom [input mapping](input-mapping.md).
The host key name is printed in **bold letters** when the physical location
of the host key differs from the Amiga key.

| Amiga Key    | Input Mapping Action      | Default Host Key(s)............ |
| ------------ | ------------------------- | ------------------------------- |
| ESC          | `action_key_esc`          | ESCAPE                          |
| F1           | `action_key_f1`           | F1                              |
| F2           | `action_key_f2`           | F2                              |
| F3           | `action_key_f3`           | F3                              |
| F4           | `action_key_f4`           | F4                              |
| F5           | `action_key_f5`           | F5                              |
| F6           | `action_key_f6`           | F6                              |
| F7           | `action_key_f7`           | F7                              |
| F8           | `action_key_f8`           | F8                              |
| F9           | `action_key_f9`           | F9                              |
| F10          | `action_key_f10`          | F10                             |
| -            |                           |
| BACKQUOTE    | `action_key_backquote`    | BACKQUOTE                       |
| 1            | `action_key_1`            | 1                               |
| 2            | `action_key_2`            | 2                               |
| 3            | `action_key_3`            | 3                               |
| 4            | `action_key_4`            | 4                               |
| 5            | `action_key_5`            | 5                               |
| 6            | `action_key_6`            | 6                               |
| 7            | `action_key_7`            | 7                               |
| 8            | `action_key_8`            | 8                               |
| 9            | `action_key_9`            | 9                               |
| 0            | `action_key_0`            | 0                               |
| SUB          | `action_key_sub`          | MINUS                           |
| EQUALS       | `action_key_equals`       | EQUALS                          |
| BACKSLASH    | `action_key_backslash`    | **BACKSLASH**                   |
| BACKSPACE    | `action_key_backspace`    | BACKSPACE                       |
| DEL          | `action_key_del`          | DELETE                          |
| HELP         | `action_key_help`         | **END**                         |
| LPAREN       | `action_key_lparen`       | **HOME**                        |
| RPAREN       | `action_key_rparen`       | **PAGEUP**                      |
| NP_DIV       | `action_key_div`          | KP_DIVIDE                       |
| NP_MUL       | `action_key_mul`          | KP_MULTIPLY                     |
| -            |                           |
| TAB          | `action_key_tab`          | TAB                             |
| Q            | `action_key_q`            | Q                               |
| W            | `action_key_w`            | W                               |
| E            | `action_key_e`            | E                               |
| R            | `action_key_r`            | R                               |
| T            | `action_key_t`            | T                               |
| Y            | `action_key_y`            | Y                               |
| U            | `action_key_u`            | U                               |
| I            | `action_key_i`            | I                               |
| O            | `action_key_o`            | O                               |
| P            | `action_key_p`            | P                               |
| LEFTBRACKET  | `action_key_leftbracket`  | LEFTBRACKET                     |
| RIGHTBRACKET | `action_key_rightbracket` | RIGHTBRACKET                    |
| RETURN       | `action_key_return`       | RETURN                          |
| NP_7         | `action_key_np_7`         | KP7                             |
| NP_8         | `action_key_np_8`         | KP8                             |
| NP_9         | `action_key_np_9`         | KP9                             |
| NP_SUB       | `action_key_sub`          | KP_MINUS                        |
| -            |                           |
| CTRL         | `action_key_ctrl`         | LCTRL                           |
| CAPS_LOCK    | `action_key_caps_lock`    | CAPSLOCK                        |
| A            | `action_key_a`            | A                               |
| S            | `action_key_s`            | S                               |
| D            | `action_key_d`            | D                               |
| F            | `action_key_f`            | F                               |
| G            | `action_key_g`            | G                               |
| H            | `action_key_h`            | H                               |
| J            | `action_key_j`            | J                               |
| K            | `action_key_k`            | K                               |
| L            | `action_key_l`            | L                               |
| SEMICOLON    | `action_key_semicolon`    | SEMICOLON                       |
| SINGLEQUOTE  | `action_key_singlequote`  | QUOTE                           |
| 2B           | `action_key_2b`           | **INSERT**                      |
| CURSOR_UP    | `action_key_cursor_up`    | UP                              |
| NP_4         | `action_key_np_4`         | KP4                             |
| NP_5         | `action_key_np_5`         | KP5                             |
| NP_6         | `action_key_np_6`         | KP6                             |
| NP_ADD       | `action_key_add`          | KP_PLUS                         |
| -            |                           |
| SHIFT_LEFT   | `action_key_shift_left`   | LSHFT                           |
| 30           | `action_key_30`           | LESS                            |
| Z            | `action_key_z`            | Z                               |
| X            | `action_key_x`            | X                               |
| C            | `action_key_c`            | C                               |
| V            | `action_key_v`            | V                               |
| B            | `action_key_b`            | B                               |
| N            | `action_key_n`            | N                               |
| M            | `action_key_m`            | M                               |
| COMMA        | `action_key_comma`        | COMMA                           |
| PERIOD       | `action_key_period`       | PERIOD                          |
| DIV          | `action_key_div`          | SLASH                           |
| SHIFT_RIGHT  | `action_key_shift_right`  | RSHIFT                          |
| CURSOR_LEFT  | `action_key_cursor_left`  | LEFT                            |
| CURSOR_DOWN  | `action_key_cursor_down`  | DOWN                            |
| CURSOR_RIGHT | `action_key_cursor_right` | RIGHT                           |
| NP_1         | `action_key_np_1`         | KP1                             |
| NP_2         | `action_key_np_2`         | KP2                             |
| NP_3         | `action_key_np_3`         | KP3                             |
| NP_ENTER     | `action_key_enter`        | KP_ENTER                        |
| ALT_LEFT     | `action_key_alt_left`     | **LALT**                        |
| AMIGA_LEFT   | `action_key_amiga_left`   | **LSUPER**                      |
| SPACE        | `action_key_space`        | SPACE                           |
| AMIGA_RIGHT  | `action_key_amiga_right`  | **RSUPER, PAGEDOWN, MENU**      |
| ALT_RIGHT    | `action_key_alt_right`    | **RALT**                        |
| NP_0         | `action_key_np_0`         | KP0                             |
| NP_PERIOD    | `action_key_period`       | KP_PERIOD                       |

- **FIXME** map MENU to ALT_LEFT instead?
- **FIXME** Alt and Amiga mappings

The keys named "2B and "30" in this list is for international Amiga keyboards
which has these physical keys:

- Amiga key code `0x2B` is the key between SINGLEQUOTE and RETURN.
- Amiga key code `0x30` is the key between LSHIFT and Z.

> **Note:** It is often possible to change the keyboard layout in Workbench
> (or other programs) running in the emulated Amiga. This will _not_ affect
> the names of the keys used in input mapping. In fact, FS-UAE will be unaware
> of such a setting, and it will work just like it would with a real Amiga
> and a real Amiga keyboard. I.e. if you change keyboard layout in Amiga
> emulation you can get a different symbol when you press a given key.

## Host key names

| Host Key     | Input Mapping Option        | Comment...................... |
| ------------ | --------------------------- | ----------------------------- |
| -            |                             | **Function key row**          |
| ESCAPE       | `keyboard_key_escape`       |
| F1           | `keyboard_key_f1`           |
| F2           | `keyboard_key_f2`           |
| F3           | `keyboard_key_f3`           |
| F4           | `keyboard_key_f4`           |
| F5           | `keyboard_key_f5`           |
| F6           | `keyboard_key_f6`           |
| F7           | `keyboard_key_f7`           |
| F8           | `keyboard_key_f8`           |
| F9           | `keyboard_key_f9`           |
| F10          | `keyboard_key_f10`          |
| F11          | `keyboard_key_f11`          |
| F12          | `keyboard_key_f12`          |
| PRINT        | `keyboard_key_print`        | Labelled F13 on Apple A1243   |
| SCROLLOCK    | `keyboard_key_scrollock`    | Labelled F14 on Apple A1243   |
| PAUSE        | `keyboard_key_pause`        | Labelled F15 on Apple A1243   |
| F16          | `keyboard_key_f16`          | Apple A1243 keyboard          |
| F17          | `keyboard_key_f17`          | Apple A1243 keyboard          |
| F18          | `keyboard_key_f18`          | Apple A1243 keyboard          |
| F19          | `keyboard_key_f19`          | Apple A1243 keyboard          |
| -            |                             | **Number row**                |
| BACKQUOTE    | `keyboard_key_backquote`    |
| 1            | `keyboard_key_1`            |
| 2            | `keyboard_key_2`            |
| 3            | `keyboard_key_3`            |
| 4            | `keyboard_key_4`            |
| 5            | `keyboard_key_5`            |
| 6            | `keyboard_key_6`            |
| 7            | `keyboard_key_7`            |
| 8            | `keyboard_key_8`            |
| 9            | `keyboard_key_9`            |
| 0            | `keyboard_key_0`            |
| MINUS        | `keyboard_key_minus`        |
| EQUALS       | `keyboard_key_equals`       |
| BACKSPACE    | `keyboard_key_backspace`    |
| INSERT       | `keyboard_key_insert`       |
| HOME         | `keyboard_key_home`         |
| PAGEUP       | `keyboard_key_pageup`       |
| NUMLOCK      | `keyboard_key_numlock`      |
| KP_DIVIDE    | `keyboard_key_kp_divide`    |
| KP_MULTIPLY  | `keyboard_key_kp_multiply`  |
| KP_MINUS     | `keyboard_key_kp_minus`     |
| -            |                             | **Top letter row**            |
| TAB          | `keyboard_key_tab`          |
| Q            | `keyboard_key_q`            |
| W            | `keyboard_key_w`            |
| E            | `keyboard_key_e`            |
| R            | `keyboard_key_r`            |
| T            | `keyboard_key_t`            |
| Y            | `keyboard_key_y`            |
| U            | `keyboard_key_u`            |
| I            | `keyboard_key_i`            |
| O            | `keyboard_key_o`            |
| P            | `keyboard_key_p`            |
| LEFTBRACKET  | `keyboard_key_rightbracket` |
| RIGHTBRACKET | `keyboard_key_rightbracket` |
| RETURN       | `keyboard_key_return`       |
| DELETE       | `keyboard_key_delete`       |
| END          | `keyboard_key_end`          |
| PAGEDOWN     | `keyboard_key_pagedown`     |
| KP7          | `keyboard_key_kp7`          |
| KP8          | `keyboard_key_kp8`          |
| KP9          | `keyboard_key_kp9`          |
| KP_PLUS      | `keyboard_key_kp_plus`      |
| -            |                             | **Middle letter row**         |
| CAPSLOCK     | `keyboard_key_capslock`     |
| A            | `keyboard_key_a`            |
| S            | `keyboard_key_s`            |
| D            | `keyboard_key_d`            |
| F            | `keyboard_key_f`            |
| G            | `keyboard_key_g`            |
| H            | `keyboard_key_h`            |
| J            | `keyboard_key_j`            |
| K            | `keyboard_key_k`            |
| L            | `keyboard_key_l`            |
| SEMICOLON    | `keyboard_key_semicolon`    |
| QUOTE        | `keyboard_key_quote`        |
| BACKSLASH    | `keyboard_key_backslash`    |
| KP4          | `keyboard_key_kp4`          |
| KP5          | `keyboard_key_kp5`          |
| KP6          | `keyboard_key_kp6`          |
| -            |                             | **Bottom letter row**         |
| LSHIFT       | `keyboard_key_lshift`       |
| LESS         | `keyboard_key_less`         |
| Z            | `keyboard_key_z`            |
| X            | `keyboard_key_x`            |
| C            | `keyboard_key_c`            |
| V            | `keyboard_key_v`            |
| B            | `keyboard_key_b`            |
| N            | `keyboard_key_n`            |
| M            | `keyboard_key_m`            |
| COMMA        | `keyboard_key_comma`        |
| PERIOD       | `keyboard_key_period`       |
| SLASH        | `keyboard_key_slash`        |
| RSHIFT       | `keyboard_key_rshift`       |
| UP           | `keyboard_key_up`           |
| KP1          | `keyboard_key_kp1`          |
| KP2          | `keyboard_key_kp2`          |
| KP3          | `keyboard_key_kp3`          |
| KP_ENTER     | `keyboard_key_kp_enter`     |
| -            |                             | **Bottom row**                |
| LCTRL        | `keyboard_key_lctrl`        |
| LSUPER       | `keyboard_key_lsuper`       | Left "Windows" key or "Cmd"   |
| LALT         | `keyboard_key_lalt`         | Different phys. pos. on Mac   |
| SPACE        | `keyboard_key_space`        |
| RALT         | `keyboard_key_ralt`         | Different phys. pos. on Mac   |
| RSUPER       | `keyboard_key_rsuper`       | Right "Windows" key or "Cmd"  |
| MENU         | `keyboard_key_menu`         |
| RCTRL        | `keyboard_key_rctrl`        |
| LEFT         | `keyboard_key_left`         |
| DOWN         | `keyboard_key_down`         |
| RIGHT        | `keyboard_key_right`        |
| KP0          | `keyboard_key_kp0`          |
| KP_PERIOD    | `keyboard_key_kp_period`    |
| -            |                             | **Special keys**              |
| VOLUMEUP     | `keyboard_key_volumeup`     |
| VOLUMEDOWN   | `keyboard_key_volumedown`   |
| MUTE         | `keyboard_key_mute`         |
| AUDIONEXT    | `keyboard_key_audionext`    |
| AUDIOPREV    | `keyboard_key_audioprev`    |
| AUDIOSTOP    | `keyboard_key_audiostop`    |
| AUDIOPLAY    | `keyboard_key_audioplay`    |
