import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_ENUM, \
    SDL_FUNC, SDL_TYPE, SDL_BINARY
from.SDL_atomic import SDL_AtomicInt
from .SDL_thread import SDL_ThreadID

SDL_MUTEX_TIMEDOUT: int = 1

class SDL_Mutex(ctypes.c_void_p):
    ...

SDL_CreateMutex: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateMutex", SDL_POINTER[SDL_Mutex], [], SDL_BINARY]
SDL_LockMutex: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LockMutex", None, [SDL_POINTER[SDL_Mutex]], SDL_BINARY]
SDL_TryLockMutex: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_TryLockMutex", ctypes.c_bool, [SDL_POINTER[SDL_Mutex]], SDL_BINARY]
SDL_UnlockMutex: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UnlockMutex", None, [SDL_POINTER[SDL_Mutex]], SDL_BINARY]
SDL_DestroyMutex: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DestroyMutex", None, [SDL_POINTER[SDL_Mutex]], SDL_BINARY]

class SDL_RWLock(ctypes.c_void_p):
    ...

SDL_RWLOCK_TIMEDOUT: int = SDL_MUTEX_TIMEDOUT

SDL_CreateRWLock: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateRWLock", SDL_POINTER[SDL_RWLock], [], SDL_BINARY]
SDL_LockRWLockForReading: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LockRWLockForReading", None, [SDL_POINTER[SDL_RWLock]], SDL_BINARY]
SDL_LockRWLockForWriting: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LockRWLockForWriting", None, [SDL_POINTER[SDL_RWLock]], SDL_BINARY]
SDL_TryLockRWLockForReading: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_TryLockRWLockForReading", ctypes.c_bool, [SDL_POINTER[SDL_RWLock]], SDL_BINARY]
SDL_TryLockRWLockForWriting: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_TryLockRWLockForWriting", ctypes.c_bool, [SDL_POINTER[SDL_RWLock]], SDL_BINARY]
SDL_UnlockRWLock: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UnlockRWLock", None, [SDL_POINTER[SDL_RWLock]], SDL_BINARY]
SDL_DestroyRWLock: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DestroyRWLock", None, [SDL_POINTER[SDL_RWLock]], SDL_BINARY]

class SDL_Semaphore(ctypes.c_void_p):
    ...

SDL_CreateSemaphore: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateSemaphore", SDL_POINTER[SDL_Semaphore], [ctypes.c_uint32], SDL_BINARY]
SDL_DestroySemaphore: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DestroySemaphore", None, [SDL_POINTER[SDL_Semaphore]], SDL_BINARY]
SDL_WaitSemaphore: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WaitSemaphore", None, [SDL_POINTER[SDL_Semaphore]], SDL_BINARY]
SDL_TryWaitSemaphore: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_TryWaitSemaphore", ctypes.c_bool, [SDL_POINTER[SDL_Semaphore]], SDL_BINARY]
SDL_WaitSemaphoreTimeout: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WaitSemaphoreTimeout", ctypes.c_bool, [SDL_POINTER[SDL_Semaphore], ctypes.c_int32], SDL_BINARY]
SDL_SignalSemaphore: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SignalSemaphore", None, [SDL_POINTER[SDL_Semaphore]], SDL_BINARY]
SDL_GetSemaphoreValue: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSemaphoreValue", ctypes.c_uint32, [SDL_POINTER[SDL_Semaphore]], SDL_BINARY]

class SDL_Condition(ctypes.c_void_p):
    ...

SDL_CreateCondition: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateCondition", SDL_POINTER[SDL_Condition], [], SDL_BINARY]
SDL_DestroyCondition: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DestroyCondition", None, [SDL_POINTER[SDL_Condition]], SDL_BINARY]
SDL_SignalCondition: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SignalCondition", None, [SDL_POINTER[SDL_Condition]], SDL_BINARY]
SDL_BroadcastCondition: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BroadcastCondition", None, [SDL_POINTER[SDL_Condition]], SDL_BINARY]
SDL_WaitCondition: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WaitCondition", None, [SDL_POINTER[SDL_Condition], SDL_POINTER[SDL_Mutex]], SDL_BINARY]
SDL_WaitConditionTimeout: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WaitConditionTimeout", ctypes.c_bool, [SDL_POINTER[SDL_Condition], SDL_POINTER[SDL_Mutex], ctypes.c_int32], SDL_BINARY]

SDL_InitStatus: typing.TypeAlias = SDL_TYPE["SDL_InitStatus", SDL_ENUM]

SDL_INIT_STATUS_UNINITIALIZED, SDL_INIT_STATUS_INITIALIZING, SDL_INIT_STATUS_INITIALIZED, SDL_INIT_STATUS_UNINITIALIZING = range(4)

class SDL_InitState(ctypes.Structure):
    _fields_ = [
        ("status", SDL_AtomicInt),
        ("thread", SDL_ThreadID),
        ("reserved", ctypes.c_void_p)
    ]

SDL_ShouldInit: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShouldInit", ctypes.c_bool, [SDL_POINTER[SDL_InitState]], SDL_BINARY]
SDL_ShouldQuit: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShouldQuit", ctypes.c_bool, [SDL_POINTER[SDL_InitState]], SDL_BINARY]

SDL_SetInitialized: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetInitialized", None, [SDL_POINTER[SDL_InitState], ctypes.c_bool], SDL_BINARY]