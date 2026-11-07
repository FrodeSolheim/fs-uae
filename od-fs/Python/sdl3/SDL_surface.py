import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_ENUM, SDL_FUNC, SDL_TYPE, SDL_BINARY
from .SDL_pixels import SDL_PixelFormat, SDL_Colorspace, SDL_Palette
from .SDL_blendmode import SDL_BlendMode
from .SDL_properties import SDL_PropertiesID
from .SDL_iostream import SDL_IOStream
from .SDL_rect import SDL_Rect

SDL_SurfaceFlags: typing.TypeAlias = SDL_TYPE["SDL_SurfaceFlags", ctypes.c_uint32]

SDL_SURFACE_PREALLOCATED, SDL_SURFACE_LOCK_NEEDED, SDL_SURFACE_LOCKED, \
    SDL_SURFACE_SIMD_ALIGNED = 0x00000001, 0x00000002, 0x00000004, 0x00000008

SDL_MUSTLOCK: abc.Callable[..., bool] = lambda s: \
    (s.flags & SDL_SURFACE_LOCK_NEEDED) == SDL_SURFACE_LOCK_NEEDED

SDL_ScaleMode: typing.TypeAlias = SDL_TYPE["SDL_ScaleMode", SDL_ENUM]

SDL_SCALEMODE_INVALID, SDL_SCALEMODE_NEAREST, SDL_SCALEMODE_LINEAR = range(-1, 2)

SDL_FlipMode: typing.TypeAlias = SDL_TYPE["SDL_FlipMode", SDL_ENUM]

SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL, SDL_FLIP_VERTICAL = range(3)

class SDL_Surface(ctypes.Structure):
    _fields_ = [
        ("flags", SDL_SurfaceFlags),
        ("format", SDL_PixelFormat),
        ("w", ctypes.c_int),
        ("h", ctypes.c_int),
        ("pitch", ctypes.c_int),
        ("pixels", ctypes.c_void_p),
        ("refcount", ctypes.c_int),
        ("reserved", ctypes.c_void_p)
    ]

SDL_CreateSurface: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateSurface", SDL_POINTER[SDL_Surface], [ctypes.c_int, ctypes.c_int, SDL_PixelFormat], SDL_BINARY]
SDL_CreateSurfaceFrom: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateSurfaceFrom", SDL_POINTER[SDL_Surface], [ctypes.c_int, ctypes.c_int, SDL_PixelFormat, ctypes.c_void_p, ctypes.c_int], SDL_BINARY]
SDL_DestroySurface: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DestroySurface", None, [SDL_POINTER[SDL_Surface]], SDL_BINARY]

SDL_GetSurfaceProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSurfaceProperties", SDL_PropertiesID, [SDL_POINTER[SDL_Surface]], SDL_BINARY]

SDL_PROP_SURFACE_SDR_WHITE_POINT_FLOAT: bytes = "SDL.surface.SDR_white_point".encode()
SDL_PROP_SURFACE_HDR_HEADROOM_FLOAT: bytes = "SDL.surface.HDR_headroom".encode()
SDL_PROP_SURFACE_TONEMAP_OPERATOR_STRING: bytes = "SDL.surface.tonemap".encode()
SDL_PROP_SURFACE_HOTSPOT_X_NUMBER: bytes = "SDL.surface.hotspot.x".encode()
SDL_PROP_SURFACE_HOTSPOT_Y_NUMBER: bytes = "SDL.surface.hotspot.y".encode()

SDL_SetSurfaceColorspace: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetSurfaceColorspace", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_Colorspace], SDL_BINARY]
SDL_GetSurfaceColorspace: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSurfaceColorspace", SDL_Colorspace, [SDL_POINTER[SDL_Surface]], SDL_BINARY]

