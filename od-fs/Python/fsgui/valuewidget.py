from typing import Generic, Self, TypeVar

from fsuae.reactive import Property, State

from fsgui.font import Font
from fsgui.types import Size
from fsgui.widget import Widget

T = TypeVar("T")


# class ValueWidget(Generic[T], Widget):
class ValueWidget(Widget, Generic[T]):
    def __init__(
        self,
        initial: T,
        *,
        font: Font | None = None,
        parent: Widget | None = None,
        size: Size | None = None,
    ) -> None:
        super().__init__(font=font, parent=parent, size=size)
        # self._value_var = Var(initial)  # the authoritative state
        # self._value_slot = BindingSlot[T](self._value_var.set)
        self._value = Property[T](initial)

    @property
    def value(self) -> T:
        """Snapshot of current value."""
        # return self._value_var.value
        return self._value.value

    # FIXME: State or Var?
    def set_value(self, v: T | State[T]) -> Self:
        """Accept a literal or bind to a stream."""
        old_value = self.value
        self._value.set(v)
        if self.value != old_value:
            self.refresh()
        return self

    def destroy(self) -> None:
        # self._value_slot.destroy()
        self._value.destroy()
        super().destroy()
