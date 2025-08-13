import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_FUNC_TYPE, \
    SDL_FUNC, SDL_TYPE, SDL_RTF_BINARY, SDL_ENUM
from .SDL_pixels import SDL_Color
from .SDL_iostream import SDL_IOStream
from .SDL_error import SDL_SetError, SDL_GetError
from .SDL_render import SDL_Renderer, SDL_Texture
from .SDL_version import SDL_VERSIONNUM
from .SDL_rect import SDL_Rect

SDL_RTF_MAJOR_VERSION, SDL_RTF_MINOR_VERSION, SDL_RTF_MICRO_VERSION = 3, 0, 0
SDL_RTF_VERSION: int = SDL_VERSIONNUM(SDL_RTF_MAJOR_VERSION, SDL_RTF_MINOR_VERSION, SDL_RTF_MICRO_VERSION)

SDL_RTF_VERSION_ATLEAST: abc.Callable[[int, int, int], bool] = lambda x, y, z: \
    (SDL_RTF_MAJOR_VERSION >= x) and (SDL_RTF_MAJOR_VERSION > x or SDL_RTF_MINOR_VERSION >= y) and \
        (SDL_RTF_MAJOR_VERSION > x or SDL_RTF_MINOR_VERSION > y or SDL_RTF_MICRO_VERSION >= z)

RTF_Version: abc.Callable[..., typing.Any] = SDL_FUNC["RTF_Version", ctypes.c_int, [], SDL_RTF_BINARY]

class RTF_Context(ctypes.c_void_p):
    ...

RTF_FontFamily: typing.TypeAlias = SDL_TYPE["RTF_FontFamily", SDL_ENUM]

RTF_FontDefault, RTF_FontRoman, RTF_FontSwiss, RTF_FontModern, \
    RTF_FontScript, RTF_FontDecor, RTF_FontTech, RTF_FontBidi = range(8)

RTF_FontStyle: typing.TypeAlias = SDL_TYPE["RTF_FontStyle", SDL_ENUM]

RTF_FontNormal, RTF_FontBold, RTF_FontItalic, \
    RTF_FontUnderline = 0x00, 0x01, 0x02, 0x04

RTF_FONT_ENGINE_VERSION: int = 1

class RTF_FontEngine(ctypes.Structure):
    _fields_ = [
        ("version", ctypes.c_int),
        ("CreateFont", SDL_FUNC_TYPE["RTF_FontEngine.CreateFont", ctypes.c_void_p, [ctypes.c_char_p, RTF_FontFamily, ctypes.c_int, ctypes.c_int, ctypes.c_int]]),
        ("GetLineSpacing", SDL_FUNC_TYPE["RTF_FontEngine.GetLineSpacing", ctypes.c_int, [ctypes.c_void_p]]),
        ("GetCharacterOffsets", SDL_FUNC_TYPE["RTF_FontEngine.GetCharacterOffsets", ctypes.c_int, [ctypes.c_void_p, ctypes.c_char_p, SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int], ctypes.c_int]]),
        ("RenderText", SDL_FUNC_TYPE["RTF_FontEngine.RenderText", SDL_POINTER[SDL_Texture], [ctypes.c_void_p, SDL_POINTER[SDL_Renderer], ctypes.c_char_p, SDL_Color]]),
        ("FreeFont", SDL_FUNC_TYPE["RTF_FontEngine.FreeFont", None, [ctypes.c_void_p]])
    ]

RTF_CreateContext: abc.Callable[..., typing.Any] = SDL_FUNC["RTF_CreateContext", SDL_POINTER[RTF_Context], [SDL_POINTER[SDL_Renderer], SDL_POINTER[RTF_FontEngine]], SDL_RTF_BINARY]
RTF_Load: abc.Callable[..., typing.Any] = SDL_FUNC["RTF_Load", ctypes.c_int, [SDL_POINTER[RTF_Context], ctypes.c_char_p], SDL_RTF_BINARY]
RTF_Load_IO: abc.Callable[..., typing.Any] = SDL_FUNC["RTF_Load_IO", ctypes.c_int, [SDL_POINTER[RTF_Context], SDL_POINTER[SDL_IOStream], ctypes.c_int], SDL_RTF_BINARY]
RTF_GetTitle: abc.Callable[..., typing.Any] = SDL_FUNC["RTF_GetTitle", ctypes.c_char_p, [SDL_POINTER[RTF_Context]], SDL_RTF_BINARY]
RTF_GetSubject: abc.Callable[..., typing.Any] = SDL_FUNC["RTF_GetSubject", ctypes.c_char_p, [SDL_POINTER[RTF_Context]], SDL_RTF_BINARY]
RTF_GetAuthor: abc.Callable[..., typing.Any] = SDL_FUNC["RTF_GetAuthor", ctypes.c_char_p, [SDL_POINTER[RTF_Context]], SDL_RTF_BINARY]
RTF_GetHeight: abc.Callable[..., typing.Any] = SDL_FUNC["RTF_GetHeight", ctypes.c_int, [SDL_POINTER[RTF_Context], ctypes.c_int], SDL_RTF_BINARY]
RTF_Render: abc.Callable[..., typing.Any] = SDL_FUNC["RTF_Render", None, [SDL_POINTER[RTF_Context], SDL_POINTER[SDL_Rect], ctypes.c_int], SDL_RTF_BINARY]
RTF_FreeContext: abc.Callable[..., typing.Any] = SDL_FUNC["RTF_FreeContext", None, [SDL_POINTER[RTF_Context]], SDL_RTF_BINARY]

RTF_SetError, RTF_GetError = SDL_SetError, SDL_GetError