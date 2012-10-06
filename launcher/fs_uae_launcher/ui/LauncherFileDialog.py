from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import fs_uae_launcher.fsui as fsui
from ..Settings import Settings
from ..I18N import _, ngettext
from .Skin import Skin

class LauncherFileDialog(fsui.FileDialog):

    def __init__(self, parent, title, type, last_path="",
                 multiple=False, dir_mode=False):
        self.dir_mode = dir_mode
        self.settings_key = "last_{0}_dir".format(type)
        directory = ""
        if last_path and last_path not in ["internal"]:
            print("last_path", repr(last_path))
            #if os.path.isdir(last_path):
            #    last_path_dir = last_path
            #else:
            last_path_dir = os.path.dirname(last_path)
            print("last_path_dir", last_path_dir)
            if last_path_dir:
                if os.path.exists(last_path_dir):
                    directory = last_path_dir
            else:
                # file was relative to default directory
                directory = self.get_default_directory(type)
        if not directory:
            value = Settings.get(self.settings_key)
            print(self.settings_key, value)
            if value and os.path.exists(value):
                directory = value
        if not directory:
            directory = self.get_default_directory(type)
        fsui.FileDialog.__init__(self, parent, title, directory,
                dir_mode=dir_mode, multiple=multiple)

    def get_default_directory(self, type):
        if type == "floppy":
            return Settings.get_floppies_dir()
        elif type == "cd":
            return Settings.get_cdroms_dir()
        elif type == "hd":
            return Settings.get_hard_drives_dir()
        elif type == "rom":
            return Settings.get_kickstarts_dir()
        raise Exception("unknown file dialog type")

    def show_modal(self):
        if fsui.FileDialog.show_modal(self):
            path = self.get_path()
            #if os.path.isdir(path):
            #    last_path_dir = path
            #lse:
            last_path_dir = os.path.dirname(path)
            Settings.set(self.settings_key, last_path_dir)
            return True
        return False
