import logging

from .resources import set_resource_dirs as set_resource_dirs

logger = logging.getLogger(__name__)


def fsgui_init() -> None:
    """Starts fsgui: its frame runs in every UI frame, and it draws at the display scale until its
    surfaces' configures bring another."""
    print("fsgui.init")

    from ._frame import frame
    from .internal.api import get_api
    from .scale import set_scale

    api = get_api()
    set_scale(api.display_scale)
    api.on_ui_frame(frame)
