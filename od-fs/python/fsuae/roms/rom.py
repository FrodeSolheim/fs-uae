from dataclasses import dataclass


@dataclass
class ROM:
    path: str
    sha1: str
    crc32: str
