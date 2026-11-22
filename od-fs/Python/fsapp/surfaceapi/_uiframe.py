"""The API objects' UI frame callbacks, which fsapp._frame runs each UI frame."""

import logging
from collections.abc import Callable

logger = logging.getLogger(__name__)

# The callbacks by the creation number of their API objects.
_callbacks: dict[int, Callable[[], object]] = {}


def set_callback(number: int, callback: Callable[[], object] | None) -> None:
    if callback is None:
        _callbacks.pop(number, None)
    else:
        _callbacks[number] = callback


def run() -> None:
    """Runs the callbacks in the order their API objects were created. An exception in one is
    logged, and the others still run."""
    for _, callback in sorted(_callbacks.items()):
        try:
            callback()
        except Exception:
            logger.exception("Exception in the UI frame callback %r", callback)
