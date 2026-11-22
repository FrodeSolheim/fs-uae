# _fsapp_font.pyi
"""Typing stubs for the `_fsapp_font` C extension (fsapp/fsapp-font.h): TrueType fonts, measured
and drawn in a surface's buffer (pixels). Call them on the UI thread."""

def load(path: str, size_px: int, /) -> object:
    """Loads the font file at the path at the size (pixels). The font is freed with the capsule.

    Raises:
        RuntimeError: If the font cannot be loaded.
    """

def measure_text(font: object, text: str, /) -> tuple[int, int]:
    """The text's width and height in pixels, on one line."""

def measure_text_wrapped(font: object, text: str, wrap_width: int, /) -> tuple[int, int]:
    """The text's width and height in pixels, wrapped at wrap_width pixels."""

def draw_text(
    font: object,
    surface: object,
    text: str,
    position: tuple[int, int],
    colour: tuple[int, int, int, int],
    clip: tuple[int, int, int, int] | None = None,
    /,
) -> None:
    """Draws the text on one line at the position in the surface's buffer, in a colour (straight
    RGBA), within the clip (_fsapp_surface). It shows once committed."""

def draw_text_wrapped(
    font: object,
    surface: object,
    text: str,
    position: tuple[int, int],
    wrap_width: int,
    colour: tuple[int, int, int, int] = (0, 0, 0, 255),
    clip: tuple[int, int, int, int] | None = None,
    /,
) -> None:
    """Draws the text wrapped at wrap_width pixels at the position in the surface's buffer, in a
    colour (straight RGBA, black by default), within the clip. It shows once committed."""
