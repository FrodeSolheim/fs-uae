# _fsapp_image.pyi
"""Typing stubs for the `_fsapp_image` C extension (fsapp/fsapp-image.h): images, drawn in a
surface's buffer (pixels), within a clip rectangle (_fsapp_surface). Call them on the UI thread. An
image is freed with its capsule."""

from typing import Final

_Clip = tuple[int, int, int, int] | None

FILTER_NEAREST: Final[int]
FILTER_LINEAR: Final[int]

def load(path: str, /) -> object:
    """Loads the image file at the path.

    Raises:
        RuntimeError: If the image cannot be loaded.
    """

def load_sized_svg(path: str, width: int, height: int, /) -> object:
    """Loads the SVG file at the path, rendered at the size (pixels).

    Raises:
        RuntimeError: If the image cannot be loaded.
    """

def get_size(image: object, /) -> tuple[int, int]:
    """The image's width and height in pixels."""

def tint(image: object, r: int, g: int, b: int, a: int, /) -> None:
    """Tints the image's own pixels: every pixel that is not transparent gets the colour (straight
    RGBA), its alpha multiplied by the colour's alpha."""

def create_tinted(image: object, colour: tuple[int, int, int, int], /) -> object:
    """A tinted copy of the image (as tint), a new image; the image is unchanged.

    Raises:
        RuntimeError: If the image cannot be copied.
    """

def get_pixels(image: object, /) -> bytes:
    """A copy of the image's pixels, premultiplied, bytes B, G, R, A, rows of width * 4 bytes.

    Raises:
        RuntimeError: If the image cannot be converted.
    """

def draw(image: object, surface: object, position: tuple[int, int], clip: _Clip = None, /) -> None:
    """Draws the image at its own size at the position in the surface's buffer. It shows once
    committed."""

def draw_part(
    image: object,
    surface: object,
    source: tuple[int, int, int, int],
    destination: tuple[int, int, int, int],
    filter: int,
    clip: _Clip = None,
    /,
) -> None:
    """Draws the source rectangle of the image (x, y, width, height) into the destination
    rectangle in the surface's buffer, scaled with the filter (FILTER_NEAREST or FILTER_LINEAR)
    when the sizes differ. It shows once committed.

    Raises:
        ValueError: If the filter is not one of them.
    """
