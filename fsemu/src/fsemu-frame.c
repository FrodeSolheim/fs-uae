#define FSEMU_INTERNAL
#include "fsemu-frame.h"

#include "fsemu-audio.h"
#include "fsemu-control.h"
#include "fsemu-option.h"
#include "fsemu-options.h"
#include "fsemu-time.h"
#include "fsemu-util.h"
#include "fsemu-video.h"

static struct {
    bool initialized;
    // Will not overflow for over a year @60 Hz
    int counter;
    int emulation_duration;
    int render_duration;
    // Helper variable to store time for duration registration
    int64_t timer;
} fsemu_frame;

int64_t fsemu_frame_origin_at = 0;
int64_t fsemu_frame_begin_at = 0;
int64_t fsemu_frame_end_at = 0;
int64_t fsemu_frame_wait_duration = 0;
int64_t fsemu_frame_emu_duration = 0;
int64_t fsemu_frame_sleep_duration = 0;
int64_t fsemu_frame_extra_duration = 0;

int64_t fsemu_frame_gui_duration;
int64_t fsemu_frame_render_duration;

int64_t fsemu_frame_epoch_at = 0;

int fsemu_frame_log_level = 1;

void fsemu_frame_init(void)
{
    fsemu_init_once(&fsemu_frame.initialized);
    fsemu_frame_log("Init\n");

    fsemu_option_read_int(FSEMU_OPTION_LOG_FRAME, &fsemu_frame_log_level);

    // fsemu_frame.emulation_duration = 10000;
    // fsemu_frame.render_duration = 5000;

    // FIXME: When using a shorter emulation duration, Mednafen needs
    // a larger audio buffer to avoid underruns. Check why. Probably
    // something to do with how/when buffer fill is calculated.
}

int64_t fsemu_frame_epoch(void)
{
    return fsemu_frame_epoch_at;
}

void fsemu_frame_reset_epoch(void)
{
    fsemu_frame_epoch_at = fsemu_time_us();
}

void fsemu_frame_end(void)
{
    fsemu_assert(fsemu_frame.initialized);

    fsemu_frame_reset_epoch();

    fsemu_audio_end_frame();
    fsemu_video_end_frame();

    // Advance the frame counter after updating stats for the current frame
    // (and possibly clearing status for the next frame).
    fsemu_frame.counter += 1;
    // fsemu_frame_log("Advanced frame counter to %d\n", fsemu_frame.counter);

    // Reset duration counters
    fsemu_frame_wait_duration = 0;
    fsemu_frame_emu_duration = 0;
    fsemu_frame_sleep_duration = 0;
    fsemu_frame_extra_duration = 0;
    fsemu_frame_gui_duration = 0;
    fsemu_frame_render_duration = 0;

    // Reset duration timer so we can begin calling the add_*_timer functions
    // without having to call reset timer after calling fsemu_frame_end.
    fsemu_frame.timer = fsemu_time_us();
}

void fsemu_frame_reset_timer(int64_t t)
{
    int64_t now = t ? t : fsemu_time_us();
    // Register diff time to unknown duration? Probably not necessary, since
    // this will be caught and registered as "other".
    fsemu_frame.timer = now;
}

void fsemu_frame_add_framewait_time(int64_t t)
{
    int64_t now = t ? t : fsemu_time_us();
    fsemu_frame_wait_duration += now - fsemu_frame.timer;
    fsemu_frame.timer = now;
}

void fsemu_frame_add_emulation_time(int64_t t)
{
    int64_t now = t ? t : fsemu_time_us();
    fsemu_frame_emu_duration += now - fsemu_frame.timer;
    fsemu_frame.timer = now;
}

void fsemu_frame_add_sleep_time(int64_t t)
{
    int64_t now = t ? t : fsemu_time_us();
    fsemu_frame_sleep_duration += now - fsemu_frame.timer;
    fsemu_frame.timer = now;
}

