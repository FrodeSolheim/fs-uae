from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import fs_uae_launcher.fsui as fsui
from .Config import Config
from .IconButton import IconButton
from .Settings import Settings
from .I18N import _, ngettext

class ConfigGroup(fsui.Group):

    def __init__(self, parent):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.HorizontalLayout()
        self.layout.padding_left = 20
        self.layout.padding_right = 20

        image = fsui.Image("fs_uae_launcher:res/configuration_group.png")
        self.image_view = fsui.ImageView(self, image)
        self.layout.add(self.image_view, valign=0.0)

        self.layout.add_spacer(20)

        self.layout2 = fsui.VerticalLayout()
        self.layout.add(self.layout2, fill=True, expand=True)

        #heading = _("Configuration")
        #label = fsui.HeadingLabel(self, heading)
        #self.layout2.add(label)

        #self.layout3.add_spacer(10)
        self.config_name_field = fsui.TextField(self)
        self.layout2.add(self.config_name_field, fill=True)

        self.layout2.add_spacer(10)
        self.layout3 = fsui.HorizontalLayout()
        self.layout2.add(self.layout3, fill=True)

        self.new_button = IconButton(self, "new_button.png")
        self.new_button.set_tooltip(_("New Configuration"))
        self.new_button.on_activate = self.on_new_button
        self.layout3.add(self.new_button, fill=True)

        self.layout3.add_spacer(10)
        self.open_button = IconButton(self, "open_button.png")
        self.open_button.set_tooltip(_("Open Configuration"))
        self.open_button.disable()
        #self.open_button.on_activate = self.on_open_button
        self.layout3.add(self.open_button, fill=True)

        self.layout3.add_spacer(10)
        self.save_button = IconButton(self, "save_button.png")
        self.save_button.disable()
        self.save_button.set_tooltip(_("Save Configuration"))
        self.save_button.on_activate = self.on_save_button
        self.layout3.add(self.save_button, fill=True)

        self.layout3.add_spacer(0, expand=True)
        #self.layout3.add_spacer(10)
        #self.config_name_field = fsui.TextField(self)
        #self.layout3.add(self.config_name_field, expand=True)

        self.layout3.add_spacer(10)
        self.hw_button = IconButton(self, "hardware_button.png")
        self.hw_button.set_tooltip(_("Amiga Hardware Options"))
        self.hw_button.on_activate = self.on_hw_button
        self.layout3.add(self.hw_button)

        self.layout3.add_spacer(10)
        self.input_button = IconButton(self, "joystick_button.png")
        self.input_button.set_tooltip(_("Input Options"))
        self.input_button.on_activate = self.on_input_button
        self.layout3.add(self.input_button)

        self.layout3.add_spacer(10)
        self.hds_button = IconButton(self, "hd_button.png")
        self.hds_button.set_tooltip(_("Hard Drives"))
        self.hds_button.on_activate = self.on_hds_button
        self.layout3.add(self.hds_button)

        self.layout3.add_spacer(10)
        self.more_button = IconButton(self, "more_button.png")
        self.more_button.set_tooltip(_("More Configuration Options"))
        self.more_button.on_activate = self.on_config_button
        self.layout3.add(self.more_button, fill=True)

        #self.save_as_button = IconButton(self, "save_as_button.png")
        #self.layout3.add(self.save_as_button)
        #self.layout3.add_spacer(10)

        self.on_setting("config_name", Settings.get("config_name"))
        self.config_name_field.on_change = self.on_config_name_change
        Settings.add_listener(self)

    def on_destroy(self):
        Settings.remove_listener(self)

    def on_setting(self, key, value):
        if key == "config_name":
            if value != self.config_name_field.get_text():
                self.config_name_field.set_text(value)

    def on_config_name_change(self):
        text = self.config_name_field.get_text().strip()
        Settings.set("config_name", text)
        # FIXME: remove
        Config.set("title", text)

    def on_new_button(self):
        Config.load_default_config()

    def on_save_button(self):
        pass

    def on_config_button(self):
        from .configui.ConfigDialog import ConfigDialog
        ConfigDialog.run(self.get_window(), ConfigDialog.CUSTOM_OPTIONS)

    def on_hds_button(self):
        from .configui.ConfigDialog import ConfigDialog
        ConfigDialog.run(self.get_window(), ConfigDialog.HARD_DRIVES)

    def on_hw_button(self):
        from .configui.ConfigDialog import ConfigDialog
        ConfigDialog.run(self.get_window(), ConfigDialog.HARDWARE)

    def on_input_button(self):
        from .configui.ConfigDialog import ConfigDialog
        ConfigDialog.run(self.get_window(), ConfigDialog.INPUT)
