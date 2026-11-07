import ctypes, typing, collections.abc as abc

from . import  SDL_FUNC, SDL_BINARY

SDL_GetPlatform: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetPlatform", ctypes.c_char_p, [], SDL_BINARY]