from typing import Callable, Protocol, TypeVar, runtime_checkable

T_co = TypeVar("T_co", covariant=True)


@runtime_checkable
class Connectable(Protocol[T_co]):
    """A value that listeners can connect to, to be called with the new value when it changes, and
    disconnect from."""

    @property
    def value(self) -> T_co: ...
    def connect(self, listener: Callable[[T_co], None], /) -> None: ...
    def disconnect(self, listener: Callable[[T_co], None], /) -> None: ...
