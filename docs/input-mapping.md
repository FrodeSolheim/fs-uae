# Input mapping

All keyboard and joystick events can be mapped to any Amiga keyboard and
joystick action, overriding default settings and (and the setup resulting from
using the joystick_port_n options). A few examples of what you can do:

* Do you want the key ‘S’ on your keyboard to press the ‘R’ key on the Amiga
  keyboard? You can do this with custom input mapping.
* Do you want to create a setup where you can play Pinball Dreams with a
  gamepad? You can map game pad buttons to the Amiga keyboard F1, F2, F3, F4,
  cursor down, left shift and right shift keys.

## General configuration

To map an action, you add a line like this to your configuration file:

    devicename_eventname = actionname

Here is a specific example, mapping the keyboard key q to the fire button on the primary Amiga joystick (joy_1 is the joystick in joystick port 1):

    keyboard_key_q = action_joy_1_fire_button

## Input actions

Examples of input actions are:

    action_joy_1_fire_button          (fire button on joystick in port 1)
    action_key_z                      (press amiga keyboard key Z)
    action_drive_0_insert_floppy_0    (insert disk from floppy image list)

I have put the list of [available actions](input-actions.md) on its own page
due to the large number of actions.

## Mapping joystick events

Here are the different types of event names you can use. joystick_0 is the
first joystick connected, joystick_1 is the second, etc. Similarly, you can
use button_0 (first button), button_1 (second button) and so on:

* joystick_0_button_0
* joystick_0_axis_0_neg
* joystick_0_axis_0_pos
* joystick_0_hat_0_up
* joystick_0_hat_0_down
* joystick_0_hat_0_left
* joystick_0_hat_0_right

For most game pads with a proper universal configuration, you can use universal
event names instead and make the configuration work identically even with
different types of game pads connected. The above configuration is not suited
for this, because button numbers have generally no relation to the physical
placement of the game pad buttons.

## Universal event names

With universal event names, you can map these events instead:

* joystick_0_dpad_left
* joystick_0_dpad_right
* joystick_0_dpad_up
* joystick_0_dpad_down
* joystick_0_lstick_left
* joystick_0_lstick_right
* joystick_0_lstick_up
* joystick_0_lstick_down
* joystick_0_lstick_button
* joystick_0_rstick_left
* joystick_0_rstick_right
* joystick_0_rstick_up
* joystick_0_rstick_down
* joystick_0_rstick_button
* joystick_0_south_button
* joystick_0_west_button
* joystick_0_north_button
* joystick_0_east_button
* joystick_0_start_button
* joystick_0_select_button
* joystick_0_left_shoulder
* joystick_0_right_shoulder
* joystick_0_left_trigger
* joystick_0_right_trigger

## Matching a joystick device

To match a specific joystick device, you can either (as in the above examples)
match joystick by number:

    joystick_0
    joystick_1
    joystick_2
    etc...

Or you can match devices by name:

    logitech_dual_action_usb

The device name is the name of the device as it appears in your operating
system, converted to lower case and all characters other than letters and
numbers converted to underscores. Adjacent underscores are merged into one, and
trailing underscores are stripped.

For instance, a device named:

    Controller (Xbox 360 Wireless Receiver)

will be matched by:

    controller_xbox_360_wireless_receiver

If you have more than game pad of the same model connected, you can match the
other ones with:

    logitech_dual_action_usb_2   (the second connected of this model)
    logitech_dual_action_usb_3   (the third connected of this model)
    etc...

### More examples

If you want to play Pinball Dreams with the first connected game pad, instead
of using keyboard controls, you might want to map something like this:

    joystick_0_left_shoulder = action_key_shift_left
    joystick_0_right_shoulder = action_key_shift_right
    joystick_0_north_button = action_key_f1
    joystick_0_dpad_down = action_key_cursor_down

Along with a few more actions, such as mappings for amiga keys F2, F3, F4 and
Space.

## Mapping keyboard keys

See [keyboard mapping](keyboard-mapping.md).
