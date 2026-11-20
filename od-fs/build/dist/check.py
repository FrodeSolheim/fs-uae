#!/usr/bin/env python3
"""Dist check: checks what someone unpacking a dist sees.

Usage: check.py [--version VERSION] [--origin ORIGIN] [--commit COMMIT] [--signed]
                <archive or dist directory> <triplet>

Checks a dist archive (.tar.xz, .zip, or on a Mac .dmg) or dist directory for a triplet, and exits
with 1 and a list of what is wrong if any check fails. The dist entry script (dist, in this
directory) runs it on the archive as its last step; it can also be run by hand. Only the Python
standard library is used, and nothing in the dist is run, so a dist for another platform can be
checked too. A .dmg is mounted read-only with hdiutil (dmg.py), which only a Mac has.

Each check is a function in CHECKS. It gets the dist and reports what is wrong with
Dist.fail(); add new checks there. The Linux library checks share the ELF reading and the exclusion
list with the dependency walk (linux_libs.py, next to this script); the macOS checks read the Mach-O
files and their code signatures with macho.py (also next to it), without needing macOS tools.
"""

import argparse
import contextlib
import hashlib
import importlib.util
import io
import os
import plistlib
import posixpath
import re
import stat
import sys
import tarfile
import tempfile
import zipfile
from collections.abc import Callable
from functools import partial
from pathlib import Path
from xml.parsers.expat import ExpatError

import dmg
from linux_libs import ElfError, is_excluded, read_elf
from macho import (
    SLOT_ENTITLEMENTS,
    SLOT_INFO_PLIST,
    SLOT_RESOURCES,
    CodeSignature,
    MachO,
    MachOError,
    format_version,
    is_macho,
    read_macho,
    read_signature,
)

LINUX = "Linux"
WINDOWS = "Windows"
MACOS = "macOS"

ARCHIVE_EXTENSIONS = {LINUX: ".tar.xz", WINDOWS: ".zip", MACOS: ".dmg"}

# The directories in od-fs/Resources that FS-UAE uses at run time, which every dist has in its
# resources directory: fonts and images for the GUI, the files a WHDLoad archive needs (WHDLoad,
# uae-configuration, SKick) and the icons. The source tarball and "make install" (Makefile.am) have
# them too, which od-fs/tests/build/test_install.py checks against this list.
RESOURCE_DIRS = ["Fonts", "Images", "WHDLoad", "UAE", "SKick", "src/svg", "svg-custom"]

# The Python minor version every dist bundles (3.14), so that the dists can share
# Python/FS-UAE.zip. The env recipe (Linux, macOS) and the MSYS2 snapshot (Windows) pin the version.
PYTHON_MINOR = 14

# The .pyc magic number (the first two bytes, little-endian) of each Python 3 minor version, for
# checking dists that bundle another Python version than the one running this script. Add new
# versions here (python3.X -c "import importlib.util; print(importlib.util.MAGIC_NUMBER)").
PYC_MAGIC = {12: 3531, 13: 3571, 14: 3627}

# The ELF machine (e_machine) of a Linux dist's executable and libraries, by the CPU part of its
# triplet, and the other way round for messages.
ELF_MACHINES = {"x86_64": 62, "aarch64": 183}
ELF_MACHINE_NAMES = {number: cpu for cpu, number in ELF_MACHINES.items()}

# The purge list: parts of the Python standard library that the assemble step leaves out of every
# dist (packages, modules and extension modules, by top-level name).
PURGED_MODULES = {
    "tkinter",
    "_tkinter",
    "idlelib",
    "turtle",
    "turtledemo",
    "curses",
    "_curses",
    "_curses_panel",
    "readline",
    "dbm",
    "_dbm",
    "_gdbm",
    "ensurepip",
    "test",
    "_ctypes_test",
}

# The purge list also has CPython's test and example extension modules: _ctypes_test above, and by
# name prefix _testcapi, _testinternalcapi and the other _test* modules (including the Windows-only
# _testconsole), xxlimited, xxsubtype and the other xx* modules, and _xxtestfuzz. The env Python is
# built without them (configure --disable-test-modules); the MSYS2 one has them.
PURGED_MODULE_PREFIXES = ("_test", "xx", "_xx")

# The minimum macOS version of the macOS dists (MACOSX_DEPLOYMENT_TARGET in the env prefix's run
# script, and -mmacosx-version-min). arm64 Macs start at macOS 11.0, so an arm64 dist's files may
# say 11.0 instead: linkers raise the minimum to it.
MACOS_MINIMUM_VERSION = (10, 13)
MACOS_FIRST_VERSION = {"arm64": (11, 0)}

# The executable in a macOS dist (@executable_path is its directory).
# REVIEW: From what assemble.sh makes on macOS; not checked against a macOS dist yet.
MACOS_EXECUTABLE = "FS-UAE.app/Contents/MacOS/FS-UAE"
# Where standalone.py puts the libraries the app loads, and the app's Info.plist.
MACOS_FRAMEWORKS = "FS-UAE.app/Contents/Frameworks"
MACOS_INFO_PLIST = "FS-UAE.app/Contents/Info.plist"
# The app category in Info.plist: a game, so that macOS 14 and later turn on Game Mode when the
# app is fullscreen.
MACOS_APP_CATEGORY = "public.app-category.games"
# The bundle versions in Info.plist: the numbers of the dist version (up to three, which macOS
# takes there).
BUNDLE_VERSION_PATTERN = r"\d+\.\d+\.\d+"

# The entitlements the executable of a signed macOS dist is signed with (assemble.sh), in macos/
# next to this script: the hardened runtime only lets the JITs make code executable with them.
MACOS_ENTITLEMENTS = {"x86_64": "entitlements-x86_64.plist"}
MACOS_DEFAULT_ENTITLEMENTS = "entitlements.plist"

# Modules FS-UAE needs: a dist without them must not be published. Each is an extension module in
# lib-dynload, or built into libpython (the Debian/Ubuntu Python has zlib built in).
REQUIRED_MODULES = ["_ctypes", "_hashlib", "zlib"]


