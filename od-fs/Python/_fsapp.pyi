# _fsapp.pyi
"""Typing stubs for the `_fsapp` C extension."""

from typing import Final

def set_fullscreen(fullscreen: bool, /) -> None: ...

"""Request entering or leaving fullscreen mode.

Args:
    fullscreen: True to enter fullscreen, False to leave.

Raises:
    RuntimeError: If the operation fails at runtime (implementation-dependent).
"""

def get_mouse_grab() -> bool: ...

"""Get the current mouse grab state.

Returns:
    True if mouse is currently grabbed, False otherwise.

When the mouse grab changes, fsapp pushes the app event FSAPP_MOUSE_GRAB (intdata 1 when grabbed,
0 when released), so the UI need not poll this.
"""

def get_monotonic_us() -> int: ...

"""Get the time now on SDL's clock (SDL_GetTicksNS), the clock all of FS-UAE reads.

Returns:
    The time in microseconds (the timer service's times, and the next timer's time that
    fsapp._frame.frame returns).
"""

def request_ui_frame() -> None: ...

"""Request a UI frame at the next host window frame (for animation).

Requests a host window frame, also without a pacer, and wakes the UI thread when the host window
side starts it. Callable from any thread.
"""

LOG_PRIORITY_TRACE: Final[int]
LOG_PRIORITY_VERBOSE: Final[int]
LOG_PRIORITY_DEBUG: Final[int]
LOG_PRIORITY_INFO: Final[int]
LOG_PRIORITY_WARN: Final[int]
LOG_PRIORITY_ERROR: Final[int]
LOG_PRIORITY_CRITICAL: Final[int]

"""SDL's log priorities (SDL_LogPriority), for log()."""

def log(priority: int, text: str, /) -> None: ...

"""Log a text with SDL's logging (SDL_LogMessage in SDL_LOG_CATEGORY_APPLICATION), the one way
from Python into the log; fsapp.log hands Python's logging, sys.stdout and sys.stderr to it.

The text is never a format. A text with newlines is one log line per line, each with the calling
thread's name and the priority. What UTF-8 cannot encode is written as a backslash escape.

Args:
    priority: One of the LOG_PRIORITY_ constants.
    text: The text.

Raises:
    ValueError: If the priority is not one of SDL's.
"""

__all__: Final = [
    "set_fullscreen",
    "get_mouse_grab",
    "get_monotonic_us",
    "request_ui_frame",
    "log",
    "LOG_PRIORITY_TRACE",
    "LOG_PRIORITY_VERBOSE",
    "LOG_PRIORITY_DEBUG",
    "LOG_PRIORITY_INFO",
    "LOG_PRIORITY_WARN",
    "LOG_PRIORITY_ERROR",
    "LOG_PRIORITY_CRITICAL",
]
