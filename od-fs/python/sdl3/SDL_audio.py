import ctypes, typing, collections.abc as abc

from . import SDL_FUNC_TYPE, SDL_POINTER, \
    SDL_FUNC, SDL_TYPE, SDL_BINARY, SDL_ENUM
from .SDL_properties import SDL_PropertiesID
from .SDL_endian import SDL_BYTEORDER, SDL_LIL_ENDIAN
from .SDL_iostream import SDL_IOStream

SDL_AUDIO_MASK_BITSIZE: int = 0xFF
SDL_AUDIO_MASK_FLOAT: int = 1 << 8
SDL_AUDIO_MASK_BIG_ENDIAN: int = 1 << 12
SDL_AUDIO_MASK_SIGNED: int = 1 << 15

SDL_DEFINE_AUDIO_FORMAT: abc.Callable[[int, int, int, int], int] = lambda signed, bigendian, _float, size: \
    (signed << 15) | (bigendian << 12) | (_float << 8) | (size & SDL_AUDIO_MASK_BITSIZE)

SDL_AudioFormat: typing.TypeAlias = SDL_TYPE["SDL_AudioFormat", SDL_ENUM]

SDL_AUDIO_UNKNOWN: int = 0x0000
SDL_AUDIO_U8: int = 0x0008
SDL_AUDIO_S8: int = 0x8008
SDL_AUDIO_S16LE: int = 0x8010
SDL_AUDIO_S16BE: int = 0x9010
SDL_AUDIO_S32LE: int = 0x8020
SDL_AUDIO_S32BE: int = 0x9020
SDL_AUDIO_F32LE: int = 0x8120
SDL_AUDIO_F32BE: int = 0x9120

if SDL_BYTEORDER == SDL_LIL_ENDIAN:
    SDL_AUDIO_S16, SDL_AUDIO_S32, SDL_AUDIO_F32 = SDL_AUDIO_S16LE, SDL_AUDIO_S32LE, SDL_AUDIO_F32LE

else:
    SDL_AUDIO_S16, SDL_AUDIO_S32, SDL_AUDIO_F32 = SDL_AUDIO_S16BE, SDL_AUDIO_S32BE, SDL_AUDIO_F32BE

SDL_AUDIO_BITSIZE: abc.Callable[[SDL_AudioFormat], int] = lambda x: x.value & SDL_AUDIO_MASK_BITSIZE
SDL_AUDIO_BYTESIZE: abc.Callable[[SDL_AudioFormat], int] = lambda x: int(SDL_AUDIO_BITSIZE(x) / 8)

SDL_AUDIO_ISFLOAT: abc.Callable[[SDL_AudioFormat], int] = lambda x: x.value & SDL_AUDIO_MASK_FLOAT
SDL_AUDIO_ISBIGENDIAN: abc.Callable[[SDL_AudioFormat], int] = lambda x: x.value & SDL_AUDIO_MASK_BIG_ENDIAN
SDL_AUDIO_ISLITTLEENDIAN: abc.Callable[[SDL_AudioFormat], bool] = lambda x: not SDL_AUDIO_ISBIGENDIAN(x)
SDL_AUDIO_ISSIGNED: abc.Callable[[SDL_AudioFormat], int] = lambda x: x.value & SDL_AUDIO_MASK_SIGNED
SDL_AUDIO_ISINT: abc.Callable[[SDL_AudioFormat], bool] = lambda x: not SDL_AUDIO_ISFLOAT(x)
SDL_AUDIO_ISUNSIGNED: abc.Callable[[SDL_AudioFormat], bool] = lambda x: not SDL_AUDIO_ISSIGNED(x)

SDL_AudioDeviceID: typing.TypeAlias = SDL_TYPE["SDL_AudioDeviceID", ctypes.c_uint32]

SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK: int = 0xFFFFFFFF
SDL_AUDIO_DEVICE_DEFAULT_RECORDING: int = 0xFFFFFFFE