# The versions of dists (see version.py): <major>.<minor>.<revision> for a tag build, otherwise
# <major>.<minor>.<revision>-dev.<YYYYMMDD>T<HHMM>+g<abbreviated commit hash, 10 or more digits>.
VERSION_PATTERN = r"\d+\.\d+\.\d+(?:-dev\.\d{8}T\d{4}\+g[0-9a-f]{10,})?"


# The version string in the FS-UAE executable, which it logs at start-up (FSUAE_VERSION_STRING in
# od-fs/fsuae/fsuae-buildinfo.cpp), as a C string.
PROGRAM_VERSION_PATTERN = rb"FS-UAE version ([\x21-\x7e]+)\0"

# The build header in the FS-UAE executable, which it logs first in FS-UAE.log.txt
# (g_build_header in od-fs/fsuae/fsuae-buildinfo.cpp), as a C string: the version line, then
# indented "<name>: <value>" lines.
BUILD_HEADER_PATTERN = rb"(FS-UAE version [\x21-\x7e]+(?:\n  [\x20-\x7e]+)+)\0"

# The Portable.ini that a user copies to make a portable directory (see README.txt), and how it must
# start for FS-UAE to take it (check_portable_ini in od-fs/fsuae/fsuae-init.cpp).
PORTABLE_INI = "Extras/Portable.ini"
PORTABLE_INI_START = b"# FS-UAE"


def os_for_triplet(triplet: str) -> str | None:
    if "-linux-" in triplet:
        return LINUX
    if triplet.endswith("-w64-mingw32"):
        return WINDOWS
    if triplet.endswith("-apple-darwin"):
        return MACOS
    return None


class Dist:
    """The files and directories of a dist, as relative paths with "/" separators."""

    def __init__(self, triplet: str, os_name: str) -> None:
        self.triplet = triplet
        self.os = os_name
        self.failures: list[str] = []
        # Path -> file mode (None when the archive format has none), for files and directories.
        self.files: dict[str, int | None] = {}
        self.dirs: set[str] = set()
        self._readers: dict[str, Callable[[], bytes]] = {}
        # The bundled Python's minor version, once check_python_version has found it.
        self.python_minor: int | None = None
        # The bundled standard library's files, by top-level module (see stdlib_modules).
        self._stdlib_modules: dict[str, list[str]] | None = None
        # The archive's file name (None for a dist directory), and the version it must have.
        self.archive_name: str | None = None
        self.version: str | None = None
        # The origin the program must have (see check_build_header).
        self.origin: str | None = None
        # The commit the program must have been built from (see check_build_header).
        self.commit: str | None = None
        # Whether the dist must be signed with a Developer ID certificate (macOS).
        self.signed = False
        # The Mach-O files in a macOS dist's app (see macos_binaries).
        self._macos_binaries: dict[str, MachO] | None = None
        # The mounted volume of a .dmg (see load_dmg), while the checks run.
        self.dmg_volume: Path | None = None

    def fail(self, message: str) -> None:
        self.failures.append(message)

    def add_file(self, path: str, mode: int | None, reader: Callable[[], bytes]) -> None:
        self.files[path] = mode
        self._readers[path] = reader
        self._add_parents(path)

    def add_dir(self, path: str) -> None:
        self.dirs.add(path)
        self._add_parents(path)

    def _add_parents(self, path: str) -> None:
        parent = posixpath.dirname(path)
        while parent:
            self.dirs.add(parent)
            parent = posixpath.dirname(parent)

    def read(self, path: str) -> bytes:
        return self._readers[path]()

    def files_in(self, path: str) -> list[str]:
        """The files in directory path and its subdirectories."""
        prefix = path + "/"
        return sorted(name for name in self.files if name.startswith(prefix))

    # Where things are in the dist (macOS has them in the app bundle).

    @property
    def python_dir(self) -> str:
        if self.os == MACOS:
            return "FS-UAE.app/Contents/Resources/Python"
        return "Python"

    @property
    def python_home(self) -> str:
        """The bundled Python's home: Libs/<triplet>, so that several platforms' dists can be
        unpacked into one directory (a combined dist). The macOS app bundle keeps it in
        Contents/Resources/Python."""
        if self.os == MACOS:
            return self.python_dir
        return self.libs_dir

    @property
    def python_lib_dir(self) -> str:
        """Where the bundled Python's standard library zip and python3.X directory are."""
        return f"{self.python_home}/lib"

    def dynload_dir(self, minor: int) -> str:
        """Where the bundled Python 3.<minor>'s extension modules are."""
        return f"{self.python_lib_dir}/python3.{minor}/lib-dynload"

    def stdlib_modules(self) -> dict[str, list[str]]:
        """The files of the bundled standard library (its zip and python3.X), by top-level module.

        The files are given as paths in the dist, with the path in the zip after a colon. Empty
        until check_python_version has found the Python version.
        """
        if self._stdlib_modules is not None:
            return self._stdlib_modules
        minor = self.python_minor
        if minor is None:
            return {}
        modules: dict[str, list[str]] = {}
        self._stdlib_modules = modules
        files = []
        zip_path = f"{self.python_lib_dir}/python3{minor}.zip"
        try:
            with zipfile.ZipFile(io.BytesIO(self.read(zip_path))) as zf:
                names = [name for name in zf.namelist() if not name.endswith("/")]
            files += [(name, f"{zip_path}:{name}") for name in names]
        except zipfile.BadZipFile as e:
            self.fail(f"{zip_path} is not a valid zip file: {e}")
        stdlib_dir = f"{self.python_lib_dir}/python3.{minor}"
        files += [(path[len(stdlib_dir) + 1 :], path) for path in self.files_in(stdlib_dir)]
        for name, path in files:
            modules.setdefault(top_level_module(name), []).append(path)
        return modules

    def macos_binaries(self) -> dict[str, MachO]:
        """The Mach-O files in a macOS dist's app, each as its slice for the dist's architecture.
        Files that are not valid Mach-O files or have no such slice are reported (once) and left
        out."""
        if self._macos_binaries is not None:
            return self._macos_binaries
        binaries: dict[str, MachO] = {}
        self._macos_binaries = binaries
        for path in self.files_in("FS-UAE.app"):
            data = self.read(path)
            if not is_macho(data):
                continue
            try:
                slices = read_macho(data)
            except MachOError as e:
                self.fail(f"{path} is not a valid Mach-O file: {e}")
                continue
            matching = [macho for macho in slices if macho.cpu == self.cpu]
            if not matching:
                cpus = " and ".join(macho.cpu for macho in slices)
                self.fail(f"{path} is for {cpus}, not {self.cpu}")
                continue
            binaries[path] = matching[0]
        return binaries

    @property
    def libs_dir(self) -> str:
        return f"Libs/{self.triplet}"

    @property
    def archive_version(self) -> str | None:
        """The version in the archive's name, if it is a well-formed archive name for the dist."""
        if self.archive_name is None:
            return None
        extension = ARCHIVE_EXTENSIONS[self.os]
        match = re.fullmatch(
            rf"FS-UAE-({VERSION_PATTERN})-" + re.escape(self.triplet) + re.escape(extension),
            self.archive_name,
        )
        return match.group(1) if match else None

    @property
    def program(self) -> str:
        """The FS-UAE executable (on Windows the real one, which the proxy at the top runs)."""
        if self.os == LINUX:
            return "FS-UAE"
        if self.os == WINDOWS:
            return f"{self.libs_dir}/FS-UAE.exe"
        return MACOS_EXECUTABLE

    @property
    def cpu(self) -> str:
        """The CPU part of the triplet (x86_64, aarch64, ...)."""
        return self.triplet.split("-", 1)[0]

    @property
    def resources_dir(self) -> str:
        if self.os == MACOS:
            return "FS-UAE.app/Contents/Resources"
        return "Resources"


