#!/usr/bin/env python3
"""The libraries a Linux dist bundles, and the ELF reading the dist check shares.

Usage: linux_libs.py --search-dir DIR [--search-dir DIR ...] <dest dir> <binary>...

Walks the dependencies (DT_NEEDED) of the binaries (the executable and Python's lib-dynload/*.so)
and copies every library they need, directly or through other libraries, from the first search
directory that has it into <dest dir>, except the libraries on the exclusion list (EXCLUDED), which
must come from the user's system. Exits with 1, naming the libraries and what needs them, if a
needed library is neither found nor excluded. Run by assemble.sh.

The ELF files are read, never run (and no ldd or readelf is needed), so the walk works on a dist for
another architecture too. A library is found by its file name, which must be the name it is needed
by (its soname), as in /usr/lib/<triplet> and the env prefix's lib directory.

Only the Python standard library is used, and the module works with Python 3.12 (the build
machine's python3 runs it).
"""

import argparse
import fnmatch
import shutil
import struct
import sys
from collections.abc import Iterable
from pathlib import Path

# Libraries that must come from the user's system, by the name before ".so" in their soname (shell
# patterns, case-sensitive): they are never bundled, and the dist check fails if one is.
EXCLUDED = [
    # glibc
    "libc",
    "libm",
    "libdl",
    "libpthread",
    "librt",
    "libresolv",
    "libutil",
    "libanl",
    "libmvec",
    "ld-linux*",
    # The C++ and GCC runtime libraries, which must match (or be newer than) the system's glibc
    "libstdc++",
    "libgcc_s",
    # GL, EGL, GLX and GLES (the system's drivers, with vendor libraries such as libEGL_mesa)
    "libGL",
    "libGLX*",
    "libGLdispatch",
    "libOpenGL",
    "libEGL*",
    "libGLES*",
    # X11, xcb and Wayland (the display server's client libraries)
    "libX*",
    "libxcb*",
    "libxkbcommon*",
    "libwayland-*",
    "libdecor-*",
    # Audio: ALSA, PulseAudio, PipeWire and JACK (the sound server's client libraries)
    "libasound",
    "libpulse*",
    "libpipewire-*",
    "libjack*",
    # D-Bus, udev, DRM and GBM
    "libdbus-1",
    "libudev",
    "libdrm*",
    "libgbm",
]


def is_excluded(soname: str) -> bool:
    """Whether a library (by soname, e.g. libGL.so.1) must come from the user's system."""
    name = soname.split(".so", 1)[0]
    return any(fnmatch.fnmatchcase(name, pattern) for pattern in EXCLUDED)


# --------------------------------------------------------------------------------------------------
# ELF reading

DT_NULL = 0
DT_NEEDED = 1
DT_RPATH = 15
DT_RUNPATH = 29
SHT_DYNAMIC = 6


class ElfError(Exception):
    pass


class ElfInfo:
    """What the dynamic linker reads from an ELF file to find its libraries."""

    def __init__(self, machine: int, needed: list[str], runpath: str | None, rpath: str | None):
        # The architecture (e_machine: 62 for x86_64, 183 for aarch64, ...).
        self.machine = machine
        self.needed = needed
        self.runpath = runpath
        self.rpath = rpath

    @property
    def search_path(self) -> str | None:
        """Where the dynamic linker looks for the needed libraries: DT_RUNPATH if the file has it
        (DT_RPATH is then ignored), otherwise DT_RPATH."""
        return self.runpath if self.runpath is not None else self.rpath