class SDL_AudioSpec(ctypes.Structure):
    _fields_ = [
        ("format", SDL_AudioFormat),
        ("channels", ctypes.c_int),
        ("freq", ctypes.c_int)
    ]

SDL_AUDIO_FRAMESIZE: abc.Callable[[SDL_AudioSpec], int] = lambda x: SDL_AUDIO_BYTESIZE(x.format) * x.channels

class SDL_AudioStream(ctypes.c_void_p):
    ...

SDL_GetNumAudioDrivers: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetNumAudioDrivers", ctypes.c_int, [], SDL_BINARY]
SDL_GetAudioDriver: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAudioDriver", ctypes.c_char_p, [ctypes.c_int], SDL_BINARY]
SDL_GetCurrentAudioDriver: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetCurrentAudioDriver", ctypes.c_char_p, [], SDL_BINARY]

SDL_GetAudioPlaybackDevices: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAudioPlaybackDevices", SDL_POINTER[SDL_AudioDeviceID], [SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_GetAudioRecordingDevices: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAudioRecordingDevices", SDL_POINTER[SDL_AudioDeviceID], [SDL_POINTER[ctypes.c_int]], SDL_BINARY]

SDL_GetAudioDeviceName: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAudioDeviceName", ctypes.c_char_p, [SDL_AudioDeviceID], SDL_BINARY]
SDL_GetAudioDeviceFormat: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAudioDeviceFormat", ctypes.c_bool, [SDL_AudioDeviceID, SDL_POINTER[SDL_AudioSpec], SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_GetAudioDeviceChannelMap: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAudioDeviceChannelMap", SDL_POINTER[ctypes.c_int], [SDL_AudioDeviceID, SDL_POINTER[ctypes.c_int]], SDL_BINARY]

SDL_OpenAudioDevice: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OpenAudioDevice", SDL_AudioDeviceID, [SDL_AudioDeviceID, SDL_POINTER[SDL_AudioSpec]], SDL_BINARY]
SDL_IsAudioDevicePhysical: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_IsAudioDevicePhysical", ctypes.c_bool, [SDL_AudioDeviceID], SDL_BINARY]
SDL_IsAudioDevicePlayback: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_IsAudioDevicePlayback", ctypes.c_bool, [SDL_AudioDeviceID], SDL_BINARY]
SDL_PauseAudioDevice: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_PauseAudioDevice", ctypes.c_bool, [SDL_AudioDeviceID], SDL_BINARY]
SDL_ResumeAudioDevice: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ResumeAudioDevice", ctypes.c_bool, [SDL_AudioDeviceID], SDL_BINARY]
SDL_AudioDevicePaused: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_AudioDevicePaused", ctypes.c_bool, [SDL_AudioDeviceID], SDL_BINARY]
SDL_GetAudioDeviceGain: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAudioDeviceGain", ctypes.c_float, [SDL_AudioDeviceID], SDL_BINARY]
SDL_SetAudioDeviceGain: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetAudioDeviceGain", ctypes.c_bool, [SDL_AudioDeviceID, ctypes.c_float], SDL_BINARY]
SDL_CloseAudioDevice: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CloseAudioDevice", None, [SDL_AudioDeviceID], SDL_BINARY]

