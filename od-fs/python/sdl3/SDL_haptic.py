import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_FUNC, SDL_TYPE, SDL_BINARY
from .SDL_joystick import SDL_Joystick

class SDL_Haptic(ctypes.c_void_p):
    ...

SDL_HAPTIC_CONSTANT: int = 1 << 0
SDL_HAPTIC_SINE: int = 1 << 1
SDL_HAPTIC_SQUARE: int = 1 << 2
SDL_HAPTIC_TRIANGLE: int = 1 << 3
SDL_HAPTIC_SAWTOOTHUP: int = 1 << 4
SDL_HAPTIC_SAWTOOTHDOWN: int = 1 << 5
SDL_HAPTIC_RAMP: int = 1 << 6
SDL_HAPTIC_SPRING: int = 1 << 7
SDL_HAPTIC_DAMPER: int = 1 << 8
SDL_HAPTIC_INERTIA: int = 1 << 9
SDL_HAPTIC_FRICTION: int = 1 << 10
SDL_HAPTIC_LEFTRIGHT: int = 1 << 11
SDL_HAPTIC_RESERVED1: int = 1 << 12
SDL_HAPTIC_RESERVED2: int = 1 << 13
SDL_HAPTIC_RESERVED3: int = 1 << 14
SDL_HAPTIC_CUSTOM: int = 1 << 15
SDL_HAPTIC_GAIN: int = 1 << 16
SDL_HAPTIC_AUTOCENTER: int = 1 << 17
SDL_HAPTIC_STATUS: int = 1 << 18
SDL_HAPTIC_PAUSE: int = 1 << 19

SDL_HAPTIC_POLAR, SDL_HAPTIC_CARTESIAN, \
    SDL_HAPTIC_SPHERICAL, SDL_HAPTIC_STEERING_AXIS = range(4)

SDL_HAPTIC_INFINITY: int = 4294967295

class SDL_HapticDirection(ctypes.Structure):
    _fields_ = [
        ("type", ctypes.c_uint8),
        ("dir", ctypes.c_int32 * 3)
    ]

class SDL_HapticConstant(ctypes.Structure):
    _fields_ = [
        ("type", ctypes.c_uint16),
        ("direction", SDL_HapticDirection),
        ("length", ctypes.c_uint32),
        ("delay", ctypes.c_uint16),
        ("button", ctypes.c_uint16),
        ("interval", ctypes.c_uint16),
        ("level", ctypes.c_int16),
        ("attack_length", ctypes.c_uint16),
        ("attack_level", ctypes.c_uint16),
        ("fade_length", ctypes.c_uint16),
        ("fade_level", ctypes.c_uint16)
    ]

class SDL_HapticPeriodic(ctypes.Structure):
    _fields_ = [
        ("type", ctypes.c_uint16),
        ("direction", SDL_HapticDirection),
        ("length", ctypes.c_uint32),
        ("delay", ctypes.c_uint16),
        ("button", ctypes.c_uint16),
        ("interval", ctypes.c_uint16),
        ("period", ctypes.c_uint16),
        ("magnitude", ctypes.c_int16),
        ("offset", ctypes.c_int16),
        ("phase", ctypes.c_uint16),
        ("attack_length", ctypes.c_uint16),
        ("attack_level", ctypes.c_uint16),
        ("fade_length", ctypes.c_uint16),
        ("fade_level", ctypes.c_uint16)
    ]

class SDL_HapticCondition(ctypes.Structure):
    _fields_ = [
        ("type", ctypes.c_uint16),
        ("direction", SDL_HapticDirection),
        ("length", ctypes.c_uint32),
        ("delay", ctypes.c_uint16),
        ("button", ctypes.c_uint16),
        ("interval", ctypes.c_uint16),
        ("right_sat", ctypes.c_uint16 * 3),
        ("left_sat", ctypes.c_uint16 * 3),
        ("right_coeff", ctypes.c_int16 * 3),
        ("left_coeff", ctypes.c_int16 * 3),
        ("deadband", ctypes.c_uint16 * 3),
        ("center", ctypes.c_int16 * 3)
    ]

class SDL_HapticRamp(ctypes.Structure):
    _fields_ = [
        ("type", ctypes.c_uint16),
        ("direction", SDL_HapticDirection),
        ("length", ctypes.c_uint32),
        ("delay", ctypes.c_uint16),
        ("button", ctypes.c_uint16),
        ("interval", ctypes.c_uint16),
        ("start", ctypes.c_int16),
        ("end", ctypes.c_int16),
        ("attack_length", ctypes.c_uint16),
        ("attack_level", ctypes.c_uint16),
        ("fade_length", ctypes.c_uint16),
        ("fade_level", ctypes.c_uint16)
    ]

class SDL_HapticLeftRight(ctypes.Structure):
    _fields_ = [
        ("type", ctypes.c_uint16),
        ("length", ctypes.c_uint32),
        ("large_magnitude", ctypes.c_uint16),
        ("small_magnitude", ctypes.c_uint16)
    ]

class SDL_HapticCustom(ctypes.Structure):
    _fields_ = [
        ("type", ctypes.c_uint16),
        ("direction", SDL_HapticDirection),
        ("length", ctypes.c_uint32),
        ("delay", ctypes.c_uint16),
        ("button", ctypes.c_uint16),
        ("interval", ctypes.c_uint16),
        ("channels", ctypes.c_uint8),
        ("period", ctypes.c_uint16),
        ("samples", ctypes.c_uint16),
        ("data", SDL_POINTER[ctypes.c_uint16]),
        ("attack_length", ctypes.c_uint16),
        ("attack_level", ctypes.c_uint16),
        ("fade_length", ctypes.c_uint16),
        ("fade_level", ctypes.c_uint16)
    ]