def read_elf(data: bytes) -> ElfInfo:
    """Reads the dynamic section of an ELF file (any class, byte order or architecture).

    The dynamic section and its string table are found through the section headers, which strip
    keeps. Raises ElfError if data is not an ELF file or has no dynamic section.
    """
    if data[:4] != b"\x7fELF":
        raise ElfError("not an ELF file")
    elf_class, byte_order = data[4], data[5]
    if elf_class not in (1, 2) or byte_order not in (1, 2):
        raise ElfError("unknown ELF class or byte order")
    is64 = elf_class == 2
    end = "<" if byte_order == 1 else ">"
    try:
        machine = struct.unpack_from(end + "H", data, 18)[0]
        if is64:
            shoff = struct.unpack_from(end + "Q", data, 40)[0]
            shentsize, shnum = struct.unpack_from(end + "HH", data, 58)
            section_format = end + "IIQQQQIIQQ"
            dynamic_format = end + "qQ"
        else:
            shoff = struct.unpack_from(end + "I", data, 32)[0]
            shentsize, shnum = struct.unpack_from(end + "HH", data, 46)
            section_format = end + "IIIIIIIIII"
            dynamic_format = end + "iI"

        # (type, offset, size, link) of each section.
        sections = []
        for i in range(shnum):
            fields = struct.unpack_from(section_format, data, shoff + i * shentsize)
            sections.append((fields[1], fields[4], fields[5], fields[6]))

        for kind, offset, size, link in sections:
            if kind != SHT_DYNAMIC:
                continue
            _, strtab_offset, strtab_size, _ = sections[link]
            strtab = data[strtab_offset : strtab_offset + strtab_size]
            entries = []
            entry_size = struct.calcsize(dynamic_format)
            for entry_offset in range(offset, offset + size - entry_size + 1, entry_size):
                tag, value = struct.unpack_from(dynamic_format, data, entry_offset)
                if tag == DT_NULL:
                    break
                entries.append((tag, value))
            return read_dynamic(machine, entries, strtab)
    except (struct.error, IndexError) as e:
        raise ElfError(f"truncated or malformed ELF file ({e})") from e
    raise ElfError("no dynamic section")


def read_dynamic(machine: int, entries: list[tuple[int, int]], strtab: bytes) -> ElfInfo:
    def string(offset: int) -> str:
        end = strtab.find(b"\0", offset)
        if offset >= len(strtab) or end < 0:
            raise ElfError("string outside the dynamic string table")
        return strtab[offset:end].decode("utf-8", "replace")

    needed = [string(value) for tag, value in entries if tag == DT_NEEDED]
    runpaths = [string(value) for tag, value in entries if tag == DT_RUNPATH]
    rpaths = [string(value) for tag, value in entries if tag == DT_RPATH]
    return ElfInfo(
        machine,
        needed,
        ":".join(runpaths) if runpaths else None,
        ":".join(rpaths) if rpaths else None,
    )


# --------------------------------------------------------------------------------------------------
# The dependency walk


def find_library(soname: str, search_dirs: Iterable[Path]) -> Path | None:
    for directory in search_dirs:
        path = directory / soname
        if path.is_file():
            return path
    return None


def bundle(binaries: list[Path], search_dirs: list[Path], dest_dir: Path) -> int:
    """Copies the libraries the binaries need into dest_dir. Returns the exit status."""
    # The libraries to bundle (soname -> where it was found), and what needs the missing ones.
    found: dict[str, Path] = {}
    missing: dict[str, list[str]] = {}
    queue = [(path, str(path)) for path in binaries]
    seen: set[str] = set()
    while queue:
        path, description = queue.pop(0)
        try:
            info = read_elf(path.read_bytes())
        except ElfError as e:
            print(f"Error: {path}: {e}", file=sys.stderr)
            return 1
        for soname in info.needed:
            if is_excluded(soname):
                continue
            if soname in seen:
                if soname in missing:
                    missing[soname].append(description)
                continue
            seen.add(soname)
            library = find_library(soname, search_dirs)
            if library is None:
                missing[soname] = [description]
                continue
            found[soname] = library
            queue.append((library, soname))

    if missing:
        directories = ", ".join(str(directory) for directory in search_dirs)
        for soname, needed_by in sorted(missing.items()):
            print(
                f"Error: {soname} (needed by {', '.join(needed_by)}) is neither in the search "
                f"directories ({directories}) nor on the exclusion list (EXCLUDED in {__file__})",
                file=sys.stderr,
            )
        return 1

    dest_dir.mkdir(parents=True, exist_ok=True)
    for soname, library in sorted(found.items()):
        # Copies the file a symlink (libz.so.1 -> libz.so.1.3) points to, under the soname.
        shutil.copyfile(library, dest_dir / soname)
        print(f"Bundled {soname} from {library}")
    print(f"Bundled {len(found)} libraries in {dest_dir}")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Copies the libraries that Linux binaries need (except the excluded ones)."
    )
    parser.add_argument(
        "--search-dir",
        action="append",
        type=Path,
        required=True,
        help="directory to find libraries in (searched in the order given)",
    )
    parser.add_argument("dest_dir", type=Path, help="directory to copy the libraries to")
    parser.add_argument("binaries", type=Path, nargs="+", help="executables and extension modules")
    args = parser.parse_args()
    return bundle(args.binaries, args.search_dir, args.dest_dir)


if __name__ == "__main__":
    sys.exit(main())
