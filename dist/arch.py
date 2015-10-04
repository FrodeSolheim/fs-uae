#!/usr/bin/env python3
import sys
import platform
machine = platform.machine().lower()
if machine in ["x86_64", "amd64", "i386", "i486", "i585", "i686"]:
    if platform.architecture()[0] == "32bit":
        print("x86")
        sys.exit(0)
    if platform.architecture()[0] == "64bit":
        print("x86-64")
        sys.exit(0)
if machine.startswith("power"):
    if platform.architecture()[0] == "32bit":
        print("ppc")
        sys.exit(0)
raise Exception("Unknown platform")
