from fsgui.drawingcontext import Colour
from fsgui.font import Font
from fsgui.types import Size
from fsgui.widget import Widget
from fsgui.widgetpainter import WidgetPainter


class CheckBox(Widget):
    def __init__(
        self,
        text: str,
        *,
        font: Font | None = None,
        parent: Widget | None = None,
        size: Size | None = None,
        wrap: bool = False,
    ) -> None:
        super().__init__(font=font, parent=parent, size=size)
        self.text = text
        self.wrap = wrap
        self.check_width = 16
        self.gap = 6

        self.value = False
        # FIXME
        # self.enabled = False

        self.enable_hover_state()
        self.enable_pressed_state()

    def calculate_min_height(self, width: int) -> int:
        return 28

    def calculate_min_width(self) -> int:
        tw, _ = self.font.measure_text(self.text)
        return self.check_width + self.gap + tw

    # def is_pressed(self) -> bool:
    #     return False

    def on_change(self) -> None:
        pass

    def on_left_click(self) -> None:
        if not self.enabled:
            return
        self.value = not self.value
        self.on_change()
        # FIXME: self.refresh

    def on_paint(self) -> None:
        dc = self.create_dc()
        painter = WidgetPainter(self, dc)

        x1 = 0
        y1 = (self.height - self.check_width) // 2
        # x2 = x1 + self.check_width - 1
        # y2 = y1 + self.check_width - 1

        # FIXME: Pressed state
        painter.paint_box((x1, y1), (self.check_width, self.check_width))

        _, th = dc.measure_text(self.text)
        dc.draw_text(self.text, (self.check_width + self.gap, (self.height - th) // 2))

        # Drawing a square for now. Maybe draw a proper checkmark later
        # (however, the square is more visible though...)
        if self.enabled:
            fill_colour = Colour.GREY_33
        else:
            # FIXME: Check if this looks nice
            fill_colour = Colour.GREY_AA
        if self.value:
            p = 3
            dc.set_fill_colour(fill_colour)
            dc.draw_filled_rectangle(
                (x1 + p, y1 + p),
                (self.check_width - 2 * p, self.check_width - 2 * p),
            )

    # def paint_checkbox(self, dc: DrawingContext) -> None:
    #     # FIXME: self.size?
    #     w, h = self.get_size()
    #     # dc.set_color((0, 0, 0))

    #     pressed = self.is_mouse_pressed()
    #     hover = self.is_mouse_hovering()
    #     # FIXME
    #     # enabled = False

    #     if self.enabled:
    #         if hover:
    #             if pressed:
    #                 fill_colour = Colour.GREY_CC
    #             else:
    #                 fill_colour = Colour.GREY_EE
    #         else:
    #             fill_colour = Colour.WHITE
    #     else:
    #         fill_colour = Colour.GREY_DD

    #     x1 = 0
    #     y1 = (h - self.check_width) // 2
    #     x2 = x1 + self.check_width - 1
    #     y2 = y1 + self.check_width - 1

    #     dc.set_fill_colour(fill_colour)
    #     dc.draw_filled_rectangle(
    #         (x1, y1), (self.check_width, self.check_width)
    #     )

    #     # FIXME: FROM THEME
    #     dc.set_pen_colour(Colour.GREY_AA)
    #     dc.draw_line((x1, y1), (x2, y1))
    #     dc.draw_line((x2, y1), (x2, y2))
    #     dc.draw_line((x1, y2), (x2, y2))
    #     dc.draw_line((x1, y1), (x1, y2))

    def set_value(self, value: bool) -> None:
        if value == self.value:
            return
        self.value = value
        self.refresh()
