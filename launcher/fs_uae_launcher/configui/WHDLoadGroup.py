from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import traceback
import fs_uae_launcher.fsui as fsui
from ..Config import Config
from ..Settings import Settings
from ..IconButton import IconButton
from ..I18N import _, ngettext

class WHDLoadGroup(fsui.Group):

    def __init__(self, parent):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.HorizontalLayout()
        self.layout.padding_right = 20
        self.layout.add_spacer(20)

        image = fsui.Image("fs_uae_launcher:res/whdload_group.png")
        self.image_view = fsui.ImageView(self, image)
        self.layout.add(self.image_view, valign=0.0)

        self.layout.add_spacer(20)

        self.layout2 = fsui.VerticalLayout()
        self.layout.add(self.layout2, fill=True, expand=True)

        label = fsui.HeadingLabel(self, _("WHDLoad Arguments"))
        self.layout2.add(label)#, expand=True, fill=True)
        self.layout2.add_spacer(10)

        self.layout3 = fsui.HorizontalLayout()
        self.layout2.add(self.layout3, fill=True)

        self.text_field = fsui.TextField(self, "")
        self.layout3.add(self.text_field, expand=True)

        self.initialize_from_config()
        self.set_config_handlers()

    def initialize_from_config(self):
        self.on_config("x_whdload_args", Config.get("x_whdload_args"))

    def set_config_handlers(self):
        self.text_field.on_change = self.on_text_change
        Config.add_listener(self)

    def on_destroy(self):
        print("on_destroy")
        Config.remove_listener(self)

    def on_config(self, key, value):
        if key == "x_whdload_args":
            #self.text_field.set_text(value)
            #print(repr(self.text_field.get_text()))
            #print(repr(value))
            if value != self.text_field.get_text():
                self.text_field.set_text(value)

    def on_text_change(self):
        Config.set("x_whdload_args", self.text_field.get_text())
