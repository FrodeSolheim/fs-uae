import ctypes, typing, collections.abc as abc

from . import SDL_PLATFORM_SPECIFIC, SDL_POINTER, \
    SDL_FUNC, SDL_TYPE, SDL_TTF_BINARY, SDL_ENUM
from .SDL_gpu import SDL_GPUDevice, SDL_GPUTexture
from .SDL_properties import SDL_PropertiesID
from .SDL_rect import SDL_Rect, SDL_FPoint
from .SDL_surface import SDL_Surface
from .SDL_version import SDL_VERSIONNUM
from .SDL_iostream import SDL_IOStream
from .SDL_render import SDL_Renderer
from .SDL_pixels import SDL_Color

SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION, SDL_TTF_MICRO_VERSION = 3, 2, 2
SDL_TTF_VERSION: int = SDL_VERSIONNUM(SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION, SDL_TTF_MICRO_VERSION)

SDL_TTF_VERSION_ATLEAST: abc.Callable[[int, int, int], bool] = lambda x, y, z: \
    (SDL_TTF_MAJOR_VERSION >= x) and (SDL_TTF_MAJOR_VERSION > x or SDL_TTF_MINOR_VERSION >= y) and \
        (SDL_TTF_MAJOR_VERSION > x or SDL_TTF_MINOR_VERSION > y or SDL_TTF_MICRO_VERSION >= z)

TTF_Version: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_Version", ctypes.c_int, [], SDL_TTF_BINARY]
TTF_GetFreeTypeVersion: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetFreeTypeVersion", None, [SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int]], SDL_TTF_BINARY]
TTF_GetHarfBuzzVersion: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetHarfBuzzVersion", None, [SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int]], SDL_TTF_BINARY]

class TTF_Font(ctypes.c_void_p):
    ...

TTF_Init: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_Init", ctypes.c_bool, [], SDL_TTF_BINARY]
TTF_OpenFont: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_OpenFont", SDL_POINTER[TTF_Font], [ctypes.c_char_p, ctypes.c_float], SDL_TTF_BINARY]
TTF_OpenFontIO: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_OpenFontIO", SDL_POINTER[TTF_Font], [SDL_POINTER[SDL_IOStream], ctypes.c_bool, ctypes.c_float], SDL_TTF_BINARY]
TTF_OpenFontWithProperties: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_OpenFontWithProperties", SDL_POINTER[TTF_Font], [SDL_PropertiesID], SDL_TTF_BINARY]

TTF_PROP_FONT_CREATE_FILENAME_STRING: bytes = "SDL_ttf.font.create.filename".encode()
TTF_PROP_FONT_CREATE_IOSTREAM_POINTER: bytes = "SDL_ttf.font.create.iostream".encode()
TTF_PROP_FONT_CREATE_IOSTREAM_OFFSET_NUMBER: bytes = "SDL_ttf.font.create.iostream.offset".encode()
TTF_PROP_FONT_CREATE_IOSTREAM_AUTOCLOSE_BOOLEAN: bytes = "SDL_ttf.font.create.iostream.autoclose".encode()
TTF_PROP_FONT_CREATE_SIZE_FLOAT: bytes = "SDL_ttf.font.create.size".encode()
TTF_PROP_FONT_CREATE_FACE_NUMBER: bytes = "SDL_ttf.font.create.face".encode()
TTF_PROP_FONT_CREATE_HORIZONTAL_DPI_NUMBER: bytes = "SDL_ttf.font.create.hdpi".encode()
TTF_PROP_FONT_CREATE_VERTICAL_DPI_NUMBER: bytes = "SDL_ttf.font.create.vdpi".encode()
TTF_PROP_FONT_CREATE_EXISTING_FONT: bytes = "SDL_ttf.font.create.existing_font".encode()

TTF_CopyFont: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_CopyFont", SDL_POINTER[TTF_Font], [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]

TTF_GetFontProperties: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetFontProperties", SDL_PropertiesID, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]

TTF_PROP_FONT_OUTLINE_LINE_CAP_NUMBER: bytes = "SDL_ttf.font.outline.line_cap".encode()
TTF_PROP_FONT_OUTLINE_LINE_JOIN_NUMBER: bytes = "SDL_ttf.font.outline.line_join".encode()
TTF_PROP_FONT_OUTLINE_MITER_LIMIT_NUMBER: bytes = "SDL_ttf.font.outline.miter_limit".encode()

