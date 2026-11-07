import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_VA_LIST, SDL_TYPE, \
    SDL_FUNC, SDL_FUNC_TYPE, SDL_BINARY, SDL_ENUM
from .SDL_properties import SDL_PropertiesID

SDL_IOStatus: typing.TypeAlias = SDL_TYPE["SDL_IOStatus", SDL_ENUM]

SDL_IO_STATUS_READY, SDL_IO_STATUS_ERROR, SDL_IO_STATUS_EOF, SDL_IO_STATUS_NOT_READY, \
    SDL_IO_STATUS_READONLY, SDL_IO_STATUS_WRITEONLY = range(6)

SDL_IOWhence: typing.TypeAlias = SDL_TYPE["SDL_IOWhence", SDL_ENUM]

SDL_IO_SEEK_SET, SDL_IO_SEEK_CUR, SDL_IO_SEEK_END = range(3)

class SDL_IOStreamInterface(ctypes.Structure):
    _fields_ = [
        ("version", ctypes.c_uint32),
        ("size", SDL_FUNC_TYPE["SDL_IOStreamInterface.size", ctypes.c_int64, [ctypes.c_void_p]]),
        ("seek", SDL_FUNC_TYPE["SDL_IOStreamInterface.seek", ctypes.c_int64, [ctypes.c_void_p, ctypes.c_int64, SDL_IOWhence]]),
        ("read", SDL_FUNC_TYPE["SDL_IOStreamInterface.read", ctypes.c_size_t, [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_size_t, SDL_POINTER[SDL_IOStatus]]]),
        ("write", SDL_FUNC_TYPE["SDL_IOStreamInterface.write", ctypes.c_size_t, [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_size_t, SDL_POINTER[SDL_IOStatus]]]),
        ("flush", SDL_FUNC_TYPE["SDL_IOStreamInterface.flush", ctypes.c_bool, [ctypes.c_void_p, SDL_POINTER[SDL_IOStatus]]]),
        ("close", SDL_FUNC_TYPE["SDL_IOStreamInterface.close", ctypes.c_bool, [ctypes.c_void_p]])
    ]

class SDL_IOStream(ctypes.c_void_p):
    ...

SDL_IOFromFile: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_IOFromFile", SDL_POINTER[SDL_IOStream], [ctypes.c_char_p, ctypes.c_char_p], SDL_BINARY]

SDL_PROP_IOSTREAM_WINDOWS_HANDLE_POINTER: bytes = "SDL.iostream.windows.handle".encode()
SDL_PROP_IOSTREAM_STDIO_FILE_POINTER: bytes = "SDL.iostream.stdio.file".encode()
SDL_PROP_IOSTREAM_FILE_DESCRIPTOR_NUMBER: bytes = "SDL.iostream.file_descriptor".encode()
SDL_PROP_IOSTREAM_ANDROID_AASSET_POINTER: bytes = "SDL.iostream.android.aasset".encode()

SDL_IOFromMem: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_IOFromMem", SDL_POINTER[SDL_IOStream], [ctypes.c_void_p, ctypes.c_size_t], SDL_BINARY]

SDL_PROP_IOSTREAM_MEMORY_POINTER: bytes = "SDL.iostream.memory.base".encode()
SDL_PROP_IOSTREAM_MEMORY_SIZE_NUMBER: bytes = "SDL.iostream.memory.size".encode()

SDL_IOFromConstMem: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_IOFromConstMem", SDL_POINTER[SDL_IOStream], [ctypes.c_void_p, ctypes.c_size_t], SDL_BINARY]
SDL_IOFromDynamicMem: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_IOFromDynamicMem", SDL_POINTER[SDL_IOStream], [], SDL_BINARY]

SDL_PROP_IOSTREAM_DYNAMIC_MEMORY_POINTER: bytes = "SDL.iostream.dynamic.memory".encode()
SDL_PROP_IOSTREAM_DYNAMIC_CHUNKSIZE_NUMBER: bytes = "SDL.iostream.dynamic.chunksize".encode()

SDL_OpenIO: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OpenIO", SDL_POINTER[SDL_IOStream], [SDL_POINTER[SDL_IOStreamInterface], ctypes.c_void_p], SDL_BINARY]
SDL_CloseIO: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CloseIO", ctypes.c_bool, [SDL_POINTER[SDL_IOStream]], SDL_BINARY]

SDL_GetIOProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetIOProperties", SDL_PropertiesID, [SDL_POINTER[SDL_IOStream]], SDL_BINARY]
SDL_GetIOStatus: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetIOStatus", SDL_IOStatus, [SDL_POINTER[SDL_IOStream]], SDL_BINARY]
SDL_GetIOSize: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetIOSize", ctypes.c_int64, [SDL_POINTER[SDL_IOStream]], SDL_BINARY]

