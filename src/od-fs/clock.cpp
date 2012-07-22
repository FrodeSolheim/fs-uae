#include "sysconfig.h"
#include "sysdeps.h"

#include <time.h>
#include <fs/base.h>

int64_t g_uae_epoch = 0;

static struct tm *g_local_time;

struct tm *uae_get_amiga_time() {
    //printf("uae_get_amiga_time\n");

    if (uae_synchronous_mode()) {
        // FIXME: get synchronized clock here
        time_t t = 0;
#ifdef DEBUG_SYNC
        write_sync_log("uae_get_amiga_time: (fixed to %d)\n", 0);
#endif
        return gmtime(&t);
    }
    else {
        time_t t = time(NULL);
        //t += currprefs.cs_rtc_adjust;
        g_local_time = localtime (&t);
        return g_local_time;
    }
}
