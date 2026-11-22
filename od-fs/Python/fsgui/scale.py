"""fsgui's scale: how many pixels a design unit is, and how design units become pixels.

fsgui's API is in design units (designed at a scale of 1): positions and sizes of windows and
widgets, padding, gaps, font sizes and mouse positions. Pixels come only when drawing: a rectangle
becomes pixels by rounding each of its edges (rect_to_pixels, relative to its window's origin), so
that adjacent rectangles share their edges; a window's surface size is rounded as the far edges of a
rectangle at 0 (size_to_pixels), so that what is drawn at its edges is inside it, and a length on
its own (a font's pixel size) is rounded by itself (scaled). Sizes measured in pixels (text, images)
become design units rounded up (units_from_pixels), and text is wrapped at the pixels its width has
wherever it is (fitting_pixels). A line (an outline, a border, a 9-patch image's edges) is its
width times the scale rounded down, at least 1 pixel (line_to_pixels), the same wherever it is.

There is one scale, set by the main window: the predicted display scale before the host window
exists, then the display scale of each configure. The scale is rounded to 1/120 (Wayland's unit for
fractional scales), so that a display scale off in its last digits (Wayland's, computed from rounded
sizes) counts as the scale it was meant to be. fsapp's hit test rounds its zones by the same rule
(fsapp_pixels_scale_length).

Only the standard library here, so that the tests can load this module alone.
"""

import math

_scale = 1.0


# REVIEW: Rounding to 1/120 treats display scales that close as equal, here and in fsapp's hit test.
def normalize_scale(scale: float) -> float:
    """The scale rounded to 1/120 (halves away from zero), at least 1/120; 1 for 0 or less."""
    if scale <= 0:
        return 1.0
    return max(math.floor(scale * 120 + 0.5), 1) / 120


def round_to_pixels(value: float, scale: float) -> int:
    """value times scale, rounded to the nearest (halves away from zero), and at least 1 pixel (or
    -1) when value is not 0."""
    pixels = math.floor(abs(value) * scale + 0.5)
    if pixels == 0 and value != 0:
        pixels = 1
    return pixels if value >= 0 else -pixels


# Edges computed along different paths (a position plus a size, or the next position) may differ in
# their last bits; the margin makes both round the same way at a half.
_EDGE_EPSILON = 1e-6


def to_pixels(value: float, scale: float) -> int:
    """A position or an edge in design units in pixels: value times scale, rounded to the nearest
    (halves away from zero)."""
    pixels = math.floor(abs(value) * scale + 0.5 + _EDGE_EPSILON)
    return pixels if value >= 0 else -pixels


def rect_to_pixels(
    position: tuple[float, float], size: tuple[float, float], scale: float
) -> tuple[int, int, int, int]:
    """A rectangle in design units in pixels (x, y, width, height), each edge rounded (to_pixels):
    the size is the difference between the rounded edges."""
    x = to_pixels(position[0], scale)
    y = to_pixels(position[1], scale)
    right = to_pixels(position[0] + size[0], scale)
    bottom = to_pixels(position[1] + size[1], scale)
    return x, y, right - x, bottom - y


def size_to_pixels(size: tuple[float, float], scale: float) -> tuple[int, int]:
    """A size in design units in pixels, the far edges of a rectangle of that size at 0 (to_pixels):
    a surface of this size holds what is drawn inside the rectangle's rounded edges."""
    return to_pixels(size[0], scale), to_pixels(size[1], scale)


def fitting_pixels(length: float, scale: float) -> int:
    """A length in design units in the whole pixels it has wherever its edges are rounded: length
    times scale, rounded down."""
    return math.floor(length * scale + _EDGE_EPSILON)


# REVIEW: The line rule. Rounded down, so that a 1-unit line is 1, 1, 1 and 2 pixels at 125, 150,
# 175 and 200 %, thin rather than bold; 2 pixels at 175 % would be round_to_pixels.
def line_to_pixels(width: float, scale: float) -> int:
    """A line's width in design units in pixels: width times scale, rounded down, and at least 1
    pixel when width is not 0."""
    if width <= 0:
        return 0
    return max(math.floor(width * scale + _EDGE_EPSILON), 1)


# REVIEW: Rounded up to whole design units, so measured content (text, images) always fits after
# edge rounding; at fractional scales a size can grow by up to one unit.
def units_from_pixels(pixels: float, scale: float) -> int:
    """A size measured in pixels in design units, rounded up to a whole unit."""
    return math.ceil(pixels / scale - _EDGE_EPSILON)


def units_from_pixel_size(size: tuple[float, float], scale: float) -> tuple[int, int]:
    """A size measured in pixels (width, height) in design units, each rounded up to whole units."""
    return units_from_pixels(size[0], scale), units_from_pixels(size[1], scale)


def position_from_pixels(position: tuple[float, float], scale: float) -> tuple[float, float]:
    """A position in pixels (a pointer's, with fractions) in design units, not rounded."""
    return position[0] / scale, position[1] / scale


def get_scale() -> float:
    return _scale


def set_scale(scale: float) -> bool:
    """Sets the scale (rounded to 1/120); True if it changed."""
    global _scale
    scale = normalize_scale(scale)
    if scale == _scale:
        return False
    _scale = scale
    return True


def scaled(value: float) -> int:
    """A length in design units in pixels at the scale."""
    return round_to_pixels(value, _scale)


def scaled_pair(pair: tuple[float, float]) -> tuple[int, int]:
    """A size or position in design units in pixels at the scale."""
    return (round_to_pixels(pair[0], _scale), round_to_pixels(pair[1], _scale))
