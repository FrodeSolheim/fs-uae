#!/usr/bin/env python3
import os
import sys
import platform
import subprocess

p = subprocess.Popen(["file", "-L", "/bin/sh"], stdout=subprocess.PIPE)
exe_info = p.stdout.read().decode("UTF-8")
if "386" in exe_info:
    arch = "x86"
elif "x86-64" in exe_info:
    arch = "x86-64"
else:
    raise Exception("unrecognized arch " + repr(exe_info))

# if os.environ.get("STEAMOS", ""):
#     os_name = "steamos"
# else:
os_name = "linux"
os_name_pretty = "Linux"

version = sys.argv[1]
package_name = "fs-uae_{0}_{1}_{2}".format(version, os_name, arch)
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
    s("cd ../.. && ./configure")
    s("make -C ../..")

s("cp -a ../../fs-uae {package_dir}/fs-uae")
# s("cp -a ../../fs-uae.dat {package_dir}/fs-uae.dat")
# s("PYTHONPATH=../../../fs-uae-launcher:../../../../fs-uae-launcher "
#   "python3 -m fspy.zipfile deterministic {package_dir}/fs-uae.dat")
s("cp -a ../../fs-uae-device-helper {package_dir}/fs-uae-device-helper")

s("mkdir FS-UAE/Data")
s("cp -a ../../fs-uae.dat FS-UAE/Data/fs-uae.dat")
s("PYTHONPATH=../../../fs-uae-launcher:../../../../fs-uae-launcher "
  "python3 -m fspy.zipfile deterministic FS-UAE/Data/fs-uae.dat")
# s("mkdir -p {package_dir}/share")
# s("cp -a ../../share/locale {package_dir}/share/locale")
s("cp -a ../../share/locale FS-UAE/Data/Locale")
# s("mkdir -p {package_dir}/share/fs-uae")
# s("touch {package_dir}/share/fs-uae/share-dir")

s("mkdir FS-UAE/Docs")
s("cp -a ../../licenses FS-UAE/Docs/Licenses")
s("cp -a ../../ChangeLog FS-UAE/Docs/ChangeLog.txt")
s("cp -a ../../README FS-UAE/ReadMe.txt")

s("cp -a ../../data/* FS-UAE/Data/")

if os.environ.get("STANDALONE") == "0":
    pass
else:
    s("./standalone-linux.py --strip --rpath='$ORIGIN' {package_dir}")

s("find {package_dir} -name '*.standalone' -delete")
s("echo {version} > FS-UAE/Version.txt")
s("echo {version} > {package_dir}/Version.txt")

if os_name == "steamos":
    wrap("fs-uae")
    wrap("fs-uae-device-helper")

# s("cd {package_dir} && tar Jcfv ../../../{package_name}.tar.xz *")
s("tar Jcfv ../../{package_name}.tar.xz FS-UAE")
print(package_name)
s("cp ../../{package_name}.tar.xz ../../{package_name_2}.tar.xz")
print(package_name_2)
print("OK")

#s("rm -Rf {dbg_package_dir}")
#s("mkdir {dbg_package_dir}")
#s("cp -a ../../fs-uae.dbg {dbg_package_dir}/")
#s("cp -a ../../fs-uae-device-helper.dbg {dbg_package_dir}/")
#s("cd {dbg_package_dir} && tar Jcfv ../../../{dbg_package_dir}.tar.xz *")