void fsemu_frame_add_render_time(int64_t t)
{
    int64_t now = t ? t : fsemu_time_us();
    fsemu_frame_render_duration += now - fsemu_frame.timer;
    fsemu_frame.timer = now;
}

void fsemu_frame_add_gui_time(int64_t t)
{
    int64_t now = t ? t : fsemu_time_us();
    fsemu_frame_gui_duration += now - fsemu_frame.timer;
    fsemu_frame.timer = now;
}

void fsemu_frame_add_extra_time(int64_t t)
{
    int64_t now = t ? t : fsemu_time_us();
    fsemu_frame_extra_duration += now - fsemu_frame.timer;
    fsemu_frame.timer = now;
}

/** Affects when the frame starts emulating. */
void fsemu_frame_set_emulation_duration_us(int emulation_duration_us)
{
    fsemu_frame_log("Emulation duration set to %d us\n",
                    emulation_duration_us);
    fsemu_frame.emulation_duration = emulation_duration_us;
}

void fsemu_frame_set_render_duration_us(int render_duration_us)
{
    fsemu_frame_log("Render duration set to %d us\n", render_duration_us);
    fsemu_frame.render_duration = render_duration_us;
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

#if 0
    if (fsemu_video_vsync()) {
        // FIXME: Right now, assumes host hz equals game hz
        double host_hz = hz;

        if (target == 0) {
            // target = fsemu_time_us();
            target = now;
        }

        target = target + (1000000 / hz);

        int64_t next_vsync_at = fsemu_video_vsync_time();
        next_vsync_at += (1000000 / host_hz);
        if (next_vsync_at < now) {
            printf("WARN: next_vsync_at < now!\n");
            next_vsync_at = now;
        }
        int64_t target_vsync_at = next_vsync_at + (1000000 / host_hz);
        // Subtract half a frame
        target = target_vsync_at - (1000000 / 2 / host_hz);

    } else {
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
    }
#endif

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

    if (fsemu_video_vsync()) {
        // FIXME: We weed to smooth the vsync times, so we can calculate
        // a realistic future vsync target time.

        // FIXME: Right now, assumes host hz equals game hz
        double host_hz = hz;

        int64_t target_vsync_at = fsemu_video_vsync_time();
        if (target_vsync_at < target - 1000000) {
            printf("WARN: target_vsync_at too far behind!\n");
            // target_vsync_at = target;
        } else {
            while (target_vsync_at < target) {
                target_vsync_at += (1000000 / host_hz);
            }
            // Subtract half a frame
            // target = target_vsync_at - (1000000 / 2 / host_hz);
            int render_duration = fsemu_frame.render_duration;
            if (!render_duration) {
                render_duration = 5000;
            }
            target = target_vsync_at - render_duration;
        }
    }

#if 0
    // FIXME: Maybe
    if (turbo) {
        // Without this, we will get too "far behind" on time.
        target = now;
    }
#endif

    int frame_duration = fsemu_frame.emulation_duration;
    if (!frame_duration) {
        frame_duration = (1000000 / hz);
    }

    fsemu_frame_end_at = target;
    // fsemu_frame_begin_at = now;
    // FIXME: Make sure FS-UAE is actually waiting for fsemu_frame_begin_at
    // To make sure emulation duration is respected
    fsemu_frame_begin_at = fsemu_frame_end_at - frame_duration;

    // FIXME: Set to previous target instead?
    fsemu_frame_origin_at = fsemu_frame_end_at - (1000000 / hz);

    if (fsemu_control_warp()) {
        // Without this, we will get too "far behind" on time.
        target = now;
        fsemu_frame_origin_at = now;
        fsemu_frame_begin_at = now;
        fsemu_frame_end_at = now;
    }

    if (fsemu_frame_log_level >= 2) {
        printf("\n");
    }
    fsemu_frame_log_trace("%d\n", fsemu_frame.counter);
    fsemu_frame_log_trace("Frame ends at %lld (+%d)\n",
                          (long long) fsemu_frame_end_at,
                          (int) (fsemu_frame_end_at - fsemu_frame_epoch_at));
}