class SDL_HapticEffect(ctypes.Union):
    _fields_ = [
        ("type", ctypes.c_uint16),
        ("constant", SDL_HapticConstant),
        ("periodic", SDL_HapticPeriodic),
        ("condition", SDL_HapticCondition),
        ("ramp", SDL_HapticRamp),
        ("leftright", SDL_HapticLeftRight),
        ("custom", SDL_HapticCustom)
    ]

SDL_HapticID: typing.TypeAlias = SDL_TYPE["SDL_HapticID", ctypes.c_uint32]

SDL_GetHaptics: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetHaptics", SDL_POINTER[SDL_HapticID], [SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_GetHapticNameForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetHapticNameForID", ctypes.c_char_p, [SDL_HapticID], SDL_BINARY]
SDL_OpenHaptic: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OpenHaptic", SDL_POINTER[SDL_Haptic], [SDL_HapticID], SDL_BINARY]
SDL_GetHapticFromID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetHapticFromID", SDL_POINTER[SDL_Haptic], [SDL_HapticID], SDL_BINARY]
SDL_GetHapticID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetHapticID", SDL_HapticID, [SDL_POINTER[SDL_Haptic]], SDL_BINARY]
SDL_GetHapticName: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetHapticName", ctypes.c_char_p, [SDL_POINTER[SDL_Haptic]], SDL_BINARY]
SDL_IsMouseHaptic: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_IsMouseHaptic", ctypes.c_bool, [], SDL_BINARY]
SDL_OpenHapticFromMouse: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OpenHapticFromMouse", SDL_POINTER[SDL_Haptic], [], SDL_BINARY]
SDL_IsJoystickHaptic: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_IsJoystickHaptic", ctypes.c_bool, [SDL_POINTER[SDL_Joystick]], SDL_BINARY]
SDL_OpenHapticFromJoystick: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OpenHapticFromJoystick", SDL_POINTER[SDL_Haptic], [SDL_POINTER[SDL_Joystick]], SDL_BINARY]
SDL_CloseHaptic: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CloseHaptic", None, [SDL_POINTER[SDL_Haptic]], SDL_BINARY]
SDL_GetMaxHapticEffects: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetMaxHapticEffects", ctypes.c_int, [SDL_POINTER[SDL_Haptic]], SDL_BINARY]
SDL_GetMaxHapticEffectsPlaying: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetMaxHapticEffectsPlaying", ctypes.c_int, [SDL_POINTER[SDL_Haptic]], SDL_BINARY]
SDL_GetHapticFeatures: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetHapticFeatures", ctypes.c_uint32, [SDL_POINTER[SDL_Haptic]], SDL_BINARY]
SDL_GetNumHapticAxes: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetNumHapticAxes", ctypes.c_int, [SDL_POINTER[SDL_Haptic]], SDL_BINARY]
SDL_HapticEffectSupported: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HapticEffectSupported", ctypes.c_bool, [SDL_POINTER[SDL_Haptic], SDL_POINTER[SDL_HapticEffect]], SDL_BINARY]
SDL_CreateHapticEffect: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateHapticEffect", ctypes.c_int, [SDL_POINTER[SDL_Haptic], SDL_POINTER[SDL_HapticEffect]], SDL_BINARY]
SDL_UpdateHapticEffect: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UpdateHapticEffect", ctypes.c_bool, [SDL_POINTER[SDL_Haptic], ctypes.c_int, SDL_POINTER[SDL_HapticEffect]], SDL_BINARY]
SDL_RunHapticEffect: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RunHapticEffect", ctypes.c_bool, [SDL_POINTER[SDL_Haptic], ctypes.c_int, ctypes.c_uint32], SDL_BINARY]
SDL_StopHapticEffect: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_StopHapticEffect", ctypes.c_bool, [SDL_POINTER[SDL_Haptic], ctypes.c_int], SDL_BINARY]
SDL_DestroyHapticEffect: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DestroyHapticEffect", None, [SDL_POINTER[SDL_Haptic], ctypes.c_int], SDL_BINARY]
SDL_GetHapticEffectStatus: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetHapticEffectStatus", ctypes.c_bool, [SDL_POINTER[SDL_Haptic], ctypes.c_int], SDL_BINARY]
SDL_SetHapticGain: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetHapticGain", ctypes.c_bool, [SDL_POINTER[SDL_Haptic], ctypes.c_int], SDL_BINARY]
SDL_SetHapticAutocenter: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetHapticAutocenter", ctypes.c_bool, [SDL_POINTER[SDL_Haptic], ctypes.c_int], SDL_BINARY]
SDL_PauseHaptic: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_PauseHaptic", ctypes.c_bool, [SDL_POINTER[SDL_Haptic]], SDL_BINARY]
SDL_ResumeHaptic: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ResumeHaptic", ctypes.c_bool, [SDL_POINTER[SDL_Haptic]], SDL_BINARY]
SDL_StopHapticEffects: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_StopHapticEffects", ctypes.c_bool, [SDL_POINTER[SDL_Haptic]], SDL_BINARY]
SDL_HapticRumbleSupported: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HapticRumbleSupported", ctypes.c_bool, [SDL_POINTER[SDL_Haptic]], SDL_BINARY]
SDL_InitHapticRumble: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_InitHapticRumble", ctypes.c_bool, [SDL_POINTER[SDL_Haptic]], SDL_BINARY]
SDL_PlayHapticRumble: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_PlayHapticRumble", ctypes.c_bool, [SDL_POINTER[SDL_Haptic], ctypes.c_float, ctypes.c_uint32], SDL_BINARY]
SDL_StopHapticRumble: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_StopHapticRumble", ctypes.c_bool, [SDL_POINTER[SDL_Haptic]], SDL_BINARY]