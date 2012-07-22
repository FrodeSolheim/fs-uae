from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import fs_uae_launcher.fsui as fsui
from .Config import Config
from .Settings import Settings
from .I18N import _, ngettext

class FloppyManager:

    @classmethod
    def clear_all(cls):
        for i in range(4):
            cls.eject(i)
        cls.clear_floppy_list()

    @classmethod
    def eject(cls, drive):
        Config.set("floppy_drive_{0}".format(drive), "")
        Config.set("x_floppy_drive_{0}_sha1".format(drive), "")
        #Config.set("x_floppy_drive_{0}_name".format(drive), "")

    @classmethod
    def clear_floppy_list(cls):
        for i in range(20):
            Config.set("floppy_image_{0}".format(i), "")
            #Config.set("x_floppy_image_{0}_name".format(i), "")
            Config.set("x_floppy_image_{0}_sha1".format(i), "")

    @classmethod
    def multiselect(cls, parent=None):
        default_dir = Settings.get_floppies_dir()
        dialog = fsui.FileDialog(parent, _("Select Multiple Floppies"),
                directory=default_dir, multiple=True)
        if not dialog.show():
            return
        paths = dialog.get_paths()
        paths.sort()

        from .ChecksumDialog import ChecksumDialog
        dialog = ChecksumDialog(parent, "")
        #dialog.show()
        for i, path in enumerate(paths):
            try:
                sha1 = dialog.checksum(path)
            except Exception:
                traceback.print_exc()
                dialog.destroy()
                return

            dir, file = os.path.split(path)
            if os.path.normcase(os.path.normpath(dir)) == \
                    os.path.normcase(os.path.normpath(default_dir)):
                path = file

            if i < 4:
                Config.set("floppy_drive_{0}".format(i), path)
                Config.set("x_floppy_drive_{0}_sha1".format(i), sha1)
                #Config.set("x_floppy_drive_{0}_name".format(i), file)
            Config.set("floppy_image_{0}".format(i), path)
            Config.set("x_floppy_image_{0}_sha1".format(i), sha1)
            #Config.set("x_floppy_image_{0}_name".format(i), file)
        # blank the rest of the drives
        for i in range(len(paths), 4):
            Config.set("floppy_drive_{0}".format(i), "")
            Config.set("x_floppy_drive_{0}_sha1".format(i), "")
            #Config.set("x_floppy_drive_{0}_name".format(i), "")
        # blank the rest of the image list
        for i in range(len(paths), 20):
            Config.set("floppy_image_{0}".format(i), "")
            Config.set("x_floppy_image_{0}_sha1".format(i), "")
            #Config.set("x_floppy_image_{0}_name".format(i), "")
        #dialog.destroy()
