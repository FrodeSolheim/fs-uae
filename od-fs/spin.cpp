#include "sysconfig.h"
#include "sysdeps.h"

#include "uae.h"
#include "xwin.h"

static uae_u64 spincount;

void target_spin(int total)
{
    STUB("");
#if 0
	if (!spincount)
		return;
	if (total > 10)
		total = 10;
	while (total-- >= 0) {
		uae_u64 v1 = __rdtsc();
		v1 += spincount;
		while (v1 > __rdtsc());
	}
#endif
}
