from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import webbrowser
import fs_uae_launcher.fsui as fsui
from ...Config import Config
from ...DeviceManager import DeviceManager
from ...I18N import _, ngettext
from ...Signal import Signal
from ..IconButton import IconButton
from ..HelpButton import HelpButton

class InputSelector(fsui.Group):

    def __init__(self, parent, port):
        #global joystick_values_initialized
        self.port = port
        self.device_option_key = "joystick_port_{0}".format(port)
        self.mode_option_key = "joystick_port_{0}_mode".format(port)
        self.autofire_mode_option_key = "joystick_port_{0}_autofire".format(
                port)

        fsui.Group.__init__(self, parent)
        self.layout = fsui.HorizontalLayout()

        self.joystick_mode_values = ["nothing", "mouse", "joystick",
                "cd32 gamepad"]
        self.joystick_mode_titles = [_("Nothing"), _("Mouse"), _("Joystick"),
                _("CD32 Pad")]

        self.mode_choice = fsui.Choice(self, self.joystick_mode_titles)
        self.layout.add(self.mode_choice)
        self.layout.add_spacer(10)
        #else:
        #    self.mode_choice = None
        if port >= 4:
            self.mode_choice.disable()

        #devices = ["", _("No Host Device"), _("Mouse"),
        #        _("Cursor Keys and Right Ctrl/Alt")]
        #for i, name in enumerate(DeviceManager.get_joystick_names()):
        #    devices.append(name)
        #    if not self.joystick_values_initialized:
        #        self.joystick_values.append(DeviceManager.device_ids[i])
        #self.joystick_values_initialized = True

        self.device_choice = fsui.ComboBox(self, [""], read_only=True)
        self.rebuild_device_list()
        self.device_choice.set_index(0)
        #print(self.device_choice.get_index())
        self.layout.add(self.device_choice, expand=True)

        if port < 4:
            self.autofire_button = IconButton(self, "autofire_off_16.png")
            self.autofire_button.on_activate = self.on_autofire_button
            self.layout.add(self.autofire_button, margin_left=10)
        else:
            self.autofire_button = None

            self.help_button = HelpButton(self,
                    "http://fengestad.no/fs-uae/custom-joystick-port")
            self.layout.add(self.help_button, margin_left=10)

        self.initialize_from_config()
        self.set_config_handlers()

    def rebuild_device_list(self):
        #self.device_choice.set_items([""])
        #self.device_choice.set_text("")
        self.joystick_values = ["", "none", "mouse", "keyboard"]
        devices = ["", _("No Host Device"), _("Mouse"),
                _("Cursor Keys and Right Ctrl/Alt")]
        for i, name in enumerate(DeviceManager.get_joystick_names()):
            devices.append(name)
            self.joystick_values.append(DeviceManager.device_ids[i])
        self.device_choice.set_items(devices)

    def initialize_from_config(self):
        self.on_config(self.device_option_key,
                Config.get(self.device_option_key))
        self.on_config(self.mode_option_key,
                Config.get(self.mode_option_key))
        self.on_config(self.autofire_mode_option_key,
                Config.get(self.autofire_mode_option_key))

    def set_config_handlers(self):
        if self.mode_choice is not None:
            self.mode_choice.on_change = self.on_mode_change
        self.device_choice.on_change = self.on_device_change
        Config.add_listener(self)
        Signal.add_listener("settings_updated", self)
        Signal.add_listener("device_list_updated", self)

    def on_destroy(self):
        print("on_destroy")
        Config.remove_listener(self)
        Signal.remove_listener("settings_updated", self)

    def on_mode_change(self):
        if self.mode_choice is not None:
            index = self.mode_choice.get_index()
            value = self.joystick_mode_values[index]
            self.set_value_or_default(value)

    def set_value_or_default(self, value):
        if self.port == 0:
            if value == "mouse":
                value = ""
        elif self.port == 1:
            if Config.get("amiga_model").startswith("CD32"):
                default = "cd32 gamepad"
            else:
                default = "joystick"
            if value == default:
                value = ""
        else:
            if value == "nothing":
                value == ""
        if Config.get(self.mode_option_key) != value:
            Config.set(self.mode_option_key, value)

    def on_device_change(self):
        index = self.device_choice.get_index()

        value = self.joystick_values[index]
        for port in range(4):
            if self.port == port:
                continue
            key = "joystick_port_{0}".format(port)
            if Config.get(key) == value:
                Config.set(key, "")
        Config.set(self.device_option_key, value)

    def on_autofire_button(self):
        if Config.get(self.autofire_mode_option_key) == "1":
            Config.set(self.autofire_mode_option_key, "")
        else:
            Config.set(self.autofire_mode_option_key, "1")

    def get_calculated_mode(self, port):
        value = Config.get("joystick_port_{0}_mode".format(port))
        if not value:
            if port == 0:
                return "mouse"
            elif port == 1:
                if Config.get("amiga_model").startswith("CD32"):
                    return "cd32 gamepad"
                else:
                    return "joystick"
            return "nothing"
        return value

    def on_config(self, key, value):
        if key == "amiga_model":
            value = Config.get("joystick_port_{0}_mode".format(self.port))
            self.set_value_or_default(value)

        if key == self.mode_option_key or key == "amiga_model":
            value = self.get_calculated_mode(self.port)
            for i, config in enumerate(self.joystick_mode_values):
                if config == value:
                    if self.mode_choice is not None:
                        self.mode_choice.set_index(i)
                        if self.port >= 4:
                            self.device_choice.enable(i != 0)
                    break
            else:
                print("FIXME: could not set mode")
        elif key == self.device_option_key or key == "amiga_model":
            #print(self.joystick_values)
            value_lower = value.lower()
            for i, name in enumerate(self.joystick_values):
                if value_lower == name.lower():
                    self.device_choice.set_index(i)
                    break
        elif key == self.autofire_mode_option_key:
            if self.autofire_button is not None:
                if value == "1":
                    self.autofire_button.set_tooltip(_("Auto-Fire is On"))
                    self.autofire_button.set_icon_name("autofire_on_16.png")
                else:
                    self.autofire_button.set_tooltip(_("Auto-Fire is Off"))
                    self.autofire_button.set_icon_name("autofire_off_16.png")

        # this is intended to catch all config changes for all ports (both
        # mode and device) to update the defaults
        if key.startswith("joystick_port_") or key == "amiga_model":
            self.update_default_device()

    def on_device_list_updated_signal(self):
        #print(self.device_choice.get_index())
        had_default = (self.device_choice.get_index() == 0)
        self.rebuild_device_list()
        self.update_default_device(had_default=had_default)

    def on_settings_updated_signal(self):
        self.update_default_device()

    def update_default_device(self, had_default=None):
        config = {}
        for port in range(4):
            key = "joystick_port_{0}".format(port)
            if self.port == port:
                config[key] = ""
            else:
                config[key] = Config.get(key)
            key = "joystick_port_{0}_mode".format(port)
            config[key] = self.get_calculated_mode(port)
        device = DeviceManager.get_device_for_port(config, self.port)
        default_description = _("Default ({0})").format(device.name)
        #print("default_description = ", default_description)

        if had_default is None:
            had_default = (self.device_choice.get_index() == 0)
        #print("had default", had_default, self.device_choice.get_index())
        self.device_choice.set_item_text(0, default_description)
        #print("had_default", had_default)
        if had_default:
            #print("set text for", self.port, default_description)
            #self.device_choice.set_index(1)
            self.device_choice.set_text(default_description)
            self.device_choice.set_index(0)
        #print(self.device_choice.get_index())
