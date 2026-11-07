from typing import Callable, Generic, ParamSpec, Protocol, TypeVar, runtime_checkable

from fsapp.signal import Signal

T = TypeVar("T")
U = TypeVar("U")


@runtime_checkable
class Observable0(Protocol):
    """0-arg event/signal: no snapshot, just notifications."""

    def connect(self, listener: Callable[[], None]) -> None: ...
    def disconnect(self, listener: Callable[[], None]) -> None: ...


P = ParamSpec("P")


@runtime_checkable
class Subscribable(Protocol[P]):
    def connect(self, listener: Callable[P, None], /) -> None: ...
    def disconnect(self, listener: Callable[P, None], /) -> None: ...


# # class Signal(Generic[P], Subscribable[P]):
# class Signal(Generic[P]):
#     def __init__(self) -> None:
#         self.listeners: list[Callable[P, None]] = []

#     def connect(self, f: Callable[P, None]) -> None:
#         self.listeners.append(f)

#     def disconnect(self, f: Callable[P, None]) -> None:
#         self.listeners.remove(f)

#     def emit(self, *args: P.args, **kwargs: P.kwargs) -> None:
#         for listener in list(self.listeners):
#             listener(*args, **kwargs)


class Var(Generic[T]):
    def __init__(self, value: T) -> None:
        self._value: T = value
        self.changed: Signal[T] = Signal()

    @property
    def value(self) -> T:
        return self._value

    def set(self, value: T) -> None:
        if value == self._value:
            return
        self._value = value
        self.changed.emit(value)

    def map(self, f: Callable[[T], U]) -> "MappedVar[U, T]":
        return MappedVar(self, f)


class MappedVar(Var[T], Generic[T, U]):
    """
    A Var[T] that mirrors a Var[U] through a transform U -> T.
    It (re)subscribes to the source when it has listeners, and detaches when it doesn't.
    """

    def __init__(self, source: Var[U], transform: Callable[[U], T]) -> None:
        super().__init__(transform(source.value))
        self._source: Var[U] = source
        self._transform: Callable[[U], T] = transform
        self._attached: bool = False
        self._attach()  # eager subscribe; optional

    # --- subscription management to the source ---

    def _attach(self) -> None:
        if not self._attached:
            self._source.connect(self._on_source)
            self._attached = True

    def _detach(self) -> None:
        if self._attached:
            self._source.disconnect(self._on_source)
            self._attached = False

    # Keep the same typed signature as Var[T]
    def connect(self, listener: Callable[[T], None]) -> None:  # override
        # ensure we’re listening to source if (re)gaining listeners
        if not self._attached:
            self._attach()
        super().connect(listener)

    def disconnect(self, listener: Callable[[T], None]) -> None:  # override
        super().disconnect(listener)
        # if nobody listens to the filtered var, detach from source
        if not self._listeners:
            self._detach()

    # --- source callback ---

    def _on_source(self, value: U) -> None:
        self.set(self._transform(value))
