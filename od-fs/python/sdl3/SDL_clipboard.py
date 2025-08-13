import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_FUNC_TYPE, SDL_FUNC, SDL_BINARY

SDL_SetClipboardText: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetClipboardText", ctypes.c_bool, [ctypes.c_char_p], SDL_BINARY]
SDL_GetClipboardText: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetClipboardText", ctypes.c_char_p, [], SDL_BINARY]
SDL_HasClipboardText: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasClipboardText", ctypes.c_bool, [], SDL_BINARY]

SDL_SetPrimarySelectionText: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetPrimarySelectionText", ctypes.c_bool, [ctypes.c_char_p], SDL_BINARY]
SDL_GetPrimarySelectionText: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetPrimarySelectionText", ctypes.c_char_p, [], SDL_BINARY]
SDL_HasPrimarySelectionText: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasPrimarySelectionText", ctypes.c_bool, [], SDL_BINARY]

SDL_ClipboardDataCallback: typing.TypeAlias = SDL_FUNC_TYPE["SDL_ClipboardDataCallback", ctypes.c_void_p, [ctypes.c_char_p, SDL_POINTER[ctypes.c_size_t]]]
SDL_ClipboardCleanupCallback: typing.TypeAlias = SDL_FUNC_TYPE["SDL_ClipboardCleanupCallback", None, [ctypes.c_void_p]]

SDL_SetClipboardData: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetClipboardData", ctypes.c_bool, [SDL_ClipboardDataCallback, SDL_ClipboardCleanupCallback, ctypes.c_void_p, SDL_POINTER[ctypes.c_char_p], ctypes.c_size_t], SDL_BINARY]
SDL_ClearClipboardData: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ClearClipboardData", ctypes.c_bool, [], SDL_BINARY]
SDL_GetClipboardData: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetClipboardData", ctypes.c_void_p, [ctypes.c_char_p, SDL_POINTER[ctypes.c_size_t]], SDL_BINARY]
SDL_HasClipboardData: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasClipboardData", ctypes.c_bool, [ctypes.c_char_p], SDL_BINARY]