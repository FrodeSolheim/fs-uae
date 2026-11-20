from enum import StrEnum, auto


class NotificationType(StrEnum):
    ASPECT_RATIO = auto()
    MOUSE_GRABBED = auto()
    # A file given on the command line cannot be started (no emulation is running).
    START_ERROR = auto()
