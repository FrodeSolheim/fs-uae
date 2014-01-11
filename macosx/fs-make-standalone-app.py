#!/usr/bin/env python
from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals


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
    data = p.stdout.read()
    p.wait()
    for line in data.split('\n'):
        line = line.strip()
        if not line:
            continue
        #if not line.startswith("/opt/local/lib/"):
	if line.startswith("/usr/lib") or line.startswith("/System"):
            old = line.split(' ')[0]
            # print("ignoring", old)
            continue
        if line.startswith("@executable_path"):
            continue

        old = line.split(' ')[0]
        if "Contents" in old:
            continue
        print(old)
        old_dir, name = os.path.split(old)
        new = old.replace(old, '@executable_path/../Frameworks/' + name)
        dst = os.path.join(frameworks_dir, os.path.basename(old))
        if not os.path.exists(dst):
            print("copying", old)
            shutil.copy(old, dst)
            os.chmod(dst, 0644)
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
    macos_dir = os.path.join(app, "Contents", "MacOS")
    frameworks_dir = os.path.join(app, "Contents", "Frameworks")
    for name in os.listdir(macos_dir):
        binaries.append(os.path.join(macos_dir, name))
    for name in os.listdir(frameworks_dir):
        binaries.append(os.path.join(frameworks_dir, name))
    changes = 0
    for binary in binaries:
       changes += fix_binary(binary, frameworks_dir)
    return changes

app = sys.argv[1]
while True:
    changes = fix_iteration(app)
    if changes == 0:
        break
