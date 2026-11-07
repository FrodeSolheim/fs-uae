import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_ENUM, \
    SDL_FUNC, SDL_TYPE, SDL_BINARY
from .SDL_guid import SDL_GUID
from .SDL_power import SDL_PowerState
from .SDL_sensor import SDL_SensorType
from .SDL_iostream import SDL_IOStream
from .SDL_properties import SDL_PropertiesID
from .SDL_joystick import SDL_Joystick, SDL_JoystickID, SDL_JoystickConnectionState, \
    SDL_PROP_JOYSTICK_CAP_MONO_LED_BOOLEAN, SDL_PROP_JOYSTICK_CAP_RGB_LED_BOOLEAN, SDL_PROP_JOYSTICK_CAP_PLAYER_LED_BOOLEAN, \
        SDL_PROP_JOYSTICK_CAP_RUMBLE_BOOLEAN, SDL_PROP_JOYSTICK_CAP_TRIGGER_RUMBLE_BOOLEAN

class SDL_Gamepad(ctypes.c_void_p):
    ...

SDL_GamepadType: typing.TypeAlias = SDL_TYPE["SDL_GamepadType", SDL_ENUM]

SDL_GAMEPAD_TYPE_UNKNOWN, SDL_GAMEPAD_TYPE_STANDARD, SDL_GAMEPAD_TYPE_XBOX360, SDL_GAMEPAD_TYPE_XBOXONE, SDL_GAMEPAD_TYPE_PS3, SDL_GAMEPAD_TYPE_PS4, SDL_GAMEPAD_TYPE_PS5, \
    SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO, SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT, SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT, SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR, \
        SDL_GAMEPAD_TYPE_COUNT = range(12)

SDL_GamepadButton: typing.TypeAlias = SDL_TYPE["SDL_GamepadButton", SDL_ENUM]

SDL_GAMEPAD_BUTTON_INVALID, SDL_GAMEPAD_BUTTON_SOUTH, SDL_GAMEPAD_BUTTON_EAST, SDL_GAMEPAD_BUTTON_WEST, SDL_GAMEPAD_BUTTON_NORTH, SDL_GAMEPAD_BUTTON_BACK, SDL_GAMEPAD_BUTTON_GUIDE, \
    SDL_GAMEPAD_BUTTON_START, SDL_GAMEPAD_BUTTON_LEFT_STICK, SDL_GAMEPAD_BUTTON_RIGHT_STICK, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, SDL_GAMEPAD_BUTTON_DPAD_UP, \
        SDL_GAMEPAD_BUTTON_DPAD_DOWN, SDL_GAMEPAD_BUTTON_DPAD_LEFT, SDL_GAMEPAD_BUTTON_DPAD_RIGHT, SDL_GAMEPAD_BUTTON_MISC1, SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1, SDL_GAMEPAD_BUTTON_LEFT_PADDLE1, \
            SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2, SDL_GAMEPAD_BUTTON_LEFT_PADDLE2, SDL_GAMEPAD_BUTTON_TOUCHPAD, SDL_GAMEPAD_BUTTON_MISC2, SDL_GAMEPAD_BUTTON_MISC3, SDL_GAMEPAD_BUTTON_MISC4, \
                SDL_GAMEPAD_BUTTON_MISC5, SDL_GAMEPAD_BUTTON_MISC6, SDL_GAMEPAD_BUTTON_COUNT = range(-1, 27)

SDL_GamepadButtonLabel: typing.TypeAlias = SDL_TYPE["SDL_GamepadButtonLabel", SDL_ENUM]

SDL_GAMEPAD_BUTTON_LABEL_UNKNOWN, SDL_GAMEPAD_BUTTON_LABEL_A, SDL_GAMEPAD_BUTTON_LABEL_B, SDL_GAMEPAD_BUTTON_LABEL_X, SDL_GAMEPAD_BUTTON_LABEL_Y, \
    SDL_GAMEPAD_BUTTON_LABEL_CROSS, SDL_GAMEPAD_BUTTON_LABEL_CIRCLE, SDL_GAMEPAD_BUTTON_LABEL_SQUARE, SDL_GAMEPAD_BUTTON_LABEL_TRIANGLE = range(9)

