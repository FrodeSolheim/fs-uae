from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import fs_uae_launcher.fsui as fsui
from .Config import Config
from .Settings import Settings
from .IconButton import IconButton
from .CDManager import CDManager
from .FloppyManager import FloppyManager
from .I18N import _, ngettext

class FloppySelector(fsui.Group):

    def __init__(self, parent, drive):
        fsui.Group.__init__(self, parent)

        self.cd_mode = False
        self.drive = drive

        self.layout = fsui.HorizontalLayout()

        self.text_field = fsui.TextField(self, "", read_only=True)
        self.layout.add(self.text_field, expand=True)#, expand=True, fill=True)

        self.layout.add_spacer(10)
        self.eject_button = IconButton(self, "eject_button.png")
        self.eject_button.set_tooltip(_("Eject"))
        self.eject_button.on_activate = self.on_eject
        self.layout.add(self.eject_button)

        self.layout.add_spacer(10)
        self.browse_button = IconButton(self, "browse_button.png")
        self.browse_button.set_tooltip(_("Browse"))
        self.browse_button.on_activate = self.on_browse
        self.layout.add(self.browse_button, fill=True)

        self.update_config_key()
        Config.add_listener(self)

    def on_destroy(self):
        Config.remove_listener(self)

    def enable(self, enable):
        self.text_field.enable(enable)
        self.browse_button.enable(enable)
        self.eject_button.enable(enable)

    def on_config(self, key, value):
        if key != self.config_key:
            return
        #if value:
        #    dir, file = os.path.split(value)
        #    self.text_field.set_text(file)
        #else:
        #    self.text_field.set_text(value)
        self.text_field.set_text(value)

    def update_config_key(self):
        if self.cd_mode:
            self.config_key = "cdrom_drive_{0}".format(self.drive)
            self.config_key_sha1 = "x_cdrom_drive_{0}_sha1".format(self.drive)
        else:
            self.config_key = "floppy_drive_{0}".format(self.drive)
            self.config_key_sha1 = "x_floppy_drive_{0}_sha1".format(self.drive)
        self.text_field.set_text(Config.get(self.config_key))

    def set_cd_mode(self, cd_mode):
        self.cd_mode = cd_mode
        self.update_config_key()

    def on_eject(self):
        if self.cd_mode:
            CDManager.eject(self.drive)
        else:
            FloppyManager.eject(self.drive)

    def on_browse(self):
        if self.cd_mode:
            title = _("Choose CD-ROM Image")
            default_dir = Settings.get_cdroms_dir()
        else:
            title = _("Choose Floppy Image")
            default_dir = Settings.get_floppies_dir()

        dialog = fsui.FileDialog(self.get_window(), title,
                directory=default_dir)
        if not dialog.show():
            return
        path = dialog.get_path()

        from .ChecksumTool import ChecksumTool
        checksum_tool = ChecksumTool(self.get_window())
        if self.cd_mode:
            sha1 = ""
            print("FIXME: not calculating CD checksum just yet")
        else:
            sha1 = checksum_tool.checksum(path)

        dir, file = os.path.split(path)
        self.text_field.set_text(file)
        if os.path.normcase(os.path.normpath(dir)) == \
                os.path.normcase(os.path.normpath(default_dir)):
            path = file

        Config.set_multiple([
                (self.config_key, path),
                (self.config_key_sha1, sha1)])
