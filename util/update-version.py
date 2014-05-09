#!/usr/bin/env python

import io
import sys

def update_version(path, strict=False, update_series=False):
    ver = version
    with io.open(path, "r", encoding="ISO-8859-1", newline="") as f:
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
        ver = ".".join(parts)
    data = data.replace("9.8.7", ver)
    if update_series:
        data = data.replace("unknown", series)
    with io.open(path, "w", encoding="ISO-8859-1", newline="") as f:
        f.write(data)

with io.open("VERSION", "r", encoding="ISO-8859-1", newline="") as f:
    version = f.read().strip()

with io.open("SERIES", "r", encoding="ISO-8859-1", newline="") as f:
    series = f.read().strip()

if len(sys.argv) > 1:
    update_version(sys.argv[1], update_series=("--update-series" in sys.argv))
else:
    update_version("README")
    # update_version("common.mk")
    update_version("fs-uae.spec")
    update_version("debian/changelog")
    # update_version("src/fs-uae/version.c")
    # update_version("src/od-fs/config.h")
    update_version("macosx/template/Contents/Info.plist", strict=True)
