from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import sys
import time
import shutil
import tempfile
import subprocess


libraries = [
    "libSDL-1.2.0.dylib",
    "libglib-2.0.0.dylib",
    "libgthread-2.0.0.dylib",
    "libintl.8.dylib",
    "libpng14.14.dylib",
    "libz.1.dylib",
    "libiconv.2.dylib",
]


def copy_libs():
    for name in libraries:
        print(name)
        shutil.copy2("/opt/local/lib/" + name,
                "fs-uae.app/Contents/Frameworks/" + name)


def fix_libs(path):
    print("fixing", path)
    args = ["otool", "-L", path] 
    p = subprocess.Popen(args, stdout=subprocess.PIPE)
    data = p.stdout.read()
    p.wait()
    for line in data.split('\n'):
        line = line.strip()
        if not line:
            continue
        if line.startswith('/opt/local/lib/'):
            old = line.split(' ')[0]
            old_dir, name = os.path.split(old)
            new = old.replace(old, '@executable_path/../Frameworks/' + name)
            args = ["install_name_tool", "-change", old, new, path]
            print(args)
            p = subprocess.Popen(args)
            assert p.wait() == 0


#def fix_deps():
#    print("fix update deps")
#    source_dir = "/opt/local/lib"
#    dest_dir = os.path.join(get_bundle_dir(), "Contents", "Frameworks")
#    #names = ["libcrypto.1.0.0.dylib", "libcurl.4.dylib"]
#    names = ["libcurl.4.dylib"]
#    for name in names:
#        source = os.path.join(source_dir, name)
#        dest = os.path.join(dest_dir, name)
#        shutil.copy2(source, dest)
#        fix_libs(dest)
#    fix_libs(os.path.join(get_bundle_dir(), "Contents", "MacOS", "fs-uae"))


for dir_name in os.listdir("."):
    org_dir = os.getcwd()
    if os.path.isdir(dir_name) and dir_name.startswith("fs-uae-"):
        os.chdir(dir_name)
        copy_libs()
        for name in libraries:
            fix_libs("fs-uae.app/Contents/Frameworks/" + name)
        fix_libs("fs-uae.app/Contents/Frameworks/libcapsimage.dylib")
        fix_libs("fs-uae.app/Contents/MacOS/fs-uae")
        os.chdir(org_dir)
