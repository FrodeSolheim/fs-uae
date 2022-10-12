#if 1

#include "uae/maccess.h"
#include "sysdeps.h"

#else
#ifdef __ARM_EABI__
#include "machdep/arm/maccess.h"
#elif __ppc__
#include "machdep/ppc/maccess.h"
#elif defined(__i386__) || defined(__x86_64__)
#include "machdep/x86/maccess.h"
#else
#include "machdep/generic/maccess.h"
#endif
#endif

STATIC_INLINE uae_u32 do_byteswap_32(uae_u32 v)
{
	return bswap_32(v);
}

STATIC_INLINE uae_u16 do_byteswap_16(uae_u16 v)
{
	return bswap_16(v);
}
