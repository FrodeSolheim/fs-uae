import ctypes, typing, collections.abc as abc

from . import SDL_FUNC_TYPE, SDL_FUNC, SDL_TYPE, SDL_BINARY

SDL_MS_PER_SECOND: int = 1000
SDL_US_PER_SECOND: int = 1000000
SDL_NS_PER_SECOND: int = 1000000000
SDL_NS_PER_MS: int = 1000000
SDL_NS_PER_US: int = 1000

SDL_SECONDS_TO_NS: abc.Callable[[int | float], int | float] = lambda s: s * SDL_NS_PER_SECOND
SDL_NS_TO_SECONDS: abc.Callable[[int | float], float] = lambda ns: ns / SDL_NS_PER_SECOND
SDL_MS_TO_NS: abc.Callable[[int | float], int | float] = lambda ms: ms * SDL_NS_PER_MS
SDL_NS_TO_MS: abc.Callable[[int | float], float] = lambda ns: ns / SDL_NS_PER_MS
SDL_US_TO_NS: abc.Callable[[int | float], int | float] = lambda us: us * SDL_NS_PER_US
SDL_NS_TO_US: abc.Callable[[int | float], float] = lambda ns: ns / SDL_NS_PER_US

SDL_GetTicks: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTicks", ctypes.c_uint64, [], SDL_BINARY]
SDL_GetTicksNS: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTicksNS", ctypes.c_uint64, [], SDL_BINARY]
SDL_GetPerformanceCounter: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetPerformanceCounter", ctypes.c_uint64, [], SDL_BINARY]
SDL_GetPerformanceFrequency: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetPerformanceFrequency", ctypes.c_uint64, [], SDL_BINARY]
SDL_Delay: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_Delay", None, [ctypes.c_uint32], SDL_BINARY]
SDL_DelayNS: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DelayNS", None, [ctypes.c_uint64], SDL_BINARY]
SDL_DelayPrecise: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DelayPrecise", None, [ctypes.c_uint64], SDL_BINARY]

SDL_TimerID: typing.TypeAlias = SDL_TYPE["SDL_TimerID", ctypes.c_uint32]
SDL_TimerCallback: typing.TypeAlias = SDL_FUNC_TYPE["SDL_TimerCallback", ctypes.c_uint32, [ctypes.c_void_p, SDL_TimerID, ctypes.c_uint32]]

SDL_AddTimer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_AddTimer", SDL_TimerID, [ctypes.c_uint32, SDL_TimerCallback, ctypes.c_void_p], SDL_BINARY]

SDL_NSTimerCallback: typing.TypeAlias = SDL_FUNC_TYPE["SDL_NSTimerCallback", ctypes.c_uint64, [ctypes.c_void_p, SDL_TimerID, ctypes.c_uint64]]

SDL_AddTimerNS: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_AddTimerNS", SDL_TimerID, [ctypes.c_uint64, SDL_NSTimerCallback, ctypes.c_void_p], SDL_BINARY]
SDL_RemoveTimer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RemoveTimer", ctypes.c_bool, [SDL_TimerID], SDL_BINARY]