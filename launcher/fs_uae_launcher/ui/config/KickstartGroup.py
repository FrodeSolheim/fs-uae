from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import traceback
import fs_uae_launcher.fsui as fsui
from ...Config import Config
from ...ChecksumTool import ChecksumTool
from ...Settings import Settings
from ...IconButton import IconButton
from ...I18N import _, ngettext

class KickstartGroup(fsui.Group):

    def __init__(self, parent):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.VerticalLayout()

        heading_label = fsui.HeadingLabel(self, _("Kickstart ROM"))
        self.layout.add(heading_label, margin=10)
        self.layout.add_spacer(0)

        hori_layout = fsui.HorizontalLayout()
        self.layout.add(hori_layout, fill=True)

        kickstart_types = [_("Default"), _("Custom"), _("Internal")]
        self.kickstart_type_choice = fsui.Choice(self, kickstart_types)
        hori_layout.add(self.kickstart_type_choice, margin=10)

        self.text_field = fsui.TextField(self, "", read_only=True)
        hori_layout.add(self.text_field, expand=True, margin=10)

        self.browse_button = IconButton(self, "browse_button.png")
        self.browse_button.set_tooltip(_("Browse"))
        self.browse_button.on_activate = self.on_browse_button
        hori_layout.add(self.browse_button, margin=10)

        heading_label = fsui.Label(self, _("Extended ROM"))
        self.layout.add(heading_label, margin=10)
        self.layout.add_spacer(0)

        hori_layout = fsui.HorizontalLayout()
        self.layout.add(hori_layout, fill=True)

        kickstart_types = [_("Default"), _("Custom")]
        self.ext_rom_type_choice = fsui.Choice(self, kickstart_types)
        hori_layout.add(self.ext_rom_type_choice, margin=10)

        self.ext_text_field = fsui.TextField(self, "", read_only=True)
        hori_layout.add(self.ext_text_field, expand=True, margin=10)

        self.ext_browse_button = IconButton(self, "browse_button.png")
        self.ext_browse_button.set_tooltip(_("Browse"))
        self.ext_browse_button.on_activate = self.on_ext_browse_button
        hori_layout.add(self.ext_browse_button, margin=10)

        self.initialize_from_config()
        self.set_config_handlers()

    def initialize_from_config(self):
        self.on_config("kickstart_file", Config.get("kickstart_file"))
        self.on_config("kickstart_ext_file", Config.get("kickstart_ext_file"))

    def set_config_handlers(self):
        self.kickstart_type_choice.on_change = self.on_kickstart_type_change
        self.ext_rom_type_choice.on_change = self.on_ext_rom_type_change
        Config.add_listener(self)

    def on_destroy(self):
        print("on_destroy")
        Config.remove_listener(self)

    def on_kickstart_type_change(self):
        index = self.kickstart_type_choice.get_index()
        if index == 0:
            if Config.get("kickstart_file") == "":
                return
            Config.set("kickstart_file", "")
        elif index == 2:
            if Config.get("kickstart_file") == "internal":
                return
            Config.set("kickstart_file", "internal")
        else:
            Config.set("kickstart_file", Config.get("x_kickstart_file"))
        Config.update_kickstart()

    def on_ext_rom_type_change(self):
        index = self.ext_rom_type_choice.get_index()
        if index == 0:
            if Config.get("kickstart_ext_file") == "":
                return
            Config.set("kickstart_ext_file", "")
        else:
            Config.set("kickstart_ext_file",
                    Config.get("x_kickstart_ext_file"))
        Config.update_kickstart()

    def on_browse_button(self, extended=False):
        default_dir = Settings.get_kickstarts_dir()
        if extended:
            title = _("Choose Extended ROM")
        else:
            title = _("Choose Kickstart ROM")
        dialog = fsui.FileDialog(self.get_window(), title,
                directory=default_dir)
        if not dialog.show():
            return
        path = dialog.get_path()

        checksum_tool = ChecksumTool(self.get_window())
        sha1 = checksum_tool.checksum_rom(path)

        dir, file = os.path.split(path)
        if extended:
            self.ext_text_field.set_text(file)
        else:
            self.text_field.set_text(file)
        if os.path.normcase(os.path.normpath(dir)) == \
                os.path.normcase(os.path.normpath(default_dir)):
            path = file

        if extended:
            Config.set_multiple([
                    ("kickstart_ext_file", path),
                    ("x_kickstart_ext_file", path),
                    ("x_kickstart_ext_file_sha1", sha1)])
        else:
            Config.set_multiple([
                    ("kickstart_file", path),
                    ("x_kickstart_file", path),
                    ("x_kickstart_file_sha1", sha1)])

    def on_ext_browse_button(self):
        return self.on_browse_button(extended=True)

    def on_config(self, key, value):
        if key == "kickstart_file":
            if value == "internal":
                self.text_field.set_text("")
                self.kickstart_type_choice.set_index(2)
            elif value:
                dir, file = os.path.split(value)
                self.text_field.set_text(file)
                self.kickstart_type_choice.set_index(1)
            else:
                self.text_field.set_text("")
                self.kickstart_type_choice.set_index(0)
        elif key == "kickstart_ext_file":
            if value:
                dir, file = os.path.split(value)
                self.ext_text_field.set_text(file)
                self.ext_rom_type_choice.set_index(1)
            else:
                self.ext_text_field.set_text("")
                self.ext_rom_type_choice.set_index(0)
