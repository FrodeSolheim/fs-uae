import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_FUNC_TYPE, \
    SDL_FUNC, SDL_TYPE, SDL_BINARY, SDL_ENUM
from .SDL_sensor import SDL_SensorType
from .SDL_properties import SDL_PropertiesID
from .SDL_power import SDL_PowerState
from .SDL_guid import SDL_GUID

class SDL_Joystick(ctypes.c_void_p):
    ...

SDL_JoystickID: typing.TypeAlias = SDL_TYPE["SDL_JoystickID", ctypes.c_uint32]
SDL_JoystickType: typing.TypeAlias = SDL_TYPE["SDL_JoystickType", SDL_ENUM]

SDL_JOYSTICK_TYPE_UNKNOWN, SDL_JOYSTICK_TYPE_GAMEPAD, SDL_JOYSTICK_TYPE_WHEEL , SDL_JOYSTICK_TYPE_ARCADE_, \
    SDL_JOYSTICK_TYPE_FLIGHT_, SDL_JOYSTICK_TYPE_DANCE_P, SDL_JOYSTICK_TYPE_GUITAR , SDL_JOYSTICK_TYPE_DRUM_KI, \
        SDL_JOYSTICK_TYPE_ARCADE_, SDL_JOYSTICK_TYPE_THROTTL, SDL_JOYSTICK_TYPE_COUNT = range(11)

SDL_JoystickConnectionState: typing.TypeAlias = SDL_TYPE["SDL_JoystickConnectionState", SDL_ENUM]

SDL_JOYSTICK_CONNECTION_INVALID, SDL_JOYSTICK_CONNECTION_UNKNOWN, \
    SDL_JOYSTICK_CONNECTION_WIRED, SDL_JOYSTICK_CONNECTION_WIRELESS = range(-1, 3)

SDL_JOYSTICK_AXIS_MAX, SDL_JOYSTICK_AXIS_MIN = 32767, -32768

SDL_LockJoysticks: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LockJoysticks", None, [], SDL_BINARY]
SDL_UnlockJoysticks: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UnlockJoysticks", None, [], SDL_BINARY]
SDL_HasJoystick: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HasJoystick", ctypes.c_bool, [], SDL_BINARY]
SDL_GetJoysticks: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoysticks", SDL_POINTER[SDL_JoystickID], [SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_GetJoystickNameForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickNameForID", ctypes.c_char_p, [SDL_JoystickID], SDL_BINARY]
SDL_GetJoystickPathForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickPathForID", ctypes.c_char_p, [SDL_JoystickID], SDL_BINARY]
SDL_GetJoystickPlayerIndexForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickPlayerIndexForID", ctypes.c_int, [SDL_JoystickID], SDL_BINARY]
SDL_GetJoystickGUIDForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickGUIDForID", SDL_GUID, [SDL_JoystickID], SDL_BINARY]
SDL_GetJoystickVendorForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickVendorForID", ctypes.c_uint16, [SDL_JoystickID], SDL_BINARY]
SDL_GetJoystickProductForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickProductForID", ctypes.c_uint16, [SDL_JoystickID], SDL_BINARY]
SDL_GetJoystickProductVersionForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickProductVersionForID", ctypes.c_uint16, [SDL_JoystickID], SDL_BINARY]
SDL_GetJoystickTypeForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickTypeForID", SDL_JoystickType, [SDL_JoystickID], SDL_BINARY]
SDL_OpenJoystick: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OpenJoystick", SDL_POINTER[SDL_Joystick], [SDL_JoystickID], SDL_BINARY]
SDL_GetJoystickFromID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickFromID", SDL_POINTER[SDL_Joystick], [SDL_JoystickID], SDL_BINARY]
SDL_GetJoystickFromPlayerIndex: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickFromPlayerIndex", SDL_POINTER[SDL_Joystick], [ctypes.c_int], SDL_BINARY]

class SDL_VirtualJoystickTouchpadDesc(ctypes.Structure):
    _fields_ = [
        ("nfingers", ctypes.c_uint16),
        ("padding", ctypes.c_uint16 * 3)
    ]

