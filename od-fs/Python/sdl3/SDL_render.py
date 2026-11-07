import ctypes, typing, collections.abc as abc

from . import  SDL_POINTER, SDL_ENUM, SDL_FUNC, SDL_TYPE, SDL_BINARY
from .SDL_rect import SDL_FPoint, SDL_Rect, SDL_FRect
from .SDL_pixels import SDL_FColor, SDL_PixelFormat
from .SDL_surface import SDL_Surface, SDL_ScaleMode, SDL_FlipMode
from .SDL_video import SDL_Window, SDL_WindowFlags
from .SDL_properties import SDL_PropertiesID
from .SDL_blendmode import SDL_BlendMode
from .SDL_events import SDL_Event

SDL_SOFTWARE_RENDERER: bytes = "software".encode()

class SDL_Vertex(ctypes.Structure):
    _fields_ = [
        ("position", SDL_FPoint),
        ("color", SDL_FColor),
        ("tex_coord", SDL_FPoint)
    ]

SDL_TextureAccess: typing.TypeAlias = SDL_TYPE["SDL_TextureAccess", SDL_ENUM]

SDL_TEXTUREACCESS_STATIC, SDL_TEXTUREACCESS_STREAMING, SDL_TEXTUREACCESS_TARGET = range(3)

SDL_RendererLogicalPresentation: typing.TypeAlias = SDL_TYPE["SDL_RendererLogicalPresentation", SDL_ENUM]

SDL_LOGICAL_PRESENTATION_DISABLED, SDL_LOGICAL_PRESENTATION_STRETCH, SDL_LOGICAL_PRESENTATION_LETTERBOX, \
    SDL_LOGICAL_PRESENTATION_OVERSCAN, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE = range(5)

class SDL_Renderer(ctypes.c_void_p):
    ...

class SDL_Texture(ctypes.Structure):
    _fields_ = [
        ("format", SDL_PixelFormat),
        ("w", ctypes.c_int),
        ("h", ctypes.c_int),
        ("refcount", ctypes.c_int)
    ]

SDL_GetNumRenderDrivers: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetNumRenderDrivers", ctypes.c_int, [], SDL_BINARY]
SDL_GetRenderDriver: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRenderDriver", ctypes.c_char_p, [ctypes.c_int], SDL_BINARY]

SDL_CreateWindowAndRenderer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateWindowAndRenderer", ctypes.c_bool, [ctypes.c_char_p, ctypes.c_int, ctypes.c_int, SDL_WindowFlags, SDL_POINTER[SDL_POINTER[SDL_Window]], SDL_POINTER[SDL_POINTER[SDL_Renderer]]], SDL_BINARY]
SDL_CreateRenderer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateRenderer", SDL_POINTER[SDL_Renderer], [SDL_POINTER[SDL_Window], ctypes.c_char_p], SDL_BINARY]
SDL_CreateRendererWithProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateRendererWithProperties", SDL_POINTER[SDL_Renderer], [SDL_PropertiesID], SDL_BINARY]

SDL_PROP_RENDERER_CREATE_NAME_STRING: bytes = "SDL.renderer.create.name".encode()
SDL_PROP_RENDERER_CREATE_WINDOW_POINTER: bytes = "SDL.renderer.create.window".encode()
SDL_PROP_RENDERER_CREATE_SURFACE_POINTER: bytes = "SDL.renderer.create.surface".encode()
SDL_PROP_RENDERER_CREATE_OUTPUT_COLORSPACE_NUMBER: bytes = "SDL.renderer.create.output_colorspace".encode()
SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER: bytes = "SDL.renderer.create.present_vsync".encode()
SDL_PROP_RENDERER_CREATE_VULKAN_INSTANCE_POINTER: bytes = "SDL.renderer.create.vulkan.instance".encode()
SDL_PROP_RENDERER_CREATE_VULKAN_SURFACE_NUMBER: bytes = "SDL.renderer.create.vulkan.surface".encode()
SDL_PROP_RENDERER_CREATE_VULKAN_PHYSICAL_DEVICE_POINTER: bytes = "SDL.renderer.create.vulkan.physical_device".encode()
SDL_PROP_RENDERER_CREATE_VULKAN_DEVICE_POINTER: bytes = "SDL.renderer.create.vulkan.device".encode()
SDL_PROP_RENDERER_CREATE_VULKAN_GRAPHICS_QUEUE_FAMILY_INDEX_NUMBER: bytes = "SDL.renderer.create.vulkan.graphics_queue_family_index".encode()
SDL_PROP_RENDERER_CREATE_VULKAN_PRESENT_QUEUE_FAMILY_INDEX_NUMBER: bytes = "SDL.renderer.create.vulkan.present_queue_family_index".encode()

