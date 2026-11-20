import logging

logger = logging.getLogger(__name__)


def fsgui_init() -> None:
    print("fsgui.init")

    from fsapp.frameservice import FrameService

    from fsgui._frame import frame

    # fsapp's UI-thread frame calls fsgui's frame with the frame's events.
    FrameService.get().register(frame)
