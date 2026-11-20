#include "sysconfig.h"
#include "sysdeps.h"

#include "uae/fs.h"
#include "uae.h"

#include "fsapp-time.h"
#include "fs/trace.h"
#include "fsemu-audio.h"
#include "fsemu-frame.h"
#include "fsemu-python.h"
#include "fsemu-quit.h"
#include "fsemu-stripe.h"
//#include <fs/emu/hacks.h>

#include "inputdevice.h"

#include <algorithm>
#include <vector>

// -----------------

static bool amiga_parse_option_changed_fixme(const char *option, const char *value, int type)
{
    // some strings are modified during parsing
    char *value2 = strdup(value);
    int result = cfgfile_parse_option(
        &changed_prefs, (char *) option, (char *) value2, type);
    config_changed = 1;
    free(value2);
    write_log(
        "Set option %s = \"%s\"%s\n", option, value, result == 1 ? "" : " FAILED");
    if (result == 1) {
        return true;
    } else {
        gui_message("Option failed: %s = %s", option, value);
        amiga_log_warning(
            "Failed to set option \"%s\" to \"%s\" "
            "(result: %d)\n",
            option,
            value,
            result);
        return false;
    }
}

static void apply_option(const char *name, const char *value)
{
    uae_log("apply option %s = %s\n", name, value);
    amiga_parse_option_changed_fixme(name, value, 0);

    if (strcmp(name, "quickstart") == 0) {
        uae_log("Resetting Amiga after running quickstart command\n");
        uae_reset(1, 1);
    }
}

void uae_fs_apply_pending_config_changes(void)
{
    fsemu_python_apply_options(apply_option);
}

// -----------------
// The timing trace's spans on the emulation thread between fsemu's own (the frame start, its start
// gate and the end wait): the emulation of the frame's lines, WinUAE's framewait in the middle of
// them (where the previous frame's picture is posted), and the frame boundary's work after the end
// wait (messages, the configuration push, options, and the core's own work up to the next frame's
// start). The scanline pacer's line waits and blocks of extra CPU time are not spans of their own
// (there are hundreds per frame): at the frame's end, it records how many lines waited, the longest
// stretch of the frame's emulation without a wait (a line wait, a block of extra CPU time or the
// framewait), the blocks of extra CPU time (with "fastest possible" CPU speed), how late the
// emulation of the line went on after those that ran to their line's planned time, and how many
// lines began with the CPU stopped. Its times and durations are in nanoseconds; the trace's named
// values are in microseconds, so those are converted at its calls.

static struct {
    int64_t emulation_began_ns;
    int64_t framewait_began_ns;
    int64_t boundary_began_ns;
    // The frame's line waits, when the emulation since the last wait began, and the frame's
    // longest emulation without a wait (the trace's "longest emulation").
    int line_waits;
    int64_t unwaited_began_ns;
    int64_t longest_emulation_ns;
    // Whether the frame's CPU speed is "fastest possible", where the lines can get extra CPU time,
    // its blocks of extra CPU time, their total and when the current one began (0: none) (the
    // trace's "extra CPU time"), and how late the emulation of the line went on after each block
    // that ran to its line's planned time (the trace's "extra CPU late", uae_fs_end_extra_cpu_late).
    bool extra_cpu;
    int extra_cpu_blocks;
    int64_t extra_cpu_ns;
    int64_t extra_cpu_began_ns;
    std::vector<int64_t> extra_cpu_late_ns;
    // The frame's lines that began with the CPU stopped (the trace's "CPU stopped lines").
    int cpu_stopped_lines;
} uae_fs_trace;

static void uae_fs_trace_begin_unwaited(int64_t now_ns)
{
    uae_fs_trace.unwaited_began_ns = now_ns;
}

static void uae_fs_trace_end_unwaited(int64_t now_ns)
{
    if (uae_fs_trace.unwaited_began_ns != 0) {
        int64_t unwaited_ns = now_ns - uae_fs_trace.unwaited_began_ns;
        if (unwaited_ns > uae_fs_trace.longest_emulation_ns) {
            uae_fs_trace.longest_emulation_ns = unwaited_ns;
        }
        uae_fs_trace.unwaited_began_ns = 0;
    }
}

