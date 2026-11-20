"""WHDLoad run preparation.

Turns a WHDLoad archive into a run directory with a bootable hard drive (`DH0`) and the `.uae`
configuration for an expanded A1200 that boots it straight into WHDLoad.

The single entry point is `prepare_whdload_run`. This module has no GUI, emulator or global-state
dependencies (only the bundled `lhafile` package, which needs `lzhlib`), so it can be used and
tested under a plain Python interpreter (the tests are in `od-fs/tests/whdload`).
"""

import hashlib
import logging
import os
import shutil
from collections.abc import Iterable
from dataclasses import dataclass
from typing import BinaryIO, Protocol

from lhafile import LhaFile

from fsuae.whdload.amigaicon import AmigaIconError, read_tool_types

logger = logging.getLogger(__name__)

# Bundled files, relative to the resources directory.
WHDLOAD_RESOURCE = ("WHDLoad", "18.5")
UAE_CONFIGURATION_RESOURCE = ("UAE", "uae-configuration")
# The unmodified SKick archive. Its license only allows redistribution in a totally unchanged
# state, so the relocation tables are extracted from it at run time (see Resources/SKick).
SKICK_RESOURCE = ("SKick", "skick346.lha")

# Kickstart relocation tables for WHDLoad: file name in the SKick archive's `Kickstarts`
# directory (and in `Devs/Kickstarts` on the hard drive) => SHA-1.
RELOCATION_TABLES = {
    "kick33180.A500.RTB": "1d1c557f4a0f5ea88aeb96d68b09f41990340f70",
    "kick34005.A500.RTB": "209c109855f94c935439b60950d049527d2f2484",
    "kick40068.A1200.RTB": "1ad1b55e7226bd5cd66def8370a69f19244da796",
    "kick40068.A4000.RTB": "973b42dcaf8d6cb111484b3c4d3b719b15f6792d",
}

# Volume name of the hard drive the run boots from.
VOLUME_NAME = "WHDLoad"

# lhafile's compress type for directory members.
LHA_DIRECTORY = b"-lhd-"

# Every file on the hard drive gets a `.uaem` sidecar (FS-UAE's directory filesystem metadata)
# with these protection bits and this date, as FS-UAE Launcher wrote them. The archive's real
# protection bits and dates are not used.
UAEM_SUFFIX = ".uaem"
UAEM_PROTECTION = "----rwed"
UAEM_DATE = "2000-01-01 00:00:00.00"

# Characters FS-UAE's directory filesystem percent-escapes in host file names (`evilchars` in
# od-fs/fsdb_host.cpp), in addition to control characters.
HOST_UNSAFE_CHARS = '%\\*?"/|<>'


@dataclass(frozen=True)
class Kickstart:
    """A Kickstart ROM image the run needs, identified by its SHA-1."""

    name: str
    sha1: str


KICKSTART_120_A500 = Kickstart(
    "Kickstart 1.2 (33.180) A500", "11f9e62cf299f72184835b7b2a70a16333fc0d88"
)
KICKSTART_130_A500 = Kickstart(
    "Kickstart 1.3 (34.005) A500", "891e9a547772fe0c6c19b610baf8bc4ea7fcb785"
)
KICKSTART_310_A1200 = Kickstart(
    "Kickstart 3.1 (40.068) A1200", "e21545723fe8374e91342617604f1b3d703094f1"
)
KICKSTART_310_A4000 = Kickstart(
    "Kickstart 3.1 (40.068) A4000", "5fe04842d04a489720f0f4bb0e46948199406f49"
)

# Kickstart copies for WHDLoad, with their file names in `Devs/Kickstarts`. Each is copied when
# the ROM list has it; WHDLoad itself tells the user when a slave needs one that is missing.
WHDLOAD_KICKSTARTS = [
    (KICKSTART_120_A500, "kick33180.A500"),
    (KICKSTART_130_A500, "kick34005.A500"),
    (KICKSTART_310_A1200, "kick40068.A1200"),
    (KICKSTART_310_A4000, "kick40068.A4000"),
]