def load_directory(path: Path, dist: Dist, root: Path | None = None) -> None:
    """Adds the files and directories in path to the dist, as paths relative to root (default:
    path itself)."""
    root = root or path
    for item in sorted(path.rglob("*")):
        name = item.relative_to(root).as_posix()
        if item.is_dir():
            dist.add_dir(name)
        elif item.is_file():
            dist.add_file(name, item.stat().st_mode, item.read_bytes)


def load_dmg_volume(volume: Path, dist: Dist) -> None:
    """Adds the files on a mounted .dmg's volume to the dist (not the Applications link or hidden
    files; check_macos_dmg checks the volume's top level)."""
    dist.dmg_volume = volume
    for item in dmg.visible_items(volume):
        if item.is_dir():
            dist.add_dir(item.name)
            load_directory(item, dist, volume)
        elif item.is_file():
            dist.add_file(item.name, item.stat().st_mode, item.read_bytes)


def archive_path(dist: Dist, name: str) -> str | None:
    """The path of an archive entry in the dist, or None (and a failure) if it is unsafe."""
    path = name.rstrip("/")
    while path.startswith("./"):
        path = path[2:]
    if path in ("", "."):
        return None
    if path.startswith("/") or ".." in path.split("/"):
        dist.fail(f"Unsafe path in the archive: {name}")
        return None
    return path


def load_tar(tf: tarfile.TarFile, dist: Dist) -> None:
    for member in tf.getmembers():
        path = archive_path(dist, member.name)
        if path is None:
            continue
        if member.isdir():
            dist.add_dir(path)
        elif member.isfile():
            dist.add_file(path, member.mode, partial(read_tar_member, tf, member))
        else:
            # Links and other special files: present, but nothing to read.
            dist.add_file(path, member.mode, bytes)


def read_tar_member(tf: tarfile.TarFile, member: tarfile.TarInfo) -> bytes:
    file = tf.extractfile(member)
    assert file is not None
    return file.read()


def load_zip(zf: zipfile.ZipFile, dist: Dist) -> None:
    for info in zf.infolist():
        path = archive_path(dist, info.filename)
        if path is None:
            continue
        if info.is_dir():
            dist.add_dir(path)
        else:
            # Unix permissions, when the zip file was made on a Unix-like system.
            mode = info.external_attr >> 16 if info.create_system == 3 else None
            dist.add_file(path, mode or None, partial(zf.read, info))


def load_dmg(stack: contextlib.ExitStack, path: Path, dist: Dist) -> None:
    """Mounts the .dmg at path (on a Mac) until stack is closed, and loads its volume."""
    if not dmg.available():
        dist.fail("Cannot look inside a .dmg without hdiutil (macOS); check the dist directory")
        return
    temp_dir = Path(stack.enter_context(tempfile.TemporaryDirectory(prefix="fs-uae-dmg-")))
    try:
        volume = stack.enter_context(dmg.attached(path, temp_dir / "volume"))
    except dmg.DmgError as e:
        dist.fail(f"Cannot open the .dmg ({e})")
        return
    load_dmg_volume(volume, dist)


# --------------------------------------------------------------------------------------------------
# Checks


def check_archive_name(dist: Dist) -> None:
    """The archive is named FS-UAE-<version>-<triplet>.<extension for the platform>, where the
    version is that of a tag build or of another build (see VERSION_PATTERN)."""
    if dist.archive_name is None:
        return
    if dist.archive_version is None or (
        dist.version is not None and dist.archive_version != dist.version
    ):
        extension = ARCHIVE_EXTENSIONS[dist.os]
        expected = f"FS-UAE-{dist.version or '<version>'}-{dist.triplet}{extension}"
        dist.fail(f"Wrong archive name: {dist.archive_name} (expected {expected})")


def check_program_version(dist: Dist) -> None:
    """The program shows the dist's version: the version string in the executable
    (PROGRAM_VERSION_PATTERN) is the version given, or else the archive name's."""
    if dist.program not in dist.files:
        return  # Reported by check_layout
    versions = set(re.findall(PROGRAM_VERSION_PATTERN, dist.read(dist.program)))
    if not versions:
        dist.fail(f"{dist.program} has no version string (FS-UAE version <version>)")
        return
    expected = dist.version or dist.archive_version
    found = ", ".join(sorted(version.decode() for version in versions))
    if expected is not None and versions != {expected.encode()}:
        dist.fail(f"{dist.program} has the version {found}, not {expected}")


