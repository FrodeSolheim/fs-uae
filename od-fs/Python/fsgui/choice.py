import logging
from typing import Any, Sequence, Union

from fsapp.surfaceapi.v1 import Navigation, NavigationEvent, Role

from .borderlesswindow import BorderlessWindow
from .drawingcontext import Colour
from .font import Font
from .item import Item
from .types import Position, Size
from .widget import Widget
from .widgetpainter import WidgetPainter
from .window import Window
from .windowmanager import get_client_size

logger = logging.getLogger(__name__)


class Choice(Widget):
    def __init__(
        self,
        items: Sequence[Union["Item", str]],
        *,
        font: Font | None = None,
        parent: Widget | None = None,
        size: Size | None = None,
        wrap: bool = False,
    ) -> None:
        super().__init__(font=font, parent=parent, size=size)

        # self.index: int | None = 0
        self.index: int = 0
        self.items: list[Item] = []

        for i, item in enumerate(items):
            if not isinstance(item, Item):
                item = Item(item)
            item.index = i
            self.items.append(item)
        # self.items = [Item(x) for x in items]
        # self.button = ChoiceButton()

        if len(items) > 0:
            self.index = 0

        # FIXME: Why incorrect place?
        # Button("Heisann").move((0, 0))
        # Label("Heisann og hoppsann").move((0, 0))
        # abel("Heisann og hoppsann").move((0, 0))

        # FIXME: Do we need to call this? Will on_resize be called at least once?
        # self._maybe_call_on_resize (storing last size?)
        # self._maybe_call_on_resize
        # -> self._on_resize (fsgui internal)
        # -> self._on_resize_function/callback (?)
        # -> self.on_resize

        # _on_resize_function overrides on_resize or the other way round?
        self.on_resize()

    # FIXME: Same as ListView?
    def add_item(self, item: Item | str) -> None:
        if not isinstance(item, Item):
            item = Item(item)
        item.index = len(self.items)
        self.items.append(item)
        self.refresh()

    def remove_item_by_index(self, index: int) -> None:
        del self.items[index]
        self.refresh()
        if self.index < index:
            # No need to do anything
            pass
        elif self.index > index:
            # Just silently decrement the index, and the selected item stays the same
            self.index -= 1
        else:
            # FIXME: What to do?
            # FIXME: WARNING: Not handled properly..!
            print('\n\nWARNING: SELECTED LIST VIEW ITEM REMOVED ("UNHANDLED")')
            # print("Setting index to 0 (no event sent) FIXME: Need better handling")
            # self.index = 0
            print("Selecting item 0")
            self.set_index(0)

    # FIXME: Same as ListView?
    def remove_item_by_data(self, data: Any) -> None:
        for i, item in enumerate(self.items):
            if item.data == data:
                self.remove_item_by_index(i)
                break
        else:
            logger.warning("remove_item_by_data - item not found")

    @property
    def button_size(self) -> Size:
        return 24, 28

    def calculate_min_height(self, width: float) -> float:
        return 28

    def calculate_min_width(self) -> float:
        # FIXME: This should be handled by Widget?
        if self._min_width is not None:
            return self._min_width

        # The outline, the text's margins and the button.
        min_width = 0.0
        for item in self.items:
            tw, _th = self.font.measure_text(item.text)
            min_width = max(min_width, 1 + 6 + 6 + 24 + tw)
        return min_width

    def find_index_by_data(self, data: Any) -> int | None:
        for item in self.items:
            if item.data == data:
                return item.index
        return None

    def on_left_press(self) -> None:
        if not self.enabled:
            return
        self._open_dropdown()

    def on_change(self) -> None:
        pass

    def on_paint(self) -> None:
        dc = self.create_dc()
        painter = WidgetPainter(self, dc)

        button_pos = (self.width - self.button_size[0], 0)

        # FIXME: Pressed state

        # A bit inefficient to draw the box behind the button. Oh well.
        painter.paint_box((0, 0), self.size)
        painter.paint_button(button_pos, self.button_size)
        painter.paint_menu_arrow(button_pos, self.button_size)

        # FIXME: From Theme
        if self.enabled:
            dc.set_pen_colour(Colour.GREY_AA)
        else:
            dc.set_pen_colour(Colour.GREY_BB)
        # Overriding button corner
        button_x, button_y, _, button_height = dc.rect_to_view_pixels(button_pos, self.button_size)
        unit = dc.design_line_to_pixels(1)
        dc.set_fill_colour(dc.pen_colour)
        pixels = dc.pixel_view
        pixels.draw_filled_rectangle((button_x, button_y), (unit, unit))
        pixels.draw_filled_rectangle((button_x, button_y + button_height - unit), (unit, unit))
        # dc.draw_line((button_pos), (button_pos[0], 0))

        # FIXME: can self.index be none?
        if len(self.items) > 0 and self.index is not None:
            text = self.items[self.index].text
        else:
            text = ""
        # FIXME: FROM THEME
        if self.enabled:
            dc.set_text_colour(Colour.GREY_22)
        else:
            dc.set_text_colour(Colour.GREY_88)

        # FIXME: 6 - variable from theme, same as TextField
        dc.draw_text_centred(text, (6, 0), (self.width - 6, self.height))

    def _open_dropdown(self) -> None:
        def on_select():
            self.set_index(dropdown.index)

        dropdown = ChoiceMenu(self.get_window(), self.items, self.index, on_select=on_select)
        dropdown.set_width(self.width)
        # dropdown.resize((300, 100))
        # window_pos = self.get_window().position
        x, y = self.get_absolute_position()
        dropdown_x = x
        dropdown_y = y + self.height

        # Subtract the outline to let the borders overlay
        dropdown_y -= 1

        main_size = get_client_size()
        if dropdown_y + dropdown.height > main_size[1]:
            dropdown_y = y - dropdown.height

            # Adding the outline to let the borders overlay
            dropdown_y += 1

        dropdown.move((dropdown_x, dropdown_y))
        dropdown.show()
        self.transfer_mouse_tracking(dropdown)

    def set_index(self, index: int) -> None:
        if index == self.index:
            return
        self.index = index
        self.on_change()
        self.refresh()
        # FIXME: on_select


