# _fsapp.pyi
"""Typing stubs for the `_fsapp` C extension."""

from collections.abc import Callable
from typing import Final

def set_fullscreen(fullscreen: bool, /) -> None: ...

"""Request entering or leaving fullscreen mode.

Args:
    fullscreen: True to enter fullscreen, False to leave.

Raises:
    RuntimeError: If the operation fails at runtime (implementation-dependent).
"""

def get_mouse_grab() -> bool: ...

"""Get whether the user wants the input grab (Mod+G, the middle button).

Returns:
    True if the grab is wanted, False otherwise. A wanted grab is suspended (not applied) while a
    surface other than the emulation surface has keyboard focus.

When it changes, fsapp pushes the app event FSAPP_MOUSE_GRAB (intdata 1 when wanted, 0 when not),
so the UI need not poll this.
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

def get_display_scale() -> float: ...

"""Get the display scale: how many pixels the UI is to draw per design unit.

Returns:
    The display scale of the last configure (fsapp.surfaceapi.v1.Configure), which is SDL's window
    display scale; before the host window exists, the scale it is predicted to have (the primary
    display's content scale times its pixel density), for the first window size.
"""

TITLE_BAR_HEIGHT: Final[int]

"""The height of the host window's title bar in design units, which the borderless host window's
hit test takes as its title bar's (FSAPP_PIXELS_TITLE_BAR_HEIGHT)."""

def create_window(title: str, size: tuple[int, int], fullscreen: bool, /) -> object: ...

"""Create the host window (unstable, for the top), on the main thread when it handles the request.

Args:
    title: The window's title.
    size: The window's size in pixels.
    fullscreen: Whether it starts full screen.

Returns:
    The window, for set_window_background_color.
"""

def set_window_background_color(window: object, colour: tuple[int, int, int, int], /) -> None: ...

"""Set the colour the host window is cleared with (RGBA), where no surface covers it."""

def minimize_window() -> None: ...

"""Request minimizing the host window."""

def maximize_window() -> None: ...

"""Request maximizing the host window."""

def restore_window() -> None: ...

"""Request restoring the host window from maximized (or minimized)."""

def close_window() -> None: ...

"""Request closing the host window, which quits FS-UAE."""

SHORTCUT_MOD: Final[int]

"""Mod, the modifier of FS-UAE's shortcuts, as SDL's modifier bits (Keymod in fsapp.surfaceapi.v1):
Left Alt, or Left GUI (Cmd) on macOS."""

def register_shortcut(key: int, modifiers: int, callback: Callable[[], object], /) -> int: ...

"""Register a shortcut whose callback runs on the UI thread.

The shortcut is looked up for each key event on the host window side before anything else sees it:
its press, repeats and release are consumed and reach neither the Amiga nor the UI. When it is
pressed, the UI thread is woken and runs the callback in its next UI frame, after the app events.
An exception from the callback is logged. A shortcut matches when all its modifiers are down
(others may be down too); the modifier keys themselves are no shortcuts. Shortcuts are never
unregistered.

Args:
    key: The key, an SDL keycode (Keycode in fsapp.surfaceapi.v1).
    modifiers: The modifiers that must be down, SDL's modifier bits (Keymod.NONE for none,
        SHORTCUT_MOD for Mod).
    callback: Called with no arguments when the shortcut is pressed.

Returns:
    The shortcut's id.

Raises:
    ValueError: If the key is already a shortcut with the same modifiers.
    TypeError: If the callback is not callable.
"""

def take_shortcut_callbacks() -> list[Callable[[], object]]: ...

"""Take the callbacks of the shortcuts and the menu input pressed since the last call, in the order
pressed.

For fsapp._frame, which runs them in the UI frame.
"""

def register_menu_input(callback: Callable[[], object], /) -> None: ...

"""Register the callback of the menu input, run on the UI thread (for the top, which opens the
gamepad session menu with it).

The menu input is Guide, or Start and Select pressed together (in either order), on any gamepad. It
never reaches the Amiga. While the emulation surface has keyboard focus or the input grab is
applied, the UI thread is woken and runs the callback in its next UI frame, with the shortcuts'
callbacks; while a UI surface has keyboard focus, that surface gets it as Navigation.MENU
(fsapp.surfaceevent) instead. With no callback registered, the menu input does nothing while the
Amiga has the input. Start pressed first has reached the Amiga at once (games pause with it), so
when Select completes the chord, the Amiga gets a release of Start. An exception from the callback
is logged. The callback is never unregistered.

Args:
    callback: Called with no arguments for each menu input.

Raises:
    ValueError: If a callback is already registered.
    TypeError: If the callback is not callable.
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
    "SHORTCUT_MOD",
    "TITLE_BAR_HEIGHT",
    "create_window",
    "set_window_background_color",
    "minimize_window",
    "maximize_window",
    "restore_window",
    "close_window",
    "register_shortcut",
    "take_shortcut_callbacks",
    "register_menu_input",
    "log",
    "LOG_PRIORITY_TRACE",
    "LOG_PRIORITY_VERBOSE",
    "LOG_PRIORITY_DEBUG",
    "LOG_PRIORITY_INFO",
    "LOG_PRIORITY_WARN",
    "LOG_PRIORITY_ERROR",
    "LOG_PRIORITY_CRITICAL",
]
