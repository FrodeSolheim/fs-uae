import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_FUNC_TYPE, \
    SDL_FUNC, SDL_TYPE, SDL_BINARY, SDL_ENUM
from .SDL_properties import SDL_PropertiesID
from .SDL_surface import SDL_Surface
from .SDL_pixels import SDL_PixelFormat
from .SDL_rect import SDL_Point, SDL_Rect
from .SDL_stdinc import SDL_FunctionPointer

SDL_DisplayID: typing.TypeAlias = SDL_TYPE["SDL_DisplayID", ctypes.c_uint32]
SDL_WindowID: typing.TypeAlias = SDL_TYPE["SDL_WindowID", ctypes.c_uint32]

SDL_PROP_GLOBAL_VIDEO_WAYLAND_WL_DISPLAY_POINTER: bytes = "SDL.video.wayland.wl_display".encode()

SDL_SystemTheme: typing.TypeAlias = SDL_TYPE["SDL_SystemTheme", SDL_ENUM]

SDL_SYSTEM_THEME_UNKNOWN, SDL_SYSTEM_THEME_LIGHT, SDL_SYSTEM_THEME_DARK = range(3)

class SDL_DisplayModeData(ctypes.c_void_p):
    ...

class SDL_DisplayMode(ctypes.Structure):
    _fields_ = [
        ("displayID", SDL_DisplayID),
        ("format", SDL_PixelFormat),
        ("w", ctypes.c_int),
        ("h", ctypes.c_int),
        ("pixel_density", ctypes.c_float),
        ("refresh_rate", ctypes.c_float),
        ("refresh_rate_numerator", ctypes.c_int),
        ("refresh_rate_denominator", ctypes.c_int),
        ("internal", SDL_POINTER[SDL_DisplayModeData])
    ]

SDL_DisplayOrientation: typing.TypeAlias = SDL_TYPE["SDL_DisplayOrientation", SDL_ENUM]

SDL_ORIENTATION_UNKNOWN, SDL_ORIENTATION_LANDSCAPE, SDL_ORIENTATION_LANDSCAPE_FLIPPED, \
    SDL_ORIENTATION_PORTRAIT, SDL_ORIENTATION_PORTRAIT_FLIPPED = range(5)

class SDL_Window(ctypes.c_void_p):
    ...

SDL_WindowFlags: typing.TypeAlias = SDL_TYPE["SDL_WindowFlags", ctypes.c_uint64]

SDL_WINDOW_FULLSCREEN: int = 0x0000000000000001
SDL_WINDOW_OPENGL: int = 0x0000000000000002
SDL_WINDOW_OCCLUDED: int = 0x0000000000000004
SDL_WINDOW_HIDDEN: int = 0x0000000000000008
SDL_WINDOW_BORDERLESS: int = 0x0000000000000010
SDL_WINDOW_RESIZABLE: int = 0x0000000000000020
SDL_WINDOW_MINIMIZED: int = 0x0000000000000040
SDL_WINDOW_MAXIMIZED: int = 0x0000000000000080
SDL_WINDOW_MOUSE_GRABBED: int = 0x0000000000000100
SDL_WINDOW_INPUT_FOCUS: int = 0x0000000000000200
SDL_WINDOW_MOUSE_FOCUS: int = 0x0000000000000400
SDL_WINDOW_EXTERNAL: int = 0x0000000000000800
SDL_WINDOW_MODAL: int = 0x0000000000001000
SDL_WINDOW_HIGH_PIXEL_DENSITY: int = 0x0000000000002000
SDL_WINDOW_MOUSE_CAPTURE: int = 0x0000000000004000
SDL_WINDOW_MOUSE_RELATIVE_MODE: int = 0x0000000000008000
SDL_WINDOW_ALWAYS_ON_TOP: int = 0x0000000000010000
SDL_WINDOW_UTILITY: int = 0x0000000000020000
SDL_WINDOW_TOOLTIP: int = 0x0000000000040000
SDL_WINDOW_POPUP_MENU: int = 0x0000000000080000
SDL_WINDOW_KEYBOARD_GRABBED: int = 0x0000000000100000
SDL_WINDOW_VULKAN: int = 0x0000000010000000
SDL_WINDOW_METAL: int = 0x0000000020000000
SDL_WINDOW_TRANSPARENT: int = 0x0000000040000000
SDL_WINDOW_NOT_FOCUSABLE: int = 0x0000000080000000

SDL_WINDOWPOS_UNDEFINED_MASK: int = 0x1FFF0000
SDL_WINDOWPOS_UNDEFINED_DISPLAY: abc.Callable[[SDL_DisplayID], int] = lambda x: SDL_WINDOWPOS_UNDEFINED_MASK | x.value
SDL_WINDOWPOS_UNDEFINED = SDL_WINDOWPOS_UNDEFINED_DISPLAY(SDL_DisplayID(0))
SDL_WINDOWPOS_ISUNDEFINED: abc.Callable[[int], bool] = lambda x: (x & 0xFFFF0000) == SDL_WINDOWPOS_UNDEFINED_MASK