# FS-UAE Launcher's default `S/WHDLoad.prefs`: every option documented and commented out.
# Written with CRLF line endings, as the Launcher does.
DEFAULT_WHDLOAD_PREFS = """
; wait for button pressed (slave must support this)
;ButtonWait

; disable cache-ability of Chip-Memory
;ChipNoCache

; path for core dump files
;CoreDumpPath=T:

; raw key code to quit with core dump (debug)
;DebugKey=$5b

; command to execute on WHDLoad startup
;ExecuteStartup=rx offline.rexx

; command to execute on WHDLoad exit
;ExecuteCleanup=rx online.rexx

; selects expert mode
;Expert

; raw key code to enter HrtMon/TK
;FreezeKey=$5d

; use MMU (for 68030)
;MMU

; ignore unwanted auto-vector interrupts
;NoAutoVec

; disable audio filter
;NoFilter

; do not flush memory
;NoFlushMem

; do not allocate memory reverse
;NoMemReverse

; disable the disk write cache
;NoWriteCache

;QuitKey=$45

; wait after reading from disk (1/50 seconds)
;ReadDelay=150

; raw key code to restart
;RestartKey=$5c

; command for Show Regs
;ShowRegs=SYS:Utilities/MuchMore W WL=80 WT=80 WW=582 WH=700

; time to display splash window (1/50 seconds)
;SplashDelay=0

; wait after saving something to disk (1/50 seconds)
;WriteDelay=150
"""


class KickstartROM(Protocol):
    """A Kickstart ROM found by the ROM scan (such as `fsuae.roms.rom.ROM`)."""

    path: str
    crc32: str
    sha1: str


@dataclass(frozen=True)
class WHDLoadRun:
    """The result of preparing a run."""

    # The populated hard drive directory (`DH0` in the run directory).
    hard_drive_dir: str
    # The generated `.uae` configuration (raw UAE options, one `key=value` per line).
    config: str
    # The chosen slave, as an Amiga path relative to the root of the hard drive.
    slave: str


class WHDLoadRunError(Exception):
    """Base class for errors that prevent a WHDLoad archive from being started.

    Raised as is for problems with FS-UAE's own support files (such as a damaged SKick archive);
    the message says what is wrong.
    """


class NotAWHDLoadArchiveError(WHDLoadRunError):
    """The file is not a WHDLoad archive (or cannot be read as an LHA archive at all).

    `reason` completes "<archive> cannot be started because ...".
    """

    def __init__(self, archive_path: str, reason: str) -> None:
        super().__init__(f"{archive_path} is not a WHDLoad archive: {reason}")
        self.archive_path = archive_path
        self.reason = reason


class KickstartMissingError(WHDLoadRunError):
    """The Kickstart ROM the run needs (`kickstart`) is not in the ROM list."""

    def __init__(self, kickstart: Kickstart) -> None:
        super().__init__(f"{kickstart.name} is needed but was not found (SHA-1 {kickstart.sha1})")
        self.kickstart = kickstart


class DamagedArchiveError(WHDLoadRunError):
    """A member of the WHDLoad archive cannot be read (for example a CRC error)."""

    def __init__(self, archive_path: str, member: str, reason: str) -> None:
        super().__init__(f"{archive_path} is damaged: cannot read {member} ({reason})")
        self.archive_path = archive_path
        self.member = member
        self.reason = reason