SDL_CreateSoftwareRenderer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateSoftwareRenderer", SDL_POINTER[SDL_Renderer], [SDL_POINTER[SDL_Surface]], SDL_BINARY]
SDL_GetRenderer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRenderer", SDL_POINTER[SDL_Renderer], [SDL_POINTER[SDL_Window]], SDL_BINARY]
SDL_GetRenderWindow: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRenderWindow", SDL_POINTER[SDL_Window], [SDL_POINTER[SDL_Renderer]], SDL_BINARY]
SDL_GetRendererName: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRendererName", ctypes.c_char_p, [SDL_POINTER[SDL_Renderer]], SDL_BINARY]

SDL_GetRendererProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRendererProperties", SDL_PropertiesID, [SDL_POINTER[SDL_Renderer]], SDL_BINARY]

SDL_PROP_RENDERER_NAME_STRING: bytes = "SDL.renderer.name".encode()
SDL_PROP_RENDERER_WINDOW_POINTER: bytes = "SDL.renderer.window".encode()
SDL_PROP_RENDERER_SURFACE_POINTER: bytes = "SDL.renderer.surface".encode()
SDL_PROP_RENDERER_VSYNC_NUMBER: bytes = "SDL.renderer.vsync".encode()
SDL_PROP_RENDERER_MAX_TEXTURE_SIZE_NUMBER: bytes = "SDL.renderer.max_texture_size".encode()
SDL_PROP_RENDERER_TEXTURE_FORMATS_POINTER: bytes = "SDL.renderer.texture_formats".encode()
SDL_PROP_RENDERER_OUTPUT_COLORSPACE_NUMBER: bytes = "SDL.renderer.output_colorspace".encode()
SDL_PROP_RENDERER_HDR_ENABLED_BOOLEAN: bytes = "SDL.renderer.HDR_enabled".encode()
SDL_PROP_RENDERER_SDR_WHITE_POINT_FLOAT: bytes = "SDL.renderer.SDR_white_point".encode()
SDL_PROP_RENDERER_HDR_HEADROOM_FLOAT: bytes = "SDL.renderer.HDR_headroom".encode()
SDL_PROP_RENDERER_D3D9_DEVICE_POINTER: bytes = "SDL.renderer.d3d9.device".encode()
SDL_PROP_RENDERER_D3D11_DEVICE_POINTER: bytes = "SDL.renderer.d3d11.device".encode()
SDL_PROP_RENDERER_D3D11_SWAPCHAIN_POINTER: bytes = "SDL.renderer.d3d11.swap_chain".encode()
SDL_PROP_RENDERER_D3D12_DEVICE_POINTER: bytes = "SDL.renderer.d3d12.device".encode()
SDL_PROP_RENDERER_D3D12_SWAPCHAIN_POINTER: bytes = "SDL.renderer.d3d12.swap_chain".encode()
SDL_PROP_RENDERER_D3D12_COMMAND_QUEUE_POINTER: bytes = "SDL.renderer.d3d12.command_queue".encode()
SDL_PROP_RENDERER_VULKAN_INSTANCE_POINTER: bytes = "SDL.renderer.vulkan.instance".encode()
SDL_PROP_RENDERER_VULKAN_SURFACE_NUMBER: bytes = "SDL.renderer.vulkan.surface".encode()
SDL_PROP_RENDERER_VULKAN_PHYSICAL_DEVICE_POINTER: bytes = "SDL.renderer.vulkan.physical_device".encode()
SDL_PROP_RENDERER_VULKAN_DEVICE_POINTER: bytes = "SDL.renderer.vulkan.device".encode()
SDL_PROP_RENDERER_VULKAN_GRAPHICS_QUEUE_FAMILY_INDEX_NUMBER: bytes = "SDL.renderer.vulkan.graphics_queue_family_index".encode()
SDL_PROP_RENDERER_VULKAN_PRESENT_QUEUE_FAMILY_INDEX_NUMBER: bytes = "SDL.renderer.vulkan.present_queue_family_index".encode()
SDL_PROP_RENDERER_VULKAN_SWAPCHAIN_IMAGE_COUNT_NUMBER: bytes = "SDL.renderer.vulkan.swapchain_image_count".encode()
SDL_PROP_RENDERER_GPU_DEVICE_POINTER: bytes = "SDL.renderer.gpu.device".encode()

