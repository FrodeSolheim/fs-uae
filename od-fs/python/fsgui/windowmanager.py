import logging
from typing import Self

from fsgui.types import Position
from fsgui.widget import Widget
from fsgui.window import Window

logger = logging.getLogger(__name__)


class WindowManager:
    _instance: Self | None = None

    @classmethod
    def get(cls) -> "WindowManager":
        if cls._instance is None:
            cls._instance = WindowManager()
        return cls._instance

    def __init__(self) -> None:
        self._windows: list[Window] = []
        self._windows_by_layer: dict[int, list[Window]] = {}
        self._mouse_entered: Widget | None = None
        # FIXME: Holding a reference here might not be ideal
        self._left_pressed_widget: Widget | None = None
        self.mouse_pos = (-1, -1)
        self.focused_window: Window | None = None

    def add_window(self, window: Window) -> None:
        self._windows_by_layer.setdefault(window.layer, []).append(window)
        self._update_window_list()

    def bring_window_to_front(self, window: Window) -> None:
        window_list = self._windows_by_layer[window.layer]
        if window != window_list[-1]:
            window_list.remove(window)
            window_list.append(window)
            window.on_bring_to_front()
        self._update_window_list()

    def _update_window_list(self) -> None:
        self._windows.clear()
        for layer in sorted(self._windows_by_layer.keys()):
            for window in self._windows_by_layer[layer]:
                self._windows.append(window)

    def find_widget_at_position(self, position: Position) -> Widget | None:
        window = WindowManager.get().find_window_at_position(position)
        if window is not None:
            window_position = window.get_window_position()
            relative_position = (
                position[0] - window_position[0],
                position[1] - window_position[1],
            )
            widget = window.find_widget_at_position(relative_position)
            if widget is not None:
                return widget
            return window
        return None

    def find_window_at_position(self, position: Position) -> Window | None:
        x, y = position
        for window in reversed(self._windows):
            # Invisible windows cannot be mouse events - which this function is
            # mainly used for. Maybe add a method parameter whether to filter
            # out invisible windows or not.
            if not window.visible:
                continue

            wx, wy = window.get_window_position()
            ww, wh = window.get_window_size()
            # print(x, y, wx, wy, ww, wh)
            if wx <= x < wx + ww and wy <= y < wy + wh:
                # print(window)
                return window
        return None

    def focus_window(self, window: Window) -> None:
        if self.focused_window:
            self.focused_window.focused = False
            self.focused_window.on_blur()

        self.focused_window = window
        self.focused_window.focused = True
        self.focused_window.on_focus()

        # Focusing a window implies bringing it to front?
        # self.bring_window_to_front(window)

    def get_mouse_position(self) -> Position:
        return self.mouse_pos

    def get_windows(self) -> list[Window]:
        return self._windows.copy()

    def handle_left_mouse_button(self, pressed: bool):
        widget: Widget | None
        window = self.find_window_at_position(self.mouse_pos)

        if pressed:
            if window is not None:
                self.bring_window_to_front(window)
                if window._focusable:
                    self.focus_window(window)

        if not pressed:
            if self._left_pressed_widget:
                widget = self._left_pressed_widget

                # FIXME: Maybe a better way to check hovering state?
                if widget == self.find_widget_at_position(self.mouse_pos):
                    print(widget)
                    widget.on_left_click()

                self._left_pressed_widget = None
                widget.on_left_release()
                # Must check if the mouse cursor is now hovering over another
                # widget!
                self.handle_mouse_moved_2(self.mouse_pos)
            return

        widget = self.find_widget_at_position(self.mouse_pos)
        if widget is not None:
            if pressed:
                self._left_pressed_widget = widget
                widget.on_left_press()
            # else:
            #     if widget == self._left_pressed_widget:
            #         widget.on_left_click()
            #     self._left_pressed_widget = None
            #     # FIXME: Only deliver a release event if the widget saw a press event?
            #     # (Do we want automatic "cursor grab" on mouse press ???)
            #     # widget.on_left_release()

            #     # Must check if the mouse cursor is now hovering over another
            #     # widget!
            #     self.handle_mouse_moved_2(self.mouse_pos)

    def handle_mouse_moved(self, mouse_pos: Position) -> None:
        self.mouse_pos = mouse_pos

        # When mouse is pressed, the widget received on_mouse_motion() events
        # automatically, also when the mouse moves outside of the widget
        # area? FIXME: hovering status...
        if self._left_pressed_widget:
            self._left_pressed_widget.on_mouse_motion()
        self.handle_mouse_moved_2(mouse_pos)

    def handle_mouse_moved_2(self, mouse_pos: Position) -> None:
        self.mouse_pos = mouse_pos

        # print("MOUSE MOVED: NEW POS", mouse_pos)
        widget = self.find_widget_at_position(self.mouse_pos)
        #  print(widget)

        if self._left_pressed_widget is not None:
            if widget != self._left_pressed_widget:
                widget = None

        if widget is not None:
            if widget != self._mouse_entered:
                if self._mouse_entered is not None:
                    self._mouse_entered.on_mouse_leave()
                self._mouse_entered = widget
                widget.on_mouse_enter()
            if widget._mouse_motion_events:
                widget.on_mouse_motion()
        else:
            if self._mouse_entered is not None:
                widget = self._mouse_entered
                self._mouse_entered = None
                # The if check is not necesary, widget was self._mouse_entered
                # we've checked that is was not None, but mypy does not
                # understand this
                if widget is not None:
                    widget.on_mouse_leave()

    def on_widget_destroyed(self, widget: Widget) -> None:
        if self._left_pressed_widget == widget:
            self._left_pressed_widget = None

        # FIXME: For later - maybe the destroyed widget has exposed a
        # widget beneath, which should get mouse entered state?
        if self._mouse_entered == widget:
            self._mouse_entered = None

        if widget == self.focused_window:
            self.focused_window = None
            logger.warning("FIXME: Refocus another/last window")

    def remove_window(self, window: Window) -> None:
        self._windows_by_layer[window.layer].remove(window)
        self._windows.remove(window)

    def transfer_mouse_tracking(self, from_widget: Widget, to_widget: Widget) -> None:
        if self._left_pressed_widget != from_widget:
            logger.warning("transfer_mouse_tracking on incorrect widget?")
            return

        self._left_pressed_widget = to_widget

    def _change_window_layer(self, window: Window, old_layer: int, new_layer: int) -> None:
        self._windows_by_layer[old_layer].remove(window)
        self._windows_by_layer.setdefault(new_layer, []).append(window)
        self._update_window_list()