def prepare_whdload_run(
    archive_path: str,
    run_dir: str,
    roms: Iterable[KickstartROM],
    resources_dir: str,
) -> WHDLoadRun:
    """Prepare a run of a WHDLoad archive.

    `run_dir` must be an existing directory without a `DH0` (a new run directory from
    `create_run_dir` has only its lock file); the hard drive is created as `DH0` inside it.
    `roms` are the Kickstart ROMs known from the ROM scan, and `resources_dir` is FS-UAE's
    resources directory (with the bundled WHDLoad, `uae-configuration` and SKick archive).

    Raises `NotAWHDLoadArchiveError` (no slave, a root-level `S/Startup-Sequence`, or not an LHA
    archive) or `KickstartMissingError` before anything is written to `run_dir`. A member that
    cannot be read raises `DamagedArchiveError`, and a damaged SKick archive a plain
    `WHDLoadRunError`. All are `WHDLoadRunError`s. On any error, nothing written to `run_dir` is
    left behind.
    """
    logger.info("Preparing WHDLoad run of %s in %s", archive_path, run_dir)
    roms = list(roms)
    with open(archive_path, "rb") as f:
        archive = _Archive(archive_path, f)
        _check_whdload_archive(archive)
        choice = _choose_slave(archive)
        kickstart_path = _find_kickstart(roms, KICKSTART_310_A1200)
        relocation_tables = _read_relocation_tables(os.path.join(resources_dir, *SKICK_RESOURCE))
        kickstart_copies = _find_kickstart_copies(roms)

        drive = _HardDrive(os.path.join(run_dir, "DH0"))
        try:
            for member in archive.members:
                if member.is_dir:
                    drive.make_dir(member.path)
                else:
                    drive.write_file(member.path, archive.read(member), member.comment)

            drive.copy_file("C/WHDLoad", os.path.join(resources_dir, *WHDLOAD_RESOURCE))
            drive.copy_file(
                "C/uae-configuration", os.path.join(resources_dir, *UAE_CONFIGURATION_RESOURCE)
            )
            for name, data in relocation_tables.items():
                drive.write_file(f"Devs/Kickstarts/{name}", data)
            for name, rom_path in kickstart_copies.items():
                drive.copy_file(f"Devs/Kickstarts/{name}", rom_path)
            drive.write_file("S/WHDLoad.prefs", _whdload_prefs())
            startup_sequence = _startup_sequence(
                drive.amiga_dir(choice.directory), choice.arguments
            )
            drive.write_file("S/Startup-Sequence", startup_sequence)
        except BaseException:
            # Leave nothing half-written behind in the run directory.
            shutil.rmtree(drive.root, ignore_errors=True)
            raise

    config = _config(kickstart_path, drive.root)
    slave = drive.amiga_path(choice.slave)
    return WHDLoadRun(hard_drive_dir=drive.root, config=config, slave=slave)


# --------------------------------------------------------------------------------------------------
# Archive


@dataclass(frozen=True)
class _Member:
    # Amiga path with "/" separators, relative to the archive root.
    path: str
    is_dir: bool
    lha_name: str
    # The Amiga file comment ("" if none).
    comment: str


class _Archive:
    def __init__(self, archive_path: str, f: BinaryIO) -> None:
        try:
            self._lha = LhaFile(f)
        except Exception as e:
            raise NotAWHDLoadArchiveError(
                archive_path, f"it cannot be read as an LHA archive ({e})"
            ) from e
        self.members: list[_Member] = []
        for info in self._lha.infolist():
            path = _member_path(info.filename)
            if path is None:
                logger.warning("Skipping archive member with unsafe path %r", info.filename)
                continue
            is_dir = info.compress_type == LHA_DIRECTORY
            self.members.append(_Member(path, is_dir, info.filename, _comment(info.comment)))
        self.archive_path = archive_path

    def read(self, member: _Member) -> bytes:
        try:
            return self._lha.read(member.lha_name)
        except Exception as e:
            raise DamagedArchiveError(self.archive_path, member.path, str(e) or repr(e)) from e


def _comment(comment: str | bytes | None) -> str:
    """The member's Amiga file comment as a string.

    lhafile gives the comment Amiga LhA stores after the file name (level 0/1 headers) as a
    string, and the one from a comment extended header (level 1/2) as bytes.
    """
    if comment is None:
        return ""
    if isinstance(comment, bytes):
        return comment.decode("ISO-8859-1")
    return comment


