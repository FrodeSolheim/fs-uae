import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_FUNC_TYPE, \
    SDL_FUNC, SDL_TYPE, SDL_BINARY, SDL_ENUM
from .SDL_stdinc import SDL_Time

SDL_GetBasePath: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetBasePath", ctypes.c_char_p, [], SDL_BINARY]
SDL_GetPrefPath: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetPrefPath", ctypes.c_char_p, [ctypes.c_char_p, ctypes.c_char_p], SDL_BINARY]

SDL_Folder: typing.TypeAlias = SDL_TYPE["SDL_Folder", SDL_ENUM]

SDL_FOLDER_HOME, SDL_FOLDER_DESKTOP, SDL_FOLDER_DOCUMENTS, SDL_FOLDER_DOWNLOADS, SDL_FOLDER_MUSIC, SDL_FOLDER_PICTURES, SDL_FOLDER_PUBLICSHARE, \
    SDL_FOLDER_SAVEDGAMES, SDL_FOLDER_SCREENSHOTS, SDL_FOLDER_TEMPLATES, SDL_FOLDER_VIDEOS , SDL_FOLDER_COUNT = range(12)

SDL_GetUserFolder: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetUserFolder", ctypes.c_char_p, [SDL_Folder], SDL_BINARY]

SDL_PathType: typing.TypeAlias = SDL_TYPE["SDL_PathType", SDL_ENUM]

SDL_PATHTYPE_NONE, SDL_PATHTYPE_FILE, SDL_PATHTYPE_DIRECTORY, SDL_PATHTYPE_OTHER = range(4)

class SDL_PathInfo(ctypes.Structure):
    _fields_ = [
        ("type", SDL_PathType),
        ("size", ctypes.c_uint64),
        ("create_time", SDL_Time),
        ("modify_time", SDL_Time),
        ("access_time", SDL_Time)
    ]

SDL_GlobFlags: typing.TypeAlias = SDL_TYPE["SDL_GlobFlags", ctypes.c_uint32]

SDL_GLOB_CASEINSENSITIVE: int = 1 << 0

SDL_CreateDirectory: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateDirectory", ctypes.c_bool, [ctypes.c_char_p], SDL_BINARY]

SDL_EnumerationResult: typing.TypeAlias = SDL_TYPE["SDL_EnumerationResult", SDL_ENUM]

SDL_ENUM_CONTINUE, SDL_ENUM_SUCCESS, SDL_ENUM_FAILURE = range(3)

SDL_EnumerateDirectoryCallback: typing.TypeAlias = SDL_FUNC_TYPE["SDL_EnumerateDirectoryCallback", SDL_EnumerationResult, [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_char_p]]

SDL_EnumerateDirectory: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_EnumerateDirectory", ctypes.c_bool, [ctypes.c_char_p, SDL_EnumerateDirectoryCallback, ctypes.c_void_p], SDL_BINARY]
SDL_RemovePath: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RemovePath", ctypes.c_bool, [ctypes.c_char_p], SDL_BINARY]
SDL_RenamePath: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenamePath", ctypes.c_bool, [ctypes.c_char_p, ctypes.c_char_p], SDL_BINARY]
SDL_CopyFile: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CopyFile", ctypes.c_bool, [ctypes.c_char_p, ctypes.c_char_p], SDL_BINARY]
SDL_GetPathInfo: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetPathInfo", ctypes.c_bool, [ctypes.c_char_p, SDL_POINTER[SDL_PathInfo]], SDL_BINARY]
SDL_GlobDirectory: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GlobDirectory", SDL_POINTER[ctypes.c_char_p], [ctypes.c_char_p, ctypes.c_char_p, SDL_GlobFlags, SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_GetCurrentDirectory: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetCurrentDirectory", ctypes.c_char_p, [], SDL_BINARY]