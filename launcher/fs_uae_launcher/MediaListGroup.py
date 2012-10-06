from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import fs_uae_launcher.fsui as fsui
from .Config import Config
from .IconButton import IconButton
from .Settings import Settings
from .FloppyManager import FloppyManager
from .I18N import _, ngettext

class MediaListGroup(fsui.Group):

    def __init__(self, parent, cd_mode):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.VerticalLayout()

        self.cd_mode = cd_mode

        #if cd_mode:
        #    image = fsui.Image("fs_uae_launcher:res/cd_group.png")
        #else:
        #    image = fsui.Image("fs_uae_launcher:res/floppy_group.png")
        #self.image_view = fsui.ImageView(self, image)
        #self.layout.add(self.image_view, valign=0.0)

        #self.layout.add_spacer(20)

        #self.layout2 = fsui.VerticalLayout()
        #self.layout.add(self.layout2, fill=True, expand=True)

        self.heading_label = fsui.HeadingLabel(self, _("Swap List"))
        self.layout.add(self.heading_label, margin=10)
        self.layout.add_spacer(0)

        self.layout.add(self.create_list_group(), expand=True, fill=True)

        self.update_list()
        Config.add_listener(self)

    def on_destroy(self):
        Config.remove_listener(self)

    def create_list_group(self):
        hori_layout = fsui.HorizontalLayout()
        #hori_layout.padding_left = 20
        #hori_layout.padding_right = 20

        self.text = fsui.TextArea(self, read_only=True)
        #self.text.set_min_height(160)
        hori_layout.add(self.text, expand=True, fill=True, margin=10,
                margin_right=0)

        vert_layout = fsui.VerticalLayout()
        hori_layout.add(vert_layout, fill=True)

        add_button = IconButton(self, "add_button.png")
        add_button.set_tooltip(_("Add Files to List"))
        add_button.disable()
        #add_button.on_activate = self.on_clear_list
        vert_layout.add(add_button, margin=10)

        remove_button = IconButton(self, "remove_button.png")
        remove_button.set_tooltip(_("Remove Selected Files"))
        remove_button.disable()
        #add_button.on_activate = self.on_clear_list
        vert_layout.add(remove_button, margin=10)

        clear_button = IconButton(self, "clear_button.png")
        clear_button.set_tooltip(_("Clear List"))
        clear_button.on_activate = self.on_clear_list
        vert_layout.add(clear_button, margin=10)

        return hori_layout

    def on_config(self, key, value):
        if self.cd_mode:
            key_prefix = "cdrom_image_"
        else:
            key_prefix = "floppy_image_"
        if key.startswith(key_prefix): # and key.endswith("_name"):
            self.update_list()

    def update_list(self):
        print("update_list")
        text = u""
        for i in range(20):
            if self.cd_mode:
                key = "cdrom_image_{0}".format(i)
            else:
                key = "floppy_image_{0}".format(i)
            value = Config.get(key)
            print(i, key, value)
            text = text + u"{0:02d}: {1}\n".format(i, value)
        self.text.set_text(text)

    def on_clear_list(self):
        if self.cd_mode:
            from .CDManager import CDManager
            CDManager.clear_cdrom_list()
        else:
            from .FloppyManager import FloppyManager
            FloppyManager.clear_floppy_list()