def _member_path(name: str) -> str | None:
    """Amiga path for an archive member name, or None if it would escape the hard drive.

    lhafile joins directory names with the host's separator, and level 0 headers use
    backslashes, so both are treated as separators.
    """
    name = name.replace("\\", "/").replace(os.sep, "/")
    parts = [part for part in name.split("/") if part not in ("", ".")]
    if not parts or ".." in parts:
        return None
    return "/".join(parts)


@dataclass(frozen=True)
class _SlaveChoice:
    # The slave, as an archive path.
    slave: str
    # The archive directory WHDLoad is started from.
    directory: str
    # The WHDLoad arguments, starting with the slave (relative to `directory`).
    arguments: list[str]


@dataclass(frozen=True)
class _SlaveIcon:
    # The icon, as an archive path.
    path: str
    # The slave its SLAVE= tool type names, as an archive path.
    slave: str
    # The WHDLoad arguments from its tool types, starting with the slave.
    arguments: list[str]


def _check_whdload_archive(archive: _Archive) -> None:
    """Raise `NotAWHDLoadArchiveError` unless the archive is a WHDLoad archive.

    A WHDLoad archive has at least one slave (any depth, any case) and no root-level
    `S/Startup-Sequence` (any case); an archive with one is a bootable hard drive archive.
    """
    files = [m.path.lower() for m in archive.members if not m.is_dir]
    if not any(path.endswith(".slave") for path in files):
        raise NotAWHDLoadArchiveError(archive.archive_path, "it does not contain a slave")
    if "s/startup-sequence" in files:
        raise NotAWHDLoadArchiveError(
            archive.archive_path,
            "it has a root-level S/Startup-Sequence (a bootable hard drive archive)",
        )


def _choose_slave(archive: _Archive) -> _SlaveChoice:
    """The slave to run, and the WHDLoad arguments to run it with.

    If exactly one icon has a SLAVE= tool type naming a slave in the archive, that slave, with the
    icon's tool types as arguments. Otherwise the slave with the fewest path components, then the
    first in case-insensitive alphabetical order, with `<slave> PRELOAD`.
    """
    slaves = [m.path for m in archive.members if not m.is_dir and m.path.lower().endswith(".slave")]
    slaves.sort(key=lambda path: (path.count("/"), path.lower()))
    icons = _slave_icons(archive, {path.lower(): path for path in slaves})

    if len(icons) == 1:
        icon = icons[0]
        choice = _SlaveChoice(icon.slave, _parent(icon.path), icon.arguments)
        rule = f"icon {icon.path}"
    else:
        slave = slaves[0]
        choice = _SlaveChoice(slave, _parent(slave), [_quote(_name(slave)), "PRELOAD"])
        if len(slaves) == 1:
            rule = "the only slave in the archive"
        elif slaves[1].count("/") > slave.count("/"):
            rule = "fewest path components"
        else:
            rule = "fewest path components, then alphabetical order"
        if icons:
            rule += "; several icons name a slave: " + ", ".join(icon.path for icon in icons)
        else:
            rule += "; no icon names a slave"
    logger.info("WHDLoad slave: %s (%s; slaves found: %s)", choice.slave, rule, ", ".join(slaves))
    logger.info("WHDLoad arguments: %s", " ".join(choice.arguments))
    return choice


def _slave_icons(archive: _Archive, slaves: dict[str, str]) -> list[_SlaveIcon]:
    """The icons in the archive with a SLAVE= tool type naming one of `slaves`.

    `slaves` maps lowercased archive paths to archive paths. Icons that cannot be read are logged
    and ignored, as are icons whose SLAVE= tool type names no slave in the archive.
    """
    icons = []
    for member in archive.members:
        if member.is_dir or not member.path.lower().endswith(".info"):
            continue
        try:
            arguments = _icon_arguments(read_tool_types(archive.read(member)))
        except AmigaIconError as e:
            logger.warning("Ignoring icon %s: %s", member.path, e)
            continue
        if arguments is None:
            continue
        slave_arg, other_args = arguments
        slave_path = _member_path(f"{_parent(member.path)}/{slave_arg}")
        slave = slaves.get(slave_path.lower()) if slave_path else None
        if slave is None:
            logger.info("Icon %s: SLAVE=%s names no slave in the archive", member.path, slave_arg)
            continue
        logger.info("Icon %s: SLAVE=%s", member.path, slave_arg)
        icons.append(_SlaveIcon(member.path, slave, [_quote(slave_arg), *other_args]))
    return icons


