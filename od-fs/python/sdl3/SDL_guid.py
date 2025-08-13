import ctypes, typing, collections.abc as abc

from . import SDL_FUNC, SDL_BINARY

class SDL_GUID(ctypes.Structure):
    _fields_ = [
        ("data", ctypes.c_uint8 * 16)
    ]

SDL_GUIDToString: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GUIDToString", None, [SDL_GUID, ctypes.c_char_p, ctypes.c_int], SDL_BINARY]
SDL_StringToGUID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_StringToGUID", SDL_GUID, [ctypes.c_char_p], SDL_BINARY]