TTF_GetFontGeneration: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetFontGeneration", ctypes.c_uint32, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]

TTF_AddFallbackFont: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_AddFallbackFont", ctypes.c_bool, [SDL_POINTER[TTF_Font], SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]
TTF_RemoveFallbackFont: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_RemoveFallbackFont", None, [SDL_POINTER[TTF_Font], SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]
TTF_ClearFallbackFonts: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_ClearFallbackFonts", None, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]

TTF_SetFontSize: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetFontSize", ctypes.c_bool, [SDL_POINTER[TTF_Font], ctypes.c_float], SDL_TTF_BINARY]
TTF_SetFontSizeDPI: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetFontSizeDPI", ctypes.c_bool, [SDL_POINTER[TTF_Font], ctypes.c_float, ctypes.c_int, ctypes.c_int], SDL_TTF_BINARY]

TTF_GetFontSize: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetFontSize", ctypes.c_float, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]
TTF_GetFontDPI: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetFontDPI", ctypes.c_bool, [SDL_POINTER[TTF_Font], SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int]], SDL_TTF_BINARY]

TTF_FontStyleFlags: typing.TypeAlias = SDL_TYPE["TTF_FontStyleFlags", ctypes.c_uint32]

TTF_STYLE_NORMAL, TTF_STYLE_BOLD, TTF_STYLE_ITALIC, TTF_STYLE_UNDERLINE, \
     TTF_STYLE_STRIKETHROUGH = 0x00, 0x01, 0x02, 0x04, 0x08

TTF_SetFontStyle: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetFontStyle", None, [SDL_POINTER[TTF_Font], TTF_FontStyleFlags], SDL_TTF_BINARY]
TTF_GetFontStyle: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetFontStyle", TTF_FontStyleFlags, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]
TTF_SetFontOutline: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetFontOutline", ctypes.c_bool, [SDL_POINTER[TTF_Font], ctypes.c_int], SDL_TTF_BINARY]
TTF_GetFontOutline: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetFontOutline", ctypes.c_int, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]

TTF_HintingFlags: typing.TypeAlias = SDL_TYPE["TTF_HintingFlags", SDL_ENUM]

TTF_HINTING_INVALID, TTF_HINTING_NORMAL, TTF_HINTING_LIGHT, TTF_HINTING_MONO, TTF_HINTING_NONE, TTF_HINTING_LIGHT_SUBPIXEL = range(-1, 5)

TTF_SetFontHinting: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetFontHinting", None, [SDL_POINTER[TTF_Font], TTF_HintingFlags], SDL_TTF_BINARY]
TTF_GetNumFontFaces: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetNumFontFaces", ctypes.c_int, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]
TTF_GetFontHinting: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetFontHinting", TTF_HintingFlags, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]

TTF_SetFontSDF: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetFontSDF", ctypes.c_bool, [SDL_POINTER[TTF_Font], ctypes.c_bool], SDL_TTF_BINARY]
TTF_GetFontSDF: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetFontSDF", ctypes.c_bool, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]

TTF_GetFontWeight: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetFontWeight", ctypes.c_int, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]

TTF_FONT_WEIGHT_THIN, TTF_FONT_WEIGHT_EXTRA_LIGHT, TTF_FONT_WEIGHT_LIGHT, TTF_FONT_WEIGHT_NORMAL, TTF_FONT_WEIGHT_MEDIUM, TTF_FONT_WEIGHT_SEMI_BOLD, \
    TTF_FONT_WEIGHT_BOLD, TTF_FONT_WEIGHT_EXTRA_BOLD, TTF_FONT_WEIGHT_BLACK, TTF_FONT_WEIGHT_EXTRA_BLACK = *range(100, 1000, 100), 950

TTF_HorizontalAlignment: typing.TypeAlias = SDL_TYPE["TTF_HorizontalAlignment", SDL_ENUM]

TTF_HORIZONTAL_ALIGN_INVALID, TTF_HORIZONTAL_ALIGN_LEFT, TTF_HORIZONTAL_ALIGN_CENTER, TTF_HORIZONTAL_ALIGN_RIGHT = range(-1, 3)

