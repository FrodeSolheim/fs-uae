import ctypes, typing, collections.abc as abc

from . import SDL_BINARY, SDL_POINTER, SDL_ENUM, SDL_FUNC, SDL_TYPE

class SDL_AsyncIO(ctypes.c_void_p):
    ...

SDL_AsyncIOTaskType: typing.TypeAlias = SDL_TYPE["SDL_AsyncIOTaskType", SDL_ENUM]

SDL_ASYNCIO_TASK_READ, SDL_ASYNCIO_TASK_WRITE, SDL_ASYNCIO_TASK_CLOSE = range(3)

SDL_AsyncIOResult: typing.TypeAlias = SDL_TYPE["SDL_AsyncIOResult", SDL_ENUM]

SDL_ASYNCIO_COMPLETE, SDL_ASYNCIO_FAILURE, SDL_ASYNCIO_CANCELED = range(3)

class SDL_AsyncIOOutcome(ctypes.Structure):
    _fields_ = [
        ("asyncio", SDL_POINTER[SDL_AsyncIO]),
        ("type", SDL_AsyncIOTaskType),
        ("result", SDL_AsyncIOResult),
        ("buffer", ctypes.c_void_p),
        ("offset", ctypes.c_uint64),
        ("bytes_requested", ctypes.c_uint64),
        ("bytes_transferred", ctypes.c_uint64),
        ("userdata", ctypes.c_void_p)
    ]

class SDL_AsyncIOQueue(ctypes.c_void_p):
    ...

SDL_AsyncIOFromFile: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_AsyncIOFromFile", SDL_POINTER[SDL_AsyncIO], [ctypes.c_char_p, ctypes.c_char_p], SDL_BINARY]
SDL_GetAsyncIOSize: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAsyncIOSize", ctypes.c_int64, [SDL_POINTER[SDL_AsyncIO]], SDL_BINARY]

SDL_ReadAsyncIO: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReadAsyncIO", ctypes.c_bool, [SDL_POINTER[SDL_AsyncIO], ctypes.c_void_p, ctypes.c_uint64, ctypes.c_uint64, SDL_POINTER[SDL_AsyncIOQueue], ctypes.c_void_p], SDL_BINARY]
SDL_WriteAsyncIO: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WriteAsyncIO", ctypes.c_bool, [SDL_POINTER[SDL_AsyncIO], ctypes.c_void_p, ctypes.c_uint64, ctypes.c_uint64, SDL_POINTER[SDL_AsyncIOQueue], ctypes.c_void_p], SDL_BINARY]
SDL_CloseAsyncIO: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CloseAsyncIO", ctypes.c_bool, [SDL_POINTER[SDL_AsyncIO], ctypes.c_bool, SDL_POINTER[SDL_AsyncIOQueue], ctypes.c_void_p], SDL_BINARY]

SDL_CreateAsyncIOQueue: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateAsyncIOQueue", SDL_POINTER[SDL_AsyncIOQueue], [], SDL_BINARY]
SDL_DestroyAsyncIOQueue: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DestroyAsyncIOQueue", None, [SDL_POINTER[SDL_AsyncIOQueue]], SDL_BINARY]
SDL_GetAsyncIOResult: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAsyncIOResult", ctypes.c_bool, [SDL_POINTER[SDL_AsyncIOQueue], SDL_POINTER[SDL_AsyncIOOutcome]], SDL_BINARY]
SDL_WaitAsyncIOResult: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WaitAsyncIOResult", ctypes.c_bool, [SDL_POINTER[SDL_AsyncIOQueue], SDL_POINTER[SDL_AsyncIOOutcome], ctypes.c_int32], SDL_BINARY]
SDL_SignalAsyncIOQueue: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SignalAsyncIOQueue", None, [SDL_POINTER[SDL_AsyncIOQueue]], SDL_BINARY]

SDL_LoadFileAsync: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LoadFileAsync", ctypes.c_bool, [ctypes.c_char_p, SDL_POINTER[SDL_AsyncIOQueue], ctypes.c_void_p], SDL_BINARY]