def check_build_header(dist: Dist) -> None:
    """The program has the build header (BUILD_HEADER_PATTERN) for the dist: its version (as in
    check_program_version), triplet, an origin (the one given, if any) and the commit given (if
    any)."""
    if dist.program not in dist.files:
        return  # Reported by check_layout
    headers = re.findall(BUILD_HEADER_PATTERN, dist.read(dist.program))
    if not headers:
        dist.fail(f"{dist.program} has no build header (FS-UAE version <version>, Origin: ...)")
        return
    if len(headers) > 1:
        dist.fail(f"{dist.program} has {len(headers)} build headers")
        return
    header = headers[0].decode()
    version = header.splitlines()[0].removeprefix("FS-UAE version ")
    expected_version = dist.version or dist.archive_version
    if expected_version is not None and version != expected_version:
        dist.fail(
            f"{dist.program} has the version {version} in its build header, not {expected_version}"
        )
    fields = build_header_fields(header)
    triplet = fields.get("Triplet")
    if triplet != dist.triplet:
        dist.fail(
            f"{dist.program} has the triplet {triplet} in its build header, not {dist.triplet}"
        )
    origin = fields.get("Origin")
    if not origin:
        dist.fail(f"{dist.program} has no origin in its build header")
    elif dist.origin is not None and origin != dist.origin:
        dist.fail(f"{dist.program} has the origin {origin}, not {dist.origin}")
    commit = fields.get("Commit")
    if dist.commit is not None and commit != dist.commit:
        dist.fail(f"{dist.program} has the commit {commit}, not {dist.commit}")


def build_header_fields(header: str) -> dict[str, str]:
    """The "<name>: <value>" lines of a build header, by name."""
    fields: dict[str, str] = {}
    for line in header.splitlines()[1:]:
        name, _, value = line.strip().partition(": ")
        fields[name] = value
    return fields


def check_layout(dist: Dist) -> None:
    """The executables, Python code, resources and other files are where FS-UAE looks for them."""
    executables = [dist.program]
    required_dirs = []
    if dist.os == LINUX:
        required_dirs.append(dist.libs_dir)
    elif dist.os == WINDOWS:
        # The proxy at the top, which runs the real executable in Libs/<triplet>.
        executables.insert(0, "FS-UAE.exe")
    required_files = executables + [
        "README.txt",
        PORTABLE_INI,
        f"{dist.python_dir}/FS-UAE.zip",
    ]
    if dist.os == MACOS:
        required_files += [MACOS_INFO_PLIST, f"{dist.resources_dir}/FS-UAE.icns"]
    for name in RESOURCE_DIRS:
        required_dirs.append(f"{dist.resources_dir}/{name}")

    for path in required_files:
        if path not in dist.files:
            dist.fail(f"{path} is missing")
    for path in required_dirs:
        if not dist.files_in(path):
            dist.fail(f"{path}/ is missing or empty")
    for path in executables:
        mode = dist.files.get(path)
        if dist.os != WINDOWS and mode is not None and not mode & stat.S_IXUSR:
            dist.fail(f"{path} is not executable")


def check_readme(dist: Dist) -> None:
    """README.txt has Windows line endings (CRLF) on every line, as assemble.sh makes it with
    unix2dos for every platform, and a macOS dist's has the macOS section."""
    if "README.txt" not in dist.files:
        return  # Reported by check_layout
    data = dist.read("README.txt")
    if data.count(b"\n") != data.count(b"\r\n"):
        dist.fail("README.txt has lines that do not end with CRLF (unix2dos)")
    # On macOS, the portable directory cannot be the .dmg, and the README says what to do instead.
    if dist.os == MACOS and not re.search(rb"^### macOS\r?$", data, re.MULTILINE):
        dist.fail("README.txt has no macOS section (### macOS)")


def check_portable_ini(dist: Dist) -> None:
    """Extras/Portable.ini is one that FS-UAE takes when the user copies it to a portable
    directory, and there is no other Portable.ini: FS-UAE looks for one from its own directory
    up, so the dist (on macOS the read-only .dmg) would be its portable directory."""
    for path in sorted(dist.files):
        if posixpath.basename(path) == "Portable.ini" and path != PORTABLE_INI:
            dist.fail(f"{path}: Portable.ini is not in Extras (the dist would be portable)")
    if PORTABLE_INI not in dist.files:
        return  # Reported by check_layout
    if not dist.read(PORTABLE_INI).startswith(PORTABLE_INI_START):
        dist.fail(f'{PORTABLE_INI} does not start with "{PORTABLE_INI_START.decode()}"')


def check_python_version(dist: Dist) -> None:
    """Python 3.14 is bundled, once: its standard library zip, lib-dynload and libpython."""
    lib_dir = dist.python_lib_dir
    minors = []
    for path in dist.files:
        match = re.fullmatch(re.escape(lib_dir) + r"/python3(\d+)\.zip", path)
        if match:
            minors.append(int(match.group(1)))
    if not minors:
        dist.fail(f"{lib_dir}/python3X.zip (the Python standard library) is missing")
        return
    if len(minors) > 1:
        names = ", ".join(f"python3{minor}.zip" for minor in sorted(minors))
        dist.fail(f"{lib_dir} has more than one Python standard library: {names}")
        return
    minor = minors[0]
    dist.python_minor = minor
    if minor != PYTHON_MINOR:
        dist.fail(f"Python 3.{minor} is bundled, but dists must bundle Python 3.{PYTHON_MINOR}")

    dynload_dir = dist.dynload_dir(minor)
    if not dist.files_in(dynload_dir):
        dist.fail(f"{dynload_dir}/ is missing or empty")
    libpython = libpython_path(dist, minor)
    if libpython not in dist.files:
        dist.fail(f"{libpython} is missing (the standard library is for Python 3.{minor})")


def check_shared_python_dir(dist: Dist) -> None:
    """The shared Python directory has FS-UAE's own code only: the standard library is in each
    platform's Python home (Libs/<triplet>), where other platforms' dists in the same directory do
    not find it."""
    if dist.os == MACOS:
        return  # The app bundle has its own Python directory, which is its Python home.
    fsuae_zip = f"{dist.python_dir}/FS-UAE.zip"
    platform_files = [path for path in dist.files_in(dist.python_dir) if path != fsuae_zip]
    fail_for_files(
        dist,
        f"Platform files in the shared {dist.python_dir} directory (the standard library belongs "
        f"in the Python home, {dist.python_lib_dir})",
        platform_files,
    )


