#!/usr/bin/env python3

import os
import sys
# import subprocess
import shutil

if os.getenv("MSYSTEM") == "MINGW32":
    prefix = "/mingw32"
elif os.getenv("MSYSTEM") == "MINGW64":
    prefix = "/mingw64"

dll = sys.argv[1]
dst = sys.argv[2]

src = f"../../fsdeps/_dlls/{dll}"
print(src)
if os.path.exists(src):
    shutil.copy(src, dst)
else:
    src = f"{prefix}/bin/{dll}"
    shutil.copy(src, dst)

# p = subprocess.Popen([
#     "sh", "-c",
#     "cp {}/bin/{} {}".format(prefix, sys.argv[1], sys.argv[2])])
# sys.exit(p.wait())