SDL_CreateSurfacePalette: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateSurfacePalette", SDL_POINTER[SDL_Palette], [SDL_POINTER[SDL_Surface]], SDL_BINARY]
SDL_SetSurfacePalette: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetSurfacePalette", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Palette]], SDL_BINARY]
SDL_GetSurfacePalette: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSurfacePalette", SDL_POINTER[SDL_Palette], [SDL_POINTER[SDL_Surface]], SDL_BINARY]
SDL_AddSurfaceAlternateImage: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_AddSurfaceAlternateImage", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Surface]], SDL_BINARY]
SDL_SurfaceHasAlternateImages: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SurfaceHasAlternateImages", ctypes.c_bool, [SDL_POINTER[SDL_Surface]], SDL_BINARY]
SDL_GetSurfaceImages: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSurfaceImages", SDL_POINTER[SDL_POINTER[SDL_Surface]], [SDL_POINTER[SDL_Surface], SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_RemoveSurfaceAlternateImages: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RemoveSurfaceAlternateImages", None, [SDL_POINTER[SDL_Surface]], SDL_BINARY]
SDL_LockSurface: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LockSurface", ctypes.c_bool, [SDL_POINTER[SDL_Surface]], SDL_BINARY]
SDL_UnlockSurface: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UnlockSurface", None, [SDL_POINTER[SDL_Surface]], SDL_BINARY]

SDL_LoadBMP_IO: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LoadBMP_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream], ctypes.c_bool], SDL_BINARY]
SDL_LoadBMP: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LoadBMP", SDL_POINTER[SDL_Surface], [ctypes.c_char_p], SDL_BINARY]

SDL_SaveBMP_IO: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SaveBMP_IO", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_IOStream], ctypes.c_bool], SDL_BINARY]
SDL_SaveBMP: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SaveBMP", ctypes.c_bool, [SDL_POINTER[SDL_Surface], ctypes.c_char_p], SDL_BINARY]

SDL_SetSurfaceRLE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetSurfaceRLE", ctypes.c_bool, [SDL_POINTER[SDL_Surface], ctypes.c_bool], SDL_BINARY]
SDL_SurfaceHasRLE: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SurfaceHasRLE", ctypes.c_bool, [SDL_POINTER[SDL_Surface]], SDL_BINARY]

SDL_SetSurfaceColorKey: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetSurfaceColorKey", ctypes.c_bool, [SDL_POINTER[SDL_Surface], ctypes.c_bool, ctypes.c_uint32], SDL_BINARY]
SDL_SurfaceHasColorKey: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SurfaceHasColorKey", ctypes.c_bool, [SDL_POINTER[SDL_Surface]], SDL_BINARY]
SDL_GetSurfaceColorKey: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSurfaceColorKey", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[ctypes.c_uint32]], SDL_BINARY]

SDL_SetSurfaceColorMod: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetSurfaceColorMod", ctypes.c_bool, [SDL_POINTER[SDL_Surface], ctypes.c_uint8, ctypes.c_uint8, ctypes.c_uint8], SDL_BINARY]
SDL_GetSurfaceColorMod: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSurfaceColorMod", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[ctypes.c_uint8], SDL_POINTER[ctypes.c_uint8], SDL_POINTER[ctypes.c_uint8]], SDL_BINARY]

SDL_SetSurfaceAlphaMod: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetSurfaceAlphaMod", ctypes.c_bool, [SDL_POINTER[SDL_Surface], ctypes.c_uint8], SDL_BINARY]
SDL_GetSurfaceAlphaMod: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSurfaceAlphaMod", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[ctypes.c_uint8]], SDL_BINARY]

SDL_SetSurfaceBlendMode: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetSurfaceBlendMode", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_BlendMode], SDL_BINARY]
SDL_GetSurfaceBlendMode: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSurfaceBlendMode", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_BlendMode]], SDL_BINARY]

SDL_SetSurfaceClipRect: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetSurfaceClipRect", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Rect]], SDL_BINARY]
SDL_GetSurfaceClipRect: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSurfaceClipRect", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Rect]], SDL_BINARY]

SDL_FlipSurface: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_FlipSurface", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_FlipMode], SDL_BINARY]
SDL_DuplicateSurface: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DuplicateSurface", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_Surface]], SDL_BINARY]
SDL_ScaleSurface: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ScaleSurface", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_Surface], ctypes.c_int, ctypes.c_int, SDL_ScaleMode], SDL_BINARY]

SDL_ConvertSurface: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ConvertSurface", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_Surface], SDL_PixelFormat], SDL_BINARY]
SDL_ConvertSurfaceAndColorspace: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ConvertSurfaceAndColorspace", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_Surface], SDL_PixelFormat, SDL_POINTER[SDL_Palette], SDL_Colorspace, SDL_PropertiesID], SDL_BINARY]
SDL_ConvertPixels: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ConvertPixels", ctypes.c_bool, [ctypes.c_int, ctypes.c_int, SDL_PixelFormat, ctypes.c_void_p, ctypes.c_int, SDL_PixelFormat, ctypes.c_void_p, ctypes.c_int], SDL_BINARY]
SDL_ConvertPixelsAndColorspace: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ConvertPixelsAndColorspace", ctypes.c_bool, [ctypes.c_int, ctypes.c_int, SDL_PixelFormat, SDL_Colorspace, SDL_PropertiesID, ctypes.c_void_p, ctypes.c_int, SDL_PixelFormat, SDL_Colorspace, SDL_PropertiesID, ctypes.c_void_p, ctypes.c_int], SDL_BINARY]

