import sys, array

SDL_LIL_ENDIAN, SDL_BIG_ENDIAN = 1234, 4321
SDL_BYTEORDER: int = {"little": SDL_LIL_ENDIAN, "big": SDL_BIG_ENDIAN}[sys.byteorder]

def SDL_Swap16(x: int) -> int:
    return ((x << 8) & 0xFF00) | ((x >> 8) & 0x00FF)

def SDL_Swap32(x: int) -> int:
    return ((x << 24) & 0xFF000000) | ((x << 8) & 0x00FF0000) | ((x >> 8) & 0x0000FF00) | ((x >> 24) & 0x000000FF)

def SDL_Swap64(x: int) -> int:
    return (SDL_Swap32(x & 0xFFFFFFFF) << 32) | (SDL_Swap32(x >> 32 & 0xFFFFFFFF))

def SDL_SwapFloat(x: float) -> float:
    y = array.array("d", (x, ))
    y.byteswap(); return y[0]

if SDL_BYTEORDER == SDL_LIL_ENDIAN:
    SDL_SwapBE16, SDL_SwapBE32, SDL_SwapBE64, SDL_SwapFloatBE = SDL_Swap16, SDL_Swap32, SDL_Swap64, SDL_SwapFloat
    SDL_SwapLE16 = SDL_SwapLE32 = SDL_SwapLE64 = SDL_SwapFloatLE = lambda x: x

else:
    SDL_SwapLE16, SDL_SwapLE32, SDL_SwapLE64, SDL_SwapFloatLE = SDL_Swap16, SDL_Swap32, SDL_Swap64, SDL_SwapFloat
    SDL_SwapBE16 = SDL_SwapBE32 = SDL_SwapBE64 = SDL_SwapFloatBE = lambda x: x