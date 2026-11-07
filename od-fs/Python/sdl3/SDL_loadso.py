import ctypes, typing, collections.abc as abc

from . import  SDL_POINTER, SDL_FUNC, SDL_BINARY
from .SDL_stdinc import SDL_FunctionPointer

class SDL_SharedObject(ctypes.c_void_p):
    ...

SDL_LoadObject: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LoadObject", SDL_POINTER[SDL_SharedObject], [ctypes.c_char_p], SDL_BINARY]
SDL_LoadFunction: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LoadFunction", SDL_FunctionPointer, [SDL_POINTER[SDL_SharedObject], ctypes.c_char_p], SDL_BINARY]
SDL_UnloadObject: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UnloadObject", None, [SDL_POINTER[SDL_SharedObject]], SDL_BINARY]