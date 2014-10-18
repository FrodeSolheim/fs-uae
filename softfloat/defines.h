#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>

typedef int8_t Bit8s;
typedef int16_t Bit16s;
typedef int32_t Bit32s;
typedef int64_t Bit64s;

typedef uint8_t Bit8u;
typedef uint16_t Bit16u;
typedef uint32_t Bit32u;
typedef uint64_t Bit64u;

#define BX_BIG_ENDIAN WORDS_BIGENDIAN
#define BX_CONST64(x) (x##LL)
#define BX_CPP_INLINE inline
