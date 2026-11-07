import logging
from typing import Any, Callable, NotRequired, Self, TypedDict

from sdl3 import SDL_Event

from fsapp.signal import Signal

logger = logging.getLogger(__name__)


class Event(TypedDict):
    type: str
    strdata: NotRequired[str]
    strdata2: NotRequired[str]
    intdata: NotRequired[int]
    intdata2: NotRequired[int]
    floatdata: NotRequired[float]
    floatdata2: NotRequired[float]


class EventService:
    _instance: Self

    @classmethod
    def instance(cls) -> Self:
        if not hasattr(cls, "_instance"):
            cls._instance = cls()
        return cls._instance

    def __init__(self) -> None:
        # self.listeners: dict[str, list[Callable[[Event], None]]] = {}

        self._events: dict[str, Signal[Any]] = {}

    def event(self, name: str) -> Signal[Event]:
        if name not in self._events:
            self._events[name] = Signal()
        return self._events[name]

    def sdl_event(self, name: str) -> Signal[SDL_Event]:
        if name not in self._events:
            self._events[name] = Signal()
        return self._events[name]

    # FIXME: deprecated
    def add_listener(self, name: str, listener: Callable[[Event], None]) -> None:
        # FIXME: Instead have a list of signal objects indexed in a dict
        # by name and using that?
        # self.listeners.setdefault(name, []).append(listener)
        self.event(name).connect(listener)

    def has_listeners(self, name: str) -> bool:
        return self.event(name).count_listeners() > 0

    # FIXME: deprecated
    def remove_listener(self, listener: Callable[[Event], None], name: str) -> None:
        # self.listeners[name].remove(listener)
        self.event(name).disconnect(listener)

    def broadcast(self, event: Event) -> None:
        type = event["type"]

        if type == "UAE_CONFIG":
            # Not debug print event (too many of them, and too large)
            pass
        else:
            logger.debug("EVENT: %r", event)

        self.event(type).emit(event)

        # for listener in self.listeners.get(type, []):
        #    try:
        #        listener(event)
        #    except Exception:
        #        logging.exception(f"Error broadcasting event {type} to listener {listener}")
