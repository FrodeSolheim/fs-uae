#define FSEMU_INTERNAL
#include "fsemu/fsemu-frame.h"

#include "fsemu/fsemu-audio.h"
#include "fsemu/fsemu-time.h"
#include "fsemu/fsemu-util.h"
#include "fsemu/fsemu-video.h"

static struct {
    bool initialized;
    // Will not overflow for over a year @60 Hz
    int counter;
} fsemu_frame;

int64_t fsemu_frame_begin_at = 0;
int64_t fsemu_frame_end_at = 0;
int64_t fsemu_frame_wait_duration = 0;
int64_t fsemu_frame_emu_duration = 0;
int64_t fsemu_frame_sleep_duration = 0;
int64_t fsemu_frame_extra_duration = 0;

void fsemu_frame_init(void)
{
    fsemu_init_once(&fsemu_frame.initialized);
    fsemu_frame_log("Init\n");
}

void fsemu_frame_end(void)
{
    fsemu_assert(fsemu_frame.initialized);

    fsemu_audio_end_frame();
    fsemu_video_end_frame();

    // Advance the frame counter after updating stats for the current frame
    fsemu_frame.counter += 1;
    fsemu_frame_log("Advanced frame counter to %d\n", fsemu_frame.counter);
}

int fsemu_frame_counter(void)
{
    return fsemu_frame.counter;
}

int fsemu_frame_counter_mod(int modulus)
{
    return fsemu_frame.counter % modulus;
}

// FIXME: WHEN USING TURBO MODE, FRAME STATS GETS CONFUSED / PROBABLY RELATED
// TO FRAME COUNTER DRIFING AWAY AND RING BUFFER GETTING WRONG POSITION

void fsemu_frame_update_timing(double hz, bool turbo)
{
    static int64_t target = 0;
    int64_t now = fsemu_time_us();
    if (target == 0) {
        // target = fsemu_time_us();
        target = now;
    }
    if (turbo) {
        // Without this, we will get too "far behind" on time.
        target = now;
    }
    target = target + (1000000 / hz);
    if (target < now + (1000000 / hz) / 2) {
        target = now + (1000000 / hz);
    }

    fsemu_frame_end_at = target;
    // fsemu_frame_begin_at = now;
    fsemu_frame_begin_at = fsemu_frame_end_at - (1000000 / hz);

    fsemu_frame_wait_duration = 0;
    fsemu_frame_emu_duration = 0;
    fsemu_frame_sleep_duration = 0;
    fsemu_frame_extra_duration = 0;
}
