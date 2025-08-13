import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_ENUM, \
    SDL_FUNC, SDL_TYPE, SDL_BINARY
from .SDL_mouse import SDL_MouseID

SDL_TouchID: typing.TypeAlias = SDL_TYPE["SDL_TouchID", ctypes.c_uint64]
SDL_FingerID: typing.TypeAlias = SDL_TYPE["SDL_FingerID", ctypes.c_uint64]

SDL_TouchDeviceType: typing.TypeAlias = SDL_TYPE["SDL_TouchDeviceType", SDL_ENUM]

SDL_TOUCH_DEVICE_INVALID, SDL_TOUCH_DEVICE_DIRECT, SDL_TOUCH_DEVICE_INDIRECT_ABSOLUTE, \
    SDL_TOUCH_DEVICE_INDIRECT_RELATIVE = range(-1, 3)

class SDL_Finger(ctypes.Structure):
    _fields_ = [
        ("id", SDL_FingerID),
        ("x", ctypes.c_float),
        ("y", ctypes.c_float),
        ("pressure", ctypes.c_float)
    ]

SDL_TOUCH_MOUSEID, SDL_MOUSE_TOUCHID = \
    SDL_MouseID(-1), SDL_TouchID(-1)

SDL_GetTouchDevices: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTouchDevices", SDL_POINTER[SDL_TouchID], [SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_GetTouchDeviceName: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTouchDeviceName", ctypes.c_char_p, [SDL_TouchID], SDL_BINARY]
SDL_GetTouchDeviceType: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTouchDeviceType", SDL_TouchDeviceType, [SDL_TouchID], SDL_BINARY]
SDL_GetTouchFingers: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTouchFingers", SDL_POINTER[SDL_POINTER[SDL_Finger]], [SDL_TouchID, SDL_POINTER[ctypes.c_int]], SDL_BINARY]
