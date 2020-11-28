#include "fsemu-internal.h"
#include "fsemu-frame.h"

#include "fsemu-action.h"
#include "fsemu-audio.h"
// #include "fsemu-control.h"
#include "fsemu-input.h"
#include "fsemu-option.h"
#include "fsemu-thread.h"
#include "fsemu-time.h"
#include "fsemu-util.h"
#include "fsemu-video.h"

static struct {
    bool initialized;
    // Will not overflow for over a year @60 Hz
    double rate_hz;
    int counter;
    int emulation_duration;
    int render_duration;
    // Helper variable to store time for duration registration
    int64_t timer;

    // Emulation is paused, as seen from the emulation thread. Main thread
    // has a different pause state variable which may be slightly out of sync.
    bool paused;
    bool warping;
    int load_state;
    int save_state;
} fsemu_frame;

double fsemu_frame_hz = 0;

// FIXME: Main thread thing? remove?
// bool fsemu_frame_warp = 0;

int64_t fsemu_frame_epoch_at = 0;
int64_t fsemu_frame_origin_at = 0;

int64_t fsemu_frame_begin_at = 0;
int64_t fsemu_frame_end_at = 0;

int64_t fsemu_frame_overshoot_duration = 0;
int64_t fsemu_frame_wait_duration = 0;
int64_t fsemu_frame_emu_duration = 0;
int64_t fsemu_frame_sleep_duration = 0;
int64_t fsemu_frame_extra_duration = 0;
int64_t fsemu_frame_gui_duration = 0;
int64_t fsemu_frame_render_duration = 0;

int fsemu_frame_log_level = 1;

void fsemu_frame_init(void)
{
    fsemu_init_once(&fsemu_frame.initialized);
    fsemu_frame_log("Init\n");

    fsemu_frame_log_level = fsemu_option_int_default(FSEMU_OPTION_LOG_FRAME,
                                                     fsemu_frame_log_level);

    fsemu_frame.load_state = -1;
    fsemu_frame.save_state = -1;

    // fsemu_frame.emulation_duration = 10000;
    // fsemu_frame.render_duration = 5000;

    // FIXME: When using a shorter emulation duration, Mednafen needs
    // a larger audio buffer to avoid underruns. Check why. Probably
    // something to do with how/when buffer fill is calculated.
}

bool fsemu_frame_check_load_state(int *slot)
{
    if (fsemu_frame.load_state != -1) {
        *slot = fsemu_frame.load_state;
        fsemu_frame.load_state = -1;
        return true;
    } else {
        return false;
    }
}

bool fsemu_frame_check_save_state(int *slot)
{
    if (fsemu_frame.save_state != -1) {
        *slot = fsemu_frame.save_state;
        fsemu_frame.save_state = -1;
        return true;
    } else {
        return false;
    }
}

bool fsemu_frame_paused(void)
{
    fsemu_thread_assert_emu();

    return fsemu_frame.paused;
}

bool fsemu_frame_warping(void)
{
    fsemu_thread_assert_emu();

    return fsemu_frame.warping;
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
    fsemu_thread_assert_emu();
    fsemu_frame_log_epoch("Frame end\n");
    fsemu_assert(fsemu_frame.initialized);

    // Note; this is probably overriden in fsemu_frame_update_timing now.
    fsemu_frame_reset_epoch();

    fsemu_audio_end_frame();
    fsemu_video_end_frame();

    // Advance the frame counter after updating stats for the current frame
    // (and possibly clearing status for the next frame).
    fsemu_frame.counter += 1;
    // fsemu_frame_log("Advanced frame counter to %d\n", fsemu_frame.counter);

    // Reset duration counters
    fsemu_frame_overshoot_duration = 0;
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
    fsemu_thread_assert_emu();

    int64_t now = t ? t : fsemu_time_us();
    // Register diff time to unknown duration? Probably not necessary, since
    // this will be caught and registered as "other".
    fsemu_frame.timer = now;
}

// FIXME: Maybe make inline function in header for performance
void fsemu_frame_add_overshoot_time(int64_t t)
{
    fsemu_thread_assert_emu();

    int64_t now = t ? t : fsemu_time_us();
    fsemu_frame_overshoot_duration += now - fsemu_frame.timer;
    fsemu_frame.timer = now;
}

// FIXME: Maybe make inline function in header for performance
void fsemu_frame_add_framewait_time(int64_t t)
{
    fsemu_thread_assert_emu();

    int64_t now = t ? t : fsemu_time_us();
    fsemu_frame_wait_duration += now - fsemu_frame.timer;
    fsemu_frame.timer = now;
}

// FIXME: Maybe make inline function in header for performance
void fsemu_frame_add_gui_time(int64_t t)
{
    fsemu_thread_assert_emu();

    int64_t now = t ? t : fsemu_time_us();
    fsemu_frame_gui_duration += now - fsemu_frame.timer;
    fsemu_frame.timer = now;
}

