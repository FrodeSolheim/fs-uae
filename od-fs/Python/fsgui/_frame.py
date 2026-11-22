import logging

from .scale import get_scale, to_pixels
from .windowmanager import WindowManager, get_client_origin

logger = logging.getLogger(__name__)


window_manager = WindowManager.get()


def frame() -> None:
    """fsgui's UI frame, its API object's UI frame callback: updates the windows (layout, and paint
    and commit when dirty) and places their surfaces relative to the client origin."""
    # Could in theory update the surfaces first, and then continue running Python code without
    # allowing surface updates. It is possible this could improve concurrency w.r.t. the main
    # thread, but as it is now, no big point (probably).

    scale = get_scale()
    client_origin = get_client_origin()
    origin = (to_pixels(client_origin[0], scale), to_pixels(client_origin[1], scale))
    for window in window_manager.get_windows():
        try:
            window.update()
            position = window.get_surface_position()
            if window.surface is not None:
                window.place_surfaces((position[0] + origin[0], position[1] + origin[1]))
        except Exception:
            # FIXME: Make sure to propagate error level to SDL logger, and use custom
            # colors when printing to stderr on supported consoles???
            logger.exception("Error updating window")
