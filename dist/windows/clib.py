#!/usr/bin/env python3

import os
import sys
import subprocess

if os.getenv("MSYSTEM") == "MINGW32":
    mingw_dir = "/mingw32"
elif os.getenv("MSYSTEM") == "MINGW64":
    mingw_dir = "/mingw64"

p = subprocess.Popen([
    "sh", "-c",
    "cp {}/bin/{} {}".format(mingw_dir, sys.argv[1], sys.argv[2])])
sys.exit(p.wait())
