import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_FUNC_TYPE, \
    SDL_FUNC, SDL_TYPE, SDL_BINARY, SDL_ENUM
from .SDL_video import SDL_Window
from .SDL_properties import SDL_PropertiesID

class SDL_DialogFileFilter(ctypes.Structure):
    _fields_ = [
        ("name", ctypes.c_char_p),
        ("pattern", ctypes.c_char_p)
    ]

SDL_DialogFileCallback: typing.TypeAlias = SDL_FUNC_TYPE["SDL_DialogFileCallback", None, [ctypes.c_void_p, SDL_POINTER[ctypes.c_char_p], ctypes.c_int]]

SDL_ShowOpenFileDialog: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShowOpenFileDialog", None, [SDL_DialogFileCallback, ctypes.c_void_p, SDL_POINTER[SDL_Window], SDL_POINTER[SDL_DialogFileFilter], ctypes.c_int, ctypes.c_char_p, ctypes.c_bool], SDL_BINARY]
SDL_ShowSaveFileDialog: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShowSaveFileDialog", None, [SDL_DialogFileCallback, ctypes.c_void_p, SDL_POINTER[SDL_Window], SDL_POINTER[SDL_DialogFileFilter], ctypes.c_int, ctypes.c_char_p], SDL_BINARY]
SDL_ShowOpenFolderDialog: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShowOpenFolderDialog", None, [SDL_DialogFileCallback, ctypes.c_void_p, SDL_POINTER[SDL_Window], ctypes.c_char_p, ctypes.c_bool], SDL_BINARY]

SDL_FileDialogType: typing.TypeAlias = SDL_TYPE["SDL_FileDialogType", SDL_ENUM]

SDL_FILEDIALOG_OPENFILE, SDL_FILEDIALOG_SAVEFILE, SDL_FILEDIALOG_OPENFOLDER = range(3)

SDL_ShowFileDialogWithProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShowFileDialogWithProperties", None, [SDL_FileDialogType, SDL_DialogFileCallback, ctypes.c_void_p, SDL_PropertiesID], SDL_BINARY]

SDL_PROP_FILE_DIALOG_FILTERS_POINTER: bytes = "SDL.filedialog.filters".encode()
SDL_PROP_FILE_DIALOG_NFILTERS_NUMBER: bytes = "SDL.filedialog.nfilters".encode()
SDL_PROP_FILE_DIALOG_WINDOW_POINTER: bytes = "SDL.filedialog.window".encode()
SDL_PROP_FILE_DIALOG_LOCATION_STRING: bytes = "SDL.filedialog.location".encode()
SDL_PROP_FILE_DIALOG_MANY_BOOLEAN: bytes = "SDL.filedialog.many".encode()
SDL_PROP_FILE_DIALOG_TITLE_STRING: bytes = "SDL.filedialog.title".encode()
SDL_PROP_FILE_DIALOG_ACCEPT_STRING: bytes = "SDL.filedialog.accept".encode()
SDL_PROP_FILE_DIALOG_CANCEL_STRING: bytes = "SDL.filedialog.cancel".encode()