SDL_WINDOWPOS_CENTERED_MASK: int = 0x2FFF0000
SDL_WINDOWPOS_CENTERED_DISPLAY: abc.Callable[[SDL_DisplayID], int] = lambda x: SDL_WINDOWPOS_CENTERED_MASK | x.value
SDL_WINDOWPOS_CENTERED = SDL_WINDOWPOS_CENTERED_DISPLAY(SDL_DisplayID(0))
SDL_WINDOWPOS_ISCENTERED: abc.Callable[[int], bool] = lambda x: (x & 0xFFFF0000) == SDL_WINDOWPOS_CENTERED_MASK

SDL_FlashOperation: typing.TypeAlias = SDL_TYPE["SDL_FlashOperation", SDL_ENUM]

SDL_FLASH_CANCEL, SDL_FLASH_BRIEFLY, SDL_FLASH_UNTIL_FOCUSED = range(3)

class SDL_GLContextState(ctypes.c_void_p):
    ...

SDL_GLContext: typing.TypeAlias = SDL_TYPE["SDL_GLContext", SDL_POINTER[SDL_GLContextState]]

SDL_EGLDisplay: typing.TypeAlias = SDL_TYPE["SDL_EGLDisplay", ctypes.c_void_p]
SDL_EGLConfig: typing.TypeAlias = SDL_TYPE["SDL_EGLConfig", ctypes.c_void_p]
SDL_EGLSurface: typing.TypeAlias = SDL_TYPE["SDL_EGLSurface", ctypes.c_void_p]
SDL_EGLAttrib: typing.TypeAlias = SDL_TYPE["SDL_EGLAttrib", ctypes.c_longlong]
SDL_EGLint: typing.TypeAlias = SDL_TYPE["SDL_EGLint", ctypes.c_int]

SDL_EGLAttribArrayCallback: typing.TypeAlias = SDL_FUNC_TYPE["SDL_EGLAttribArrayCallback", SDL_POINTER[SDL_EGLAttrib], [ctypes.c_void_p]]
SDL_EGLIntArrayCallback: typing.TypeAlias = SDL_FUNC_TYPE["SDL_EGLIntArrayCallback", SDL_POINTER[SDL_EGLint], [ctypes.c_void_p, SDL_EGLDisplay, SDL_EGLConfig]]

SDL_GLAttr: typing.TypeAlias = SDL_TYPE["SDL_GLAttr", SDL_ENUM]

SDL_GL_RED_SIZE, SDL_GL_GREEN_SIZE, SDL_GL_BLUE_SIZE, SDL_GL_ALPHA_SIZE, SDL_GL_BUFFER_SIZE, SDL_GL_DOUBLEBUFFER, SDL_GL_DEPTH_SIZE, SDL_GL_STENCIL_SIZE, \
    SDL_GL_ACCUM_RED_SIZE, SDL_GL_ACCUM_GREEN_SIZE, SDL_GL_ACCUM_BLUE_SIZE, SDL_GL_ACCUM_ALPHA_SIZE, SDL_GL_STEREO, SDL_GL_MULTISAMPLEBUFFERS, SDL_GL_MULTISAMPLESAMPLES, \
        SDL_GL_ACCELERATED_VISUAL, SDL_GL_RETAINED_BACKING, SDL_GL_CONTEXT_MAJOR_VERSION, SDL_GL_CONTEXT_MINOR_VERSION, SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_PROFILE_MASK, \
            SDL_GL_SHARE_WITH_CURRENT_CONTEXT, SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, SDL_GL_CONTEXT_RELEASE_BEHAVIOR, SDL_GL_CONTEXT_RESET_NOTIFICATION, \
                SDL_GL_CONTEXT_NO_ERROR, SDL_GL_FLOATBUFFERS, SDL_GL_EGL_PLATFORM = range(28)

SDL_GLProfile: typing.TypeAlias = SDL_TYPE["SDL_GLProfile", ctypes.c_uint32]

SDL_GL_CONTEXT_PROFILE_CORE: int = 0x0001
SDL_GL_CONTEXT_PROFILE_COMPATIBILITY: int = 0x0002
SDL_GL_CONTEXT_PROFILE_ES: int = 0x0004

SDL_GLContextFlag: typing.TypeAlias = SDL_TYPE["SDL_GLContextFlag", ctypes.c_uint32]

SDL_GL_CONTEXT_DEBUG_FLAG: int = 0x0001
SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG: int = 0x0002
SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG: int = 0x0004
SDL_GL_CONTEXT_RESET_ISOLATION_FLAG: int = 0x0008

SDL_GLContextReleaseFlag: typing.TypeAlias = SDL_TYPE["SDL_GLContextReleaseFlag", ctypes.c_uint32]