TTF_SetFontWrapAlignment: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetFontWrapAlignment", None, [SDL_POINTER[TTF_Font], TTF_HorizontalAlignment], SDL_TTF_BINARY]
TTF_GetFontWrapAlignment: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetFontWrapAlignment", TTF_HorizontalAlignment, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]

TTF_GetFontHeight: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetFontHeight", ctypes.c_int, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]
TTF_GetFontAscent: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetFontAscent", ctypes.c_int, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]
TTF_GetFontDescent: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetFontDescent", ctypes.c_int, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]

TTF_SetFontLineSkip: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetFontLineSkip", None, [SDL_POINTER[TTF_Font], ctypes.c_int], SDL_TTF_BINARY]
TTF_GetFontLineSkip: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetFontLineSkip", ctypes.c_int, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]

TTF_SetFontKerning: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetFontKerning", None, [SDL_POINTER[TTF_Font], ctypes.c_bool], SDL_TTF_BINARY]
TTF_GetFontKerning: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetFontKerning", ctypes.c_bool, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]

TTF_FontIsFixedWidth: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_FontIsFixedWidth", ctypes.c_bool, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]
TTF_FontIsScalable: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_FontIsScalable", ctypes.c_bool, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]

TTF_GetFontFamilyName: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetFontFamilyName", ctypes.c_char_p, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]
TTF_GetFontStyleName: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetFontStyleName", ctypes.c_char_p, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]

TTF_Direction: typing.TypeAlias = SDL_TYPE["TTF_Direction", SDL_ENUM]

TTF_DIRECTION_INVALID, (TTF_DIRECTION_LTR, TTF_DIRECTION_RTL, TTF_DIRECTION_TTB, TTF_DIRECTION_BTT) = 0, range(4, 8)

TTF_SetFontDirection: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetFontDirection", ctypes.c_bool, [SDL_POINTER[TTF_Font], TTF_Direction], SDL_TTF_BINARY]
TTF_GetFontDirection: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetFontDirection", TTF_Direction, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]

TTF_StringToTag: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_StringToTag", ctypes.c_uint32, [ctypes.c_char_p], SDL_TTF_BINARY]
TTF_TagToString: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_TagToString", None, [ctypes.c_uint32, ctypes.c_char_p, ctypes.c_size_t], SDL_TTF_BINARY]

TTF_SetFontScript: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetFontScript", ctypes.c_bool, [SDL_POINTER[TTF_Font], ctypes.c_uint32], SDL_TTF_BINARY]
TTF_GetFontScript: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetFontScript", ctypes.c_uint32, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]

TTF_GetGlyphScript: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetGlyphScript", ctypes.c_uint32, [ctypes.c_uint32], SDL_TTF_BINARY]

TTF_SetFontLanguage: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetFontLanguage", ctypes.c_bool, [SDL_POINTER[TTF_Font], ctypes.c_char_p], SDL_TTF_BINARY]
TTF_FontHasGlyph: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_FontHasGlyph", ctypes.c_bool, [SDL_POINTER[TTF_Font], ctypes.c_uint32], SDL_TTF_BINARY]

TTF_ImageType: typing.TypeAlias = SDL_TYPE["TTF_ImageType", SDL_ENUM]

TTF_IMAGE_INVALID, TTF_IMAGE_ALPHA, TTF_IMAGE_COLOR, TTF_IMAGE_SDF = range(4)

TTF_GetGlyphImage: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetGlyphImage", SDL_POINTER[SDL_Surface], [SDL_POINTER[TTF_Font], ctypes.c_uint32, SDL_POINTER[TTF_ImageType]], SDL_TTF_BINARY]
TTF_GetGlyphImageForIndex: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetGlyphImageForIndex", SDL_POINTER[SDL_Surface], [SDL_POINTER[TTF_Font], ctypes.c_uint32, SDL_POINTER[TTF_ImageType]], SDL_TTF_BINARY]
TTF_GetGlyphMetrics: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetGlyphMetrics", ctypes.c_bool, [SDL_POINTER[TTF_Font], ctypes.c_uint32, SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int]], SDL_TTF_BINARY]
TTF_GetGlyphKerning: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetGlyphKerning", ctypes.c_bool, [SDL_POINTER[TTF_Font], ctypes.c_uint32, ctypes.c_uint32, SDL_POINTER[ctypes.c_int]], SDL_TTF_BINARY]

