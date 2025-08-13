import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_FUNC_TYPE, SDL_FUNC, SDL_BINARY
from .SDL_filesystem import SDL_GlobFlags, SDL_EnumerateDirectoryCallback, SDL_PathInfo
from .SDL_properties import SDL_PropertiesID

class SDL_StorageInterface(ctypes.Structure):
    _fields_ = [
        ("version", ctypes.c_uint32),
        ("close", SDL_FUNC_TYPE["SDL_StorageInterface.close", ctypes.c_bool, [ctypes.c_void_p]]),
        ("ready", SDL_FUNC_TYPE["SDL_StorageInterface.ready", ctypes.c_bool, [ctypes.c_void_p]]),
        ("enumerate", SDL_FUNC_TYPE["SDL_StorageInterface.enumerate", ctypes.c_bool, [ctypes.c_void_p, ctypes.c_char_p, SDL_EnumerateDirectoryCallback, ctypes.c_void_p]]),
        ("info", SDL_FUNC_TYPE["SDL_StorageInterface.info", ctypes.c_bool, [ctypes.c_void_p, ctypes.c_char_p, SDL_POINTER[SDL_PathInfo]]]),
        ("read_file", SDL_FUNC_TYPE["SDL_StorageInterface.read_file", ctypes.c_bool, [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_void_p, ctypes.c_uint64]]),
        ("write_file", SDL_FUNC_TYPE["SDL_StorageInterface.write_file", ctypes.c_bool, [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_void_p, ctypes.c_uint64]]),
        ("mkdir", SDL_FUNC_TYPE["SDL_StorageInterface.mkdir", ctypes.c_bool, [ctypes.c_void_p, ctypes.c_char_p]]),
        ("remove", SDL_FUNC_TYPE["SDL_StorageInterface.remove", ctypes.c_bool, [ctypes.c_void_p, ctypes.c_char_p]]),
        ("rename", SDL_FUNC_TYPE["SDL_StorageInterface.rename", ctypes.c_bool, [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_char_p]]),
        ("copy", SDL_FUNC_TYPE["SDL_StorageInterface.copy", ctypes.c_bool, [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_char_p]]),
        ("space_remaining", SDL_FUNC_TYPE["SDL_StorageInterface.space_remaining", ctypes.c_uint64, [ctypes.c_void_p]])
    ]

class SDL_Storage(ctypes.c_void_p):
    ...

SDL_OpenTitleStorage: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OpenTitleStorage", SDL_POINTER[SDL_Storage], [ctypes.c_char_p, SDL_PropertiesID], SDL_BINARY]
SDL_OpenUserStorage: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OpenUserStorage", SDL_POINTER[SDL_Storage], [ctypes.c_char_p, ctypes.c_char_p, SDL_PropertiesID], SDL_BINARY]
SDL_OpenFileStorage: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OpenFileStorage", SDL_POINTER[SDL_Storage], [ctypes.c_char_p], SDL_BINARY]
SDL_OpenStorage: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OpenStorage", SDL_POINTER[SDL_Storage], [SDL_POINTER[SDL_StorageInterface], ctypes.c_void_p], SDL_BINARY]
SDL_CloseStorage: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CloseStorage", ctypes.c_bool, [SDL_POINTER[SDL_Storage]], SDL_BINARY]
SDL_StorageReady: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_StorageReady", ctypes.c_bool, [SDL_POINTER[SDL_Storage]], SDL_BINARY]
SDL_GetStorageFileSize: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetStorageFileSize", ctypes.c_bool, [SDL_POINTER[SDL_Storage], ctypes.c_char_p, SDL_POINTER[ctypes.c_uint64]], SDL_BINARY]
SDL_ReadStorageFile: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReadStorageFile", ctypes.c_bool, [SDL_POINTER[SDL_Storage], ctypes.c_char_p, ctypes.c_void_p, ctypes.c_uint64], SDL_BINARY]
SDL_WriteStorageFile: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WriteStorageFile", ctypes.c_bool, [SDL_POINTER[SDL_Storage], ctypes.c_char_p, ctypes.c_void_p, ctypes.c_uint64], SDL_BINARY]
SDL_CreateStorageDirectory: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateStorageDirectory", ctypes.c_bool, [SDL_POINTER[SDL_Storage], ctypes.c_char_p], SDL_BINARY]
SDL_EnumerateStorageDirectory: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_EnumerateStorageDirectory", ctypes.c_bool, [SDL_POINTER[SDL_Storage], ctypes.c_char_p, SDL_EnumerateDirectoryCallback, ctypes.c_void_p], SDL_BINARY]
SDL_RemoveStoragePath: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RemoveStoragePath", ctypes.c_bool, [SDL_POINTER[SDL_Storage], ctypes.c_char_p], SDL_BINARY]
SDL_RenameStoragePath: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenameStoragePath", ctypes.c_bool, [SDL_POINTER[SDL_Storage], ctypes.c_char_p, ctypes.c_char_p], SDL_BINARY]
SDL_CopyStorageFile: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CopyStorageFile", ctypes.c_bool, [SDL_POINTER[SDL_Storage], ctypes.c_char_p, ctypes.c_char_p], SDL_BINARY]
SDL_GetStoragePathInfo: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetStoragePathInfo", ctypes.c_bool, [SDL_POINTER[SDL_Storage], ctypes.c_char_p, SDL_POINTER[SDL_PathInfo]], SDL_BINARY]
SDL_GetStorageSpaceRemaining: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetStorageSpaceRemaining", ctypes.c_uint64, [SDL_POINTER[SDL_Storage]], SDL_BINARY]
SDL_GlobStorageDirectory: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GlobStorageDirectory", SDL_POINTER[ctypes.c_char_p], [SDL_POINTER[SDL_Storage], ctypes.c_char_p, ctypes.c_char_p, SDL_GlobFlags, SDL_POINTER[ctypes.c_int]], SDL_BINARY]