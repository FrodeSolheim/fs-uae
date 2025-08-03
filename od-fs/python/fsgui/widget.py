import logging
import traceback
from typing import Callable, Optional, Self

from fsgui.font import Font
from fsgui.internal.surface import Surface
from fsgui.parentstack import ParentStack
from fsgui.style import Style
from fsgui.types import PaddingParam, Position, Size, SizeParam
from fsgui.widgetorlayout import WidgetOrLayout

logger = logging.getLogger(__name__)


class Widget(WidgetOrLayout):
    def __init__(
        self,
        *,
        font: Font | None = None,
        padding: PaddingParam = None,
        parent: Optional["Widget"] = None,
        size: SizeParam = None,
    ):
        from fsgui.layout import Layout
        from fsgui.window import Window

        self.style = Style()

        if font is not None:
            self.font = font
        else:
            self.font = Font("UI")

        self.enabled = True
        # self.pressed = False

        # self.parent = parent
        self._children: list[Widget] = []
        self._destroyed = False

        self.layout: Layout | None = None

        if padding is None:
            padding = (0, 0, 0, 0)
        elif isinstance(padding, int):
            padding = (padding, padding, padding, padding)
        self.padding: tuple[int, int, int, int] = padding

        self._pressed_state_enabled = False
        self._is_mouse_pressed = False
        self._refresh_on_pressed_state = False

        self._hover_state_enabled = False
        self._mouse_motion_events = False
        self._is_mouse_hovering = False
        self._refresh_on_hover_state = False

        # assume Window here?
        self._surface: Surface
        self._window: Window

        add_to_layout: Layout | None = None

        if parent is None:
            if isinstance(self, Window):
                pass
            else:
                # parent = ParentStack.parent_widget()
                parent = ParentStack.parent()
                logger.debug("PARENT %r", parent)
                if isinstance(parent, Layout):
                    add_to_layout = parent
                    while isinstance(parent, Layout):
                        parent = parent.parent
                        logger.debug("PARENT <- %r", parent)

        if add_to_layout is None:
            if parent is not None:
                add_to_layout = parent.layout

        # self.parent = parent
        super().__init__(parent=parent)

        if add_to_layout:
            add_to_layout.add(self)

        if parent is not None:
            self._surface = parent._surface
            self._window = parent._window
            parent._children.append(self)
        else:
            # assume Window here?

            # For now, not setting these attributes at all... (?) Because of
            # they are None we might have to type them as Optional, and all
            # code then needs to check for None-ness.
            pass
            # self._surface = None
            # self._window = None

            # assume Window here?
            # self._window = cast(Window, self)
            # self._surface = Surface(size)

        self._surface_pos = (0, 0)

        self.x = 0
        self.y = 0

        if size:
            self.width = size[0]
            self.height = size[1]
        else:
            self.width = 1
            self.height = 1

        self._min_width: int | None = None
        self._min_height: int | None = None

        # self._old_size = self._size

        # FIXME: Any -> StateVar?
        # self._listeners: list[tuple[Any, Callable]] = []
        self._listeners = []
        self._handlers: dict[str, list[Callable]] = {
            "on_destroy": [],
        }

    def connect(self, state, listener) -> Self:
        if isinstance(listener, str):
            listener = getattr(self, listener)
        self._listeners.append((state, listener))
        state.connect(listener)
        # FIXME: Right now the only difference between connect and listen
        # is this call. Resolve somehow?
        listener(state.value)
        return self

    def connect_enabled(self, state) -> Self:
        self.connect(state, self.set_enabled)
        return self

    def listen(self, signal, listener) -> Self:
        if isinstance(listener, str):
            listener = getattr(self, listener)
        self._listeners.append((signal, listener))
        signal.connect(listener)
        return self

    # def _on_destroy(self):
    #     for state, listener in self._listeners:
    #         state.disconnect(listener)

    def __del__(self):
        logger.debug("__del__ %r", self)

    def _call_handlers(self, event_name: str) -> None:
        for handler in self._handlers.get(event_name, []):
            try:
                handler()
            except Exception:
                # FIXME: Log to log file
                traceback.print_exc()

    def calculate_min_height(self, width):
        # FIXME: Not accounting for padding...!
        if self.layout is not None:
            return self.layout.calculate_min_height(width)
        raise NotImplementedError()

    def calculate_min_width(self):
        if self.layout is not None:
            return self.layout.calculate_min_width()
        raise NotImplementedError()

    def create_dc(self) -> "DrawingContext":
        dc = DrawingContext(self)
        if not self.enabled:
            dc.set_text_colour(Colour.GREY_88)
        return dc

    def destroy(self):
        from fsgui.windowmanager import WindowManager

        # Call all on_destroy event listeners
        try:
            self._on_destroy()
        except Exception:
            # FIXME: Log!
            import traceback

            traceback.print_exc()

        # Clear all handlers to break up possible reference cycles
        self._handlers.clear()

        # Disconnect all connected listeners
        for state, listener in self._listeners:
            try:
                logger.debug("Disconnecting %r", listener)
                state.disconnect(listener)
            except Exception:
                # FIXME: Log!
                import traceback

                traceback.print_exc()
        # And finally clear the internal listeners list itself to remove
        # references cycles.
        self._listeners.clear()

        # FIXME: When destroying widgets, also destroy layout hierarchy...!!

        for child in self._children:
            logger.debug("Calling child.destroy %r", child)
            child.destroy()
        self._children.clear()

        if self.layout is not None:
            self.layout.destroy()

        self.parent = None
        self._parent = None
        self._window = None
        self._surface = None
        self.surface = None
        self._destroyed = True

        WindowManager.get().on_widget_destroyed(self)

        # FIXME: Widget focus / mouse state code might still reference Widget
        # (Move that to WindowManager?)

    def disable(self) -> Self:
        self.set_enabled(False)
        return self

    def draw(self) -> None:
        self.on_paint()
        # FIXME: Should draw be responsible for drawing children or not?
        self.draw_children()

    def draw_children(self):
        # print(self._children)
        for child in self._children:
            # Update child surface position (recursively)
            child_pos = child.get_position()
            # print(child.get_position())
            child._surface_pos = (
                self._surface_pos[0] + child_pos[0],
                self._surface_pos[1] + child_pos[1],
            )

            child.draw()

    # def draw_background(self):
    #     # Delayed import due to circular imports between DrawingContext and Widget
    #     # from fsgui.drawingcontext import DrawingContext

    #     dc = DrawingContext(self)
    #     dc.set_fill_color(self.style.background_color)
    #     dc.draw_filled_rectangle((0, 0), self.get_size())

    # FIXME: Or self.enable(enable=True)
    def enable(self) -> None:
        self.set_enabled(True)

    def enable_hover_state(self) -> None:
        self._hover_state_enabled = True
        self._refresh_on_hover_state = True

    def enable_mouse_motion_events(self) -> None:
        self._mouse_motion_events = True

    def enable_pressed_state(self) -> None:
        # FIXME: Maybe just enable_mouse_states (refresh_on_pressed, refresh_on_hover, ..)
        self._pressed_state_enabled = True
        self._refresh_on_pressed_state = True

    # Could also be called find_child_widget_at_position (don't return self)
    def find_widget_at_position(self, position: Position):
        """Coordinates are relative to the widget itself."""

        x, y = position
        for widget in reversed(self._children):
            wx, wy = widget.get_position()
            ww, wh = widget.get_size()
            # print(x, y, wx, wy, ww, wh)
            if wx <= x < wx + ww and wy <= y < wy + wh:
                return widget.find_widget_at_position(
                    (position[0] - wx, position[1] - wy)
                )
        # No children matched, check if self is within bounds
        ww, wh = self.get_size()
        if 0 <= x < ww and 0 <= y < wh:
            # print(x, ww, y, wh)
            # print("returning self", self)
            return self
        # Nope, coordinates are outside even self
        return None

    def get_dc_offset(self) -> Position:
        return self._surface_pos

    def get_height(self) -> int:
        return self._size[1]

    def get_absolute_position(self) -> Position:
        x = self.x
        y = self.y
        parent = self.parent
        while parent:
            x += parent.x
            y += parent.y
            parent = parent.parent
        return (x, y)

    def get_mouse_position(self) -> tuple[int, int]:
        from fsgui.windowmanager import WindowManager

        mouse_pos = WindowManager.get().get_mouse_position()
        window_pos = self.get_window().get_window_position()
        widget_pos = self._surface_pos
        return (
            mouse_pos[0] - window_pos[0] - widget_pos[0],
            mouse_pos[1] - window_pos[1] - widget_pos[1],
        )

    def get_position(self) -> Position:
        return self._position

    def get_size(self) -> Size:
        return self._size

    def get_width(self) -> int:
        return self._size[0]

    def get_window(self):
        return self._window

    def is_destroyed(self) -> bool:
        return self._destroyed

    def is_mouse_hovering(self) -> bool:
        return self._is_mouse_hovering

    def is_mouse_pressed(self) -> bool:
        return self._is_mouse_pressed

    def layout_children(self):
        if self.layout is not None:
            top, right, bottom, left = self.padding
            self.layout.x = left
            self.layout.y = top
            self.layout.width = self.get_width() - left - right
            self.layout.height = self.get_height() - top - bottom
            self.layout.layout_children()

    def on_destroy(self, on_destroy: Callable) -> Self:
        self._handlers["on_destroy"].append(on_destroy)
        return self

    def _on_destroy(self) -> None:
        self._call_handlers("on_destroy")

    # def on_left_down(self):
    #     print("left_down")
    #     # FIXME: Grab mouse support

    #     if self._pressed_state_enabled:
    #         self._is_mouse_pressed = True
    #         if self._refresh_on_pressed_state:
    #             self.refresh()

    # def move(self, position: Position) -> Self:
    #     self.set_position(position)
    #     return self

    def on_left_click(self):
        pass

    def on_left_press(self):
        if self._pressed_state_enabled:
            self._is_mouse_pressed = True
            if self._refresh_on_pressed_state:
                self.refresh()

    def on_left_release(self):
        # FIXME: grab input?

        if self._pressed_state_enabled:
            self._is_mouse_pressed = False
            if self._refresh_on_pressed_state:
                self.refresh()

    def on_mouse_enter(self):
        # FIXME: mouse enter/leave events also when mouse exits the main window
        # (e.g. MainTitlebar)

        if self._hover_state_enabled:
            self._is_mouse_hovering = True
            if self._refresh_on_hover_state:
                self.refresh()

    def on_mouse_leave(self):
        if self._hover_state_enabled:
            self._is_mouse_hovering = False
            if self._refresh_on_hover_state:
                self.refresh()

    def on_mouse_motion(self):
        pass

    def on_move(self):
        pass

    def on_paint(self):
        dc = DrawingContext(self)
        dc.set_fill_colour(self.style.background_color)
        dc.draw_filled_rectangle((0, 0), self.get_size())

    def on_resize(self):
        pass

    # FIXME: Not sure about this. Added here mostly so painter can just check
    # for pressed state without caring about the widget type.
    @property
    def pressed(self) -> bool:
        return False

    def refresh(self) -> None:
        # No-op right now
        pass

    def set_enabled(self, enabled: bool) -> None:
        self.enabled = enabled
        self.refresh()

    def set_min_width(self, min_width: int | None) -> Self:
        self._min_width = min_width
        # FIXME: Schedule parent re-layout?
        return self

    def _set_position(self, position: Position):
        if position == self._position:
            return
        super()._set_position(position)
        self.on_move()

    # def set_size(self, size: Size):
    #     # FIXME: Register fixed size?
    #     self.set_size_internal(size)

    def _set_size(self, size: Size):
        if size == self.size:
            return
        super()._set_size(size)
        # Do we want to call on_resize instantly, or queue up size changes
        # somehow and do a resize pass? Not sure... -but maybe.
        self.on_resize()

    def set_width(self, width: int) -> Self:
        self.set_size((width, self.get_height()))
        return self

    def transfer_mouse_tracking(self, widget: "Widget") -> None:
        from fsgui.windowmanager import WindowManager

        WindowManager.get().transfer_mouse_tracking(self, widget)


# Put down here due to circular import and typing
from fsgui.drawingcontext import Colour, DrawingContext  # noqa: E402
