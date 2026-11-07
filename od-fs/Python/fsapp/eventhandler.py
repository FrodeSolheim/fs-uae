import logging
from typing import Any, Callable, Self, TypeVar, overload

from fsgui.widget import Observable0, State
from fsuae.reactive import Observable1
from fsuae.var import Var

logger = logging.getLogger(__name__)

T = TypeVar("T")


# FIXME: Copied methods from Widget. Get this DRY-ified somehow
# FIXME: Better name? EventBinder? SignalBinder?
class EventHandler:
    def __init__(self) -> None:
        self._listeners: list[tuple[Var[Any], Callable[..., None]]] = []

    def destroy(self):
        # Disconnect all connected listeners
        for state, listener in self._listeners:
            try:
                logger.debug("Disconnecting %r", listener)
                state.disconnect(listener)
            except Exception:
                # FIXME: Log!
                import traceback

                traceback.print_exc()

        # And finally clear the internal listeners list itself to remove references cycles
        self._listeners.clear()

    @overload
    def on(self, src: State[T], fn: Callable[[T], None], /, *, immediate: bool = False) -> Self: ...

    @overload
    def on(self, src: Observable1[T], fn: Callable[[T], None], /) -> Self: ...

    @overload
    def on(self, src: Observable0, fn: Callable[[], None], /) -> Self: ...

    def on(self, src: Any, listener: Any, /, *, immediate: bool = False) -> Self:
        # if isinstance(listener, str):
        #     listener = getattr(self, listener)
        self._listeners.append((src, listener))
        src.connect(listener)
        # FIXME: Right now the only difference between connect and listen
        # is this call. Resolve somehow?

        # if immediate?
        # Are we OK with this runtime check?
        if isinstance(src, State):
            listener(src.value)
        return self

        # # FIXME: ...
        # self.listen(src, fn)  # type: ignore
        # # self.connect(src, fn)
        # # track and auto-disconnect here
        # return self
