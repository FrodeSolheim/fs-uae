#define FSEMU_INTERNAL
#include "fsemu-frame.h"

#include <stdio.h>

#include "fsapp-time.h"
#include "fs/trace.h"
#include "fsapp-util.h"
#include "fsemu-action.h"
#include "fsemu-audio.h"
#include "fsemu-control.h"
#include "fsemu-emuthread.h"
#include "fsemu-follow.h"
#include "fsemu-frameinfo.h"
#include "fsemu-input.h"
#include "fsemu-mainloop.h"
#include "fsemu-module.h"
#include "fsemu-option.h"
#include "fsemu-perfgui.h"
#include "fsemu-quit.h"
#include "fsemu-recording.h"
#include "fsemu-slotplanner.h"
#include "fsemu-stripe.h"
#include "fsemu-video.h"

#include <atomic>
#include <math.h>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-Wconversion"

// ----------------------------------------------------------------------------

int fsemu_frame_log_level = SDL_LOG_PRIORITY_INFO;

// How many frames the average emulation duration is over.
#define FSEMU_FRAME_AVERAGE_FRAMES 256

static struct {
    bool initialized;
    // The frame rate given to fsemu_frame_start (emulation thread), also read by other threads
    // (fsemu_frame_rate_hz: the pacer's wait and the performance graph on the drawing side), so
    // written and read there under rate_lock.
    double rate_hz;
    SDL_SpinLock rate_lock;
    // Will not overflow for over a year @60 Hz. Counted by the emulation thread at each frame's
    // end, and read by other threads (the audio callback's statistics), so atomic.
    SDL_AtomicInt counter;
    int64_t emulation_duration_ns;
    int64_t render_duration_ns;
    // Helper variable to store time for duration registration
    int64_t timer_ns;

    // Emulation is paused, as seen from the emulation thread. Main thread
    // has a different pause state variable which may be slightly out of sync.
    bool paused;

    bool warping;
    // The core's own warp mode (fsemu_frame_set_core_warping).
    bool core_warping;
    int load_state;
    int save_state;

    int quit_after_n_frames;

    // The moving average of the frames' emulation durations, over the last
    // FSEMU_FRAME_AVERAGE_FRAMES frames (emulation thread).
    fsapp_util_moving_average_t emulation_average;
    int64_t emulation_average_values_ns[FSEMU_FRAME_AVERAGE_FRAMES];
    int64_t average_emulation_duration_ns;

    double frame_rate_multiplier;
    // The rate of fsemu's timeline for the current frame (Hz): the emulated frame rate, or the host
    // refresh rate when following the host vertical blank (emulation thread).
    double timeline_rate_hz;
    bool busy_wait;

    // Whether the start gate placed the current frame's planned start from the predicted host
    // vertical blank (else from the host vertical blank or swap two frames back, or not at all),
    // and whether the gate did so last time, to log when that changes (emulation thread).
    bool planned_start_predicted;
    bool planned_starts_were_predicted;

    // Whether the session's first frame has started (emulation thread).
    bool session_started;

    // While the emulation follows the host vertical blank, the slot planner plans the frames' slots
    // once the predicted host vertical blank has enough observations (fsemu_frame_plan_slot):
    // whether the current frame's slot was planned, and the slot; whether the frame before was
    // planned (else the planning starts afresh), and whether slots were planned when that was last
    // logged (emulation thread).
    fsemu_slotplanner_t planner;
    bool slot_planned;
    fsemu_slotplanner_slot_t slot;
    // The host refresh interval the current frame's slot was planned with: the interval of the
    // prediction the slots are planned from, which the slots follow each other by.
    double slot_interval_ns;
    bool slot_planned_before;
    bool slots_were_planned;
    // Whether the planned frame started before the picture of the frame before was posted whole
    // (its last rows are drawn in this frame's first lines: short frames), so that its first lines
    // run without waiting until that picture is posted (fsemu_frame_get_line_planned_ns).
    bool picture_pending;

    // A paused emulation thread sleeps on this condition until fsemu_frame_wake_paused sets wake.
    SDL_Mutex *pause_mutex;
    SDL_Condition *pause_condition;
    bool pause_wake;
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

int64_t fsemu_frame_epoch_ns = 0;
int64_t fsemu_frame_planned_start_ns = 0;
// The current frame's origin (GLOSSARY.md): the moment its times are measured from. With a swap
// deadline (a planned slot), the predicted host vertical blank one refresh before it; without one,
// the planned start.
static int64_t fsemu_frame_origin_ns = 0;

int64_t fsemu_frame_begin_ns = 0;
int64_t fsemu_frame_planned_end_ns = 0;

// ----------------------------------------------------------------------------

int64_t fsemu_frame_overshoot_duration_ns = 0;
int64_t fsemu_frame_wait_duration_ns = 0;
int64_t fsemu_frame_emu_duration_ns = 0;
int64_t fsemu_frame_sleep_duration_ns = 0;
int64_t fsemu_frame_extra_duration_ns = 0;
int64_t fsemu_frame_gui_duration_ns = 0;
int64_t fsemu_frame_render_duration_ns = 0;

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
    fsemu_emuthread_assert_current();

    // Like fsemu_control_paused: when quitting, leave the pause loop so the emulation thread can
    // see the quit request and end.
    if (fsemu_quit_check()) {
        return false;
    }
    return fsemu_frame.paused;
}

bool fsemu_frame_warping(void)
{
    fsemu_emuthread_assert_current();

    return fsemu_frame.warping || fsemu_frame.core_warping;
}

void fsemu_frame_set_core_warping(bool warping)
{
    fsemu_emuthread_assert_current();

    fsemu_frame.core_warping = warping;
}

// A planned frame that starts before the picture of the frame before was posted whole runs at most
// this many of its first lines without waiting, until that picture is posted (from WinUAE's
// framewait, at the display's vertical sync line, a few lines into the frame).
#define FSEMU_FRAME_PENDING_PICTURE_LINES 16

int64_t fsemu_frame_get_line_planned_ns(int line, int lines)
{
    if (fsemu_frame.picture_pending) {
        if (line < FSEMU_FRAME_PENDING_PICTURE_LINES &&
            fsemu_video_get_last_whole_posted_frame() < fsemu_frame_number_began - 1) {
            // Not waiting: the picture of the frame before is still to be posted.
            return 0;
        }
        fsemu_frame.picture_pending = false;
    }
    if (lines <= 0 || line >= lines) {
        return fsemu_frame_planned_end_ns;
    }
    return fsemu_frame_planned_start_ns +
           (fsemu_frame_planned_end_ns - fsemu_frame_planned_start_ns) * line / lines;
}

int64_t fsemu_frame_get_epoch_ns(void)
{
    return fsemu_frame_epoch_ns;
}

