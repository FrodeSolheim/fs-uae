import ctypes, typing, collections.abc as abc

from . import SDL_PLATFORM_SPECIFIC, SDL_POINTER, \
    SDL_FUNC, SDL_FUNC_TYPE, SDL_TYPE, SDL_BINARY, SDL_ENUM
from .SDL_video import SDL_DisplayID

if SDL_PLATFORM_SPECIFIC(system = ["Windows"]):
    import ctypes.wintypes as wintypes

    MSG: typing.TypeAlias = wintypes.tagMSG
    SDL_WindowsMessageHook: typing.TypeAlias = SDL_FUNC_TYPE["SDL_WindowsMessageHook", ctypes.c_bool, [ctypes.c_void_p, SDL_POINTER[MSG]]]
    SDL_SetWindowsMessageHook: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowsMessageHook", None, [SDL_WindowsMessageHook, ctypes.c_void_p], SDL_BINARY]

    SDL_GetDirect3D9AdapterIndex: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetDirect3D9AdapterIndex", ctypes.c_int, [SDL_DisplayID], SDL_BINARY]
    SDL_GetDXGIOutputInfo: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetDXGIOutputInfo", ctypes.c_bool, [SDL_DisplayID, SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int]], SDL_BINARY]

    class XEvent(ctypes.Union):
        ...

    SDL_X11EventHook: typing.TypeAlias = SDL_FUNC_TYPE["SDL_X11EventHook", ctypes.c_bool, [ctypes.c_void_p, SDL_POINTER[XEvent]]]
    SDL_SetX11EventHook: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetX11EventHook", None, [SDL_X11EventHook, ctypes.c_void_p], SDL_BINARY] 

if SDL_PLATFORM_SPECIFIC(system = ["Linux"]):
    SDL_SetLinuxThreadPriority: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetLinuxThreadPriority", ctypes.c_bool, [ctypes.c_int64, ctypes.c_int], SDL_BINARY]
    SDL_SetLinuxThreadPriorityAndPolicy: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetLinuxThreadPriorityAndPolicy", ctypes.c_bool, [ctypes.c_int64, ctypes.c_int, ctypes.c_int], SDL_BINARY]

SDL_IsTablet: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_IsTablet", ctypes.c_bool, [], SDL_BINARY]
SDL_IsTV: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_IsTV", ctypes.c_bool, [], SDL_BINARY]

SDL_Sandbox: typing.TypeAlias = SDL_TYPE["SDL_Sandbox", SDL_ENUM]

SDL_SANDBOX_NONE, SDL_SANDBOX_UNKNOWN_CONTAINER, SDL_SANDBOX_FLATPAK, \
    SDL_SANDBOX_SNAP, SDL_SANDBOX_MACOS = range(5)

SDL_GetSandbox: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSandbox", SDL_Sandbox, [], SDL_BINARY]

SDL_OnApplicationWillTerminate: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OnApplicationWillTerminate", None, [], SDL_BINARY]
SDL_OnApplicationDidReceiveMemoryWarning: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OnApplicationDidReceiveMemoryWarning", None, [], SDL_BINARY]
SDL_OnApplicationWillEnterBackground: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OnApplicationWillEnterBackground", None, [], SDL_BINARY]
SDL_OnApplicationDidEnterBackground: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OnApplicationDidEnterBackground", None, [], SDL_BINARY]
SDL_OnApplicationWillEnterForeground: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OnApplicationWillEnterForeground", None, [], SDL_BINARY]
SDL_OnApplicationDidEnterForeground: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OnApplicationDidEnterForeground", None, [], SDL_BINARY]