import logging
from typing import Callable, Self, TypedDict

logger = logging.getLogger(__name__)


class Event(TypedDict):
    type: str
    strdata: str
    strdata2: str
    intdata: int
    intdata2: int
    floatdata: float
    floatdata2: float


class EventService:
    _instance: Self

    @classmethod
    def instance(cls) -> Self:
        if not hasattr(cls, "_instance"):
            cls._instance = cls()
        return cls._instance

    def __init__(self) -> None:
        self.listeners: dict[str, list[Callable]] = {}

    def add_listener(self, listener: Callable, name: str) -> None:
        # FIXME: Instead have a list of signal objects indexed in a dict
        # by name and using that?
        self.listeners.setdefault(name, []).append(listener)

    def remove_listener(self, listener: Callable, name: str) -> None:
        self.listeners[name].remove(listener)

    def broadcast(self, event: Event):
        type = event["type"]

        if type == "UAE_CONFIG":
            # Not debug print event (too many of them, and too large)
            pass
        else:
            logger.debug("EVENT: %r", event)

        for listener in self.listeners.get(type, []):
            try:
                listener(event)
            except Exception:
                logging.exception(
                    f"Error broadcasting event {type} to listener {listener}"
                )
