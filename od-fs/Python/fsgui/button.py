from typing import Any, Callable, Self

from fsgui.types import Size
from fsgui.widget import Widget
from fsgui.widgetpainter import WidgetPainter


class BaseButton(Widget):
    def __init__(
        self,
        *,
        on_activate: Any = None,
        parent: Widget | None = None,
        size: Size | None = None,
    ) -> None:
        super().__init__(parent=parent, size=size)
        self._pressed = False
        self._hover = False

        self.__on_activate = on_activate

        self.enable_hover_state()
        self.enable_pressed_state()

    # # FIXME: better name
    # def is_hover(self):
    #     return self._hover

    # Not sure if this one is needed...
    # Use is_mouse_pressed() instead?
    def is_pressed(self) -> bool:
        return self.is_mouse_pressed()

    def on_activate(self, on_activate: Callable[[], None]) -> Self:
        self._handlers.setdefault("on_activate", []).append(on_activate)
        return self

    def _on_activate(self) -> None:
        self._call_handlers("on_activate")
        if self.__on_activate:
            self.__on_activate()

    # def on_left_press(self):
    #     self._pressed = True

    #     # FIXME: Actually: button needs mouse grab support (explicit or
    #     # implicit) in order to reset pressed state...

    def on_left_click(self) -> None:
        if not self.enabled:
            return
        self._on_activate()

    @property
    def pressed(self) -> bool:
        return self.is_mouse_pressed()

    # def on_left_release(self):
    #     self._pressed = False


class CustomButton(BaseButton):
    pass


class Button(BaseButton):
    def __init__(
        self,
        text: str,
        *,
        on_activate: Callable[[], None] | None = None,
        parent: Widget | None = None,
        size: Size | None = None,
    ) -> None:
        super().__init__(on_activate=on_activate, parent=parent, size=size)
        self.padding = (0, 16, 0, 16)

        self.text = text

        # # FIXME: Only load image one time..! -Not for each button
        # # style manager / style service?
        # # or cache in static class methods

        # self.normal_image = Image.from_resource("widgets/button.9p.png")
        # self.pressed_image = Image.from_resource(
        #     "widgets/button-pressed.9p.png"
        # )

    def calculate_min_height(self, width: int) -> int:
        return 28

    def calculate_min_width(self) -> int:
        tw, _ = self.font.measure_text(self.text)
        return tw + self.padding[1] + self.padding[3]

    # def get_text(self) -> str:
    #     return self.text

    def on_paint(self) -> None:
        dc = self.create_dc()
        painter = WidgetPainter(self, dc)
        painter.paint_button((0, 0), self.size)

        # pressed = self.is_pressed()
        # hover = self.is_mouse_hovering()

        # if self.enabled:
        #     if hover:
        #         if pressed:
        #             image = self.pressed_image
        #         else:
        #             # FIXME
        #             image = self.normal_image
        #     else:
        #         image = self.normal_image
        # else:
        #     # FIXME: disabled_image
        #     image = self.normal_image

        # w, h = self.get_size()
        # dc.draw_image_stretched(image, (0, 0), (w, h))

        available = self.width - self.padding[1] - self.padding[3]

        tw, th = dc.measure_text(self.text)

        # dc.draw_text(self._text, ((ww - tw) // 2, 6))
        dc.draw_text(
            self.text,
            (self.padding[3] + (available - tw) // 2, (self.height - th) // 2),
        )

    def set_text(self, text: str) -> None:
        self.text = text
        self.refresh()
