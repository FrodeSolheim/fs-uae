import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_ENUM, \
    SDL_FUNC, SDL_TYPE, SDL_BINARY
from .SDL_pixels import SDL_PixelFormat, SDL_Colorspace
from .SDL_properties import SDL_PropertiesID
from .SDL_surface import SDL_Surface

SDL_CameraID: typing.TypeAlias = SDL_TYPE["SDL_CameraID", ctypes.c_uint32]

class SDL_Camera(ctypes.c_void_p):
    ...

class SDL_CameraSpec(ctypes.Structure):
    _fields_ = [
        ("format", SDL_PixelFormat),
        ("colorspace", SDL_Colorspace),
        ("width", ctypes.c_int),
        ("height", ctypes.c_int),
        ("framerate_numerator", ctypes.c_int),
        ("framerate_denominator", ctypes.c_int)
    ]

SDL_CameraPosition: typing.TypeAlias = SDL_TYPE["SDL_CameraPosition", SDL_ENUM]

SDL_CAMERA_POSITION_UNKNOWN, SDL_CAMERA_POSITION_FRONT_FACING, SDL_CAMERA_POSITION_BACK_FACING = range(3)

SDL_GetNumCameraDrivers: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetNumCameraDrivers", ctypes.c_int, [], SDL_BINARY]
SDL_GetCameraDriver: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetCameraDriver", ctypes.c_char_p, [ctypes.c_int], SDL_BINARY]
SDL_GetCurrentCameraDriver: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetCurrentCameraDriver", ctypes.c_char_p, [], SDL_BINARY]
SDL_GetCameras: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetCameras", SDL_POINTER[SDL_CameraID], [SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_GetCameraSupportedFormats: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetCameraSupportedFormats", SDL_POINTER[SDL_POINTER[SDL_CameraSpec]], [SDL_CameraID, SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_GetCameraName: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetCameraName", ctypes.c_char_p, [SDL_CameraID], SDL_BINARY]
SDL_GetCameraPosition: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetCameraPosition", SDL_CameraPosition, [SDL_CameraID], SDL_BINARY]
SDL_OpenCamera: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OpenCamera", SDL_POINTER[SDL_Camera], [SDL_CameraID, SDL_POINTER[SDL_CameraSpec]], SDL_BINARY]
SDL_GetCameraPermissionState: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetCameraPermissionState", ctypes.c_int, [SDL_POINTER[SDL_Camera]], SDL_BINARY]
SDL_GetCameraID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetCameraID", SDL_CameraID, [SDL_POINTER[SDL_Camera]], SDL_BINARY]
SDL_GetCameraProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetCameraProperties", SDL_PropertiesID, [SDL_POINTER[SDL_Camera]], SDL_BINARY]
SDL_GetCameraFormat: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetCameraFormat", ctypes.c_bool, [SDL_POINTER[SDL_Camera], SDL_POINTER[SDL_CameraSpec]], SDL_BINARY]
SDL_AcquireCameraFrame: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_AcquireCameraFrame", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_Camera], SDL_POINTER[ctypes.c_uint64]], SDL_BINARY]
SDL_ReleaseCameraFrame: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReleaseCameraFrame", None, [SDL_POINTER[SDL_Camera], SDL_POINTER[SDL_Surface]], SDL_BINARY]
SDL_CloseCamera: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CloseCamera", None, [SDL_POINTER[SDL_Camera]], SDL_BINARY]