def _icon_arguments(tool_types: list[str]) -> tuple[str, list[str]] | None:
    """The WHDLoad arguments in an icon's tool types: (SLAVE= value, other arguments).

    None if there is no SLAVE= tool type. Follows FS-UAE Launcher's cleanup: tool types are cut
    at `;` comments, disabled tool types (in parentheses) are dropped, and the SLAVE= prefix is
    stripped. Also like the Launcher, the tool types holding NewIcons image data (from the
    "*** DON'T EDIT THE FOLLOWING LINES!! ***" marker on) are not arguments.
    """
    slave = None
    arguments = []
    for tool_type in tool_types:
        if tool_type.lstrip().startswith("***"):
            break
        tool_type = tool_type.split(";", 1)[0].strip()
        if not tool_type or tool_type.startswith("("):
            continue
        if any(ord(c) < 0x20 or 0x7F <= ord(c) < 0xA0 for c in tool_type):
            raise AmigaIconError(f"tool type with binary data: {tool_type!r}")
        if tool_type.lower().startswith("slave="):
            if slave is None:
                slave = tool_type[6:].strip().strip('"')
        else:
            arguments.append(_quote_tool_type(tool_type))
    if not slave:
        return None
    return slave, arguments


def _parent(path: str) -> str:
    return path.rpartition("/")[0]


def _name(path: str) -> str:
    return path.rpartition("/")[2]


# --------------------------------------------------------------------------------------------------
# Kickstarts


def _find_rom(roms: list[KickstartROM], kickstart: Kickstart) -> str | None:
    for rom in roms:
        if rom.sha1.lower() == kickstart.sha1:
            return rom.path
    return None


def _find_kickstart(roms: list[KickstartROM], kickstart: Kickstart) -> str:
    path = _find_rom(roms, kickstart)
    if path is None:
        raise KickstartMissingError(kickstart)
    logger.info("%s: %s", kickstart.name, path)
    return path


def _find_kickstart_copies(roms: list[KickstartROM]) -> dict[str, str]:
    """The Kickstart copies for WHDLoad found in the ROM list: name => ROM path."""
    copies = {}
    for kickstart, name in WHDLOAD_KICKSTARTS:
        path = _find_rom(roms, kickstart)
        if path is None:
            logger.info("Devs/Kickstarts/%s: %s not found, skipped", name, kickstart.name)
        else:
            logger.info("Devs/Kickstarts/%s: %s", name, path)
            copies[name] = path
    return copies


def _read_relocation_tables(skick_path: str) -> dict[str, bytes]:
    """Read the relocation tables WHDLoad needs from the SKick archive: name => contents.

    The archive is only read; it stays unmodified in the resources directory.
    """
    try:
        with open(skick_path, "rb") as f:
            lha = LhaFile(f)
            members = {}
            for info in lha.infolist():
                path = _member_path(info.filename)
                if path is not None:
                    members[path.lower()] = info.filename
            tables = {}
            for name, sha1 in RELOCATION_TABLES.items():
                member = members.get(f"kickstarts/{name}".lower())
                data = lha.read(member) if member is not None else None
                if data is None or hashlib.sha1(data).hexdigest() != sha1:
                    raise WHDLoadRunError(
                        f"FS-UAE's SKick archive {skick_path} is damaged: "
                        f"no valid Kickstarts/{name}"
                    )
                tables[name] = data
    except WHDLoadRunError:
        raise
    except Exception as e:
        raise WHDLoadRunError(f"Cannot read FS-UAE's SKick archive {skick_path}: {e}") from e
    return tables


