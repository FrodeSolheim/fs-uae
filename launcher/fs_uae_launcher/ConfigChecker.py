from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import traceback
from .Config import Config

class ConfigChecker:

    def __init__(self):
        Config.add_listener(self)

    def on_config(self, key, value):
        if key == "floppy_drive_0" and value:
            if not Config.get("x_floppy_drive_0_sha1"):
                print("")
                traceback.print_stack()
                print("(floppy_drive_0 set without x_floppy_drive_0_sha1)")
                print("")

        elif key == "floppy_image_0" and value:
            if not Config.get("x_floppy_image_0_sha1"):
                print("")
                traceback.print_stack()
                print("(floppy_image_0 set without x_floppy_image_0_sha1)")
                print("")

        elif key == "hard_drive_0" and value:
            if not Config.get("x_hard_drive_0_sha1"):
                print("")
                traceback.print_stack()
                print("(hard_drive_0 set without x_hard_drive_0_sha1)")
                print("")

        elif key == "kickstart_file" and value:
            if not Config.get("x_kickstart_file_sha1"):
                print("")
                traceback.print_stack()
                print("(kickstart_file set without x_kickstart_file_sha1)")
                print("")

        elif key == "x_kickstart_file" and value:
            if not Config.get("x_kickstart_file_sha1"):
                print("")
                traceback.print_stack()
                print("(x_kickstart_file set without x_kickstart_file_sha1)")
                print("")

        elif key == "kickstart_ext_file" and value:
            if not Config.get("x_kickstart_ext_file_sha1"):
                print("")
                traceback.print_stack()
                print("(kickstart_ext_file set without x_kickstart_ext_file_sha1)")
                print("")

        elif key == "x_kickstart_ext_file" and value:
            if not Config.get("x_kickstart_ext_file_sha1"):
                print("")
                traceback.print_stack()
                print("(x_kickstart_ext_file set without x_kickstart_ext_file_sha1)")
                print("")