class SDL_VirtualJoystickSensorDesc(ctypes.Structure):
    _fields_ = [
        ("type", SDL_SensorType),
        ("rate", ctypes.c_float)
    ]

class SDL_VirtualJoystickDesc(ctypes.Structure):
    _fields_ = [
        ("version", ctypes.c_uint32),
        ("type", ctypes.c_uint16),
        ("padding", ctypes.c_uint16),
        ("vendor_id", ctypes.c_uint16),
        ("product_id", ctypes.c_uint16),
        ("naxes", ctypes.c_uint16),
        ("nbuttons", ctypes.c_uint16),
        ("nballs", ctypes.c_uint16),
        ("nhats", ctypes.c_uint16),
        ("ntouchpads", ctypes.c_uint16),
        ("nsensors", ctypes.c_uint16),
        ("padding2", ctypes.c_uint16 * 2),
        ("button_mask", ctypes.c_uint32),
        ("axis_mask", ctypes.c_uint32),
        ("name", ctypes.c_char_p),
        ("touchpads", SDL_POINTER[SDL_VirtualJoystickTouchpadDesc]),
        ("sensors", SDL_POINTER[SDL_VirtualJoystickSensorDesc]),
        ("userdata", ctypes.c_void_p),
        ("Update", SDL_FUNC_TYPE["SDL_VirtualJoystickDesc.Update", None, [ctypes.c_void_p]]),
        ("SetPlayerIndex", SDL_FUNC_TYPE["SDL_VirtualJoystickDesc.SetPlayerIndex", None, [ctypes.c_void_p, ctypes.c_int]]),
        ("Rumble", SDL_FUNC_TYPE["SDL_VirtualJoystickDesc.Rumble", ctypes.c_bool, [ctypes.c_void_p, ctypes.c_uint16, ctypes.c_uint16]]),
        ("RumbleTriggers", SDL_FUNC_TYPE["SDL_VirtualJoystickDesc.RumbleTriggers", ctypes.c_bool, [ctypes.c_void_p, ctypes.c_uint16, ctypes.c_uint16]]),
        ("SetLED", SDL_FUNC_TYPE["SDL_VirtualJoystickDesc.SetLED", ctypes.c_bool, [ctypes.c_void_p, ctypes.c_uint8, ctypes.c_uint8, ctypes.c_uint8]]),
        ("SendEffect", SDL_FUNC_TYPE["SDL_VirtualJoystickDesc.SendEffect", ctypes.c_bool, [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_int]]),
        ("SetSensorsEnabled", SDL_FUNC_TYPE["SDL_VirtualJoystickDesc.SetSensorsEnabled", ctypes.c_bool, [ctypes.c_void_p, ctypes.c_bool]]),
        ("Cleanup", SDL_FUNC_TYPE["SDL_VirtualJoystickDesc.Cleanup", None, [ctypes.c_void_p]])
    ]

SDL_AttachVirtualJoystick: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_AttachVirtualJoystick", SDL_JoystickID, [SDL_POINTER[SDL_VirtualJoystickDesc]], SDL_BINARY]
SDL_DetachVirtualJoystick: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DetachVirtualJoystick", ctypes.c_bool, [SDL_JoystickID], SDL_BINARY]
SDL_IsJoystickVirtual: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_IsJoystickVirtual", ctypes.c_bool, [SDL_JoystickID], SDL_BINARY]
SDL_SetJoystickVirtualAxis: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetJoystickVirtualAxis", ctypes.c_bool, [SDL_POINTER[SDL_Joystick], ctypes.c_int, ctypes.c_int16], SDL_BINARY]
SDL_SetJoystickVirtualBall: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetJoystickVirtualBall", ctypes.c_bool, [SDL_POINTER[SDL_Joystick], ctypes.c_int, ctypes.c_int16, ctypes.c_int16], SDL_BINARY]
SDL_SetJoystickVirtualButton: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetJoystickVirtualButton", ctypes.c_bool, [SDL_POINTER[SDL_Joystick], ctypes.c_int, ctypes.c_bool], SDL_BINARY]
SDL_SetJoystickVirtualHat: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetJoystickVirtualHat", ctypes.c_bool, [SDL_POINTER[SDL_Joystick], ctypes.c_int, ctypes.c_uint8], SDL_BINARY]
SDL_SetJoystickVirtualTouchpad: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetJoystickVirtualTouchpad", ctypes.c_bool, [SDL_POINTER[SDL_Joystick], ctypes.c_int, ctypes.c_int, ctypes.c_bool, ctypes.c_float, ctypes.c_float, ctypes.c_float], SDL_BINARY]
SDL_SendJoystickVirtualSensorData: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SendJoystickVirtualSensorData", ctypes.c_bool, [SDL_POINTER[SDL_Joystick], SDL_SensorType, ctypes.c_uint64, SDL_POINTER[ctypes.c_float], ctypes.c_int], SDL_BINARY]
SDL_GetJoystickProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickProperties", SDL_PropertiesID, [SDL_POINTER[SDL_Joystick]], SDL_BINARY]

