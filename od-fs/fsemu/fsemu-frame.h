#ifndef FSEMU_FRAME_H
#define FSEMU_FRAME_H

#include <stdint.h>

#include "fsapp-time.h"
#include "fsemu-config.h"

// FIXME: Check / define order of
// - fsemu_frame_end
// - fsemu_frame_update_timing
// - and the actual posting of frames
// (making sure frame numbers and stats are consistent)

#ifdef __cplusplus
extern "C" {
#endif

// fsemu's frame timing is on SDL's clock (SDL_GetTicksNS), and holds its times and durations as
// int64_t nanoseconds (named _ns).

int64_t fsemu_frame_get_epoch_ns(void);
void fsemu_frame_reset_epoch(void);

double fsemu_frame_rate_multiplier(void);

bool fsemu_frame_paused(void);
// Whether the frame is not paced: fsemu's warp command, or the core's own warp mode
// (fsemu_frame_set_core_warping). The timeline then follows the emulation (planned start, begin and
// end are now) instead of the clock.
bool fsemu_frame_warping(void);

// Tells fsemu whether the emulation core warps on its own (a warp mode of the core's, set from its
// configuration or an input event), so that fsemu does not pace the frame either. Called on the
// emulation thread before fsemu_frame_start, which takes it into account.
void fsemu_frame_set_core_warping(bool warping);

// With vsync on, whether the emulation follows the host vertical blank (fsemu-follow.h), as decided
// at the start of the last frame (true before the first). Any thread.
bool fsemu_frame_is_following_host_vertical_blank(void);

// Whether the slots are whole-frame slots while following the host vertical blank (the default;
// FSEMU_WHOLE_FRAME_SLOTS=0 starts each slot the event pump allowance after the host vertical blank
// instead, until there is a setting). The same for the whole process; any thread.
bool fsemu_frame_has_whole_frame_slots(void);

// The scanline pacer (one pacer): the core waits after each line of a frame until the line's
// planned time, so that the frame's emulation is spread over its frame period. Returns when the
// first `line` lines of a frame of `lines` lines are planned to be done (the start of line `line`,
// counted from 0): the planned start + (planned end - planned start) * line / lines, and the
// planned end from line `lines` on. 0 for the first lines of a frame that started before the
// picture of the frame before was posted whole (they do not wait).
int64_t fsemu_frame_get_line_planned_ns(int line, int lines);

bool fsemu_frame_check_load_state(int *slot);
bool fsemu_frame_check_save_state(int *slot);

// Affects when the frame starts emulating.
// FIXME: In use?
void fsemu_frame_set_emulation_duration_ns(int64_t emulation_duration_ns);

// Affects when the frame starts rendering before vsync.
// FIXME: In use?
void fsemu_frame_set_render_duration_ns(int64_t render_duration_ns);

// Waits until until_ns (busy-waiting by default), recorded in the performance graph's stripe as a
// line wait (sleeping or busy-waiting).
void fsemu_frame_wait_until_ns(int64_t until_ns);

// Emulation done: the core has emulated the current frame's last line (before it posts the frame's
// last slice). Recorded in the frame's info (the slot margin is measured from it) and the
// performance graph's stripe; without it, fsemu_frame_end takes its own start as emulation done.
// Called on the emulation thread.
void fsemu_frame_on_emulation_done(void);

// Registers the currently emulated frame as done.
void fsemu_frame_end(void);

// Returns the current frame number.
// FIXME: Rename to fsemu_frame_no?
int fsemu_frame_counter(void);

int fsemu_frame_counter_mod(int modulus);

// It is important to use as close to real (emulated) Hz as possible here, so
// that the audio system knows how much data to expect and can correct if
// video output rate deviates from the emulated frame rate.
//
// The session's first call tells fsemu's pacer that the session has started. When the emulation is
// paused (the pause command is handled here), it tells the pacer so and sleeps until it is resumed
// or asked to quit (fsemu_frame_wake_paused), and then starts a new frame.
void fsemu_frame_start(double hz);

// The emulation session has ended (its last frame has been posted): fsemu's pacer is told so.
// Called on the emulation thread.
void fsemu_frame_on_session_ended(void);

// Wakes the emulation thread if it is paused, so it handles the commands posted since (resuming
// it) or sees a quit request. Called by fsemu when a command is posted and when quit is requested.
// Thread safe.
void fsemu_frame_wake_paused(void);

// Returns the frame rate as given to fsemu_frame_start. Any thread.
double fsemu_frame_rate_hz(void);

// The rate of fsemu's timeline for the current frame (Hz): the frame rate given to
// fsemu_frame_start, or with vsync on and following the host vertical blank, the host refresh
// rate (predicted, or measured until the prediction has settled). 0 before the first frame.
// Emulation thread.
double fsemu_frame_get_timeline_rate_hz(void);

// Deprecated; use fsemu_frame_start instead
// void fsemu_frame_update_timing(double hz, bool turbo);

// The average time spent emulating a frame (ns), over the last 256 frames, as of the last frame's
// end; 0 before the first frame has ended. Emulation thread.
int64_t fsemu_frame_get_average_emulation_duration_ns(void);

// The duration timer: the add_*_time functions add the time since the timer (or the last of them)
// to their duration and set the timer to now_ns (0: the clock is read).
void fsemu_frame_reset_timer(int64_t now_ns);

void fsemu_frame_toggle_sleep_busywait(void);
// Waits until the frame's planned end, as fsemu_frame_wait_until_ns, recorded in the performance
// graph's stripe as the end wait.
void fsemu_frame_wait_until_frame_end(void);

void fsemu_frame_add_overshoot_time(int64_t now_ns);
void fsemu_frame_add_framewait_time(int64_t now_ns);
void fsemu_frame_add_gui_time(int64_t now_ns);
void fsemu_frame_add_emulation_time(int64_t now_ns);
void fsemu_frame_add_render_time(int64_t now_ns);
void fsemu_frame_add_sleep_time(int64_t now_ns);
void fsemu_frame_add_extra_time(int64_t now_ns);

extern double fsemu_frame_hz;
// extern bool fsemu_frame_warp;

extern int64_t fsemu_frame_epoch_ns;
extern int64_t fsemu_frame_planned_start_ns;

extern int64_t fsemu_frame_begin_ns;
extern int64_t fsemu_frame_planned_end_ns;

extern int64_t fsemu_frame_overshoot_duration_ns;
extern int64_t fsemu_frame_wait_duration_ns;
extern int64_t fsemu_frame_gui_duration_ns;
extern int64_t fsemu_frame_emu_duration_ns;
extern int64_t fsemu_frame_render_duration_ns;
extern int64_t fsemu_frame_sleep_duration_ns;
extern int64_t fsemu_frame_extra_duration_ns;

// The number of the emulated frame that started last (fsemu_frame_start), counted from 0, or -1
// before the first. The frame number in the timing trace (fs/trace.h).
int fsemu_frame_get_number(void);

#ifdef FSEMU_INTERNAL

// Initializes the frame module (not a single frame).
void fsemu_frame_init(void);

extern volatile int fsemu_frame_number_began;
extern volatile int fsemu_frame_number_ended;
extern volatile int fsemu_frame_number_posted;
extern volatile int fsemu_frame_number_rendering;
extern volatile int fsemu_frame_number_rendered;
extern volatile int fsemu_frame_number_displaying;
extern volatile int fsemu_frame_number_displayed;
extern volatile int fsemu_frame_number_swapped;

#endif // FSEMU_INTERNAL

extern int fsemu_frame_log_level;

#define fsemu_frame_log(format, ...)                                                               \
    FS_LOG_INFO(fsemu_frame_log_level, "[FSE] [FRM]", format, ##__VA_ARGS__)

#define fsemu_frame_log_debug(format, ...)                                                         \
    FS_LOG_DEBUG(fsemu_frame_log_level, "[FSE] [FRM]", format, ##__VA_ARGS__)

#define fsemu_frame_log_trace(format, ...)                                                         \
    FS_LOG_TRACE(fsemu_frame_log_level, "[FSE] [FRM]", format, ##__VA_ARGS__)

#define fsemu_frame_log_warning(format, ...)                                                       \
    FS_LOG_WARNING(fsemu_frame_log_level, "[FSE] [FRM]", format, ##__VA_ARGS__)

#define fsemu_frame_log_epoch(format, ...)                                                         \
    FS_LOG_TRACE(fsemu_frame_log_level, "[FSE] [%5lld]", format,                                   \
                 (long long)SDL_NS_TO_US((int64_t)SDL_GetTicksNS() - fsemu_frame_epoch_ns),        \
                 ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* FSEMU_FRAME_H */
