#ifndef FSEMU_FRAMEINFO_H_
#define FSEMU_FRAMEINFO_H_

#include <stdint.h>

#include "fsemu-config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FSEMU_FRAMEINFO_COUNT 256

typedef struct {
    double frame_hz;
    bool frame_warp;

    int overshoot_us;
    int wait_us;
    int gui_us;  // Only used when emulation thread == video thread
    int emu_us;
    int render_us;  // Only used when emulation thread == video thread
    int sleep_us;
    int extra_us;
    int other_us;

    volatile int64_t vsync_allow_start_at;

    int64_t origin_at;

    int64_t begins_at;

    int64_t began_at;

    // When the frame is scheduled to end
    int64_t ends_at;
    // When the frame actully ended
    int64_t ended_at;

    int64_t rendered_at;
    int64_t swapped_at;
    int64_t vsync_estimated_at;
    int64_t vsync_at;

    bool render_skipped;

} fsemu_frameinfo_t;

extern fsemu_frameinfo_t fsemu_frameinfo[FSEMU_FRAMEINFO_COUNT];

#define FSEMU_FRAMEINFO(frame) \
    (fsemu_frameinfo[(frame) % FSEMU_FRAMEINFO_COUNT])

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_FRAMEINFO_H_
