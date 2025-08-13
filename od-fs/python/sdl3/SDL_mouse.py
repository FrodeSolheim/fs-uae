import ctypes, typing, collections.abc as abc

from . import SDL_FUNC_TYPE, SDL_POINTER, \
    SDL_ENUM, SDL_FUNC, SDL_TYPE, SDL_BINARY
from .SDL_video import SDL_Window
from .SDL_surface import SDL_Surface

SDL_MouseID: typing.TypeAlias = SDL_TYPE["SDL_MouseID", ctypes.c_uint32]

class SDL_Cursor(ctypes.c_void_p):
    ...

SDL_SystemCursor: typing.TypeAlias = SDL_TYPE["SDL_SystemCursor", SDL_ENUM]

SDL_SYSTEM_CURSOR_DEFAULT, SDL_SYSTEM_CURSOR_TEXT, SDL_SYSTEM_CURSOR_WAIT, SDL_SYSTEM_CURSOR_CROSSHAIR, SDL_SYSTEM_CURSOR_PROGRESS, SDL_SYSTEM_CURSOR_NWSE_RESIZE, \
    SDL_SYSTEM_CURSOR_NESW_RESIZE, SDL_SYSTEM_CURSOR_EW_RESIZE, SDL_SYSTEM_CURSOR_NS_RESIZE, SDL_SYSTEM_CURSOR_MOVE, SDL_SYSTEM_CURSOR_NOT_ALLOWED, SDL_SYSTEM_CURSOR_POINTER, \
        SDL_SYSTEM_CURSOR_NW_RESIZE, SDL_SYSTEM_CURSOR_N_RESIZE, SDL_SYSTEM_CURSOR_NE_RESIZE, SDL_SYSTEM_CURSOR_E_RESIZE, SDL_SYSTEM_CURSOR_SE_RESIZE, SDL_SYSTEM_CURSOR_S_RESIZE, \
            SDL_SYSTEM_CURSOR_SW_RESIZE, SDL_SYSTEM_CURSOR_W_RESIZE, SDL_SYSTEM_CURSOR_COUNT = range(21)

SDL_MouseWheelDirection: typing.TypeAlias = SDL_TYPE["SDL_MouseWheelDirection", SDL_ENUM]

SDL_MOUSEWHEEL_NORMAL, SDL_MOUSEWHEEL_FLIPPED = range(2)

SDL_MouseButtonFlags: typing.TypeAlias = SDL_TYPE["SDL_MouseButtonFlags", ctypes.c_uint32]

SDL_MouseMotionTransformCallback: typing.TypeAlias = SDL_FUNC_TYPE["SDL_MouseMotionTransformCallback", None, [ctypes.c_void_p, ctypes.c_uint64, SDL_POINTER[SDL_Window], SDL_MouseID, SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float]]]

SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, SDL_BUTTON_RIGHT, SDL_BUTTON_X1, SDL_BUTTON_X2 = range(1, 6)

SDL_BUTTON_MASK: abc.Callable[..., int] = lambda x: 1 << (x - 1)

SDL_BUTTON_LMASK, SDL_BUTTON_MMASK, SDL_BUTTON_RMASK, \
    SDL_BUTTON_X1MASK, SDL_BUTTON_X2MASK = [SDL_BUTTON_MASK(x) for x in range(1, 6)]

SDL_HasMouse: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasMouse", ctypes.c_bool, [], SDL_BINARY]
SDL_GetMice: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetMice", SDL_POINTER[SDL_MouseID], [SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_GetMouseNameForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetMouseNameForID", ctypes.c_char_p, [SDL_MouseID], SDL_BINARY]
SDL_GetMouseFocus: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetMouseFocus", SDL_POINTER[SDL_Window], [], SDL_BINARY]
SDL_GetMouseState: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetMouseState", SDL_MouseButtonFlags, [SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float]], SDL_BINARY]
SDL_GetGlobalMouseState: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGlobalMouseState", SDL_MouseButtonFlags, [SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float]], SDL_BINARY]
SDL_GetRelativeMouseState: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRelativeMouseState", SDL_MouseButtonFlags, [SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float]], SDL_BINARY]
SDL_WarpMouseInWindow: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WarpMouseInWindow", None, [SDL_POINTER[SDL_Window], ctypes.c_float, ctypes.c_float], SDL_BINARY]
SDL_WarpMouseGlobal: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WarpMouseGlobal", ctypes.c_bool, [ctypes.c_float, ctypes.c_float], SDL_BINARY]
SDL_SetWindowRelativeMouseMode: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowRelativeMouseMode", ctypes.c_bool, [SDL_POINTER[SDL_Window], ctypes.c_bool], SDL_BINARY]
SDL_GetWindowRelativeMouseMode: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowRelativeMouseMode", ctypes.c_bool, [SDL_POINTER[SDL_Window]], SDL_BINARY]
SDL_CaptureMouse: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CaptureMouse", ctypes.c_bool, [ctypes.c_bool], SDL_BINARY]

SDL_CreateCursor: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateCursor", SDL_POINTER[SDL_Cursor], [SDL_POINTER[ctypes.c_uint8], SDL_POINTER[ctypes.c_uint8], ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int], SDL_BINARY]
SDL_CreateColorCursor: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateColorCursor", SDL_POINTER[SDL_Cursor], [SDL_POINTER[SDL_Surface], ctypes.c_int, ctypes.c_int], SDL_BINARY]
SDL_CreateSystemCursor: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateSystemCursor", SDL_POINTER[SDL_Cursor], [SDL_SystemCursor], SDL_BINARY]
SDL_SetCursor: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetCursor", ctypes.c_bool, [SDL_POINTER[SDL_Cursor]], SDL_BINARY]
SDL_GetCursor: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetCursor", SDL_POINTER[SDL_Cursor], [], SDL_BINARY]
SDL_GetDefaultCursor: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetDefaultCursor", SDL_POINTER[SDL_Cursor], [], SDL_BINARY]
SDL_DestroyCursor: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DestroyCursor", None, [SDL_POINTER[SDL_Cursor]], SDL_BINARY]
SDL_ShowCursor: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShowCursor", ctypes.c_bool, [], SDL_BINARY]
SDL_HideCursor: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HideCursor", ctypes.c_bool, [], SDL_BINARY]
SDL_CursorVisible: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CursorVisible", ctypes.c_bool, [], SDL_BINARY]