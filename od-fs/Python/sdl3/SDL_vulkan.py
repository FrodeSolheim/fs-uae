import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_FUNC, SDL_TYPE, SDL_BINARY
from .SDL_stdinc import SDL_FunctionPointer
from .SDL_video import SDL_Window

class VkInstance(ctypes.c_void_p):
    ...

class VkPhysicalDevice(ctypes.c_void_p):
    ...

class VkAllocationCallbacks(ctypes.c_void_p):
    ...
    
VkSurfaceKHR: typing.TypeAlias = SDL_TYPE["VkSurfaceKHR", ctypes.c_uint64]

SDL_Vulkan_LoadLibrary: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_Vulkan_LoadLibrary", ctypes.c_bool, [ctypes.c_char_p], SDL_BINARY]
SDL_Vulkan_GetVkGetInstanceProcAddr: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_Vulkan_GetVkGetInstanceProcAddr", SDL_FunctionPointer, [], SDL_BINARY]
SDL_Vulkan_UnloadLibrary: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_Vulkan_UnloadLibrary", None, [], SDL_BINARY]
SDL_Vulkan_GetInstanceExtensions: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_Vulkan_GetInstanceExtensions", SDL_POINTER[ctypes.c_char_p], [SDL_POINTER[ctypes.c_uint32]], SDL_BINARY]

SDL_Vulkan_CreateSurface: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_Vulkan_CreateSurface", ctypes.c_bool, [SDL_POINTER[SDL_Window], VkInstance, SDL_POINTER[VkAllocationCallbacks], SDL_POINTER[VkSurfaceKHR]], SDL_BINARY]
SDL_Vulkan_DestroySurface: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_Vulkan_DestroySurface", None, [VkInstance, VkSurfaceKHR, SDL_POINTER[VkAllocationCallbacks]], SDL_BINARY]
SDL_Vulkan_GetPresentationSupport: abc.Callable[..., typing.Any] = SDL_FUNC["SDL_Vulkan_GetPresentationSupport", ctypes.c_bool, [VkInstance, VkPhysicalDevice, ctypes.c_uint32], SDL_BINARY]