SDL_PROP_JOYSTICK_CAP_MONO_LED_BOOLEAN: bytes = "SDL.joystick.cap.mono_led".encode()
SDL_PROP_JOYSTICK_CAP_RGB_LED_BOOLEAN: bytes = "SDL.joystick.cap.rgb_led".encode()
SDL_PROP_JOYSTICK_CAP_PLAYER_LED_BOOLEAN: bytes = "SDL.joystick.cap.player_led".encode()
SDL_PROP_JOYSTICK_CAP_RUMBLE_BOOLEAN: bytes = "SDL.joystick.cap.rumble".encode()
SDL_PROP_JOYSTICK_CAP_TRIGGER_RUMBLE_BOOLEAN: bytes = "SDL.joystick.cap.trigger_rumble".encode()

SDL_GetJoystickName: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickName", ctypes.c_char_p, [SDL_POINTER[SDL_Joystick]], SDL_BINARY]
SDL_GetJoystickPath: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickPath", ctypes.c_char_p, [SDL_POINTER[SDL_Joystick]], SDL_BINARY]
SDL_GetJoystickPlayerIndex: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickPlayerIndex", ctypes.c_int, [SDL_POINTER[SDL_Joystick]], SDL_BINARY]
SDL_SetJoystickPlayerIndex: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetJoystickPlayerIndex", ctypes.c_bool, [SDL_POINTER[SDL_Joystick], ctypes.c_int], SDL_BINARY]
SDL_GetJoystickGUID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickGUID", SDL_GUID, [SDL_POINTER[SDL_Joystick]], SDL_BINARY]
SDL_GetJoystickVendor: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickVendor", ctypes.c_uint16, [SDL_POINTER[SDL_Joystick]], SDL_BINARY]
SDL_GetJoystickProduct: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickProduct", ctypes.c_uint16, [SDL_POINTER[SDL_Joystick]], SDL_BINARY]
SDL_GetJoystickProductVersion: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickProductVersion", ctypes.c_uint16, [SDL_POINTER[SDL_Joystick]], SDL_BINARY]
SDL_GetJoystickFirmwareVersion: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickFirmwareVersion", ctypes.c_uint16, [SDL_POINTER[SDL_Joystick]], SDL_BINARY]
SDL_GetJoystickSerial: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickSerial", ctypes.c_char_p, [SDL_POINTER[SDL_Joystick]], SDL_BINARY]
SDL_GetJoystickType: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickType", SDL_JoystickType, [SDL_POINTER[SDL_Joystick]], SDL_BINARY]
SDL_GetJoystickGUIDInfo: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickGUIDInfo", None, [SDL_GUID, SDL_POINTER[ctypes.c_uint16], SDL_POINTER[ctypes.c_uint16], SDL_POINTER[ctypes.c_uint16], SDL_POINTER[ctypes.c_uint16]], SDL_BINARY]
SDL_JoystickConnected: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_JoystickConnected", ctypes.c_bool, [SDL_POINTER[SDL_Joystick]], SDL_BINARY]
SDL_GetJoystickID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickID", SDL_JoystickID, [SDL_POINTER[SDL_Joystick]], SDL_BINARY]
SDL_GetNumJoystickAxes: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetNumJoystickAxes", ctypes.c_int, [SDL_POINTER[SDL_Joystick]], SDL_BINARY]
SDL_GetNumJoystickBalls: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetNumJoystickBalls", ctypes.c_int, [SDL_POINTER[SDL_Joystick]], SDL_BINARY]
SDL_GetNumJoystickHats: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetNumJoystickHats", ctypes.c_int, [SDL_POINTER[SDL_Joystick]], SDL_BINARY]
SDL_GetNumJoystickButtons: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetNumJoystickButtons", ctypes.c_int, [SDL_POINTER[SDL_Joystick]], SDL_BINARY]
SDL_SetJoystickEventsEnabled: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetJoystickEventsEnabled", None, [ctypes.c_bool], SDL_BINARY]
SDL_JoystickEventsEnabled: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_JoystickEventsEnabled", ctypes.c_bool, [], SDL_BINARY]
SDL_UpdateJoysticks: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UpdateJoysticks", None, [], SDL_BINARY]
SDL_GetJoystickAxis: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickAxis", ctypes.c_int16, [SDL_POINTER[SDL_Joystick], ctypes.c_int], SDL_BINARY]
SDL_GetJoystickAxisInitialState: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickAxisInitialState", ctypes.c_bool, [SDL_POINTER[SDL_Joystick], ctypes.c_int, SDL_POINTER[ctypes.c_int16]], SDL_BINARY]
SDL_GetJoystickBall: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickBall", ctypes.c_bool, [SDL_POINTER[SDL_Joystick], ctypes.c_int, SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_GetJoystickHat: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickHat", ctypes.c_uint8, [SDL_POINTER[SDL_Joystick], ctypes.c_int], SDL_BINARY]

