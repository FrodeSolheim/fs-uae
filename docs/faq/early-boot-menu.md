# Early boot menu

Some Amiga models allows access to an early boot menu by pressing both buttons
on the Amiga mouse simultaneously. This requires that that (emulated) mouse
buttons are held very early in the Kickstart boot sequence. If you have trouble
pressing the mouse buttons quickly enough after booting FS-UAE, you can add the
following option to give you an extra second to press the buttons:

    uae_cpu_reset_pause = yes

If you are stuck on a laptop with only a trackpad, pressing both mouse buttons
on the same time might not be feasible. In this case, you can temporarily map a
keyboard key to the right click action, so you can hold down this key while
also holding down the left mouse button.

    keyboard_key_tab = action_joy_0_2nd_button

In this example, you can press the tab key and hold the left mouse button to
activate the early boot menu. See the
[keyboard mapping documentation](../keyboard-mapping.md) for other keys you can
map. You can also combine this option with `uae_cpu_reset_pause` to give you
some additional time.
