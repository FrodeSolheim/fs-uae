#!/usr/bin/env python3
"""Copy the MinGW DLLs that Windows executables depend on into a directory.

Usage: copy-windows-dlls.py <mingw prefix> <dest dir> <binary>...

Dependencies are found recursively with objdump. DLLs found in <mingw prefix>/bin are copied
(when missing or changed); all others are assumed to be Windows system DLLs. Python extension
modules (lib/python3.*/lib-dynload/*.pyd) are always scanned as well, since fs-uae.exe loads them
at runtime, so their dependencies (libffi, OpenSSL, ...) are copied too. The exceptions are the
modules in IGNORED_PYTHON_MODULES, which FS-UAE does not use (and would pull in Tcl/Tk and ncurses).

Used by od-fs/build/windows/crossbuild/build when cross-compiling on Linux.
"""

import glob
import os
import shutil
import subprocess
import sys

IGNORED_PYTHON_MODULES = ["_tkinter", "_curses", "_curses_panel"]


def find_objdump() -> str:
    for name in ["x86_64-w64-mingw32-objdump", "llvm-objdump", "objdump"]:
        path = shutil.which(name)
        if path:
            return path
    raise SystemExit("Error: objdump not found (install binutils-mingw-w64-x86-64)")


def dependencies(objdump: str, path: str) -> list[str]:
    output = subprocess.run(
        [objdump, "-p", path], check=True, capture_output=True, text=True
    ).stdout
    result = []
    for line in output.splitlines():
        line = line.strip()
        if line.startswith("DLL Name:"):
            result.append(line.split(":", 1)[1].strip())
    return result


def main() -> None:
    if len(sys.argv) < 4:
        raise SystemExit(__doc__)
    prefix, dest_dir, binaries = sys.argv[1], sys.argv[2], sys.argv[3:]
    bin_dir = os.path.join(prefix, "bin")
    # Windows file names are case-insensitive
    available = {name.lower(): name for name in os.listdir(bin_dir)}
    objdump = find_objdump()

    queue = list(binaries)
    for pyd in sorted(glob.glob(os.path.join(prefix, "lib", "python3.*", "lib-dynload", "*.pyd"))):
        if os.path.basename(pyd).split(".")[0] not in IGNORED_PYTHON_MODULES:
            queue.append(pyd)
    seen: set[str] = set()
    needed: set[str] = set()
    while queue:
        path = queue.pop()
        for dll in dependencies(objdump, path):
            key = dll.lower()
            if key in seen:
                continue
            seen.add(key)
            if key in available:
                needed.add(available[key])
                queue.append(os.path.join(bin_dir, available[key]))

    copied = 0
    for name in sorted(needed):
        src = os.path.join(bin_dir, name)
        dst = os.path.join(dest_dir, name)
        if os.path.exists(dst):
            src_stat, dst_stat = os.stat(src), os.stat(dst)
            if src_stat.st_size == dst_stat.st_size and src_stat.st_mtime <= dst_stat.st_mtime:
                continue
        shutil.copy2(src, dst)
        copied += 1
    print(f"Windows DLLs: {len(needed)} needed, {copied} copied to {dest_dir}")


if __name__ == "__main__":
    main()