def libpython_path(dist: Dist, minor: int) -> str:
    """Where the dist has libpython for Python 3.<minor>."""
    if dist.os == LINUX:
        return f"{dist.libs_dir}/libpython3.{minor}.so.1.0"
    if dist.os == WINDOWS:
        return f"{dist.libs_dir}/libpython3.{minor}.dll"
    # standalone.py copies the libraries the executable loads into Contents/Frameworks, by the file
    # name in their install name (the env Python's is <prefix>/lib/libpython3.X.dylib).
    # REVIEW: Not checked against a macOS dist yet.
    return f"{MACOS_FRAMEWORKS}/libpython3.{minor}.dylib"


def top_level_module(path: str) -> str:
    """The top-level module that a file in the standard library belongs to.

    tkinter/ttk.py -> tkinter, __pycache__/turtle.cpython-314.pyc -> turtle,
    lib-dynload/_tkinter.cpython-314-x86_64-linux-gnu.so -> _tkinter.
    """
    parts = path.split("/")
    if len(parts) > 1 and parts[0] in ("__pycache__", "lib-dynload"):
        parts = parts[1:]
    if len(parts) > 1:
        return parts[0]
    return parts[0].split(".")[0]


def is_purged_module(name: str) -> bool:
    """Whether a top-level module of the standard library is on the purge list."""
    return name in PURGED_MODULES or name.startswith(PURGED_MODULE_PREFIXES)


def check_purged_modules(dist: Dist) -> None:
    """The parts of the standard library on the purge list are left out."""
    modules = dist.stdlib_modules()
    for name in sorted(filter(is_purged_module, modules)):
        paths = sorted(modules[name])
        more = f" and {len(paths) - 1} more files" if len(paths) > 1 else ""
        dist.fail(f"Purged Python module {name} is in the dist: {paths[0]}{more}")


def check_required_modules(dist: Dist) -> None:
    """The modules FS-UAE needs are there."""
    minor = dist.python_minor
    if minor is None:
        return  # Reported by check_python_version
    modules = dist.stdlib_modules()
    dynload_dir = dist.dynload_dir(minor)
    libpython = libpython_path(dist, minor)
    libpython_data = None
    for name in REQUIRED_MODULES:
        if any(path.startswith(dynload_dir + "/") for path in modules.get(name, [])):
            continue
        # A module built into libpython has its init function there, as an exported symbol: a
        # string of its own in the symbol table (with a leading underscore on macOS).
        if libpython in dist.files:
            if libpython_data is None:
                libpython_data = dist.read(libpython)
            if re.search(rb"\0_?PyInit_" + re.escape(name.encode()) + rb"\0", libpython_data):
                continue
        dist.fail(
            f"Required Python module {name} is missing (not in {dynload_dir}/ or built into "
            f"{libpython})"
        )


def is_shared_object(path: str) -> bool:
    name = posixpath.basename(path)
    return name.endswith(".so") or ".so." in name


def expected_rpath(path: str, libs_dir: str) -> str:
    """The rpath that finds libs_dir from where the ELF file path is: $ORIGIN/Libs/<triplet> for the
    executable, $ORIGIN for the bundled libraries, and so on."""
    relative = posixpath.relpath(libs_dir, posixpath.dirname(path) or ".")
    return "$ORIGIN" if relative == "." else f"$ORIGIN/{relative}"


def check_linux_libraries(dist: Dist) -> None:
    """Every library the executable, the bundled libraries and the Python extension modules need is
    bundled in Libs/<triplet> or must come from the user's system (the exclusion list), nothing on
    the exclusion list is anywhere in the dist, the rpaths find Libs/<triplet> from where each file
    is, and every ELF file is for the triplet's architecture."""
    if dist.os != LINUX:
        return
    machine = ELF_MACHINES.get(dist.cpu)
    if machine is None:
        dist.fail(f"The ELF machine for {dist.cpu} is not known (add it to ELF_MACHINES)")
    libs_dir = dist.libs_dir
    shared_objects = sorted(path for path in dist.files if is_shared_object(path))
    bundled = {
        posixpath.basename(path) for path in shared_objects if posixpath.dirname(path) == libs_dir
    }
    for path in shared_objects:
        if is_excluded(posixpath.basename(path)):
            dist.fail(
                f"{path} is bundled, but it is on the exclusion list (it must come from the "
                "user's system)"
            )

    elf_files = ["FS-UAE"] + shared_objects if "FS-UAE" in dist.files else shared_objects
    needed_by: dict[str, list[str]] = {}
    for path in elf_files:
        try:
            info = read_elf(dist.read(path))
        except ElfError as e:
            dist.fail(f"{path} is not a valid ELF shared object or executable: {e}")
            continue
        if machine is not None and info.machine != machine:
            name = ELF_MACHINE_NAMES.get(info.machine, f"ELF machine {info.machine}")
            dist.fail(f"{path} is for {name}, not {dist.cpu}")
        for soname in info.needed:
            if soname not in bundled and not is_excluded(soname):
                needed_by.setdefault(soname, []).append(path)
        expected = expected_rpath(path, libs_dir)
        if info.search_path is None:
            dist.fail(f"{path} has no rpath (expected {expected})")
        elif info.search_path != expected:
            dist.fail(f"{path} has the rpath {info.search_path} (expected {expected})")
    for soname, paths in sorted(needed_by.items()):
        fail_for_files(
            dist,
            f"{soname} is neither bundled in {libs_dir} nor on the exclusion list, but it is "
            "needed by",
            paths,
        )


def macos_reference_origin(path: str) -> str | None:
    """Where an absolute library path or rpath in a macOS dist points, if not to the system's
    libraries (/usr/lib, /System), which every Mac has."""
    if path.startswith(("/usr/lib/", "/System/")):
        return None
    if "/.build/env/" in path:
        return "into the env prefix"
    if path.startswith(("/opt/homebrew/", "/usr/local/")):
        return "into Homebrew"
    return "outside the app and the system"


