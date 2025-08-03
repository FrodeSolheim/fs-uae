import logging
from typing import Callable

logger = logging.getLogger(__name__)


# FIXME: Make "StateVar" inherit from Signal?
# fslib.signal?
# typing, template?  Signal[int], Signal[InputDevice]?
class Signal:
    def __init__(self) -> None:
        self._listeners: list[Callable] = []

    def broadcast(self, value):
        logger.debug("Calling %r", self._listeners)
        for listener in self._listeners:
            listener(value)

    # FIXME: add filter to connect/listener?
    def connect(self, listener: Callable) -> None:
        self._listeners.append(listener)

    def disconnect(self, listener: Callable) -> None:
        self._listeners.remove(listener)