void fsemu_frame_reset_epoch(void)
{
    fsemu_frame_epoch_ns = (int64_t)SDL_GetTicksNS();
}

void fsemu_frame_toggle_sleep_busywait(void)
{
    fsemu_frame.busy_wait = !fsemu_frame.busy_wait;
    fsemu_frame_log("Busy waiting: %s\n", fsemu_frame.busy_wait ? "on" : "off");
}

// Waits until until_ns, recorded in the performance graph's stripe as category (a sleep's
// oversleep as oversleep), then back to what the thread was doing before it.
static void fsemu_frame_wait_until_ns_as(int64_t until_ns, fsemu_stripe_category_t category)
{
    int64_t now_ns = (int64_t)SDL_GetTicksNS();
    fsemu_stripe_category_t before = fsemu_stripe_push(category, now_ns);
    if (fsemu_frame.busy_wait) {
        while (now_ns < until_ns) {
            fsapp_time_mm_pause();
            now_ns = (int64_t)SDL_GetTicksNS();
        }
    } else {
        now_ns = fsapp_time_sleep_until_with_current_time_ns(until_ns, now_ns);
        if (now_ns > until_ns) {
            fsemu_stripe_set(FSEMU_STRIPE_CATEGORY_OVERSLEEP, until_ns);
        }
    }
    fsemu_stripe_pop(before, now_ns);
}

void fsemu_frame_wait_until_ns(int64_t until_ns)
{
    fsemu_frame_wait_until_ns_as(until_ns, fsemu_frame.busy_wait
                                               ? FSEMU_STRIPE_CATEGORY_BUSY_WAITING
                                               : FSEMU_STRIPE_CATEGORY_SLEEPING);
}

void fsemu_frame_wait_until_frame_end(void)
{
    fsemu_frame_wait_until_ns_as(fsemu_frame_planned_end_ns, FSEMU_STRIPE_CATEGORY_END_WAIT);
}

// Records emulation done at now_ns, unless the frame has it already.
static void fsemu_frame_set_emulation_done(int64_t now_ns)
{
    if (fsemu_frame_number_began < 0) {
        return;
    }
    fsemu_frameinfo_t *frameinfo = &FSEMU_FRAMEINFO(fsemu_frame_number_began);
    if (frameinfo->emulation_done_ns == 0) {
        frameinfo->emulation_done_ns = now_ns;
    }
    fsemu_stripe_on_emulation_done(now_ns);
}

void fsemu_frame_on_emulation_done(void)
{
    fsemu_emuthread_assert_current();
    fsemu_frame_set_emulation_done((int64_t)SDL_GetTicksNS());
}

void fsemu_frame_end(void)
{
    fsemu_frame_log_trace("%s\n", __func__);
    fsemu_emuthread_assert_current();
    fsemu_frame_log_epoch("Frame end\n");
    fsapp_assert(fsemu_frame.initialized);

    int64_t now_ns = (int64_t)SDL_GetTicksNS();
    // Unless the core said when the frame's last line was emulated (before posting its last slice),
    // it was now. A planned slot's emulation done also tells the slot planner whether a whole-frame
    // slot was late.
    fsemu_frame_set_emulation_done(now_ns);
    if (fsemu_frame.slot_planned && fsemu_frame_number_began >= 0) {
        fsemu_slotplanner_on_emulation_done(
            &fsemu_frame.planner, FSEMU_FRAMEINFO(fsemu_frame_number_began).emulation_done_ns);
    }
    if (fsemu_frame_planned_end_ns == 0) {
        // The timing has not planned an end yet (the first frames of a session can end before it
        // has), so there is nothing to wait for or be late for.
    } else if (now_ns > fsemu_frame_planned_end_ns + SDL_MS_TO_NS(1)) {
        if (FS_IsTraceEnabled()) {
            FS_AddTraceInstant("late end", now_ns, fsemu_frame_number_began, "late_us",
                               now_ns - fsemu_frame_planned_end_ns);
        }
    } else if (now_ns >= fsemu_frame_planned_end_ns) {
    } else {
        int64_t wait_began_ns = now_ns;
        fsemu_frame_wait_until_frame_end();
        now_ns = (int64_t)SDL_GetTicksNS();
        if (FS_IsTraceEnabled()) {
            FS_AddTraceSpan("end wait", wait_began_ns, now_ns, fsemu_frame_number_began,
                            FS_TRACE_NO_VALUE, 0);
        }
        if (now_ns > fsemu_frame_planned_end_ns + SDL_MS_TO_NS(1)) {
            fsemu_frame_log_warning("Overslept %lld us\n",
                                    (long long)SDL_NS_TO_US(now_ns - fsemu_frame_planned_end_ns));
        }
        fsemu_frame_add_sleep_time(0);
    }
    // The stripe: the frame's record ends after its end wait; what follows is the next frame's.
    fsemu_stripe_end_frame(now_ns);

    // fsemu_frameinfo_t *frameinfo =
    //     &fsemu_frameinfo[fsemu_frame_number_began % FSEMU_FRAMEINFO_COUNT];

    // Temporary
    if (fsemu_frame_number_began < 0) {
        fsemu_frame_number_began = 0;
        fsemu_frame_log_warning("Forcing fsemu_frame_number_began = 0\n");
    }

    SDL_assert_release(fsemu_frame_number_began >= 0);

    fsemu_frameinfo_t *frameinfo = &FSEMU_FRAMEINFO(fsemu_frame_number_began);
    frameinfo->frame_hz = fsemu_frame_hz;
    // The frame's warp and planned start are written at its start only (fsemu_frame_start_2): the
    // drawing side's performance graph reads them once the frame is presented, which can be before
    // the frame has ended.

    frameinfo->emu_ns = fsemu_frame_emu_duration_ns;
    frameinfo->extra_ns = fsemu_frame_extra_duration_ns;
    frameinfo->gui_ns = fsemu_frame_gui_duration_ns;
    frameinfo->overshoot_ns = fsemu_frame_overshoot_duration_ns;
    frameinfo->render_ns = fsemu_frame_render_duration_ns;
    frameinfo->sleep_ns = fsemu_frame_sleep_duration_ns;
    frameinfo->wait_ns = fsemu_frame_wait_duration_ns;

    frameinfo->began_ns = fsemu_frame_begin_ns;
    frameinfo->ended_ns = now_ns;

    // Note; this is probably overriden in fsemu_frame_update_timing now.
    fsemu_frame_reset_epoch();

    fsemu_audio_end_frame();
    fsemu_video_end_frame();

    fsemu_recording_end_frame();

    // Advance the frame counter after updating stats for the current frame
    // (and possibly clearing status for the next frame).
    SDL_AddAtomicInt(&fsemu_frame.counter, 1);

    int64_t emulation_average_ns = fsapp_util_moving_average_add(
        &fsemu_frame.emulation_average, fsemu_frame.emulation_average_values_ns,
        FSEMU_FRAME_AVERAGE_FRAMES, fsemu_frame_emu_duration_ns);
    fsemu_frame.average_emulation_duration_ns = emulation_average_ns;
    static int64_t emulation_total_ns;
    emulation_total_ns += fsemu_frame_emu_duration_ns;

    static int64_t emulation_average_sum_ns;
    static int emulation_average_count;
    static int64_t emulation_average_max_ns;

    if (fsemu_frame_counter() % FSEMU_FRAME_AVERAGE_FRAMES == 0) {
        emulation_average_sum_ns += emulation_average_ns;
        emulation_average_count += 1;
    }
    if (emulation_average_ns > emulation_average_max_ns) {
        emulation_average_max_ns = emulation_average_ns;
    }

    if (fsemu_frame_counter() == fsemu_frame.quit_after_n_frames) {
        double ns_per_second = (double)SDL_NS_PER_SECOND;
        // The benchmark's result (quit_after_n_frames), whatever the frame log level.
        SDL_Log("Emu total: %0.3f s\n", (double)emulation_total_ns / ns_per_second);
        SDL_Log("Emu FPS (avg): %0.1f\n",
                ns_per_second / ((double)emulation_average_sum_ns / emulation_average_count));
        SDL_Log("Emu FPS (avg) min: %0.1f\n", ns_per_second / (double)emulation_average_max_ns);
        SDL_Log("Benchmark: %7.1f    Avg fps: %7.1f    "
                "Avg fps min: %7.1f\n",
                (double)emulation_total_ns / ns_per_second,
                ns_per_second / ((double)emulation_average_sum_ns / emulation_average_count),
                ns_per_second / (double)emulation_average_max_ns);
        fsemu_quit_maybe();
    }

    // fsemu_frame_log("Advanced frame counter to %d\n", fsemu_frame.counter);

    // Reset duration counters
    fsemu_frame_emu_duration_ns = 0;
    fsemu_frame_extra_duration_ns = 0;
    fsemu_frame_gui_duration_ns = 0;
    fsemu_frame_overshoot_duration_ns = 0;
    fsemu_frame_render_duration_ns = 0;
    fsemu_frame_sleep_duration_ns = 0;
    fsemu_frame_wait_duration_ns = 0;

    // Reset duration timer so we can begin calling the add_*_timer functions
    // without having to call reset timer after calling fsemu_frame_end.
    fsemu_frame.timer_ns = (int64_t)SDL_GetTicksNS();
}

