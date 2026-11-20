#!/usr/bin/env python3
"""Lay out a Windows development build like a dist, so that it runs from any shell, Explorer or a
debugger, and not only from MSYS2's MINGW64 shell.

Usage: dev_layout.py [--wine] <mingw prefix> <build dir> [<od-fs dir>]

Run after every Windows development build, by od-fs/Makefile for a native build (in the MINGW64
shell) and by od-fs/build/windows/crossbuild/build for a cross-build, so that both are laid out the
same way. In Libs/x86_64-w64-mingw32/ it puts:

- FS-UAE.exe: the build directory's fs-uae.exe.
- The MinGW DLLs that it and Python's extension modules need (copy-windows-dlls.py).
- FS-UAE._pth: the MinGW prefix's lib/python3.X and lib/python3.X/lib-dynload as absolute Windows
  paths, one per line (no "import site"), where Python finds its standard library. With a ._pth
  file next to the executable, these are all of sys.path that does not come from FS-UAE itself.
  The paths come from "cygpath -w" (native), or with --wine, are the paths on Wine's Z: drive, which
  is the Linux root directory (cross-build). 3.X is the version of the libpython3.X.dll that
  fs-uae.exe imports.

With <od-fs dir> (the default build), Libs/x86_64-w64-mingw32/ is in od-fs, as in a dist unpacked
in place, and the proxy (proxy.exe in the build directory) is copied to od-fs/FS-UAE.exe. Without,
it is in the build directory, and there is no proxy: FS-UAE.exe next to the build directory's
fs-uae.exe would be the same file name on Windows. Either way, FS-UAE has the dist layout: its root
is two levels above Libs/x86_64-w64-mingw32/FS-UAE.exe, where it finds the Python and Resources
folders. od-fs has them; in a build directory, they lead to the source tree's. For a cross-build,
od-fs/configure links them (Linux symbolic links, which Wine follows). Natively, MSYS2's ln -s makes
copies, so od-fs/configure leaves them out and this step makes them as junctions, replacing a link
or junction there (never touching what it points to). A folder that is neither is left alone, with
a warning.

Earlier versions linked lib/ in Libs/x86_64-w64-mingw32/ to the MinGW lib folder. That link is
removed (never what it points to): FS-UAE would use the folder as the Python home and ignore the
._pth file, and a recursive delete that follows links or junctions could empty the MinGW lib folder.
Dists do not have a ._pth file: they have their own lib/ with the standard library, which FS-UAE
uses as the Python home.
"""

import argparse
import importlib.util
import os
import re
import shutil
import subprocess
from pathlib import Path
from types import ModuleType

TRIPLET = "x86_64-w64-mingw32"
HERE = Path(__file__).resolve().parent


def load_copy_windows_dlls() -> ModuleType:
    """copy-windows-dlls.py as a module (its file name is not a module name)."""
    spec = importlib.util.spec_from_file_location(
        "copy_windows_dlls", HERE / "copy-windows-dlls.py"
    )
    assert spec is not None and spec.loader is not None
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def windows_path(path: str, wine: bool) -> str:
    """The absolute Windows path of a path: on Wine's Z: drive, or from cygpath -w."""
    if wine:
        return "Z:" + os.path.abspath(path).replace("/", "\\")
    result = subprocess.run(["cygpath", "-w", path], check=True, capture_output=True, text=True)
    return result.stdout.strip()


def pth_content(stdlib_dir: str, wine: bool) -> str:
    """The ._pth file for the standard library in stdlib_dir (<prefix>/lib/python3.X)."""
    lines = [windows_path(stdlib_dir, wine), windows_path(stdlib_dir + "/lib-dynload", wine)]
    return "".join(line + "\r\n" for line in lines)


def python_minor_version(imports: list[str]) -> int:
    """The minor version of the libpython3.X.dll in an executable's DLL imports."""
    for name in imports:
        match = re.fullmatch(r"libpython3\.(\d+)\.dll", name, re.IGNORECASE)
        if match:
            return int(match.group(1))
    raise SystemExit("Error: fs-uae.exe does not import a libpython3.X.dll")