static void uae_fs_trace_end_emulation(int64_t now_ns)
{
    if (uae_fs_trace.emulation_began_ns != 0) {
        FS_AddTraceSpan("emulation", uae_fs_trace.emulation_began_ns, now_ns,
                        fsemu_frame_get_number(), FS_TRACE_NO_VALUE, 0);
        uae_fs_trace.emulation_began_ns = 0;
    }
}

// The performance graph's background stripe (fsemu-stripe.h): posting a slice and WinUAE's framewait
// are other work, after which the emulation thread returns to what it was doing before.
static struct {
    fsemu_stripe_category_t before_slice;
    fsemu_stripe_category_t before_framewait;
} uae_fs_stripe;

void uae_fs_begin_slice(void)
{
    uae_fs_stripe.before_slice = fsemu_stripe_push(FSEMU_STRIPE_CATEGORY_OTHER_WORK, FSEMU_STRIPE_NOW);
}

void uae_fs_end_slice(void)
{
    fsemu_stripe_pop(uae_fs_stripe.before_slice, FSEMU_STRIPE_NOW);
}

void uae_fs_on_frame_lines_done(void)
{
    fsemu_frame_on_emulation_done();
}

void uae_fs_begin_framewait(void)
{
    uae_fs_stripe.before_framewait =
        fsemu_stripe_push(FSEMU_STRIPE_CATEGORY_OTHER_WORK, FSEMU_STRIPE_NOW);
    if (FS_IsTraceEnabled()) {
        int64_t now_ns = (int64_t)SDL_GetTicksNS();
        uae_fs_trace_end_emulation(now_ns);
        uae_fs_trace_end_unwaited(now_ns);
        uae_fs_trace.framewait_began_ns = now_ns;
    }
}

void uae_fs_end_framewait(void)
{
    fsemu_stripe_pop(uae_fs_stripe.before_framewait, FSEMU_STRIPE_NOW);
    if (FS_IsTraceEnabled()) {
        int64_t now_ns = (int64_t)SDL_GetTicksNS();
        FS_AddTraceSpan("WinUAE framewait", uae_fs_trace.framewait_began_ns, now_ns,
                        fsemu_frame_get_number(), FS_TRACE_NO_VALUE, 0);
        uae_fs_trace.emulation_began_ns = now_ns;
        uae_fs_trace_begin_unwaited(now_ns);
    }
}

// -----------------
// The scanline pacer: fsemu alone paces the emulation, line by line, on its frame timeline.
// WinUAE's framewait and line sync are bypassed under FSUAE (custom.cpp). Its times are in
// nanoseconds, on fsemu's timeline, as the performance graph's stripe takes them.

static struct {
    // "Fastest possible" CPU speed: the CPU runs on, with the chipset stopped, until this time.
    int64_t extra_cpu_until_ns;
    // Extra CPU late: after a block of extra CPU time that ran to its line's planned time, the
    // line is emulated late from that time until the next line starts (or the frame ends); 0 when
    // the emulation is not late. Only kept for the timing trace and the performance graph's stripe.
    int64_t extra_cpu_late_since_ns;
} uae_fs_pacer;

// A block of extra CPU time ran to its line's planned time: the line is emulated late from that
// time on. The deadline is checked only every few calls of do_cycles_slow (event_check_vsync), and
// after the block the CPU still runs its pissoff budget before the chipset goes on.
static void uae_fs_begin_extra_cpu_late(void)
{
    uae_fs_pacer.extra_cpu_late_since_ns = uae_fs_pacer.extra_cpu_until_ns;
    fsemu_stripe_set(FSEMU_STRIPE_CATEGORY_EXTRA_CPU_LATE, uae_fs_pacer.extra_cpu_late_since_ns);
}

