"""Mach-O reading for the dist check (check.py): what dyld and codesign read from a macOS binary.

Reads the load commands of a Mach-O file (thin or universal) without running anything or needing
macOS tools (otool, codesign), so a macOS dist can be checked on any system: the libraries it loads
(LC_LOAD_DYLIB and the like), its install name (LC_ID_DYLIB), its rpaths (LC_RPATH), its minimum
macOS version (LC_BUILD_VERSION or LC_VERSION_MIN_MACOSX) and its code signature (LC_CODE_SIGNATURE:
the CodeDirectory, with the hashes of the file's pages and of the app bundle's Info.plist and
CodeResources, and whether a certificate signed it).

Only the Python standard library is used, and the module works with Python 3.12.
"""

import hashlib
import struct

# The CPU types (cputype) of the architectures in the dists' triplets.
CPU_TYPES = {"x86_64": 0x01000007, "arm64": 0x0100000C}
CPU_NAMES = {number: cpu for cpu, number in CPU_TYPES.items()}

MH_MAGIC_64 = 0xFEEDFACF
MH_MAGIC = 0xFEEDFACE
FAT_MAGIC = 0xCAFEBABE
FAT_MAGIC_64 = 0xCAFEBABF

LC_REQ_DYLD = 0x80000000
LC_LOAD_DYLIB = 0xC
LC_ID_DYLIB = 0xD
LC_CODE_SIGNATURE = 0x1D
LC_LAZY_LOAD_DYLIB = 0x20
LC_VERSION_MIN_MACOSX = 0x24
LC_BUILD_VERSION = 0x32
LC_LOAD_WEAK_DYLIB = 0x18 | LC_REQ_DYLD
LC_RPATH = 0x1C | LC_REQ_DYLD
LC_REEXPORT_DYLIB = 0x1F | LC_REQ_DYLD
LC_LOAD_UPWARD_DYLIB = 0x23 | LC_REQ_DYLD

# The load commands for libraries the file loads, and whether a missing library is an error.
LOAD_COMMANDS = {
    LC_LOAD_DYLIB: True,
    LC_REEXPORT_DYLIB: True,
    LC_LAZY_LOAD_DYLIB: True,
    LC_LOAD_UPWARD_DYLIB: True,
    LC_LOAD_WEAK_DYLIB: False,
}


class MachOError(Exception):
    pass


def is_macho(data: bytes) -> bool:
    """Whether data starts like a Mach-O file (thin, 32 or 64 bits, or universal)."""
    if len(data) < 8:
        return False
    magic_le = struct.unpack_from("<I", data)[0]
    if magic_le in (MH_MAGIC_64, MH_MAGIC):
        return True
    magic_be, count = struct.unpack_from(">II", data)
    # Java class files start with 0xCAFEBABE too, followed by a version number (45 or more).
    return magic_be in (FAT_MAGIC, FAT_MAGIC_64) and 0 < count < 30


class MachO:
    """One architecture's slice of a Mach-O file."""

    def __init__(self, data: bytes, cputype: int, filetype: int) -> None:
        # The slice's bytes (the whole file for a thin file).
        self.data = data
        self.cputype = cputype
        self.filetype = filetype
        # (path, required) for each library the file loads.
        self.dylibs: list[tuple[str, bool]] = []
        self.install_name: str | None = None
        self.rpaths: list[str] = []
        # The minimum macOS version as a tuple, e.g. (10, 13), or None if the file has none.
        self.minimum_version: tuple[int, ...] | None = None
        # The code signature (LC_CODE_SIGNATURE: offset and size in the slice), if any.
        self.signature_range: tuple[int, int] | None = None

    @property
    def cpu(self) -> str:
        return CPU_NAMES.get(self.cputype, f"CPU type {self.cputype:#x}")


def read_macho(data: bytes) -> list[MachO]:
    """Reads the slices of a Mach-O file: one for a thin file and one per architecture for a
    universal file. Raises MachOError if data is not a valid Mach-O file."""
    try:
        magic_be = struct.unpack_from(">I", data)[0]
        if magic_be in (FAT_MAGIC, FAT_MAGIC_64):
            count = struct.unpack_from(">I", data, 4)[0]
            slices = []
            for i in range(count):
                if magic_be == FAT_MAGIC:
                    _, _, offset, size, _ = struct.unpack_from(">IIIII", data, 8 + i * 20)
                else:
                    _, _, offset, size, _, _ = struct.unpack_from(">IIQQII", data, 8 + i * 32)
                if offset + size > len(data):
                    raise MachOError("a slice is outside the file")
                slices.append(read_thin(data[offset : offset + size]))
            return slices
        return [read_thin(data)]
    except (struct.error, IndexError) as e:
        raise MachOError(f"truncated or malformed Mach-O file ({e})") from e