SDL_GamepadAxis: typing.TypeAlias = SDL_TYPE["SDL_GamepadAxis", SDL_ENUM]

SDL_GAMEPAD_AXIS_INVALID, SDL_GAMEPAD_AXIS_LEFTX, SDL_GAMEPAD_AXIS_LEFTY, SDL_GAMEPAD_AXIS_RIGHTX, SDL_GAMEPAD_AXIS_RIGHTY, \
    SDL_GAMEPAD_AXIS_LEFT_TRIGGER, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER, SDL_GAMEPAD_AXIS_COUNT = range(-1, 7)

SDL_GamepadBindingType: typing.TypeAlias = SDL_TYPE["SDL_GamepadBindingType", SDL_ENUM]

SDL_GAMEPAD_BINDTYPE_NONE, SDL_GAMEPAD_BINDTYPE_BUTTON, \
    SDL_GAMEPAD_BINDTYPE_AXIS, SDL_GAMEPAD_BINDTYPE_HAT = range(4)

class _SDL_GamepadBinding_axis(ctypes.Structure):
    _fields_ = [
        ("axis", SDL_GamepadAxis),
        ("axis_min", ctypes.c_int),
        ("axis_max", ctypes.c_int)
    ]

class _SDL_GamepadBinding_hat(ctypes.Structure):
    _fields_ = [
        ("hat", ctypes.c_int),
        ("hat_mask", ctypes.c_int)
    ]

class _SDL_GamepadBinding_input(ctypes.Union):
    _fields_ = [
        ("button", SDL_GamepadButton),
        ("axis", _SDL_GamepadBinding_axis),
        ("hat", _SDL_GamepadBinding_hat)
    ]

class _SDL_GamepadBinding_output(ctypes.Union):
    _fields_ = [
        ("button", SDL_GamepadButton),
        ("axis", _SDL_GamepadBinding_axis)
    ]

class SDL_GamepadBinding(ctypes.Structure):
    _fields_ = [
        ("input_type", SDL_GamepadBindingType),
        ("input", _SDL_GamepadBinding_input),
        ("output_type", SDL_GamepadBindingType),
        ("output", _SDL_GamepadBinding_output)
    ]