def remove_link(path: Path) -> None:
    """Removes path if it is a link or junction, never what it points to."""
    if path.is_symlink():
        path.unlink()
        print(f"Removed the link {path}")
    elif path.is_junction():
        # rmdir removes the junction itself, not the folder's contents.
        path.rmdir()
        print(f"Removed the junction {path}")


def make_junction(link: Path, target: Path) -> None:
    """Makes link a junction to the folder target (mklink /J, Windows only)."""
    result = subprocess.run(
        ["cmd", "/c", "mklink", "/J", os.path.abspath(link), os.path.abspath(target)],
        capture_output=True,
        text=True,
    )
    if result.returncode != 0:
        output = result.stdout + result.stderr
        raise SystemExit(f"Error: could not make the junction {link}: {output}")


def link_source_folders(build_dir: Path) -> None:
    """Natively, for a build laid out in its build directory: <build dir>/Python and
    <build dir>/Resources as junctions to the source tree's (the od-fs that has this script)."""
    source_dir = HERE.parents[1]
    for name in ["Python", "Resources"]:
        link = build_dir / name
        remove_link(link)
        if link.exists():
            # REVIEW: Left alone rather than deleted, since it is not something this step made.
            print(f"Warning: {link} is in the way: not a link or junction, so it is left alone")
            continue
        make_junction(link, source_dir / name)
        print(f"Made the junction {link} -> {source_dir / name}")


def lay_out(prefix: str, build_dir: Path, od_fs_dir: Path | None, wine: bool) -> Path:
    """Lays out the build, and returns Libs/<triplet>."""
    copy_windows_dlls = load_copy_windows_dlls()
    libs_dir = (od_fs_dir or build_dir) / "Libs" / TRIPLET
    # An earlier version made Libs/<triplet> a link to the MinGW prefix; never copy into that.
    remove_link(libs_dir)
    libs_dir.mkdir(parents=True, exist_ok=True)
    remove_link(libs_dir / "lib")
    if (libs_dir / "lib").exists():
        print(
            f"Warning: {libs_dir / 'lib'} is in the way: FS-UAE uses the folder as the Python "
            "home when it has lib/python3.X, and then ignores FS-UAE._pth"
        )

    exe = libs_dir / "FS-UAE.exe"
    shutil.copy2(build_dir / "fs-uae.exe", exe)
    copy_windows_dlls.copy_dlls(prefix, str(libs_dir), [str(exe)])

    imports = copy_windows_dlls.dependencies(copy_windows_dlls.find_objdump(), str(exe))
    # Joined with "/", which cygpath and Windows take too (MinGW's Python in MSYS2 gets the prefix
    # as C:/msys64/mingw64).
    stdlib_dir = f"{prefix}/lib/python3.{python_minor_version(imports)}"
    for path in [stdlib_dir, stdlib_dir + "/lib-dynload"]:
        if not os.path.isdir(path):
            raise SystemExit(f"Error: {path} is missing")
    # Bytes, so that the line ends stay CRLF.
    (libs_dir / "FS-UAE._pth").write_bytes(pth_content(stdlib_dir, wine).encode())

    if od_fs_dir is not None:
        shutil.copy2(build_dir / "proxy.exe", od_fs_dir / "FS-UAE.exe")
    elif not wine:
        link_source_folders(build_dir)
    return libs_dir


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument(
        "--wine", action="store_true", help="Z: drive paths in FS-UAE._pth (cross-build)"
    )
    parser.add_argument("prefix", help="the MinGW prefix (/mingw64 in MSYS2)")
    parser.add_argument("build_dir", type=Path, help="the build directory, with fs-uae.exe")
    parser.add_argument(
        "od_fs_dir",
        type=Path,
        nargs="?",
        help="od-fs, to lay out the default build there, with the proxy",
    )
    args = parser.parse_args()
    libs_dir = lay_out(args.prefix, args.build_dir, args.od_fs_dir, args.wine)
    print(f"Built {libs_dir / 'FS-UAE.exe'}")


if __name__ == "__main__":
    main()
