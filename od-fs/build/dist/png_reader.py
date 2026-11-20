"""Reads the PNG files that FS-UAE's GL grab writes (GLGrab_<swap>.png): 8-bit RGBA, not
interlaced. Shared by the dist smoke test (smoke.py here), the WHDLoad smoke test
(od-fs/tests/whdload-smoke) and the compositor test (od-fs/tests/compositor), with the standard
library only.
"""

import zlib
from pathlib import Path

PNG_SIGNATURE = b"\x89PNG\r\n\x1a\n"


class Png:
    """A decoded 8-bit RGBA PNG: its size, and its rows from the top, as RGBA bytes."""

    def __init__(self, width: int, height: int, rows: list[bytes]) -> None:
        self.width = width
        self.height = height
        self.rows = rows

    def pixel(self, x: int, y: int) -> tuple[int, int, int, int]:
        """The (r, g, b, a) of the pixel at x, y (from the top left)."""
        row = self.rows[y]
        return row[x * 4], row[x * 4 + 1], row[x * 4 + 2], row[x * 4 + 3]

    def rgb(self, x: int, y: int) -> tuple[int, int, int]:
        """The (r, g, b) of the pixel at x, y (from the top left)."""
        row = self.rows[y]
        return row[x * 4], row[x * 4 + 1], row[x * 4 + 2]


def read_png(path: Path) -> Png:
    """Decodes an 8-bit RGBA non-interlaced PNG, as GL grabs are. Raises ValueError (or zlib.error)
    when the file is not one."""
    data = path.read_bytes()
    if data[:8] != PNG_SIGNATURE:
        raise ValueError(f"not a PNG file: {path}")
    pos = 8
    idat = bytearray()
    width = height = 0
    while pos < len(data):
        length = int.from_bytes(data[pos : pos + 4])
        kind = data[pos + 4 : pos + 8]
        chunk = data[pos + 8 : pos + 8 + length]
        if kind == b"IHDR":
            width, height = int.from_bytes(chunk[0:4]), int.from_bytes(chunk[4:8])
            depth, colour_type, interlace = chunk[8], chunk[9], chunk[12]
            if depth != 8 or colour_type != 6 or interlace:
                raise ValueError(f"not an 8-bit RGBA non-interlaced PNG: {path}")
        elif kind == b"IDAT":
            idat += chunk
        pos += 12 + length
    raw = zlib.decompress(idat)
    stride = width * 4
    rows = []
    previous = bytearray(stride)
    for y in range(height):
        start = y * (stride + 1)
        filter_type = raw[start]
        line = bytearray(raw[start + 1 : start + 1 + stride])
        if filter_type == 1:  # Sub
            for i in range(4, stride):
                line[i] = (line[i] + line[i - 4]) & 0xFF
        elif filter_type == 2:  # Up
            for i in range(stride):
                line[i] = (line[i] + previous[i]) & 0xFF
        elif filter_type == 3:  # Average
            for i in range(stride):
                left = line[i - 4] if i >= 4 else 0
                line[i] = (line[i] + (left + previous[i]) // 2) & 0xFF
        elif filter_type == 4:  # Paeth
            for i in range(stride):
                a = line[i - 4] if i >= 4 else 0
                b = previous[i]
                c = previous[i - 4] if i >= 4 else 0
                p = a + b - c
                pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
                line[i] = (line[i] + (a if pa <= pb and pa <= pc else b if pb <= pc else c)) & 0xFF
        elif filter_type != 0:
            raise ValueError(f"unknown PNG filter type {filter_type} in {path}")
        rows.append(bytes(line))
        previous = line
    return Png(width, height, rows)
