#define FSEMU_INTERNAL
#include "fsemu-frame.h"

#include "fsemu-action.h"
#include "fsemu-audio.h"
#include "fsemu-control.h"
#include "fsemu-frameinfo.h"
#include "fsemu-hud.h"
#include "fsemu-input.h"
#include "fsemu-module.h"
#include "fsemu-option.h"
#include "fsemu-quit.h"
#include "fsemu-recording.h"
#include "fsemu-thread.h"
#include "fsemu-time.h"
#include "fsemu-util.h"
#include "fsemu-video.h"

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-Wconversion"

// ----------------------------------------------------------------------------

int fsemu_frame_log_level = FSEMU_LOG_LEVEL_INFO;

static struct {
    bool initialized;
    double rate_hz;
    // Will not overflow for over a year @60 Hz
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

    int quit_after_n_frames;

    double frame_rate_multiplier;
    bool busy_wait;
} fsemu_frame;

double fsemu_frame_hz = 0;

// FIXME: Main thread thing? remove?
// bool fsemu_frame_warp = 0;

// ----------------------------------------------------------------------------

volatile int fsemu_frame_number_began = -1;
volatile int fsemu_frame_number_ended = -1;
volatile int fsemu_frame_number_posted = -1;
volatile int fsemu_frame_number_rendering = -1;
volatile int fsemu_frame_number_rendered = -1;
volatile int fsemu_frame_number_displaying = -1;
volatile int fsemu_frame_number_displayed = -1;
// FIXME: Used?
volatile int fsemu_frame_number_swapped = -1;

// ----------------------------------------------------------------------------

int64_t fsemu_frame_epoch_at = 0;
int64_t fsemu_frame_origin_at = 0;

int64_t fsemu_frame_begin_at = 0;
int64_t fsemu_frame_end_at = 0;

// ----------------------------------------------------------------------------

int64_t fsemu_frame_overshoot_duration = 0;
int64_t fsemu_frame_wait_duration = 0;
int64_t fsemu_frame_emu_duration = 0;
int64_t fsemu_frame_sleep_duration = 0;
int64_t fsemu_frame_extra_duration = 0;
int64_t fsemu_frame_gui_duration = 0;
int64_t fsemu_frame_render_duration = 0;

// ----------------------------------------------------------------------------

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

void fsemu_frame_toggle_sleep_busywait(void)
{
    printf("fsemu_frame_toggle_sleep_busywait\n");
    fsemu_frame.busy_wait = !fsemu_frame.busy_wait;
    fsemu_hud_show_notification(
        14034819788300734,
        fsemu_frame.busy_wait ? "Busywaiting" : "Sleeping",
        NULL,
        NULL,
        FSEMU_HUD_NOTIFICATION_DEFAULT_DURATION);
}

void fsemu_frame_wait_until(int64_t until_us)
{
    int64_t now_us = fsemu_time_us();
    if (fsemu_frame.busy_wait) {
        while (now_us < until_us) {
            fsemu_time_mm_pause();
            now_us = fsemu_time_us();
        }
    } else {
        fsemu_time_sleep_until_us_2(until_us, now_us);
    }
}

void fsemu_frame_wait_until_frame_end(void)
{
    fsemu_frame_wait_until(fsemu_frame_end_at);
}

