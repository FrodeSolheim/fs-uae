import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_ENUM, SDL_FUNC, SDL_TYPE, SDL_BINARY
from .SDL_video import SDL_Window

SDL_MessageBoxFlags: typing.TypeAlias = SDL_TYPE["SDL_MessageBoxFlags", ctypes.c_uint32]

SDL_MESSAGEBOX_ERROR: int = 0x00000010
SDL_MESSAGEBOX_WARNING: int = 0x00000020
SDL_MESSAGEBOX_INFORMATION: int = 0x00000040
SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT: int = 0x00000080
SDL_MESSAGEBOX_BUTTONS_RIGHT_TO_LEFT: int = 0x00000100

SDL_MessageBoxButtonFlags: typing.TypeAlias = SDL_TYPE["SDL_MessageBoxButtonFlags", ctypes.c_uint32]

SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT: int = 0x00000001
SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT: int = 0x00000002

class SDL_MessageBoxButtonData(ctypes.Structure):
    _fields_ = [
        ("flags", SDL_MessageBoxButtonFlags),
        ("buttonID", ctypes.c_int),
        ("text", ctypes.c_char_p)
    ]

class SDL_MessageBoxColor(ctypes.Structure):
    _fields_ = [
        ("r", ctypes.c_uint8),
        ("g", ctypes.c_uint8),
        ("b", ctypes.c_uint8)
    ]

SDL_MessageBoxColorType: typing.TypeAlias = SDL_TYPE["SDL_MessageBoxColorType", SDL_ENUM]

SDL_MESSAGEBOX_COLOR_BACKGROUND, SDL_MESSAGEBOX_COLOR_TEXT, SDL_MESSAGEBOX_COLOR_BUTTON_BORDER, \
    SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND, SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED, SDL_MESSAGEBOX_COLOR_COUNT = range(6)

class SDL_MessageBoxColorScheme(ctypes.Structure):
    _fields_ = [
        ("colors", SDL_MessageBoxColor * SDL_MESSAGEBOX_COLOR_COUNT)
    ]

class SDL_MessageBoxData(ctypes.Structure):
    _fields_ = [
        ("flags", SDL_MessageBoxFlags),
        ("window", SDL_POINTER[SDL_Window]),
        ("title", ctypes.c_char_p),
        ("message", ctypes.c_char_p),
        ("numbuttons", ctypes.c_int),
        ("buttons", SDL_POINTER[SDL_MessageBoxButtonData]),
        ("colorScheme", SDL_POINTER[SDL_MessageBoxColorScheme])
    ]

SDL_ShowMessageBox: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShowMessageBox", ctypes.c_bool, [SDL_POINTER[SDL_MessageBoxData], SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_ShowSimpleMessageBox: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShowSimpleMessageBox", ctypes.c_bool, [SDL_MessageBoxFlags, ctypes.c_char_p, ctypes.c_char_p, SDL_POINTER[SDL_Window]], SDL_BINARY]