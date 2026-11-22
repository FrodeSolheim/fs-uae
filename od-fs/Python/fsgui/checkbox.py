from typing import Self, TypeVar

from .drawingcontext import Colour
from .font import Font
from .reactive import Emitter, Property, State
from .types import Size
from .widget import Widget
from .widgetpainter import WidgetPainter

# class WidgetWithValue[bool] ?

# T = TypeVar("T")


# class WidgetOrLayout:
#     parent: Union["WidgetOrLayout", None] = None

#     x: int = 0
#     y: int = 0
#     width: int = 0
#     height: int = 0

#     def __init_subclass__(cls, **kwargs: Any):
#         # print("__init_subclass begin", cls, kwargs)

#         super().__init_subclass__(**kwargs)  # <<< crucial

#         # cls.__init__2 = cls.__init__
#         original_init: Callable[..., None] = cls.__init__

#         def init_wrapper(self: Any, *args: Any, **kwargs: Any) -> None:
#             # print(" --- initWrapper begin", cls.__name__, self)
#             ParentStack.push(self)
#             try:
#                 # cls.__init__2(*args, **kwargs)
#                 original_init(self, *args, **kwargs)
#             finally:
#                 ParentStack.pop(self)
#             # print(" --- initWrapper end", cls.__name__, self)

#         cls.__init__ = init_wrapper  # type: ignore


# class Widget:
#     def __init__(
#         self,
#         *,
#         font: Font | None = None,
#         parent: Optional["Widget"] = None,
#         size: Size | None = None,
#     ) -> None:
#         pass


# class ValueWidget(Widget, Generic[T]):
# # class ValueWidget(Generic[T], Widget):
#     def __init__(
#         self,
#         initial: T,
#         *,
#         font: Font | None = None,
#         parent: Widget | None = None,
#         size: Size | None = None,
#     ) -> None:
#         super().__init__(font=font, parent=parent, size=size)
#         self._value_var = Var(initial)  # the authoritative state
#         self._value_slot = BindingSlot[T](self._value_var.set)

#     @property
#     def value(self) -> T:
#         """Snapshot of current value."""
#         return self._value_var.value

#     # --- writes/binds ---
#     def set_value(self, v: T | Connectable[T]) -> Self:
#         """Accept a literal or bind to a stream."""
#         self._value_slot.set(v)
#         self.refresh()
#         return self

#     def destroy(self) -> None:
#         self._value_slot.destroy()
#         super().destroy()


T = TypeVar("T")


class CheckBox(Widget):
    def __init__(
        self,
        text: str,
        *,
        font: Font | None = None,
        parent: Widget | None = None,
        size: Size | None = None,
        wrap: bool = False,
    ) -> None:
        super().__init__(font=font, parent=parent, size=size)
        self.text = text
        self.wrap = wrap
        self.check_width = 16
        self.gap = 6

        # self._value = False
        # FIXME
        # self.enabled = False

        # Maybe make these listenable / readonly "properties", and make the enabled functionality
        # implicit when there is at least one listener?
        self.enable_hover_state()
        self.enable_pressed_state()

        # self._value_var: Optional[Var[bool]] = None
        # self._value
        # self._value_slot = BindingSlot[bool](self._set_value)

        # self._checked = ...
        self.checked_state = Property[bool](False)
        self.checked_change = Emitter[bool]()
        # Set or bound, the checked state is drawn.
        self.on(self.checked_state, self._refresh_on_change)

        # FIXME: self bind
        # self.checked.change.connect(self.on_change)

        # Not important to use self.on in this case

        # FIXME: Consider removing this, and let subclasses decide for themselves if they want/need
        # this or not
        # self.on(self.checked_state.change, self.on_change)

        # self.checked_on_last_paint = False

    @property
    def checked(self) -> bool:
        return self.checked_state.value

    def bind_checked(self, state: State[bool]) -> Self:
        self.checked_state.bind(state)
        return self

    def set_checked(self, value: bool = True) -> Self:
        self.checked_state.set(value)
        return self

    # @property
    # def checked_state(self) -> Property[bool]:
    #     return self._checked

    # def __del__(self):
    #     print("__del__", self)

    def destroy(self):
        self.checked_state.destroy()
        super().destroy()

    # def on_change(self, value: bool) -> None:
    #     print("on_change")

    # FIXME: Or def _on_check(self, new_value) -> None:
    def _on_check(self) -> None:
        new_value = not self.checked
        if self.checked_state.is_bound:
            self.checked_change.emit(new_value)
        else:
            self.set_checked(not self.checked)

    def on_left_click(self) -> None:
        if not self.enabled:
            return
        self._on_check()

    def calculate_min_height(self, width: float) -> float:
        return 28

    def calculate_min_width(self) -> float:
        tw, _ = self.font.measure_text(self.text)
        return self.check_width + self.gap + tw

    def on_paint(self) -> None:
        dc = self.create_dc()
        painter = WidgetPainter(self, dc)

        check_width = self.check_width
        x1 = 0
        y1 = (self.height - check_width) // 2
        # x2 = x1 + self.check_width - 1
        # y2 = y1 + self.check_width - 1

        # FIXME: Pressed state
        painter.paint_box((x1, y1), (check_width, check_width))

        text_x = self.check_width + self.gap
        dc.draw_text_centred(self.text, (text_x, 0), (self.width - text_x, self.height))

        # Drawing a square for now. Maybe draw a proper checkmark later
        # (however, the square is more visible though...)
        if self.enabled:
            fill_colour = Colour.GREY_33
        else:
            # FIXME: Check if this looks nice
            fill_colour = Colour.GREY_AA

        # if self.checked != self.checked_on_last_paint:
        #     print("PAINT: CHECKED changed to ", self.checked)
        #     self.checked_on_last_paint = self.checked

        if self.checked:
            p = 3
            dc.set_fill_colour(fill_colour)
            dc.draw_filled_rectangle(
                (x1 + p, y1 + p),
                (check_width - 2 * p, check_width - 2 * p),
            )

    def set_text(self, text: str) -> None:
        self.text = text
        self.refresh()
