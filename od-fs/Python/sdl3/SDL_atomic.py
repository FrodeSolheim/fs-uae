import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_FUNC, SDL_TYPE, SDL_BINARY

SDL_SpinLock: typing.TypeAlias = SDL_TYPE["SDL_SpinLock", ctypes.c_int]

SDL_TryLockSpinlock: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_TryLockSpinlock", ctypes.c_bool, [SDL_POINTER[SDL_SpinLock]], SDL_BINARY]
SDL_LockSpinlock: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LockSpinlock", None, [SDL_POINTER[SDL_SpinLock]], SDL_BINARY]
SDL_UnlockSpinlock: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UnlockSpinlock", None, [SDL_POINTER[SDL_SpinLock]], SDL_BINARY]

SDL_MemoryBarrierReleaseFunction: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_MemoryBarrierReleaseFunction", None, [], SDL_BINARY]
SDL_MemoryBarrierAcquireFunction: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_MemoryBarrierAcquireFunction", None, [], SDL_BINARY]

class SDL_AtomicInt(ctypes.Structure):
    _fields_ = [
        ("value", ctypes.c_int)
    ]

SDL_CompareAndSwapAtomicInt: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CompareAndSwapAtomicInt", ctypes.c_bool, [SDL_POINTER[SDL_AtomicInt], ctypes.c_int, ctypes.c_int], SDL_BINARY]
SDL_SetAtomicInt: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetAtomicInt", ctypes.c_int, [SDL_POINTER[SDL_AtomicInt], ctypes.c_int], SDL_BINARY]
SDL_GetAtomicInt: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAtomicInt", ctypes.c_int, [SDL_POINTER[SDL_AtomicInt]], SDL_BINARY]
SDL_AddAtomicInt: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_AddAtomicInt", ctypes.c_int, [SDL_POINTER[SDL_AtomicInt], ctypes.c_int], SDL_BINARY]

SDL_AtomicIncRef: abc.Callable[[SDL_POINTER[SDL_AtomicInt]], ctypes.c_int] = lambda a: SDL_AddAtomicInt(a, 1)
SDL_AtomicDecRef: abc.Callable[[SDL_POINTER[SDL_AtomicInt]], bool] = lambda a: SDL_AddAtomicInt(a, -1) == 1

class SDL_AtomicU32(ctypes.Structure):
    _fields_ = [
        ("value", ctypes.c_uint32)
    ]

SDL_CompareAndSwapAtomicU32: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CompareAndSwapAtomicU32", ctypes.c_bool, [SDL_POINTER[SDL_AtomicU32], ctypes.c_uint32, ctypes.c_uint32], SDL_BINARY]
SDL_SetAtomicU32: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetAtomicU32", ctypes.c_uint32, [SDL_POINTER[SDL_AtomicU32], ctypes.c_uint32], SDL_BINARY]
SDL_GetAtomicU32: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAtomicU32", ctypes.c_uint32, [SDL_POINTER[SDL_AtomicU32]], SDL_BINARY]

SDL_CompareAndSwapAtomicPointer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CompareAndSwapAtomicPointer", ctypes.c_bool, [SDL_POINTER[ctypes.c_void_p], ctypes.c_void_p, ctypes.c_void_p], SDL_BINARY]
SDL_SetAtomicPointer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetAtomicPointer", ctypes.c_void_p, [SDL_POINTER[ctypes.c_void_p], ctypes.c_void_p], SDL_BINARY]
SDL_GetAtomicPointer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAtomicPointer", ctypes.c_void_p, [SDL_POINTER[ctypes.c_void_p]], SDL_BINARY]