TTF_GetStringSize: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetStringSize", ctypes.c_bool, [SDL_POINTER[TTF_Font], ctypes.c_char_p, ctypes.c_size_t, SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int]], SDL_TTF_BINARY]
TTF_GetStringSizeWrapped: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetStringSizeWrapped", ctypes.c_bool, [SDL_POINTER[TTF_Font], ctypes.c_char_p, ctypes.c_size_t, ctypes.c_int, SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int]], SDL_TTF_BINARY]

TTF_MeasureString: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_MeasureString", ctypes.c_bool, [SDL_POINTER[TTF_Font], ctypes.c_char_p, ctypes.c_size_t, ctypes.c_int, SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_size_t]], SDL_TTF_BINARY]

TTF_RenderText_Solid: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_RenderText_Solid", SDL_POINTER[SDL_Surface], [SDL_POINTER[TTF_Font], ctypes.c_char_p, ctypes.c_size_t, SDL_Color], SDL_TTF_BINARY]
TTF_RenderText_Solid_Wrapped: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_RenderText_Solid_Wrapped", SDL_POINTER[SDL_Surface], [SDL_POINTER[TTF_Font], ctypes.c_char_p, ctypes.c_size_t, SDL_Color, ctypes.c_int], SDL_TTF_BINARY]
TTF_RenderGlyph_Solid: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_RenderGlyph_Solid", SDL_POINTER[SDL_Surface], [SDL_POINTER[TTF_Font], ctypes.c_uint32, SDL_Color], SDL_TTF_BINARY]

TTF_RenderText_Shaded: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_RenderText_Shaded", SDL_POINTER[SDL_Surface], [SDL_POINTER[TTF_Font], ctypes.c_char_p, ctypes.c_size_t, SDL_Color, SDL_Color], SDL_TTF_BINARY]
TTF_RenderText_Shaded_Wrapped: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_RenderText_Shaded_Wrapped", SDL_POINTER[SDL_Surface], [SDL_POINTER[TTF_Font], ctypes.c_char_p, ctypes.c_size_t, SDL_Color, SDL_Color, ctypes.c_int], SDL_TTF_BINARY]
TTF_RenderGlyph_Shaded: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_RenderGlyph_Shaded", SDL_POINTER[SDL_Surface], [SDL_POINTER[TTF_Font], ctypes.c_uint32, SDL_Color, SDL_Color], SDL_TTF_BINARY]

TTF_RenderText_Blended: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_RenderText_Blended", SDL_POINTER[SDL_Surface], [SDL_POINTER[TTF_Font], ctypes.c_char_p, ctypes.c_size_t, SDL_Color], SDL_TTF_BINARY]
TTF_RenderText_Blended_Wrapped: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_RenderText_Blended_Wrapped", SDL_POINTER[SDL_Surface], [SDL_POINTER[TTF_Font], ctypes.c_char_p, ctypes.c_size_t, SDL_Color, ctypes.c_int], SDL_TTF_BINARY]
TTF_RenderGlyph_Blended: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_RenderGlyph_Blended", SDL_POINTER[SDL_Surface], [SDL_POINTER[TTF_Font], ctypes.c_uint32, SDL_Color], SDL_TTF_BINARY]

TTF_RenderText_LCD: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_RenderText_LCD", SDL_POINTER[SDL_Surface], [SDL_POINTER[TTF_Font], ctypes.c_char_p, ctypes.c_size_t, SDL_Color, SDL_Color], SDL_TTF_BINARY]
TTF_RenderText_LCD_Wrapped: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_RenderText_LCD_Wrapped", SDL_POINTER[SDL_Surface], [SDL_POINTER[TTF_Font], ctypes.c_char_p, ctypes.c_size_t, SDL_Color, SDL_Color, ctypes.c_int], SDL_TTF_BINARY]
TTF_RenderGlyph_LCD: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_RenderGlyph_LCD", SDL_POINTER[SDL_Surface], [SDL_POINTER[TTF_Font], ctypes.c_uint32, SDL_Color, SDL_Color], SDL_TTF_BINARY]

class TTF_TextEngine(ctypes.c_void_p):
    ...

