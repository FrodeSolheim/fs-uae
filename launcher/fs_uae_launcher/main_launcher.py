from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import sys
import platform
from fsbc.Signal import Signal
from fs_uae_launcher.netplay.IRC import IRC
from .main_common import setup_logging, setup_common


def main():
    setup_common()

    if "--joystick-config" in sys.argv:
        setup_logging("joystick-configuration.log.txt")
        print("importing pygame")
        import pygame
        print("initializing pygame")
        pygame.init()
        pygame.joystick.init()
        from fs_uae_launcher.JoystickConfigDialog import joystick_config_main
        return joystick_config_main()

    if "--server" in sys.argv:
        setup_logging("fs-uae-net-play-server.log.txt")
        from fs_uae_launcher.server.game import run_server
        return run_server()

    setup_logging("fs-uae-launcher.log.txt")

    from .Version import Version
    print("FS-UAE Launcher {0}".format(Version.VERSION))
    print("System: {0}".format(repr(platform.uname())))
    print(sys.argv)

    for arg in sys.argv:
        if arg.startswith("--"):
            if "=" in arg:
                key, value = arg[2:].split("=", 1)
                key = key.replace("-", "_")
                if key == "fake_version":
                    from .Version import Version
                    Version.VERSION = value
                elif key == "fake_series":
                    from .Version import Version
                    Version.SERIES = value

    from .ConfigChecker import ConfigChecker
    ConfigChecker()

    from .FSUAELauncher import FSUAELauncher
    application = FSUAELauncher()
    application.start()
    application.run()
    application.save_settings()
    IRC.stop()
    Signal("quit").notify()
