#!/usr/bin/env python3
import os
import sys
import shutil
import subprocess

strip = False
rpath = False
no_copy = False
# steam_runtime = os.getenv("STEAM_RUNTIME", "")
steam_runtime = os.getenv("STEAMOS", "")


def fix_binary(path):
    changes = 0
    if os.path.exists(path + ".standalone"):
        return changes
    if not os.path.exists(path):
        raise Exception("could not find " + repr(path))

    # find library locations
    args = ["ldd", path]
    p = subprocess.Popen(args, stdout=subprocess.PIPE)
    # noinspection PyUnresolvedReferences
    data = p.stdout.read().decode("UTF-8")
    if p.wait() != 0:
        return 0
    print("fixing", path, "no_copy =", no_copy)
    library_locations = {}
    for line in data.split("\n"):
        line = line.strip()
        if "=>" not in line:
            continue
        library_locations[line.split(" ")[0]] = line.split(" ")[2]

    # find direct dependencies
    args = ["objdump", "-p", path]
    p = subprocess.Popen(args, stdout=subprocess.PIPE)
    # noinspection PyUnresolvedReferences
    data = p.stdout.read().decode("UTF-8")
    if p.wait() != 0:
        return 0
    for line in data.split("\n"):
        line = line.strip()
        # print(line)
        if not line.startswith("NEEDED"):
            continue
        print(line)
        library = line.split(" ")[-1]
        print(library)
        if ignore_library(library):
            continue
        library_source = library_locations[library]
        library_source = os.path.normpath(library_source)
        print(library, library_source)
        # if steam_runtime and library_source.startswith(steam_runtime):
        # if steam_runtime and not library_source.startswith("/usr/local"):
        if steam_runtime and not library_source.startswith("/home"):
            print("skipping steam runtime library")
            continue
        if no_copy:
            print("no_copy is set")
            continue
        dst = os.path.join(os.path.dirname(path), library)
        if not os.path.exists(dst):
            print("copying", library)
            shutil.copy(library_source, dst)
            os.chmod(dst, 0o644)
            changes += 1
    if strip:
        # strip does not work after patchelf has been run
        command = "strip '{}'".format(path)
        print(command)
        os.system(command)
    if rpath:
        command = "patchelf --set-rpath '{}' '{}'".format(rpath, path)
        print(command)
        assert os.system(command) == 0
    # to make sure strip is not run again
    os.system("touch '{}.standalone'".format(path))
    return changes


def ignore_library(name):
    if name.startswith("libgpg-error.so"):
        raise Exception(
            "Bundling libgpg-error (libgcrypt?) breaks Intel GL driver")

    if name.startswith("linux-gate.so"):
        return True
    if name.startswith("linux-vdso.so"):
        return True
    if name.startswith("ld-linux.so"):
        return True
    if name.startswith("ld-linux-x86-64.so"):
        return True
    if name.startswith("ld-linux-armhf.so"):
        return True

    if name.startswith("libc.so"):
        return True
    if name.startswith("libstdc++.so"):
        # Including libstdc++.sp breaks libGL loading with Intel on Ubuntu 16.10
        # libGL error: unable to load driver: i965_dri.so
        return True
    if name.startswith("libgcc_s.so"):
        # Might as well skip this one also, to avoid potential similar problems.
        return True
    if name.startswith("libpthread.so"):
        return True
    if name.startswith("libm.so"):
        return True
    if name.startswith("libdl.so"):
        return True
    if name.startswith("libresolv.so"):
        return True
    if name.startswith("librt.so"):
        return True
    if name.startswith("libutil.so"):
        return True
    # if name.startswith("libpcre.so"):
    #     # Problem with OpenAL on Ubuntu 16.04 if this is included.
    #     return True

    if name.startswith("libGL.so"):
        return True
    if name.startswith("libGLU.so"):
        return True
    if name.startswith("libEGL.so"):
        return True

    # Not really sure if this one should be skipped or not. It is linked to on
    # Raspberry Pi 4, but might be a system library that should not be
    # distributed.
    if name.startswith("libatomic.so"):
        return True

    if name.startswith("libasound.so"):
        # Alsa library is in LSB, looks like only "old" interfaces are used
        # by SDL2.
        return True

    if name.startswith("libX11.so"):
        return True
    if name.startswith("libXext.so"):
        return True
    if name.startswith("libXcursor.so"):
        return True
    if name.startswith("libXinerama.so"):
        return True
    if name.startswith("libXi.so"):
        return True
    if name.startswith("libXrandr.so"):
        return True
    if name.startswith("libXss.so"):
        return True
    if name.startswith("libXxf86vm.so"):
        return True
    # if name.startswith("libxkbcommon.so"):
    #     return True
    if name.startswith("libxcb.so"):
        return True

    return False


def fix_iteration(app):
    binaries = []
    binaries_dir = app
    for name in os.listdir(binaries_dir):
        binaries.append(os.path.join(binaries_dir, name))
    changes = 0
    for binary in binaries:
        changes += fix_binary(binary)
    return changes


def main():
    global no_copy, strip, rpath
    for arg in list(sys.argv):
        if arg.startswith("--rpath="):
            sys.argv.remove(arg)
            rpath = arg[8:]
        elif arg == "--no-copy":
            sys.argv.remove("--no-copy")
            no_copy = True
        elif arg == "--strip":
            sys.argv.remove("--strip")
            strip = True
    app = sys.argv[1]
    while True:
        changes = fix_iteration(app)
        if changes == 0:
            break
    for name in os.listdir(app):
        if name.endswith(".standalone"):
            os.remove(os.path.join(app, name))


if __name__ == "__main__":
    main()