class TTF_TextData(ctypes.c_void_p):
    ...

class TTF_Text(ctypes.Structure):
    _fields_ = [
        ("text", ctypes.c_char_p),
        ("num_lines", ctypes.c_int),
        ("refcount", ctypes.c_int),
        ("internal", SDL_POINTER[TTF_TextData])
    ]

TTF_CreateSurfaceTextEngine: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_CreateSurfaceTextEngine", SDL_POINTER[TTF_TextEngine], [], SDL_TTF_BINARY]
TTF_DrawSurfaceText: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_DrawSurfaceText", ctypes.c_bool, [SDL_POINTER[TTF_Text], ctypes.c_int, ctypes.c_int, SDL_POINTER[SDL_Surface]], SDL_TTF_BINARY]
TTF_DestroySurfaceTextEngine: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_DestroySurfaceTextEngine", None, [SDL_POINTER[TTF_TextEngine]], SDL_TTF_BINARY]

TTF_CreateRendererTextEngine: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_CreateRendererTextEngine", SDL_POINTER[TTF_TextEngine], [SDL_POINTER[SDL_Renderer]], SDL_TTF_BINARY]
TTF_CreateRendererTextEngineWithProperties: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_CreateRendererTextEngineWithProperties", SDL_POINTER[TTF_TextEngine], [SDL_PropertiesID], SDL_TTF_BINARY]

TTF_PROP_RENDERER_TEXT_ENGINE_RENDERER: bytes = "SDL_ttf.renderer_text_engine.create.renderer".encode()
TTF_PROP_RENDERER_TEXT_ENGINE_ATLAS_TEXTURE_SIZE: bytes = "SDL_ttf.renderer_text_engine.create.atlas_texture_size".encode()

TTF_DrawRendererText: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_DrawRendererText", ctypes.c_bool, [SDL_POINTER[TTF_Text], ctypes.c_float, ctypes.c_float], SDL_TTF_BINARY]
TTF_DestroyRendererTextEngine: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_DestroyRendererTextEngine", None, [SDL_POINTER[TTF_TextEngine]], SDL_TTF_BINARY]

TTF_CreateGPUTextEngine: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_CreateGPUTextEngine", SDL_POINTER[TTF_TextEngine], [SDL_POINTER[SDL_GPUDevice]], SDL_TTF_BINARY]
TTF_CreateGPUTextEngineWithProperties: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_CreateGPUTextEngineWithProperties", SDL_POINTER[TTF_TextEngine], [SDL_PropertiesID], SDL_TTF_BINARY]

TTF_PROP_GPU_TEXT_ENGINE_DEVICE: bytes = "SDL_ttf.gpu_text_engine.create.device".encode()
TTF_PROP_GPU_TEXT_ENGINE_ATLAS_TEXTURE_SIZE: bytes = "SDL_ttf.gpu_text_engine.create.atlas_texture_size".encode()

class TTF_GPUAtlasDrawSequence(ctypes.Structure):
    _fields_ = [
        ("atlas_texture", SDL_POINTER[SDL_GPUTexture]),
        ("xy", SDL_POINTER[SDL_FPoint]),
        ("uv", SDL_POINTER[SDL_FPoint]),
        ("num_vertices", ctypes.c_int),
        ("indices", SDL_POINTER[ctypes.c_int]),
        ("num_indices", ctypes.c_int),
        ("image_type", TTF_ImageType),
        ("next", SDL_POINTER[ctypes.c_void_p])
    ]

TTF_GetGPUTextDrawData: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetGPUTextDrawData", SDL_POINTER[TTF_GPUAtlasDrawSequence], [SDL_POINTER[TTF_Text]], SDL_TTF_BINARY]
TTF_DestroyGPUTextEngine: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_DestroyGPUTextEngine", None, [SDL_POINTER[TTF_TextEngine]], SDL_TTF_BINARY]

TTF_GPUTextEngineWinding: typing.TypeAlias = SDL_TYPE["TTF_GPUTextEngineWinding", SDL_ENUM]

TTF_GPU_TEXTENGINE_WINDING_INVALID, TTF_GPU_TEXTENGINE_WINDING_CLOCKWISE, TTF_GPU_TEXTENGINE_WINDING_COUNTER_CLOCKWISE = range(-1, 2)

