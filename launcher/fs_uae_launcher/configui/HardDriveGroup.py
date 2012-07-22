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

class HardDriveGroup(fsui.Group):

    def __init__(self, parent, index):
        fsui.Group.__init__(self, parent)
        self.index = index
        self.hard_drive_key = "hard_drive_{0}".format(index)

        self.layout = fsui.HorizontalLayout()
        self.layout.padding_right = 20
        self.layout.add_spacer(20)

        image = fsui.Image("fs_uae_launcher:res/hd_group.png")
        self.image_view = fsui.ImageView(self, image)
        self.layout.add(self.image_view, valign=0.0)

        self.layout.add_spacer(20)

        self.layout2 = fsui.VerticalLayout()
        self.layout.add(self.layout2, fill=True, expand=True)

        label = fsui.HeadingLabel(self, _("Hard Drive {0}").format(index + 1))
        self.layout2.add(label)#, expand=True, fill=True)
        self.layout2.add_spacer(10)

        self.layout3 = fsui.HorizontalLayout()
        self.layout2.add(self.layout3, fill=True)

        self.text_field = fsui.TextField(self, "", read_only=True)
        self.layout3.add(self.text_field, expand=True)

        self.layout3.add_spacer(10)

        self.eject_button = IconButton(self, "eject_button.png")
        self.eject_button.set_tooltip(_("Eject"))
        self.eject_button.on_activate = self.on_eject_button
        self.layout3.add(self.eject_button, fill=True)

        self.layout3.add_spacer(10)

        self.browse_button = IconButton(self, "hd_button.png")
        self.browse_button.set_tooltip(_("Browse for File"))
        self.browse_button.on_activate = self.on_browse_file_button
        self.layout3.add(self.browse_button, fill=True)

        self.layout3.add_spacer(10)

        self.browse_button = IconButton(self, "browse_button.png")
        self.browse_button.set_tooltip(_("Browse for Folder"))
        self.browse_button.on_activate = self.on_browse_folder_button
        self.layout3.add(self.browse_button, fill=True)

        self.initialize_from_config()
        self.set_config_handlers()

    def initialize_from_config(self):
        self.on_config(self.hard_drive_key, Config.get(self.hard_drive_key))

    def set_config_handlers(self):
        Config.add_listener(self)

    def on_destroy(self):
        Config.remove_listener(self)

    def on_eject_button(self):
        Config.set(self.hard_drive_key, "")

    def browse(self, dialog_class):
        default_dir = Settings.get_hard_drives_dir()
        dialog = dialog_class(self.get_window(), _("Choose Hard Drive"),
                directory=default_dir)
        if not dialog.show_modal():
            dialog.destroy()
            return
        path = dialog.get_path()
        self.text_field.set_text(path)
        Config.set(self.hard_drive_key, path)
        dialog.destroy()

    def on_browse_folder_button(self):
        self.browse(fsui.DirDialog)

    def on_browse_file_button(self):
        self.browse(fsui.FileDialog)

    def on_config(self, key, value):
        if key == self.hard_drive_key:
            #print(repr(self.text_field.get_text()))
            #print(repr(value))
            #if value != self.text_field.get_text():
            self.text_field.set_text(value)
