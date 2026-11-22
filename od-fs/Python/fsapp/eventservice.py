import logging
from typing import Any, Callable, NotRequired, Self, TypedDict

from sdl3 import SDL_Event

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
        # The listeners by event name: of app events (Event) and of the SDL events that fsapp._frame
        # passes on by name (SDL_Event).
        self._listeners: dict[str, list[Callable[[Any], None]]] = {}

    def add_listener(self, name: str, listener: Callable[[Event], None]) -> None:
        self._listeners.setdefault(name, []).append(listener)

    def add_sdl_listener(self, name: str, listener: Callable[[SDL_Event], None]) -> None:
        """Adds a listener of the SDL events that fsapp._frame passes on (EventName.SDL_EVENT_*)."""
        self._listeners.setdefault(name, []).append(listener)

    def has_listeners(self, name: str) -> bool:
        return len(self._listeners.get(name, [])) > 0

    def remove_listener(self, listener: Callable[[Event], None], name: str) -> None:
        self._listeners.get(name, []).remove(listener)

    def broadcast(self, event: Event) -> None:
        type = event["type"]

        if type == "UAE_CONFIG":
            # Not debug print event (too many of them, and too large)
            pass
        else:
            logger.debug("EVENT: %r", event)

        self._call_listeners(type, event)

    def broadcast_sdl_event(self, name: str, event: SDL_Event) -> None:
        self._call_listeners(name, event)

    def _call_listeners(self, name: str, event: Any) -> None:
        for listener in list(self._listeners.get(name, [])):
            try:
                listener(event)
            except Exception:
                logger.exception("Error in the listener %r of event %s", listener, name)