SDL_AddGamepadMapping: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_AddGamepadMapping", ctypes.c_int, [ctypes.c_char_p], SDL_BINARY]
SDL_AddGamepadMappingsFromIO: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_AddGamepadMappingsFromIO", ctypes.c_int, [SDL_POINTER[SDL_IOStream], ctypes.c_bool], SDL_BINARY]
SDL_AddGamepadMappingsFromFile: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_AddGamepadMappingsFromFile", ctypes.c_int, [ctypes.c_char_p], SDL_BINARY]
SDL_ReloadGamepadMappings: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReloadGamepadMappings", ctypes.c_bool, [], SDL_BINARY]
SDL_GetGamepadMappings: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadMappings", SDL_POINTER[ctypes.c_char_p], [SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_GetGamepadMappingForGUID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadMappingForGUID", ctypes.c_char_p, [SDL_GUID], SDL_BINARY]
SDL_GetGamepadMapping: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadMapping", ctypes.c_char_p, [SDL_POINTER[SDL_Gamepad]], SDL_BINARY]
SDL_SetGamepadMapping: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetGamepadMapping", ctypes.c_bool, [SDL_JoystickID, ctypes.c_char_p], SDL_BINARY]
SDL_HasGamepad: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasGamepad", ctypes.c_bool, [], SDL_BINARY]
SDL_GetGamepads: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepads", SDL_POINTER[SDL_JoystickID], [SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_IsGamepad: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_IsGamepad", ctypes.c_bool, [SDL_JoystickID], SDL_BINARY]
SDL_GetGamepadNameForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadNameForID", ctypes.c_char_p, [SDL_JoystickID], SDL_BINARY]
SDL_GetGamepadPathForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadPathForID", ctypes.c_char_p, [SDL_JoystickID], SDL_BINARY]
SDL_GetGamepadPlayerIndexForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadPlayerIndexForID", ctypes.c_int, [SDL_JoystickID], SDL_BINARY]
SDL_GetGamepadGUIDForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadGUIDForID", SDL_GUID, [SDL_JoystickID], SDL_BINARY]
SDL_GetGamepadVendorForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadVendorForID", ctypes.c_uint16, [SDL_JoystickID], SDL_BINARY]
SDL_GetGamepadProductForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadProductForID", ctypes.c_uint16, [SDL_JoystickID], SDL_BINARY]
SDL_GetGamepadProductVersionForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadProductVersionForID", ctypes.c_uint16, [SDL_JoystickID], SDL_BINARY]
SDL_GetGamepadTypeForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadTypeForID", SDL_GamepadType, [SDL_JoystickID], SDL_BINARY]
SDL_GetRealGamepadTypeForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRealGamepadTypeForID", SDL_GamepadType, [SDL_JoystickID], SDL_BINARY]
SDL_GetGamepadMappingForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadMappingForID", ctypes.c_char_p, [SDL_JoystickID], SDL_BINARY]
SDL_OpenGamepad: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OpenGamepad", SDL_POINTER[SDL_Gamepad], [SDL_JoystickID], SDL_BINARY]
SDL_GetGamepadFromID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadFromID", SDL_POINTER[SDL_Gamepad], [SDL_JoystickID], SDL_BINARY]
SDL_GetGamepadFromPlayerIndex: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadFromPlayerIndex", SDL_POINTER[SDL_Gamepad], [ctypes.c_int], SDL_BINARY]
SDL_GetGamepadProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadProperties", SDL_PropertiesID, [SDL_POINTER[SDL_Gamepad]], SDL_BINARY]

SDL_PROP_GAMEPAD_CAP_MONO_LED_BOOLEAN: bytes = SDL_PROP_JOYSTICK_CAP_MONO_LED_BOOLEAN
SDL_PROP_GAMEPAD_CAP_RGB_LED_BOOLEAN: bytes = SDL_PROP_JOYSTICK_CAP_RGB_LED_BOOLEAN
SDL_PROP_GAMEPAD_CAP_PLAYER_LED_BOOLEAN: bytes = SDL_PROP_JOYSTICK_CAP_PLAYER_LED_BOOLEAN
SDL_PROP_GAMEPAD_CAP_RUMBLE_BOOLEAN: bytes = SDL_PROP_JOYSTICK_CAP_RUMBLE_BOOLEAN
SDL_PROP_GAMEPAD_CAP_TRIGGER_RUMBLE_BOOLEAN: bytes = SDL_PROP_JOYSTICK_CAP_TRIGGER_RUMBLE_BOOLEAN

