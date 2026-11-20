import logging

logger = logging.getLogger(__name__)


def init():
    """FS-UAE's Python start-up, run first on the UI thread.

    fsuae-main.cpp gives this function to fsapp_pythonthread_start as the UI thread's entry point.
    """

    logger.info("FS-UAE Init (Python)")

    # raise Exception("test")

    from fsapp import fsapp_init

    fsapp_init()

    from fsgui import fsgui_init

    fsgui_init()

    from app.app_init import app_init

    app_init()