// FIXME: Maybe make inline function in header for performance
void fsemu_frame_add_emulation_time(int64_t t)
{
    fsemu_thread_assert_emu();

    int64_t now = t ? t : fsemu_time_us();
    fsemu_frame_emu_duration += now - fsemu_frame.timer;
    fsemu_frame.timer = now;
}

// FIXME: Maybe make inline function in header for performance
void fsemu_frame_add_render_time(int64_t t)
{
    fsemu_thread_assert_emu();

    int64_t now = t ? t : fsemu_time_us();
    fsemu_frame_render_duration += now - fsemu_frame.timer;
    fsemu_frame.timer = now;
}

// FIXME: Maybe make inline function in header for performance
void fsemu_frame_add_sleep_time(int64_t t)
{
    fsemu_thread_assert_emu();

    int64_t now = t ? t : fsemu_time_us();
    fsemu_frame_sleep_duration += now - fsemu_frame.timer;
    fsemu_frame.timer = now;
}

// FIXME: Maybe make inline function in header for performance
void fsemu_frame_add_extra_time(int64_t t)
{
    fsemu_thread_assert_emu();

    int64_t now = t ? t : fsemu_time_us();
    fsemu_frame_extra_duration += now - fsemu_frame.timer;
    fsemu_frame.timer = now;
}

// Affects when the frame starts emulating.
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

static int64_t fsemu_frame_framewait(double hz)
{
    return 0;
}

// FIXME: WHEN USING TURBO MODE, FRAME STATS GETS CONFUSED / PROBABLY RELATED
// TO FRAME COUNTER DRIFING AWAY AND RING BUFFER GETTING WRONG POSITION

static void fsemu_frame_update_timing_timer_based(double hz)
{
    static int late_frames;

    int64_t now = fsemu_time_us();
    if (fsemu_frame_origin_at == 0) {
        fsemu_frame_origin_at = now;
    } else {
        fsemu_frame_origin_at += (1000000 / hz);
    }

    if ((now - fsemu_frame_origin_at) > (1000000 / 2 / hz)) {
        // More than half a frame has already passed since origin. Assume
        // that we are behind scedule and might need to catch up. We avoid
        // doing it immediately since we might be able to catch up, thus
        // keeping the correct average frame rate.
        late_frames += 1;
    } else {
        late_frames = 0;
    }

    if (late_frames >= 10) {
        // We might not be able to catch up normally, so we reset origin.
        fsemu_frame_origin_at = now;
        printf("\n--------------------------------\n\n");
        // FIXME: REGISTER THIS AS AN UNDERRUN EVENT
    }

    fsemu_frame_end_at = fsemu_frame_origin_at + (1000000 / hz);

    fsemu_frame_begin_at = fsemu_frame_origin_at + fsemu_frame_framewait(hz);
    if (fsemu_frame_begin_at < now) {
        fsemu_frame_begin_at = now;
    }

    if (fsemu_frame_warping() && !fsemu_frame_paused()) {
        // Without this, we will get too "far behind" on time.
        fsemu_frame_origin_at = now;
        fsemu_frame_begin_at = now;
        fsemu_frame_end_at = now;
    }

    fsemu_frame_epoch_at = fsemu_frame_origin_at;
}

static void fsemu_frame_update_timing_vsync_based(double hz)
{
}

void fsemu_frame_update_timing(double hz, bool turbo)
{
    fsemu_thread_assert_emu();

    static double last_hz;
    if (hz != last_hz) {
        fsemu_log("[FSEMU] Emulation frame rate %f Hz\n", hz);
        last_hz = hz;
    }

    fsemu_frame_hz = hz;
    // fsemu_frame_warp = fsemu_control_warp();

    if (fsemu_video_vsync()) {
        fsemu_frame_update_timing_vsync_based(hz);
    } else {
        fsemu_frame_update_timing_timer_based(hz);
    }

    if (fsemu_frame_log_level >= 2) {
        printf(
            "-----------------------------------------------------------------"
            "--------------\n");
        fflush(stdout);
    }
    fsemu_frame_log_epoch("Frame %16d        ends at %16lld (+%d)\n",
                          fsemu_frame.counter,
                          (long long) fsemu_frame_end_at,
                          (int) (fsemu_frame_end_at - fsemu_frame_epoch_at));
    // fsemu_frame_log_trace("Frame ends at %lld (+%d)\n",
    //                       (long long) fsemu_frame_end_at,
    //                       (int) (fsemu_frame_end_at -
    //                       fsemu_frame_epoch_at));

#if 0


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

#if 0
    // FIXME: Turbo mode should be read from FSEMU instead?
    if (turbo) {
        // Without this, we will get too "far behind" on time.
        target = now;
    }
#endif

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
#endif
}

