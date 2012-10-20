from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import fs_uae_launcher.fsui as fsui
from .ui.LauncherFileDialog import LauncherFileDialog
from .Amiga import Amiga
from .Archive import Archive
from .Config import Config
from .I18N import _, ngettext
from .Paths import Paths
from .Settings import Settings

class FloppyManager:

    @classmethod
    def clear_all(cls):
        for i in range(4):
            cls.eject(i)
        cls.clear_floppy_list()

    @classmethod
    def eject(cls, drive):
        values = []
        values.append(("floppy_drive_{0}".format(drive), ""))
        values.append(("x_floppy_drive_{0}_sha1".format(drive), ""))
        Config.set_multiple(values)

    @classmethod
    def clear_floppy_list(cls):
        values = []
        for i in range(Amiga.MAX_FLOPPY_IMAGES):
            values.append(("floppy_image_{0}".format(i), ""))
            values.append(("x_floppy_image_{0}_sha1".format(i), ""))
        Config.set_multiple(values)

    @classmethod
    def multiselect(cls, parent=None):
        default_dir = Settings.get_floppies_dir()
        dialog = LauncherFileDialog(parent, _("Select Multiple Floppies"),
                "floppy", multiple=True)
        if not dialog.show():
            return
        original_paths = dialog.get_paths()
        original_paths.sort()
        paths = []
        embedded_files = []
        for path in original_paths:
            if path.endswith(".zip"):
                archive = Archive(path)
                files = archive.list_files()
                for file in files:
                    name, ext = os.path.splitext(file)
                    # FIXME: get list of floppy extensions from a central
                    # place
                    if ext in [".adf", ".ipf"]:
                        embedded_files.append(file)
        if len(embedded_files) > 0:
            embedded_files.sort()
            print("found embedded floppy images:")
            print(embedded_files)
            for file in embedded_files:
                paths.append(file)
        else:
            paths.append(path)

        from .ChecksumTool import ChecksumTool
        checksum_tool = ChecksumTool(parent)
        for i, path in enumerate(paths):
            sha1 = checksum_tool.checksum(path)
            path = Paths.contract_path(path, default_dir)

            if i < 4:
                Config.set_multiple([
                        ("floppy_drive_{0}".format(i), path),
                        ("x_floppy_drive_{0}_sha1".format(i), sha1)])
            Config.set_multiple([
                    ("floppy_image_{0}".format(i), path),
                    ("x_floppy_image_{0}_sha1".format(i), sha1)])

        # blank the rest of the drives
        for i in range(len(paths), 4):
            Config.set_multiple([
                    ("floppy_drive_{0}".format(i), ""),
                    ("x_floppy_drive_{0}_sha1".format(i), "")])
        # blank the rest of the image list
        for i in range(len(paths), 20):
            Config.set_multiple([
                    ("floppy_image_{0}".format(i), ""),
                    ("x_floppy_image_{0}_sha1".format(i), "")])
        #dialog.destroy()
