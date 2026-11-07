import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_TYPE, \
    SDL_FUNC, SDL_FUNC_TYPE, SDL_BINARY
from .SDL_surface import SDL_Surface

class SDL_Tray(ctypes.c_void_p):
    ...

class SDL_TrayMenu(ctypes.c_void_p):
    ...

class SDL_TrayEntry(ctypes.c_void_p):
    ...

SDL_TrayEntryFlags: typing.TypeAlias = SDL_TYPE["SDL_TrayEntryFlags", ctypes.c_uint32]

SDL_TRAYENTRY_BUTTON, SDL_TRAYENTRY_CHECKBOX, SDL_TRAYENTRY_SUBMENU, SDL_TRAYENTRY_DISABLED, \
    SDL_TRAYENTRY_CHECKED = 0x00000001, 0x00000002, 0x00000004, 0x80000000, 0x40000000

SDL_TrayCallback: typing.TypeAlias = SDL_FUNC_TYPE["SDL_TrayCallback", None, [ctypes.c_void_p, SDL_POINTER[SDL_TrayEntry]]]

SDL_CreateTray: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateTray", SDL_POINTER[SDL_Tray], [SDL_POINTER[SDL_Surface], ctypes.c_char_p], SDL_BINARY]
SDL_SetTrayIcon: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetTrayIcon", None, [SDL_POINTER[SDL_Tray], SDL_POINTER[SDL_Surface]], SDL_BINARY]
SDL_SetTrayTooltip: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetTrayTooltip", None, [SDL_POINTER[SDL_Tray], ctypes.c_char_p], SDL_BINARY]
SDL_CreateTrayMenu: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateTrayMenu", SDL_POINTER[SDL_TrayMenu], [SDL_POINTER[SDL_Tray]], SDL_BINARY]
SDL_CreateTraySubmenu: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateTraySubmenu", SDL_POINTER[SDL_TrayMenu], [SDL_POINTER[SDL_TrayEntry]], SDL_BINARY]
SDL_GetTrayMenu: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTrayMenu", SDL_POINTER[SDL_TrayMenu], [SDL_POINTER[SDL_Tray]], SDL_BINARY]
SDL_GetTraySubmenu: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTraySubmenu", SDL_POINTER[SDL_TrayMenu], [SDL_POINTER[SDL_TrayEntry]], SDL_BINARY]
SDL_GetTrayEntries: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTrayEntries", SDL_POINTER[SDL_POINTER[SDL_TrayEntry]], [SDL_POINTER[SDL_TrayMenu], SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_RemoveTrayEntry: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RemoveTrayEntry", None, [SDL_POINTER[SDL_TrayEntry]], SDL_BINARY]
SDL_InsertTrayEntryAt: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_InsertTrayEntryAt", SDL_POINTER[SDL_TrayEntry], [SDL_POINTER[SDL_TrayMenu], ctypes.c_int, ctypes.c_char_p, SDL_TrayEntryFlags], SDL_BINARY]
SDL_SetTrayEntryLabel: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetTrayEntryLabel", None, [SDL_POINTER[SDL_TrayEntry], ctypes.c_char_p], SDL_BINARY]
SDL_GetTrayEntryLabel: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTrayEntryLabel", ctypes.c_char_p, [SDL_POINTER[SDL_TrayEntry]], SDL_BINARY]
SDL_SetTrayEntryChecked: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetTrayEntryChecked", None, [SDL_POINTER[SDL_TrayEntry], ctypes.c_bool], SDL_BINARY]
SDL_GetTrayEntryChecked: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTrayEntryChecked", ctypes.c_bool, [SDL_POINTER[SDL_TrayEntry]], SDL_BINARY]
SDL_SetTrayEntryEnabled: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetTrayEntryEnabled", None, [SDL_POINTER[SDL_TrayEntry], ctypes.c_bool], SDL_BINARY]
SDL_GetTrayEntryEnabled: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTrayEntryEnabled", ctypes.c_bool, [SDL_POINTER[SDL_TrayEntry]], SDL_BINARY]
SDL_SetTrayEntryCallback: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetTrayEntryCallback", None, [SDL_POINTER[SDL_TrayEntry], SDL_TrayCallback, ctypes.c_void_p], SDL_BINARY]
SDL_ClickTrayEntry: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ClickTrayEntry", None, [SDL_POINTER[SDL_TrayEntry]], SDL_BINARY]
SDL_DestroyTray: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DestroyTray", None, [SDL_POINTER[SDL_Tray]], SDL_BINARY]
SDL_GetTrayEntryParent: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTrayEntryParent", SDL_POINTER[SDL_TrayMenu], [SDL_POINTER[SDL_TrayEntry]], SDL_BINARY]
SDL_GetTrayMenuParentEntry: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTrayMenuParentEntry", SDL_POINTER[SDL_TrayEntry], [SDL_POINTER[SDL_TrayMenu]], SDL_BINARY]
SDL_GetTrayMenuParentTray: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTrayMenuParentTray", SDL_POINTER[SDL_Tray], [SDL_POINTER[SDL_TrayMenu]], SDL_BINARY]
SDL_UpdateTrays: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UpdateTrays", None, [], SDL_BINARY]