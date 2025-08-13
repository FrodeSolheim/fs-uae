def SDL_MostSignificantBitIndex32(x: int) -> int:
    if x == 0: return -1
    b: list[int] = [0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000]
    S: list[int] = [1, 2, 4, 8, 16]
    msbIndex: int = 0

    for i in range(4, -1, -1):
        if x & b[i]:
            x >>= S[i]
            msbIndex |= S[i]

    return msbIndex

def SDL_HasExactlyOneBitSet32(x: int) -> bool:
    if x and not (x & (x - 1)): return True
    else: return False