#include "sysconfig.h"
#include "sysdeps.h"

#include "uae.h"
#include "xwin.h"

static uae_u64 spincount;

#warning target_spin not really implemented
void target_spin(int total)
{
    UAE_LOG_STUB_MAX(1, "");
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
