#ifndef MACHDEP_RPT_H
#define MACHDEP_RPT_H

#include <machdep/machdep.h>
#include <fs/base.h>

extern int64_t g_uae_epoch;

static inline frame_time_t read_processor_time() {
    // with frame_time_t being int, the value will wrap around in
    // about 23 days..
    return (fs_get_monotonic_time() - g_uae_epoch);
    //return (fs_get_monotonic_time() - g_uae_epoch) / 100;
}

#endif // MACHDEP_RPT_H