SDL_GetRenderOutputSize: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRenderOutputSize", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_GetCurrentRenderOutputSize: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetCurrentRenderOutputSize", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int]], SDL_BINARY]

SDL_CreateTexture: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateTexture", SDL_POINTER[SDL_Texture], [SDL_POINTER[SDL_Renderer], SDL_PixelFormat, SDL_TextureAccess, ctypes.c_int, ctypes.c_int], SDL_BINARY]
SDL_CreateTextureFromSurface: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateTextureFromSurface", SDL_POINTER[SDL_Texture], [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_Surface]], SDL_BINARY]
SDL_CreateTextureWithProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateTextureWithProperties", SDL_POINTER[SDL_Texture], [SDL_POINTER[SDL_Renderer], SDL_PropertiesID], SDL_BINARY]

SDL_PROP_TEXTURE_CREATE_COLORSPACE_NUMBER: bytes = "SDL.texture.create.colorspace".encode()
SDL_PROP_TEXTURE_CREATE_FORMAT_NUMBER: bytes = "SDL.texture.create.format".encode()
SDL_PROP_TEXTURE_CREATE_ACCESS_NUMBER: bytes = "SDL.texture.create.access".encode()
SDL_PROP_TEXTURE_CREATE_WIDTH_NUMBER: bytes = "SDL.texture.create.width".encode()
SDL_PROP_TEXTURE_CREATE_HEIGHT_NUMBER: bytes = "SDL.texture.create.height".encode()
SDL_PROP_TEXTURE_CREATE_SDR_WHITE_POINT_FLOAT: bytes = "SDL.texture.create.SDR_white_point".encode()
SDL_PROP_TEXTURE_CREATE_HDR_HEADROOM_FLOAT: bytes = "SDL.texture.create.HDR_headroom".encode()
SDL_PROP_TEXTURE_CREATE_D3D11_TEXTURE_POINTER: bytes = "SDL.texture.create.d3d11.texture".encode()
SDL_PROP_TEXTURE_CREATE_D3D11_TEXTURE_U_POINTER: bytes = "SDL.texture.create.d3d11.texture_u".encode()
SDL_PROP_TEXTURE_CREATE_D3D11_TEXTURE_V_POINTER: bytes = "SDL.texture.create.d3d11.texture_v".encode()
SDL_PROP_TEXTURE_CREATE_D3D12_TEXTURE_POINTER: bytes = "SDL.texture.create.d3d12.texture".encode()
SDL_PROP_TEXTURE_CREATE_D3D12_TEXTURE_U_POINTER: bytes = "SDL.texture.create.d3d12.texture_u".encode()
SDL_PROP_TEXTURE_CREATE_D3D12_TEXTURE_V_POINTER: bytes = "SDL.texture.create.d3d12.texture_v".encode()
SDL_PROP_TEXTURE_CREATE_METAL_PIXELBUFFER_POINTER: bytes = "SDL.texture.create.metal.pixelbuffer".encode()
SDL_PROP_TEXTURE_CREATE_OPENGL_TEXTURE_NUMBER: bytes = "SDL.texture.create.opengl.texture".encode()
SDL_PROP_TEXTURE_CREATE_OPENGL_TEXTURE_UV_NUMBER: bytes = "SDL.texture.create.opengl.texture_uv".encode()
SDL_PROP_TEXTURE_CREATE_OPENGL_TEXTURE_U_NUMBER: bytes = "SDL.texture.create.opengl.texture_u".encode()
SDL_PROP_TEXTURE_CREATE_OPENGL_TEXTURE_V_NUMBER: bytes = "SDL.texture.create.opengl.texture_v".encode()
SDL_PROP_TEXTURE_CREATE_OPENGLES2_TEXTURE_NUMBER: bytes = "SDL.texture.create.opengles2.texture".encode()
SDL_PROP_TEXTURE_CREATE_OPENGLES2_TEXTURE_UV_NUMBER: bytes = "SDL.texture.create.opengles2.texture_uv".encode()
SDL_PROP_TEXTURE_CREATE_OPENGLES2_TEXTURE_U_NUMBER: bytes = "SDL.texture.create.opengles2.texture_u".encode()
SDL_PROP_TEXTURE_CREATE_OPENGLES2_TEXTURE_V_NUMBER: bytes = "SDL.texture.create.opengles2.texture_v".encode()
SDL_PROP_TEXTURE_CREATE_VULKAN_TEXTURE_NUMBER: bytes = "SDL.texture.create.vulkan.texture".encode()

