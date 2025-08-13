import fsgui_font  # type: ignore
import fsgui_image  # type: ignore
import fsgui_surface  # type: ignore

from fsgui.font import Font
from fsgui.fontmanager import FontManager
from fsgui.image import Image
from fsgui.types import Colour, Position, Size, TColour


class DrawingContext:
    def __init__(self, widget: "Widget") -> None:
        self.widget = widget
        self.surface = widget._surface.get_surface()

        # self.origin = widget._surface_pos
        self.origin = widget.get_dc_offset()

        self.pen_colour: TColour = Colour.BLACK
        self.fill_colour: TColour = Colour.BLACK
        self.text_colour: TColour = Colour.BLACK

        # self._font = self.get_ui_font()
        self._font = widget.font

        # FIXME: Setup clipping based on widget rect!

    # FIXME: Use floats to support high-dpi interfaces?
    def draw_filled_rectangle(self, position: Position, size: Size) -> None:
        color = self.fill_colour
        p = self.origin[0] + position[0], self.origin[1] + position[1]
        fsgui_surface.draw_filled_rectangle(self.surface, p, size, color)

        # FIXME: draw_filled_rectangle_with_outline

    def draw_filled_rectangle_with_outline(self, position: Position, size: Size) -> None:
        # FIXME: Replace with optimized C function

        x1, y1 = position
        w, h = size

        self.draw_filled_rectangle((x1 + 1, y1 + 1), (w - 2, h - 2))

        x2 = x1 + w - 1
        y2 = y1 + h - 1

        self.draw_line((x1, y1), (x2, y1))
        self.draw_line((x1, y1 + 1), (x1, y2 - 1))
        self.draw_line((x2, y1 + 1), (x2, y2 - 1))
        self.draw_line((x1, y2), (x2, y2))

    def draw_image(self, image: Image, position: Position) -> None:
        p = self.origin[0] + position[0], self.origin[1] + position[1]
        fsgui_image.draw(image._image, self.surface, p)

    def draw_image_stretched(self, image: Image, position: Position, size: Size) -> None:
        p = self.origin[0] + position[0], self.origin[1] + position[1]
        fsgui_image.draw_stretched(image._image, self.surface, p, size)

    def draw_line(
        self,
        point_a: Position,
        point_b: Position,
        color: TColour | None = None,
    ) -> None:
        if color is None:
            color = self.pen_colour
        p1 = self.origin[0] + point_a[0], self.origin[1] + point_a[1]
        p2 = self.origin[0] + point_b[0], self.origin[1] + point_b[1]
        fsgui_surface.draw_line(self.surface, p1, p2, color)

    def draw_point(self, point_a: Position, color: TColour | None = None) -> None:
        if color is None:
            color = self.pen_colour
        p1 = self.origin[0] + point_a[0], self.origin[1] + point_a[1]
        # FIXME: Using line draw for now
        fsgui_surface.draw_line(self.surface, p1, p1, color)

    def draw_text(self, text, position: Position) -> None:
        # FIXME: Cache...
        _, th = fsgui_font.measure_text(self._font.font, "Åg")
        tw, th2 = fsgui_font.measure_text(self._font.font, text)
        # Offset the text to account for improper positioning
        dy = th - th2

        # FIXME: For later - we also need to send a bounding box... for clipping!
        p = self.origin[0] + position[0], self.origin[1] + position[1] + dy

        fsgui_font.draw_text(self._font._font, self.surface, text, p, self.text_colour)

    def draw_text_wrapped(self, text, position: Position, wrap_width: int) -> None:
        # FIXME: For later - we also need to send a bounding box... for clipping!
        p = self.origin[0] + position[0], self.origin[1] + position[1]
        fsgui_font.draw_text_wrapped(self._font.font, self.surface, text, p, wrap_width)

    def get_bold_ui_font(self) -> Font:
        return FontManager.get().get_bold_ui_font()

    def get_ui_font(self) -> Font:
        return FontManager.get().get_ui_font()

    def measure_text(self, text: str) -> Size:
        # FIXME: Cache...
        _, th = fsgui_font.measure_text(self._font.font, "Åg")
        tw, th2 = fsgui_font.measure_text(self._font.font, text)
        return tw, th

    def set_pen_colour(self, pen_colour: TColour) -> None:
        self.pen_colour = pen_colour

    def set_fill_colour(self, fill_colour: TColour) -> None:
        self.fill_colour = fill_colour

    def set_text_colour(self, text_colour: TColour) -> None:
        self.text_colour = text_colour

    def set_font(self, font: Font) -> None:
        self._font = font


from .widget import Widget  # noqa: E402
