import logging
from dataclasses import dataclass
from typing import Callable, Generic, ParamSpec, Protocol, Self, TypeVar, cast, runtime_checkable

# , runtime_checkable

logger = logging.getLogger(__name__)

T = TypeVar("T")  # the value type carried by this Var

U = TypeVar("U")  # the value type carried by this Var

# R = TypeVar("R")

P = ParamSpec("P")
T_co = TypeVar("T_co", covariant=True)


@runtime_checkable
class Subscribable(Protocol[P]):
    def connect(self, listener: Callable[P, None], /) -> None: ...
    def disconnect(self, listener: Callable[P, None], /) -> None: ...


# @runtime_checkable
# class State(Subscribable[T_co]):
#     @property
#     def value(self) -> T_co: ...


@runtime_checkable
class State(Protocol[T_co]):
    """Value + stream: has a current snapshot and notifies on change."""

    @property
    def value(self) -> T_co: ...
    def connect(self, listener: Callable[[T_co], None]) -> None: ...
    def disconnect(self, listener: Callable[[T_co], None]) -> None: ...


@runtime_checkable
class Observable0(Protocol):
    """0-arg event/signal: no snapshot, just notifications."""

    def connect(self, listener: Callable[[], None]) -> None: ...
    def disconnect(self, listener: Callable[[], None]) -> None: ...


# Same as subscribable (almost) T vs T_co
class Observable1(Protocol[T_co]):
    def connect(self, listener: Callable[[T_co], None]) -> None: ...
    def disconnect(self, listener: Callable[[T_co], None]) -> None: ...


class Var(Generic[T]):
    def __init__(self, value: T) -> None:
        self._value: T = value
        self._listeners: list[Callable[[T], None]] = []
        self._changed_view: _ChangedView[T] | None = None

    @property
    def value(self) -> T:
        return self._value

    def connect(self, listener: Callable[[T], None]) -> None:
        self._listeners.append(listener)

    def disconnect(self, listener: Callable[[T], None]) -> None:
        self._listeners.remove(listener)

    def set(self, value: T) -> None:
        if value == self._value:
            return
        self._value = value
        for listener in list(self._listeners):
            listener(value)

    # nice ergonomic API
    def map(self, f: Callable[[T], U]) -> "MappedVar[U, T]":
        return MappedVar(self, f)

    @property
    def changed(self) -> Observable0:
        """Zero-arg event view for methods like `refresh`."""
        if self._changed_view is None:
            self._changed_view = _ChangedView(self)
        return self._changed_view


class _ChangedView(Generic[T]):
    """Adapter: State[T] -> Observable0 (keeps listener->wrapper map for proper disconnect)."""

    def __init__(self, src: State[T]) -> None:
        self._src = src
        self._wrappers: dict[Callable[[], None], Callable[[T], None]] = {}

    def connect(self, listener: Callable[[], None]) -> None:
        def wrapper(_: T) -> None:
            listener()

        self._wrappers[listener] = wrapper
        self._src.connect(wrapper)

    def disconnect(self, listener: Callable[[], None]) -> None:
        wrapper = self._wrappers.pop(listener)
        self._src.disconnect(wrapper)


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


class Emitter(Generic[P], Subscribable[P]):
    def __init__(self) -> None:
        self.listeners: list[Callable[P, None]] = []

    def connect(self, f: Callable[P, None]) -> None:
        self.listeners.append(f)

    def disconnect(self, f: Callable[P, None]) -> None:
        self.listeners.remove(f)

    def emit(self, *args: P.args, **kwargs: P.kwargs) -> None:
        for listener in list(self.listeners):
            listener(*args, **kwargs)


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
    state: State[T]
    listener: Callable[[T], None]


def noop(_: object) -> None:
    pass


class BindingSlot(Generic[T]):
    def __init__(self, apply: Callable[[T], None]) -> None:
        self._apply = apply
        self._binding: _Binding[T] | None = None

    def is_bound(self):
        return self._binding is not None

    def set(self, value: T | State[T]) -> None:
        # tear down existing binding
        if self._binding:
            self._binding.state.disconnect(self._binding.listener)
            self._binding = None

        # bind or apply. Note: generic Protocol + isinstance loses T; cast to restore.
        if isinstance(value, State):
            state = cast(State[T], value)
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


class Property(Var[T]):
    def __init__(self, initial: T) -> None:
        super().__init__(initial)
        self._slot = BindingSlot[T](self._update_from_binding)
        # self.change = Emitter[T]()

    def _update_from_binding(self, value: T) -> None:
        # Use Var's set method but bypass the binding slot to avoid cycles
        super().set(value)

    @property
    def is_bound(self):
        return self._slot.is_bound()

    # FIXME: Move to Var?
    def bind(self, value: State[T]) -> None:
        """Bind this property to a reactive source."""
        self._slot.set(value)

    # FIXME: bind vs set - maybe set should not accept State? or just take the current value
    # from state?

    # def set(self, value: T | State[T]) -> None:
    def set(self, value: T) -> None:
        """XXX - Bind this property to a literal value or reactive source."""

        # if self.is_bound:
        #    # print("FIXME: Bound value, emitting change value")
        #    # self.change.emit(value)
        #
        # else:
        self._slot.set(value)

    def destroy(self) -> None:
        self._slot.destroy()


# Var - Variable with connect/disconnect methods so anyone can subscribe to changes
# Property - Is a Var, so subscribers can listen for changes. In addition, the property can be
#            bound to other Var-s and reflect changes in their values.

# FIXME: property - event that only says
# FIXME: property.changed0 - event that only says
# FIXME: property.change - event with new desired value?
### FIXME: property.changed - event with new desired value?
