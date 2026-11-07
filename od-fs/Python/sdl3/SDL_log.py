import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_VA_LIST, SDL_TYPE, \
    SDL_FUNC, SDL_FUNC_TYPE, SDL_BINARY, SDL_ENUM

SDL_LogCategory: typing.TypeAlias = SDL_TYPE["SDL_LogCategory", SDL_ENUM]

SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_CATEGORY_ERROR, SDL_LOG_CATEGORY_ASSERT, SDL_LOG_CATEGORY_SYSTEM, SDL_LOG_CATEGORY_AUDIO, \
    SDL_LOG_CATEGORY_VIDEO, SDL_LOG_CATEGORY_RENDER, SDL_LOG_CATEGORY_INPUT, SDL_LOG_CATEGORY_TEST, SDL_LOG_CATEGORY_GPU, SDL_LOG_CATEGORY_RESERVED1, \
        SDL_LOG_CATEGORY_RESERVED2, SDL_LOG_CATEGORY_RESERVED3, SDL_LOG_CATEGORY_RESERVED4, SDL_LOG_CATEGORY_RESERVED5, SDL_LOG_CATEGORY_RESERVED6, SDL_LOG_CATEGORY_RESERVED7, \
            SDL_LOG_CATEGORY_RESERVED8, SDL_LOG_CATEGORY_RESERVED9, SDL_LOG_CATEGORY_RESERVED10, SDL_LOG_CATEGORY_CUSTOM = range(21)

SDL_LogPriority: typing.TypeAlias = SDL_TYPE["SDL_LogPriority", SDL_ENUM]

SDL_LOG_PRIORITY_INVALID, SDL_LOG_PRIORITY_TRACE, SDL_LOG_PRIORITY_VERBOSE, SDL_LOG_PRIORITY_DEBUG, SDL_LOG_PRIORITY_INFO, \
    SDL_LOG_PRIORITY_WARN, SDL_LOG_PRIORITY_ERROR, SDL_LOG_PRIORITY_CRITICAL, SDL_LOG_PRIORITY_COUNT = range(9)

SDL_SetLogPriorities: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetLogPriorities", None, [SDL_LogPriority], SDL_BINARY]
SDL_SetLogPriority: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetLogPriority", None, [ctypes.c_int, SDL_LogPriority], SDL_BINARY]
SDL_GetLogPriority: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetLogPriority", SDL_LogPriority, [ctypes.c_int], SDL_BINARY]
SDL_ResetLogPriorities: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ResetLogPriorities", None, [], SDL_BINARY]
SDL_SetLogPriorityPrefix: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetLogPriorityPrefix", ctypes.c_bool, [SDL_LogPriority, ctypes.c_char_p], SDL_BINARY]

SDL_Log: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_Log", None, [ctypes.c_char_p, ...], SDL_BINARY]
SDL_LogTrace: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LogTrace", None, [ctypes.c_int, ctypes.c_char_p, ...], SDL_BINARY]
SDL_LogVerbose: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LogVerbose", None, [ctypes.c_int, ctypes.c_char_p, ...], SDL_BINARY]
SDL_LogDebug: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LogDebug", None, [ctypes.c_int, ctypes.c_char_p, ...], SDL_BINARY]
SDL_LogInfo: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LogInfo", None, [ctypes.c_int, ctypes.c_char_p, ...], SDL_BINARY]
SDL_LogWarn: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LogWarn", None, [ctypes.c_int, ctypes.c_char_p, ...], SDL_BINARY]
SDL_LogError: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LogError", None, [ctypes.c_int, ctypes.c_char_p, ...], SDL_BINARY]
SDL_LogCritical: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LogCritical", None, [ctypes.c_int, ctypes.c_char_p, ...], SDL_BINARY]
SDL_LogMessage: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LogMessage", None, [ctypes.c_int, SDL_LogPriority, ctypes.c_char_p, ...], SDL_BINARY]
SDL_LogMessageV: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LogMessageV", None, [ctypes.c_int, SDL_LogPriority, ctypes.c_char_p, SDL_VA_LIST], SDL_BINARY]

SDL_LogOutputFunction: typing.TypeAlias = SDL_FUNC_TYPE["SDL_LogOutputFunction", None, [ctypes.c_void_p, ctypes.c_int, SDL_LogPriority, ctypes.c_char_p]]

SDL_GetDefaultLogOutputFunction: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetDefaultLogOutputFunction", SDL_LogOutputFunction, [], SDL_BINARY]
SDL_GetLogOutputFunction: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetLogOutputFunction", None, [SDL_POINTER[SDL_LogOutputFunction], SDL_POINTER[ctypes.c_void_p]], SDL_BINARY]
SDL_SetLogOutputFunction: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetLogOutputFunction", None, [SDL_LogOutputFunction, ctypes.c_void_p], SDL_BINARY]