import sys
import time
import subprocess

do_sign = True
tries = 0

while do_sign:
    args = [
        "codesign",
        "-f",
        "--deep",
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