void fsemu_frame_reset_timer(int64_t now_ns)
{
    fsemu_emuthread_assert_current();

    // Register diff time to unknown duration? Probably not necessary, since
    // this will be caught and registered as "other".
    fsemu_frame.timer_ns = now_ns ? now_ns : (int64_t)SDL_GetTicksNS();
}

// FIXME: Maybe make inline function in header for performance
void fsemu_frame_add_overshoot_time(int64_t now_ns)
{
    fsemu_emuthread_assert_current();

    if (now_ns == 0) {
        now_ns = (int64_t)SDL_GetTicksNS();
    }
    fsemu_frame_overshoot_duration_ns += now_ns - fsemu_frame.timer_ns;
    fsemu_frame.timer_ns = now_ns;
}

// FIXME: Maybe make inline function in header for performance
void fsemu_frame_add_framewait_time(int64_t now_ns)
{
    fsemu_emuthread_assert_current();

    if (now_ns == 0) {
        now_ns = (int64_t)SDL_GetTicksNS();
    }
    fsemu_frame_wait_duration_ns += now_ns - fsemu_frame.timer_ns;
    fsemu_frame.timer_ns = now_ns;
}

// FIXME: Maybe make inline function in header for performance
void fsemu_frame_add_gui_time(int64_t now_ns)
{
    fsemu_emuthread_assert_current();

    if (now_ns == 0) {
        now_ns = (int64_t)SDL_GetTicksNS();
    }
    fsemu_frame_gui_duration_ns += now_ns - fsemu_frame.timer_ns;
    fsemu_frame.timer_ns = now_ns;
}

// FIXME: Maybe make inline function in header for performance
void fsemu_frame_add_emulation_time(int64_t now_ns)
{
    fsemu_emuthread_assert_current();

    if (now_ns == 0) {
        now_ns = (int64_t)SDL_GetTicksNS();
    }
    fsemu_frame_emu_duration_ns += now_ns - fsemu_frame.timer_ns;
    fsemu_frame.timer_ns = now_ns;
}

// FIXME: Maybe make inline function in header for performance
void fsemu_frame_add_render_time(int64_t now_ns)
{
    fsemu_emuthread_assert_current();

    if (now_ns == 0) {
        now_ns = (int64_t)SDL_GetTicksNS();
    }
    fsemu_frame_render_duration_ns += now_ns - fsemu_frame.timer_ns;
    fsemu_frame.timer_ns = now_ns;
}

// FIXME: Maybe make inline function in header for performance
void fsemu_frame_add_sleep_time(int64_t now_ns)
{
    fsemu_emuthread_assert_current();

    if (now_ns == 0) {
        now_ns = (int64_t)SDL_GetTicksNS();
    }
    fsemu_frame_sleep_duration_ns += now_ns - fsemu_frame.timer_ns;
    fsemu_frame.timer_ns = now_ns;
}

// FIXME: Maybe make inline function in header for performance
void fsemu_frame_add_extra_time(int64_t now_ns)
{
    fsemu_emuthread_assert_current();

    if (now_ns == 0) {
        now_ns = (int64_t)SDL_GetTicksNS();
    }
    fsemu_frame_extra_duration_ns += now_ns - fsemu_frame.timer_ns;
    fsemu_frame.timer_ns = now_ns;
}

// Affects when the frame starts emulating.
void fsemu_frame_set_emulation_duration_ns(int64_t emulation_duration_ns)
{
    fsemu_frame_log("Emulation duration set to %lld us\n",
                    (long long)SDL_NS_TO_US(emulation_duration_ns));
    fsemu_frame.emulation_duration_ns = emulation_duration_ns;
}

void fsemu_frame_set_render_duration_ns(int64_t render_duration_ns)
{
    fsemu_frame_log("Render duration set to %lld us\n",
                    (long long)SDL_NS_TO_US(render_duration_ns));
    fsemu_frame.render_duration_ns = render_duration_ns;
}

int fsemu_frame_counter(void)
{
    return SDL_GetAtomicInt(&fsemu_frame.counter);
}

