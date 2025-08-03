import logging

from fsgui.font import Font
from fsgui.types import Colour, Size
from fsgui.widget import Widget
from fsgui.widgetpainter import WidgetPainter

logger = logging.getLogger(__name__)


class BaseSlider(Widget):
    def __init__(
        self,
        *,
        font: Font | None = None,
        parent: Widget | None = None,
        size: Size | None = None,
        wrap: bool = False,
    ) -> None:
        super().__init__(font=font, parent=parent, size=size)
        # self.text = "DUMMY"
        # self.wrap = wrap
        self.handle_width = 17
        self.handle_height = 28

        self._handle_rect = (0, 0, 0, 0)
        self._handle_tracking = False

        self.stops: list[int] = []

        self.min = 0
        self.max = 0

        self.steps = 0
        self.value = 0

        self.checked = False
        # FIXME
        # self.enabled = False

        self.enable_hover_state()
        self.enable_pressed_state()

    def calculate_min_height(self, width: int) -> int:
        return 28

    def calculate_min_width(self) -> int:
        # FIXME:
        return 260

    def on_change(self) -> None:
        """Event handler for value being changed."""
        pass

    def on_left_press(self) -> None:
        if not self.enabled:
            return
        x, y = self.get_mouse_position()
        rect = self._handle_rect
        if x < rect[0]:
            if self.value > self.min:
                self.set_value(self.value - 1)
        elif x >= rect[0] + rect[2]:
            if self.value < self.max:
                self.set_value(self.value + 1)
        elif rect[1] <= y < rect[1] + rect[3]:
            logger.info("handle tracking!!")
            self._handle_tracking = True

    def on_left_release(self) -> None:
        if self._handle_tracking:
            self._handle_tracking = False

    def on_mouse_motion(self) -> None:
        if self._handle_tracking:
            x, y = self.get_mouse_position()
            if x < 0:
                value = self.min
            elif x > self.width:
                value = self.max
            else:
                value = self.min + round(
                    (self.max - self.min) * x / self.width
                )
            # print(x, y, value)
            self.set_value(value)

    # FIXME: Maybe on_mouse_tracking instead?
    def on_mouse_tracking(self) -> None:
        pass

    def on_paint(self) -> None:
        dc = self.create_dc()
        painter = WidgetPainter(self, dc)

        slider_area_height = 8
        y = (self.height - slider_area_height) // 2

        if self.enabled:
            dc.set_pen_colour(Colour.GREY_AA)
            dc.set_fill_colour(Colour.WHITE)
        else:
            dc.set_pen_colour(Colour.GREY_BB)
            dc.set_fill_colour(Colour.GREY_EE)

        # FIXME: draw_filled_rectangle_with_outline
        # dc.draw_filled_rectangle_with_outline((0, y), (self.width, hei))
        painter.paint_box((0, y), (self.width, slider_area_height))

        # dc.set_fill_color(Colour.WHITE)
        if self.enabled:
            dc.set_pen_colour(Colour.GREY_99)
        else:
            dc.set_pen_colour(Colour.GREY_BB)

        for x in self.stops:
            dc.draw_line((x, 0), (x, 4))
            dc.draw_line((x, self.height - 5), (x, self.height - 1))

        # self.paint_handle(dc, 0, 0)
        painter.paint_button(
            (self._handle_rect[0], self._handle_rect[1]),
            (self._handle_rect[2], self._handle_rect[3]),
        )

    def on_resize(self) -> None:
        self._update_positions()

    # def paint_handle(self, dc: DrawingContext, x: int, y: int) -> None:
    #     # FIXME: self.size?
    #     w, h = self.get_size()
    #     # dc.set_color((0, 0, 0))

    #     pressed = self.is_mouse_pressed()
    #     hover = self.is_mouse_hovering()
    #     # FIXME
    #     # enabled = False
    #     # print("pressed", pressed)

    #     if self.enabled:
    #         if hover:
    #             if pressed:
    #                 fill_colour = Colour.GREY_CC
    #             else:
    #                 fill_colour = Colour.WHITE
    #         else:
    #             fill_colour = Colour.GREY_EE
    #     else:
    #         fill_colour = Colour.GREY_DD

    #     x1 = self._handle_rect[0]
    #     y1 = self._handle_rect[1]
    #     x2 = self._handle_rect[0] + self._handle_rect[2] - 1
    #     y2 = self._handle_rect[1] + self._handle_rect[3] - 1

    #     dc.set_fill_colour(fill_colour)
    #     dc.draw_filled_rectangle(
    #         (x1, y1), (self.handle_width, self.handle_height)
    #     )

    #     if self.enabled:
    #         dc.set_pen_colour(Colour.GREY_AA)
    #     else:
    #         dc.set_pen_colour(Colour.GREY_BB)
    #     dc.draw_line((x1, y1), (x2, y1))
    #     dc.draw_line((x2, y1), (x2, y2))
    #     dc.draw_line((x1, y2), (x2, y2))
    #     dc.draw_line((x1, y1), (x1, y2))

    #     # Drawing a square for now. Maybe draw a proper checkmark later
    #     # (however, the square is more visible though...)
    #     if self.enabled:
    #         fill_colour = Colour.GREY_33
    #     else:
    #         # FIXME: Check if this looks nice
    #         fill_colour = Colour.GREY_AA
    #     if self.checked:
    #         p = 3
    #         dc.set_fill_colour(fill_colour)
    #         dc.draw_filled_rectangle(
    #             (x1 + p, y1 + p),
    #             (self.handle_width - 2 * p, self.handle_width - 2 * p),
    #         )

    def set_value(self, value: int) -> None:
        if value == self.value:
            return
        self.value = value
        self._update_handle_rect()
        self.on_change()
        self.refresh()

    def _update_positions(self) -> None:
        self._update_handle_rect()

        self.stops.clear()
        padd = self.handle_width // 2
        pos_x: float = padd
        width = self.width - 2 * padd
        if self.steps > 1:
            step_width = width / (self.steps - 1)
            for _ in range(self.steps):
                pos_x_int = int(round(pos_x))
                self.stops.append(pos_x_int)

                # FIXME: Check if the last mark is in exactly the right
                # position and not off by one or something like that...
                pos_x += step_width

    def _update_handle_rect(self) -> None:
        w, h = self.get_size()
        x = 0
        y = (h - self.handle_height) // 2

        if self.max - self.min == 0:
            x = 0
        else:
            x = int(
                round(
                    (
                        (w - self.handle_width)
                        * (self.value - self.min)
                        / (self.max - self.min)
                    )
                )
            )

        self._handle_rect = (x, y, self.handle_width, self.handle_height)


class DiscreteSlider(BaseSlider):
    def __init__(self, steps: int) -> None:
        super().__init__()
        self.steps = steps
        self.min = 1
        self.max = steps
        self.value = self.min
