from fsgui.button import Size
from fsgui.drawingcontext import DrawingContext
from fsgui.image import Image
from fsgui.types import Colour, Position
from fsgui.widget import Widget


class WidgetPainter:
    STATE_NORMAL = 0
    STATE_HOVER = 1
    STATE_PRESSED = 1

    _initialized = False
    _menu_arrow: Image
    _menu_arrow_disabled: Image
    _button: Image
    _button_pressed: Image
    _button_disabled: Image

    @classmethod
    def initialize_class(cls):
        if cls._initialized:
            return
        cls._initialized = True
        cls._menu_arrow = Image.from_resource("widgets/choice-arrow.png")
        cls._menu_arrow_disabled = Image.from_resource(
            "widgets/choice-arrow-disabled.png"
        )
        cls._button = Image.from_resource("widgets/button.9p.png")
        cls._button_disabled = Image.from_resource(
            "widgets/button_disabled.9p.png"
        )
        cls._button_pressed = Image.from_resource(
            "widgets/button-pressed.9p.png"
        )

    def __init__(
        self, widget: Widget | None, dc: DrawingContext | None = None
    ) -> None:
        self.initialize_class()

        if dc is not None:
            self.dc = dc
        elif widget:
            self.dc = widget.create_dc()
        else:
            raise Exception("dc cannot be None")

        if widget:
            self.enabled = widget.enabled
            self.pressed = widget.pressed
        else:
            self.enabled = True
            self.pressed = False

        if self.dc is None:
            raise Exception("dc cannot be None")

    def paint_box(self, position: Position, size: Size):
        dc = self.dc
        width, height = size
        x1, y1 = position
        x2 = x1 + width - 1
        y2 = y1 + height - 1

        # FIXME: FROM THEME
        if self.enabled:
            dc.set_text_colour(Colour.BLACK)
            dc.set_pen_colour(Colour.GREY_AA)
            dc.set_fill_colour(Colour.WHITE)
        else:
            dc.set_text_colour(Colour.GREY_88)
            dc.set_pen_colour(Colour.GREY_BB)
            dc.set_fill_colour(Colour.GREY_EE)

        dc.draw_filled_rectangle_with_outline(position, size)

        if self.enabled:
            # Add a subtle shade
            dc.set_pen_colour((0xF2, 0xF2, 0xF2, 0xFF))
            dc.draw_line((x1 + 1, y1 + 1), (x2 - 1, y1 + 1))
            dc.draw_line((x1 + 1, y1 + 2), (x1 + 1, y2 - 1))

            dc.set_pen_colour((0xF6, 0xF6, 0xF6, 0xFF))
            dc.draw_line((x1 + 2, y1 + 2), (x2 - 1, y1 + 2))
            dc.draw_line((x1 + 2, y1 + 3), (x1 + 2, y2 - 1))

        # Add corners
        dc.set_pen_colour((0xD2, 0xD2, 0xD2, 0xFF))
        for point in [(x1, y1), (x2, y1), (x1, y2), (x2, y2)]:
            dc.draw_line(point, point)

        # Add corners 2
        dc.set_pen_colour((0xCC, 0xCC, 0xCC, 0xFF))
        for point in [
            (x1 + 1, y1 + 1),
            (x2 - 1, y1 + 1),
            (x1 + 1, y2 - 1),
            (x2 - 1, y2 - 1),
        ]:
            dc.draw_line(point, point)

    def paint_button(self, position: Position, size: Size):
        if self.enabled:
            if self.pressed:
                image = self._button_pressed
            else:
                image = self._button
        else:
            image = self._button_disabled
        self.dc.draw_image_stretched(image, position, size)

    def paint_menu_arrow(self, position: Position, size: Size):
        if self.enabled:
            icon = self._menu_arrow
        else:
            icon = self._menu_arrow_disabled
        x = position[0] + (size[0] - icon.width) // 2
        y = position[1] + (size[1] - icon.height) // 2
        self.dc.draw_image(icon, (x, y))