// The next line starts, or the frame ends, at now_ns: the end of extra CPU late, if the emulation
// was late, measured from the line's planned time (with the line's own emulation).
static void uae_fs_end_extra_cpu_late(int64_t now_ns)
{
    if (uae_fs_pacer.extra_cpu_late_since_ns == 0) {
        return;
    }
    if (FS_IsTraceEnabled()) {
        uae_fs_trace.extra_cpu_late_ns.push_back(now_ns - uae_fs_pacer.extra_cpu_late_since_ns);
    }
    uae_fs_pacer.extra_cpu_late_since_ns = 0;
}

// The planned time of line `line` of `lines` (fsemu_frame_get_line_planned_ns), or false while
// warping, which is not paced.
static bool uae_fs_get_line_planned_ns(int line, int lines, int64_t *until_ns)
{
    if (fsemu_frame_warping()) {
        return false;
    }
    *until_ns = fsemu_frame_get_line_planned_ns(line, lines);
    return true;
}

void uae_fs_wait_for_line(int line, int lines)
{
    int64_t until_ns;
    if (!uae_fs_get_line_planned_ns(line, lines, &until_ns)) {
        return;
    }
    int64_t now_ns = (int64_t)SDL_GetTicksNS();
    if (now_ns >= until_ns) {
        // A line already past its time does not wait.
        return;
    }
    // Busy-waits by default, as fsemu's end wait does.
    fsemu_frame_wait_until_ns(until_ns);
    if (FS_IsTraceEnabled()) {
        uae_fs_trace_end_unwaited(now_ns);
        uae_fs_trace.line_waits += 1;
        uae_fs_trace_begin_unwaited((int64_t)SDL_GetTicksNS());
    }
}

bool uae_fs_begin_line_extra_cpu(int line, int lines)
{
    int64_t until_ns;
    if (!uae_fs_get_line_planned_ns(line, lines, &until_ns)) {
        return false;
    }
    int64_t now_ns = (int64_t)SDL_GetTicksNS();
    if (until_ns - now_ns <= UAE_FS_EXTRA_CPU_MIN_NS) {
        return false;
    }
    uae_fs_pacer.extra_cpu_until_ns = until_ns;
    fsemu_stripe_set(FSEMU_STRIPE_CATEGORY_EXTRA_CPU, now_ns);
    if (FS_IsTraceEnabled()) {
        uae_fs_trace_end_unwaited(now_ns);
        uae_fs_trace.extra_cpu_began_ns = now_ns;
    }
    return true;
}

bool uae_fs_has_line_extra_cpu_time(void)
{
    return (int64_t)SDL_GetTicksNS() < uae_fs_pacer.extra_cpu_until_ns;
}

void uae_fs_end_line_extra_cpu(void)
{
    if (!fsemu_stripe_is_recording() && !FS_IsTraceEnabled()) {
        return;
    }
    int64_t now_ns = (int64_t)SDL_GetTicksNS();
    if (now_ns >= uae_fs_pacer.extra_cpu_until_ns) {
        // Ran to the line's planned time: late from then on.
        uae_fs_begin_extra_cpu_late();
    } else {
        fsemu_stripe_set(FSEMU_STRIPE_CATEGORY_EMULATING, now_ns);
    }
    if (!FS_IsTraceEnabled() || uae_fs_trace.extra_cpu_began_ns == 0) {
        return;
    }
    uae_fs_trace.extra_cpu_blocks += 1;
    uae_fs_trace.extra_cpu_ns += now_ns - uae_fs_trace.extra_cpu_began_ns;
    uae_fs_trace.extra_cpu_began_ns = 0;
    uae_fs_trace_begin_unwaited(now_ns);
}

void uae_fs_on_line_start(bool cpu_stopped, bool fastest)
{
    if (fastest) {
        uae_fs_trace.extra_cpu = true;
    }
    if (cpu_stopped) {
        uae_fs_trace.cpu_stopped_lines += 1;
    }
    if (uae_fs_pacer.extra_cpu_late_since_ns != 0) {
        int64_t now_ns = (int64_t)SDL_GetTicksNS();
        uae_fs_end_extra_cpu_late(now_ns);
        // The stripe: the line is emulated (at the frame's end, its end work has taken over).
        fsemu_stripe_set(FSEMU_STRIPE_CATEGORY_EMULATING, now_ns);
    }
}

