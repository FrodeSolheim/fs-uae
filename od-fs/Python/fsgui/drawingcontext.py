from fsapp.surfaceapi.v1 import Canvas

from .font import Font
from .fontmanager import FontManager
from .image import Image, nine_patch_parts
from .internal.surface import Surface
from .scale import (
    fitting_pixels,
    get_scale,
    line_to_pixels,
    rect_to_pixels,
    round_to_pixels,
    to_pixels,
    units_from_pixel_size,
)
from .types import Colour, PixelPosition, PixelRect, PixelSize, Position, Size, TColour

__all__ = ["DrawingContext"]


class _Paint:
    """The colours and the font, shared by a drawing context and its pixel view."""

    def __init__(self, font: Font) -> None:
        self.pen_colour: TColour = Colour.BLACK
        self.fill_colour: TColour = Colour.BLACK
        self.text_colour: TColour = Colour.BLACK
        self.font = font


class DrawingContext:
    """Draws in a window's surface, through a canvas whose origin is the widget's position in the
    surface's pixels. Every call takes design units, relative to origin (the widget's position in
    its window, in design units); each rectangle becomes pixels by rounding its edges relative to
    the window's origin (fsgui/scale.py), so that what a widget draws matches its rounded
    rectangle. Lines (outlines, line widths, 9-patch images' edges) follow the line rule
    (line_to_pixels).

    To draw with pixel values, use the pixel view (pixel_view): the same calls in pixels, relative
    to the widget's rectangle in the surface's pixels; rect_to_view_pixels and
    design_line_to_pixels give the pixels of design values for it."""

    def __init__(self, surface: Surface, origin: Position, font: Font, size: Size = (0, 0)) -> None:
        self._setup(surface.canvas(), origin, size, get_scale(), get_scale(), _Paint(font))
        x, y, _, _ = self._rect_to_surface_pixels((0, 0), size)
        self._canvas.origin = (x, y)

    def _setup(
        self,
        canvas: Canvas,
        origin: Position,
        size: Size,
        scale: float,
        pixels_per_unit: float,
        paint: _Paint,
    ) -> None:
        # Shared with the pixel view.
        self._canvas = canvas
        # The widget's rectangle in the surface, in this context's units.
        self.origin = origin
        self.size = size
        # fsgui's scale, also in the pixel view.
        self.scale = scale
        # The scale of this context's units: fsgui's scale, or 1 in the pixel view.
        self._pixels_per_unit = pixels_per_unit
        self._paint = paint
        # In this context's units.
        self.line_width: float = 1

        # FIXME: Setup clipping based on widget rect!

    @property
    def pen_colour(self) -> TColour:
        return self._paint.pen_colour

    @pen_colour.setter
    def pen_colour(self, colour: TColour) -> None:
        self._paint.pen_colour = colour

    @property
    def fill_colour(self) -> TColour:
        return self._paint.fill_colour

    @fill_colour.setter
    def fill_colour(self, colour: TColour) -> None:
        self._paint.fill_colour = colour

    @property
    def text_colour(self) -> TColour:
        return self._paint.text_colour

    @text_colour.setter
    def text_colour(self, colour: TColour) -> None:
        self._paint.text_colour = colour

    @property
    def _font(self) -> Font:
        return self._paint.font

    @property
    def pixel_view(self) -> "DrawingContext":
        """A view of this drawing context in pixels: the same calls, taking pixels relative to the
        widget's rectangle in the surface's pixels, with the same colours and font."""
        x, y, w, h = self._rect_to_surface_pixels((0, 0), self.size)
        view = DrawingContext.__new__(DrawingContext)
        view._setup(self._canvas, (x, y), (w, h), self.scale, 1.0, self._paint)
        return view

    def rect_to_view_pixels(self, position: Position, size: Size) -> PixelRect:
        """A rectangle in this context's units in the pixel view's pixels: its edges rounded,
        relative to the widget's rectangle in the surface's pixels."""
        x, y, w, h = self._rect_to_surface_pixels(position, size)
        widget_x, widget_y = self._point_to_surface_pixels((0, 0))
        return x - widget_x, y - widget_y, w, h

    def design_line_to_pixels(self, width: float = 1) -> int:
        """A line of width design units in pixels (the line rule), also in the pixel view."""
        return line_to_pixels(width, self.scale)

    def _line_to_pixels(self, width: float) -> int:
        """A line of width in this context's units in pixels."""
        return line_to_pixels(width, self._pixels_per_unit)

    def _point_to_surface_pixels(self, point: Position) -> PixelPosition:
        return (
            to_pixels(self.origin[0] + point[0], self._pixels_per_unit),
            to_pixels(self.origin[1] + point[1], self._pixels_per_unit),
        )

    def _rect_to_surface_pixels(self, position: Position, size: Size) -> PixelRect:
        return rect_to_pixels(
            (self.origin[0] + position[0], self.origin[1] + position[1]),
            size,
            self._pixels_per_unit,
        )

    def _point_to_canvas(self, point: PixelPosition) -> PixelPosition:
        """A point in the surface's pixels in the canvas's coordinates."""
        return point[0] - self._canvas.origin[0], point[1] - self._canvas.origin[1]

    def _point_to_canvas_pixels(self, point: Position) -> PixelPosition:
        return self._point_to_canvas(self._point_to_surface_pixels(point))

    def _fill_surface_pixels(self, x: int, y: int, w: int, h: int, colour: TColour) -> None:
        self._canvas.fill_rectangle(*self._point_to_canvas((x, y)), w, h, colour)

    def draw_filled_rectangle(self, position: Position, size: Size) -> None:
        x, y, w, h = self._rect_to_surface_pixels(position, size)
        self._fill_surface_pixels(x, y, w, h, self.fill_colour)

    def draw_filled_rectangle_with_outline(
        self, position: Position, size: Size, width: float = 1
    ) -> None:
        """A rectangle filled with the fill colour inside an outline of width (a line) in the pen
        colour."""
        x, y, w, h = self._rect_to_surface_pixels(position, size)
        thickness = self._line_to_pixels(width)
        self._fill_surface_pixels(
            x + thickness, y + thickness, w - 2 * thickness, h - 2 * thickness, self.fill_colour
        )
        self.draw_outline(position, size, width=width)

    def draw_outline(
        self,
        position: Position,
        size: Size,
        *,
        width: float = 1,
        top: bool = True,
        right: bool = True,
        bottom: bool = True,
        left: bool = True,
    ) -> None:
        """The sides asked for of an outline of width (a line) in the pen colour, inside the
        rectangle's rounded edges, the same thickness on every side. The top and bottom sides span
        the whole width; the left and right sides span the height between them."""
        x, y, w, h = self._rect_to_surface_pixels(position, size)
        thickness = self._line_to_pixels(width)
        side_top = y + thickness if top else y
        side_bottom = y + h - thickness if bottom else y + h
        colour = self.pen_colour
        if top:
            self._fill_surface_pixels(x, y, w, thickness, colour)
        if left:
            self._fill_surface_pixels(x, side_top, thickness, side_bottom - side_top, colour)
        if right:
            self._fill_surface_pixels(
                x + w - thickness, side_top, thickness, side_bottom - side_top, colour
            )
        if bottom:
            self._fill_surface_pixels(x, side_bottom, w, thickness, colour)

    def draw_image(self, image: Image, position: Position) -> None:
        """Draws an image at its own size in pixels."""
        self._canvas.draw_image(image._image, *self._point_to_canvas_pixels(position))

    # REVIEW: Stretched (linear) at a scale other than 1, so a little blurred; the icons that use it
    # (the title bars' buttons, the choice arrow) have no SVG.
    def draw_image_scaled(self, image: Image, position: Position) -> None:
        """Draws an image made for a scale of 1 (a PNG resource) at the scale: its size in pixels is
        its size in design units."""
        x, y, w, h = self._rect_to_surface_pixels(position, (image.width, image.height))
        x, y = self._point_to_canvas((x, y))
        self._canvas.draw_image(image._image, x, y, width=w, height=h)

    # REVIEW: The 9-patch edge factor is the line width of 1 unit, so edges match the outlines (1 px
    # at 125 to 175 %); an @2x image is used for even factors only, at half the factor.
    def draw_image_stretched(self, image: Image, position: Position, size: Size) -> None:
        """Draws an image stretched to size. A 9-patch image's corners and edges are drawn at a
        whole factor, the line width of 1 unit, from its @2x image where it has one; only its
        centre is stretched."""
        x, y, w, h = self._rect_to_surface_pixels(position, size)
        x, y = self._point_to_canvas((x, y))
        factor = self.design_line_to_pixels(1)
        if image.image_2x is not None and factor % 2 == 0:
            image = image.image_2x
            factor //= 2
        if image.nine_patch is None:
            self._canvas.draw_image(image._image, x, y, width=w, height=h)
            return
        parts = nine_patch_parts(
            (image.width, image.height), image.nine_patch, (x, y, w, h), factor
        )
        for source, part, filter in parts:
            self._canvas.draw_image(
                image._image,
                part[0],
                part[1],
                width=part[2],
                height=part[3],
                source=source,
                filter=filter,
            )

    def draw_line(
        self,
        point_a: Position,
        point_b: Position,
        color: TColour | None = None,
    ) -> None:
        if color is None:
            color = self.pen_colour
        self._canvas.draw_line(
            *self._point_to_canvas_pixels(point_a),
            *self._point_to_canvas_pixels(point_b),
            color,
            self._line_to_pixels(self.line_width),
        )

    def draw_point(self, point_a: Position, color: TColour | None = None) -> None:
        if color is None:
            color = self.pen_colour
        p1 = self._point_to_canvas_pixels(point_a)
        # FIXME: Using line draw for now
        self._canvas.draw_line(*p1, *p1, color, self._line_to_pixels(self.line_width))

    def draw_text(self, text: str, position: Position) -> None:
        self._draw_text_at_surface_pixels(text, self._point_to_surface_pixels(position))

    def draw_text_centred(
        self, text: str, position: Position, size: Size, *, horizontally: bool = False
    ) -> None:
        """Draws text centred vertically in the rectangle of position and size, and horizontally
        too if asked; centred in pixels, so that it sits centred at any scale."""
        x, y, w, h = self._rect_to_surface_pixels(position, size)
        text_width, line_height = self._measure_text_pixels(text)
        if horizontally:
            x += (w - text_width) // 2
        self._draw_text_at_surface_pixels(text, (x, y + (h - line_height) // 2))

    def _draw_text_at_surface_pixels(self, text: str, point: PixelPosition) -> None:
        font = self._font.font
        # FIXME: Cache...
        _, th = font.measure("Åg")
        tw, th2 = font.measure(text)
        # Offset the text to account for improper positioning
        dy = th - th2

        # FIXME: For later - we also need to send a bounding box... for clipping!
        x, y = self._point_to_canvas(point)
        self._canvas.draw_text(font, text, x, y + dy, self.text_colour)

    def draw_text_wrapped(self, text: str, position: Position, wrap_width: float) -> None:
        """Draws text in the text colour wrapped at the pixels wrap_width has wherever it is
        (fitting_pixels), as Label measures it."""
        # FIXME: For later - we also need to send a bounding box... for clipping!
        self._canvas.draw_text_wrapped(
            self._font.font,
            text,
            *self._point_to_canvas_pixels(position),
            fitting_pixels(wrap_width, self._pixels_per_unit),
            self.text_colour,
        )

    def get_bold_ui_font(self) -> Font:
        return FontManager.get().get_bold_ui_font()

    def get_ui_font(self) -> Font:
        return FontManager.get().get_ui_font()

    def measure_text(self, text: str) -> Size:
        """The text's width and the font's line height, in this context's units (rounded up)."""
        return units_from_pixel_size(self._measure_text_pixels(text), self._pixels_per_unit)

    def _measure_text_pixels(self, text: str) -> PixelSize:
        """The text's width and the font's line height in pixels."""
        # FIXME: Cache...
        _, th = self._font.font.measure("Åg")
        tw, _ = self._font.font.measure(text)
        return tw, th

    def set_pen_colour(self, pen_colour: TColour) -> None:
        self.pen_colour = pen_colour

    def set_fill_colour(self, fill_colour: TColour) -> None:
        self.fill_colour = fill_colour

    def set_text_colour(self, text_colour: TColour) -> None:
        self.text_colour = text_colour

    def set_font(self, font: Font) -> None:
        self._paint.font = font

    def set_line_colour(self, line_colour: TColour) -> None:
        """Set the line/pen colour"""
        self.pen_colour = line_colour

    def set_line_width(self, width: float) -> None:
        """Set the line width for drawing operations, in this context's units (a line: the line
        rule)."""
        self.line_width = width

    def draw_circle(self, center: Position, radius: float) -> None:
        """Draw a circle outline using optimized C implementation"""
        self._canvas.draw_circle(
            *self._point_to_canvas_pixels(center),
            round_to_pixels(radius, self._pixels_per_unit),
            self.pen_colour,
            self._line_to_pixels(self.line_width),
        )

    def draw_filled_circle(self, center: Position, radius: float) -> None:
        """Draw a filled circle using optimized C implementation"""
        self._canvas.fill_circle(
            *self._point_to_canvas_pixels(center),
            round_to_pixels(radius, self._pixels_per_unit),
            self.fill_colour,
        )