class ChoiceMenu(BorderlessWindow):
    """A choice's menu: a popup of the choice's window, so that fsapp makes it the key target while
    it is open and tells it of a press outside it (OutsideClick), which closes it."""

    # FIXME: on_activate vs on_select
    # FIXME: callback type
    def __init__(self, parent: Window, items: list[Item], index: int, on_select) -> None:
        size = 200, 100
        super().__init__(size=size, role=Role.POPUP, parent=parent)

        self.set_focusable(False)

        self.items = items  # .copy()

        self.index = index

        self._on_select = on_select
        self._temp_index = self.index

        # self.style.background_color = Colour.RED

        # FIXME: Free surfaces and textures

        self.item_height = WidgetPainter.get_item_height()

        # FIXME: Could use padding
        # self.border_width = 1
        self.padding = (1, 1, 1, 1)

        # FIXME: Maybe set width based on largest item
        # width = 320 + self.padding[1] + self.padding[3]
        width = 1
        height = self.item_height * len(self.items) + self.padding[0] + self.padding[2]
        self.resize((width, height))

        # ...?
        self.enable_mouse_motion_events()

        # Do not close on release the first time unless having hovered over an
        # item. This is to avoid closing the menu immediately after having
        # opened in (on mouse press).
        self._close_on_release = False

    def get_index_at_position(self, position: Position) -> int | None:
        x, y = position
        if x < self.padding[3] or x >= self.width - self.padding[1]:
            return None
        if y < self.padding[0] or y >= self.height - self.padding[2]:
            return None
        return int(
            ((y - self.padding[0]) / (self.height - self.padding[0] - self.padding[2]))
            * len(self.items)
        )

    def on_left_press(self) -> None:
        # A press on the open menu itself (not the one that opened it, handed over by the choice).
        self._close_on_release = True

    def on_left_release(self) -> None:
        index = self.get_index_at_position(self.get_mouse_position())
        if index is not None:
            if self.items[index].enabled:
                self.index = index
                self._on_select()
        if self._close_on_release:
            self.close()

    def on_mouse_motion(self):
        index = self.get_index_at_position(self.get_mouse_position())
        if index is not None:
            self._close_on_release = True
            if index != self._temp_index:
                if self.items[index].enabled:
                    self.set_temp_index(index)

    def on_outside_click(self) -> None:
        self.close()

    def on_navigation(self, event: NavigationEvent) -> None:
        match event.value:
            case Navigation.UP:
                self._move_temp_index(-1)
            case Navigation.DOWN:
                self._move_temp_index(1)
            case Navigation.ACCEPT:
                if self.items[self._temp_index].enabled:
                    self.index = self._temp_index
                    self._on_select()
                self.close()
            case Navigation.BACK:
                self.close()
            case _:
                pass

    def _move_temp_index(self, step: int) -> None:
        """Highlights the next enabled item up (step -1) or down (step 1), if there is one."""
        index = self._temp_index + step
        while 0 <= index < len(self.items):
            if self.items[index].enabled:
                self.set_temp_index(index)
                return
            index += step

    def _on_destroy(self) -> None:
        self._on_select = None

    def on_paint(self) -> None:
        dc = self.create_dc()
        painter = WidgetPainter(self, dc)
        painter.paint_items(self.items, self.size, self.padding, self._temp_index)

    def set_index(self, index: int) -> None:
        if index == self.index:
            return
        self.index = index

    # FIXME: Rename "temp_index" to "something_index"
    def set_temp_index(self, index: int) -> None:
        if index == self._temp_index:
            return
        self._temp_index = index
        self.refresh()


# class ChoiceButton(CustomButton):
#     def __init__(self) -> None:
#         super().__init__()

#     # # FIXME: Should not be necessary with explicit size
#     # def calculate_min_height(self, width: int) -> int:
#     #     return 28

#     # # FIXME: Should not be necessary with explicit size
#     # def calculate_min_width(self) -> int:
#     #     return 28

#     def on_paint(self) -> None:
#         dc = self.create_dc()

#         # FIXME: FROM THEME
#         dc.set_pen_colour(Colour.GREY_AA)
#         dc.set_fill_colour(Colour.GREY_EE)

#         dc.draw_filled_rectangle_with_outline((0, 0), self.size)
#         dc.draw_filled_rectangle_with_outline((0, 0), (28, 28))