SDL_BindAudioStreams: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BindAudioStreams", ctypes.c_bool, [SDL_AudioDeviceID, SDL_POINTER[SDL_POINTER[SDL_AudioStream]], ctypes.c_int], SDL_BINARY]
SDL_BindAudioStream: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BindAudioStream", ctypes.c_bool, [SDL_AudioDeviceID, SDL_POINTER[SDL_AudioStream]], SDL_BINARY]
SDL_UnbindAudioStreams: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UnbindAudioStreams", None, [SDL_POINTER[SDL_POINTER[SDL_AudioStream]], ctypes.c_int], SDL_BINARY]
SDL_UnbindAudioStream: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UnbindAudioStream", None, [SDL_POINTER[SDL_AudioStream]], SDL_BINARY]
SDL_GetAudioStreamDevice: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAudioStreamDevice", SDL_AudioDeviceID, [SDL_POINTER[SDL_AudioStream]], SDL_BINARY]
SDL_CreateAudioStream: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateAudioStream", SDL_POINTER[SDL_AudioStream], [SDL_POINTER[SDL_AudioSpec], SDL_POINTER[SDL_AudioSpec]], SDL_BINARY]
SDL_GetAudioStreamProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAudioStreamProperties", SDL_PropertiesID, [SDL_POINTER[SDL_AudioStream]], SDL_BINARY]
SDL_GetAudioStreamFormat: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAudioStreamFormat", ctypes.c_bool, [SDL_POINTER[SDL_AudioStream], SDL_POINTER[SDL_AudioSpec], SDL_POINTER[SDL_AudioSpec]], SDL_BINARY]
SDL_SetAudioStreamFormat: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetAudioStreamFormat", ctypes.c_bool, [SDL_POINTER[SDL_AudioStream], SDL_POINTER[SDL_AudioSpec], SDL_POINTER[SDL_AudioSpec]], SDL_BINARY]
SDL_GetAudioStreamFrequencyRatio: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAudioStreamFrequencyRatio", ctypes.c_float, [SDL_POINTER[SDL_AudioStream]], SDL_BINARY]
SDL_SetAudioStreamFrequencyRatio: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetAudioStreamFrequencyRatio", ctypes.c_bool, [SDL_POINTER[SDL_AudioStream], ctypes.c_float], SDL_BINARY]
SDL_GetAudioStreamGain: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAudioStreamGain", ctypes.c_float, [SDL_POINTER[SDL_AudioStream]], SDL_BINARY]
SDL_SetAudioStreamGain: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetAudioStreamGain", ctypes.c_bool, [SDL_POINTER[SDL_AudioStream], ctypes.c_float], SDL_BINARY]
SDL_GetAudioStreamInputChannelMap: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAudioStreamInputChannelMap", SDL_POINTER[ctypes.c_int], [SDL_POINTER[SDL_AudioStream], SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_GetAudioStreamOutputChannelMap: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAudioStreamOutputChannelMap", SDL_POINTER[ctypes.c_int], [SDL_POINTER[SDL_AudioStream], SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_SetAudioStreamInputChannelMap: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetAudioStreamInputChannelMap", ctypes.c_bool, [SDL_POINTER[SDL_AudioStream], SDL_POINTER[ctypes.c_int], ctypes.c_int], SDL_BINARY]
SDL_SetAudioStreamOutputChannelMap: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetAudioStreamOutputChannelMap", ctypes.c_bool, [SDL_POINTER[SDL_AudioStream], SDL_POINTER[ctypes.c_int], ctypes.c_int], SDL_BINARY]
SDL_PutAudioStreamData: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_PutAudioStreamData", ctypes.c_bool, [SDL_POINTER[SDL_AudioStream], ctypes.c_void_p, ctypes.c_int], SDL_BINARY]
SDL_GetAudioStreamData: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAudioStreamData", ctypes.c_int, [SDL_POINTER[SDL_AudioStream], ctypes.c_void_p, ctypes.c_int], SDL_BINARY]
SDL_GetAudioStreamAvailable: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAudioStreamAvailable", ctypes.c_int, [SDL_POINTER[SDL_AudioStream]], SDL_BINARY]
SDL_GetAudioStreamQueued: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAudioStreamQueued", ctypes.c_int, [SDL_POINTER[SDL_AudioStream]], SDL_BINARY]
SDL_FlushAudioStream: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_FlushAudioStream", ctypes.c_bool, [SDL_POINTER[SDL_AudioStream]], SDL_BINARY]
SDL_ClearAudioStream: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ClearAudioStream", ctypes.c_bool, [SDL_POINTER[SDL_AudioStream]], SDL_BINARY]
SDL_PauseAudioStreamDevice: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_PauseAudioStreamDevice", ctypes.c_bool, [SDL_POINTER[SDL_AudioStream]], SDL_BINARY]
SDL_ResumeAudioStreamDevice: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ResumeAudioStreamDevice", ctypes.c_bool, [SDL_POINTER[SDL_AudioStream]], SDL_BINARY]
SDL_AudioStreamDevicePaused: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_AudioStreamDevicePaused", ctypes.c_bool, [SDL_POINTER[SDL_AudioStream]], SDL_BINARY]
SDL_LockAudioStream: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LockAudioStream", ctypes.c_bool, [SDL_POINTER[SDL_AudioStream]], SDL_BINARY]
SDL_UnlockAudioStream: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UnlockAudioStream", ctypes.c_bool, [SDL_POINTER[SDL_AudioStream]], SDL_BINARY]

SDL_AudioStreamCallback: typing.TypeAlias = SDL_FUNC_TYPE["SDL_AudioStreamCallback", None, [ctypes.c_void_p, SDL_POINTER[SDL_AudioStream], ctypes.c_int, ctypes.c_int]]

SDL_SetAudioStreamGetCallback: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetAudioStreamGetCallback", ctypes.c_bool, [SDL_POINTER[SDL_AudioStream], SDL_AudioStreamCallback, ctypes.c_void_p], SDL_BINARY]
SDL_SetAudioStreamPutCallback: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetAudioStreamPutCallback", ctypes.c_bool, [SDL_POINTER[SDL_AudioStream], SDL_AudioStreamCallback, ctypes.c_void_p], SDL_BINARY]
SDL_DestroyAudioStream: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DestroyAudioStream", None, [SDL_POINTER[SDL_AudioStream]], SDL_BINARY]
SDL_OpenAudioDeviceStream: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_OpenAudioDeviceStream", SDL_POINTER[SDL_AudioStream], [SDL_AudioDeviceID, SDL_POINTER[SDL_AudioSpec], SDL_AudioStreamCallback, ctypes.c_void_p], SDL_BINARY]

SDL_AudioPostmixCallback: typing.TypeAlias = SDL_FUNC_TYPE["SDL_AudioPostmixCallback", None, [ctypes.c_void_p, SDL_POINTER[SDL_AudioSpec], SDL_POINTER[ctypes.c_float], ctypes.c_int]]

SDL_SetAudioPostmixCallback: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetAudioPostmixCallback", ctypes.c_bool, [SDL_AudioDeviceID, SDL_AudioPostmixCallback, ctypes.c_void_p], SDL_BINARY]

SDL_LoadWAV_IO: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LoadWAV_IO", ctypes.c_bool, [SDL_POINTER[SDL_IOStream], ctypes.c_bool, SDL_POINTER[SDL_AudioSpec], SDL_POINTER[SDL_POINTER[ctypes.c_uint8]], SDL_POINTER[ctypes.c_uint32]], SDL_BINARY]
SDL_LoadWAV: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LoadWAV", ctypes.c_bool, [ctypes.c_char_p, SDL_POINTER[SDL_AudioSpec], SDL_POINTER[SDL_POINTER[ctypes.c_uint8]], SDL_POINTER[ctypes.c_uint32]], SDL_BINARY]
SDL_MixAudio: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_MixAudio", ctypes.c_bool, [SDL_POINTER[ctypes.c_uint8], SDL_POINTER[ctypes.c_uint8], SDL_AudioFormat, ctypes.c_uint32, ctypes.c_float], SDL_BINARY]
SDL_ConvertAudioSamples: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ConvertAudioSamples", ctypes.c_bool, [SDL_POINTER[SDL_AudioSpec], SDL_POINTER[ctypes.c_uint8], ctypes.c_int, SDL_POINTER[SDL_AudioSpec], SDL_POINTER[SDL_POINTER[ctypes.c_uint8]], SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_GetAudioFormatName: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetAudioFormatName", ctypes.c_char_p, [SDL_AudioFormat], SDL_BINARY]
SDL_GetSilenceValueForFormat: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSilenceValueForFormat", ctypes.c_int, [SDL_AudioFormat], SDL_BINARY]