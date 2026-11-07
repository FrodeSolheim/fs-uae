import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_ENUM, SDL_FUNC, SDL_TYPE, SDL_BINARY
from .SDL_properties import SDL_PropertiesID

class SDL_Sensor(ctypes.c_void_p):
    ...

SDL_SensorID: typing.TypeAlias = SDL_TYPE["SDL_SensorID", ctypes.c_uint32]

SDL_STANDARD_GRAVITY: float = 9.80665

SDL_SensorType: typing.TypeAlias = SDL_TYPE["SDL_SensorType", SDL_ENUM]

SDL_SENSOR_INVALID, SDL_SENSOR_UNKNOWN, SDL_SENSOR_ACCEL, SDL_SENSOR_GYRO, \
    SDL_SENSOR_ACCEL_L, SDL_SENSOR_GYRO_L, SDL_SENSOR_ACCEL_R, SDL_SENSOR_GYRO_R = range(-1, 7)

SDL_GetSensors: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSensors", SDL_POINTER[SDL_SensorID], [SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_GetSensorNameForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSensorNameForID", ctypes.c_char_p, [SDL_SensorID], SDL_BINARY]
SDL_GetSensorTypeForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSensorTypeForID", SDL_SensorType, [SDL_SensorID], SDL_BINARY]
SDL_GetSensorNonPortableTypeForID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSensorNonPortableTypeForID", ctypes.c_int, [SDL_SensorID], SDL_BINARY]
SDL_OpenSensor: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OpenSensor", SDL_POINTER[SDL_Sensor], [SDL_SensorID], SDL_BINARY]
SDL_GetSensorFromID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSensorFromID", SDL_POINTER[SDL_Sensor], [SDL_SensorID], SDL_BINARY]
SDL_GetSensorProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSensorProperties", SDL_PropertiesID, [SDL_POINTER[SDL_Sensor]], SDL_BINARY]
SDL_GetSensorName: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSensorName", ctypes.c_char_p, [SDL_POINTER[SDL_Sensor]], SDL_BINARY]
SDL_GetSensorType: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSensorType", SDL_SensorType, [SDL_POINTER[SDL_Sensor]], SDL_BINARY]
SDL_GetSensorNonPortableType: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSensorNonPortableType", ctypes.c_int, [SDL_POINTER[SDL_Sensor]], SDL_BINARY]
SDL_GetSensorID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSensorID", SDL_SensorID, [SDL_POINTER[SDL_Sensor]], SDL_BINARY]
SDL_GetSensorData: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSensorData", ctypes.c_bool, [SDL_POINTER[SDL_Sensor], SDL_POINTER[ctypes.c_float], ctypes.c_int], SDL_BINARY]
SDL_CloseSensor: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CloseSensor", None, [SDL_POINTER[SDL_Sensor]], SDL_BINARY]
SDL_UpdateSensors: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UpdateSensors", None, [], SDL_BINARY]