SDL_GetTextureProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTextureProperties", SDL_PropertiesID, [SDL_POINTER[SDL_Texture]], SDL_BINARY]

SDL_PROP_TEXTURE_COLORSPACE_NUMBER: bytes = "SDL.texture.colorspace".encode()
SDL_PROP_TEXTURE_FORMAT_NUMBER: bytes = "SDL.texture.format".encode()
SDL_PROP_TEXTURE_ACCESS_NUMBER: bytes = "SDL.texture.access".encode()
SDL_PROP_TEXTURE_WIDTH_NUMBER: bytes = "SDL.texture.width".encode()
SDL_PROP_TEXTURE_HEIGHT_NUMBER: bytes = "SDL.texture.height".encode()
SDL_PROP_TEXTURE_SDR_WHITE_POINT_FLOAT: bytes = "SDL.texture.SDR_white_point".encode()
SDL_PROP_TEXTURE_HDR_HEADROOM_FLOAT: bytes = "SDL.texture.HDR_headroom".encode()
SDL_PROP_TEXTURE_D3D11_TEXTURE_POINTER: bytes = "SDL.texture.d3d11.texture".encode()
SDL_PROP_TEXTURE_D3D11_TEXTURE_U_POINTER: bytes = "SDL.texture.d3d11.texture_u".encode()
SDL_PROP_TEXTURE_D3D11_TEXTURE_V_POINTER: bytes = "SDL.texture.d3d11.texture_v".encode()
SDL_PROP_TEXTURE_D3D12_TEXTURE_POINTER: bytes = "SDL.texture.d3d12.texture".encode()
SDL_PROP_TEXTURE_D3D12_TEXTURE_U_POINTER: bytes = "SDL.texture.d3d12.texture_u".encode()
SDL_PROP_TEXTURE_D3D12_TEXTURE_V_POINTER: bytes = "SDL.texture.d3d12.texture_v".encode()
SDL_PROP_TEXTURE_OPENGL_TEXTURE_NUMBER: bytes = "SDL.texture.opengl.texture".encode()
SDL_PROP_TEXTURE_OPENGL_TEXTURE_UV_NUMBER: bytes = "SDL.texture.opengl.texture_uv".encode()
SDL_PROP_TEXTURE_OPENGL_TEXTURE_U_NUMBER: bytes = "SDL.texture.opengl.texture_u".encode()
SDL_PROP_TEXTURE_OPENGL_TEXTURE_V_NUMBER: bytes = "SDL.texture.opengl.texture_v".encode()
SDL_PROP_TEXTURE_OPENGL_TEXTURE_TARGET_NUMBER: bytes = "SDL.texture.opengl.target".encode()
SDL_PROP_TEXTURE_OPENGL_TEX_W_FLOAT: bytes = "SDL.texture.opengl.tex_w".encode()
SDL_PROP_TEXTURE_OPENGL_TEX_H_FLOAT: bytes = "SDL.texture.opengl.tex_h".encode()
SDL_PROP_TEXTURE_OPENGLES2_TEXTURE_NUMBER: bytes = "SDL.texture.opengles2.texture".encode()
SDL_PROP_TEXTURE_OPENGLES2_TEXTURE_UV_NUMBER: bytes = "SDL.texture.opengles2.texture_uv".encode()
SDL_PROP_TEXTURE_OPENGLES2_TEXTURE_U_NUMBER: bytes = "SDL.texture.opengles2.texture_u".encode()
SDL_PROP_TEXTURE_OPENGLES2_TEXTURE_V_NUMBER: bytes = "SDL.texture.opengles2.texture_v".encode()
SDL_PROP_TEXTURE_OPENGLES2_TEXTURE_TARGET_NUMBER: bytes = "SDL.texture.opengles2.target".encode()
SDL_PROP_TEXTURE_VULKAN_TEXTURE_NUMBER: bytes = "SDL.texture.vulkan.texture".encode()

