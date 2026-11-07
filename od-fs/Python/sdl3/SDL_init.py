import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_FUNC_TYPE, \
    SDL_FUNC, SDL_TYPE, SDL_BINARY, SDL_ENUM
from .SDL_events import SDL_Event

SDL_InitFlags: typing.TypeAlias = SDL_TYPE["SDL_InitFlags", ctypes.c_uint32]

SDL_INIT_AUDIO: int = 0x00000010
SDL_INIT_VIDEO: int = 0x00000020
SDL_INIT_JOYSTICK: int = 0x00000200
SDL_INIT_HAPTIC: int = 0x00001000
SDL_INIT_GAMEPAD: int= 0x00002000
SDL_INIT_EVENTS: int = 0x00004000
SDL_INIT_SENSOR: int = 0x00008000
SDL_INIT_CAMERA: int = 0x00010000

SDL_AppResult: typing.TypeAlias = SDL_TYPE["SDL_AppResult", SDL_ENUM]

SDL_APP_CONTINUE, SDL_APP_SUCCESS, SDL_APP_FAILURE = range(3)

SDL_AppInit_func: typing.TypeAlias = SDL_FUNC_TYPE["SDL_AppInit_func", SDL_AppResult, [SDL_POINTER[ctypes.c_void_p], ctypes.c_int, SDL_POINTER[ctypes.c_char_p]]]
SDL_AppIterate_func: typing.TypeAlias = SDL_FUNC_TYPE["SDL_AppIterate_func", SDL_AppResult, [ctypes.c_void_p]]
SDL_AppEvent_func: typing.TypeAlias = SDL_FUNC_TYPE["SDL_AppEvent_func", SDL_AppResult, [ctypes.c_void_p, SDL_POINTER[SDL_Event]]]
SDL_AppQuit_func: typing.TypeAlias = SDL_FUNC_TYPE["SDL_AppQuit_func", None, [ctypes.c_void_p, SDL_AppResult]]

SDL_Init: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_Init", ctypes.c_bool, [SDL_InitFlags], SDL_BINARY]
SDL_InitSubSystem: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_InitSubSystem", ctypes.c_bool, [SDL_InitFlags], SDL_BINARY]
SDL_QuitSubSystem: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_QuitSubSystem", None, [SDL_InitFlags], SDL_BINARY]
SDL_WasInit: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WasInit", SDL_InitFlags, [SDL_InitFlags], SDL_BINARY]
SDL_Quit: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_Quit", None, [], SDL_BINARY]

SDL_IsMainThread: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_IsMainThread", ctypes.c_bool, [], SDL_BINARY]
SDL_MainThreadCallback: typing.TypeAlias = SDL_FUNC_TYPE["SDL_MainThreadCallback", None, [ctypes.c_void_p]]
SDL_RunOnMainThread: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RunOnMainThread", ctypes.c_bool, [SDL_MainThreadCallback, ctypes.c_void_p, ctypes.c_bool], SDL_BINARY]

SDL_SetAppMetadata: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetAppMetadata", ctypes.c_bool, [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p], SDL_BINARY]
SDL_SetAppMetadataProperty: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetAppMetadataProperty", ctypes.c_bool, [ctypes.c_char_p, ctypes.c_char_p], SDL_BINARY]

SDL_PROP_APP_METADATA_NAME_STRING: bytes = "SDL.app.metadata.name".encode()
SDL_PROP_APP_METADATA_VERSION_STRING: bytes = "SDL.app.metadata.version".encode()
SDL_PROP_APP_METADATA_IDENTIFIER_STRING: bytes = "SDL.app.metadata.identifier".encode()
SDL_PROP_APP_METADATA_CREATOR_STRING: bytes = "SDL.app.metadata.creator".encode()
SDL_PROP_APP_METADATA_COPYRIGHT_STRING: bytes = "SDL.app.metadata.copyright".encode()
SDL_PROP_APP_METADATA_URL_STRING: bytes = "SDL.app.metadata.url".encode()
SDL_PROP_APP_METADATA_TYPE_STRING: bytes = "SDL.app.metadata.type".encode()

SDL_GetAppMetadataProperty: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAppMetadataProperty", ctypes.c_char_p, [ctypes.c_char_p], SDL_BINARY]