import ctypes, typing, collections.abc as abc

from . import SDL_VA_LIST, SDL_FUNC, SDL_BINARY

SDL_SetError: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetError", ctypes.c_bool, [ctypes.c_char_p, ...], SDL_BINARY]
SDL_SetErrorV: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetErrorV", ctypes.c_bool, [ctypes.c_char_p, SDL_VA_LIST], SDL_BINARY]
SDL_OutOfMemory: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OutOfMemory", ctypes.c_bool, [], SDL_BINARY]
SDL_GetError: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetError", ctypes.c_char_p, [], SDL_BINARY]
SDL_ClearError: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ClearError", ctypes.c_bool, [], SDL_BINARY]

def SDL_Unsupported() -> ctypes.c_int:
    return SDL_SetError("That operation is not supported".encode())

def SDL_InvalidParamError(param: ctypes.c_char_p) -> ctypes.c_int:
    return SDL_SetError("Parameter '%s' is invalid".encode(), param)