from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import fsui as fsui
from fsgs.amiga.Amiga import Amiga
from ...Config import Config
from ...I18N import _, ngettext
from ...Settings import Settings
from ..IconButton import IconButton

class MemoryWidget(fsui.Group):

    def __init__(self, parent, label, option_key, values):
        fsui.Group.__init__(self, parent)
        self.option_key = option_key
        self.values = values
        self.layout = fsui.HorizontalLayout()

        #label = fsui.Label(self, label + ":")
        #self.layout.add(label, expand=True, margin_right=10)
        self.checkbox = fsui.CheckBox(self, label + ":")
        self.checkbox.on_change = self.on_checkbox_change
        self.layout.add(self.checkbox, expand=True, margin_right=10)

        #choices = [_("Default")]
        choices = []
        for val in values:
            if val >= 1024 * 1024:
                choices.append("{0:0.1f} GB".format(val / (1024 * 1024)))
            elif val >= 10 * 1024:
                choices.append("{0:0.0f} MB".format(val / 1024))
            elif val >= 1024:
                choices.append("{0:0.1f} MB".format(val / 1024))
            else:
                choices.append("{0} KB".format(val))
        self.choice = fsui.Choice(self, choices)
        #self.layout.add_spacer(0, expand=True)
        self.layout.add(self.choice)

        self.initialize_from_config()
        self.set_config_handlers()

    def initialize_from_config(self):
        self.on_config(self.option_key,
                Config.get(self.option_key))

    def set_config_handlers(self):
        self.choice.on_change = self.on_choice_change
        Config.add_listener(self)

    def on_destroy(self):
        Config.remove_listener(self)

    def get_default_value(self):
        return Amiga.get_default_option_value(Config.get("amiga_model"),
                self.option_key)

    def on_checkbox_change(self):
        print("\n\n\n\non_checkbox_change\n\n\n\n")
        if self.checkbox.is_checked():
            if Config.get(self.option_key) == "":
                print("-- setting config to default value")
                Config.set(self.option_key, self.get_default_value())
            #self.choice.enable()
            #self.select_value(Config.get(self.option_key))
        else:
            # use default value
            Config.set(self.option_key, "")
            #self.choice.disable()
            #self.select_value(self.get_default_value())

    #def update_ui(self):
    #    print("\n\n\n\n -- update_ui -- \n\n\n\n")
    #    if self.checkbox.is_checked():
    #        if Config.get(self.option_key) == "":
    #            print("-- setting config to default value")
    #            Config.set(self.option_key, self.get_default_value())
    #        self.choice.enable()
    #        self.select_value(Config.get(self.option_key))
    #    else:
    #        # use default value
    #        Config.set(self.option_key, "")
    #        self.choice.disable()
    #        self.select_value(self.get_default_value())

    def on_choice_change(self):
        print("\n\n\n\n -- on_choice_change -- \n\n\n\n")
        index = self.choice.get_index()
        #if index == 0:
        #    value = ""
        #else:
        #value = str(self.values[index - 1])
        value = str(self.values[index])
        Config.set(self.option_key, value)

    def on_config(self, key, value):
        if key == "amiga_model":
            #self.update_ui()
            self.on_config(self.option_key, Config.get(self.option_key))
        elif key == self.option_key:
            print("-----", key, value)
            if value:
                if not self.checkbox.is_checked():
                    self.checkbox.check(True)
                #self.update_ui()
                if not self.select_value(value):
                    print("WARNING: could not find memory setting")
                self.choice.enable()
            else:
                #
                if self.checkbox.is_checked():
                    self.checkbox.check(False)
                self.select_value(self.get_default_value())
                self.choice.disable()
                #self.update_ui()
                #self.choice.set_index(0)

    def select_value(self, value):
        for i, config in enumerate(self.values):
            if str(config) == value:
                #self.choice.set_index(i + 1)
                self.choice.set_index(i)
                return True
        return False
