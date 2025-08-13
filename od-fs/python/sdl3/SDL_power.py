import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_ENUM, SDL_FUNC, SDL_TYPE, SDL_BINARY

SDL_PowerState: typing.TypeAlias = SDL_TYPE["SDL_PowerState", SDL_ENUM]

SDL_POWERSTATE_ERROR, SDL_POWERSTATE_UNKNOWN, SDL_POWERSTATE_ON_BATTERY, \
    SDL_POWERSTATE_NO_BATTERY, SDL_POWERSTATE_CHARGING, SDL_POWERSTATE_CHARGED = range(-1, 5)

SDL_GetPowerInfo: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetPowerInfo", SDL_PowerState, [SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int]], SDL_BINARY]