def read_thin(data: bytes) -> MachO:
    magic = struct.unpack_from("<I", data)[0]
    if magic == MH_MAGIC_64:
        header_size = 32
    elif magic == MH_MAGIC:
        header_size = 28
    else:
        raise MachOError("not a Mach-O file")
    cputype, _, filetype, ncmds, sizeofcmds = struct.unpack_from("<iIIII", data, 4)
    macho = MachO(data, cputype & 0xFFFFFFFF, filetype)
    offset = header_size
    end = header_size + sizeofcmds
    if end > len(data):
        raise MachOError("the load commands are outside the file")
    for _ in range(ncmds):
        cmd, cmdsize = struct.unpack_from("<II", data, offset)
        if cmdsize < 8 or offset + cmdsize > end:
            raise MachOError("malformed load command")
        command = data[offset : offset + cmdsize]
        if cmd in LOAD_COMMANDS:
            macho.dylibs.append((command_string(command, 8), LOAD_COMMANDS[cmd]))
        elif cmd == LC_ID_DYLIB:
            macho.install_name = command_string(command, 8)
        elif cmd == LC_RPATH:
            macho.rpaths.append(command_string(command, 8))
        elif cmd == LC_BUILD_VERSION:
            macho.minimum_version = unpack_version(struct.unpack_from("<I", command, 12)[0])
        elif cmd == LC_VERSION_MIN_MACOSX:
            macho.minimum_version = unpack_version(struct.unpack_from("<I", command, 8)[0])
        elif cmd == LC_CODE_SIGNATURE:
            dataoff, datasize = struct.unpack_from("<II", command, 8)
            if dataoff + datasize > len(data):
                raise MachOError("the code signature is outside the file")
            macho.signature_range = (dataoff, datasize)
        offset += cmdsize
    return macho


def command_string(command: bytes, offset_field: int) -> str:
    """The string in a load command, at the offset given in the field at offset_field."""
    offset = struct.unpack_from("<I", command, offset_field)[0]
    if offset >= len(command):
        raise MachOError("string outside its load command")
    end = command.find(b"\0", offset)
    return command[offset : end if end >= 0 else len(command)].decode("utf-8", "replace")


def unpack_version(value: int) -> tuple[int, ...]:
    """A version tuple from its Mach-O encoding (xxxx.yy.zz in nibbles), without a trailing .0
    patch level: 0x000A0D00 -> (10, 13)."""
    major, minor, patch = value >> 16, (value >> 8) & 0xFF, value & 0xFF
    return (major, minor, patch) if patch else (major, minor)


def format_version(version: tuple[int, ...]) -> str:
    return ".".join(str(part) for part in version)


# --------------------------------------------------------------------------------------------------
# Code signatures (big-endian blobs, see Apple's cs_blobs.h)

CSMAGIC_EMBEDDED_SIGNATURE = 0xFADE0CC0
CSMAGIC_CODEDIRECTORY = 0xFADE0C02
CSMAGIC_BLOBWRAPPER = 0xFADE0B01
CSMAGIC_EMBEDDED_ENTITLEMENTS = 0xFADE7171
# A blob's header: its magic and length (then its data, such as the entitlements' property list).
BLOB_HEADER_SIZE = 8
# The slots in the embedded signature's index (the SuperBlob).
CSSLOT_CODEDIRECTORY = 0
CSSLOT_ENTITLEMENTS = 5
CSSLOT_ALTERNATE_CODEDIRECTORIES = range(0x1000, 0x1005)
CSSLOT_SIGNATURESLOT = 0x10000
CS_ADHOC = 0x2
CS_RUNTIME = 0x10000

# The special slots (negative indexes in a CodeDirectory, with the hashes of what the signature
# seals besides the code): an app bundle's Info.plist and resources, and the entitlements blob
# (in the SuperBlob's slot CSSLOT_ENTITLEMENTS, which has the same number).
SLOT_INFO_PLIST = 1
SLOT_RESOURCES = 3
SLOT_ENTITLEMENTS = 5

HASH_FUNCTIONS = {1: "sha1", 2: "sha256", 3: "sha256", 4: "sha384"}


