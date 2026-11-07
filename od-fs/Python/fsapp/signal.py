import logging
from typing import Callable, Generic, ParamSpec

logger = logging.getLogger(__name__)

P = ParamSpec("P")


# class Signal(Generic[P], Subscribable[P]):
class Signal(Generic[P]):
    def __init__(self) -> None:
        self._listeners: list[Callable[P, None]] = []

    def connect(self, listener: Callable[P, None]) -> None:
        self._listeners.append(listener)

    def disconnect(self, listener: Callable[P, None]) -> None:
        self._listeners.remove(listener)

    def count_listeners(self) -> int:
        return len(self._listeners)

    def emit(self, *args: P.args, **kwargs: P.kwargs) -> None:
        for listener in list(self._listeners):
            try:
                listener(*args, **kwargs)
            except Exception:
                logger.exception(f"Signal emit error to listener {listener}")