SDL_GetGamepadID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadID", SDL_JoystickID, [SDL_POINTER[SDL_Gamepad]], SDL_BINARY]
SDL_GetGamepadName: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadName", ctypes.c_char_p, [SDL_POINTER[SDL_Gamepad]], SDL_BINARY]
SDL_GetGamepadPath: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadPath", ctypes.c_char_p, [SDL_POINTER[SDL_Gamepad]], SDL_BINARY]
SDL_GetGamepadType: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadType", SDL_GamepadType, [SDL_POINTER[SDL_Gamepad]], SDL_BINARY]
SDL_GetRealGamepadType: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRealGamepadType", SDL_GamepadType, [SDL_POINTER[SDL_Gamepad]], SDL_BINARY]
SDL_GetGamepadPlayerIndex: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadPlayerIndex", ctypes.c_int, [SDL_POINTER[SDL_Gamepad]], SDL_BINARY]
SDL_SetGamepadPlayerIndex: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetGamepadPlayerIndex", ctypes.c_bool, [SDL_POINTER[SDL_Gamepad], ctypes.c_int], SDL_BINARY]
SDL_GetGamepadVendor: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadVendor", ctypes.c_uint16, [SDL_POINTER[SDL_Gamepad]], SDL_BINARY]
SDL_GetGamepadProduct: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadProduct", ctypes.c_uint16, [SDL_POINTER[SDL_Gamepad]], SDL_BINARY]
SDL_GetGamepadProductVersion: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadProductVersion", ctypes.c_uint16, [SDL_POINTER[SDL_Gamepad]], SDL_BINARY]
SDL_GetGamepadFirmwareVersion: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadFirmwareVersion", ctypes.c_uint16, [SDL_POINTER[SDL_Gamepad]], SDL_BINARY]
SDL_GetGamepadSerial: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadSerial", ctypes.c_char_p, [SDL_POINTER[SDL_Gamepad]], SDL_BINARY]
SDL_GetGamepadSteamHandle: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadSteamHandle", ctypes.c_uint64, [SDL_POINTER[SDL_Gamepad]], SDL_BINARY]
SDL_GetGamepadConnectionState: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadConnectionState", SDL_JoystickConnectionState, [SDL_POINTER[SDL_Gamepad]], SDL_BINARY]
SDL_GetGamepadPowerInfo: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadPowerInfo", SDL_PowerState, [SDL_POINTER[SDL_Gamepad], SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_GamepadConnected: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GamepadConnected", ctypes.c_bool, [SDL_POINTER[SDL_Gamepad]], SDL_BINARY]
SDL_GetGamepadJoystick: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadJoystick", SDL_POINTER[SDL_Joystick], [SDL_POINTER[SDL_Gamepad]], SDL_BINARY]
SDL_SetGamepadEventsEnabled: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetGamepadEventsEnabled", None, [ctypes.c_bool], SDL_BINARY]
SDL_GamepadEventsEnabled: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GamepadEventsEnabled", ctypes.c_bool, [], SDL_BINARY]
SDL_GetGamepadBindings: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadBindings", SDL_POINTER[SDL_POINTER[SDL_GamepadBinding]], [SDL_POINTER[SDL_Gamepad], SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_UpdateGamepads: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UpdateGamepads", None, [], SDL_BINARY]
SDL_GetGamepadTypeFromString: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadTypeFromString", SDL_GamepadType, [ctypes.c_char_p], SDL_BINARY]
SDL_GetGamepadStringForType: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadStringForType", ctypes.c_char_p, [SDL_GamepadType], SDL_BINARY]
SDL_GetGamepadAxisFromString: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadAxisFromString", SDL_GamepadAxis, [ctypes.c_char_p], SDL_BINARY]
SDL_GetGamepadStringForAxis: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadStringForAxis", ctypes.c_char_p, [SDL_GamepadAxis], SDL_BINARY]
SDL_GamepadHasAxis: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GamepadHasAxis", ctypes.c_bool, [SDL_POINTER[SDL_Gamepad], SDL_GamepadAxis], SDL_BINARY]
SDL_GetGamepadAxis: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadAxis", ctypes.c_int16, [SDL_POINTER[SDL_Gamepad], SDL_GamepadAxis], SDL_BINARY]
SDL_GetGamepadButtonFromString: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadButtonFromString", SDL_GamepadButton, [ctypes.c_char_p], SDL_BINARY]
SDL_GetGamepadStringForButton: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadStringForButton", ctypes.c_char_p, [SDL_GamepadButton], SDL_BINARY]
SDL_GamepadHasButton: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GamepadHasButton", ctypes.c_bool, [SDL_POINTER[SDL_Gamepad], SDL_GamepadButton], SDL_BINARY]
SDL_GetGamepadButton: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadButton", ctypes.c_bool, [SDL_POINTER[SDL_Gamepad], SDL_GamepadButton], SDL_BINARY]
SDL_GetGamepadButtonLabelForType: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadButtonLabelForType", SDL_GamepadButtonLabel, [SDL_GamepadType, SDL_GamepadButton], SDL_BINARY]
SDL_GetGamepadButtonLabel: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadButtonLabel", SDL_GamepadButtonLabel, [SDL_POINTER[SDL_Gamepad], SDL_GamepadButton], SDL_BINARY]
SDL_GetNumGamepadTouchpads: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetNumGamepadTouchpads", ctypes.c_int, [SDL_POINTER[SDL_Gamepad]], SDL_BINARY]
SDL_GetNumGamepadTouchpadFingers: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetNumGamepadTouchpadFingers", ctypes.c_int, [SDL_POINTER[SDL_Gamepad], ctypes.c_int], SDL_BINARY]
SDL_GetGamepadTouchpadFinger: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadTouchpadFinger", ctypes.c_bool, [SDL_POINTER[SDL_Gamepad], ctypes.c_int, ctypes.c_int, SDL_POINTER[ctypes.c_bool], SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float]], SDL_BINARY]
SDL_GamepadHasSensor: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GamepadHasSensor", ctypes.c_bool, [SDL_POINTER[SDL_Gamepad], SDL_SensorType], SDL_BINARY]
SDL_SetGamepadSensorEnabled: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetGamepadSensorEnabled", ctypes.c_bool, [SDL_POINTER[SDL_Gamepad], SDL_SensorType, ctypes.c_bool], SDL_BINARY]
SDL_GamepadSensorEnabled: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GamepadSensorEnabled", ctypes.c_bool, [SDL_POINTER[SDL_Gamepad], SDL_SensorType], SDL_BINARY]
SDL_GetGamepadSensorDataRate: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadSensorDataRate", ctypes.c_float, [SDL_POINTER[SDL_Gamepad], SDL_SensorType], SDL_BINARY]
SDL_GetGamepadSensorData: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadSensorData", ctypes.c_bool, [SDL_POINTER[SDL_Gamepad], SDL_SensorType, SDL_POINTER[ctypes.c_float], ctypes.c_int], SDL_BINARY]
SDL_RumbleGamepad: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RumbleGamepad", ctypes.c_bool, [SDL_POINTER[SDL_Gamepad], ctypes.c_uint16, ctypes.c_uint16, ctypes.c_uint32], SDL_BINARY]
SDL_RumbleGamepadTriggers: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RumbleGamepadTriggers", ctypes.c_bool, [SDL_POINTER[SDL_Gamepad], ctypes.c_uint16, ctypes.c_uint16, ctypes.c_uint32], SDL_BINARY]
SDL_SetGamepadLED: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetGamepadLED", ctypes.c_bool, [SDL_POINTER[SDL_Gamepad], ctypes.c_uint8, ctypes.c_uint8, ctypes.c_uint8], SDL_BINARY]
SDL_SendGamepadEffect: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SendGamepadEffect", ctypes.c_bool, [SDL_POINTER[SDL_Gamepad], ctypes.c_void_p, ctypes.c_int], SDL_BINARY]
SDL_CloseGamepad: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CloseGamepad", None, [SDL_POINTER[SDL_Gamepad]], SDL_BINARY]

SDL_GetGamepadAppleSFSymbolsNameForButton: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadAppleSFSymbolsNameForButton", ctypes.c_char_p, [SDL_POINTER[SDL_Gamepad], SDL_GamepadButton], SDL_BINARY]
SDL_GetGamepadAppleSFSymbolsNameForAxis: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGamepadAppleSFSymbolsNameForAxis", ctypes.c_char_p, [SDL_POINTER[SDL_Gamepad], SDL_GamepadAxis], SDL_BINARY]