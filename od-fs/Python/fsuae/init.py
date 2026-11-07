import logging

logger = logging.getLogger(__name__)


def init():
    """This function is called from fsapp-pythonthread.c"""

    logger.info("FS-UAE Init (Python)")

    # raise Exception("test")

    # FIXME: Set up logging via _fsapp (-> SDL_Log)

    from fsapp import fsapp_init

    fsapp_init()

    from fsgui import fsgui_init

    fsgui_init()

    from app.app_init import app_init

    app_init()
