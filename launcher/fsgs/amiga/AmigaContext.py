from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from fs_uae_launcher.ChecksumTool import ChecksumTool
from fs_uae_launcher.FSUAEDirectories import FSUAEDirectories
from ..BaseContext import BaseContext
from ..Paths import Paths

class AmigaContext(BaseContext):

    def __init__(self, main_context):
        BaseContext.__init__(self, main_context)

    def reset_all_floppies(self):
        # FIXME: MAX_AMIGA_FLOPPY_DRIVES
        for i in range(4):
            self.eject(i)
        self.clear_floppy_list()

    def eject_floppy(self, drive):
        values = []
        values.append(("floppy_drive_{0}".format(drive), ""))
        values.append(("x_floppy_drive_{0}_sha1".format(drive), ""))
        self.set_config(values)

        #self.notify("fsgs:config:floppy_drive_{0}".format(drive), "")
        #self.notify("fsgs:config:x_floppy_drive_{0}_sha1".format(drive), "")

    def clear_floppy_list(self):
        values = []
        for i in range(Amiga.MAX_FLOPPY_IMAGES):
            values.append(("floppy_image_{0}".format(i), ""))
            values.append(("x_floppy_image_{0}_sha1".format(i), ""))
        self.set_config(values)

    def insert_floppy_in_free_drive(self, path, sha1=None):
        # FIXME: max floppy drives
        for i in range(4):
            if not self.config.get("floppy_drive_{0}".format(i)):
                self.insert_floppy(i, path, sha1=sha1)
                return True
        return False

    def insert_floppy(self, drive, path, sha1=None):
        if sha1 is None:
            sha1 = ChecksumTool().checksum(path)
        default_dir = FSUAEDirectories.get_floppies_dir()
        path = Paths.contract_path(path, default_dir)
        self.set_config([
                ("floppy_drive_{0}".format(drive), path),
                ("x_floppy_drive_{0}_sha1".format(drive), sha1)])

    def insert_cd(self, drive, path, sha1=None):
        if sha1 is None:
            sha1 = ""
            print("FIXME: not calculating CD checksum just yet")
        default_dir = FSUAEDirectories.get_cdroms_dir()
        path = Paths.contract_path(path, default_dir)
        self.set_config([
                ("cdrom_drive_{0}".format(drive), path),
                ("x_cdrom_drive_{0}_sha1".format(drive), sha1)])

    def insert_multiple_floppies(self, insert_paths):
        paths = []
        for path in insert_paths:
            embedded_files = []
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

        checksum_tool = ChecksumTool()
        for i, path in enumerate(paths):
            sha1 = checksum_tool.checksum(path)
            path = Paths.contract_path(path, default_dir)

            if i < 4:
                self.set_config([
                        ("floppy_drive_{0}".format(i), path),
                        ("x_floppy_drive_{0}_sha1".format(i), sha1)])
            self.set_config([
                    ("floppy_image_{0}".format(i), path),
                    ("x_floppy_image_{0}_sha1".format(i), sha1)])

        # blank the rest of the drives
        for i in range(len(paths), 4):
            self.set_config([
                    ("floppy_drive_{0}".format(i), ""),
                    ("x_floppy_drive_{0}_sha1".format(i), "")])
        # blank the rest of the image list
        for i in range(len(paths), 20):
            self.set_config([
                    ("floppy_image_{0}".format(i), ""),
                    ("x_floppy_image_{0}_sha1".format(i), "")])

    def get_base_dir(self):
        # FIXME: dependency loop
        return FSUAEDirectories.get_base_dir()

    def get_floppies_dir(self):
        # FIXME: dependency loop
        return FSUAEDirectories.get_floppies_dir()

    def get_cdroms_dir(self):
        # FIXME: dependency loop
        return FSUAEDirectories.get_cdroms_dir()

    def get_hard_drives_dir(self):
        # FIXME: dependency loop
        return FSUAEDirectories.get_hard_drives_dir()

    def get_whdload_dir(self):
        # FIXME: dependency loop
        return FSUAEDirectories.get_whdload_dir()

    def get_kickstarts_dir(self):
        # FIXME: dependency loop
        return FSUAEDirectories.get_kickstarts_dir()

    def get_controllers_dir(self):
        # FIXME: dependency loop
        return FSUAEDirectories.get_controllers_dir()
