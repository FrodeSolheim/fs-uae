import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_ENUM, \
    SDL_FUNC, SDL_TYPE, SDL_BINARY
from .SDL_stdinc import SDL_Time

class SDL_DateTime(ctypes.Structure):
    _fields_ = [
        ("year", ctypes.c_int),
        ("month", ctypes.c_int),
        ("day", ctypes.c_int),
        ("hour", ctypes.c_int),
        ("minute", ctypes.c_int),
        ("second", ctypes.c_int),
        ("nanosecond", ctypes.c_int),
        ("day_of_week", ctypes.c_int),
        ("utc_offset", ctypes.c_int)
    ]

SDL_DateFormat: typing.TypeAlias = SDL_TYPE["SDL_DateFormat", SDL_ENUM]

SDL_DATE_FORMAT_YYYYMMDD, SDL_DATE_FORMAT_DDMMYYYY, SDL_DATE_FORMAT_MMDDYYYY = range(3)

SDL_TimeFormat: typing.TypeAlias = SDL_TYPE["SDL_TimeFormat", SDL_ENUM]

SDL_TIME_FORMAT_24HR, SDL_TIME_FORMAT_12HR = range(2)

SDL_GetDateTimeLocalePreferences: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetDateTimeLocalePreferences", ctypes.c_bool, [SDL_POINTER[SDL_DateFormat], SDL_POINTER[SDL_TimeFormat]], SDL_BINARY]
SDL_GetCurrentTime: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetCurrentTime", ctypes.c_bool, [SDL_POINTER[SDL_Time]], SDL_BINARY]
SDL_TimeToDateTime: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_TimeToDateTime", ctypes.c_bool, [SDL_Time, SDL_POINTER[SDL_DateTime], ctypes.c_bool], SDL_BINARY]
SDL_DateTimeToTime: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DateTimeToTime", ctypes.c_bool, [SDL_POINTER[SDL_DateTime], SDL_POINTER[SDL_Time]], SDL_BINARY]
SDL_TimeToWindows: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_TimeToWindows", None, [SDL_Time, SDL_POINTER[ctypes.c_uint32], SDL_POINTER[ctypes.c_uint32]], SDL_BINARY]
SDL_TimeFromWindows: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_TimeFromWindows", SDL_Time, [ctypes.c_uint32, ctypes.c_uint32], SDL_BINARY]
SDL_GetDaysInMonth: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetDaysInMonth", ctypes.c_int, [ctypes.c_int, ctypes.c_int], SDL_BINARY]
SDL_GetDayOfYear: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetDayOfYear", ctypes.c_int, [ctypes.c_int, ctypes.c_int, ctypes.c_int], SDL_BINARY]
SDL_GetDayOfWeek: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetDayOfWeek", ctypes.c_int, [ctypes.c_int, ctypes.c_int, ctypes.c_int], SDL_BINARY]