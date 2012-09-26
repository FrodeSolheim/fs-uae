from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import fs_uae_launcher.fsui as fsui
from .ui.LauncherFileDialog import LauncherFileDialog
from .Amiga import Amiga
from .Config import Config
from .Settings import Settings
from .I18N import _, ngettext

class CDManager:

    @classmethod
    def clear_all(cls):
        for i in range(1):
            cls.eject(i)
        cls.clear_cdrom_list()

    @classmethod
    def eject(cls, drive):
        values = []
        values.append(("cdrom_drive_{0}".format(drive), ""))
        values.append(("x_cdrom_drive_{0}_sha1".format(drive), ""))
        Config.set_multiple(values)

    @classmethod
    def clear_cdrom_list(cls):
        values = []
        for i in range(Amiga.MAX_CDROM_IMAGES):
            values.append(("cdrom_image_{0}".format(i), ""))
            values.append(("x_cdrom_image_{0}_sha1".format(i), ""))
        Config.set_multiple(values)

    @classmethod
    def multiselect(cls, parent=None):
        default_dir = Settings.get_cdroms_dir()
        dialog = LauncherFileDialog(parent, _("Select Multiple CD-ROMs"),
                "cd", multiple=True)

        if not dialog.show():
            return
        paths = dialog.get_paths()
        paths.sort()

        from .ChecksumTool import ChecksumTool
        checksum_tool = ChecksumTool(parent)
        for i, path in enumerate(paths):
            #sha1 = checksum_tool.checksum(path)
            sha1 = ""
            print("FIXME: not calculating CD checksum just yet")

            dir, file = os.path.split(path)
            if os.path.normcase(os.path.normpath(dir)) == \
                    os.path.normcase(os.path.normpath(default_dir)):
                path = file

            if i < 1:
                Config.set_multiple([
                        ("cdrom_drive_{0}".format(i), path),
                        ("x_cdrom_drive_{0}_sha1".format(i), sha1)])
            Config.set_multiple([
                    ("cdrom_image_{0}".format(i), path),
                    ("x_cdrom_image_{0}_sha1".format(i), sha1)])


        # blank the rest of the drives
        for i in range(len(paths), 1):
            Config.set_multiple([
                    ("cdrom_drive_{0}".format(i), ""),
                    ("x_cdrom_drive_{0}_sha1".format(i), "")])
            
            #Config.set("x_cdrom_drive_{0}_sha1".format(i), "")
            #Config.set("x_cdrom_drive_{0}_name".format(i), "")
        # blank the rest of the image list
        for i in range(len(paths), Amiga.MAX_CDROM_IMAGES):
            Config.set_multiple([
                    ("cdrom_image_{0}".format(i), ""),
                    ("x_cdrom_image_{0}_sha1".format(i), "")])