SDL_GL_CONTEXT_RELEASE_BEHAVIOR_NONE: int = 0x0000
SDL_GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH: int = 0x0001

SDL_GLContextResetNotification: typing.TypeAlias = SDL_TYPE["SDL_GLContextResetNotification", ctypes.c_uint32]

SDL_GL_CONTEXT_RESET_NO_NOTIFICATION: int = 0x0000
SDL_GL_CONTEXT_RESET_LOSE_CONTEXT: int = 0x0001

SDL_GetNumVideoDrivers: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetNumVideoDrivers", ctypes.c_int, [], SDL_BINARY]
SDL_GetVideoDriver: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetVideoDriver", ctypes.c_char_p, [ctypes.c_int], SDL_BINARY]
SDL_GetCurrentVideoDriver: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetCurrentVideoDriver", ctypes.c_char_p, [], SDL_BINARY]
SDL_GetSystemTheme: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetSystemTheme", SDL_SystemTheme, [], SDL_BINARY]
SDL_GetDisplays: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetDisplays", SDL_POINTER[SDL_DisplayID], [SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_GetPrimaryDisplay: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetPrimaryDisplay", SDL_DisplayID, [], SDL_BINARY]
SDL_GetDisplayProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetDisplayProperties", SDL_PropertiesID, [SDL_DisplayID], SDL_BINARY]

SDL_PROP_DISPLAY_HDR_ENABLED_BOOLEAN: bytes = "SDL.display.HDR_enabled".encode()
SDL_PROP_DISPLAY_KMSDRM_PANEL_ORIENTATION_NUMBER: bytes = "SDL.display.KMSDRM.panel_orientation".encode()

SDL_GetDisplayName: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetDisplayName", ctypes.c_char_p, [SDL_DisplayID], SDL_BINARY]
SDL_GetDisplayBounds: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetDisplayBounds", ctypes.c_bool, [SDL_DisplayID, SDL_POINTER[SDL_Rect]], SDL_BINARY]
SDL_GetDisplayUsableBounds: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetDisplayUsableBounds", ctypes.c_bool, [SDL_DisplayID, SDL_POINTER[SDL_Rect]], SDL_BINARY]

SDL_GetNaturalDisplayOrientation: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetNaturalDisplayOrientation", SDL_DisplayOrientation, [SDL_DisplayID], SDL_BINARY]
SDL_GetCurrentDisplayOrientation: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetCurrentDisplayOrientation", SDL_DisplayOrientation, [SDL_DisplayID], SDL_BINARY]

SDL_GetDisplayContentScale: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetDisplayContentScale", ctypes.c_float, [SDL_DisplayID], SDL_BINARY]

SDL_GetFullscreenDisplayModes: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetFullscreenDisplayModes", SDL_POINTER[SDL_POINTER[SDL_DisplayMode]], [SDL_DisplayID, SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_GetClosestFullscreenDisplayMode: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetClosestFullscreenDisplayMode", ctypes.c_bool, [SDL_DisplayID, ctypes.c_int, ctypes.c_int, ctypes.c_float, ctypes.c_bool, SDL_POINTER[SDL_DisplayMode]], SDL_BINARY]

SDL_GetDesktopDisplayMode: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetDesktopDisplayMode", SDL_POINTER[SDL_DisplayMode], [SDL_DisplayID], SDL_BINARY]
SDL_GetCurrentDisplayMode: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetCurrentDisplayMode", SDL_POINTER[SDL_DisplayMode], [SDL_DisplayID], SDL_BINARY]

SDL_GetDisplayForPoint: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetDisplayForPoint", SDL_DisplayID, [SDL_POINTER[SDL_Point]], SDL_BINARY]
SDL_GetDisplayForRect: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetDisplayForRect", SDL_DisplayID, [SDL_POINTER[SDL_Rect]], SDL_BINARY]
SDL_GetDisplayForWindow: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetDisplayForWindow", SDL_DisplayID, [SDL_POINTER[SDL_Window]], SDL_BINARY]

SDL_GetWindowPixelDensity: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowPixelDensity", ctypes.c_float, [SDL_POINTER[SDL_Window]], SDL_BINARY]
SDL_GetWindowDisplayScale: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowDisplayScale", ctypes.c_float, [SDL_POINTER[SDL_Window]], SDL_BINARY]

SDL_SetWindowFullscreenMode: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowFullscreenMode", ctypes.c_bool, [SDL_POINTER[SDL_Window], SDL_POINTER[SDL_DisplayMode]], SDL_BINARY]
SDL_GetWindowFullscreenMode: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowFullscreenMode", SDL_POINTER[SDL_DisplayMode], [SDL_POINTER[SDL_Window]], SDL_BINARY]

SDL_GetWindowICCProfile: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowICCProfile", ctypes.c_void_p, [SDL_POINTER[SDL_Window], SDL_POINTER[ctypes.c_size_t]], SDL_BINARY]
SDL_GetWindowPixelFormat: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowPixelFormat", SDL_PixelFormat, [SDL_POINTER[SDL_Window]], SDL_BINARY]

