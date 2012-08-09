#!/usr/bin/env python

import sys

def update_version(path, strict=False):
    ver = version
    with open(path, "rb") as f:
        data = f.read()
    if strict:
        # only use the first three components
        parts = ver.split(".")
        parts = parts[:3]
        for i, part in enumerate(parts):
            p = ""
            for c in part:
                if c in "0123456789":
                    p += c
                else:
                    break
            parts[i] = p
        ver = '.'.join(parts)
    data = data.replace("9.8.7", ver)
    with open(path, "wb") as f:
        f.write(data)

with open("VERSION", "rb") as f:
    version = f.read().strip()

if len(sys.argv) > 1:
    update_version(sys.argv[1])
else:
    update_version("README")
    update_version("common.mk")
    update_version("fs-uae.spec")
    update_version("debian/changelog")
    update_version("server/debian/changelog")
    update_version("server/setup.py")
    update_version("launcher/setup.py")
    update_version("launcher/setup_py2exe.py")
    update_version("launcher/setup_py2app.py", strict=True)
    update_version("launcher/debian/changelog")
    update_version("launcher/fs_uae_launcher/Version.py")
    update_version("src/fs-uae/version.c")
    update_version("macosx/template/Contents/Info.plist", strict=True)
    update_version("windows/fs-uae-setup.iss")