// At the frame's end: its extra CPU time, how late its blocks ended, and its lines that began with
// the CPU stopped.
static void uae_fs_trace_add_pacer_instants(int64_t now_ns, int frame)
{
    if (uae_fs_trace.extra_cpu) {
        FS_TraceValue time[] = {
            {"count", uae_fs_trace.extra_cpu_blocks},
            {"total_us", uae_fs_trace.extra_cpu_ns}};
        FS_AddTraceInstantWithValues("extra CPU time", now_ns, frame, time,
                                     (int) (sizeof(time) / sizeof(time[0])));
    }
    uae_fs_end_extra_cpu_late(now_ns);
    std::vector<int64_t> *late_ns = &uae_fs_trace.extra_cpu_late_ns;
    if (!late_ns->empty()) {
        std::nth_element(late_ns->begin(), late_ns->begin() + late_ns->size() / 2, late_ns->end());
        int64_t median_ns = (*late_ns)[late_ns->size() / 2];
        int64_t max_ns = *std::max_element(late_ns->begin(), late_ns->end());
        FS_TraceValue late[] = {{"median_us", median_ns}, {"max_us", max_ns}};
        FS_AddTraceInstantWithValues("extra CPU late", now_ns, frame, late,
                                     (int) (sizeof(late) / sizeof(late[0])));
    }
    FS_AddTraceInstant("CPU stopped lines", now_ns, frame, "count",
                       uae_fs_trace.cpu_stopped_lines);
}

// -----------------

// Whether a savestate was loaded or saved in this frame: the audio rate adjustment's controller is
// held for it (a save runs in the frame, a load in the core's reset loop after it).
static bool uae_fs_savestate_frame;

void uae_fs_begin_frame(float vblank_hz)
{
    if (FS_IsTraceEnabled() && uae_fs_trace.boundary_began_ns != 0) {
        FS_AddTraceSpan("frame boundary", uae_fs_trace.boundary_began_ns,
                        (int64_t)SDL_GetTicksNS(), fsemu_frame_get_number(),
                        FS_TRACE_NO_VALUE, 0);
        uae_fs_trace.boundary_began_ns = 0;
    }

    // fsemu_frame_update_timing(vblank_hz, currprefs.turbo_emulation);
    // printf("vblank_hz = %0.2f\n", vblank_hz);

    // WinUAE's warp mode is not paced by fsemu either (neither the lines nor the frame's end).
    fsemu_frame_set_core_warping(currprefs.turbo_emulation != 0);
    fsemu_frame_start(vblank_hz);
    // The audio rate adjustment fsemu decided at the frame's start, applied to Paula's sample clock.
    amiga_set_audio_rate_adjustment(fsemu_audio_get_rate_adjustment());

#if 0
    // Now we sleep until the start of the next frame. Using the less
    // accurate sleep function to use less system resources. Accuracy is
    // not that important right here.
    int64_t now_ns = fsapp_time_sleep_until_ns(fsemu_frame_begin_ns);
    fsemu_frame_add_sleep_time(now_ns);
#endif

    int slot;
    if (fsemu_frame_check_load_state(&slot)) {
        uae_fs_savestate_frame = true;
        fsemu_frame_log_epoch("Load state %d\n", slot);
        if (slot == 0) {
            uae_log("Slot 0 not supported yet\n");
        } else {
            amiga_send_input_event(INPUTEVENT_SPC_STATERESTORE1 - 1 + slot, 1);
        }
    }
    if (fsemu_frame_check_save_state(&slot)) {
        uae_fs_savestate_frame = true;
        fsemu_frame_log_epoch("Save state %d\n", slot);
        if (slot == 0) {
            uae_log("Slot 0 not supported yet\n");
        } else {
            // FIXME: Going to save state; store state slot number in a
            // global variable - this should signal that the video module
            // should save a screenshot copy that can be saved together with
            // the savestate.
            amiga_send_input_event(INPUTEVENT_SPC_STATESAVE1 - 1 + slot, 1);
        }
    }

    if (FS_IsTraceEnabled()) {
        int64_t now_ns = (int64_t)SDL_GetTicksNS();
        uae_fs_trace.emulation_began_ns = now_ns;
        uae_fs_trace.line_waits = 0;
        uae_fs_trace.longest_emulation_ns = 0;
        uae_fs_trace_begin_unwaited(now_ns);
    }
    uae_fs_trace.extra_cpu = false;
    uae_fs_trace.extra_cpu_blocks = 0;
    uae_fs_trace.extra_cpu_ns = 0;
    uae_fs_trace.extra_cpu_began_ns = 0;
    uae_fs_pacer.extra_cpu_late_since_ns = 0;
    uae_fs_trace.extra_cpu_late_ns.clear();
    uae_fs_trace.cpu_stopped_lines = 0;
}