# --------------------------------------------------------------------------------------------------
# Hard drive


class _HardDrive:
    """A directory hard drive being built on the host, as FS-UAE's directory filesystem sees it.

    Amiga paths use "/" and are case-insensitive: a directory is created with the spelling it is
    first seen with, and later paths that differ only in case go into the same directory. A file
    written again with a path that differs only in case replaces the earlier one.

    Each path component is stored under its host file name (`_host_file_name`), and every file
    gets a `.uaem` sidecar with its protection bits, date and comment.
    """

    def __init__(self, root: str) -> None:
        self.root = root
        os.makedirs(root)
        # Lowercased Amiga directory path => Amiga directory path as created.
        self._dirs: dict[str, str] = {"": ""}
        # Lowercased Amiga file path => Amiga file path as written.
        self._files: dict[str, str] = {}

    def amiga_dir(self, path: str) -> str:
        """The directory path as it exists on the drive (with the spelling used there)."""
        return self._dirs.get(path.lower(), path)

    def amiga_path(self, path: str) -> str:
        """The path as it exists on the drive (with the directory spelling used there)."""
        parent, _, name = path.rpartition("/")
        parent = self.amiga_dir(parent)
        return f"{parent}/{name}" if parent else name

    def make_dir(self, path: str) -> str:
        key = path.lower()
        if key in self._dirs:
            return self._dirs[key]
        parent, _, name = path.rpartition("/")
        parent = self.make_dir(parent)
        created = f"{parent}/{name}" if parent else name
        os.makedirs(self._host_path(created), exist_ok=True)
        self._dirs[key] = created
        return created

    def write_file(self, path: str, data: bytes, comment: str = "") -> None:
        host_path = self._prepare_file(path)
        with open(host_path, "wb") as f:
            f.write(data)
        _write_uaem(host_path, comment)

    def copy_file(self, path: str, source: str, comment: str = "") -> None:
        host_path = self._prepare_file(path)
        shutil.copyfile(source, host_path)
        _write_uaem(host_path, comment)

    def _prepare_file(self, path: str) -> str:
        """The host path to write the file to, with its directory created."""
        parent, _, name = path.rpartition("/")
        parent = self.make_dir(parent) if parent else ""
        path = f"{parent}/{name}" if parent else name
        previous = self._files.get(path.lower())
        if previous is not None and previous != path:
            # Same file for the Amiga, but another host file: remove the earlier one.
            previous_host_path = self._host_path(previous)
            os.remove(previous_host_path)
            os.remove(previous_host_path + UAEM_SUFFIX)
        self._files[path.lower()] = path
        return self._host_path(path)

    def _host_path(self, amiga_path: str) -> str:
        return os.path.join(self.root, *(_host_file_name(part) for part in amiga_path.split("/")))


def _host_file_name(amiga_name: str) -> str:
    """The host file name for an Amiga file name, as FS-UAE's directory filesystem stores it.

    A port of `aname_to_nname` in od-fs/fsdb_host.cpp (which FS-UAE Launcher's
    `amiga_filename_to_host_filename` was converted from), so that the emulated Amiga sees the
    original name: characters that are not safe on the host (control characters and
    HOST_UNSAFE_CHARS, including `%` itself), a trailing dot or space, the third character of a
    reserved DOS device name (`AUX`, `CON`, `PRN`, `NUL`, `LPT<digit>` and `COM<digit>`, alone or
    followed by a dot and anything), and the last character of a name ending in `.uaem` (any
    case) are written as `%xx` (lowercase hex). The escaping is the same on every host OS, as in
    the core. The name is ISO-8859-1 text; Python writes it to the host as Unicode (UTF-8 on
    Linux), as the core does.
    """
    length = len(amiga_name)
    escape = set()
    upper = amiga_name[:3].upper()
    device_length = 0
    if upper in ("AUX", "CON", "PRN", "NUL"):
        device_length = 3
    elif upper in ("LPT", "COM") and length > 3 and "0" <= amiga_name[3] <= "9":
        device_length = 4
    if device_length and (length == device_length or amiga_name[device_length] == "."):
        escape.add(2)
    if amiga_name.endswith((".", " ")):
        escape.add(length - 1)
    if amiga_name.lower().endswith(UAEM_SUFFIX):
        escape.add(length - 1)
    return "".join(
        f"%{ord(c):02x}" if i in escape or ord(c) < 32 or c in HOST_UNSAFE_CHARS else c
        for i, c in enumerate(amiga_name)
    )


