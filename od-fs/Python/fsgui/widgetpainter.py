import math

from .drawingcontext import DrawingContext
from .image import Image
from .item import Item
from .types import Colour, PixelRect, Position, Size
from .widget import Widget


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
        cls._menu_arrow_disabled = Image.from_resource("widgets/choice-arrow-disabled.png")
        cls._button = Image.from_resource("widgets/button.9p.png")
        cls._button_disabled = Image.from_resource("widgets/button_disabled.9p.png")
        cls._button_pressed = Image.from_resource("widgets/button-pressed.9p.png")

    def __init__(self, widget: Widget | None, dc: DrawingContext | None = None) -> None:
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

        # if self.dc is None:
        #     raise Exception("dc cannot be None")

    def paint_box(self, position: Position, size: Size):
        """A white box with an outline, a shade inside its top and left edges and softened corners,
        each a line of one design unit, drawn in pixels."""
        dc = self.dc
        x1, y1, width, height = dc.rect_to_view_pixels(position, size)
        unit = dc.design_line_to_pixels(1)
        # Where the right and bottom edges' last lines start.
        x2 = x1 + width - unit
        y2 = y1 + height - unit
        pixels = dc.pixel_view

        # FIXME: FROM THEME
        if self.enabled:
            dc.set_text_colour(Colour.BLACK)
            dc.set_pen_colour(Colour.GREY_AA)
            dc.set_fill_colour(Colour.WHITE)
        else:
            dc.set_text_colour(Colour.GREY_88)
            dc.set_pen_colour(Colour.GREY_BB)
            dc.set_fill_colour(Colour.GREY_EE)

        pixels.draw_filled_rectangle_with_outline((x1, y1), (width, height), unit)

        if self.enabled:
            # Add a subtle shade
            dc.set_fill_colour((0xF2, 0xF2, 0xF2, 0xFF))
            pixels.draw_filled_rectangle((x1 + unit, y1 + unit), (width - 2 * unit, unit))
            pixels.draw_filled_rectangle((x1 + unit, y1 + 2 * unit), (unit, height - 3 * unit))

            dc.set_fill_colour((0xF6, 0xF6, 0xF6, 0xFF))
            pixels.draw_filled_rectangle((x1 + 2 * unit, y1 + 2 * unit), (width - 3 * unit, unit))
            pixels.draw_filled_rectangle((x1 + 2 * unit, y1 + 3 * unit), (unit, height - 4 * unit))

        # Add corners
        dc.set_fill_colour((0xD2, 0xD2, 0xD2, 0xFF))
        for point in [(x1, y1), (x2, y1), (x1, y2), (x2, y2)]:
            pixels.draw_filled_rectangle(point, (unit, unit))

        # Add corners 2
        dc.set_fill_colour((0xCC, 0xCC, 0xCC, 0xFF))
        for point in [
            (x1 + unit, y1 + unit),
            (x2 - unit, y1 + unit),
            (x1 + unit, y2 - unit),
            (x2 - unit, y2 - unit),
        ]:
            pixels.draw_filled_rectangle(point, (unit, unit))

    @staticmethod
    def get_item_height() -> float:
        """The height of an item in a list view or a choice's menu: a choice's height inside its
        outline."""
        choice_height = 28
        outline = 1
        return choice_height - 2 * outline

    def paint_items(
        self,
        items: list[Item],
        size: Size,
        padding: tuple[float, float, float, float],
        selected_index: int | None,
    ) -> None:
        """A list view's or a choice menu's items in a white box of size with an outline, inside the
        padding (top, right, bottom, left), which is the outline's unit. An item's fill spans the
        box inside the outline in pixels, and reaches the outline above the first item and below an
        item that ends at the padding, so that a highlighted item meets the outline on every
        side."""
        dc = self.dc
        dc.set_pen_colour(Colour.GREY_AA)
        dc.set_fill_colour(Colour.WHITE)
        dc.draw_filled_rectangle_with_outline((0, 0), size)

        top, right, bottom, left = padding
        line = dc.design_line_to_pixels(1)
        _, _, box_width, box_height = dc.rect_to_view_pixels((0, 0), size)
        width = size[0] - left - right
        height = self.get_item_height()
        y = top
        for i, item in enumerate(items):
            _, fill_top, _, fill_height = dc.rect_to_view_pixels((left, y), (width, height))
            fill_bottom = fill_top + fill_height
            if i == 0:
                fill_top = line
            if math.isclose(y + height, size[1] - bottom):
                fill_bottom = box_height - line
            fill = (line, fill_top, box_width - 2 * line, fill_bottom - fill_top)
            self._paint_item(item, (left, y), (width, height), i == selected_index, fill)
            y += height

    def _paint_item(
        self, item: Item, position: Position, size: Size, selected: bool, fill: PixelRect
    ) -> None:
        """An item of a list view or a choice's menu, its background filling fill (in the pixel
        view). Its text is where a choice's text is (6 design units in, less the outline)."""
        dc = self.dc
        if selected:
            fill_colour = (0x66, 0x88, 0xBB, 0xFF)
            text_colour = Colour.WHITE
        elif item.enabled:
            fill_colour = Colour.WHITE
            text_colour = Colour.GREY_22
        else:
            fill_colour = Colour.WHITE
            text_colour = Colour.GREY_99

        dc.set_text_colour(text_colour)
        dc.set_fill_colour(fill_colour)
        dc.pixel_view.draw_filled_rectangle(fill[:2], fill[2:])
        # FIXME: Get 6 from theme
        text_x = position[0] + 6 - 1
        dc.draw_text_centred(
            item.text, (text_x, position[1]), (position[0] + size[0] - text_x, size[1])
        )

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
        self.dc.draw_image_scaled(icon, (x, y))