SDL_GetWindows: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindows", SDL_POINTER[SDL_POINTER[SDL_Window]], [SDL_POINTER[ctypes.c_int]], SDL_BINARY]

SDL_CreateWindow: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateWindow", SDL_POINTER[SDL_Window], [ctypes.c_char_p, ctypes.c_int, ctypes.c_int, SDL_WindowFlags], SDL_BINARY]
SDL_CreatePopupWindow: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreatePopupWindow", SDL_POINTER[SDL_Window], [SDL_POINTER[SDL_Window], ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, SDL_WindowFlags], SDL_BINARY]
SDL_CreateWindowWithProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_CreateWindowWithProperties", SDL_POINTER[SDL_Window], [SDL_PropertiesID], SDL_BINARY]

SDL_PROP_WINDOW_CREATE_ALWAYS_ON_TOP_BOOLEAN: bytes = "SDL.window.create.always_on_top".encode()
SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN: bytes = "SDL.window.create.borderless".encode()
SDL_PROP_WINDOW_CREATE_CONSTRAIN_POPUP_BOOLEAN: bytes = "SDL.window.create.constrain_popup".encode()
SDL_PROP_WINDOW_CREATE_FOCUSABLE_BOOLEAN: bytes = "SDL.window.create.focusable".encode()
SDL_PROP_WINDOW_CREATE_EXTERNAL_GRAPHICS_CONTEXT_BOOLEAN: bytes = "SDL.window.create.external_graphics_context".encode()
SDL_PROP_WINDOW_CREATE_FLAGS_NUMBER: bytes = "SDL.window.create.flags".encode()
SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN: bytes = "SDL.window.create.fullscreen".encode()
SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER: bytes = "SDL.window.create.height".encode()
SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN: bytes = "SDL.window.create.hidden".encode()
SDL_PROP_WINDOW_CREATE_HIGH_PIXEL_DENSITY_BOOLEAN: bytes = "SDL.window.create.high_pixel_density".encode()
SDL_PROP_WINDOW_CREATE_MAXIMIZED_BOOLEAN: bytes = "SDL.window.create.maximized".encode()
SDL_PROP_WINDOW_CREATE_MENU_BOOLEAN: bytes = "SDL.window.create.menu".encode()
SDL_PROP_WINDOW_CREATE_METAL_BOOLEAN: bytes = "SDL.window.create.metal".encode()
SDL_PROP_WINDOW_CREATE_MINIMIZED_BOOLEAN: bytes = "SDL.window.create.minimized".encode()
SDL_PROP_WINDOW_CREATE_MODAL_BOOLEAN: bytes = "SDL.window.create.modal".encode()
SDL_PROP_WINDOW_CREATE_MOUSE_GRABBED_BOOLEAN: bytes = "SDL.window.create.mouse_grabbed".encode()
SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN: bytes = "SDL.window.create.opengl".encode()
SDL_PROP_WINDOW_CREATE_PARENT_POINTER: bytes = "SDL.window.create.parent".encode()
SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN: bytes = "SDL.window.create.resizable".encode()
SDL_PROP_WINDOW_CREATE_TITLE_STRING: bytes = "SDL.window.create.title".encode()
SDL_PROP_WINDOW_CREATE_TRANSPARENT_BOOLEAN: bytes = "SDL.window.create.transparent".encode()
SDL_PROP_WINDOW_CREATE_TOOLTIP_BOOLEAN: bytes = "SDL.window.create.tooltip".encode()
SDL_PROP_WINDOW_CREATE_UTILITY_BOOLEAN: bytes = "SDL.window.create.utility".encode()
SDL_PROP_WINDOW_CREATE_VULKAN_BOOLEAN: bytes = "SDL.window.create.vulkan".encode()
SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER: bytes = "SDL.window.create.width".encode()
SDL_PROP_WINDOW_CREATE_X_NUMBER: bytes = "SDL.window.create.x".encode()
SDL_PROP_WINDOW_CREATE_Y_NUMBER: bytes = "SDL.window.create.y".encode()
SDL_PROP_WINDOW_CREATE_COCOA_WINDOW_POINTER: bytes = "SDL.window.create.cocoa.window".encode()
SDL_PROP_WINDOW_CREATE_COCOA_VIEW_POINTER: bytes = "SDL.window.create.cocoa.view".encode()
SDL_PROP_WINDOW_CREATE_WAYLAND_SURFACE_ROLE_CUSTOM_BOOLEAN: bytes = "SDL.window.create.wayland.surface_role_custom".encode()
SDL_PROP_WINDOW_CREATE_WAYLAND_CREATE_EGL_WINDOW_BOOLEAN: bytes = "SDL.window.create.wayland.create_egl_window".encode()
SDL_PROP_WINDOW_CREATE_WAYLAND_WL_SURFACE_POINTER: bytes = "SDL.window.create.wayland.wl_surface".encode()
SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER: bytes = "SDL.window.create.win32.hwnd".encode()
SDL_PROP_WINDOW_CREATE_WIN32_PIXEL_FORMAT_HWND_POINTER: bytes = "SDL.window.create.win32.pixel_format_hwnd".encode()
SDL_PROP_WINDOW_CREATE_X11_WINDOW_NUMBER: bytes = "SDL.window.create.x11.window".encode()

