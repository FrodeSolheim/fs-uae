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

class KickstartGroup(fsui.Group):

    def __init__(self, parent):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.HorizontalLayout()
        self.layout.padding_right = 20
        self.layout.add_spacer(20)

        image = fsui.Image("fs_uae_launcher:res/kickstart.png")
        self.image_view = fsui.ImageView(self, image)
        self.layout.add(self.image_view, valign=0.0)

        self.layout.add_spacer(20)

        self.layout2 = fsui.VerticalLayout()
        self.layout.add(self.layout2, fill=True, expand=True)

        label = fsui.HeadingLabel(self, _("Kickstart"))
        self.layout2.add(label)#, expand=True, fill=True)
        self.layout2.add_spacer(10)

        self.layout3 = fsui.HorizontalLayout()
        self.layout2.add(self.layout3, fill=True)

        kickstart_types = [_("Default"), _("Custom"), _("Internal")]
        self.kickstart_type_choice = fsui.Choice(self, kickstart_types)
        self.layout3.add(self.kickstart_type_choice)

        self.layout3.add_spacer(10)

        self.text_field = fsui.TextField(self, "", read_only=True)
        self.layout3.add(self.text_field, expand=True)#, expand=True, fill=True)

        self.layout3.add_spacer(10)

        self.browse_button = IconButton(self, "browse_button.png")
        self.browse_button.set_tooltip(_("Browse"))
        self.browse_button.on_activate = self.on_browse_button
        self.layout3.add(self.browse_button, fill=True)

        #self.browse_button = fsui.Button(self, _("Browse"))
        #self.browse_button.on_activate = self.on_browse_button
        #self.layout3.add(self.browse_button)

        self.initialize_from_config()
        self.set_config_handlers()

    def initialize_from_config(self):
        self.on_config("x_kickstart_type", Config.get("x_kickstart_type"))
        self.on_config("kickstart_file", Config.get("kickstart_file"))

    def set_config_handlers(self):
        self.kickstart_type_choice.on_change = self.on_kickstart_type_change
        Config.add_listener(self)

    def on_destroy(self):
        print("on_destroy")
        Config.remove_listener(self)

    def on_kickstart_type_change(self):
        index = self.kickstart_type_choice.get_index()
        if index == 0:
            if Config.get("x_kickstart_type") == "default":
                return
            #Config.set("kickstart_file", "")
            #Config.set("x_kickstart_sha1", sha1)
            #Config.set("x_kickstart_name", file)
            Config.set("x_kickstart_type", "default")
            Config.update_kickstart()
        elif index == 2:
            if Config.get("x_kickstart_type") == "internal":
                return
            Config.set("x_kickstart_type", "internal")
            Config.set("kickstart_file", "internal")
            Config.set("x_kickstart_sha1", "")
            Config.set("x_kickstart_name", "")
        else:
            if Config.get("x_kickstart_type") == "custom":
                return
            Config.set("x_kickstart_type", "custom")
            # FIXME
            Config.set("kickstart_file", "")
            Config.set("x_kickstart_sha1", "")
            Config.set("x_kickstart_name", "")

    def on_browse_button(self):
        default_dir = Settings.get_kickstarts_dir()
        dialog = fsui.FileDialog(self.get_window(), _("Choose Kickstart ROM"),
                directory=default_dir)
        if not dialog.show():
            return
        path = dialog.get_path()

        from ..ChecksumDialog import ChecksumDialog
        dialog = ChecksumDialog(self.get_window(), path)
        dialog.show()
        try:
            sha1 = dialog.checksum(path)
        except Exception:
            traceback.print_exc()
            dialog.destroy()
            return
        dialog.destroy()

        dir, file = os.path.split(path)
        self.text_field.set_text(file)
        if os.path.normcase(os.path.normpath(dir)) == \
                os.path.normcase(os.path.normpath(default_dir)):
            path = file

        Config.set("kickstart_file", path)
        Config.set("x_kickstart_sha1", sha1)
        Config.set("x_kickstart_name", file)
        Config.set("x_kickstart_type", "custom")

    def on_config(self, key, value):
        if key == "x_kickstart_type":
            if value == "default":
                self.kickstart_type_choice.set_index(0)
            elif value == "custom":
                self.kickstart_type_choice.set_index(1)
            elif value == "internal":
                self.kickstart_type_choice.set_index(2)
        elif key == "kickstart_file":
            if value == "internal":
                Config.set("x_kickstart_type", "internal")
                self.text_field.set_text("")
            elif value:
                dir, file = os.path.split(value)
                self.text_field.set_text(file)
            else:
                self.text_field.set_text(value)
