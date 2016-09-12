#!/usr/bin/env python3
import os
import sys
import platform
import subprocess

p = subprocess.Popen(["file", "-L", "/bin/sh"], stdout=subprocess.PIPE)
exe_info = p.stdout.read().decode("UTF-8")
if "386" in exe_info:
    # arch = "i386"
    arch = "x86"
elif "x86-64" in exe_info:
    # arch = "amd64"
    arch = "x86-64"
else:
    raise Exception("unrecognized arch " + repr(exe_info))

#if os.environ.get("STEAM_RUNTIME", ""):
if os.environ.get("STEAMOS", ""):
    os_name = "steamos"
    # if arch == "i386":
    #     # steam runtime sdk compiles with -mtune=generic -march=i686
    #     arch = "i686"
else:
    os_name = "linux"

#if os.environ.get("STEAM_RUNTIME_TARGET_ARCH", ""):
#    os_name = "steamos"
#    arch = os.environ["STEAM_RUNTIME_TARGET_ARCH"]
#elif platform.machine() == "x86_64":
#    os_name = "linux"
#    arch = "amd64"
#else:
#    os_name = "linux"
#    arch = "i386"

version = sys.argv[1]
package_name = "fs-uae_{0}_{1}_{2}".format(version, os_name, arch)
package_dir = "../{1}/fs-uae_{0}_{1}_{2}".format(version, os_name, arch)
#dbg_package_dir = "fs-uae-dbg-{0}-{1}-{2}".format(version, os_name, arch)


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


s("rm -Rf {package_dir}")
s("mkdir {package_dir}")
if os.environ.get("BUILD") == "0":
    pass
else:
    s("cd ../.. && ./configure")
    s("make -C ../..")
s("cp -a ../../fs-uae {package_dir}/fs-uae")
s("cp -a ../../fs-uae.dat {package_dir}/fs-uae.dat")
s("cp -a ../../fs-uae-device-helper {package_dir}/fs-uae-device-helper")

s("mkdir -p {package_dir}/share")
s("cp -a ../../share/locale {package_dir}/share/locale")
s("mkdir -p {package_dir}/share/fs-uae")
s("touch {package_dir}/share/fs-uae/share-dir")

s("cp -a ../../licenses {package_dir}/licenses")
s("cp -a ../../README {package_dir}/fs-uae.txt")
s("./standalone-linux.py --strip --rpath='$ORIGIN' {package_dir}")
s("find {package_dir} -name '*.standalone' -delete")
# s("strip {package_dir}/*.bin")
# s("strip {package_dir}/*.so.* || true")

# wrap("fs-uae", "fs-uae.bin")
# wrap("fs-uae-device-helper", "fs-uae-device-helper.bin")
if os_name == "steamos":
    wrap("fs-uae")
    wrap("fs-uae-device-helper")

s("cd {package_dir} && tar Jcfv ../../../{package_name}.tar.xz *")

#s("rm -Rf {dbg_package_dir}")
#s("mkdir {dbg_package_dir}")
#s("cp -a ../../fs-uae.dbg {dbg_package_dir}/")
#s("cp -a ../../fs-uae-device-helper.dbg {dbg_package_dir}/")
#s("cd {dbg_package_dir} && tar Jcfv ../../../{dbg_package_dir}.tar.xz *")
