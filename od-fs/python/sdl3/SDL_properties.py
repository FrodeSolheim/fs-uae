import ctypes, typing, collections.abc as abc

from . import SDL_FUNC_TYPE, SDL_ENUM, SDL_FUNC, SDL_TYPE, SDL_BINARY

SDL_PropertiesID: typing.TypeAlias = SDL_TYPE["SDL_PropertiesID", ctypes.c_uint32]
SDL_PropertyType: typing.TypeAlias = SDL_TYPE["SDL_PropertyType", SDL_ENUM]

SDL_PROPERTY_TYPE_INVALID, SDL_PROPERTY_TYPE_POINTER, SDL_PROPERTY_TYPE_STRING, \
    SDL_PROPERTY_TYPE_NUMBER, SDL_PROPERTY_TYPE_FLOAT, SDL_PROPERTY_TYPE_BOOLEAN = range(6)

SDL_GetGlobalProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGlobalProperties", SDL_PropertiesID, [], SDL_BINARY]
SDL_CreateProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateProperties", SDL_PropertiesID, [], SDL_BINARY]
SDL_CopyProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CopyProperties", ctypes.c_bool, [SDL_PropertiesID, SDL_PropertiesID], SDL_BINARY]
SDL_LockProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LockProperties", ctypes.c_bool, [SDL_PropertiesID], SDL_BINARY]
SDL_UnlockProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UnlockProperties", None, [SDL_PropertiesID], SDL_BINARY]

SDL_CleanupPropertyCallback: typing.TypeAlias = SDL_FUNC_TYPE["SDL_CleanupPropertyCallback", None, [ctypes.c_void_p, ctypes.c_void_p]]

SDL_SetPointerPropertyWithCleanup: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetPointerPropertyWithCleanup", ctypes.c_bool, [SDL_PropertiesID, ctypes.c_char_p, ctypes.c_void_p, SDL_CleanupPropertyCallback, ctypes.c_void_p], SDL_BINARY]
SDL_SetPointerProperty: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetPointerProperty", ctypes.c_bool, [SDL_PropertiesID, ctypes.c_char_p, ctypes.c_void_p], SDL_BINARY]
SDL_SetStringProperty: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetStringProperty", ctypes.c_bool, [SDL_PropertiesID, ctypes.c_char_p, ctypes.c_char_p], SDL_BINARY]
SDL_SetNumberProperty: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetNumberProperty", ctypes.c_bool, [SDL_PropertiesID, ctypes.c_char_p, ctypes.c_int64], SDL_BINARY]
SDL_SetFloatProperty: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetFloatProperty", ctypes.c_bool, [SDL_PropertiesID, ctypes.c_char_p, ctypes.c_float], SDL_BINARY]
SDL_SetBooleanProperty: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetBooleanProperty", ctypes.c_bool, [SDL_PropertiesID, ctypes.c_char_p, ctypes.c_bool], SDL_BINARY]
SDL_HasProperty: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasProperty", ctypes.c_bool, [SDL_PropertiesID, ctypes.c_char_p], SDL_BINARY]
SDL_GetPropertyType: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetPropertyType", SDL_PropertyType, [SDL_PropertiesID, ctypes.c_char_p], SDL_BINARY]
SDL_GetPointerProperty: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetPointerProperty", ctypes.c_void_p, [SDL_PropertiesID, ctypes.c_char_p, ctypes.c_void_p], SDL_BINARY]
SDL_GetStringProperty: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetStringProperty", ctypes.c_char_p, [SDL_PropertiesID, ctypes.c_char_p, ctypes.c_char_p], SDL_BINARY]
SDL_GetNumberProperty: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetNumberProperty", ctypes.c_int64, [SDL_PropertiesID, ctypes.c_char_p, ctypes.c_int64], SDL_BINARY]
SDL_GetFloatProperty: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetFloatProperty", ctypes.c_float, [SDL_PropertiesID, ctypes.c_char_p, ctypes.c_float], SDL_BINARY]
SDL_GetBooleanProperty: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetBooleanProperty", ctypes.c_bool, [SDL_PropertiesID, ctypes.c_char_p, ctypes.c_bool], SDL_BINARY]
SDL_ClearProperty: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ClearProperty", ctypes.c_bool, [SDL_PropertiesID, ctypes.c_char_p], SDL_BINARY]

SDL_EnumeratePropertiesCallback: typing.TypeAlias = SDL_FUNC_TYPE["SDL_EnumeratePropertiesCallback", None, [ctypes.c_void_p, SDL_PropertiesID, ctypes.c_char_p]]

SDL_EnumerateProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_EnumerateProperties", ctypes.c_bool, [SDL_PropertiesID, SDL_EnumeratePropertiesCallback, ctypes.c_void_p], SDL_BINARY]
SDL_DestroyProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DestroyProperties", None, [SDL_PropertiesID], SDL_BINARY]