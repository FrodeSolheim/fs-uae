import logging

logger = logging.getLogger(__name__)


def init():
    logger.info("FS-UAE Init (Python)")

    # import os

    # os.environ["SDL_DISABLE_METADATA"] = "1"
    # os.environ["SDL_BINARY_PATH"] = "fsenv/_prefix/lib"
    # os.environ["SDL_CHECK_BINARY_VERSION"] = "0"
    # os.environ["SDL_IGNORE_MISSING_FUNCTIONS"] = "1"
    # os.environ["SDL_FIND_BINARIES"] = "1"

    # # from sdl3 import SDL3
    # # print(SDL3.SDL_GetVERSION())

    # import sdl3

    # print("SDL3 version as reported by PySDL3:", sdl3.SDL_GetVersion())
