#include "sysconfig.h"
#include "sysdeps.h"

#include "uae.h"
#include "uae/fs.h"

#include <time.h>
#include <fs/base.h>
#include <fs/time.h>

int64_t g_uae_epoch = 0;

static struct tm g_amiga_time;

// using a global variable, this function should be access only
// from the emulation thread

struct tm *uae_get_amiga_time() {
    //printf("uae_get_amiga_time\n");

    if (uae_deterministic_mode()) {
        // FIXME: get synchronized clock here
        time_t t = 0;
#ifdef DEBUG_SYNC
        write_sync_log("uae_get_amiga_time: (fixed to %d)\n", 0);
#endif
        fs_gmtime_r(&t, &g_amiga_time);
    }
    else {
        time_t t = time(NULL);
        //t += currprefs.cs_rtc_adjust;
        fs_localtime_r(&t, &g_amiga_time);
        //printf("returning year %d\n", g_local_time->tm_year);
    }
    return &g_amiga_time;
}

// added for WinUAE 2.5.0 beta 21 merge

uae_u32 getlocaltime (void)
{
    // check src/od-win32/posixemu.cpp
    STUB("");
    return 0;
}
