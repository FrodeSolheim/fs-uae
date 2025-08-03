from typing import Any, Self, Sequence, Union

from fsgui.borderlesswindow import BorderlessWindow
from fsgui.drawingcontext import Colour, DrawingContext
from fsgui.font import Font
from fsgui.transparentwindow import TransparentWindow
from fsgui.types import Position, Size
from fsgui.widget import Widget
from fsgui.widgetpainter import WidgetPainter
from fsuae.fsuaemainwindow import MainWindow


class Choice(Widget):
    def __init__(
        self,
        items: Sequence[Union["MenuItem", str]],
        *,
        font: Font | None = None,
        parent: Widget | None = None,
        size: Size | None = None,
        wrap: bool = False,
    ) -> None:
        super().__init__(font=font, parent=parent, size=size)

        # self.index: int | None = 0
        self.index: int = 0
        self.items: list[MenuItem] = []

        for i, item in enumerate(items):
            if not isinstance(item, MenuItem):
                item = MenuItem(item)
            item.index = i
            self.items.append(item)
        # self.items = [MenuItem(x) for x in items]
        self.button_size = (24, 28)
        # self.button = ChoiceButton()

        self.min_width = 0
        for item in self.items:
            tw, th = self.font.measure_text(item.text)
            mw = 1 + 6 + tw + 6 + self.button_size[0]
            if mw > self.min_width:
                self.min_width = mw

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

    def add_item(self, item: str) -> None:
        if not isinstance(item, MenuItem):
            item = MenuItem(item)
        item.index = len(self.items)
        self.items.append(item)
        self.refresh()

    def calculate_min_height(self, width: int) -> int:
        return 28

    def calculate_min_width(self) -> int:
        # FIXME: This should be handled by Widget?
        if self._min_width is not None:
            return self._min_width

        # tw, _ = self.font.measure_text(self.text)
        # return self.check_width + self.gap + tw
        return self.min_width

    def find_index_by_data(self, data) -> int | None:
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
        dc.draw_point(button_pos)
        dc.draw_point((button_pos[0], self.height - 1))
        # dc.draw_line((button_pos), (button_pos[0], 0))

        # FIXME: can self.index be none?
        if len(self.items) > 0 and self.index is not None:
            text = self.items[self.index].text
        else:
            text = ""
        _, th = dc.measure_text(text)

        # FIXME: FROM THEME
        if self.enabled:
            dc.set_text_colour(Colour.GREY_22)
        else:
            dc.set_text_colour(Colour.GREY_88)

        # FIXME: 6 - variable from theme, same as TextField
        dc.draw_text(text, (6, (self.height - th) // 2))

    def _open_dropdown(self) -> None:
        def on_select():
            self.set_index(dropdown.index)

        dropdown = ChoiceMenu(self.items, self.index, on_select=on_select)
        dropdown.set_width(self.width)
        # dropdown.resize((300, 100))
        # window_pos = self.get_window().position
        x, y = self.get_absolute_position()
        dropdown_x = x
        dropdown_y = y + self.height

        # Subtract 1 to let the borders overlay
        dropdown_y -= 1

        main_size = MainWindow.get().get_size()
        if dropdown_y + dropdown.height > main_size[1]:
            dropdown_y = y - dropdown.height

            # Adding 1 to let the borders overlay
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


class MenuItem:
    def __init__(self, text: str, data: Any = None) -> None:
        self.index: int = 0
        self.text = text
        self.enabled = True
        self.data = data

    # def disable(self) -> None:
    #     self.enabled = False

    def set_enabled(self, enabled=True) -> Self:
        self.enabled = enabled
        return self


# FIXME: The transparent window should resize if the main window resizes?
# FIXME: Do not cover main window titlebar (???)
class ChoiceBackgroundWindow(TransparentWindow):
    def __init__(self, controlling_widget) -> None:
        size = MainWindow.get().get_size()
        super().__init__(size=size)
        self.controlling_widget = controlling_widget

        self.set_focusable(False)

    def _on_destroy(self) -> None:
        self.controlling_widget = None

    def on_left_press(self) -> None:
        self.controlling_widget.close()


class ChoiceMenu(BorderlessWindow):
    # FIXME: on_activate vs on_select
    # FIXME: callback type
    def __init__(self, items: list[MenuItem], index: int, on_select) -> None:
        size = 200, 100
        super().__init__(size=size)

        self.set_focusable(False)

        self.items = items  # .copy()

        self.index = index

        self._on_select = on_select
        self._temp_index = self.index

        # self.style.background_color = Colour.RED

        self.background_window = ChoiceBackgroundWindow(self)
        self.background_window.show()

        # FIXME: Free surfaces and textures

        self.item_height = 28 - 2

        # FIXME: Could use padding
        # self.border_width = 1
        self.padding = (1, 1, 1, 1)

        # FIXME: Maybe set width based on largest item
        # width = 320 + self.padding[1] + self.padding[3]
        width = 1
        height = (
            self.item_height * len(self.items)
            + self.padding[0]
            + self.padding[2]
        )
        self.resize((width, height))

        # ...?
        self.enable_mouse_motion_events()

        # Do not close on release the first time unless having hovered over an
        # item. This is to avoid closing the menu immediately after having
        # opened in (on mouse press).
        self._close_on_release = False

    # def close(self) -> None:
    #     # self.background_window.destroy()
    #     # self.destroy()
    #     # self.background_window.destroy()

    def get_index_at_position(self, position: Position) -> int | None:
        x, y = position
        if x < self.padding[3] or x >= self.width - self.padding[1]:
            return None
        if y < self.padding[0] or y >= self.height - self.padding[2]:
            return None
        return int(
            (
                (y - self.padding[0])
                / (self.height - self.padding[0] - self.padding[2])
            )
            * len(self.items)
        )

    def on_close(self) -> None:
        self.background_window.close()

    def _on_destroy(self) -> None:
        self._on_select = None

    # def on_left_press(self) -> None:
    #     self.close()

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

    def on_paint(self) -> None:
        dc = self.create_dc()
        # FIXME: Only draw border/outline! Items will be drawn over!
        dc.set_pen_colour(Colour.GREY_AA)
        dc.set_fill_colour(Colour.WHITE)
        dc.draw_filled_rectangle_with_outline((0, 0), self.size)

        x = self.padding[3]
        y = self.padding[0]
        w = self.width - self.padding[1] - self.padding[3]
        h = self.item_height

        for i, item in enumerate(self.items):
            self.paint_item(dc, item, (x, y), (w, h), i == self._temp_index)
            y += h

    def paint_item(
        self,
        dc: DrawingContext,
        item: MenuItem,
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
        tw, th = dc.measure_text(item.text)
        # FIXME: Get 6 from theme
        x = position[0] + 6 - 1
        y = position[1] + (size[1] - th) // 2
        dc.draw_text(item.text, (x, y))

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
