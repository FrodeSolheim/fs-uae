from types import TracebackType
from typing import Any, Callable, Optional, Self, Type, Union

from fsgui.parentstack import ParentStack
from fsgui.types import ExpandParam, Position, Size, SizeParam


class LayoutData:
    min_size = (0, 0)
    size = 0


class WidgetOrLayout:
    parent: Union["WidgetOrLayout", None] = None

    x: int = 0
    y: int = 0
    width: int = 0
    height: int = 0

    def __init_subclass__(cls, **kwargs: Any):
        # print("__init_subclass begin", cls, kwargs)

        super().__init_subclass__(**kwargs)  # <<< crucial

        # cls.__init__2 = cls.__init__
        original_init: Callable[..., None] = cls.__init__

        def init_wrapper(self: Any, *args: Any, **kwargs: Any) -> None:
            # print(" --- initWrapper begin", cls.__name__, self)
            ParentStack.push(self)
            try:
                # cls.__init__2(*args, **kwargs)
                original_init(self, *args, **kwargs)
            finally:
                ParentStack.pop(self)
            # print(" --- initWrapper end", cls.__name__, self)

        cls.__init__ = init_wrapper  # type: ignore

    def __init__(
        self,
        parent: "WidgetOrLayout",
        *,
        expand: ExpandParam = False,
        size: SizeParam = None,
    ):
        self._halign = 0.0
        self._valign = 0.5
        self._expand = expand
        self._fill = False

        self.layout_data = LayoutData()
        self.parent = parent

    def __enter__(self) -> None:
        ParentStack.stack.append(self)

    def __exit__(
        self,
        exc_type: Optional[Type[BaseException]],
        exc_val: Optional[BaseException],
        exc_tb: Optional[TracebackType],
    ) -> None:
        assert ParentStack.stack.pop() is self

    @property
    def _align(self) -> tuple[float, float]:
        # Mainly useful for Layouts to index align based on direction (0/1)
        return (self._halign, self._valign)

    def calculate_min_height(self, width: int) -> int:
        raise NotImplementedError()

    def calculate_min_width(self) -> int:
        raise NotImplementedError()

    def expand(self, expand: float = 1.0) -> Self:
        self._expand = expand
        return self

    @property
    def none(self) -> None:
        """Use at end of chain when it is important that the expression result is None"""
        pass

    def fill(self, fill: bool = True) -> Self:
        self._fill = fill
        return self

    def get_position(self) -> Position:
        return (self.x, self.y)

    def get_height(self) -> int:
        return self.height

    def get_size(self) -> Size:
        return (self.width, self.height)

    def get_width(self) -> int:
        return self.width

    def layout_children(self) -> None:
        pass

    def move(self, position: Position) -> Self:
        self._set_position(position)
        return self

    @property
    def position(self) -> Position:
        return (self.x, self.y)

    @property
    def _position(self) -> Position:
        return (self.x, self.y)

    def resize(self, size: Size) -> Self:
        self._set_size(size)
        return self

    @property
    def size(self) -> Size:
        return (self.width, self.height)

    @property
    def _size(self) -> Size:
        return (self.width, self.height)

    def set_position(self, position: Position) -> None:
        self._set_position(position)

    def _set_position(self, position: Position) -> None:
        self.x = position[0]
        self.y = position[1]

    def set_size(self, size: Size) -> None:
        self._set_size(size)

    def _set_size(self, size: Size) -> None:
        self.width = size[0]
        self.height = size[1]

    def set_size_internal(self, size: Size) -> None:
        self._set_size(size)
