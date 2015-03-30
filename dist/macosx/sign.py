import sys
import subprocess

do_sign = True

if do_sign:
    args = [
        "codesign",
        "-f",
        "--deep",
        "-s",
        "Developer ID Application",
        sys.argv[1],
    ]
    print(args)
    p = subprocess.Popen(args)
    assert p.wait() == 0
