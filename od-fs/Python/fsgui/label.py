from .font import Font
from .scale import fitting_pixels, get_scale, units_from_pixel_size
from .types import Size, TColour
from .widget import Widget


class Label(Widget):
    def __init__(
        self,
        text: str = "",
        *,
        font: Font | None = None,
        parent: Widget | None = None,
        size: Size | None = None,
        wrap: bool = False,
        text_color: TColour | None = None,
    ):
        super().__init__(font=font, parent=parent, size=size)
        self._text = text
        self.wrap = wrap
        self._text_color = text_color

        # Maybe a good idea, maybe not
        if self.wrap:
            self._fill = True

    # def __del__(self):
    #     print("Label.__del__")

    def calculate_min_height(self, width: float) -> float:
        # FIXME: calculate and cache min size
        # return 28

        # FIXME: self.font!

        # from fsgui.fontmanager import FontManager

        # font = FontManager.get().get_ui_font()
        font = self.font

        if not self.wrap:
            _, line_height = font.measure_text("Åg")
            return line_height
        # Wrapped at the same width as DrawingContext.draw_text_wrapped wraps it.
        scale = get_scale()
        wrapped_size = font.font.measure_wrapped(self._text, fitting_pixels(width, scale))

        # FIXME: Cache...
        # _, th = _fsapp_font.measure_text(font.font, "Åg")
        # tw, th2 = _fsapp_font.measure_text(self._font.font, self._text)
        # Offset the text to account for improper positioning
        # dy = th - th2
        # tw, th = _fsapp_font.measure_text(font.font, self._text)

        # if self.wrap:
        #     print("calculate_min_height", width, "=", th)
        return units_from_pixel_size(wrapped_size, scale)[1]

    def calculate_min_width(self) -> float:
        if self.wrap:
            # print("calculate_min_width", "=", 0)
            return 0

        # FIXME: self.font!

        # from fsgui.fontmanager import FontManager

        # font = FontManager.get().get_ui_font()
        tw, _ = self.font.measure_text(self._text)
        return tw

    def get_text(self) -> str:
        return self._text

    def on_paint(self):
        dc = self.create_dc()

        # Set text color if specified
        if self._text_color is not None:
            dc.set_text_colour(self._text_color)

        # print(self._text, self.font.size)
        if self.wrap:
            # print(self._size, self.width)
            dc.draw_text_wrapped(self._text, (0, 0), self.width)
        else:
            dc.draw_text_centred(self._text, (0, 0), self.size)

    def set_text(self, text: str):
        if text != self._text:
            self._text = text
            self.refresh()

    def set_text_color(self, color: TColour | None):
        """Set the text color for this label."""
        if color != self._text_color:
            self._text_color = color
            self.refresh()

    def set_size_internal(self, size):
        # if self.wrap:
        #     print("set_size_internal", size)
        super().set_size_internal(size)
