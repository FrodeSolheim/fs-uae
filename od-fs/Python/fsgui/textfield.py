from fsgui.drawingcontext import Colour
from fsgui.types import Size
from fsgui.widget import Widget
from fsgui.widgetpainter import WidgetPainter


class TextField(Widget):
    def __init__(
        self,
        text: str,
        *,
        parent: Widget | None = None,
        size: Size | None = None,
    ):
        super().__init__(parent=parent, size=size)
        self.style.background_color = (255, 255, 255, 255)
        self._text = text

    def calculate_min_height(self, width: int) -> int:
        return 28

    def calculate_min_width(self) -> int:
        return 28

    def get_text(self) -> str:
        return self._text

    def on_erase(self):
        pass

    def on_paint(self):
        # dc = DrawingContext(self)
        dc = self.create_dc()
        painter = WidgetPainter(self, dc)
        painter.paint_box((0, 0), self.size)

        # A bit lighter text color on white background # FIXME: From theme?
        if self.enabled:
            dc.set_text_colour(Colour.GREY_22)

        _, th = dc.measure_text(self._text)
        # FIXME: 6 - variable from theme, same as TextField
        dc.draw_text(self._text, (6, (self.height - th) // 2))

    def set_text(self, text: str):
        self._text = text