def resolve_macos_path(path: str, loader: str) -> str:
    """A path with @executable_path or @loader_path (the directory of loader, the file with the
    load command) as a path in the dist."""
    for prefix, directory in [
        ("@executable_path/", posixpath.dirname(MACOS_EXECUTABLE)),
        ("@loader_path/", posixpath.dirname(loader)),
    ]:
        if path.startswith(prefix):
            return posixpath.normpath(posixpath.join(directory, path[len(prefix) :]))
    return path


def resolve_macos_library(library: str, loader: str, rpaths: list[tuple[str, str]]) -> list[str]:
    """Where dyld looks for a library that loader loads, as paths in the dist. rpaths are the
    (rpath, file it is in) that dyld tries for @rpath: the loader's, then the executable's."""
    if library.startswith("@rpath/"):
        name = library[len("@rpath/") :]
        return [
            resolve_macos_path(posixpath.join(rpath, name), rpath_loader)
            for rpath, rpath_loader in rpaths
        ]
    return [resolve_macos_path(library, loader)]


def check_macos_binaries(dist: Dist) -> None:
    """The executable, the bundled libraries and the Python extension modules load libraries only
    from the app and the system, and every library they load from the app is there: no references
    into the env prefix, Homebrew or anywhere else on the build machine (as library paths, install
    names or rpaths). Every library in Contents/Frameworks is loaded by something in the app."""
    if dist.os != MACOS:
        return
    binaries = dist.macos_binaries()
    executable = binaries.get(MACOS_EXECUTABLE)
    executable_rpaths = (
        [(rpath, MACOS_EXECUTABLE) for rpath in executable.rpaths] if executable else []
    )
    loaded: set[str] = set()
    for path, macho in binaries.items():
        name = macho.install_name
        if name and name.startswith("/") and (origin := macos_reference_origin(name)):
            dist.fail(f"{path} has the install name {name}, {origin}")
        for rpath in macho.rpaths:
            if rpath.startswith("/"):
                origin = macos_reference_origin(rpath) or "on the system"
                dist.fail(f"{path} has the rpath {rpath}, {origin} (rpaths must be in the app)")
        rpaths = [(rpath, path) for rpath in macho.rpaths]
        if path != MACOS_EXECUTABLE:
            rpaths += executable_rpaths
        for library, required in macho.dylibs:
            if library.startswith("/"):
                if origin := macos_reference_origin(library):
                    dist.fail(f"{path} loads {library}, {origin}")
                continue
            candidates = resolve_macos_library(library, path, rpaths)
            loaded.update(candidates)
            if required and not any(candidate in dist.files for candidate in candidates):
                dist.fail(f"{path} loads {library}, which is not in the app")
    # standalone.py copies the libraries that are loaded into Contents/Frameworks, so one that
    # nothing loads was left behind (for example with a purged extension module).
    # REVIEW: Not checked against a real macOS dist yet.
    unused = [path for path in dist.files_in(MACOS_FRAMEWORKS) if path not in loaded]
    fail_for_files(dist, "Libraries that nothing in the app loads", unused)


def check_macos_minimum_version(dist: Dist) -> None:
    """The executable is built for macOS 10.13 (the dists' minimum), and no file in the app needs
    a newer macOS (on arm64, 11.0, the first version for arm64 Macs)."""
    if dist.os != MACOS:
        return
    minimum = MACOS_MINIMUM_VERSION
    dist_minimum = max(minimum, MACOS_FIRST_VERSION.get(dist.cpu, minimum))
    for path, macho in dist.macos_binaries().items():
        version = macho.minimum_version
        if version is None:
            dist.fail(
                f"{path} has no minimum macOS version (LC_BUILD_VERSION or LC_VERSION_MIN_MACOSX)"
            )
        elif version > dist_minimum:
            dist.fail(
                f"{path} needs macOS {format_version(version)} (the dist's minimum is "
                f"{format_version(dist_minimum)})"
            )
        elif path == MACOS_EXECUTABLE and version < minimum:
            dist.fail(
                f"{path} has the minimum macOS version {format_version(version)}, not "
                f"{format_version(minimum)}"
            )


def check_macos_dmg(dist: Dist) -> None:
    """The top of a .dmg's volume has the files in dmg.VOLUME_ITEMS, the link to /Applications and
    the volume icon, and nothing else apart from hidden files (Finder's)."""
    volume = dist.dmg_volume
    if volume is None:
        return  # A dist directory.
    others = [item.name for item in dmg.visible_items(volume) if item.name not in dmg.VOLUME_ITEMS]
    fail_for_files(dist, "The .dmg has files that do not belong there", others)
    link = volume / dmg.APPLICATIONS_LINK
    if not link.is_symlink():
        dist.fail(f"The .dmg has no {dmg.APPLICATIONS_LINK} link (to {dmg.APPLICATIONS_TARGET})")
    elif (target := os.readlink(link)) != dmg.APPLICATIONS_TARGET:
        dist.fail(
            f"The .dmg's {dmg.APPLICATIONS_LINK} links to {target}, not {dmg.APPLICATIONS_TARGET}"
        )
    if not (volume / dmg.VOLUME_ICON).is_file():
        dist.fail(f"The .dmg has no volume icon ({dmg.VOLUME_ICON})")


def check_macos_info_plist(dist: Dist) -> None:
    """The app's Info.plist names the executable, its category is games (for Game Mode), and its
    bundle versions are the dist version's numbers (<major>.<minor>.<revision>, the most macOS
    takes there; the -dev part of a dist version is left out)."""
    if dist.os != MACOS:
        return
    path = MACOS_INFO_PLIST
    if path not in dist.files:
        return  # Reported by check_layout
    try:
        info = plistlib.loads(dist.read(path))
    except (ValueError, ExpatError) as e:
        dist.fail(f"{path} is not a valid property list: {e}")
        return
    if not isinstance(info, dict):
        dist.fail(f"{path} is not a valid property list: not a dictionary")
        return
    executable = posixpath.basename(MACOS_EXECUTABLE)
    if info.get("CFBundleExecutable") != executable:
        dist.fail(
            f"{path} has CFBundleExecutable {info.get('CFBundleExecutable')}, not {executable}"
        )
    category = info.get("LSApplicationCategoryType")
    if category != MACOS_APP_CATEGORY:
        dist.fail(f"{path} has LSApplicationCategoryType {category}, not {MACOS_APP_CATEGORY}")
    version = dist.version or dist.archive_version
    match = re.match(BUNDLE_VERSION_PATTERN, version) if version else None
    expected = match.group() if match else None
    for key in ["CFBundleShortVersionString", "CFBundleVersion"]:
        value = info.get(key)
        numbers = isinstance(value, str) and re.fullmatch(BUNDLE_VERSION_PATTERN, value)
        if not numbers or (expected is not None and value != expected):
            dist.fail(f"{path} has {key} {value}, not {expected or '<major>.<minor>.<revision>'}")