SDL_GetWindowID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowID", SDL_WindowID, [SDL_POINTER[SDL_Window]], SDL_BINARY]
SDL_GetWindowFromID: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowFromID", SDL_POINTER[SDL_Window], [SDL_WindowID], SDL_BINARY]
SDL_GetWindowParent: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowParent", SDL_POINTER[SDL_Window], [SDL_POINTER[SDL_Window]], SDL_BINARY]

SDL_GetWindowProperties: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowProperties", SDL_PropertiesID, [SDL_POINTER[SDL_Window]], SDL_BINARY]

SDL_PROP_WINDOW_SHAPE_POINTER: bytes = "SDL.window.shape".encode()
SDL_PROP_WINDOW_HDR_ENABLED_BOOLEAN: bytes = "SDL.window.HDR_enabled".encode()
SDL_PROP_WINDOW_SDR_WHITE_LEVEL_FLOAT: bytes = "SDL.window.SDR_white_level".encode()
SDL_PROP_WINDOW_HDR_HEADROOM_FLOAT: bytes = "SDL.window.HDR_headroom".encode()
SDL_PROP_WINDOW_ANDROID_WINDOW_POINTER: bytes = "SDL.window.android.window".encode()
SDL_PROP_WINDOW_ANDROID_SURFACE_POINTER: bytes = "SDL.window.android.surface".encode()
SDL_PROP_WINDOW_UIKIT_WINDOW_POINTER: bytes = "SDL.window.uikit.window".encode()
SDL_PROP_WINDOW_UIKIT_METAL_VIEW_TAG_NUMBER: bytes = "SDL.window.uikit.metal_view_tag".encode()
SDL_PROP_WINDOW_UIKIT_OPENGL_FRAMEBUFFER_NUMBER: bytes = "SDL.window.uikit.opengl.framebuffer".encode()
SDL_PROP_WINDOW_UIKIT_OPENGL_RENDERBUFFER_NUMBER: bytes = "SDL.window.uikit.opengl.renderbuffer".encode()
SDL_PROP_WINDOW_UIKIT_OPENGL_RESOLVE_FRAMEBUFFER_NUMBER: bytes = "SDL.window.uikit.opengl.resolve_framebuffer".encode()
SDL_PROP_WINDOW_KMSDRM_DEVICE_INDEX_NUMBER: bytes = "SDL.window.kmsdrm.dev_index".encode()
SDL_PROP_WINDOW_KMSDRM_DRM_FD_NUMBER: bytes = "SDL.window.kmsdrm.drm_fd".encode()
SDL_PROP_WINDOW_KMSDRM_GBM_DEVICE_POINTER: bytes = "SDL.window.kmsdrm.gbm_dev".encode()
SDL_PROP_WINDOW_COCOA_WINDOW_POINTER: bytes = "SDL.window.cocoa.window".encode()
SDL_PROP_WINDOW_COCOA_METAL_VIEW_TAG_NUMBER: bytes = "SDL.window.cocoa.metal_view_tag".encode()
SDL_PROP_WINDOW_OPENVR_OVERLAY_ID: bytes = "SDL.window.openvr.overlay_id".encode()
SDL_PROP_WINDOW_VIVANTE_DISPLAY_POINTER: bytes = "SDL.window.vivante.display".encode()
SDL_PROP_WINDOW_VIVANTE_WINDOW_POINTER: bytes = "SDL.window.vivante.window".encode()
SDL_PROP_WINDOW_VIVANTE_SURFACE_POINTER: bytes = "SDL.window.vivante.surface".encode()
SDL_PROP_WINDOW_WIN32_HWND_POINTER: bytes = "SDL.window.win32.hwnd".encode()
SDL_PROP_WINDOW_WIN32_HDC_POINTER: bytes = "SDL.window.win32.hdc".encode()
SDL_PROP_WINDOW_WIN32_INSTANCE_POINTER: bytes = "SDL.window.win32.instance".encode()
SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER: bytes = "SDL.window.wayland.display".encode()
SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER: bytes = "SDL.window.wayland.surface".encode()
SDL_PROP_WINDOW_WAYLAND_VIEWPORT_POINTER: bytes = "SDL.window.wayland.viewport".encode()
SDL_PROP_WINDOW_WAYLAND_EGL_WINDOW_POINTER: bytes = "SDL.window.wayland.egl_window".encode()
SDL_PROP_WINDOW_WAYLAND_XDG_SURFACE_POINTER: bytes = "SDL.window.wayland.xdg_surface".encode()
SDL_PROP_WINDOW_WAYLAND_XDG_TOPLEVEL_POINTER: bytes = "SDL.window.wayland.xdg_toplevel".encode()
SDL_PROP_WINDOW_WAYLAND_XDG_TOPLEVEL_EXPORT_HANDLE_STRING: bytes = "SDL.window.wayland.xdg_toplevel_export_handle".encode()
SDL_PROP_WINDOW_WAYLAND_XDG_POPUP_POINTER: bytes = "SDL.window.wayland.xdg_popup".encode()
SDL_PROP_WINDOW_WAYLAND_XDG_POSITIONER_POINTER: bytes = "SDL.window.wayland.xdg_positioner".encode()
SDL_PROP_WINDOW_X11_DISPLAY_POINTER: bytes = "SDL.window.x11.display".encode()
SDL_PROP_WINDOW_X11_SCREEN_NUMBER: bytes = "SDL.window.x11.screen".encode()
SDL_PROP_WINDOW_X11_WINDOW_NUMBER: bytes = "SDL.window.x11.window".encode()

