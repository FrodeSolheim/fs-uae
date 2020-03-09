#!/usr/bin/env python3

import os
import sys
import shutil
import subprocess


def fix_binary(path, frameworks_dir):
    print("fixing", path)
    changes = 0
    if not os.path.exists(path):
        raise Exception("could not find " + repr(path))
    args = ["otool", "-L", path]
    p = subprocess.Popen(args, stdout=subprocess.PIPE)
    # noinspection PyUnresolvedReferences
    data = p.stdout.read().decode("UTF-8")
    p.wait()
    for line in data.split("\n"):
        line = line.strip()
        if not line:
            continue
        if line.startswith("/usr/lib") or line.startswith("/System"):
            # old = line.split(" ")[0]
            continue
        if line.startswith("@executable_path"):
            continue
        old = line.split(" ")[0]
        if "Contents" in old:
            continue
        print(old)
        old_dir, name = os.path.split(old)
        new = old.replace(old, "@executable_path/../Frameworks/" + name)
        dst = os.path.join(frameworks_dir, os.path.basename(old))
        if not os.path.exists(dst):
            print("copying", old)
            shutil.copy(old, dst)
            os.chmod(dst, 0o644)
            changes += 1
        if os.path.basename(path) == os.path.basename(old):
            args = ["install_name_tool", "-id", new, path]
        else:
            args = ["install_name_tool", "-change", old, new, path]
        print(args)
        p = subprocess.Popen(args)
        assert p.wait() == 0
    return changes


def fix_iteration(app):
    binaries = []
    mac_os_dir = os.path.join(app, "Contents", "MacOS")
    frameworks_dir = os.path.join(app, "Contents", "Frameworks")
    for name in os.listdir(mac_os_dir):
        binaries.append(os.path.join(mac_os_dir, name))
    for name in os.listdir(frameworks_dir):
        binaries.append(os.path.join(frameworks_dir, name))
    changes = 0
    for binary in binaries:
        changes += fix_binary(binary, frameworks_dir)
    return changes


def main():
    app = sys.argv[1]
    while True:
        changes = fix_iteration(app)
        if changes == 0:
            break


if __name__ == "__main__":
    main()