def _write_uaem(host_path: str, comment: str) -> None:
    """Write the `.uaem` sidecar for a file, in the format od-fs/fsdb_host.cpp reads.

    `<protection bits> <date> <comment>`, then a newline. Every character of the comment is
    percent-escaped, as FS-UAE Launcher did (`encode_file_comment`), but always with two hex
    digits: the core decodes `%` followed by exactly two hex digits.
    """
    encoded = "".join(f"%{byte:02x}" for byte in comment.encode("ISO-8859-1", "replace"))
    with open(host_path + UAEM_SUFFIX, "w", encoding="ascii", newline="\n") as f:
        f.write(f"{UAEM_PROTECTION} {UAEM_DATE} {encoded}\n")


# --------------------------------------------------------------------------------------------------
# Startup-Sequence and configuration


def _quote(arg: str) -> str:
    return _quoted(arg) if " " in arg else arg


def _quoted(arg: str) -> str:
    """An AmigaDOS quoted argument: `*` is the escape character inside quotes."""
    return '"' + arg.replace("*", "**").replace('"', '*"') + '"'


def _quote_tool_type(tool_type: str) -> str:
    """A tool type as a command line argument: `KEY=value with spaces` => `KEY="value with spaces"`.

    Tool types that already have quotes are passed on as they are.
    """
    if " " not in tool_type or '"' in tool_type:
        return tool_type
    key, sep, value = tool_type.partition("=")
    return f'{key}="{value}"' if sep else f'"{tool_type}"'


def _whdload_prefs() -> bytes:
    return DEFAULT_WHDLOAD_PREFS.replace("\n", "\r\n").encode("ISO-8859-1")


def _startup_sequence(directory: str, arguments: list[str]) -> bytes:
    """Run WHDLoad from the slave's directory, then quit FS-UAE when WHDLoad returns.

    `uae-configuration SPC_QUIT 1` asks the emulator to quit, so quitting the game with
    WHDLoad's quit key exits FS-UAE.
    """
    lines = [
        "cd " + _quoted(f"SYS:{directory}"),
        "WHDLoad " + " ".join(arguments),
        "uae-configuration SPC_QUIT 1",
    ]
    return "".join(line + "\n" for line in lines).encode("ISO-8859-1")


def _config(kickstart_path: str, hard_drive_dir: str) -> str:
    """Raw UAE options for an expanded A1200 booting from the hard drive.

    Game ports are left out, so FS-UAE's defaults apply. Order matters: `cpu_model` resets
    `fpu_model`.
    """
    options = [
        ("chipset", "aga"),
        ("chipset_compatible", "A1200"),
        ("cpu_model", "68020"),
        ("fpu_model", "0"),
        ("cpu_24bit_addressing", "false"),
        ("cachesize", "0"),
        ("cpu_compatible", "true"),
        ("cycle_exact", "false"),
        ("chipmem_size", "4"),
        ("bogomem_size", "0"),
        ("fastmem_size", "0"),
        ("z3mem_size", "32"),
        ("kickstart_rom_file", kickstart_path),
        ("filesystem2", f"rw,DH0:{VOLUME_NAME}:{hard_drive_dir},0"),
    ]
    return "".join(f"{key}={value}\n" for key, value in options)