void fsemu_frame_end(void)
{
    fsemu_frame_log_trace("%s\n", __func__);
    fsemu_thread_assert_emu();
    fsemu_frame_log_epoch("Frame end\n");
    fsemu_assert(fsemu_frame.initialized);

    int64_t now = fsemu_time_us();
    if (now > fsemu_frame_end_at + 1000) {
        printf(
            "fsemu_frame_end called %d ms too late "
            "(now=%lld, fsemu_frame_end_at=%lld)\n",
            (int) (now - fsemu_frame_end_at) / 1000,
            lld(now),
            lld(fsemu_frame_end_at));
    } else if (now >= fsemu_frame_end_at) {
    } else {
        fsemu_frame_wait_until_frame_end();
        now = fsemu_time_us();
        if (now > fsemu_frame_end_at + 1000) {
            printf("Overslept %d\n",
                   (int) (fsemu_time_us() - fsemu_frame_end_at));
        }
        fsemu_frame_add_sleep_time(0);
    }

#if 0
    while (now < fsemu_frame_end_at) {
        now = fsemu_time_us();
    }
#endif
    // FSEMU_FRAMEINFO(fsemu_frame_number_began).ended_at = now;

    // fsemu_frameinfo_t *frameinfo =
    //     &fsemu_frameinfo[fsemu_frame_number_began % FSEMU_FRAMEINFO_COUNT];
    fsemu_frameinfo_t *frameinfo = &FSEMU_FRAMEINFO(fsemu_frame_number_began);

    frameinfo->frame_hz = fsemu_frame_hz;
    // stats->frame_warp = fsemu_frame_warp;
    frameinfo->frame_warp = fsemu_frame_warping();

    frameinfo->emu_us = (int) fsemu_frame_emu_duration;
    frameinfo->extra_us = (int) fsemu_frame_extra_duration;
    frameinfo->gui_us = (int) fsemu_frame_gui_duration;
    frameinfo->overshoot_us = (int) fsemu_frame_overshoot_duration;
    frameinfo->render_us = (int) fsemu_frame_render_duration;
    frameinfo->sleep_us = (int) fsemu_frame_sleep_duration;
    frameinfo->wait_us = (int) fsemu_frame_wait_duration;

    frameinfo->origin_at = fsemu_frame_origin_at;
    frameinfo->began_at = fsemu_frame_begin_at;
    frameinfo->ended_at = now;

#if 0
    // FIXME:
    if (last != 0) {
        frameinfo->other_us = (now - last) - stats->overshoot_us - stats->wait_us -
                          stats->emu_us - stats->sleep_us - stats->extra_us -
                          stats->gui_us - stats->render_us;
    }
#endif

    // Note; this is probably overriden in fsemu_frame_update_timing now.
    fsemu_frame_reset_epoch();

    fsemu_audio_end_frame();
    fsemu_video_end_frame();

    fsemu_recording_end_frame(fsemu_frame.counter);

    // Advance the frame counter after updating stats for the current frame
    // (and possibly clearing status for the next frame).
    fsemu_frame.counter += 1;

    static fsemu_mavgi_t emu_us_mavgi;
#define FSEMU_FRAME_MAX_FRAME_STATS 256
    static int emu_us_mavgi_values[FSEMU_FRAME_MAX_FRAME_STATS];
    int emu_us_avg = fsemu_mavgi(&emu_us_mavgi,
                                 emu_us_mavgi_values,
                                 FSEMU_FRAME_MAX_FRAME_STATS,
                                 (int) fsemu_frame_emu_duration);
    static int64_t emu_us_total;
    emu_us_total += fsemu_frame_emu_duration;

    static int64_t emu_us_avg_sum;
    static int emu_us_avg_count;
    static int emu_us_avg_max;

    if (fsemu_frame.counter % FSEMU_FRAME_MAX_FRAME_STATS == 0) {
        emu_us_avg_sum += emu_us_avg;
        emu_us_avg_count += 1;
    }
    if (emu_us_avg > emu_us_avg_max) {
        emu_us_avg_max = emu_us_avg;
    }

    // printf("Frame count %d\n", fsemu_frame.counter);
    if (fsemu_frame.counter == fsemu_frame.quit_after_n_frames) {
        printf(
            "----------------------------------------"
            "----------------------------------------\n");
        printf("Emu total: %0.3f s\n", (double) emu_us_total / 1000000.0);
        printf("Emu FPS (avg): %0.1f\n",
               1000000.0 / ((double) emu_us_avg_sum / emu_us_avg_count));
        printf("Emu FPS (avg) min: %0.1f\n",
               1000000.0 / (double) emu_us_avg_max);
        printf(
            "----------------------------------------"
            "----------------------------------------\n");
        printf(
            "Benchmark: %7.1f    Avg fps: %7.1f    "
            "Avg fps min: %7.1f\n",
            (double) emu_us_total / 1000000.0,
            1000000.0 / ((double) emu_us_avg_sum / emu_us_avg_count),
            1000000.0 / (double) emu_us_avg_max);
        fsemu_quit_maybe();
    }

    // fsemu_frame_log("Advanced frame counter to %d\n", fsemu_frame.counter);

    // Reset duration counters
    fsemu_frame_emu_duration = 0;
    fsemu_frame_extra_duration = 0;
    fsemu_frame_gui_duration = 0;
    fsemu_frame_overshoot_duration = 0;
    fsemu_frame_render_duration = 0;
    fsemu_frame_sleep_duration = 0;
    fsemu_frame_wait_duration = 0;

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

double fsemu_frame_rate_multiplier(void)
{
    return fsemu_frame.frame_rate_multiplier;
}

// FIXME: WHEN USING TURBO MODE, FRAME STATS GETS CONFUSED / PROBABLY RELATED
// TO FRAME COUNTER DRIFING AWAY AND RING BUFFER GETTING WRONG POSITION

static void fsemu_frame_update_timing_timer_based(int frame_number,
                                                  double hz,
                                                  int64_t frame_duration)
{
    int64_t now = fsemu_time_us();
    if (fsemu_frame_origin_at == 0) {
        fsemu_frame_origin_at = now;
    } else {
        fsemu_frame_origin_at += frame_duration;
    }

    static int late_frames;
    if ((now - fsemu_frame_origin_at) > frame_duration / 2) {
        // More than half a frame has already passed since origin. Assume
        // that we are behind scedule and might need to catch up. We avoid
        // doing it immediately since we might be able to catch up, thus
        // keeping the correct average frame rate.
        late_frames += 1;
    } else {
        late_frames = 0;
    }

#if 0
    if (late_frames >= 10) {
        // We might not be able to catch up normally, so we reset origin.
        fsemu_frame_origin_at = now;
        printf("\n--------------------------------\n\n");
        // FIXME: REGISTER THIS AS AN UNDERRUN EVENT
    }
#endif

    fsemu_frame_end_at = fsemu_frame_origin_at + frame_duration;

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

int64_t laaaast_vsync_at = 0;

static void fsemu_frame_update_timing_vsync_based(int frame_number,
                                                  double hz,
                                                  int64_t frame_duration)
{
    // hz = 50.0;
    // frame_duration = 20000;

    int64_t now = fsemu_time_us();

#if 1
    // fsemu_frame_origin_at = now;
    // fsemu_frame_begin_at = fsemu_frame_origin_at;
    // fsemu_frame_end_at = fsemu_frame_origin_at + frame_duration;
    // fsemu_frame_epoch_at = fsemu_frame_origin_at;

    // printf("FSEMU_FRAMEINFO(%d).vsync_allow_start_at == %lld\n",
    //        frame_number,
    //        lld(FSEMU_FRAMEINFO(frame_number).vsync_allow_start_at));
    fsemu_frame_origin_at = FSEMU_FRAMEINFO(frame_number).vsync_allow_start_at;
    fsemu_frame_end_at = fsemu_frame_origin_at + frame_duration;
    fsemu_frame_begin_at = fsemu_frame_origin_at;
    //

    if (fsemu_frame_warping() && !fsemu_frame_paused()) {
        // Without this, we will get too "far behind" on time.
        fsemu_frame_origin_at = now;
        fsemu_frame_begin_at = now;
        fsemu_frame_end_at = now;
    }

    fsemu_frame_epoch_at = fsemu_frame_origin_at;

    // fsemu_frame_begin_at
#else

    // static int late_frames;

    int frame = frame_number;

    int64_t last_vsync_at = 0;
    int64_t last = 0;
    int64_t avgsum = 0;
    int avgcnt = 0;
    for (int i = 0; i < 256; i++) {
        // FIXME: Maybe -2 since the previous frame is probably *not* done
        if (FSEMU_FRAMEINFO(frame - 1 - i).vsync_at >
            FSEMU_FRAMEINFO(frame - 1 - i).begins_at) {
            if (last_vsync_at == 0) {
                last_vsync_at = FSEMU_FRAMEINFO(frame - 1 - i).vsync_at;
                printf("vsync %lld\n", lld(last_vsync_at));
                last = last_vsync_at;
                // continue;
            }
            int64_t diff = FSEMU_FRAMEINFO(frame - 1 - i).vsync_at - last;
            last = FSEMU_FRAMEINFO(frame - 1 - i).vsync_at;
            int64_t mod =
                FSEMU_FRAMEINFO(frame - 1 - i).vsync_at % frame_duration;
            printf("vsync %lld ( -%lld ) mod %lld\n",
                   lld(FSEMU_FRAMEINFO(frame - 1 - i).vsync_at),
                   lld(diff),
                   lld(mod));
            avgsum += mod;
            avgcnt += 1;
        }
    }

    int64_t b = 0;
    if (avgcnt > 0) {
        b = avgsum / avgcnt;
    }
    printf("intercept %d\n", (int) b);

    int64_t a = 20000;
    int64_t next_vsync_at = b + (int) (0.5 + ((double) now - b) / a) * a;
    while (next_vsync_at - now < frame_duration * 1.0) {
        next_vsync_at += frame_duration;
    }
    printf("next vsync at %lld (+%lld)\n",
           lld(next_vsync_at),
           lld(next_vsync_at - last_vsync_at));

#if 0
    int avgsum = 0;
    int avgcnt = 0;
    for (int i = 0; i < 32; i++) {
        int diff = FSEMU_FRAMEINFO(frame - 1 - i).vsync_at -
                       FSEMU_FRAMEINFO(frame - 1 - i).begins_at;
        printf("-%d diff is %d\n", 1 + i, diff);
        // Diff can be negative if vsync did not happen
        if (diff > 0 && diff < 100000) {
            avgsum += diff;
            avgcnt += 1;
        }
    }
    int avgdiff = 0;
    if (avgcnt > 0) {
        avgdiff = avgsum / avgcnt;
    }
    printf("avgdiff is %d\n", avgdiff);

    avgsum = 0;
    avgcnt = 0;
    for (int i = 0; i < 32; i++) {
        int diff = FSEMU_FRAMEINFO(frame - 1 - i).vsync_at -
                       FSEMU_FRAMEINFO(frame - 1 - i).begins_at;
        if (diff > 0 && diff <= avgdiff) {
            avgsum += diff;
            avgcnt += 1;
        }
    }
    if (avgcnt > 0) {
        avgdiff = avgsum / avgcnt;
    }
    printf("avgdiff (adjusted) is %d\n", avgdiff);

    int wantavgdiff = frame_duration * 1.75;
#endif
#if 0
    if (fsemu_frame_origin_at == 0) {
        fsemu_frame_origin_at = now;
    } else {
        fsemu_frame_origin_at += frame_duration;
    }
#if 0
    if (avgdiff - wantavgdiff > 50) {
        fsemu_frame_origin_at += 50;
    } else if (avgdiff - wantavgdiff < 50) {
        fsemu_frame_origin_at -= 50;
    }
#endif
    // fsemu_frame_origin_at += (wantavgdiff - avgdiff);

    if ((now - fsemu_frame_origin_at) > frame_duration / 2) {
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
#endif

    FSEMU_FRAMEINFO(frame).vsync_estimated_at = next_vsync_at;

#if 0
    // int64_t now = fsemu_time_us();
    if (fsemu_frame_origin_at == 0) {
        fsemu_frame_origin_at = now;
    } else {
        fsemu_frame_origin_at += frame_duration;
    }

    int64_t origin_b = (b + 5000) % frame_duration;
    int64_t b1 = fsemu_frame_origin_at % frame_duration;
    // if (origin_b < b1) {
    //     origin_b += frame_duration;
    // }
    printf("intercept %d vs ideal %d\n", (int) b1, (int) origin_b);
    if (origin_b > b1 - 100) {
        fsemu_frame_origin_at += (origin_b - b1) / 100;
    }
    if (origin_b < b1 - 100) {
        fsemu_frame_origin_at -= (origin_b - b1) / 100;
    }

    // static int late_frames;
    // if ((now - fsemu_frame_origin_at) > frame_duration / 2) {
    //     // More than half a frame has already passed since origin. Assume
    //     // that we are behind scedule and might need to catch up. We avoid
    //     // doing it immediately since we might be able to catch up, thus
    //     // keeping the correct average frame rate.
    //     late_frames += 1;
    // } else {
    //     late_frames = 0;
    // }

    // if (late_frames >= 10) {
    //     // We might not be able to catch up normally, so we reset origin.
    //     fsemu_frame_origin_at += frame_duration;
    //     printf("\n--------------------------------\n\n");
    //     // FIXME: REGISTER THIS AS AN UNDERRUN EVENT
    // }

    while ((now - fsemu_frame_origin_at) > frame_duration / 2) {
        fsemu_frame_origin_at += frame_duration;
    }

    fsemu_frame_end_at = fsemu_frame_origin_at + frame_duration;

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
#endif

#if 0
    fsemu_frame_origin_at = next_vsync_at - frame_duration * 1.5;

    fsemu_frame_end_at = fsemu_frame_origin_at + frame_duration;

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
#endif
#if 0
    // This is called (indirectly) by fsemu_frame_start when vsync is enabled.

    // FIXME: Difference between epoch_at, origin_at and begin_at ?

    // This is the frame number of the currently starting frame.
    int frame = fsemu_frame_number_began;

    // FIXME: Calculate once in fsemu_frame_start and pass on?

    // For now; assume we need 1 frame duration to emulate + 0.5 frame
    // durations to render and display.

    // Find next vsync *at least* 1.5 frames ahead

    static int64_t last_used_vsync;

#if 0
    int64_t last_vsync_at = FSEMU_FRAMEINFO(frame - 1).vsync_at;
    if (now_us - last_vsync_at > 1000000) {
        // More than one second ago, maybe not done yet / wraparound?
        last_vsync_at = FSEMU_FRAMEINFO(frame - 2).vsync_at;
        if (now_us - last_vsync_at > 1000000) {
            last_vsync_at = FSEMU_FRAMEINFO(frame - 2).vsync_at;
            if (now_us - last_vsync_at > 1000000) {
                // Giving up
                last_vsync_at = now_us;
            }
        }
    }
#else
    int64_t last_vsync_at = laaaast_vsync_at;
#endif
    int64_t frame_duration = 1000000.0 / hz;
    int64_t next_vsync_at = last_vsync_at;
    int64_t now_us = fsemu_time_us();
    // int64_t next_vsync_after = now_us + frame_duration * 1.5;
#if 1
    // while (next_vsync_at < next_vsync_after) {
    //     next_vsync_at += frame_duration;
    // }
    if (next_vsync_at < now_us) {
        next_vsync_at =
            next_vsync_at +
            ((int) (0.5 + ((next_vsync_at - now_us) / frame_duration))) *
                frame_duration;
    }
    while (next_vsync_at <= last_used_vsync) {
        next_vsync_at += frame_duration;
    }
    last_used_vsync = next_vsync_at;

#else
    next_vsync_at =
        next_vsync_at +
        ((int) (0.5 + ((next_vsync_after - now_us) / frame_duration))) *
            frame_duration;
#endif
    fsemu_frame_origin_at = next_vsync_at - frame_duration * 1.5;

    // fsemu_frame_origin_at = now_us;

    // Find last v-sync...
    // fsemu_frameinfo[frame & FSEMU_FRAMEINFO_COUNT];
    // if (FSEMU_FRAMEINFO(frame - 1).vsync_at == now_us) {

    // }

    // fsemu_frame_origin_at = fsemu_video_vsync_time() + (1000000 / hz) * 1.2;
    // fsemu_frame_origin_at =
    //     fsemu_video_vsync_time() + (int64_t)(1000000 / hz * 0.2);
    fsemu_frame_begin_at = fsemu_frame_origin_at;
    fsemu_frame_end_at = fsemu_frame_begin_at + frame_duration;

    fsemu_frame_epoch_at = fsemu_frame_origin_at;

    // fsemu_frame_estimated_vsync_at = next_vsync_at;
    static int64_t last_next_vsync_at;

    printf("Estimated next vsync at %lld (+%lld)\n",
           lld(next_vsync_at),
           lld(next_vsync_at - last_next_vsync_at));
    last_next_vsync_at = next_vsync_at;

    FSEMU_FRAMEINFO(frame).vsync_estimated_at = next_vsync_at;
#endif
#endif
}

static void fsemu_frame_update_timing(int frame_number, double hz, bool turbo)
{
    fsemu_thread_assert_emu();

    static double last_hz;
    if (hz != last_hz) {
        fsemu_frame_log("Emulation frame rate is now %f Hz\n", hz);
        last_hz = hz;
    }

    // FIXME: This is emulation hz, not vsync hz, define what's correct
    fsemu_frame_hz = hz;
    // fsemu_frame_warp = fsemu_control_warp();

    int64_t frame_duration = (int64_t)(1000000.0 / hz);
    if (fsemu_video_vsync()) {
        double vsync_hz = fsemu_video_vsync_frequency();
        fsemu_frame.frame_rate_multiplier = vsync_hz / hz;
        fsemu_frame_update_timing_vsync_based(
            frame_number, vsync_hz, fsemu_video_vsync_interval());
    } else {
        fsemu_frame.frame_rate_multiplier = 1.0;
        fsemu_frame_update_timing_timer_based(
            frame_number, hz, frame_duration);
    }

    // if (fsemu_frame_log_level <= FSEMU_LOG_LEVEL_TRACE) {
    //     printf(
    //         "-----------------------------------------------------------------"
    //         "--------------\n");
    //     fflush(stdout);
    // }
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

extern volatile int allow_frame;

static int64_t fsemu_frame_can_start_frame_vsync(int frame_number)
{
    if (frame_number < 2) {
        return true;
    }

    int64_t vsync_at;
    if (FSEMU_FRAMEINFO(frame_number - 2).render_skipped) {
        vsync_at = FSEMU_FRAMEINFO(frame_number - 1).vsync_at;
    } else {
        vsync_at = FSEMU_FRAMEINFO(frame_number - 2).vsync_at;
    }
    if (vsync_at == 0) {
        return 0;
    }

    // FIXME: Don't need to set for each iteration,
    // FSEMU_FRAMEINFO(frame_number).vsync_allow_start_at = vsync_at;

    int64_t target = vsync_at + 9000;
    int64_t t = fsemu_time_us();
    // printf("now = %lld vsync_at = %lld (+%lld)\n",
    //        lld(t),
    //        lld(vsync_at),
    //        lld(t - vsync_at));
    if (t >= target) {
        return target;
    }
    return 0;
}

static void fsemu_frame_wait_until_frame_start_vsync(int frame_number)
{
    // FIXME: Allow choice between busy-wait and sleeping.
    // FIXME: If vsync time isn't available in
    // fsemu_frame_can_start_frame_vsync, maybe wait on a condition.
    int64_t origin_at;
    while ((origin_at = fsemu_frame_can_start_frame_vsync(frame_number)) ==
           0) {
        if (fsemu_quit_check()) {
            return;
        }
        // FIXME
        // fsemu_sleep_us(500);
    }
    FSEMU_FRAMEINFO(frame_number).vsync_allow_start_at = origin_at;
    // printf("FSEMU_FRAMEINFO(%d).vsync_allow_start_at = %lld\n",
    //        frame_number,
    //        lld(FSEMU_FRAMEINFO(frame_number).vsync_allow_start_at));

    fsemu_frame_add_sleep_time(0);
}

static void fsemu_frame_start_2(double hz)
{
    fsemu_frame.rate_hz = hz;

    // FIXME: Also for non-vsync we should make sure that we don't get more
    // video frames than we can display. So limiting to 2 ahead or something
    // makes sense there also.

    // FIXME: Use a condition to wait on?
    // (Plus also possibly sleep after allow start is known - for vsync)

    // Adding one here because we haven't increased fsemu_frame_number_began
    // yet.
    // while (allow_frame < fsemu_frame_number_began + 1) {
    //     fsemu_sleep_us(500);
    // }

    // FIXME: If vsync
    // FIXME: Not here?

    int frame_number = fsemu_frame_number_began += 1;

    fsemu_frameinfo_t *frameinfo = &FSEMU_FRAMEINFO(frame_number);
    memset(frameinfo, 0, sizeof(fsemu_frameinfo_t));

    static bool was_warping;
    if (fsemu_frame_warping()) {
        was_warping = true;
    } else {
        if (was_warping) {
            // Allow next two frames to start
            FSEMU_FRAMEINFO(frame_number + 0).vsync_allow_start_at = 1;
            FSEMU_FRAMEINFO(frame_number + 1).vsync_allow_start_at = 1;
        }
        was_warping = false;
    }

    if (fsemu_video_vsync() && !fsemu_frame_warping()) {
        fsemu_frame_wait_until_frame_start_vsync(frame_number);
        // while (!fsemu_frame_can_start_frame_vsync(frame_number)) {

        // }
#if 0
        // FIXME: Can we avoid these quit checks here?
        while (FSEMU_FRAMEINFO(fsemu_frame_number_began + 1)
                   .vsync_allow_start_at == 0) {
            fsemu_sleep_us(1000);
            if (fsemu_quit_check()) {
                break;
            }
        }
        if (!fsemu_quit_check() &&
            FSEMU_FRAMEINFO(fsemu_frame_number_began + 1)
                    .vsync_allow_start_at > fsemu_time_us()) {
            fsemu_time_sleep_until_us(
                FSEMU_FRAMEINFO(fsemu_frame_number_began + 1)
                    .vsync_allow_start_at);
        }
#endif
    }

    fsemu_frame_number_began = frame_number;

    // FIXME: update timing depends on warp mode, so handle commands first?
    fsemu_frame_start_handle_commands();

    fsemu_frame_update_timing(frame_number, hz, false);
    // We have now a new value for the (ideal) origin time for the new frame.
    // We use this as basis for duration logging during the coming frame, and
    // start by registering time spent since (ideal) origin until now as
    // "overshoot" time.

    FSEMU_FRAMEINFO(frame_number).begins_at = fsemu_frame_begin_at;
    FSEMU_FRAMEINFO(frame_number).ends_at = fsemu_frame_end_at;

    // FIXME: Clear future timers, e.g. vsync which might not happen
    // ...
    // FSEMU_FRAMEINFO(fsemu_frame_number_began).vsync_at = 0;
    // FSEMU_FRAMEINFO(fsemu_frame_number_began).render_skipped = false;

    // while (fsemu_time_us() < fsemu_frame_begin_at) {

    // }

    fsemu_frame_reset_timer(fsemu_frame_origin_at);

    // static int64_t last_origin_at;
    // printf("diff since last origin_at: %d  (%d)\n",
    //        (int) (fsemu_frame_origin_at - last_origin_at),
    //        (int) (fsemu_time_us() - fsemu_frame_origin_at));
    // last_origin_at = fsemu_frame_origin_at;

    fsemu_frame_add_overshoot_time(0);
    // fsemu_frame_reset_epoch() FIXME: Maybe reset epoch to same time here?
}

// FIXME: Move hz to a separate function that can be called before start?
// FIXME: Rename to fsemu_frame_begin?
void fsemu_frame_start(double hz)
{
    // printf("fsemu_frame_start hz=%f\n", hz);
    fsemu_frame_log_trace("%s hz=%f\n", __func__, hz);
    fsemu_thread_assert_emu();

    fsemu_frame_start_2(hz);
    // printf("START_OR_POST start %d ------------------\n",
    //        fsemu_frame_number_began);

#if 0
    static int first = true;
    if (first && fsemu_video_vsync()) {
        for (int i = 0; i < 100; i++) {
            printf("Rendering dummy vsync frame %d ending at %lld\n",
                   i,
                   lld(fsemu_frame_end_at));
            fsemu_video_frame_t *frame = fsemu_video_alloc_frame();
            frame->dummy = true;
            fsemu_video_post_frame(frame);

            fsemu_time_wait_until_us(fsemu_frame_end_at);
            fsemu_frame_end();
            fsemu_frame_start_2(hz);

            while (fsemu_frame_number_displayed != fsemu_frame_number_posted) {
            }
        }
    }
    first = false;
#endif

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

    // fsemu_recording_begin_frame(fsemu_frame.counter);
    fsemu_recording_begin_frame();
}

double fsemu_frame_rate_hz(void)
{
    return fsemu_frame.rate_hz;
}

// ----------------------------------------------------------------------------

static void fsemu_frame_quit(void)
{
}

// ----------------------------------------------------------------------------

void fsemu_frame_init(void)
{
    if (FSEMU_MODULE_INIT(frame)) {
        return;
    }

    fsemu_frame_log_level = fsemu_option_int_default(FSEMU_OPTION_LOG_FRAME,
                                                     fsemu_frame_log_level);

    fsemu_frame.quit_after_n_frames =
        fsemu_option_int_default(FSEMU_OPTION_QUIT_AFTER_N_FRAMES, -1);

    fsemu_frame.load_state = -1;
    fsemu_frame.save_state = -1;

    fsemu_frame.busy_wait = true;

    // Allow first two frames to start
    FSEMU_FRAMEINFO(0).vsync_allow_start_at = 1;
    FSEMU_FRAMEINFO(1).vsync_allow_start_at = 1;

    // fsemu_frame.emulation_duration = 10000;
    // fsemu_frame.render_duration = 5000;

    // FIXME: When using a shorter emulation duration, Mednafen needs
    // a larger audio buffer to avoid underruns. Check why. Probably
    // something to do with how/when buffer fill is calculated.
}

// ----------------------------------------------------------------------------

#pragma GCC diagnostic pop
