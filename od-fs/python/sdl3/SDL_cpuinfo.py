import ctypes, typing, collections.abc as abc

from . import SDL_FUNC, SDL_BINARY

SDL_CACHELINE_SIZE: int = 128

SDL_GetNumLogicalCPUCores: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetNumLogicalCPUCores", ctypes.c_int, [], SDL_BINARY]
SDL_GetCPUCacheLineSize: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetCPUCacheLineSize", ctypes.c_int, [], SDL_BINARY]

SDL_HasAltiVec: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasAltiVec", ctypes.c_bool, [], SDL_BINARY]
SDL_HasMMX: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasMMX", ctypes.c_bool, [], SDL_BINARY]
SDL_HasSSE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasSSE", ctypes.c_bool, [], SDL_BINARY]
SDL_HasSSE2: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasSSE2", ctypes.c_bool, [], SDL_BINARY]
SDL_HasSSE3: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasSSE3", ctypes.c_bool, [], SDL_BINARY]
SDL_HasSSE41: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasSSE41", ctypes.c_bool, [], SDL_BINARY]
SDL_HasSSE42: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasSSE42", ctypes.c_bool, [], SDL_BINARY]
SDL_HasAVX: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasAVX", ctypes.c_bool, [], SDL_BINARY]
SDL_HasAVX2: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasAVX2", ctypes.c_bool, [], SDL_BINARY]
SDL_HasAVX512F: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasAVX512F", ctypes.c_bool, [], SDL_BINARY]
SDL_HasARMSIMD: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasARMSIMD", ctypes.c_bool, [], SDL_BINARY]
SDL_HasNEON: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasNEON", ctypes.c_bool, [], SDL_BINARY]
SDL_HasLSX: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasLSX", ctypes.c_bool, [], SDL_BINARY]
SDL_HasLASX: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasLASX", ctypes.c_bool, [], SDL_BINARY]

SDL_GetSystemRAM: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSystemRAM", ctypes.c_int, [], SDL_BINARY]
SDL_GetSIMDAlignment: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSIMDAlignment", ctypes.c_size_t, [], SDL_BINARY]