#!/usr/bin/env python3
import os
import sys
import platform
import subprocess

package = {}
with open("../../PACKAGE.FS", "r") as f:
    for line in f:
        try:
            key, value = line.strip().split("=", 1)
            package[key] = value
        except ValueError:
            pass

version = package["PACKAGE_VERSION"]
arch = os.environ["SYSTEM_ARCH"]

# if os.environ.get("STEAMOS", ""):
#     os_name = "steamos"
# else:
os_name = "linux"
os_name_pretty = "Linux"

version = sys.argv[1]
# package_name = "fs-uae_{0}_{1}_{2}".format(version, os_name, arch)
package_name_2 = "FS-UAE_{0}_{1}_{2}".format(version, os_name_pretty, arch)
package_dir = "../{}/FS-UAE/{}/{}".format(os_name, os_name_pretty, arch)


def s(command):
    c = command.format(**globals())
    print(c)
    assert os.system(c) == 0


def wrap(name, target=None, args=None):
    if target is None:
        target = name + ".bin"
        os.rename(os.path.join(package_dir, name),
                  os.path.join(package_dir, target))
    if args is None:
        args = ["$@"]
    path = os.path.join(package_dir, name)
    with open(path, "w") as f:
        f.write("#!/bin/sh\n")
        f.write("MYDIR=$(dirname \"$0\")\n")
        # f.write("export LD_LIBRARY_PATH=\"$MYDIR:$LD_LIBRARY_PATH\"\n")
        command = "\"$MYDIR/{0}\"".format(target)
        for arg in args:
            command += " \"{0}\"".format(arg)
        if os_name == "steamos":
            # if arch == "i686":
            if arch == "x86":
                bin_dir = "bin32"
            # elif arch == "amd64":
            elif arch == "x86-64":
                bin_dir = "bin32"
            else:
                raise Exception("unsupported steamos arch?")
            f.write("if [ -e \"$HOME/.steam/{0}/steam-runtime/"
                    "run.sh\" ]; then\n".format(bin_dir))
            f.write("RUN_SH=\"$HOME/.steam/{0}/steam-runtime/"
                    "run.sh\"\n".format(bin_dir))
            f.write("else\n")
            f.write("RUN_SH=\"/home/steam/.steam/{0}/steam-runtime/"
                    "run.sh\"\n".format(bin_dir))
            f.write("fi\n")
            f.write("exec $RUN_SH {0}\n".format(command))
        else:
            f.write("exec {0}\n".format(command))
    os.chmod(path, 0o755)


s("rm -Rf FS-UAE")
s("mkdir -p {package_dir}")
if os.environ.get("BUILD") == "0":
    pass
else:
    if os.environ.get("RPI4") == "1":
        s("../rpi4/configure")
    else:
        s("cd ../.. && ./configure")
    s("make -C ../..")

s("cp -a ../../fs-uae {package_dir}/fs-uae")
# s("cp -a ../../fs-uae.dat {package_dir}/fs-uae.dat")
# s("PYTHONPATH=../../../fs-uae-launcher:../../../../fs-uae-launcher "
#   "python3 -m fspy.zipfile deterministic {package_dir}/fs-uae.dat")
s("cp -a ../../fs-uae-device-helper {package_dir}/fs-uae-device-helper")

# data_dir = "FS-UAE/Data"
data_dir = package_dir

s(f"mkdir -p {data_dir}")
s(f"cp -a ../../fs-uae.dat {data_dir}/fs-uae.dat")
# FIXME: Disabled for now since it references another project
# s(f"PYTHONPATH=../../../fs-uae-launcher:../../../../fs-uae-launcher "
#   f"python3 -m fspy.zipfile deterministic {data_dir}/fs-uae.dat")
s(f"cp -a ../../data/* {data_dir}/")

s(f"cp -a ../../share/locale FS-UAE/Locale")

s("mkdir FS-UAE/Docs")
s("cp -a ../../licenses FS-UAE/Docs/Licenses")
s("cp -a ../../ChangeLog FS-UAE/Docs/ChangeLog.txt")
s("cp -a ../../README FS-UAE/ReadMe.txt")

if os.environ.get("STANDALONE") == "0":
    pass
else:
    # s("./standalone-linux.py --strip --rpath='$ORIGIN' {package_dir}")
    s("cd ../.. && python3 fsbuild/standalone.py --strip --rpath='$ORIGIN' dist/linux/{package_dir}")

s("find {package_dir} -name '*.standalone' -delete")
s("echo {version} > FS-UAE/Version.txt")
s("echo {version} > {package_dir}/Version.txt")

if os_name == "steamos":
    wrap("fs-uae")
    wrap("fs-uae-device-helper")

if os.environ.get("PACKAGE", "") != "0":
    # s("cd {package_dir} && tar Jcfv ../../../{package_name}.tar.xz *")
    s("tar Jcfv ../../{package_name_2}.tar.xz FS-UAE")
    print(package_name_2)
    # s("cp ../../{package_name_2}.tar.xz ../../{package_name}.tar.xz")
    # print(package_name_2)
print("OK")

#s("rm -Rf {dbg_package_dir}")
#s("mkdir {dbg_package_dir}")
#s("cp -a ../../fs-uae.dbg {dbg_package_dir}/")
#s("cp -a ../../fs-uae-device-helper.dbg {dbg_package_dir}/")
#s("cd {dbg_package_dir} && tar Jcfv ../../../{dbg_package_dir}.tar.xz *")