def check_macos_signatures(dist: Dist) -> None:
    """Every Mach-O file in the app is signed (ad hoc in an unsigned dist, which arm64 Macs need to
    run it at all), and was not changed after it was signed; the executable's signature seals the
    app's Info.plist and resources (_CodeSignature/CodeResources), which were not changed either.
    With --signed, everything is signed with the same team's Developer ID certificate, and the
    executable with the hardened runtime, as notarization needs.

    This reads the signatures and compares the hashes in them, as codesign --verify does, but it
    cannot check the certificates (that needs macOS: codesign, spctl); assemble.sh runs codesign
    --verify on a Mac."""
    if dist.os != MACOS:
        return
    signatures: dict[str, CodeSignature] = {}
    unsigned = []
    for path, macho in dist.macos_binaries().items():
        try:
            signature = read_signature(macho)
        except MachOError as e:
            dist.fail(f"{path} has a malformed code signature: {e}")
            continue
        if signature is None:
            unsigned.append(path)
            continue
        signatures[path] = signature
        if any(cd.changed_pages(macho.data) for cd in signature.code_directories):
            dist.fail(f"{path} was changed after it was signed")
        expected = signature.code_directory.special_hashes.get(SLOT_ENTITLEMENTS)
        blob = signature.entitlements_blob
        if expected and (blob is None or signature.code_directory.hash(blob) != expected):
            dist.fail(f"{path}: its entitlements were changed after it was signed")
    fail_for_files(dist, "No code signature", unsigned)

    executable = signatures.get(MACOS_EXECUTABLE)
    if executable is not None:
        check_macos_bundle_seal(dist, executable, signatures)
    if not dist.signed:
        return
    ad_hoc = [path for path, signature in signatures.items() if signature.ad_hoc]
    fail_for_files(dist, "Signed ad hoc, not with a Developer ID certificate", ad_hoc)
    if executable is None or executable.ad_hoc:
        return
    if not executable.runtime:
        dist.fail(
            f"{MACOS_EXECUTABLE} is signed without the hardened runtime (which notarization needs)"
        )
    check_macos_entitlements(dist, executable)
    for path, signature in signatures.items():
        if not signature.ad_hoc and signature.team_id != executable.team_id:
            dist.fail(f"{path} is signed by the team {signature.team_id}, not {executable.team_id}")


def check_macos_entitlements(dist: Dist, executable: CodeSignature) -> None:
    """The executable of a signed dist has the entitlements assemble.sh signs it with (the file
    for the dist's architecture in macos/), no more and no fewer."""
    name = MACOS_ENTITLEMENTS.get(dist.cpu, MACOS_DEFAULT_ENTITLEMENTS)
    expected = plistlib.loads((Path(__file__).resolve().parent / "macos" / name).read_bytes())
    found: dict[str, object] = {}
    if executable.entitlements:
        try:
            loaded = plistlib.loads(executable.entitlements)
        except (ValueError, ExpatError) as e:
            dist.fail(f"{MACOS_EXECUTABLE} has entitlements that are not a property list: {e}")
            return
        found = loaded if isinstance(loaded, dict) else {}
    missing = sorted(key for key, value in expected.items() if found.get(key) != value)
    if missing:
        dist.fail(
            f"{MACOS_EXECUTABLE} is signed without the entitlements of {name}: {', '.join(missing)}"
        )
    extra = sorted(key for key in found if key not in expected)
    if extra:
        dist.fail(
            f"{MACOS_EXECUTABLE} is signed with entitlements that {name} does not have: "
            f"{', '.join(extra)}"
        )


def check_macos_bundle_seal(
    dist: Dist, executable: CodeSignature, signatures: dict[str, CodeSignature]
) -> None:
    """The app's Info.plist and the resources and nested code listed in CodeResources have the
    hashes the executable's signature has for them."""
    app = "FS-UAE.app/Contents"
    cd = executable.code_directory
    info_plist = f"{app}/Info.plist"
    expected = cd.special_hashes.get(SLOT_INFO_PLIST)
    if expected and info_plist in dist.files and cd.hash(dist.read(info_plist)) != expected:
        dist.fail(f"{info_plist} was changed after the app was signed")

    resources_path = f"{app}/_CodeSignature/CodeResources"
    expected = cd.special_hashes.get(SLOT_RESOURCES)
    if not expected:
        dist.fail(f"{MACOS_EXECUTABLE} does not seal the app's resources (no CodeResources)")
        return
    if resources_path not in dist.files:
        dist.fail(f"{resources_path} is missing")
        return
    data = dist.read(resources_path)
    if cd.hash(data) != expected:
        dist.fail(f"{resources_path} was changed after the app was signed")
        return
    try:
        resources = plistlib.loads(data)
    except (ValueError, ExpatError) as e:
        dist.fail(f"{resources_path} is not a valid property list: {e}")
        return
    files = resources.get("files2", {}) if isinstance(resources, dict) else {}
    for name, entry in sorted(files.items()):
        path = f"{app}/{name}"
        if not isinstance(entry, dict) or "symlink" in entry:
            continue
        if path not in dist.files:
            if not entry.get("optional"):
                dist.fail(f"{path} was removed after the app was signed")
            continue
        if "hash2" in entry:
            changed = hashlib.sha256(dist.read(path)).digest() != entry["hash2"]
        elif "cdhash" in entry:
            # REVIEW: For nested code with several code directories (or a universal file), any of
            # them is taken to match; check against codesign --verify on a Mac.
            signature = signatures.get(path)
            if signature is None:
                continue  # Reported as unsigned or invalid above.
            changed = all(cd.cdhash != entry["cdhash"] for cd in signature.code_directories)
        else:
            continue
        if changed:
            dist.fail(f"{path} was changed after the app was signed")


