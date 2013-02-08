from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import sys
import subprocess
import fs_uae_launcher.fsui as fsui
from ...Config import Config
from ...DeviceManager import DeviceManager
from ...I18N import _, ngettext
from ...Settings import Settings

joystick_mode_values = ["nothing", "mouse", "joystick"]
joystick_mode_titles = [_("Nothing"), _("Mouse"), _("Joystick")]

joystick_values = ["none", "mouse", "keyboard"]

class PreferredJoysticksGroup(fsui.Group):

    def __init__(self, parent):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.HorizontalLayout()
        self.layout.padding_left = 20
        self.layout.padding_right = 20

        image = fsui.Image("fs_uae_launcher:res/joystick.png")
        self.image_view = fsui.ImageView(self, image)
        self.layout.add(self.image_view, valign=0.0)

        self.layout.add_spacer(20)

        self.layout2 = fsui.VerticalLayout()
        self.layout.add(self.layout2, fill=True, expand=True)

        heading = _("Preferred Joystick")
        label = fsui.HeadingLabel(self, heading)
        self.layout2.add(label)

        #self.layout2.add_spacer(10)
        #label = fsui.Label(self, _("Preferred device for primary joystick:"))
        #self.layout2.add(label)

        self.layout2.add_spacer(10)
        selector = PreferredJoystickSelector(self, 0)
        self.layout2.add(selector, fill=True)

        self.layout2.add_spacer(10)
        label = fsui.Label(self, _("Preferred device for secondary joystick:"))
        self.layout2.add(label)

        self.layout2.add_spacer(10)
        selector = PreferredJoystickSelector(self, 1)
        self.layout2.add(selector, fill=True)


class PreferredJoystickSelector(fsui.Group):

    def __init__(self, parent, index):
        self.index = index
        if index:
            self.key = "secondary_joystick"
        else:
            self.key = "primary_joystick"

        fsui.Group.__init__(self, parent)
        self.layout = fsui.HorizontalLayout()

        devices = []
        devices.append(get_keyboard_title())
        for i, name in enumerate(DeviceManager.get_joystick_names()):
            devices.append(name)
        self.device_choice = fsui.ComboBox(self, devices)

        self.layout.add(self.device_choice, expand=True)

        #Config.add_listener(self)

        self.initialize_from_settings()
        self.set_settings_handlers()

    def initialize_from_settings(self):
        self.on_setting(self.key, Settings.get(self.key))

    def set_settings_handlers(self):
        self.device_choice.on_change = self.on_device_change
        Settings.add_listener(self)

    def on_destroy(self):
        Settings.remove_listener(self)

    def on_device_change(self):
        value = self.device_choice.get_text()
        print("on_device_change", value)
        if value == get_keyboard_title():
            value = "keyboard"
        Settings.set(self.key, value)

    def on_setting(self, key, value):
        if key == self.key:
            if value == "keyboard":
                value = get_keyboard_title()
            self.device_choice.set_text(value)

def get_keyboard_title():
    return _("Cursor Keys and Right Ctrl/Alt")