SDL_GetRendererFromTexture: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRendererFromTexture", SDL_POINTER[SDL_Renderer], [SDL_POINTER[SDL_Texture]], SDL_BINARY]
SDL_GetTextureSize: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTextureSize", ctypes.c_bool, [SDL_POINTER[SDL_Texture], SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float]], SDL_BINARY]

SDL_SetTextureColorMod: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetTextureColorMod", ctypes.c_bool, [SDL_POINTER[SDL_Texture], ctypes.c_uint8, ctypes.c_uint8, ctypes.c_uint8], SDL_BINARY]
SDL_SetTextureColorModFloat: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetTextureColorModFloat", ctypes.c_bool, [SDL_POINTER[SDL_Texture], ctypes.c_float, ctypes.c_float, ctypes.c_float], SDL_BINARY]

SDL_GetTextureColorMod: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTextureColorMod", ctypes.c_bool, [SDL_POINTER[SDL_Texture], SDL_POINTER[ctypes.c_uint8], SDL_POINTER[ctypes.c_uint8], SDL_POINTER[ctypes.c_uint8]], SDL_BINARY]
SDL_GetTextureColorModFloat: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTextureColorModFloat", ctypes.c_bool, [SDL_POINTER[SDL_Texture], SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float]], SDL_BINARY]

SDL_SetTextureAlphaMod: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetTextureAlphaMod", ctypes.c_bool, [SDL_POINTER[SDL_Texture], ctypes.c_uint8], SDL_BINARY]
SDL_SetTextureAlphaModFloat: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetTextureAlphaModFloat", ctypes.c_bool, [SDL_POINTER[SDL_Texture], ctypes.c_float], SDL_BINARY]

SDL_GetTextureAlphaMod: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTextureAlphaMod", ctypes.c_bool, [SDL_POINTER[SDL_Texture], SDL_POINTER[ctypes.c_uint8]], SDL_BINARY]
SDL_GetTextureAlphaModFloat: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTextureAlphaModFloat", ctypes.c_bool, [SDL_POINTER[SDL_Texture], SDL_POINTER[ctypes.c_float]], SDL_BINARY]

SDL_SetTextureBlendMode: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetTextureBlendMode", ctypes.c_bool, [SDL_POINTER[SDL_Texture], SDL_BlendMode], SDL_BINARY]
SDL_GetTextureBlendMode: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTextureBlendMode", ctypes.c_bool, [SDL_POINTER[SDL_Texture], SDL_POINTER[SDL_BlendMode]], SDL_BINARY]

SDL_SetTextureScaleMode: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetTextureScaleMode", ctypes.c_bool, [SDL_POINTER[SDL_Texture], SDL_ScaleMode], SDL_BINARY]
SDL_GetTextureScaleMode: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetTextureScaleMode", ctypes.c_bool, [SDL_POINTER[SDL_Texture], SDL_POINTER[SDL_ScaleMode]], SDL_BINARY]

SDL_UpdateTexture: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UpdateTexture", ctypes.c_bool, [SDL_POINTER[SDL_Texture], SDL_POINTER[SDL_Rect], ctypes.c_void_p, ctypes.c_int], SDL_BINARY]
SDL_UpdateYUVTexture: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UpdateYUVTexture", ctypes.c_bool, [SDL_POINTER[SDL_Texture], SDL_POINTER[SDL_Rect], SDL_POINTER[ctypes.c_uint8], ctypes.c_int, SDL_POINTER[ctypes.c_uint8], ctypes.c_int, SDL_POINTER[ctypes.c_uint8], ctypes.c_int], SDL_BINARY]
SDL_UpdateNVTexture: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UpdateNVTexture", ctypes.c_bool, [SDL_POINTER[SDL_Texture], SDL_POINTER[SDL_Rect], SDL_POINTER[ctypes.c_uint8], ctypes.c_int, SDL_POINTER[ctypes.c_uint8], ctypes.c_int], SDL_BINARY]

