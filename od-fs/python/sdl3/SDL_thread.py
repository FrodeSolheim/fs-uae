import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_FUNC_TYPE, \
    SDL_ENUM, SDL_FUNC, SDL_TYPE, SDL_BINARY
from .SDL_atomic import SDL_AtomicInt
from .SDL_stdinc import SDL_FunctionPointer
from .SDL_properties import SDL_PropertiesID

class SDL_Thread(ctypes.c_void_p):
    ...

SDL_ThreadID: typing.TypeAlias = SDL_TYPE["SDL_ThreadID", ctypes.c_uint64]
SDL_TLSID: typing.TypeAlias = SDL_TYPE["SDL_TLSID", SDL_AtomicInt]

SDL_ThreadPriority: typing.TypeAlias = SDL_TYPE["SDL_ThreadPriority", SDL_ENUM]

SDL_THREAD_PRIORITY_LOW, SDL_THREAD_PRIORITY_NORMAL, SDL_THREAD_PRIORITY_HIGH, SDL_THREAD_PRIORITY_TIME_CRITICAL = range(4)

SDL_ThreadState: typing.TypeAlias = SDL_TYPE["SDL_ThreadState", SDL_ENUM]

SDL_THREAD_UNKNOWN, SDL_THREAD_ALIVE, SDL_THREAD_DETACHED, SDL_THREAD_COMPLETE = range(4)

SDL_ThreadFunction: typing.TypeAlias = SDL_FUNC_TYPE["SDL_ThreadFunction", ctypes.c_int, [ctypes.c_void_p]]

SDL_BeginThreadFunction, SDL_EndThreadFunction = SDL_FunctionPointer(0), SDL_FunctionPointer(0)

SDL_CreateThreadRuntime: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateThreadRuntime", SDL_POINTER[SDL_Thread], [SDL_ThreadFunction, ctypes.c_char_p, ctypes.c_void_p, SDL_FunctionPointer, SDL_FunctionPointer], SDL_BINARY]
SDL_CreateThreadWithPropertiesRuntime: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateThreadWithPropertiesRuntime", SDL_POINTER[SDL_Thread], [SDL_PropertiesID, SDL_FunctionPointer, SDL_FunctionPointer], SDL_BINARY]

SDL_CreateThread: abc.Callable[[SDL_ThreadFunction, ctypes.c_char_p, ctypes.c_void_p], SDL_POINTER[SDL_Thread]] = \
    lambda fn, name, data: SDL_CreateThreadRuntime(fn, name, data, SDL_BeginThreadFunction, SDL_EndThreadFunction)

SDL_CreateThreadWithProperties: abc.Callable[[SDL_PropertiesID], SDL_POINTER[SDL_Thread]] = \
    lambda props: SDL_CreateThreadWithPropertiesRuntime(props, SDL_BeginThreadFunction, SDL_EndThreadFunction)

SDL_PROP_THREAD_CREATE_ENTRY_FUNCTION_POINTER: bytes = "SDL.thread.create.entry_function".encode()
SDL_PROP_THREAD_CREATE_NAME_STRING: bytes = "SDL.thread.create.name".encode()
SDL_PROP_THREAD_CREATE_USERDATA_POINTER: bytes = "SDL.thread.create.userdata".encode()
SDL_PROP_THREAD_CREATE_STACKSIZE_NUMBER: bytes = "SDL.thread.create.stacksize".encode()

SDL_GetThreadName: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetThreadName", ctypes.c_char_p, [SDL_POINTER[SDL_Thread]], SDL_BINARY]
SDL_GetCurrentThreadID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetCurrentThreadID", SDL_ThreadID, [], SDL_BINARY]
SDL_GetThreadID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetThreadID", SDL_ThreadID, [SDL_POINTER[SDL_Thread]], SDL_BINARY]
SDL_SetCurrentThreadPriority: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetCurrentThreadPriority", ctypes.c_bool, [SDL_ThreadPriority], SDL_BINARY]
SDL_WaitThread: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WaitThread", None, [SDL_POINTER[SDL_Thread], SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_GetThreadState: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetThreadState", SDL_ThreadState, [SDL_POINTER[SDL_Thread]], SDL_BINARY]
SDL_DetachThread: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DetachThread", None, [SDL_POINTER[SDL_Thread]], SDL_BINARY]
SDL_GetTLS: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTLS", ctypes.c_void_p, [SDL_POINTER[SDL_TLSID]], SDL_BINARY]

SDL_TLSDestructorCallback: typing.TypeAlias = SDL_FUNC_TYPE["SDL_TLSDestructorCallback", None, [ctypes.c_void_p]]

SDL_SetTLS: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetTLS", ctypes.c_bool, [SDL_POINTER[SDL_TLSID], ctypes.c_void_p, SDL_TLSDestructorCallback], SDL_BINARY]
SDL_CleanupTLS: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CleanupTLS", None, [], SDL_BINARY]