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
"""

__all__: Final = ["set_fullscreen", "get_mouse_grab"]