SDL_GetWindowFlags: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowFlags", SDL_WindowFlags, [SDL_POINTER[SDL_Window]], SDL_BINARY]

SDL_SetWindowTitle: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowTitle", ctypes.c_bool, [SDL_POINTER[SDL_Window], ctypes.c_char_p], SDL_BINARY]
SDL_GetWindowTitle: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowTitle", ctypes.c_char_p, [SDL_POINTER[SDL_Window]], SDL_BINARY]

SDL_SetWindowIcon: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowIcon", ctypes.c_bool, [SDL_POINTER[SDL_Window], SDL_POINTER[SDL_Surface]], SDL_BINARY]

SDL_SetWindowPosition: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowPosition", ctypes.c_bool, [SDL_POINTER[SDL_Window], ctypes.c_int, ctypes.c_int], SDL_BINARY]
SDL_GetWindowPosition: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowPosition", ctypes.c_bool, [SDL_POINTER[SDL_Window], SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int]], SDL_BINARY]

SDL_SetWindowSize: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowSize", ctypes.c_bool, [SDL_POINTER[SDL_Window], ctypes.c_int, ctypes.c_int], SDL_BINARY]
SDL_GetWindowSize: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowSize", ctypes.c_bool, [SDL_POINTER[SDL_Window], SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int]], SDL_BINARY]

SDL_GetWindowSafeArea: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowSafeArea", ctypes.c_bool, [SDL_POINTER[SDL_Window], SDL_POINTER[SDL_Rect]], SDL_BINARY]

SDL_SetWindowAspectRatio: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowAspectRatio", ctypes.c_bool, [SDL_POINTER[SDL_Window], ctypes.c_float, ctypes.c_float], SDL_BINARY]
SDL_GetWindowAspectRatio: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowAspectRatio", ctypes.c_bool, [SDL_POINTER[SDL_Window], SDL_POINTER[ctypes.c_float], SDL_POINTER[ctypes.c_float]], SDL_BINARY]

SDL_GetWindowBordersSize: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowBordersSize", ctypes.c_bool, [SDL_POINTER[SDL_Window], SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int]], SDL_BINARY]
SDL_GetWindowSizeInPixels: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowSizeInPixels", ctypes.c_bool, [SDL_POINTER[SDL_Window], SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int]], SDL_BINARY]

SDL_SetWindowMinimumSize: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowMinimumSize", ctypes.c_bool, [SDL_POINTER[SDL_Window], ctypes.c_int, ctypes.c_int], SDL_BINARY]
SDL_GetWindowMinimumSize: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowMinimumSize", ctypes.c_bool, [SDL_POINTER[SDL_Window], SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int]], SDL_BINARY]

SDL_SetWindowMaximumSize: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowMaximumSize", ctypes.c_bool, [SDL_POINTER[SDL_Window], ctypes.c_int, ctypes.c_int], SDL_BINARY]
SDL_GetWindowMaximumSize: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowMaximumSize", ctypes.c_bool, [SDL_POINTER[SDL_Window], SDL_POINTER[ctypes.c_int], SDL_POINTER[ctypes.c_int]], SDL_BINARY]

SDL_SetWindowBordered: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowBordered", ctypes.c_bool, [SDL_POINTER[SDL_Window], ctypes.c_bool], SDL_BINARY]
SDL_SetWindowResizable: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowResizable", ctypes.c_bool, [SDL_POINTER[SDL_Window], ctypes.c_bool], SDL_BINARY]
SDL_SetWindowAlwaysOnTop: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowAlwaysOnTop", ctypes.c_bool, [SDL_POINTER[SDL_Window], ctypes.c_bool], SDL_BINARY]

