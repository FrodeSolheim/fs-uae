import fsgui_font  # type: ignore

from fsgui.font import Font
from fsgui.types import Size
from fsgui.widget import Widget


class Label(Widget):
    def __init__(
        self,
        text: str = "",
        *,
        font: Font | None = None,
        parent: Widget | None = None,
        size: Size | None = None,
        wrap: bool = False,
    ):
        super().__init__(font=font, parent=parent, size=size)
        self._text = text
        self.wrap = wrap

        # Maybe a good idea, maybe not
        if self.wrap:
            self._fill = True

    # def __del__(self):
    #     print("Label.__del__")

    def calculate_min_height(self, width: int) -> int:
        # FIXME: calculate and cache min size
        # return 28

        # FIXME: self.font!

        # from fsgui.fontmanager import FontManager

        # font = FontManager.get().get_ui_font()
        font = self.font

        if self.wrap:
            _, th = fsgui_font.measure_text_wrapped(
                font.font,
                self._text,
                width,
            )
        else:
            _, th = fsgui_font.measure_text(font.font, "Åg")

        # FIXME: Cache...
        # _, th = fsgui_font.measure_text(font.font, "Åg")
        # tw, th2 = fsgui_font.measure_text(self._font.font, self._text)
        # Offset the text to account for improper positioning
        # dy = th - th2
        # tw, th = fsgui_font.measure_text(font.font, self._text)

        # if self.wrap:
        #     print("calculate_min_height", width, "=", th)
        return th

    def calculate_min_width(self) -> int:
        if self.wrap:
            # print("calculate_min_width", "=", 0)
            return 0

        # FIXME: self.font!

        # from fsgui.fontmanager import FontManager

        # font = FontManager.get().get_ui_font()
        font = self.font
        tw, _ = fsgui_font.measure_text(font.font, self._text)
        # if self.wrap:
        #     print("calculate_min_width", "=", tw)
        return tw

    def get_text(self) -> str:
        return self._text

    def on_paint(self):
        dc = self.create_dc()
        _, th = dc.measure_text(self._text)
        # print(self._text, self.font.size)
        if self.wrap:
            # print(self._size, self.width)
            dc.draw_text_wrapped(self._text, (0, 0), self.width)
        else:
            dc.draw_text(self._text, (0, (self.height - th) // 2))

    def set_text(self, text: str):
        self._text = text

    def set_size_internal(self, size):
        # if self.wrap:
        #     print("set_size_internal", size)
        super().set_size_internal(size)