static void fsemu_frame_start_handle_command(fsemu_action_t action,
                                             fsemu_action_state_t state)
{
    switch (action) {
        case FSEMU_ACTION_PAUSE_DISABLE:
            if (state) {
                fsemu_frame_log("FSEMU_ACTION_PAUSE_DISABLE\n");
                printf("FSEMU_ACTION_PAUSE_DISABLE\n");
                fsemu_frame.paused = false;
            }
            break;

        case FSEMU_ACTION_PAUSE_ENABLE:
            if (state) {
                fsemu_frame_log("FSEMU_ACTION_PAUSE_ENABLE\n");
                printf("FSEMU_ACTION_PAUSE_ENABLE\n");
                fsemu_frame.paused = true;
            }
            break;

        case FSEMU_ACTION_WARP_DISABLE:
            if (state) {
                fsemu_frame_log("FSEMU_ACTION_WARP_DISABLE\n");
                printf("FSEMU_ACTION_WARP_DISABLE\n");
                fsemu_frame.warping = false;
            }
            break;

        case FSEMU_ACTION_WARP_ENABLE:
            if (state) {
                fsemu_frame_log("FSEMU_ACTION_WARP_ENABLE\n");
                printf("FSEMU_ACTION_WARP_ENABLE\n");
                fsemu_frame.warping = true;
            }
            break;

        case FSEMU_ACTION_LOADSTATE0:
        case FSEMU_ACTION_LOADSTATE1:
        case FSEMU_ACTION_LOADSTATE2:
        case FSEMU_ACTION_LOADSTATE3:
        case FSEMU_ACTION_LOADSTATE4:
        case FSEMU_ACTION_LOADSTATE5:
        case FSEMU_ACTION_LOADSTATE6:
        case FSEMU_ACTION_LOADSTATE7:
        case FSEMU_ACTION_LOADSTATE8:
        case FSEMU_ACTION_LOADSTATE9:
            if (state) {
                fsemu_frame.load_state = action - FSEMU_ACTION_LOADSTATE0;
            }
            break;

        case FSEMU_ACTION_SAVESTATE0:
        case FSEMU_ACTION_SAVESTATE1:
        case FSEMU_ACTION_SAVESTATE2:
        case FSEMU_ACTION_SAVESTATE3:
        case FSEMU_ACTION_SAVESTATE4:
        case FSEMU_ACTION_SAVESTATE5:
        case FSEMU_ACTION_SAVESTATE6:
        case FSEMU_ACTION_SAVESTATE7:
        case FSEMU_ACTION_SAVESTATE8:
        case FSEMU_ACTION_SAVESTATE9:
            if (state) {
                fsemu_frame.save_state = action - FSEMU_ACTION_SAVESTATE0;
            }
            break;
    }
}

static void fsemu_frame_start_handle_commands(void)
{
    fsemu_action_t action;
    fsemu_action_state_t state;
    while (fsemu_input_next_command(&action, &state)) {
        printf(" ----- fsemu frame command %04x %04x\n", action, state);
        fsemu_frame_start_handle_command(action, state);
    }
}

static void fsemu_frame_start_2(double hz)
{
    fsemu_frame.rate_hz = hz;

    // FIXME: update timing depends on warp mode, so handle commands first?
    fsemu_frame_start_handle_commands();

    fsemu_frame_update_timing(hz, false);
    // We have now a new value for the (ideal) origin time for the new frame.
    // We use this as basis for duration logging during the coming fram, and
    // start by registering time spent since (ideal) origin until now as
    // "overshoot" time.
    fsemu_frame_reset_timer(fsemu_frame_origin_at);
    fsemu_frame_add_overshoot_time(0);
    // fsemu_frame_reset_epoch() FIXME: Maybe reset epoch to same time here?
}

void fsemu_frame_start(double hz)
{
    fsemu_thread_assert_emu();

    fsemu_frame_start_2(hz);

    if (fsemu_frame_paused()) {
        do {
            // FIXME: Read and discard input events... ? Probably...
            // But consider how frame counting and events are handled for
            // netplay.
            // FIXME: NETPLAY

            // FIXME: AUDIO: Pause/resume audio as needed here

            fsemu_video_frame_t *frame = fsemu_video_alloc_frame();
            frame->dummy = true;
            fsemu_video_post_frame(frame);
            // fsemu_video_free_frame(frame);

            fsemu_time_wait_until_us(fsemu_frame_end_at);

            fsemu_frame_end();

            fsemu_frame_start_2(hz);
        } while (fsemu_frame_paused());
        printf("RESUME\n");
    }
}

double fsemu_frame_rate_hz(void)
{
    return fsemu_frame.rate_hz;
}
