import logging
from typing import Any

from fsgui.drawingcontext import Colour, DrawingContext
from fsgui.item import Item
from fsgui.types import Position, Size
from fsgui.widget import Widget
from fsgui.widgetpainter import WidgetPainter

logger = logging.getLogger(__name__)

# FIXME: ListView and Choice are maybe WidgetWithItems and can use common methods to manage
# the list of items?


class ListView(Widget):
    def __init__(
        self,
        *,
        parent: Widget | None = None,
        size: Size | None = None,
    ):
        super().__init__(parent=parent, size=size)

        # Add padding to account for the drawn border
        self.padding = (1, 1, 1, 1)

        self.index: int | None = 0
        self.items: list[Item] = []
        self.item_height = 28 - 2

    # FIXME: Same as Choice
    def add_item(self, item: Item | str) -> None:
        if not isinstance(item, Item):
            item = Item(item)
        item.index = len(self.items)
        self.items.append(item)
        self.refresh()

    # FIXME: Same as Choice
    def remove_item_by_data(self, data: Any) -> None:
        for i, item in enumerate(self.items):
            if item.data == data:
                del self.items[i]
                break
        else:
            logger.warning("remove_item_by_data - item not found")

    def on_erase(self):
        pass

    def on_paint(self):
        # dc = DrawingContext(self)
        dc = self.create_dc()
        painter = WidgetPainter(self, dc)
        painter.paint_box((0, 0), self.size)

        # # A bit lighter text color on white background # FIXME: From theme?
        # if self.enabled:
        #     dc.set_text_colour(Colour.GREY_22)

        # for i, item in enumerate(self.items):

        # _, th = dc.measure_text(self._text)
        # # FIXME: 6 - variable from theme, same as TextField
        # dc.draw_text(self._text, (6, (self.height - th) // 2))

        # FIXME: Only draw border/outline! Items will be drawn over!
        dc.set_pen_colour(Colour.GREY_AA)
        dc.set_fill_colour(Colour.WHITE)
        dc.draw_filled_rectangle_with_outline((0, 0), self.size)

        x = self.padding[3]
        y = self.padding[0]
        w = self.width - self.padding[1] - self.padding[3]
        h = self.item_height

        for i, item in enumerate(self.items):
            self.paint_item(dc, item, (x, y), (w, h), i == self.index)
            y += h

    def paint_item(
        self,
        dc: DrawingContext,
        item: Item,
        position: Position,
        size: Size,
        selected: bool,
    ) -> None:
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
        dc.draw_filled_rectangle(position, size)
        _tw, th = dc.measure_text(item.text)
        # FIXME: Get 6 from theme
        x = position[0] + 6 - 1
        y = position[1] + (size[1] - th) // 2
        dc.draw_text(item.text, (x, y))

    def get_index_at_position(self, position: Position) -> int | None:
        x, y = position
        if x < self.padding[3] or x >= self.width - self.padding[1]:
            return None
        if y < self.padding[0] or y >= self.height - self.padding[2]:
            return None
        index = (int)((y - self.padding[0]) / self.item_height)
        if index >= len(self.items):
            return None
        return index

    def on_left_press(self) -> None:
        index = self.get_index_at_position(self.get_mouse_position())
        if index is not None:
            if self.items[index].enabled:
                self.index = index
                self.refresh()
                # self._on_select()

    def on_mouse_motion(self):
        index = self.get_index_at_position(self.get_mouse_position())
        if index is not None:
            if self.items[index].enabled:
                self.index = index
                self.refresh()
