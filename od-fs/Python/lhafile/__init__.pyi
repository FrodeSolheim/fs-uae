"""Typing stubs for the bundled `lhafile` package (reads LHA/LZH archives, using `lzhlib`).

Covers the public API in `lhafile.lhafile`, which `__init__.py` re-exports. The module itself is
untyped third-party code; see docs/adr/0003-fully-typed-python.md.
"""

import datetime
from collections.abc import Callable
from typing import IO, Any

class BadLhafile(Exception): ...

class LhaInfo:
    orig_filename: str | None
    # The member's name, with directories joined by the host's path separator.
    filename: str
    directory: str | None
    date_time: datetime.datetime
    # The compression method, for example b"-lh5-" (b"-lhd-" for directories).
    compress_type: bytes
    # The Amiga file comment: a str from the name field (level 0/1 headers), bytes from a comment
    # extended header, or None.
    comment: str | bytes | None
    extra: Any
    create_system: int | None
    create_version: int | None
    extract_version: int | None
    reserved: int | None
    flag_bits: int | None
    volume: int | None
    internal_attr: int | None
    external_attr: int | None
    header_offset: int
    file_offset: int
    CRC: int
    compress_size: int
    file_size: int

class LhaFile:
    SUPPORTED_COMPRESS_TYPE: tuple[bytes, ...]
    filename: str | None
    filelist: list[LhaInfo]
    NameToInfo: dict[str, LhaInfo]
    def __init__(
        self,
        file: str | IO[bytes],
        mode: str = "r",
        compression: object = None,
        callback: Callable[..., object] | None = None,
        args: object = None,
    ) -> None: ...
    def lhaname(self) -> str | None: ...
    def namelist(self) -> list[str] | None: ...
    def infolist(self) -> list[LhaInfo]: ...
    def read(self, name: str) -> bytes: ...

Lhafile = LhaFile

def is_lhafile(filename: str) -> bool: ...
