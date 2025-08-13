import ctypes, typing, collections.abc as abc

from . import SDL_FUNC, SDL_BINARY

SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION = 3, 2, 18

SDL_VERSIONNUM: abc.Callable[[int, int, int], int] = \
    lambda major, minor, patch: major * 1000000 + minor * 1000 + patch

SDL_VERSIONNUM_MAJOR: abc.Callable[[int], int] = lambda version: int(version / 1000000)
SDL_VERSIONNUM_MINOR: abc.Callable[[int], int] = lambda version: int(version / 1000) % 1000
SDL_VERSIONNUM_MICRO: abc.Callable[[int], int] = lambda version: version % 1000

SDL_VERSION: int = SDL_VERSIONNUM(SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION)
SDL_VERSION_ATLEAST: abc.Callable[[int, int, int], bool] = lambda x, y, z: SDL_VERSION >= SDL_VERSIONNUM(x, y, z)

SDL_GetVersion: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetVersion", ctypes.c_int, [], SDL_BINARY]
SDL_GetRevision: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRevision", ctypes.c_char_p, [], SDL_BINARY]