SDL_PremultiplyAlpha: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_PremultiplyAlpha", ctypes.c_bool, [ctypes.c_int, ctypes.c_int, SDL_PixelFormat, ctypes.c_void_p, ctypes.c_int, SDL_PixelFormat, ctypes.c_void_p, ctypes.c_int, ctypes.c_bool], SDL_BINARY]
SDL_PremultiplySurfaceAlpha: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_PremultiplySurfaceAlpha", ctypes.c_bool, [SDL_POINTER[SDL_Surface], ctypes.c_bool], SDL_BINARY]

SDL_ClearSurface: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ClearSurface", ctypes.c_bool, [SDL_POINTER[SDL_Surface], ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float], SDL_BINARY]

SDL_FillSurfaceRect: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_FillSurfaceRect", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Rect], ctypes.c_uint32], SDL_BINARY]
SDL_FillSurfaceRects: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_FillSurfaceRects", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Rect], ctypes.c_int, ctypes.c_uint32], SDL_BINARY]

SDL_BlitSurface: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BlitSurface", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Rect], SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Rect]], SDL_BINARY]
SDL_BlitSurfaceUnchecked: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BlitSurfaceUnchecked", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Rect], SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Rect]], SDL_BINARY]
SDL_BlitSurfaceScaled: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BlitSurfaceScaled", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Rect], SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Rect], SDL_ScaleMode], SDL_BINARY]
SDL_BlitSurfaceUncheckedScaled: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BlitSurfaceUncheckedScaled", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Rect], SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Rect], SDL_ScaleMode], SDL_BINARY]
SDL_StretchSurface: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_StretchSurface", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Rect], SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Rect], SDL_ScaleMode], SDL_BINARY]
SDL_BlitSurfaceTiled: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BlitSurfaceTiled", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Rect], SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Rect]], SDL_BINARY]
SDL_BlitSurfaceTiledWithScale: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BlitSurfaceTiledWithScale", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Rect], ctypes.c_float, SDL_ScaleMode, SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Rect]], SDL_BINARY]
SDL_BlitSurface9Grid: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_BlitSurface9Grid", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Rect], ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_float, SDL_ScaleMode, SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_Rect]], SDL_BINARY]

SDL_MapSurfaceRGB: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_MapSurfaceRGB", ctypes.c_uint32, [SDL_POINTER[SDL_Surface], ctypes.c_uint8, ctypes.c_uint8, ctypes.c_uint8], SDL_BINARY]
SDL_MapSurfaceRGBA: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_MapSurfaceRGBA", ctypes.c_uint32, [SDL_POINTER[SDL_Surface], ctypes.c_uint8, ctypes.c_uint8, ctypes.c_uint8, ctypes.c_uint8], SDL_BINARY]

SDL_ReadSurfacePixel: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReadSurfacePixel", ctypes.c_bool, [SDL_POINTER[SDL_Surface], ctypes.c_int, ctypes.c_int, SDL_POINTER[ctypes.c_uint8], SDL_POINTER[ctypes.c_uint8], SDL_POINTER[ctypes.c_uint8], SDL_POINTER[ctypes.c_uint8]], SDL_BINARY]
SDL_ReadSurfacePixelFloat: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ReadSurfacePixelFloat", ctypes.c_bool, [SDL_POINTER[SDL_Surface], ctypes.c_int, ctypes.c_int, SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float]], SDL_BINARY]

SDL_WriteSurfacePixel: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WriteSurfacePixel", ctypes.c_bool, [SDL_POINTER[SDL_Surface], ctypes.c_int, ctypes.c_int, ctypes.c_uint8, ctypes.c_uint8, ctypes.c_uint8, ctypes.c_uint8], SDL_BINARY]
SDL_WriteSurfacePixelFloat: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WriteSurfacePixelFloat", ctypes.c_bool, [SDL_POINTER[SDL_Surface], ctypes.c_int, ctypes.c_int, ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float], SDL_BINARY]