SDL_LockTexture: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LockTexture", ctypes.c_bool, [SDL_POINTER[SDL_Texture], SDL_POINTER[SDL_Rect], SDL_POINTER[ctypes.c_void_p], SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_LockTextureToSurface: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_LockTextureToSurface", ctypes.c_bool, [SDL_POINTER[SDL_Texture], SDL_POINTER[SDL_Rect], SDL_POINTER[SDL_POINTER[SDL_Surface]]], SDL_BINARY]
SDL_UnlockTexture: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UnlockTexture", None, [SDL_POINTER[SDL_Texture]], SDL_BINARY]

SDL_SetRenderTarget: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetRenderTarget", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_Texture]], SDL_BINARY]
SDL_GetRenderTarget: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRenderTarget", SDL_POINTER[SDL_Texture], [SDL_POINTER[SDL_Renderer]], SDL_BINARY]

SDL_SetRenderLogicalPresentation: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetRenderLogicalPresentation", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], ctypes.c_int, ctypes.c_int, SDL_RendererLogicalPresentation], SDL_BINARY]
SDL_GetRenderLogicalPresentation: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRenderLogicalPresentation", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int], SDL_POINTER[SDL_RendererLogicalPresentation]], SDL_BINARY]
SDL_GetRenderLogicalPresentationRect: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRenderLogicalPresentationRect", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_FRect]], SDL_BINARY]

SDL_RenderCoordinatesFromWindow: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderCoordinatesFromWindow", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], ctypes.c_float, ctypes.c_float, SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float]], SDL_BINARY]
SDL_RenderCoordinatesToWindow: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderCoordinatesToWindow", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], ctypes.c_float, ctypes.c_float, SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float]], SDL_BINARY]

SDL_ConvertEventToRenderCoordinates: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ConvertEventToRenderCoordinates", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_Event]], SDL_BINARY]

SDL_SetRenderViewport: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetRenderViewport", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_Rect]], SDL_BINARY]
SDL_GetRenderViewport: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRenderViewport", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_Rect]], SDL_BINARY]

SDL_RenderViewportSet: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderViewportSet", ctypes.c_bool, [SDL_POINTER[SDL_Renderer]], SDL_BINARY]
SDL_GetRenderSafeArea: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRenderSafeArea", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_Rect]], SDL_BINARY]

SDL_SetRenderClipRect: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetRenderClipRect", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_Rect]], SDL_BINARY]
SDL_GetRenderClipRect: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRenderClipRect", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_Rect]], SDL_BINARY]

SDL_RenderClipEnabled: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderClipEnabled", ctypes.c_bool, [SDL_POINTER[SDL_Renderer]], SDL_BINARY]

SDL_SetRenderScale: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetRenderScale", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], ctypes.c_float, ctypes.c_float], SDL_BINARY]
SDL_GetRenderScale: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRenderScale", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float]], SDL_BINARY]

SDL_SetRenderDrawColor: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetRenderDrawColor", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], ctypes.c_uint8, ctypes.c_uint8, ctypes.c_uint8, ctypes.c_uint8], SDL_BINARY]
SDL_SetRenderDrawColorFloat: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetRenderDrawColorFloat", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float], SDL_BINARY]

SDL_GetRenderDrawColor: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRenderDrawColor", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[ctypes.c_uint8], SDL_POINTER[ctypes.c_uint8], SDL_POINTER[ctypes.c_uint8], SDL_POINTER[ctypes.c_uint8]], SDL_BINARY]
SDL_GetRenderDrawColorFloat: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRenderDrawColorFloat", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float]], SDL_BINARY]

SDL_SetRenderColorScale: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetRenderColorScale", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], ctypes.c_float], SDL_BINARY]
SDL_GetRenderColorScale: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRenderColorScale", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[ctypes.c_float]], SDL_BINARY]

SDL_SetRenderDrawBlendMode: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetRenderDrawBlendMode", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_BlendMode], SDL_BINARY]
SDL_GetRenderDrawBlendMode: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRenderDrawBlendMode", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_BlendMode]], SDL_BINARY]

SDL_RenderClear: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderClear", ctypes.c_bool, [SDL_POINTER[SDL_Renderer]], SDL_BINARY]

SDL_RenderPoint: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderPoint", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], ctypes.c_float, ctypes.c_float], SDL_BINARY]
SDL_RenderPoints: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderPoints", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_FPoint], ctypes.c_int], SDL_BINARY]