SDL_HAT_CENTERED, SDL_HAT_UP, SDL_HAT_RIGHT, SDL_HAT_DOWN, SDL_HAT_LEFT = 0x00, 0x01, 0x02, 0x04, 0x08

SDL_HAT_RIGHTUP, SDL_HAT_RIGHTDOWN, SDL_HAT_LEFTUP, SDL_HAT_LEFTDOWN = \
    SDL_HAT_RIGHT | SDL_HAT_UP, SDL_HAT_RIGHT | SDL_HAT_DOWN, SDL_HAT_LEFT | SDL_HAT_UP, SDL_HAT_LEFT | SDL_HAT_DOWN

SDL_GetJoystickButton: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickButton", ctypes.c_bool, [SDL_POINTER[SDL_Joystick], ctypes.c_int], SDL_BINARY]
SDL_RumbleJoystick: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RumbleJoystick", ctypes.c_bool, [SDL_POINTER[SDL_Joystick], ctypes.c_uint16, ctypes.c_uint16, ctypes.c_uint32], SDL_BINARY]
SDL_RumbleJoystickTriggers: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RumbleJoystickTriggers", ctypes.c_bool, [SDL_POINTER[SDL_Joystick], ctypes.c_uint16, ctypes.c_uint16, ctypes.c_uint32], SDL_BINARY]
SDL_SetJoystickLED: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetJoystickLED", ctypes.c_bool, [SDL_POINTER[SDL_Joystick], ctypes.c_uint8, ctypes.c_uint8, ctypes.c_uint8], SDL_BINARY]
SDL_SendJoystickEffect: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SendJoystickEffect", ctypes.c_bool, [SDL_POINTER[SDL_Joystick], ctypes.c_void_p, ctypes.c_int], SDL_BINARY]
SDL_CloseJoystick: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CloseJoystick", None, [SDL_POINTER[SDL_Joystick]], SDL_BINARY]
SDL_GetJoystickConnectionState: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickConnectionState", SDL_JoystickConnectionState, [SDL_POINTER[SDL_Joystick]], SDL_BINARY]
SDL_GetJoystickPowerInfo: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetJoystickPowerInfo", SDL_PowerState, [SDL_POINTER[SDL_Joystick], SDL_POINTER[ctypes.c_int]], SDL_BINARY]