TTF_SetGPUTextEngineWinding: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetGPUTextEngineWinding", None, [SDL_POINTER[TTF_TextEngine], TTF_GPUTextEngineWinding], SDL_TTF_BINARY]
TTF_GetGPUTextEngineWinding: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetGPUTextEngineWinding", TTF_GPUTextEngineWinding, [SDL_POINTER[TTF_TextEngine]], SDL_TTF_BINARY]

TTF_CreateText: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_CreateText", SDL_POINTER[TTF_Text], [SDL_POINTER[TTF_TextEngine], SDL_POINTER[TTF_Font], ctypes.c_char_p, ctypes.c_size_t], SDL_TTF_BINARY]

if SDL_PLATFORM_SPECIFIC(system = ["Windows"]):
    TTF_GetTextProperties: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetTextProperties", SDL_PropertiesID, [SDL_POINTER[TTF_Text]], SDL_TTF_BINARY]

TTF_SetTextEngine: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetTextEngine", ctypes.c_bool, [SDL_POINTER[TTF_Text], SDL_POINTER[TTF_TextEngine]], SDL_TTF_BINARY]
TTF_GetTextEngine: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetTextEngine", SDL_POINTER[TTF_TextEngine], [SDL_POINTER[TTF_Text]], SDL_TTF_BINARY]

TTF_SetTextFont: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetTextFont", ctypes.c_bool, [SDL_POINTER[TTF_Text], SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]
TTF_GetTextFont: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetTextFont", SDL_POINTER[TTF_Font], [SDL_POINTER[TTF_Text]], SDL_TTF_BINARY]

TTF_SetTextDirection: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetTextDirection", ctypes.c_bool, [SDL_POINTER[TTF_Text], TTF_Direction], SDL_TTF_BINARY]
TTF_GetTextDirection: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetTextDirection", TTF_Direction, [SDL_POINTER[TTF_Text]], SDL_TTF_BINARY]

TTF_SetTextScript: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetTextScript", ctypes.c_bool, [SDL_POINTER[TTF_Text], ctypes.c_uint32], SDL_TTF_BINARY]
TTF_GetTextScript: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetTextScript", ctypes.c_uint32, [SDL_POINTER[TTF_Text]], SDL_TTF_BINARY]

TTF_SetTextColor: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetTextColor", ctypes.c_bool, [SDL_POINTER[TTF_Text], ctypes.c_uint8, ctypes.c_uint8, ctypes.c_uint8, ctypes.c_uint8], SDL_TTF_BINARY]
TTF_SetTextColorFloat: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetTextColorFloat", ctypes.c_bool, [SDL_POINTER[TTF_Text], ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float], SDL_TTF_BINARY]

TTF_GetTextColor: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetTextColor", ctypes.c_bool, [SDL_POINTER[TTF_Text], SDL_POINTER[ctypes.c_uint8], SDL_POINTER[ctypes.c_uint8], SDL_POINTER[ctypes.c_uint8], SDL_POINTER[ctypes.c_uint8]], SDL_TTF_BINARY]
TTF_GetTextColorFloat: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetTextColorFloat", ctypes.c_bool, [SDL_POINTER[TTF_Text], SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float]], SDL_TTF_BINARY]

TTF_SetTextPosition: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetTextPosition", ctypes.c_bool, [SDL_POINTER[TTF_Text], ctypes.c_int, ctypes.c_int], SDL_TTF_BINARY]
TTF_GetTextPosition: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetTextPosition", ctypes.c_bool, [SDL_POINTER[TTF_Text], SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int]], SDL_TTF_BINARY]

TTF_SetTextWrapWidth: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetTextWrapWidth", ctypes.c_bool, [SDL_POINTER[TTF_Text], ctypes.c_int], SDL_TTF_BINARY]
TTF_GetTextWrapWidth: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetTextWrapWidth", ctypes.c_bool, [SDL_POINTER[TTF_Text], SDL_POINTER[ctypes.c_int]], SDL_TTF_BINARY]

TTF_SetTextWrapWhitespaceVisible: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetTextWrapWhitespaceVisible", ctypes.c_bool, [SDL_POINTER[TTF_Text], ctypes.c_bool], SDL_TTF_BINARY]
TTF_TextWrapWhitespaceVisible: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_TextWrapWhitespaceVisible", ctypes.c_bool, [SDL_POINTER[TTF_Text]], SDL_TTF_BINARY]

