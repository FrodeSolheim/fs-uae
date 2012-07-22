from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import fs_uae_launcher.fsui as fsui
from ..Config import Config
from ..Settings import Settings
from ..DeviceManager import DeviceManager
from ..I18N import _, ngettext

joystick_mode_values = ["nothing", "mouse", "joystick", "cd32 gamepad"]
joystick_mode_titles = [_("Nothing"), _("Mouse"), _("Joystick"),
        _("CD32 Pad")]

joystick_values = ["none", "mouse", "keyboard"]
joystick_values_initialized = False

class InputSelector(fsui.Group):

    def __init__(self, parent, port):
        global joystick_values_initialized
        self.port = port
        self.device_option_key = "joystick_port_{0}".format(port)
        self.mode_option_key = "joystick_port_{0}_mode".format(port)

        fsui.Group.__init__(self, parent)
        self.layout = fsui.HorizontalLayout()

        self.mode_choice = fsui.Choice(self, joystick_mode_titles)
        #if port == 0:
        #    self.mode_choice.set_index(1)
        #elif port == 1:
        #    self.mode_choice.set_index(2)
        self.layout.add(self.mode_choice)

        self.layout.add_spacer(10)

        devices = [_("No Host Device"), _("Mouse"),
                _("Cursor Keys and Right Ctrl/Alt")]
        for i, name in enumerate(DeviceManager.get_joystick_names()):
            devices.append(name)
            if not joystick_values_initialized:
                joystick_values.append(DeviceManager.device_ids[i])
        joystick_values_initialized = True

        self.device_choice = fsui.ComboBox(self, devices, read_only=True)
        #if port == 0:
        #    self.device_choice.set_index(1)
        #elif port == 1:
        #    if len(devices) > 3:
        #        self.device_choice.set_index(3)
        #    else:
        #        self.device_choice.set_index(2)

        self.layout.add(self.device_choice, expand=True)

        #Config.add_listener(self)

        self.initialize_from_config()
        self.set_config_handlers()

    def initialize_from_config(self):
        self.on_config(self.device_option_key,
                Config.get(self.device_option_key))
        self.on_config(self.mode_option_key,
                Config.get(self.mode_option_key))

    def set_config_handlers(self):
        self.mode_choice.on_change = self.on_mode_change
        self.device_choice.on_change = self.on_device_change
        Config.add_listener(self)

    def on_destroy(self):
        print("on_destroy")
        Config.remove_listener(self)

    def on_mode_change(self):
        index = self.mode_choice.get_index()
        Config.set(self.mode_option_key, joystick_mode_values[index])

    def on_device_change(self):
        index = self.device_choice.get_index()
        if index == 1:
            # mouse
            Config.set(self.mode_option_key, "mouse")
        elif index > 1:
            # joystick / keyboard emulation
            prev = Config.get(self.mode_option_key)
            if prev == "nothing" or prev == "mouse":
                if Config.get("amiga_model") == "CD32":
                    Config.set(self.mode_option_key, "cd32 gamepad")
                else:
                    Config.set(self.mode_option_key, "joystick")
        Config.set(self.device_option_key, joystick_values[index])

    def on_config(self, key, value):
        if key == self.mode_option_key:
            for i, config in enumerate(joystick_mode_values):
                if config == value:
                    self.mode_choice.set_index(i)
                    break
            else:
                print("FIXME: could not set model")
        elif key == self.device_option_key:
            #print(joystick_values)
            value_lower = value.lower()
            for i, name in enumerate(joystick_values):
                if value_lower == name.lower():
                    self.device_choice.set_index(i)
                    break
