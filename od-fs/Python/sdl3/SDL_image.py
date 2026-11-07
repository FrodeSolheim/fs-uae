import ctypes, typing, collections.abc as abc

from . import SDL_POINTER, SDL_FUNC, SDL_IMAGE_BINARY
from .SDL_surface import SDL_Surface
from .SDL_render import SDL_Texture, SDL_Renderer
from .SDL_iostream import SDL_IOStream
from .SDL_version import SDL_VERSIONNUM

SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_MICRO_VERSION = 3, 2, 4
SDL_IMAGE_VERSION: int = SDL_VERSIONNUM(SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_MICRO_VERSION)

SDL_IMAGE_VERSION_ATLEAST: abc.Callable[[int, int, int], bool] = lambda x, y, z: \
    (SDL_IMAGE_MAJOR_VERSION >= x) and (SDL_IMAGE_MAJOR_VERSION > x or SDL_IMAGE_MINOR_VERSION >= y) and \
        (SDL_IMAGE_MAJOR_VERSION > x or SDL_IMAGE_MINOR_VERSION > y or SDL_IMAGE_MICRO_VERSION >= z)

IMG_Version: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_Version", ctypes.c_int, [], SDL_IMAGE_BINARY]

IMG_LoadTyped_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadTyped_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream], ctypes.c_bool, ctypes.c_char_p], SDL_IMAGE_BINARY]
IMG_Load: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_Load", SDL_POINTER[SDL_Surface], [ctypes.c_char_p], SDL_IMAGE_BINARY]
IMG_Load_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_Load_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream], ctypes.c_bool], SDL_IMAGE_BINARY]

IMG_LoadTexture: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadTexture", SDL_POINTER[SDL_Texture], [SDL_POINTER[SDL_Renderer], ctypes.c_char_p], SDL_IMAGE_BINARY]
IMG_LoadTexture_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadTexture_IO", SDL_POINTER[SDL_Texture], [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_IOStream], ctypes.c_bool], SDL_IMAGE_BINARY]
IMG_LoadTextureTyped_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadTextureTyped_IO", SDL_POINTER[SDL_Texture], [SDL_POINTER[SDL_Renderer], SDL_POINTER[SDL_IOStream], ctypes.c_bool, ctypes.c_char_p], SDL_IMAGE_BINARY]

IMG_isAVIF: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_isAVIF", ctypes.c_bool, [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_isICO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_isICO", ctypes.c_bool, [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_isCUR: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_isCUR", ctypes.c_bool, [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_isBMP: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_isBMP", ctypes.c_bool, [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_isGIF: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_isGIF", ctypes.c_bool, [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_isJPG: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_isJPG", ctypes.c_bool, [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_isJXL: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_isJXL", ctypes.c_bool, [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_isLBM: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_isLBM", ctypes.c_bool, [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_isPCX: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_isPCX", ctypes.c_bool, [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_isPNG: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_isPNG", ctypes.c_bool, [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_isPNM: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_isPNM", ctypes.c_bool, [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_isSVG: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_isSVG", ctypes.c_bool, [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_isQOI: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_isQOI", ctypes.c_bool, [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_isTIF: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_isTIF", ctypes.c_bool, [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_isXCF: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_isXCF", ctypes.c_bool, [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_isXPM: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_isXPM", ctypes.c_bool, [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_isXV: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_isXV", ctypes.c_bool, [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_isWEBP: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_isWEBP", ctypes.c_bool, [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]

IMG_LoadAVIF_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadAVIF_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_LoadICO_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadICO_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_LoadCUR_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadCUR_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_LoadBMP_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadBMP_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_LoadGIF_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadGIF_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_LoadJPG_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadJPG_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_LoadJXL_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadJXL_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_LoadLBM_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadLBM_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_LoadPCX_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadPCX_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_LoadPNG_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadPNG_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_LoadPNM_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadPNM_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_LoadSVG_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadSVG_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_LoadQOI_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadQOI_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_LoadTGA_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadTGA_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_LoadTIF_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadTIF_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_LoadXCF_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadXCF_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_LoadXPM_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadXPM_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_LoadXV_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadXV_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_LoadWEBP_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadWEBP_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_LoadSizedSVG_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadSizedSVG_IO", SDL_POINTER[SDL_Surface], [SDL_POINTER[SDL_IOStream], ctypes.c_int, ctypes.c_int], SDL_IMAGE_BINARY]

IMG_ReadXPMFromArray: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_ReadXPMFromArray", SDL_POINTER[SDL_Surface], [SDL_POINTER[ctypes.c_char_p]], SDL_IMAGE_BINARY]
IMG_ReadXPMFromArrayToRGB888: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_ReadXPMFromArrayToRGB888", SDL_POINTER[SDL_Surface], [SDL_POINTER[ctypes.c_char_p]], SDL_IMAGE_BINARY]

IMG_SaveAVIF: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_SaveAVIF", ctypes.c_bool, [SDL_POINTER[SDL_Surface], ctypes.c_char_p, ctypes.c_int], SDL_IMAGE_BINARY]
IMG_SaveAVIF_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_SaveAVIF_IO", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_IOStream], ctypes.c_bool, ctypes.c_int], SDL_IMAGE_BINARY]
IMG_SavePNG: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_SavePNG", ctypes.c_bool, [SDL_POINTER[SDL_Surface], ctypes.c_char_p], SDL_IMAGE_BINARY]
IMG_SavePNG_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_SavePNG_IO", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_IOStream], ctypes.c_bool], SDL_IMAGE_BINARY]
IMG_SaveJPG: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_SaveJPG", ctypes.c_bool, [SDL_POINTER[SDL_Surface], ctypes.c_char_p, ctypes.c_int], SDL_IMAGE_BINARY]
IMG_SaveJPG_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_SaveJPG_IO", ctypes.c_bool, [SDL_POINTER[SDL_Surface], SDL_POINTER[SDL_IOStream], ctypes.c_bool, ctypes.c_int], SDL_IMAGE_BINARY]

class IMG_Animation(ctypes.Structure):
    _fields_ = [
        ("w", ctypes.c_int),
        ("h", ctypes.c_int),
        ("count", ctypes.c_int),
        ("frames", SDL_POINTER[SDL_POINTER[SDL_Surface]]),
        ("delays", SDL_POINTER[ctypes.c_int])
    ]

IMG_LoadAnimation: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadAnimation", SDL_POINTER[IMG_Animation], [ctypes.c_char_p], SDL_IMAGE_BINARY]
IMG_LoadAnimation_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadAnimation_IO", SDL_POINTER[IMG_Animation], [SDL_POINTER[SDL_IOStream], ctypes.c_bool], SDL_IMAGE_BINARY]
IMG_LoadAnimationTyped_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadAnimationTyped_IO", SDL_POINTER[IMG_Animation], [SDL_POINTER[SDL_IOStream], ctypes.c_bool, ctypes.c_char_p], SDL_IMAGE_BINARY]
IMG_FreeAnimation: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_FreeAnimation", None, [SDL_POINTER[IMG_Animation]], SDL_IMAGE_BINARY]
IMG_LoadGIFAnimation_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadGIFAnimation_IO", SDL_POINTER[IMG_Animation], [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]
IMG_LoadWEBPAnimation_IO: abc.Callable[..., typing.Any] = SDL_FUNC["IMG_LoadWEBPAnimation_IO", SDL_POINTER[IMG_Animation], [SDL_POINTER[SDL_IOStream]], SDL_IMAGE_BINARY]