SDL_RenderLine: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderLine", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float], SDL_BINARY]
SDL_RenderLines: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderLines", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_FPoint], ctypes.c_int], SDL_BINARY]

SDL_RenderRect: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderRect", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_FRect]], SDL_BINARY]
SDL_RenderRects: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderRects", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_FRect], ctypes.c_int], SDL_BINARY]

SDL_RenderFillRect: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderFillRect", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_FRect]], SDL_BINARY]
SDL_RenderFillRects: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderFillRects", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_FRect], ctypes.c_int], SDL_BINARY]

SDL_RenderTexture: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderTexture", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_Texture], SDL_POINTER[SDL_FRect], SDL_POINTER[SDL_FRect]], SDL_BINARY]
SDL_RenderTextureRotated: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderTextureRotated", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_Texture], SDL_POINTER[SDL_FRect], SDL_POINTER[SDL_FRect], ctypes.c_double, SDL_POINTER[SDL_FPoint], SDL_FlipMode], SDL_BINARY]
SDL_RenderTextureAffine: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderTextureAffine", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_Texture], SDL_POINTER[SDL_FRect], SDL_POINTER[SDL_FPoint], SDL_POINTER[SDL_FPoint], SDL_POINTER[SDL_FPoint]], SDL_BINARY]
SDL_RenderTextureTiled: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderTextureTiled", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_Texture], SDL_POINTER[SDL_FRect], ctypes.c_float, SDL_POINTER[SDL_FRect]], SDL_BINARY]
SDL_RenderTexture9Grid: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderTexture9Grid", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_Texture], SDL_POINTER[SDL_FRect], ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float, SDL_POINTER[SDL_FRect]], SDL_BINARY]

SDL_RenderGeometry: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderGeometry", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_Texture], SDL_POINTER[SDL_Vertex], ctypes.c_int, SDL_POINTER[ctypes.c_int], ctypes.c_int], SDL_BINARY]
SDL_RenderGeometryRaw: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderGeometryRaw", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_Texture], SDL_POINTER[ctypes.c_float], ctypes.c_int, SDL_POINTER[SDL_FColor], ctypes.c_int, SDL_POINTER[ctypes.c_float], ctypes.c_int, ctypes.c_int, ctypes.c_void_p, ctypes.c_int, ctypes.c_int], SDL_BINARY]

SDL_RenderReadPixels: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderReadPixels", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_Rect]], SDL_BINARY]
SDL_RenderPresent: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderPresent", ctypes.c_bool, [SDL_POINTER[SDL_Renderer]], SDL_BINARY]

SDL_DestroyTexture: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DestroyTexture", None, [SDL_POINTER[SDL_Texture]], SDL_BINARY]
SDL_DestroyRenderer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DestroyRenderer", None, [SDL_POINTER[SDL_Renderer]], SDL_BINARY]
SDL_FlushRenderer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_FlushRenderer", ctypes.c_bool, [SDL_POINTER[SDL_Renderer]], SDL_BINARY]

SDL_GetRenderMetalLayer: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRenderMetalLayer", ctypes.c_void_p, [SDL_POINTER[SDL_Renderer]], SDL_BINARY]
SDL_GetRenderMetalCommandEncoder: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRenderMetalCommandEncoder", ctypes.c_void_p, [SDL_POINTER[SDL_Renderer]], SDL_BINARY]

SDL_AddVulkanRenderSemaphores: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_AddVulkanRenderSemaphores", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], ctypes.c_uint32, ctypes.c_int64, ctypes.c_int64], SDL_BINARY]

SDL_SetRenderVSync: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetRenderVSync", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], ctypes.c_int], SDL_BINARY]

SDL_RENDERER_VSYNC_DISABLED, SDL_RENDERER_VSYNC_ADAPTIVE = 0, -1

SDL_GetRenderVSync: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetRenderVSync", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], SDL_POINTER[ctypes.c_int]], SDL_BINARY]

SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE: int = 8

SDL_RenderDebugText: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderDebugText", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], ctypes.c_float, ctypes.c_float, ctypes.c_char_p], SDL_BINARY]
SDL_RenderDebugTextFormat: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RenderDebugTextFormat", ctypes.c_bool, [SDL_POINTER[SDL_Renderer], ctypes.c_float, ctypes.c_float, ctypes.c_char_p, ...], SDL_BINARY]