SDL_SeekIO: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SeekIO", ctypes.c_int64, [SDL_POINTER[SDL_IOStream], ctypes.c_int64, SDL_IOWhence], SDL_BINARY]
SDL_TellIO: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_TellIO", ctypes.c_int64, [SDL_POINTER[SDL_IOStream]], SDL_BINARY]
SDL_ReadIO: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReadIO", ctypes.c_size_t, [SDL_POINTER[SDL_IOStream], ctypes.c_void_p, ctypes.c_size_t], SDL_BINARY]
SDL_WriteIO: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WriteIO", ctypes.c_size_t, [SDL_POINTER[SDL_IOStream], ctypes.c_void_p, ctypes.c_size_t], SDL_BINARY]

SDL_IOprintf: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_IOprintf", ctypes.c_size_t, [SDL_POINTER[SDL_IOStream], ctypes.c_char_p, ...], SDL_BINARY]
SDL_IOvprintf: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_IOvprintf", ctypes.c_size_t, [SDL_POINTER[SDL_IOStream], ctypes.c_char_p, SDL_VA_LIST], SDL_BINARY]

SDL_FlushIO: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_FlushIO", ctypes.c_bool, [SDL_POINTER[SDL_IOStream]], SDL_BINARY]

SDL_LoadFile_IO: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LoadFile_IO", ctypes.c_void_p, [SDL_POINTER[SDL_IOStream], SDL_POINTER[ctypes.c_size_t], ctypes.c_bool], SDL_BINARY]
SDL_LoadFile: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LoadFile", ctypes.c_void_p, [ctypes.c_char_p, SDL_POINTER[ctypes.c_size_t]], SDL_BINARY]

SDL_SaveFile_IO: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SaveFile_IO", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], ctypes.c_void_p, ctypes.c_size_t, ctypes.c_bool], SDL_BINARY]
SDL_SaveFile: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SaveFile", ctypes.c_bool, [ctypes.c_char_p, ctypes.c_void_p, ctypes.c_size_t], SDL_BINARY]

SDL_ReadU8: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReadU8", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], SDL_POINTER[ctypes.c_uint8]], SDL_BINARY]
SDL_ReadS8: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReadS8", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], SDL_POINTER[ctypes.c_int8]], SDL_BINARY]
SDL_ReadU16LE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReadU16LE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], SDL_POINTER[ctypes.c_uint16]], SDL_BINARY]
SDL_ReadS16LE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReadS16LE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], SDL_POINTER[ctypes.c_int16]], SDL_BINARY]
SDL_ReadU16BE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReadU16BE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], SDL_POINTER[ctypes.c_uint16]], SDL_BINARY]
SDL_ReadS16BE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReadS16BE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], SDL_POINTER[ctypes.c_int16]], SDL_BINARY]
SDL_ReadU32LE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReadU32LE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], SDL_POINTER[ctypes.c_uint32]], SDL_BINARY]
SDL_ReadS32LE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReadS32LE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], SDL_POINTER[ctypes.c_int32]], SDL_BINARY]
SDL_ReadU32BE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReadU32BE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], SDL_POINTER[ctypes.c_uint32]], SDL_BINARY]
SDL_ReadS32BE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReadS32BE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], SDL_POINTER[ctypes.c_int32]], SDL_BINARY]
SDL_ReadU64LE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReadU64LE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], SDL_POINTER[ctypes.c_uint64]], SDL_BINARY]
SDL_ReadS64LE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReadS64LE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], SDL_POINTER[ctypes.c_int64]], SDL_BINARY]
SDL_ReadU64BE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReadU64BE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], SDL_POINTER[ctypes.c_uint64]], SDL_BINARY]
SDL_ReadS64BE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReadS64BE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], SDL_POINTER[ctypes.c_int64]], SDL_BINARY]

SDL_WriteU8: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WriteU8", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], ctypes.c_uint8], SDL_BINARY]
SDL_WriteS8: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WriteS8", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], ctypes.c_int8], SDL_BINARY]
SDL_WriteU16LE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WriteU16LE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], ctypes.c_uint16], SDL_BINARY]
SDL_WriteS16LE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WriteS16LE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], ctypes.c_int16], SDL_BINARY]
SDL_WriteU16BE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WriteU16BE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], ctypes.c_uint16], SDL_BINARY]
SDL_WriteS16BE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WriteS16BE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], ctypes.c_int16], SDL_BINARY]
SDL_WriteU32LE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WriteU32LE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], ctypes.c_uint32], SDL_BINARY]
SDL_WriteS32LE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WriteS32LE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], ctypes.c_int32], SDL_BINARY]
SDL_WriteU32BE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WriteU32BE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], ctypes.c_uint32], SDL_BINARY]
SDL_WriteS32BE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WriteS32BE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], ctypes.c_int32], SDL_BINARY]
SDL_WriteU64LE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WriteU64LE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], ctypes.c_uint64], SDL_BINARY]
SDL_WriteS64LE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WriteS64LE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], ctypes.c_int64], SDL_BINARY]
SDL_WriteU64BE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WriteU64BE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], ctypes.c_uint64], SDL_BINARY]
SDL_WriteS64BE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WriteS64BE", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], ctypes.c_int64], SDL_BINARY]