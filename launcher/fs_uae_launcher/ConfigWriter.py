from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

from .Config import Config
from .Settings import Settings

class ConfigWriter:

    def __init__(self, config):
        self.config = config

    def create_fsuae_config(self):

        config = {}
        for key, value in Settings.settings.iteritems():
            config[key] = value

        for key, value in self.config.iteritems():
            if value == "" and key in Config.skip_if_empty:
                pass
            else:
                config[key] = value

        # make sure FS-UAE does not load other config files (Host.fs-uae)
        config["end_config"] = "1"
        if not config["sub_title"]:
            config["sub_title"] = config["amiga_model"] or "A500"

        c = []

        num_drives = 0
        for i in range(4):
            key = "floppy_drive_{0}".format(i)
            if config.get(key):
                num_drives = i + 1
        num_drives = max(1, num_drives)

        c.append("[config]")
        for key in sorted(config.keys()):
            ignore = False
            if key.startswith("floppy_drive_"):
                for i in range(4):
                    if key == "floppy_drive_{0}".format(i):
                        if i >= num_drives:
                            ignore = True
                            break
            if ignore:
                continue
            value = config[key]
            print(repr((key, value)))
            value = value.replace("\\", "\\\\")
            c.append(u"{0} = {1}".format(key, value))



        return c