def pyc_magic(minor: int) -> bytes | None:
    if sys.version_info[:2] == (3, minor):
        return importlib.util.MAGIC_NUMBER
    if minor in PYC_MAGIC:
        return PYC_MAGIC[minor].to_bytes(2, "little") + b"\r\n"
    return None


def hidden_part(path: str) -> str | None:
    """The path up to its first hidden (dot) file or directory, or None if it has none."""
    parts = path.split("/")
    for i, part in enumerate(parts):
        if part.startswith("."):
            return "/".join(parts[: i + 1])
    return None


def check_fsuae_zip(dist: Dist) -> None:
    """Python/FS-UAE.zip has FS-UAE's Python code, byte-compiled for the bundled Python only."""
    path = f"{dist.python_dir}/FS-UAE.zip"
    if path not in dist.files:
        return  # Reported by check_layout
    try:
        with zipfile.ZipFile(io.BytesIO(dist.read(path))) as zf:
            names = [name for name in zf.namelist() if not name.endswith("/")]
            pycs = {name: zf.read(name)[:4] for name in names if name.endswith(".pyc")}
    except zipfile.BadZipFile as e:
        dist.fail(f"{path} is not a valid zip file: {e}")
        return

    # Leftovers from the source tree (.mypy_cache, .ruff_cache, editor files, ...).
    hidden = {part for name in names if (part := hidden_part(name)) is not None}
    for name in sorted(hidden):
        dist.fail(f"{path} has hidden files or directories: {name}")

    modules = [name for name in names if name.endswith(".py")]
    if not modules:
        dist.fail(f"{path} has no Python modules")
    minor = dist.python_minor
    if minor is None:
        return  # Reported by check_python_version
    tag = f"cpython-3{minor}"
    not_compiled = []
    for module in modules:
        directory, filename = posixpath.split(module)
        pyc = posixpath.join(directory, "__pycache__", filename[:-3] + f".{tag}.pyc")
        if pyc not in pycs:
            not_compiled.append(module)
    fail_for_files(dist, f"{path}: not byte-compiled for Python 3.{minor} ({tag})", not_compiled)

    magic = pyc_magic(minor)
    if magic is None:
        dist.fail(f"The .pyc magic number for Python 3.{minor} is not known (add it to PYC_MAGIC)")
    other_tag = [name for name in pycs if not name.endswith(f".{tag}.pyc")]
    fail_for_files(dist, f"{path}: .pyc files not for the bundled Python 3.{minor}", other_tag)
    wrong_magic = [
        name
        for name, header in pycs.items()
        if name.endswith(f".{tag}.pyc") and magic is not None and header != magic
    ]
    fail_for_files(dist, f"{path}: wrong magic number for Python 3.{minor}", wrong_magic)


def fail_for_files(dist: Dist, message: str, files: list[str]) -> None:
    """One failure for a list of files, naming the first ten."""
    if not files:
        return
    files = sorted(files)
    names = ", ".join(files[:10])
    if len(files) > 10:
        names += f" and {len(files) - 10} more"
    dist.fail(f"{message}: {names}")


# Run in this order: check_python_version sets Dist.python_minor for the checks after it.
CHECKS: list[Callable[[Dist], None]] = [
    check_archive_name,
    check_layout,
    check_readme,
    check_portable_ini,
    check_program_version,
    check_build_header,
    check_python_version,
    check_shared_python_dir,
    check_fsuae_zip,
    check_purged_modules,
    check_required_modules,
    check_linux_libraries,
    check_macos_binaries,
    check_macos_minimum_version,
    check_macos_info_plist,
    check_macos_signatures,
    check_macos_dmg,
]


# --------------------------------------------------------------------------------------------------


def main() -> int:
    parser = argparse.ArgumentParser(description="Checks a dist archive or dist directory.")
    parser.add_argument(
        "--version",
        help="the version the archive name and the program must have (default: the archive "
        "name's, for the program)",
    )
    parser.add_argument(
        "--origin",
        help="the origin the program's build header must have (default: any)",
    )
    parser.add_argument(
        "--commit",
        help="the commit the program's build header must have (default: any)",
    )
    parser.add_argument(
        "--signed",
        action="store_true",
        help="the dist must be signed with a Developer ID certificate (macOS)",
    )
    parser.add_argument(
        "path", type=Path, help="dist archive (.tar.xz, .zip, .dmg on a Mac) or dist directory"
    )
    parser.add_argument("triplet", help="the triplet the dist is for, e.g. x86_64-linux-gnu")
    args = parser.parse_args()

    os_name = os_for_triplet(args.triplet)
    if os_name is None:
        parser.error(f"unknown triplet: {args.triplet}")
    dist = Dist(args.triplet, os_name)
    dist.version = args.version
    dist.origin = args.origin
    dist.commit = args.commit
    dist.signed = args.signed
    with contextlib.ExitStack() as stack:
        if args.path.is_dir():
            load_directory(args.path, dist)
        elif args.path.is_file():
            dist.archive_name = args.path.name
            if args.path.name.endswith(".dmg"):
                load_dmg(stack, args.path, dist)
            elif zipfile.is_zipfile(args.path):
                load_zip(stack.enter_context(zipfile.ZipFile(args.path)), dist)
            elif tarfile.is_tarfile(args.path):
                load_tar(stack.enter_context(tarfile.open(args.path)), dist)
            else:
                parser.error(f"not a .tar.xz or .zip archive: {args.path}")
        else:
            parser.error(f"no such archive or directory: {args.path}")

        for check in CHECKS:
            check(dist)

    if dist.failures:
        print(f"Dist check failed for {args.path}:")
        for failure in dist.failures:
            print(f"  {failure}")
        return 1
    print(f"Dist check passed for {args.path}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