TTF_SetTextString: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_SetTextString", ctypes.c_bool, [SDL_POINTER[TTF_Text], ctypes.c_char_p, ctypes.c_size_t], SDL_TTF_BINARY]
TTF_InsertTextString: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_InsertTextString", ctypes.c_bool, [SDL_POINTER[TTF_Text], ctypes.c_int, ctypes.c_char_p, ctypes.c_size_t], SDL_TTF_BINARY]
TTF_AppendTextString: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_AppendTextString", ctypes.c_bool, [SDL_POINTER[TTF_Text], ctypes.c_char_p, ctypes.c_size_t], SDL_TTF_BINARY]
TTF_DeleteTextString: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_DeleteTextString", ctypes.c_bool, [SDL_POINTER[TTF_Text], ctypes.c_int, ctypes.c_int], SDL_TTF_BINARY]

TTF_GetTextSize: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetTextSize", ctypes.c_bool, [SDL_POINTER[TTF_Text], SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int]], SDL_TTF_BINARY]

TTF_SubStringFlags: typing.TypeAlias = SDL_TYPE["TTF_SubStringFlags", ctypes.c_uint32]

TTF_SUBSTRING_DIRECTION_MASK, TTF_SUBSTRING_TEXT_START, TTF_SUBSTRING_LINE_START, \
    TTF_SUBSTRING_LINE_END, TTF_SUBSTRING_TEXT_END = 0x000000FF, 0x00000100, 0x00000200, 0x00000400, 0x00000800

class TTF_SubString(ctypes.Structure):
    _fields_ = [
        ("flags", TTF_SubStringFlags),
        ("offset", ctypes.c_int),
        ("length", ctypes.c_int),
        ("line_index", ctypes.c_int),
        ("cluster_index", ctypes.c_int),
        ("rect", SDL_Rect)
    ]

TTF_GetTextSubString: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetTextSubString", ctypes.c_bool, [SDL_POINTER[TTF_Text], ctypes.c_int, SDL_POINTER[TTF_SubString]], SDL_TTF_BINARY]
TTF_GetTextSubStringForLine: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetTextSubStringForLine", ctypes.c_bool, [SDL_POINTER[TTF_Text], ctypes.c_int, SDL_POINTER[TTF_SubString]], SDL_TTF_BINARY]
TTF_GetTextSubStringsForRange: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetTextSubStringsForRange", SDL_POINTER[SDL_POINTER[TTF_SubString]], [SDL_POINTER[TTF_Text], ctypes.c_int, ctypes.c_int, SDL_POINTER[ctypes.c_int]], SDL_TTF_BINARY]
TTF_GetTextSubStringForPoint: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetTextSubStringForPoint", ctypes.c_bool, [SDL_POINTER[TTF_Text], ctypes.c_int, ctypes.c_int, SDL_POINTER[TTF_SubString]], SDL_TTF_BINARY]
TTF_GetPreviousTextSubString: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetPreviousTextSubString", ctypes.c_bool, [SDL_POINTER[TTF_Text], SDL_POINTER[TTF_SubString], SDL_POINTER[TTF_SubString]], SDL_TTF_BINARY]
TTF_GetNextTextSubString: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_GetNextTextSubString", ctypes.c_bool, [SDL_POINTER[TTF_Text], SDL_POINTER[TTF_SubString], SDL_POINTER[TTF_SubString]], SDL_TTF_BINARY]
TTF_UpdateText: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_UpdateText", ctypes.c_bool, [SDL_POINTER[TTF_Text]], SDL_TTF_BINARY]
TTF_DestroyText: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_DestroyText", None, [SDL_POINTER[TTF_Text]], SDL_TTF_BINARY]

TTF_CloseFont: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_CloseFont", None, [SDL_POINTER[TTF_Font]], SDL_TTF_BINARY]
TTF_Quit: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_Quit", None, [], SDL_TTF_BINARY]
TTF_WasInit: abc.Callable[..., typing.Any] = SDL_FUNC["TTF_WasInit", ctypes.c_int, [], SDL_TTF_BINARY]