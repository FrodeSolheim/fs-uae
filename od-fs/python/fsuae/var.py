import logging
from dataclasses import dataclass
from typing import Callable, Generic, Protocol, TypeVar, cast, runtime_checkable

logger = logging.getLogger(__name__)

T = TypeVar("T")  # the value type carried by this Var

U = TypeVar("U")  # the value type carried by this Var

R = TypeVar("R")

T_co = TypeVar("T_co", covariant=True)


@runtime_checkable
class Connectable(Protocol[T_co]):
    @property
    def value(self) -> T_co: ...
    def connect(self, listener: Callable[[T_co], None], /) -> None: ...
    def disconnect(self, listener: Callable[[T_co], None], /) -> None: ...


class Var(Generic[T], Connectable[T]):
    def __init__(self, value: T) -> None:
        self._value: T = value
        self._listeners: list[Callable[[T], None]] = []
        # self._listeners: list[tuple[Callable[[T], None], Optional[Callable[[T], T]]]] = []

    # @overload
    # def connect(
    #     self,
    #     listener: Callable[[T], None],
    # ) -> None: ...

    # @overload
    # def connect(
    #     self,
    #     listener: Callable[[U], None],
    #     *,
    #     filter: Callable[[T], U] | None = None,
    # ) -> None: ...

    # def connect(
    #     self,
    #     listener: Callable[[T], None] | Callable[[U], None],
    #     *,
    #     filter: Callable[[T], T] | Callable[[T], U] | None = None,
    # ) -> None:
    #     self._listeners.append((listener, filter))

    def connect(
        self,
        listener: Callable[[T], None],
        # *,
        # filter: Callable[[T], T] | None = None,
    ) -> None:
        # self._listeners.append((listener, filter))
        self._listeners.append(listener)

    def disconnect(self, listener: Callable[[T], None]) -> None:
        # self._listeners = [(x, y) for (x, y) in self._listeners if x is not listener]
        self._listeners.remove(listener)

    @property
    def value(self) -> T:
        return self._value

    # FIXME: Remove?
    def get(self) -> T:
        return self._value

    def set(self, value: T) -> None:
        if value == self._value:
            return
        logger.debug("(UAECONFIG2) SET  %r", value)
        self._value = value
        logger.debug("calling %r", self._listeners)
        # for listener, filter in list(self._listeners):  # copy to avoid mutation surprises
        #     if filter is not None:
        #         listener(filter(value))
        #     else:
        #         listener(value)
        for listener in list(self._listeners):  # copy to avoid mutation surprises
            listener(value)

    # nice ergonomic API
    def map(self, f: Callable[[T], R]) -> "MappedVar[R, T]":
        return MappedVar(self, f)


class StrVar(Var[str]):
    def __init__(self, value: str = "") -> None:
        super().__init__(value)


class IntVar(Var[int]):
    def __init__(self, value: int = 0) -> None:
        super().__init__(value)


# FIXME: Need weak reference somewhere...!

# class VarFilter(Generic[T, U]):
#     def __init__(self, var: Var[T], filter: Callable[[T], U]) -> None:
#         self._var = var
#         self._filter = filter
#         self._var.connect(self._on_var)

#     def _on_var(self, value: T):
#         pass


class MappedVar(Var[T], Generic[T, U]):
    """
    A Var[T] that mirrors a Var[U] through a transform U -> T.
    It (re)subscribes to the source when it has listeners, and detaches when it doesn't.
    """

    def __init__(self, source: Var[U], transform: Callable[[U], T]) -> None:
        super().__init__(transform(source.get()))
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


# a = StrVar()

# def fil(value: str) -> bool:
#     return False


# b = MappedVar(a, fil)

# def accept(v: Var[bool]):
#     pass

# accept(a)
# accept(b)

# --------------------------------------------------------------------------------------------------
# Move to binding.py file?


@dataclass
class _Binding(Generic[T]):
    state: Connectable[T]
    listener: Callable[[T], None]


def noop(_: object) -> None:
    pass


class BindingSlot(Generic[T]):
    def __init__(self, apply: Callable[[T], None]) -> None:
        self._apply = apply
        self._binding: _Binding[T] | None = None

    def set(self, value: T | Connectable[T]) -> None:
        # tear down existing binding
        if self._binding:
            self._binding.state.disconnect(self._binding.listener)
            self._binding = None

        # bind or apply. Note: generic Protocol + isinstance loses T; cast to restore.
        if isinstance(value, Connectable):
            state = cast(Connectable[T], value)
            state.connect(self._apply)
            self._binding = _Binding(state, self._apply)
            self._apply(state.value)  # immediate
        else:
            self._apply(value)

    def destroy(self) -> None:
        if self._binding:
            self._binding.state.disconnect(self._binding.listener)
            self._binding = None

        # Remove the self._apply reference to ensure no reference cycle issues if __del__ is
        # being used somewhere.
        self._apply = noop
        # self._apply = cast(Callable[[T], None], _noop)
