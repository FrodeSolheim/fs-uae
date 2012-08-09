from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

from .Config import Config
from .Settings import Settings

class ConfigWriter:

    def __init__(self, config):
        self.config = config

    def create_fsuae_config(self):
        print("create_fsuae_config")
        config = {}
        for key, value in Settings.settings.iteritems():
            if key in Config.config_keys:
                print("... ignoring config key from settings:", key)
                continue
            config[key] = value

        for key, value in self.config.iteritems():
            if value == "" and key in Config.skip_empty_keys:
                pass
            else:
                config[key] = value

        # overwrite netplay config
        if config["__netplay_host"]:
            config["netplay_server"] = config["__netplay_host"]
        if config["__netplay_password"]:
            config["netplay_password"] = config["__netplay_password"]
        if config["__netplay_port"]:
            config["netplay_port"] = config["__netplay_port"]

        # copy actual kickstart options from x_ options
        config["kickstart_file"] = config["x_kickstart_file"]
        config["kickstart_ext_file"] = config["x_kickstart_ext_file"]

        # make sure FS-UAE does not load other config files (Host.fs-uae)
        config["end_config"] = "1"
        if not config["sub_title"]:
            config["sub_title"] = config["amiga_model"] or "A500"

        if config["__netplay_game"]:
            print("\nfixing config for netplay game")
            for key in [x for x in config.keys() if x.startswith("uae_")]:
                print("* removing option", key)
                del config[key]

        c = []

        num_drives = 0
        for i in range(4):
            key = "floppy_drive_{0}".format(i)
            if config.get(key):
                num_drives = i + 1
        num_drives = max(1, num_drives)

        print("")
        print("-------------" * 6)
        print("CONFIG")
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
            print(key, repr(value))
            value = value.replace("\\", "\\\\")
            c.append(u"{0} = {1}".format(key, value))

        return c
