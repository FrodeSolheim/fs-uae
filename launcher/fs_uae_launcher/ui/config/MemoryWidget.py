from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import fs_uae_launcher.fsui as fsui
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

        label = fsui.Label(self, label + ":")
        self.layout.add(label, expand=True, margin_right=10)

        choices = ["Default"]
        for val in values:
            if val >= 1024:
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

    def on_choice_change(self):
        index = self.choice.get_index()
        if index == 0:
            value = ""
        else:
            value = str(self.values[index - 1])
        Config.set(self.option_key, value)

    def on_config(self, key, value):
        if key == self.option_key:
            if not value:
                self.choice.set_index(0)
                return
            for i, config in enumerate(self.values):
                if str(config) == value:
                    self.choice.set_index(i + 1)
                    break
            else:
                print("WARNING: could not find memory setting")