SDL_ShowWindow: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShowWindow", ctypes.c_bool, [SDL_POINTER[SDL_Window]], SDL_BINARY]
SDL_HideWindow: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_HideWindow", ctypes.c_bool, [SDL_POINTER[SDL_Window]], SDL_BINARY]
SDL_RaiseWindow: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RaiseWindow", ctypes.c_bool, [SDL_POINTER[SDL_Window]], SDL_BINARY]
SDL_MaximizeWindow: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_MaximizeWindow", ctypes.c_bool, [SDL_POINTER[SDL_Window]], SDL_BINARY]
SDL_MinimizeWindow: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_MinimizeWindow", ctypes.c_bool, [SDL_POINTER[SDL_Window]], SDL_BINARY]
SDL_RestoreWindow: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_RestoreWindow", ctypes.c_bool, [SDL_POINTER[SDL_Window]], SDL_BINARY]

SDL_SetWindowFullscreen: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowFullscreen", ctypes.c_bool, [SDL_POINTER[SDL_Window], ctypes.c_bool], SDL_BINARY]
SDL_SyncWindow: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SyncWindow", ctypes.c_bool, [SDL_POINTER[SDL_Window]], SDL_BINARY]
SDL_WindowHasSurface: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_WindowHasSurface", ctypes.c_bool, [SDL_POINTER[SDL_Window]], SDL_BINARY]

SDL_GetWindowSurface: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowSurface", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_Window]], SDL_BINARY]
SDL_SetWindowSurfaceVSync: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowSurfaceVSync", ctypes.c_bool, [SDL_POINTER[SDL_Window], ctypes.c_int], SDL_BINARY]

SDL_WINDOW_SURFACE_VSYNC_DISABLED, SDL_WINDOW_SURFACE_VSYNC_ADAPTIVE = 0, -1

SDL_GetWindowSurfaceVSync: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowSurfaceVSync", ctypes.c_bool, [SDL_POINTER[SDL_Window], SDL_POINTER[ctypes.c_int]], SDL_BINARY]

SDL_UpdateWindowSurface: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UpdateWindowSurface", ctypes.c_bool, [SDL_POINTER[SDL_Window]], SDL_BINARY]
SDL_UpdateWindowSurfaceRects: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_UpdateWindowSurfaceRects", ctypes.c_bool, [SDL_POINTER[SDL_Window], SDL_POINTER[SDL_Rect], ctypes.c_int], SDL_BINARY]
SDL_DestroyWindowSurface: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DestroyWindowSurface", ctypes.c_bool, [SDL_POINTER[SDL_Window]], SDL_BINARY]

SDL_SetWindowKeyboardGrab: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowKeyboardGrab", ctypes.c_bool, [SDL_POINTER[SDL_Window], ctypes.c_bool], SDL_BINARY]
SDL_SetWindowMouseGrab: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowMouseGrab", ctypes.c_bool, [SDL_POINTER[SDL_Window], ctypes.c_bool], SDL_BINARY]

SDL_GetWindowKeyboardGrab: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowKeyboardGrab", ctypes.c_bool, [SDL_POINTER[SDL_Window]], SDL_BINARY]
SDL_GetWindowMouseGrab: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowMouseGrab", ctypes.c_bool, [SDL_POINTER[SDL_Window]], SDL_BINARY]

SDL_GetGrabbedWindow: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetGrabbedWindow", SDL_POINTER[SDL_Window], [], SDL_BINARY]

SDL_SetWindowMouseRect: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowMouseRect", ctypes.c_bool, [SDL_POINTER[SDL_Window], SDL_POINTER[SDL_Rect]], SDL_BINARY]
SDL_GetWindowMouseRect: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowMouseRect", SDL_POINTER[SDL_Rect], [SDL_POINTER[SDL_Window]], SDL_BINARY]

SDL_SetWindowOpacity: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowOpacity", ctypes.c_bool, [SDL_POINTER[SDL_Window], ctypes.c_float], SDL_BINARY]
SDL_GetWindowOpacity: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GetWindowOpacity", ctypes.c_float, [SDL_POINTER[SDL_Window]], SDL_BINARY]

SDL_SetWindowParent: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowParent", ctypes.c_bool, [SDL_POINTER[SDL_Window], SDL_POINTER[SDL_Window]], SDL_BINARY]
SDL_SetWindowModal: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowModal", ctypes.c_bool, [SDL_POINTER[SDL_Window], ctypes.c_bool], SDL_BINARY]
SDL_SetWindowFocusable: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowFocusable", ctypes.c_bool, [SDL_POINTER[SDL_Window], ctypes.c_bool], SDL_BINARY]

SDL_ShowWindowSystemMenu: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ShowWindowSystemMenu", ctypes.c_bool, [SDL_POINTER[SDL_Window], ctypes.c_int, ctypes.c_int], SDL_BINARY]

SDL_HitTestResult: typing.TypeAlias = SDL_TYPE["SDL_HitTestResult", SDL_ENUM]

