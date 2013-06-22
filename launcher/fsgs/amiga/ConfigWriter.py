from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.six as six
from fsgs.amiga.Amiga import Amiga

class ConfigWriter:

    def __init__(self, config):
        self.config = config

    def create_fsuae_config(self):
        print("create_fsuae_config")
        c = []

        num_drives = 0
        for i in range(Amiga.MAX_FLOPPY_DRIVES):
            key = "floppy_drive_{0}".format(i)
            value = self.config.get(key)
            if value:
                num_drives = i + 1
        num_drives = max(1, num_drives)

        print("")
        print("-------------" * 6)
        print("CONFIG")
        c.append("[fs-uae]")
        for key in sorted(self.config.keys()):
            ignore = False
            if key.startswith("floppy_drive_"):
                for i in range(4):
                    if key == "floppy_drive_{0}".format(i):
                        if i >= num_drives:
                            ignore = True
                            break
            if ignore:
                continue
            value = self.config[key]
            print(key, repr(value))
            value = value.replace("\\", "\\\\")
            c.append("{0} = {1}".format(key, value))

        return c