int fsemu_frame_counter_mod(int modulus)
{
    return fsemu_frame_counter() % modulus;
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

// A timeline reset: the emulation's timeline starts again from now instead of following on from
// the frames before it. With vsync off (or not following the host vertical blank), when frames
// were late ten times in a row; while following it, when the start gate gives up waiting for its
// host vertical blank, or when the slot planner decides it (slots that started late too many times
// in a row, or a stall). Traced as "timeline reset". The predicted host vertical blank goes on as
// it was, so that the slots after it are planned from a prediction that is already good. The audio
// ring buffer is reset to the target.
static void fsemu_frame_on_timeline_reset(int frame_number, int64_t now_ns)
{
    FSEMU_FRAMEINFO(frame_number).timeline_reset = true;
    if (FS_IsTraceEnabled()) {
        FS_AddTraceInstant("timeline reset", now_ns, frame_number, FS_TRACE_NO_VALUE, 0);
    }
    fsemu_audio_on_timeline_reset();
}

static void fsemu_frame_update_timing_timer_based(int frame_number, double hz,
                                                  int64_t frame_duration_ns)
{
    int64_t now_ns = (int64_t)SDL_GetTicksNS();
    if (fsemu_frame_planned_start_ns == 0) {
        fsemu_frame_planned_start_ns = now_ns;
    } else {
        fsemu_frame_planned_start_ns += frame_duration_ns;
    }

    static int late_frames;
    if ((now_ns - fsemu_frame_planned_start_ns) > frame_duration_ns / 2) {
        // More than half a frame has already passed since the planned start. Assume
        // that we are behind scedule and might need to catch up. We avoid
        // doing it immediately since we might be able to catch up, thus
        // keeping the correct average frame rate.
        late_frames += 1;
    } else {
        late_frames = 0;
    }

    if (late_frames >= 10) {
        // We might not be able to catch up normally, so we reset the planned start.
        fsemu_frame_planned_start_ns = now_ns;
        fsemu_frame_log_warning("late_frames >= 10: the planned start is reset to now\n");
        fsemu_frame_on_timeline_reset(frame_number, now_ns);
        // FIXME: REGISTER THIS AS AN UNDERRUN EVENT
    }

    fsemu_frame_planned_end_ns = fsemu_frame_planned_start_ns + frame_duration_ns;

    fsemu_frame_begin_ns = fsemu_frame_planned_start_ns + fsemu_frame_framewait(hz);
    if (fsemu_frame_begin_ns < now_ns) {
        fsemu_frame_begin_ns = now_ns;
    }

    if (fsemu_frame_warping() && !fsemu_frame_paused()) {
        // Without this, we will get too "far behind" on time.
        fsemu_frame_planned_start_ns = now_ns;
        fsemu_frame_begin_ns = now_ns;
        fsemu_frame_planned_end_ns = now_ns;
    }

    // No swap deadline: the slots follow each other from their origins.
    fsemu_frame_origin_ns = fsemu_frame_planned_start_ns;
    fsemu_frame_epoch_ns = fsemu_frame_planned_start_ns;
}

static void fsemu_frame_update_timing_vsync_based(int frame_number, double hz,
                                                  int64_t frame_duration_ns)
{
    int64_t now_ns = (int64_t)SDL_GetTicksNS();

    if (fsemu_frame.slot_planned) {
        // The slot planner's slot (fsemu_frame_plan_slot), taken as it is.
        fsemu_frame_origin_ns = fsemu_frame.slot.origin_ns;
        fsemu_frame_planned_start_ns = fsemu_frame.slot.planned_start_ns;
        fsemu_frame_planned_end_ns = fsemu_frame.slot.planned_end_ns;
    } else {
        // The start gate's planned start (fsemu_frame_wait_until_frame_start_vsync), a whole frame
        // slot with no swap deadline, so also its origin. 1 is not a time but lets the frame start
        // at once (the first two frames of a session, and the two after warping): its planned
        // start is now, not 1 ns after the clock's start, which made it end hours late.
        int64_t allow_start_ns = FSEMU_FRAMEINFO(frame_number).vsync_allow_start_ns;
        fsemu_frame_planned_start_ns = allow_start_ns == 1 ? now_ns : allow_start_ns;
        fsemu_frame_planned_end_ns = fsemu_frame_planned_start_ns + frame_duration_ns;
        fsemu_frame_origin_ns = fsemu_frame_planned_start_ns;
    }
    fsemu_frame_begin_ns = fsemu_frame_planned_start_ns;

    if (fsemu_frame_warping() && !fsemu_frame_paused()) {
        // Without this, we will get too "far behind" on time.
        fsemu_frame_planned_start_ns = now_ns;
        fsemu_frame_begin_ns = now_ns;
        fsemu_frame_planned_end_ns = now_ns;
        fsemu_frame_origin_ns = now_ns;
    }

    fsemu_frame_epoch_ns = fsemu_frame_planned_start_ns;
}

// With vsync on, whether the emulation follows the host vertical blank (fsemu-follow.h), as last
// decided; it follows until the host refresh rate is measured. Written by the emulation thread,
// read by any thread (the main loop's pacer). Relaxed: the main loop is told of a change
// (fsemu_mainloop_on_following_changed) after it is stored, and a stale value only lasts a frame.
static std::atomic<bool> fsemu_frame_following{true};

bool fsemu_frame_is_following_host_vertical_blank(void)
{
    return fsemu_frame_following.load(std::memory_order_relaxed);
}

// With vsync on, decides once per frame, before its start gate, whether the frame follows the host
// vertical blank (fsemu_follow_update, with hysteresis), and tells the main loop's pacer when the
// decision changes, so that fsapp looks for the host vertical blank only while the emulation
// follows it. Otherwise the emulation keeps time on its own at the emulated frame rate, as with
// vsync off, and each emulated frame is shown at the next host vertical blank, so frames are
// doubled or dropped as the rates dictate (one in five doubled for 50 Hz on 60 Hz). Returns false
// with vsync off.
static bool fsemu_frame_update_following(int frame_number, double hz)
{
    fsemu_emuthread_assert_current();

    static fsemu_follow_t follow;
    static bool decided_before = false;
    if (!decided_before) {
        fsemu_follow_init(&follow);
    }
    if (!fsemu_video_vsync()) {
        return false;
    }
    int64_t interval_ns = fsemu_video_get_vsync_interval_ns();
    double host_refresh_rate_hz =
        interval_ns > 0 ? (double)SDL_NS_PER_SECOND / (double)interval_ns : 0.0;
    bool following = follow.following;
    // REVIEW: A predicted host vertical blank that is not steady (adaptive sync, but also the 5 s
    // after a few snaps, as after a resume where the swaps did not wait for the repaint) stops the
    // emulation following it. While it does not follow, the frame records' host vertical blanks
    // are estimated (the swap plus a refresh), and it follows again once the prediction has been
    // steady for FSEMU_FOLLOW_FRAMES frames. Check on Wayland that a resume does not leave the
    // emulation not following for long.
    bool steady = fsemu_mainloop_is_predicted_host_vertical_blank_steady();
    bool decided = fsemu_follow_update(&follow, host_refresh_rate_hz, hz, steady);
    if (decided != following || !decided_before) {
        fsemu_frame_following.store(decided, std::memory_order_relaxed);
        fsemu_frame_log("The emulation %s the host vertical blank (host %0.2f Hz, emulation "
                        "%0.2f Hz%s)\n",
                        decided ? "follows" : "does not follow", host_refresh_rate_hz, hz,
                        steady ? "" : ", the predicted host vertical blank is not steady");
        if (FS_IsTraceEnabled()) {
            FS_AddTraceInstant("follows host vertical blank", (int64_t)SDL_GetTicksNS(),
                               frame_number, "follows", decided ? 1 : 0);
        }
        if (decided != following) {
            fsemu_mainloop_on_following_changed();
        }
        decided_before = true;
    }
    return decided;
}

// follow: whether the frame follows the host vertical blank (fsemu_frame_update_following).
static void fsemu_frame_update_timing(int frame_number, double hz, bool follow)
{
    fsemu_emuthread_assert_current();

    static double last_hz;
    if (hz != last_hz) {
        fsemu_frame_log("Emulation frame rate is now %f Hz\n", hz);
        last_hz = hz;
    }

    // FIXME: This is emulation hz, not vsync hz, define what's correct
    fsemu_frame_hz = hz;
    // fsemu_frame_warp = fsemu_control_warp();

    // REVIEW: The emulated frame period was rounded down to whole microseconds, and is now rounded
    // down to whole nanoseconds: with vsync off, the timeline of a rate whose period is not a
    // whole number of microseconds runs at the emulated rate rather than slightly faster (the
    // AROS boot's 50.000282 Hz: 19999.887 us a frame, where it was 19999 us, 44 ppm fast).
    int64_t frame_duration_ns = (int64_t)((double)SDL_NS_PER_SECOND / hz);
    if (follow) {
        double vsync_hz = fsemu_video_vsync_frequency();
        fsemu_frame.frame_rate_multiplier = vsync_hz / hz;
        // Planned slots follow each other by the interval of the prediction they were planned
        // from (which it keeps through the prediction's restarts). Before they are planned, the
        // planned starts come from the start gate on the predicted host vertical blank once it has
        // settled (else from the host vertical blank two frames back), so the timeline runs at the
        // predicted host refresh interval, or the measured one before. Not vsync_hz: that is a
        // whole number of Hz.
        // The drawing side updates the prediction and publishes its interval after each swap.
        double interval_ns = fsemu_frame.slot_planned && fsemu_frame.slot_interval_ns > 0.0
                                 ? fsemu_frame.slot_interval_ns
                             : fsemu_mainloop_is_predicted_host_vertical_blank_settled()
                                 ? fsemu_mainloop_get_predicted_host_refresh_interval_ns()
                                 : fsemu_video_get_host_refresh_interval_ns();
        fsemu_frame.timeline_rate_hz =
            interval_ns > 0 ? (double)SDL_NS_PER_SECOND / interval_ns : hz;
        fsemu_frame_update_timing_vsync_based(frame_number, vsync_hz,
                                              fsemu_video_get_vsync_interval_ns());
    } else {
        fsemu_frame.frame_rate_multiplier = 1.0;
        fsemu_frame.timeline_rate_hz = hz;
        fsemu_frame_update_timing_timer_based(frame_number, hz, frame_duration_ns);
    }

    fsemu_frame_log_epoch(
        "Frame %16d        ends at %16lld (+%lld us)\n", fsemu_frame_counter(),
        (long long)fsemu_frame_planned_end_ns,
        (long long)SDL_NS_TO_US(fsemu_frame_planned_end_ns - fsemu_frame_epoch_ns));
}

static void fsemu_frame_start_handle_command(fsemu_action_t action, fsemu_action_state_t state)
{
    switch (action) {
    case FSEMU_ACTION_PAUSE_DISABLE:
        if (state) {
            fsemu_frame_log("FSEMU_ACTION_PAUSE_DISABLE\n");
            fsemu_frame.paused = false;
        }
        break;

    case FSEMU_ACTION_PAUSE_ENABLE:
        if (state) {
            fsemu_frame_log("FSEMU_ACTION_PAUSE_ENABLE\n");
            fsemu_frame.paused = true;
        }
        break;

    case FSEMU_ACTION_WARP_DISABLE:
        if (state) {
            fsemu_frame_log("FSEMU_ACTION_WARP_DISABLE\n");
            fsemu_frame.warping = false;
        }
        break;

    case FSEMU_ACTION_WARP_ENABLE:
        if (state) {
            fsemu_frame_log("FSEMU_ACTION_WARP_ENABLE\n");
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
        fsemu_frame_log_debug("Frame command %04x %04x\n", action, state);
        fsemu_frame_start_handle_command(action, state);
    }
}

extern volatile int allow_frame;

// The start gate places frame N's planned start this share of a host refresh before frame N - 1's
// host vertical blank (x = 0 on the performance graph's axis): 8 ms at 50 Hz. Its whole-frame slot
// then ends as long before the frame's target vertical blank, and the planned start is within the
// half refresh the performance graph shows before x = 0. The same as the planned whole-frame slots'
// fixed margin, so that the phase does not change when the slots are planned.
#define FSEMU_FRAME_START_GATE_LEAD FSEMU_SLOTPLANNER_WHOLE_FRAME_MARGIN

// Whole-frame slots by default while following the host vertical blank, so that slow hosts keep
// up; FSEMU_WHOLE_FRAME_SLOTS=0 starts each slot the event pump allowance after the host vertical
// blank instead (less input lag where the emulation is fast enough).
bool fsemu_frame_has_whole_frame_slots(void)
{
    const char *value = SDL_getenv("FSEMU_WHOLE_FRAME_SLOTS");
    return value == NULL || strcmp(value, "0") != 0;
}

static int64_t fsemu_frame_can_start_frame_vsync(int frame_number)
{
    if (frame_number < 2) {
        return true;
    }

    int swapped_frame =
        FSEMU_FRAMEINFO(frame_number - 2).render_skipped ? frame_number - 1 : frame_number - 2;
    int64_t vsync_ns = FSEMU_FRAMEINFO(swapped_frame).vsync_ns;
    if (vsync_ns == 0) {
        return 0;
    }
    // Once the predicted host vertical blank has settled, the planned start is placed from it, so
    // that a late swap (one wakeup later than the others; seen on macOS after a while without
    // input) does not move the planned start, and the frame after it does not start late against
    // its planned start. Written before vsync_ns (fsemu_video_set_frame_vsync). This places the
    // planned start (a host refresh after frame N - 2's host vertical blank, less the start gate's
    // lead) until the slots are planned from the prediction (fsemu_frame_plan_slot, once it has
    // enough observations).
    SDL_MemoryBarrierAcquire();
    int64_t predicted_ns = FSEMU_FRAMEINFO(swapped_frame).vsync_predicted_ns;
    fsemu_frame.planned_start_predicted = predicted_ns != 0;
    if (predicted_ns != 0) {
        vsync_ns = predicted_ns;
    }

    double interval_ns = fsemu_video_get_host_refresh_interval_ns();
    if (interval_ns <= 0.0) {
        interval_ns =
            (double)SDL_NS_PER_SECOND / (fsemu_frame.rate_hz > 0 ? fsemu_frame.rate_hz : 50.0);
    }
    int64_t target_ns =
        vsync_ns + (int64_t)llround(interval_ns * (1.0 - FSEMU_FRAME_START_GATE_LEAD));
    if ((int64_t)SDL_GetTicksNS() >= target_ns) {
        return target_ns;
    }
    return 0;
}

// The start gate gives up after this many frame periods without the host vertical blank it waits
// for, and starts the frame now, so that no frame waits for ever. That host vertical blank may
// never come: when picture N - 2 was dropped (a newer frame's slice came before the main thread
// took it in), the gate looks at picture N - 1 instead, which is posted only after frame N starts
// when the core posts it from WinUAE's framewait (fsemu_video_get_posting_frame_number).
#define FSEMU_FRAME_START_GATE_MAX_PERIODS 3

static void fsemu_frame_wait_until_frame_start_vsync(int frame_number)
{
    // FIXME: Allow choice between busy-wait and sleeping.
    // FIXME: If vsync time isn't available in
    // fsemu_frame_can_start_frame_vsync, maybe wait on a condition.
    int64_t planned_start_ns;
    int64_t wait_began_ns = (int64_t)SDL_GetTicksNS();
    fsemu_stripe_set(FSEMU_STRIPE_CATEGORY_BEFORE_SLOT, wait_began_ns);
    double rate_hz = fsemu_frame.rate_hz > 0 ? fsemu_frame.rate_hz : 50.0;
    int64_t give_up_ns = wait_began_ns + (int64_t)(FSEMU_FRAME_START_GATE_MAX_PERIODS *
                                                   (double)SDL_NS_PER_SECOND / rate_hz);
    while ((planned_start_ns = fsemu_frame_can_start_frame_vsync(frame_number)) == 0) {
        if (fsemu_quit_check()) {
            return;
        }
        int64_t now_ns = (int64_t)SDL_GetTicksNS();
        if (now_ns >= give_up_ns) {
            fsemu_frame_log("Start gate of frame %d gave up waiting\n", frame_number);
            planned_start_ns = now_ns;
            fsemu_frame.planned_start_predicted = false;
            fsemu_frame_on_timeline_reset(frame_number, now_ns);
            break;
        }
    }
    fsemu_stripe_set(FSEMU_STRIPE_CATEGORY_OTHER_WORK, FSEMU_STRIPE_NOW);
    if (FS_IsTraceEnabled()) {
        FS_AddTraceSpan("start gate", wait_began_ns, (int64_t)SDL_GetTicksNS(), frame_number,
                        FS_TRACE_NO_VALUE, 0);
    }
    FSEMU_FRAMEINFO(frame_number).vsync_allow_start_ns = planned_start_ns;

    fsemu_frame_add_sleep_time(0);
}

// While the emulation follows the host vertical blank: plans the frame's slot with the slot planner
// (fsemu-slotplanner.h), once the predicted host vertical blank that the main thread publishes
// (fsemu_mainloop_get_slot_timing) has enough observations, and spins until its planned start. The
// emulation thread does not wait for the main thread: the planned start, the slot end and the swap
// deadline come from the prediction alone, so a late swap does not move them. The swap deadline and
// the slot margin are stored in the frame's info before its first slice is posted. Returns false,
// and plans nothing, until the prediction has enough observations after a session start or
// resuming, or while its interval is far from the emulated frame period (then the start gate places
// the planned start); once the slots are planned, a restart of the predicted host vertical blank
// (on an early observation, a changed interval) does not stop it.
static bool fsemu_frame_plan_slot(int frame_number)
{
    fsemu_slotplanner_prediction_t prediction;
    int64_t margin_ns;
    if (!fsemu_mainloop_get_slot_timing(&prediction, &margin_ns) ||
        !fsemu_slotplanner_can_plan(&prediction, (double)SDL_NS_PER_SECOND / fsemu_frame.rate_hz)) {
        return false;
    }
    if (!fsemu_frame.slot_planned_before) {
        // The first planned slot (at a session start, on resuming, or after the start gate or
        // warping): the first deadline whose planned start is not yet past.
        fsemu_slotplanner_reset(&fsemu_frame.planner);
    }
    int64_t now_ns = (int64_t)SDL_GetTicksNS();
    fsemu_slotplanner_slot_t *slot = &fsemu_frame.slot;
    fsemu_slotplanner_plan_slot(&fsemu_frame.planner, &prediction, margin_ns, now_ns, slot);
    fsemu_frame.slot_interval_ns = prediction.interval_ns;
    if (slot->timeline_reset) {
        fsemu_frame_on_timeline_reset(frame_number, now_ns);
    }
    FSEMU_FRAMEINFO(frame_number).deadline_ns = slot->deadline_ns;
    FSEMU_FRAMEINFO(frame_number).slot_margin_ns = slot->margin_ns;
    if (FS_IsTraceEnabled()) {
        FS_TraceValue values[] = {{"margin_us", slot->margin_ns},
                                  {"observations", prediction.observations}};
        FS_AddTraceInstantWithValues("swap deadline", slot->deadline_ns, frame_number, values, 2);
    }

    // In a short frame (AROS's, and Kickstart 1.3's first), the picture's last rows are drawn in
    // the next frame's first lines, and WinUAE's framewait, a few lines in, posts it whole. Those
    // lines run at once, without waiting for the planned start, so that the picture is ready a slot
    // margin before its swap deadline instead of after it; the lines after them wait for their
    // planned times. Their vertical blank interrupt then runs before the event pump.
    // REVIEW: The slot planning does not provide for pictures completed in the next frame's first
    // lines; this keeps short frames from missing every deadline, but their vertical blank
    // interrupt runs before the event pump. Long frames (most games) are not affected.
    fsemu_frame.picture_pending = fsemu_video_get_last_whole_posted_frame() < frame_number - 1;
    if (fsemu_frame.picture_pending) {
        return true;
    }
    // Spins until the planned start, as the start gate does (sleeping is a later change).
    int64_t wait_began_ns = now_ns;
    fsemu_stripe_set(FSEMU_STRIPE_CATEGORY_BEFORE_SLOT, wait_began_ns);
    while (now_ns < slot->planned_start_ns) {
        fsapp_time_mm_pause();
        now_ns = (int64_t)SDL_GetTicksNS();
    }
    fsemu_stripe_set(FSEMU_STRIPE_CATEGORY_OTHER_WORK, FSEMU_STRIPE_NOW);
    if (FS_IsTraceEnabled()) {
        FS_AddTraceSpan("start wait", wait_began_ns, now_ns, frame_number, FS_TRACE_NO_VALUE, 0);
    }
    fsemu_frame_add_sleep_time(0);
    return true;
}

static void fsemu_frame_start_2(double hz)
{
    SDL_LockSpinlock(&fsemu_frame.rate_lock);
    fsemu_frame.rate_hz = hz;
    SDL_UnlockSpinlock(&fsemu_frame.rate_lock);

    // FIXME: Also for non-vsync we should make sure that we don't get more
    // video frames than we can display. So limiting to 2 ahead or something
    // makes sense there also.

    // FIXME: Use a condition to wait on?
    // (Plus also possibly sleep after allow start is known - for vsync)

    // Adding one here because we haven't increased fsemu_frame_number_began
    // yet.
    // while (allow_frame < fsemu_frame_number_began + 1) {
    //     fsapp_time_sleep_ns((int64_t)SDL_US_TO_NS(500));
    // }

    // FIXME: If vsync
    // FIXME: Not here?

    int frame_number = fsemu_frame_number_began += 1;

    fsemu_frameinfo_t *frameinfo = &FSEMU_FRAMEINFO(frame_number);
    memset(frameinfo, 0, sizeof(fsemu_frameinfo_t));
    fsemu_stripe_begin_frame(frame_number, fsemu_perfgui_mode() != FSEMU_PERFGUI_MODE_OFF);

    static bool was_warping;
    if (fsemu_frame_warping()) {
        was_warping = true;
    } else {
        if (was_warping) {
            // Allow next two frames to start
            FSEMU_FRAMEINFO(frame_number + 0).vsync_allow_start_ns = 1;
            FSEMU_FRAMEINFO(frame_number + 1).vsync_allow_start_ns = 1;
        }
        was_warping = false;
    }

    bool follow = fsemu_frame_update_following(frame_number, hz);
    fsemu_frame.planned_start_predicted = false;
    fsemu_frame.slot_planned = false;
    fsemu_frame.picture_pending = false;
    if (follow && !fsemu_frame_warping()) {
        fsemu_frame.slot_planned = fsemu_frame_plan_slot(frame_number);
        if (fsemu_frame.slot_planned != fsemu_frame.slots_were_planned) {
            fsemu_frame_log("Frame %d: the slots are now %s\n", frame_number,
                            fsemu_frame.slot_planned
                                ? "planned from the predicted host vertical blank"
                                : "placed by the start gate");
            fsemu_frame.slots_were_planned = fsemu_frame.slot_planned;
        }
    }
    fsemu_frame.slot_planned_before = fsemu_frame.slot_planned;
    if (fsemu_frame.slot_planned) {
        // Planned from the predicted host vertical blank.
        fsemu_frame.planned_start_predicted = true;
    } else if (follow && !fsemu_frame_warping()) {
        fsemu_frame_wait_until_frame_start_vsync(frame_number);
        if (fsemu_frame.planned_start_predicted != fsemu_frame.planned_starts_were_predicted) {
            fsemu_frame_log("Frame %d: the planned start is now placed from %s\n", frame_number,
                            fsemu_frame.planned_start_predicted
                                ? "the predicted host vertical blank"
                                : "the host vertical blank two frames back");
            fsemu_frame.planned_starts_were_predicted = fsemu_frame.planned_start_predicted;
        }
    }

    fsemu_frame_number_began = frame_number;

    // FIXME: update timing depends on warp mode, so handle commands first?
    fsemu_frame_start_handle_commands();

    fsemu_frame_update_timing(frame_number, hz, follow);
    // We have now a new value for the (ideal) planned start time for the new frame.
    // We use this as basis for duration logging during the coming frame, and
    // start by registering time spent since (ideal) planned start until now as
    // "overshoot" time.

    FSEMU_FRAMEINFO(frame_number).begins_ns = fsemu_frame_begin_ns;
    FSEMU_FRAMEINFO(frame_number).ends_ns = fsemu_frame_planned_end_ns;
    // At the frame's start (only), so that the drawing side has them when the frame is swapped
    // before it has ended.
    FSEMU_FRAMEINFO(frame_number).origin_ns = fsemu_frame_origin_ns;
    FSEMU_FRAMEINFO(frame_number).planned_start_ns = fsemu_frame_planned_start_ns;
    FSEMU_FRAMEINFO(frame_number).frame_warp = fsemu_frame_warping();
    if (FS_IsTraceEnabled()) {
        FS_AddTraceInstant("origin", fsemu_frame_origin_ns, frame_number, FS_TRACE_NO_VALUE, 0);
        FS_AddTraceInstant("planned start", fsemu_frame_planned_start_ns, frame_number, "predicted",
                           fsemu_frame.planned_start_predicted ? 1 : 0);
        FS_AddTraceInstant("planned end", fsemu_frame_planned_end_ns, frame_number,
                           FS_TRACE_NO_VALUE, 0);
    }

    // FIXME: Clear future timers, e.g. vsync which might not happen

    fsemu_frame_reset_timer(fsemu_frame_planned_start_ns);
    fsemu_frame_add_overshoot_time(0);
    // The actual slot start: the timer is now.
    FSEMU_FRAMEINFO(frame_number).slot_started_ns = fsemu_frame.timer_ns;
    // fsemu_frame_reset_epoch() FIXME: Maybe reset epoch to same time here?
}

void fsemu_frame_wake_paused(void)
{
    if (fsemu_frame.pause_mutex == NULL) {
        return;
    }
    SDL_LockMutex(fsemu_frame.pause_mutex);
    fsemu_frame.pause_wake = true;
    SDL_SignalCondition(fsemu_frame.pause_condition);
    SDL_UnlockMutex(fsemu_frame.pause_mutex);
}

// Sleeps until fsemu_frame_wake_paused is called (or was called since the last sleep).
static void fsemu_frame_sleep_while_paused(void)
{
    SDL_LockMutex(fsemu_frame.pause_mutex);
    while (!fsemu_frame.pause_wake) {
        SDL_WaitCondition(fsemu_frame.pause_condition, fsemu_frame.pause_mutex);
    }
    fsemu_frame.pause_wake = false;
    SDL_UnlockMutex(fsemu_frame.pause_mutex);
}

// The emulation was paused when the current frame started (the pause command was handled in
// fsemu_frame_start_2). fsemu's pacer is cleared, so the main loop draws the emulation surface's
// old picture without it, and the emulation thread sleeps, posting no frames, until a command (the
// one that resumes it) or a quit request wakes it. The paused frame is then ended without being
// emulated, and a new frame is started.
static void fsemu_frame_pause(double hz)
{
    fsemu_frame_log("Paused at frame %d\n", fsemu_frame_number_began);
    int64_t paused_ns = FS_IsTraceEnabled() ? (int64_t)SDL_GetTicksNS() : 0;
    int paused_frame = fsemu_frame_number_began;
    fsemu_mainloop_set_paused(true);
    // The audio device is paused and its stream cleared; on resume, the audio ring buffer starts
    // again at the target with silence.
    fsemu_audio_set_paused(true);
    do {
        // This frame number is used up by the pause, with no picture (the performance graph
        // writes no row for it).
        FSEMU_FRAMEINFO(fsemu_frame_number_began).paused = true;
        // FIXME: NETPLAY: Consider how frame counting and input events are handled while paused.
        while (fsemu_frame_paused()) {
            fsemu_frame_sleep_while_paused();
            fsemu_frame_start_handle_commands();
        }

        // The paused frame posted no picture. With vsync on, a frame starts only once the picture
        // of the frame two before has been swapped (fsemu_frame_can_start_frame_vsync), so the
        // next frame would wait forever for this one's predecessor, which posted no picture
        // either: they count as swapped now.
        // REVIEW: The dummy frames that were posted while paused fed this before.
        int64_t now_ns = (int64_t)SDL_GetTicksNS();
        for (int i = fsemu_frame_number_began - 1; i <= fsemu_frame_number_began; i++) {
            if (i >= 0 && FSEMU_FRAMEINFO(i).vsync_ns == 0) {
                FSEMU_FRAMEINFO(i).vsync_ns = now_ns;
            }
        }

        fsemu_frame_end();
        // The frame timeline starts afresh at the resumed frame, so the emulation does not run fast
        // to catch up with the time spent paused: with vsync off from now (the timer based timing);
        // while following the host vertical blank, the predicted host vertical blank and the slot
        // margin start afresh, and the start gate places the planned starts until the slots can be
        // planned again.
        fsemu_frame_planned_start_ns = 0;
        fsemu_mainloop_restart_slot_timing();
        fsemu_frame_start_2(hz);
        // A pause command may have come again with the new frame.
    } while (fsemu_frame_paused());
    if (FS_IsTraceEnabled()) {
        FS_AddTraceSpan("paused", paused_ns, (int64_t)SDL_GetTicksNS(), paused_frame,
                        FS_TRACE_NO_VALUE, 0);
    }
    if (fsemu_quit_check()) {
        // Left the pause to quit: the pacer stays cleared.
        return;
    }
    fsemu_frame_log("Resumed at frame %d\n", fsemu_frame_number_began);
    fsemu_mainloop_set_paused(false);
    fsemu_audio_set_paused(false);
}

void fsemu_frame_on_session_ended(void)
{
    fsemu_emuthread_assert_current();
    if (fsemu_frame.session_started) {
        fsemu_mainloop_set_session(false);
    }
}

// FIXME: Move hz to a separate function that can be called before start?
// FIXME: Rename to fsemu_frame_begin?
void fsemu_frame_start(double hz)
{
    // printf("fsemu_frame_start hz=%f\n", hz);
    fsemu_frame_log_trace("%s hz=%f\n", __func__, hz);
    fsemu_emuthread_assert_current();
    int64_t began_ns = FS_IsTraceEnabled() ? (int64_t)SDL_GetTicksNS() : 0;

    fsemu_frame_start_2(hz);
    // printf("START_OR_POST start %d ------------------\n",
    //        fsemu_frame_number_began);

    if (!fsemu_frame.session_started) {
        // The session's first emulated frame: fsemu's pacer is set from now on (until pause).
        fsemu_frame.session_started = true;
        FSEMU_FRAMEINFO(fsemu_frame_number_began).session_start = true;
        fsemu_mainloop_set_session(true);
    }
    if (fsemu_frame_paused()) {
        fsemu_frame_pause(hz);
    }
    // The audio rate adjustment for the frame, from the audio buffer level at the last one's end.
    fsemu_audio_begin_frame();

    // fsemu_recording_begin_frame(fsemu_frame.counter);
    fsemu_recording_begin_frame();

    // The stripe: the emulation slot, the core emulates from now on.
    fsemu_stripe_set(FSEMU_STRIPE_CATEGORY_EMULATING, FSEMU_STRIPE_NOW);

    if (FS_IsTraceEnabled()) {
        FS_AddTraceSpan("frame start", began_ns, (int64_t)SDL_GetTicksNS(),
                        fsemu_frame_number_began, FS_TRACE_NO_VALUE, 0);
    }
}

int fsemu_frame_get_number(void)
{
    return fsemu_frame_number_began;
}

double fsemu_frame_rate_hz(void)
{
    SDL_LockSpinlock(&fsemu_frame.rate_lock);
    double rate_hz = fsemu_frame.rate_hz;
    SDL_UnlockSpinlock(&fsemu_frame.rate_lock);
    return rate_hz;
}

double fsemu_frame_get_timeline_rate_hz(void)
{
    return fsemu_frame.timeline_rate_hz;
}

int64_t fsemu_frame_get_average_emulation_duration_ns(void)
{
    return fsemu_frame.average_emulation_duration_ns;
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

    fsemu_frame_log_level = fsemu_option_int_default(FSEMU_OPTION_LOG_FRAME, fsemu_frame_log_level);

    fsemu_frame.quit_after_n_frames =
        fsemu_option_int_default(FSEMU_OPTION_QUIT_AFTER_N_FRAMES, -1);

    fsemu_frame.load_state = -1;
    fsemu_frame.save_state = -1;

    fsemu_frame.busy_wait = true;

    fsemu_slotplanner_init(&fsemu_frame.planner);
    if (fsemu_frame_has_whole_frame_slots()) {
        fsemu_frame_log("Whole-frame slots\n");
        fsemu_slotplanner_set_whole_frame_slots(&fsemu_frame.planner, true);
    } else {
        fsemu_frame_log("Slots after the host vertical blank (FSEMU_WHOLE_FRAME_SLOTS=0)\n");
    }

    fsemu_frame.pause_mutex = SDL_CreateMutex();
    fsemu_frame.pause_condition = SDL_CreateCondition();

    // Allow first two frames to start
    FSEMU_FRAMEINFO(0).vsync_allow_start_ns = 1;
    FSEMU_FRAMEINFO(1).vsync_allow_start_ns = 1;

    // FIXME: When using a shorter emulation duration, Mednafen needs
    // a larger audio buffer to avoid underruns. Check why. Probably
    // something to do with how/when buffer fill is calculated.
}

// ----------------------------------------------------------------------------

#pragma GCC diagnostic pop
