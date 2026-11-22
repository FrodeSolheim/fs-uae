import logging

from fsapp.surfaceapi.v1 import MouseButton

from .scale import get_scale, position_from_pixels, set_scale
from .signal import Signal
from .types import PointerPosition, Position, Size
from .widget import Widget
from .window import Window

logger = logging.getLogger(__name__)

# The client area in the host window, in design units: the windows are placed relative to its
# origin, and centred in its size.
_client_origin: Position = (0, 0)
_client_size: Size = (0, 0)


def set_client_origin(origin: Position) -> None:
    """Sets where the client area starts in the host window, in design units (the top's chrome,
    such as a title bar, is outside it): fsgui places its windows relative to it."""
    global _client_origin
    _client_origin = origin


def get_client_origin() -> Position:
    return _client_origin


# REVIEW: The top sets the client area's size as well as its origin, for centred windows and a
# choice's menu.
def set_client_size(size: Size) -> None:
    """Sets the client area's size, in design units: centred windows are centred in it, and a
    choice's menu opens upwards where it would not fit below."""
    global _client_size
    _client_size = size


def get_client_size() -> Size:
    return _client_size


class WindowManager:
    """The toolkit's side of fsgui's windows: the widget pressed and the widget hovered (across
    windows, since a press can be handed to a widget in another window), the focused window's look,
    and the pointer's position in the main window's client area. fsapp stacks the windows, hit
    tests between them and raises and focuses on a press; each window gets its own pointer events
    (Window._handle_surface_event) and hands them here with the position in design units relative to
    the window."""

    _instance: "WindowManager | None" = None

    @classmethod
    def get(cls) -> "WindowManager":
        if cls._instance is None:
            cls._instance = WindowManager()
        return cls._instance

    def __init__(self) -> None:
        # In the order they were created.
        self._windows: list[Window] = []
        self._mouse_entered: Widget | None = None
        # FIXME: Holding a reference here might not be ideal
        self._left_pressed_widget: Widget | None = None
        self._right_pressed_widget: Widget | None = None
        # The click count of the last press (PointerButton.clicks).
        self._click_count = 0
        self.mouse_pos: PointerPosition = (-1.0, -1.0)
        self.focused_window: Window | None = None
        # Emitted with the new mouse position (as get_mouse_position) when the mouse has moved.
        self.mouse_moved = Signal[[PointerPosition]]()

    def add_window(self, window: Window) -> None:
        self._windows.append(window)

    def bring_window_to_front(self, window: Window) -> None:
        """Asks fsapp to raise the window in front of the others in its band (or of the
        overlays)."""
        if window.surface is not None:
            window.surface.raise_surface()
        window.on_bring_to_front()

    def focus_window(self, window: Window) -> None:
        """The window has got keyboard focus (FocusIn): fsapp decides it, on the window's request
        (Window.focus) or on a press. The focus changes how a window draws its border and title
        bar."""
        if self.focused_window is window:
            return
        if self.focused_window:
            self.unfocus_window(self.focused_window)

        self.focused_window = window
        self.focused_window.focused = True
        self.focused_window.on_focus()
        self.focused_window.refresh()

    def unfocus_window(self, window: Window) -> None:
        """The window has lost keyboard focus (FocusOut)."""
        if self.focused_window is window:
            self.focused_window = None
        if window.focused:
            window.focused = False
            window.on_blur()
            window.refresh()

    def get_click_count(self) -> int:
        """The click count of the last press: 1 for a single click, 2 for a double click."""
        return self._click_count

    def get_mouse_position(self) -> PointerPosition:
        """The mouse pointer's position relative to the main window's client area (design units),
        from the host window's pointer motion."""
        return self.mouse_pos

    def get_windows(self) -> list[Window]:
        return self._windows.copy()

    def handle_host_pointer_motion(self, pixel_pos: PointerPosition) -> None:
        """The pointer has moved to pixel_pos, in pixels relative to the main window's client area
        (the host window's pointer motion, wherever the pointer is)."""
        self.mouse_pos = position_from_pixels(pixel_pos, get_scale())
        self.mouse_moved.emit(self.get_mouse_position())

    def _find_widget(self, window: Window, position: PointerPosition) -> Widget | None:
        """The widget at position (design units relative to window) in the window, the window
        itself where it has none, or None outside it. While a press is handed to a widget in
        another window (transfer_mouse_tracking), that window is searched instead, at the main
        window's pointer position."""
        pressed = self._left_pressed_widget
        if pressed is not None and pressed.get_window() is not window:
            window = pressed.get_window()
            window_position = window.get_window_position()
            position = (
                self.mouse_pos[0] - window_position[0],
                self.mouse_pos[1] - window_position[1],
            )
        widget = window.find_widget_at_position(position)
        if widget is not None:
            return widget
        width, height = window.get_window_size()
        if 0 <= position[0] < width and 0 <= position[1] < height:
            return window
        return None

    def handle_pointer_motion(self, window: Window, position: PointerPosition) -> None:
        """The pointer has entered window or moved in it (or, during a drag that started in it,
        anywhere) to position, in design units relative to the window."""
        # The pressed widget gets motion wherever the pointer is, until the release.
        if self._left_pressed_widget:
            self._left_pressed_widget.on_mouse_motion()
        self._update_hover(window, position)

    def handle_pointer_leave(self, window: Window) -> None:
        """The pointer has left window: its hovered widget is no longer hovered."""
        widget = self._mouse_entered
        if widget is not None and widget.get_window() is window:
            self._mouse_entered = None
            widget.on_mouse_leave()

    def handle_pointer_button(
        self, window: Window, position: PointerPosition, button: int, down: bool, clicks: int
    ) -> None:
        """A button pressed or released at position, in design units relative to window: the left
        button presses, clicks and releases a widget, the right one has hooks of its own."""
        if button == MouseButton.LEFT:
            if down:
                self._click_count = clicks
                widget = self._find_widget(window, position)
                if widget is not None:
                    self._left_pressed_widget = widget
                    widget.on_left_press()
            elif self._left_pressed_widget is not None:
                widget = self._left_pressed_widget
                if widget == self._find_widget(window, position):
                    widget.on_left_click()
                self._left_pressed_widget = None
                widget.on_left_release()
                # The pointer may now be over another widget (unless the release closed the window).
                if not window.is_destroyed():
                    self._update_hover(window, position)
        elif button == MouseButton.RIGHT:
            if down:
                self._click_count = clicks
                self._right_pressed_widget = self._find_widget(window, position)
                if self._right_pressed_widget is not None:
                    self._right_pressed_widget.on_right_press()
            elif self._right_pressed_widget is not None:
                widget = self._right_pressed_widget
                self._right_pressed_widget = None
                widget.on_right_release()

    def handle_pointer_wheel(
        self, window: Window, position: PointerPosition, dx: float, dy: float
    ) -> None:
        """The wheel turned with the pointer at position, in design units relative to window."""
        widget = self._find_widget(window, position)
        if widget is not None:
            widget.on_wheel(dx, dy)

    def _update_hover(self, window: Window, position: PointerPosition) -> None:
        widget = self._find_widget(window, position)
        # While a widget is pressed, only it can be hovered.
        if self._left_pressed_widget is not None and widget != self._left_pressed_widget:
            widget = None

        if widget is not None:
            if widget != self._mouse_entered:
                if self._mouse_entered is not None:
                    self._mouse_entered.on_mouse_leave()
                self._mouse_entered = widget
                widget.on_mouse_enter()
            if widget._mouse_motion_events:
                widget.on_mouse_motion()
        elif self._mouse_entered is not None:
            entered = self._mouse_entered
            self._mouse_entered = None
            entered.on_mouse_leave()

    def on_widget_destroyed(self, widget: Widget) -> None:
        if self._left_pressed_widget == widget:
            self._left_pressed_widget = None
        if self._right_pressed_widget == widget:
            self._right_pressed_widget = None

        # FIXME: For later - maybe the destroyed widget has exposed a
        # widget beneath, which should get mouse entered state?
        if self._mouse_entered == widget:
            self._mouse_entered = None

        if widget == self.focused_window:
            # fsapp moves keyboard focus to another surface, and that window gets FocusIn.
            self.focused_window = None

    # REVIEW: Every window's surface gets a configure with the scale; the first that brings a new
    # scale rescales every window, and the others find it unchanged.
    def handle_surface_scale(self, scale: float) -> None:
        """A window's surface was configured at scale, its surface scale (the display scale): fsgui
        draws at it, and a new scale rescales the windows."""
        if set_scale(scale):
            logger.info("Scale %g (surface scale %g)", get_scale(), scale)
            self.rescale_windows()

    def rescale_windows(self) -> None:
        """The scale has changed: every window takes its new size in pixels and is painted
        again."""
        for window in self._windows:
            window.rescale()

    def remove_window(self, window: Window) -> None:
        self._windows.remove(window)

    def transfer_mouse_tracking(self, from_widget: Widget, to_widget: Widget) -> None:
        """Hands the press of from_widget to to_widget, which may be in another window: it gets
        the motion and the release (fsapp's pointer focus stays on the window the press started
        in, which hands them on here)."""
        if self._left_pressed_widget != from_widget:
            logger.warning("transfer_mouse_tracking on incorrect widget?")
            return

        self._left_pressed_widget = to_widget
