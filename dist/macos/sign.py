#!/usr/bin/env python3

import os
import sys
import time
import subprocess

if os.environ.get("SIGN", "") == "0":
    print("SIGN=0 via environment, skipping signing step")
    sys.exit(0)
if not os.environ.get("APPLE_ID_USER", ""):
    print("APPLE_ID_USER not specified, skipping signing step")
    sys.exit(0)

do_sign = True
tries = 0

# Signing sometimes fails due to Apple errors (timeouts, etc). So we try
# multiple times before giving up.

while do_sign:
    args = [
        "codesign",
        "-f",
        "--deep",
        "--options=runtime",
        "--entitlements",
        "Entitlements.plist",
        "-s",
        "Developer ID Application",
        sys.argv[1],
    ]
    print(args, "tries", tries)
    p = subprocess.Popen(args)
    if p.wait() == 0:
        break
    else:
        tries += 1
        time.sleep(tries)
        if tries == 20:
            print("Giving up")
            sys.exit(1)
        continue
