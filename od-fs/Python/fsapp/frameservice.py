import logging
from typing import Callable, ClassVar, Self

import _fsapp
import sdl3

logger = logging.getLogger(__name__)

FrameHandler = Callable[[list[sdl3.SDL_Event]], None]


class FrameService:
    """Runs the handlers registered for the UI thread's frame, such as the UI toolkit's (fsgui).

    Each UI frame, fsapp calls the handlers with the UI frame's SDL events, after the due timers
    and before its event service handles the events. An exception in a handler ends the UI frame
    (fsapp logs it).
    """

    _instance: ClassVar[Self | None] = None

    @classmethod
    def get(cls) -> Self:
        if cls._instance is None:
            cls._instance = cls()
        return cls._instance

    def __init__(self) -> None:
        self._handlers: list[FrameHandler] = []

    def register(self, handler: FrameHandler) -> None:
        self._handlers.append(handler)

    @staticmethod
    def request_ui_frame() -> None:
        """Requests a UI frame at the next host window frame (for animation).

        The UI thread sleeps between UI frames until something wakes it (events, app events, a
        file dialog's result, a due timer or this request); requesting a UI frame in each UI frame
        runs one per host window frame.
        """
        _fsapp.request_ui_frame()

    def frame(self, events: list[sdl3.SDL_Event]) -> None:
        for handler in list(self._handlers):
            handler(events)