#include "fsapp-events.h"
#include "fsuae-main.h"
#include "fsuae-messages.h"
#include "savestate.h" // for save_configuration

void uae_fs_end_frame()
{
    if (FS_IsTraceEnabled()) {
        int64_t now_ns = (int64_t)SDL_GetTicksNS();
        uae_fs_trace_end_emulation(now_ns);
        uae_fs_trace_end_unwaited(now_ns);
        int frame = fsemu_frame_get_number();
        if (frame >= 0) {
            FS_AddTraceInstant("line waits", now_ns, frame, "count", uae_fs_trace.line_waits);
            FS_AddTraceInstant("longest emulation", now_ns, frame, "longest_us",
                               uae_fs_trace.longest_emulation_ns);
            uae_fs_trace_add_pacer_instants(now_ns, frame);
        }
    }
    // Paula's partly filled buffer counts in the audio buffer level fsemu measures at the end.
    fsemu_audio_set_pending_bytes(amiga_get_pending_audio_bytes());
    // The level measured at the end of a frame with a savestate loaded or saved says nothing about
    // the clocks.
    fsemu_audio_set_rate_adjustment_held(uae_fs_savestate_frame || savestate_state != 0);
    uae_fs_savestate_frame = false;
    fsemu_frame_end();
    if (FS_IsTraceEnabled()) {
        uae_fs_trace.boundary_began_ns = (int64_t)SDL_GetTicksNS();
    }

    fsuae_main_log_test_lines();

    // FIXME: Make into a callback instead (later)
    fsuae_messages_process();

    // The main thread asks the emulation to stop (window closed, quit shortcut) by setting the
    // fsemu quit flag. uae_quit is called here, on the emulation thread, so the core can end
    // m68k_go and run its normal shutdown (including the filesystem flush in amiga_main).
    static bool quit_requested = false;
    if (!quit_requested && fsemu_quit_check()) {
        quit_requested = true;
        uae_log("Quit requested by the main thread, calling uae_quit\n");
        uae_quit();
    }

    // FIXME: THREADING / SYNC
    // FIXME: Do we want to do this at another point in time, Or, even better,
    // maybe have fsapp main lopp - after rendering a frame - wait until
    // the fs emu frame is complete and any post frame handlers (?) are done.
    // Checking/updating config copy might be one such task.

    // FIXME: PERFORMANCE
    // FIXME: Maybe replace this function with some possibly more efficient
    size_t length;
    //char *config = (char*) save_configuration(&length, false);
    char *config = (char*) save_configuration(&length, true);

    // FIXME: This will do (another) copy of the data, avoid this later by
    // including a free function to free the strings?
    fsapp_events_push_string("UAE_CONFIG", config);

    free(config);

    // FIXME: Additionally, Python will make another copy...!!
    // Maybe use ctypes.c_char_p somehow, or copy/parse into dict before
    // passing onto python? Buffer protocol?
}
