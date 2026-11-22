import os
from dataclasses import dataclass

from fsapp.surfaceapi.v1 import Filter
from fsapp.surfaceapi.v1 import Image as LoadedImage

from .internal.api import get_api
from .resources import get_images_dirs

_Rectangle = tuple[int, int, int, int]

# A 9-patch marker pixel: opaque black (premultiplied BGRA).
_MARKER = b"\x00\x00\x00\xff"


@dataclass(frozen=True)
class NinePatch:
    """The sizes in pixels of a 9-patch image's edges (its corners' widths and heights), inside its
    1-pixel frame of markers."""

    left: int
    top: int
    right: int
    bottom: int


def read_nine_patch(image: LoadedImage) -> NinePatch | None:
    """A 9-patch image's edges, from the black markers in its frame: along the top row they span
    the stretched columns, and down the left column the stretched rows. None if there is no marker
    (from the second pixel) along the top."""
    width, height = image.size
    pixels = bytes(image.pixels())

    def is_marker(x: int, y: int) -> bool:
        offset = (y * width + x) * 4
        return pixels[offset : offset + 4] == _MARKER

    left = right = 0
    for x in range(1, width - 1):
        if left == 0:
            if is_marker(x, 0):
                left = x - 1
        elif not is_marker(x, 0):
            right = width - x - 1
            break
    top = bottom = 0
    for y in range(1, height - 1):
        if top == 0:
            if is_marker(0, y):
                top = y - 1
        elif not is_marker(0, y):
            bottom = height - y - 1
            break
    if left == 0:
        return None
    return NinePatch(left, top, right, bottom)


def _fit_edges(first: int, last: int, factor: int, size: int) -> tuple[int, int]:
    """The pixel sizes of two opposite edges at factor, shrunk in proportion when they do not fit
    in size pixels."""
    first_out = first * factor
    last_out = last * factor
    if first_out + last_out > size:
        first_out = int(size * first / (first + last)) if first + last > 0 else 0
        last_out = size - first_out
    return first_out, last_out


def nine_patch_parts(
    image_size: tuple[int, int], nine_patch: NinePatch, destination: _Rectangle, factor: int
) -> list[tuple[_Rectangle, _Rectangle, Filter]]:
    """The nine parts (source, destination and filter) that draw a 9-patch image into the
    destination rectangle: corners and edges at the whole factor with nearest sampling (sharp at
    any scale), the centre stretched linearly. Empty parts are left out."""
    width, height = image_size
    x, y, w, h = destination
    factor = max(factor, 1)
    source_x = [1, 1 + nine_patch.left, width - 1 - nine_patch.right, width - 1]
    source_y = [1, 1 + nine_patch.top, height - 1 - nine_patch.bottom, height - 1]
    left, right = _fit_edges(nine_patch.left, nine_patch.right, factor, w)
    top, bottom = _fit_edges(nine_patch.top, nine_patch.bottom, factor, h)
    destination_x = [x, x + left, x + w - right, x + w]
    destination_y = [y, y + top, y + h - bottom, y + h]
    parts = []
    for row in range(3):
        for column in range(3):
            source = (
                source_x[column],
                source_y[row],
                source_x[column + 1] - source_x[column],
                source_y[row + 1] - source_y[row],
            )
            part = (
                destination_x[column],
                destination_y[row],
                destination_x[column + 1] - destination_x[column],
                destination_y[row + 1] - destination_y[row],
            )
            if min(source[2], source[3], part[2], part[3]) <= 0:
                continue
            centre = row == 1 and column == 1
            parts.append((source, part, Filter.LINEAR if centre else Filter.NEAREST))
    return parts


class Image:
    @classmethod
    def from_resource(cls, name: str) -> "Image":
        """An image from the images directories. A 9-patch image (<name>.9p.png) gets its edges
        from its markers, and its @2x image (<name>@2x.9p.png) where there is one."""
        for images_dir in get_images_dirs():
            image_path = os.path.join(images_dir, name)
            print(image_path)
            if os.path.exists(image_path):
                break
        else:
            raise FileNotFoundError(name)

        image = cls.from_file(image_path)
        if image_path.endswith(".9p.png"):
            image.nine_patch = read_nine_patch(image._image)
            path_2x = image_path.removesuffix(".9p.png") + "@2x.9p.png"
            if os.path.exists(path_2x):
                image.image_2x = cls.from_file(path_2x)
                image.image_2x.nine_patch = read_nine_patch(image.image_2x._image)
        return image

    @classmethod
    def from_file(cls, path: str) -> "Image":
        """The image file at path (OSError if it cannot be loaded)."""
        return cls(get_api().load_image(path))

    @classmethod
    def from_svg(cls, path: str, size: tuple[int, int]) -> "Image":
        """The SVG file at path, rendered at size in pixels (OSError if it cannot be loaded)."""
        return cls(get_api().load_svg(path, size))

    def __init__(self, image: LoadedImage) -> None:
        self._image = image
        # A 9-patch image's edges: it is drawn stretched in nine parts.
        self.nine_patch: NinePatch | None = None
        # A 9-patch image's @2x image (<name>@2x.9p.png), drawn for edge factors of 2 and more.
        self.image_2x: Image | None = None
        self.width: int
        self.height: int
        self.width, self.height = image.size

    def tint(self, r: int, g: int, b: int, a: int = 255) -> "Image":
        """A copy of the image tinted with the RGBA colour; the image itself is unchanged."""
        tinted = Image(self._image.tinted((r, g, b, a)))
        tinted.nine_patch = self.nine_patch
        if self.image_2x is not None:
            tinted.image_2x = self.image_2x.tint(r, g, b, a)
        return tinted
