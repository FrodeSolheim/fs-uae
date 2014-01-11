from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import sys
import platform
from fsbc.Signal import Signal
from .main_common import setup_logging, setup_common


def main():
    setup_common()

    setup_logging("fs-uae-game-center.log.txt")

    from .Version import Version
    print("FS-UAE Game Center {0}".format(Version.VERSION))
    print("System: {0}".format(repr(platform.uname())))
    print(sys.argv)

    try:
        import fengestad.gamecenter.main
        fengestad.gamecenter.main.main()
    except KeyboardInterrupt:
        print("Interrupted by keyboard")
    from fengestad.base.Application import Application
    Application.get().stop()
    Signal("quit").notify()
