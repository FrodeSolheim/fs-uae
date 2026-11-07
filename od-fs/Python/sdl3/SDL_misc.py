import ctypes, typing, collections.abc as abc

from . import SDL_FUNC, SDL_BINARY

SDL_OpenURL: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OpenURL", ctypes.c_bool, [ctypes.c_char_p], SDL_BINARY]