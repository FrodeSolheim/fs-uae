import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_FUNC, SDL_BINARY

class SDL_Locale(ctypes.Structure):
    _fields_ = [
        ("language", ctypes.c_char_p),
        ("country", ctypes.c_char_p)
    ]

SDL_GetPreferredLocales: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetPreferredLocales", SDL_POINTER[SDL_POINTER[SDL_Locale]], [SDL_POINTER[ctypes.c_int]], SDL_BINARY]