class CodeDirectory:
    """A CodeDirectory: the hashes of the signed file's pages and of the special slots."""

    def __init__(self, blob: bytes) -> None:
        (
            magic,
            length,
            version,
            self.flags,
            hash_offset,
            _,
            n_special,
            n_code,
            self.code_limit,
            self.hash_size,
            hash_type,
            _,
            page_size_log2,
        ) = struct.unpack_from(">IIIIIIIIIBBBB", blob)
        if magic != CSMAGIC_CODEDIRECTORY or length > len(blob):
            raise MachOError("malformed CodeDirectory")
        if hash_type not in HASH_FUNCTIONS:
            raise MachOError(f"unknown hash type {hash_type} in the CodeDirectory")
        self.blob = blob[:length]
        self.hash_function = HASH_FUNCTIONS[hash_type]
        self.page_size = 1 << page_size_log2 if page_size_log2 else 0
        self.team_id: str | None = None
        if version >= 0x20200:
            team_offset = struct.unpack_from(">I", blob, 48)[0]
            if team_offset:
                end = blob.index(b"\0", team_offset)
                self.team_id = blob[team_offset:end].decode("utf-8", "replace")
        if (
            hash_offset + n_code * self.hash_size > length
            or n_special * self.hash_size > hash_offset
        ):
            raise MachOError("the CodeDirectory's hashes are outside it")
        self.code_hashes = [
            blob[hash_offset + i * self.hash_size : hash_offset + (i + 1) * self.hash_size]
            for i in range(n_code)
        ]
        # Slot number (1 for -1, ...) -> hash, for the special slots that are set (not all zeros).
        self.special_hashes = {}
        for slot in range(1, n_special + 1):
            value = blob[
                hash_offset - slot * self.hash_size : hash_offset - (slot - 1) * self.hash_size
            ]
            if any(value):
                self.special_hashes[slot] = value

    def hash(self, data: bytes) -> bytes:
        return hashlib.new(self.hash_function, data).digest()[: self.hash_size]

    @property
    def cdhash(self) -> bytes:
        """The hash that identifies the signed code (in CodeResources and code requirements)."""
        return hashlib.new(self.hash_function, self.blob).digest()[:20]

    def changed_pages(self, data: bytes) -> list[int]:
        """The pages of data (the slice) that do not have the hashes they were signed with."""
        page_size = self.page_size or self.code_limit
        if self.code_limit > len(data):
            return list(range(len(self.code_hashes)))
        changed = []
        for i, expected in enumerate(self.code_hashes):
            start = i * page_size
            page = data[start : min(start + page_size, self.code_limit)]
            if self.hash(page) != expected:
                changed.append(i)
        return changed


class CodeSignature:
    """A Mach-O slice's embedded code signature."""

    def __init__(
        self,
        code_directories: list[CodeDirectory],
        cms_size: int,
        entitlements_blob: bytes | None = None,
    ) -> None:
        # The primary CodeDirectory first, then the alternate ones (other hash types).
        self.code_directories = code_directories
        # The size of the CMS signature: 0 when signed ad hoc (no certificate).
        self.cms_size = cms_size
        # The entitlements blob (its header, then the XML property list), if signed with any.
        self.entitlements_blob = entitlements_blob

    @property
    def entitlements(self) -> bytes | None:
        """The entitlements (an XML property list), or None if signed without any."""
        if self.entitlements_blob is None:
            return None
        return self.entitlements_blob[BLOB_HEADER_SIZE:]

    @property
    def code_directory(self) -> CodeDirectory:
        return self.code_directories[0]

    @property
    def ad_hoc(self) -> bool:
        return bool(self.code_directory.flags & CS_ADHOC) or self.cms_size == 0

    @property
    def runtime(self) -> bool:
        """Whether the code is signed with the hardened runtime (which notarization needs)."""
        return bool(self.code_directory.flags & CS_RUNTIME)

    @property
    def team_id(self) -> str | None:
        return self.code_directory.team_id


def read_signature(macho: MachO) -> CodeSignature | None:
    """The code signature of a slice, or None if it has none. Raises MachOError if it is
    malformed."""
    if macho.signature_range is None:
        return None
    offset, size = macho.signature_range
    blob = macho.data[offset : offset + size]
    try:
        magic, _, count = struct.unpack_from(">III", blob)
        if magic != CSMAGIC_EMBEDDED_SIGNATURE:
            raise MachOError("the code signature is not an embedded signature")
        code_directories: list[CodeDirectory] = []
        cms_size = 0
        entitlements_blob = None
        for i in range(count):
            slot, blob_offset = struct.unpack_from(">II", blob, 12 + i * 8)
            if slot == CSSLOT_CODEDIRECTORY:
                code_directories.insert(0, CodeDirectory(blob[blob_offset:]))
            elif slot in CSSLOT_ALTERNATE_CODEDIRECTORIES:
                code_directories.append(CodeDirectory(blob[blob_offset:]))
            elif slot == CSSLOT_ENTITLEMENTS:
                blob_magic, length = struct.unpack_from(">II", blob, blob_offset)
                if blob_magic == CSMAGIC_EMBEDDED_ENTITLEMENTS:
                    entitlements_blob = blob[blob_offset : blob_offset + length]
            elif slot == CSSLOT_SIGNATURESLOT:
                wrapper_magic, length = struct.unpack_from(">II", blob, blob_offset)
                if wrapper_magic == CSMAGIC_BLOBWRAPPER:
                    cms_size = length - 8
    except (struct.error, ValueError) as e:
        raise MachOError(f"malformed code signature ({e})") from e
    if not code_directories:
        raise MachOError("the code signature has no CodeDirectory")
    return CodeSignature(code_directories, cms_size, entitlements_blob)
