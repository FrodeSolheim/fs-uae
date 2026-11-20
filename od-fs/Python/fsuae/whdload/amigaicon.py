"""Reading tool types from Amiga icons (`.info` files, Workbench DiskObjects).

Only what is needed to get at the tool types is parsed: the DiskObject header and the structures
stored before the tool types (DrawerData and the Image structures with their bitplanes), whose
presence is given by the non-zero pointers in the header. Everything after the tool types (the
tool window, OS 2.x DrawerData extension, NewIcons and OS 3.5 `FORM ICON` data) is ignored.

The layout is checked as it is read, so an icon that is truncated, has another format, or is not
an icon at all raises `AmigaIconError` instead of returning bytes from the wrong place.
"""

import struct

# do_Magic of a Workbench DiskObject (WB_DISKMAGIC).
DISK_MAGIC = 0xE310

# Sizes of the structures as stored in the file.
DISK_OBJECT_SIZE = 78
DRAWER_DATA_SIZE = 56  # OldDrawerData: struct NewWindow plus dd_CurrentX/Y.
IMAGE_SIZE = 20


class AmigaIconError(ValueError):
    """The data is not an Amiga icon that can be read."""


def read_tool_types(data: bytes) -> list[str]:
    """The tool types of an Amiga icon, in order, decoded as ISO-8859-1.

    Each tool type is cut at its first NUL byte, as AmigaOS sees it. Raises `AmigaIconError` if
    the data cannot be read as an icon.
    """
    reader = _Reader(data)
    if len(data) < DISK_OBJECT_SIZE:
        raise AmigaIconError(f"too short for an icon ({len(data)} bytes)")
    (magic,) = struct.unpack_from(">H", data, 0)
    if magic != DISK_MAGIC:
        raise AmigaIconError(f"not an icon (magic {magic:#06x})")
    # do_Gadget.GadgetRender and SelectRender: the normal and the selected image.
    gadget_render, select_render = struct.unpack_from(">II", data, 22)
    # do_DefaultTool, do_ToolTypes, do_CurrentX/Y, do_DrawerData, do_ToolWindow, do_StackSize.
    default_tool, tool_types, _, _, drawer_data, _, _ = struct.unpack_from(">IIiiIII", data, 50)

    reader.pos = DISK_OBJECT_SIZE
    if drawer_data:
        reader.skip(DRAWER_DATA_SIZE, "DrawerData")
    for render in (gadget_render, select_render):
        if render:
            reader.skip_image()
    if default_tool:
        reader.read_string("default tool")
    if not tool_types:
        return []
    # The size of the tool type pointer array, including its terminating NULL pointer.
    size = reader.read_ulong("tool type count")
    count = size // 4 - 1
    if size % 4 or count < 0 or count * 4 > reader.remaining:
        raise AmigaIconError(f"invalid tool type array size {size}")
    return [reader.read_string(f"tool type {i + 1}") for i in range(count)]


class _Reader:
    def __init__(self, data: bytes) -> None:
        self.data = data
        self.pos = 0

    @property
    def remaining(self) -> int:
        return len(self.data) - self.pos

    def skip(self, size: int, what: str) -> None:
        if size > self.remaining:
            raise AmigaIconError(f"truncated {what} at offset {self.pos}")
        self.pos += size

    def read_ulong(self, what: str) -> int:
        if self.remaining < 4:
            raise AmigaIconError(f"truncated {what} at offset {self.pos}")
        value: int = struct.unpack_from(">I", self.data, self.pos)[0]
        self.pos += 4
        return value

    def read_string(self, what: str) -> str:
        """A string stored as its length (including the terminating NUL) and its bytes."""
        length = self.read_ulong(what)
        if length > self.remaining:
            raise AmigaIconError(f"invalid {what} length {length} at offset {self.pos - 4}")
        raw = self.data[self.pos : self.pos + length]
        self.pos += length
        return raw.split(b"\0", 1)[0].decode("ISO-8859-1")

    def skip_image(self) -> None:
        """An Image structure, followed by its bitplanes when ImageData is set."""
        if self.remaining < IMAGE_SIZE:
            raise AmigaIconError(f"truncated image at offset {self.pos}")
        _, _, width, height, depth, image_data = struct.unpack_from(">hhhhhI", self.data, self.pos)
        if width < 0 or height < 0 or depth < 0:
            raise AmigaIconError(f"invalid image size {width}x{height}x{depth}")
        self.pos += IMAGE_SIZE
        if image_data:
            self.skip((width + 15) // 16 * 2 * height * depth, "image data")