SDL_HITTEST_NORMAL, SDL_HITTEST_DRAGGABLE, SDL_HITTEST_RESIZE_TOPLEFT, SDL_HITTEST_RESIZE_TOP, SDL_HITTEST_RESIZE_TOPRIGHT, \
    SDL_HITTEST_RESIZE_RIGHT, SDL_HITTEST_RESIZE_BOTTOMRIGHT, SDL_HITTEST_RESIZE_BOTTOM, SDL_HITTEST_RESIZE_BOTTOMLEFT, SDL_HITTEST_RESIZE_LEFT = range(10)

SDL_HitTest: typing.TypeAlias = SDL_FUNC_TYPE["SDL_HitTest", SDL_HitTestResult, [SDL_POINTER[SDL_Window], SDL_POINTER[SDL_Point], ctypes.c_void_p]]

SDL_SetWindowHitTest: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowHitTest", ctypes.c_bool, [SDL_POINTER[SDL_Window], SDL_HitTest, ctypes.c_void_p], SDL_BINARY]
SDL_SetWindowShape: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_SetWindowShape", ctypes.c_bool, [SDL_POINTER[SDL_Window], SDL_POINTER[SDL_Surface]], SDL_BINARY]

SDL_FlashWindow: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_FlashWindow", ctypes.c_bool, [SDL_POINTER[SDL_Window], SDL_FlashOperation], SDL_BINARY]
SDL_DestroyWindow: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DestroyWindow", None, [SDL_POINTER[SDL_Window]], SDL_BINARY]

SDL_ScreenSaverEnabled: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_ScreenSaverEnabled", ctypes.c_bool, [], SDL_BINARY]
SDL_EnableScreenSaver: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_EnableScreenSaver", ctypes.c_bool, [], SDL_BINARY]
SDL_DisableScreenSaver: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_DisableScreenSaver", ctypes.c_bool, [], SDL_BINARY]

SDL_GL_LoadLibrary: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GL_LoadLibrary", ctypes.c_bool, [ctypes.c_char_p], SDL_BINARY]
SDL_GL_GetProcAddress: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GL_GetProcAddress", SDL_FunctionPointer, [ctypes.c_char_p], SDL_BINARY]
SDL_EGL_GetProcAddress: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_EGL_GetProcAddress", SDL_FunctionPointer, [ctypes.c_char_p], SDL_BINARY]
SDL_GL_UnloadLibrary: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GL_UnloadLibrary", None, [], SDL_BINARY]

SDL_GL_ExtensionSupported: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GL_ExtensionSupported", ctypes.c_bool, [ctypes.c_char_p], SDL_BINARY]

SDL_GL_ResetAttributes: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GL_ResetAttributes", None, [], SDL_BINARY]
SDL_GL_SetAttribute: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GL_SetAttribute", ctypes.c_bool, [SDL_GLAttr, ctypes.c_int], SDL_BINARY]
SDL_GL_GetAttribute: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GL_GetAttribute", ctypes.c_bool, [SDL_GLAttr, SDL_POINTER[ctypes.c_int]], SDL_BINARY]

SDL_GL_CreateContext: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GL_CreateContext", SDL_GLContext, [SDL_POINTER[SDL_Window]], SDL_BINARY]
SDL_GL_MakeCurrent: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GL_MakeCurrent", ctypes.c_bool, [SDL_POINTER[SDL_Window], SDL_GLContext], SDL_BINARY]

SDL_GL_GetCurrentWindow: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GL_GetCurrentWindow", SDL_POINTER[SDL_Window], [], SDL_BINARY]
SDL_GL_GetCurrentContext: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GL_GetCurrentContext", SDL_GLContext, [], SDL_BINARY]

SDL_EGL_GetCurrentDisplay: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_EGL_GetCurrentDisplay", SDL_EGLDisplay, [], SDL_BINARY]
SDL_EGL_GetCurrentConfig: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_EGL_GetCurrentConfig", SDL_EGLConfig, [], SDL_BINARY]
SDL_EGL_GetWindowSurface: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_EGL_GetWindowSurface", SDL_EGLSurface, [SDL_POINTER[SDL_Window]], SDL_BINARY]

SDL_EGL_SetAttributeCallbacks: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_EGL_SetAttributeCallbacks", None, [SDL_EGLAttribArrayCallback, SDL_EGLIntArrayCallback, SDL_EGLIntArrayCallback, ctypes.c_void_p], SDL_BINARY]

SDL_GL_SetSwapInterval: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GL_SetSwapInterval", ctypes.c_bool, [ctypes.c_int], SDL_BINARY]
SDL_GL_GetSwapInterval: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GL_GetSwapInterval", ctypes.c_bool, [SDL_POINTER[ctypes.c_int]], SDL_BINARY]

SDL_GL_SwapWindow: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GL_SwapWindow", ctypes.c_bool, [SDL_POINTER[SDL_Window]], SDL_BINARY]
SDL_GL_DestroyContext: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_GL_DestroyContext", ctypes.c_bool, [SDL_GLContext], SDL_BINARY]