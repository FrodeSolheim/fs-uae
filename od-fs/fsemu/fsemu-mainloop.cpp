#define FSEMU_INTERNAL
#include "fsemu-mainloop.h"

#include "fsapp-input.h"
#include "fsapp-main.h"
#include "fsapp-mainthread.h"
#include "fsapp-snapshot.h"
#include "fsapp-surface.h"
#include "fsapp-time.h"
#include "fs/trace.h"
#include "fsemu-control.h"
#include "fsemu-frame.h"
#include "fsemu-frameinfo.h"
#include "fsemu-input.h"
#include "fsemu-layout.h"
#include "fsemu-mouse.h"
#include "fsemu-perfgui.h"
#include "fsemu-sdlinput.h"
#include "fsemu-slotplanner.h"
#include "fsemu-testactions.h"
#include "fsemu-verticalblank.h"
#include "fsemu-video.h"

// The emulation surface, created by fsemu_mainloop_init and kept for the life of the process.
static fsapp_surface_t *fsemu_mainloop_surface;

// Set on the drawing side once the video is initialised (fsemu_mainloop_video_initialized).
static SDL_AtomicInt fsemu_mainloop_video_initialized_flag;

// Mod+S: the integer fit policy on, or back to the fit policy it replaced (fit, or stretch with the
// FILL_SCREEN stretch mode).
static void fsemu_mainloop_toggle_integer_fit(void)
{
    // Only the event hook (the host window side) uses this.
    static fsemu_layout_fit_policy_t policy_before_integer = FSEMU_LAYOUT_FIT_POLICY_FIT;
    fsemu_layout_fit_policy_t policy = fsemu_layout_get_fit_policy();
    if (policy == FSEMU_LAYOUT_FIT_POLICY_INTEGER) {
        fsemu_layout_set_fit_policy(policy_before_integer);
    } else {
        policy_before_integer = policy;
        fsemu_layout_set_fit_policy(FSEMU_LAYOUT_FIT_POLICY_INTEGER);
    }
}

// Mod+C: the auto crop policy on or off.
static void fsemu_mainloop_toggle_auto_crop(void)
{
    if (fsemu_layout_get_crop_policy() == FSEMU_LAYOUT_CROP_POLICY_AUTO) {
        fsemu_layout_set_crop_policy(FSEMU_LAYOUT_CROP_POLICY_FIXED);
    } else {
        fsemu_layout_set_crop_policy(FSEMU_LAYOUT_CROP_POLICY_AUTO);
    }
}

void fsemu_mainloop_handle_event(SDL_Event *event)
{
    fsemu_sdlinput_handle_event(event);
}

// fsemu's shortcuts (Mod+P, Mod+S, Mod+C and Mod+O), called on the host window side.

static void fsemu_mainloop_toggle_paused_shortcut(void *data)
{
    // As in FS-UAE 4 (Mod+P).
    fsemu_control_toggle_paused();
}

static void fsemu_mainloop_toggle_integer_fit_shortcut(void *data)
{
    fsemu_mainloop_toggle_integer_fit();
}

static void fsemu_mainloop_toggle_auto_crop_shortcut(void *data)
{
    fsemu_mainloop_toggle_auto_crop();
}

static void fsemu_mainloop_toggle_perfgui_mode_shortcut(void *data)
{
    fsemu_perfgui_toggle_mode();
}

static void fsemu_mainloop_register_shortcuts(void)
{
    SDL_Keymod mod = fsapp_main_get_shortcut_mod();
    fsapp_main_register_shortcut(SDLK_P, mod, fsemu_mainloop_toggle_paused_shortcut, NULL);
    fsapp_main_register_shortcut(SDLK_S, mod, fsemu_mainloop_toggle_integer_fit_shortcut, NULL);
    fsapp_main_register_shortcut(SDLK_C, mod, fsemu_mainloop_toggle_auto_crop_shortcut, NULL);
    fsapp_main_register_shortcut(SDLK_O, mod, fsemu_mainloop_toggle_perfgui_mode_shortcut, NULL);
}

// Mouse input for the emulation: only while the mouse is in relative mode (the input grab). The
// middle button is fsapp's (it toggles the input grab).
static void fsemu_mainloop_handle_mouse_event(SDL_Event *event)
{
    fsemu_mouse_event_t mouse_event;

    switch (event->type) {
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (event->button.button == SDL_BUTTON_MIDDLE) {
            break;
        }
        fsemu_mouse_log_debug("%s button=%d\n",
                              event->button.down ? "SDL_MOUSEBUTTONDOWN" : "SDL_MOUSEBUTTONUP",
                              event->button.button);
        memset(&mouse_event, 0, sizeof(fsemu_mouse_event_t));
        mouse_event.pressed = event->button.down ? event->button.button : 0;
        mouse_event.released = event->button.down ? 0 : event->button.button;
        mouse_event.x = event->button.x;
        mouse_event.y = event->button.y;
        mouse_event.button = event->button.button;
        mouse_event.state = event->button.down;
        // Routing gives the hook mouse input only while the input grab is applied, and the click
        // on the Amiga, which comes before relative mouse mode is set.
        fsemu_mouse_handle_mouse(&mouse_event);
        break;

    case SDL_EVENT_MOUSE_MOTION:
        fsemu_mouse_log_debug("SDL_MOUSEMOTION x=%g y=%g xrel=%g yrel=%g\n", event->motion.x,
                              event->motion.y, event->motion.xrel, event->motion.yrel);

        memset(&mouse_event, 0, sizeof(fsemu_mouse_event_t));
        mouse_event.moved = true;
        mouse_event.x = event->motion.x;
        mouse_event.y = event->motion.y;
        mouse_event.rel_x = event->motion.xrel;
        mouse_event.rel_y = event->motion.yrel;
        fsemu_mouse_handle_mouse(&mouse_event);
        break;
    }
}

bool fsemu_mainloop_video_initialized(void)
{
    return SDL_GetAtomicInt(&fsemu_mainloop_video_initialized_flag);
}

static void fsemu_mainloop_init_video(const fsapp_snapshot_window_t *window)
{
    fsemu_video_init();
    fsemu_video_set_size_2(window->width, window->height, window->pixel_width,
                           window->pixel_height);
    SDL_SetAtomicInt(&fsemu_mainloop_video_initialized_flag, 1);
}

// ----------------------------------------------------------------------------
// fsemu's pacer (see fsapp-mainthread.h): it paces the main loop by the emulated frames.

// The emulation session, as the emulation thread tells it (fsemu_mainloop_set_session).
typedef enum {
    FSEMU_MAINLOOP_SESSION_NOT_STARTED,
    FSEMU_MAINLOOP_SESSION_RUNNING,
    FSEMU_MAINLOOP_SESSION_ENDED,
} fsemu_mainloop_session_t;

static struct {
    // The emulation thread decides when the session runs (fsemu_mainloop_session_t) and when it is
    // paused; fsemu wants its pacer while the session runs and is not paused
    // (fsemu_mainloop_pacer_wanted). Written by the emulation thread (each change wakes the main
    // loop), read by the frame hook (host window side) and the pacer's hooks (drawing side), which
    // may run at the same time later, so atomic.
    SDL_AtomicInt session;
    SDL_AtomicInt paused;
    // Set when the program says that no session is coming (a launch that failed).
    SDL_AtomicInt no_session_coming;
    // Set by the frame hook when it sets the pacer, so the wait hook starts its timing afresh.
    SDL_AtomicInt restart;
    // Whether the pacer is set with fsapp (host window side).
    bool set;
    // The drawing side's: when the wait hook last returned, whether with new content, and whether
    // the pacer was still wanted then (not paused or ended), and whether any new content was
    // returned since the pacer was set.
    int64_t last_return_ns;
    bool last_new_content;
    bool last_wanted;
    bool new_content_since_restart;
    // Whether the new content last returned was a held frame, swapped at its hold time, and how
    // long it was held (fsemu_mainloop_pacer_wait).
    bool last_held;
    int64_t last_held_ns;
} fsemu_mainloop_pacer;

// Whether fsemu wants its pacer: from the session's first emulated frame until pause or the
// session's end, and again on resume. A run without emulation never wants it.
static bool fsemu_mainloop_pacer_wanted(void)
{
    return SDL_GetAtomicInt(&fsemu_mainloop_pacer.session) == FSEMU_MAINLOOP_SESSION_RUNNING &&
           !SDL_GetAtomicInt(&fsemu_mainloop_pacer.paused);
}

// Whether a session is starting or running, paused or not: until the program says that no session
// is coming, or the session has ended.
static bool fsemu_mainloop_session_starting_or_running(void)
{
    switch (SDL_GetAtomicInt(&fsemu_mainloop_pacer.session)) {
    case FSEMU_MAINLOOP_SESSION_NOT_STARTED:
        return !SDL_GetAtomicInt(&fsemu_mainloop_pacer.no_session_coming);
    case FSEMU_MAINLOOP_SESSION_RUNNING:
        return true;
    default:
        return false;
    }
}

// The emulated frame period, from the emulated frame rate (50 Hz before the first frame).
static int64_t fsemu_mainloop_frame_period_ns(void)
{
    // Written by the emulation thread at each frame start; a value a frame old only changes the
    // bound below by a little.
    double hz = fsemu_frame_rate_hz();
    if (hz < 1.0) {
        hz = 50.0;
    }
    return (int64_t)((double)SDL_NS_PER_SECOND / hz);
}

// A frame held by the wait hook (fsemu_mainloop_pacer_wait), -1 when none: since when (its frame
// ready), and until when.
typedef struct {
    int frame;
    int64_t held_since_ns;
    int64_t until_ns;
} fsemu_mainloop_hold_t;

static bool fsemu_mainloop_should_hold(int frame);
static int64_t fsemu_mainloop_get_hold_until_ns(int frame);

// Ends holding a frame, which was dropped (a newer frame was ready in time) or is swapped now.
static void fsemu_mainloop_end_hold(fsemu_mainloop_hold_t *hold, int64_t now_ns, bool dropped)
{
    if (FS_IsTraceEnabled()) {
        FS_AddTraceSpan("frame held", hold->held_since_ns, now_ns, hold->frame, "dropped",
                        dropped ? 1 : 0);
    }
    hold->frame = -1;
}

// The wait hook (drawing side): takes in the slices as they come (handing them to fsapp, which
// uploads them, so the uploads stay in the wait) and returns new content at frame ready, after
// fsemu has committed the frame. The emulation thread wakes the frame wait whenever it posts a
// slice (fsemu_video_post_frame). A frame that became ready after its swap deadline is held, not
// returned: swapped now, it would be presented a refresh late, and the main thread, blocked in that
// swap, would take the next frame in after its deadline too. The wait goes on until the next frame
// is ready, which is returned instead (the held frame is dropped), or until the slot planner's hold
// time, when the held frame is returned (presented a refresh late). Either way nothing is swapped
// at the missed host vertical blank, so the frame before is repeated there. Frames without a swap
// deadline are returned as soon as they are ready. The bound: when the expected emulated frame is
// about one frame period overdue (the emulation stalls), it returns no new content (or the held
// frame), so the old content is drawn again and the window keeps updating at about the emulated
// frame rate. It returns quit when the frame wait says quit, and no new content (or the held
// frame) at once when the pacer is no longer wanted (paused, or the session has ended), after
// taking in what was posted before; the frame hook then clears the pacer at the next frame.
static fsapp_pacer_wait_result_t fsemu_mainloop_pacer_wait(void)
{
    int64_t period_ns = fsemu_mainloop_frame_period_ns();
    if (SDL_SetAtomicInt(&fsemu_mainloop_pacer.restart, 0)) {
        // The first wait since the pacer was set: an emulated frame is expected a frame period
        // from now.
        fsemu_mainloop_pacer.last_return_ns = (int64_t)SDL_GetTicksNS();
        fsemu_mainloop_pacer.last_new_content = true;
        fsemu_mainloop_pacer.new_content_since_restart = false;
    }
    // After new content, the next emulated frame is expected one frame period later, and is
    // overdue by about a frame period one period after that. After no new content (it was already
    // overdue), the old content is drawn again one frame period later.
    int64_t until_ns = fsemu_mainloop_pacer.last_return_ns +
                       (fsemu_mainloop_pacer.last_new_content ? 2 : 1) * period_ns;
    fsapp_pacer_wait_result_t result;
    bool wanted;
    fsemu_mainloop_hold_t hold = {-1, 0, 0};
    fsemu_mainloop_pacer.last_held = false;
    fsemu_mainloop_pacer.last_held_ns = 0;
    while (true) {
        // Read before taking in the slices: the emulation thread posts a paused frame's
        // predecessor before it pauses, so that frame is taken in even when pausing.
        wanted = fsemu_mainloop_pacer_wanted();
        if (fsemu_mainloop_video_initialized()) {
            // Take in the slices that have come, up to frame ready (drawing only the newest ready
            // frame; the emulation thread drops older frames from the queue when the main thread
            // falls behind).
            while (!fsemu_video_ready() && fsemu_video_work(0)) {
            }
            if (fsemu_video_ready()) {
                // Committed: fsapp draws it from now on, so a held frame is dropped.
                fsemu_video_take_frame();
                int frame = fsemu_frame_number_rendering;
                if (hold.frame >= 0) {
                    fsemu_mainloop_end_hold(&hold, (int64_t)SDL_GetTicksNS(), true);
                }
                if (wanted && fsemu_mainloop_should_hold(frame)) {
                    hold.frame = frame;
                    hold.held_since_ns = FSEMU_FRAMEINFO(frame).frame_ready_ns;
                    hold.until_ns = SDL_min(fsemu_mainloop_get_hold_until_ns(frame), until_ns);
                    continue;
                }
                result = FSAPP_PACER_NEW_CONTENT;
                break;
            }
        }
        int64_t now_ns = (int64_t)SDL_GetTicksNS();
        if (hold.frame >= 0 && (!wanted || now_ns >= hold.until_ns)) {
            // No newer frame was ready in time: the held frame is swapped now.
            fsemu_mainloop_pacer.last_held = true;
            fsemu_mainloop_pacer.last_held_ns = now_ns - hold.held_since_ns;
            fsemu_mainloop_end_hold(&hold, now_ns, false);
            result = FSAPP_PACER_NEW_CONTENT;
            break;
        }
        if (!wanted) {
            // Paused, or the session has ended: the frame hook clears the pacer at the next frame.
            result = FSAPP_PACER_NO_NEW_CONTENT;
            break;
        }
        if (now_ns >= until_ns) {
            result = FSAPP_PACER_NO_NEW_CONTENT;
            break;
        }
        // Woken (a slice was posted, or a stale wake) or the time reached: look again.
        if (fsapp_frame_wait(hold.frame >= 0 ? hold.until_ns : until_ns) == FSAPP_FRAME_WAIT_QUIT) {
            result = FSAPP_PACER_QUIT;
            break;
        }
    }
    fsemu_mainloop_pacer.last_return_ns = (int64_t)SDL_GetTicksNS();
    fsemu_mainloop_pacer.last_new_content = result == FSAPP_PACER_NEW_CONTENT;
    fsemu_mainloop_pacer.last_wanted = wanted;
    if (fsemu_mainloop_pacer.last_new_content) {
        fsemu_mainloop_pacer.new_content_since_restart = true;
    }
    return result;
}

// The predicted host vertical blank (fsemu-verticalblank.h), fed by the swapped hook from the frame
// records with vsync on. The performance graph draws from it; once it has settled, the start gate
// places the pacer's planned starts from it (fsemu_video_set_frame_vsync), and once it has
// enough observations, the emulation thread plans the slots from it
// (fsemu_mainloop_get_slot_timing). With it, the slot margin (fsemu-slotplanner.h), measured from
// the swapped frames.
static struct {
    // Counted up from any thread at a session start and on resuming
    // (fsemu_mainloop_restart_slot_timing); the swapped hook then starts the prediction and the
    // slot margin afresh.
    SDL_AtomicInt restarts;
    // The drawing side's: the restarts it has started afresh for, the prediction, the prediction
    // the slots are planned from (which goes on through the prediction's own restarts) and the
    // margin.
    int restarted;
    fsemu_verticalblank_t prediction;
    bool settled;
    fsemu_slotplanner_prediction_t planning;
    fsemu_slotplanner_margin_t margin;
    // What the drawing side publishes for the emulation thread after each swap, under the lock:
    // the restarts it had started afresh for, the prediction and the slot margin (in the slot
    // planner's nanoseconds), and the predicted host refresh interval and whether the prediction
    // has settled (fsemu_mainloop_get_predicted_host_refresh_interval_ns, which the start gate
    // reads), and whether it is steady (which the decision to follow it reads).
    SDL_SpinLock lock;
    int published_restarted;
    fsemu_slotplanner_prediction_t published_prediction;
    int64_t published_margin_ns;
    double published_interval_ns;
    bool published_settled;
    bool published_steady;
    // Whether the prediction was steady after the last swap (drawing side), to log a change.
    bool steady;
} fsemu_mainloop_vertical_blank;

// The prediction the slots are planned from (drawing side).
static void fsemu_mainloop_get_slot_prediction(fsemu_slotplanner_prediction_t *prediction)
{
    *prediction = fsemu_mainloop_vertical_blank.planning;
}

// Takes the predicted host vertical blank into the prediction the slots are planned from, after it
// was updated (drawing side). Once the slots are planned, a restart of the predicted host vertical
// blank (an early observation, a changed interval) does not hand them back to the start gate.
static void fsemu_mainloop_follow_prediction(void)
{
    const fsemu_verticalblank_t *vb = &fsemu_mainloop_vertical_blank.prediction;
    fsemu_slotplanner_prediction_t predicted = {};
    predicted.predicted_ns = vb->predicted_ns;
    predicted.interval_ns = vb->interval_ns;
    predicted.lead_ns = fsemu_verticalblank_get_lead_ns(vb);
    predicted.observations = vb->observations;
    fsemu_slotplanner_follow_prediction(&fsemu_mainloop_vertical_blank.planning, &predicted);
}

// Whether the wait hook holds a frame that is ready (drawing side): one that became ready after
// its swap deadline. Frames without one (the start gate's, and every frame when the emulation does
// not follow the host vertical blank) are never held.
static bool fsemu_mainloop_should_hold(int frame)
{
    const fsemu_frameinfo_t *info = &FSEMU_FRAMEINFO(frame);
    return info->deadline_ns != 0 && info->frame_ready_ns > info->deadline_ns;
}

// Until when the wait hook holds a frame that became ready after its swap deadline (drawing side):
// the slot planner's hold time, before the next frame's swap deadline.
static int64_t fsemu_mainloop_get_hold_until_ns(int frame)
{
    fsemu_slotplanner_prediction_t prediction;
    fsemu_mainloop_get_slot_prediction(&prediction);
    return fsemu_slotplanner_margin_get_hold_until_ns(
        &fsemu_mainloop_vertical_blank.margin, &prediction, FSEMU_FRAMEINFO(frame).deadline_ns);
}

// Sets up the slot margin afresh (fsemu_slotplanner_margin_init), fixed for whole-frame slots.
static void fsemu_mainloop_init_slot_margin(void)
{
    fsemu_slotplanner_margin_init(&fsemu_mainloop_vertical_blank.margin);
    fsemu_slotplanner_margin_set_whole_frame_slots(&fsemu_mainloop_vertical_blank.margin,
                                                   fsemu_frame_has_whole_frame_slots());
}

// Publishes the prediction and the slot margin for the emulation thread (drawing side).
static void fsemu_mainloop_publish_slot_timing(void)
{
    fsemu_slotplanner_prediction_t prediction;
    fsemu_mainloop_get_slot_prediction(&prediction);
    int64_t margin_ns =
        fsemu_slotplanner_margin_get_slot_margin_ns(&fsemu_mainloop_vertical_blank.margin);
    SDL_LockSpinlock(&fsemu_mainloop_vertical_blank.lock);
    fsemu_mainloop_vertical_blank.published_restarted = fsemu_mainloop_vertical_blank.restarted;
    fsemu_mainloop_vertical_blank.published_prediction = prediction;
    fsemu_mainloop_vertical_blank.published_margin_ns = margin_ns;
    fsemu_mainloop_vertical_blank.published_interval_ns =
        fsemu_mainloop_vertical_blank.prediction.interval_ns;
    fsemu_mainloop_vertical_blank.published_settled = fsemu_mainloop_vertical_blank.settled;
    fsemu_mainloop_vertical_blank.published_steady = fsemu_mainloop_vertical_blank.steady;
    SDL_UnlockSpinlock(&fsemu_mainloop_vertical_blank.lock);
}

void fsemu_mainloop_restart_slot_timing(void)
{
    SDL_AddAtomicInt(&fsemu_mainloop_vertical_blank.restarts, 1);
}

bool fsemu_mainloop_get_slot_timing(fsemu_slotplanner_prediction_t *prediction, int64_t *margin_ns)
{
    int restarts = SDL_GetAtomicInt(&fsemu_mainloop_vertical_blank.restarts);
    SDL_LockSpinlock(&fsemu_mainloop_vertical_blank.lock);
    bool current = fsemu_mainloop_vertical_blank.published_restarted == restarts;
    *prediction = fsemu_mainloop_vertical_blank.published_prediction;
    *margin_ns = fsemu_mainloop_vertical_blank.published_margin_ns;
    SDL_UnlockSpinlock(&fsemu_mainloop_vertical_blank.lock);
    return current;
}

// Feeds the slot margin with a swapped emulated frame (drawing side, the swapped hook): its swap
// deadline (none for the start gate's frames), emulation done and frame ready from its frame info,
// rendering finished and the host vertical blank from the swap's frame record, and whether it was
// held, and how long. Traces the margin when it changes.
static void fsemu_mainloop_measure_slot_margin(int frame, const fsapp_frame_record_t *record,
                                               int64_t vertical_blank_ns, bool held,
                                               int64_t held_ns)
{
    const fsemu_frameinfo_t *info = &FSEMU_FRAMEINFO(frame);
    if (info->emulation_done_ns == 0 || info->frame_ready_ns == 0) {
        return;
    }
    fsemu_slotplanner_frame_t swapped = {};
    swapped.deadline_ns = info->deadline_ns;
    // REVIEW: A held frame's times are measured without the hold, as if it had been ready when the
    // hold ended: they are to say what the host needs. With the hold in them, the drawing time
    // (and so the hold time) would grow with each held frame swapped late, and the frames after it
    // would be held and swapped late too.
    swapped.emulation_done_ns = info->emulation_done_ns + held_ns;
    swapped.frame_ready_ns = info->frame_ready_ns + held_ns;
    swapped.rendering_finished_ns = record->rendering_finished_ns;
    swapped.vertical_blank_ns = vertical_blank_ns;
    swapped.held = held;
    fsemu_slotplanner_prediction_t prediction;
    fsemu_mainloop_get_slot_prediction(&prediction);
    fsemu_slotplanner_margin_t *margin = &fsemu_mainloop_vertical_blank.margin;
    int64_t before_ns = fsemu_slotplanner_margin_get_slot_margin_ns(margin);
    fsemu_slotplanner_margin_on_swapped(margin, &prediction, &swapped);
    int64_t margin_ns = fsemu_slotplanner_margin_get_slot_margin_ns(margin);
    if (margin_ns != before_ns && FS_IsTraceEnabled()) {
        FS_AddTraceInstant("slot margin", record->swap_ns, frame, "margin_us", margin_ns);
    }
}

// Whether a reported presentation was shown on the display, to be fed to the predicted host
// vertical blank: on Wayland one with the vsync kind bit (shown at a vertical blank, not as soon as
// possible), on X11 a flip or a copy (not skipped); not a discarded one.
static bool fsemu_mainloop_is_presentation_shown(const fsapp_frame_presented_t *presented)
{
    if (presented->presented_ns == 0) {
        return false;
    }
    if (presented->mode == FSAPP_FRAME_PRESENTED_MODE_NONE) {
        return (presented->flags & FSAPP_FRAME_PRESENTED_VSYNC) != 0;
    }
    return presented->mode == FSAPP_FRAME_PRESENTED_MODE_FLIP ||
           presented->mode == FSAPP_FRAME_PRESENTED_MODE_COPY ||
           presented->mode == FSAPP_FRAME_PRESENTED_MODE_SUBOPTIMAL_COPY;
}

// Feeds the predicted host vertical blank with the presentations of a swap's frame record (those
// shown) and its host vertical blank, the swap return (drawing side, the swapped hook). With vsync
// off, the swaps do not wait for the host vertical blank, so the record's estimate (the swap plus
// one refresh period) does not say where it is. Traces the prediction against the record's host
// vertical blank, with the compositor's lead, and logs when it has settled and when it stops or
// starts being steady.
static void fsemu_mainloop_predict_vertical_blank(const fsapp_frame_record_t *record)
{
    fsemu_verticalblank_t *prediction = &fsemu_mainloop_vertical_blank.prediction;
    int restarts = SDL_GetAtomicInt(&fsemu_mainloop_vertical_blank.restarts);
    if (restarts != fsemu_mainloop_vertical_blank.restarted) {
        // The learned slot margin goes with the prediction it was measured against.
        fsemu_verticalblank_reset(prediction);
        fsemu_mainloop_vertical_blank.settled = false;
        fsemu_mainloop_vertical_blank.steady = true;
        fsemu_slotplanner_follow_prediction(&fsemu_mainloop_vertical_blank.planning, NULL);
        fsemu_mainloop_init_slot_margin();
        fsemu_mainloop_vertical_blank.restarted = restarts;
    }
    if (!fsemu_video_vsync() ||
        record->host_vertical_blank == FSAPP_FRAME_HOST_VERTICAL_BLANK_UNKNOWN) {
        return;
    }
    // The presentations reported since the record before (of earlier swaps), oldest first, before
    // this swap's return.
    for (int i = 0; i < record->presented_count; i++) {
        const fsapp_frame_presented_t *presented = &record->presented[i];
        if (fsemu_mainloop_is_presentation_shown(presented)) {
            fsemu_verticalblank_add_presentation(prediction, presented->presented_ns,
                                                 presented->refresh_ns);
        }
    }
    int64_t observed_ns = record->host_vertical_blank_ns;
    // The predicted host vertical blank nearest the observed one, before it is fed.
    int64_t predicted_ns = fsemu_verticalblank_get_predicted_ns(
        prediction, observed_ns - (int64_t)(prediction->interval_ns / 2));
    int64_t observed_after_ns = observed_ns - predicted_ns;
    fsemu_verticalblank_update(prediction, observed_ns);
    fsemu_mainloop_follow_prediction();
    bool settled = fsemu_verticalblank_is_settled(prediction);
    bool steady = fsemu_verticalblank_is_steady(prediction);
    if (predicted_ns != 0 && FS_IsTraceEnabled()) {
        FS_TraceValue values[] = {
            {"observed_after_us", observed_after_ns},
            {"settled", settled ? 1 : 0},
            {"lead_us", fsemu_verticalblank_get_lead_ns(prediction)},
            {"presentations", fsemu_verticalblank_is_from_presentations(prediction) ? 1 : 0}};
        FS_AddTraceInstantWithValues("predicted host vertical blank", predicted_ns,
                                     FS_TRACE_NO_FRAME, values, 4);
    }
    if (settled && !fsemu_mainloop_vertical_blank.settled) {
        fsemu_video_log("The predicted host vertical blank has settled (interval %0.1f us, swap "
                        "%lld observed %+lld us after it, lead %lld us, %s)\n",
                        prediction->interval_ns / SDL_NS_PER_US, (long long)record->swap_number,
                        (long long)SDL_NS_TO_US(observed_after_ns),
                        (long long)SDL_NS_TO_US(fsemu_verticalblank_get_lead_ns(prediction)),
                        fsemu_verticalblank_is_from_presentations(prediction)
                            ? "on the presentations"
                            : "on the swap returns");
    }
    if (steady != fsemu_mainloop_vertical_blank.steady) {
        fsemu_video_log("The predicted host vertical blank is %s (swap %lld)\n",
                        steady ? "steady again" : "not steady", (long long)record->swap_number);
        if (FS_IsTraceEnabled()) {
            FS_AddTraceInstant("predicted host vertical blank steadiness", record->swap_ns,
                               FS_TRACE_NO_FRAME, "steady", steady ? 1 : 0);
        }
    }
    fsemu_mainloop_vertical_blank.settled = settled;
    fsemu_mainloop_vertical_blank.steady = steady;
}

int64_t fsemu_mainloop_get_predicted_host_vertical_blank_ns(int64_t at_or_after_ns)
{
    return fsemu_verticalblank_get_predicted_ns(&fsemu_mainloop_vertical_blank.prediction,
                                                at_or_after_ns);
}

double fsemu_mainloop_get_predicted_host_refresh_interval_ns(void)
{
    SDL_LockSpinlock(&fsemu_mainloop_vertical_blank.lock);
    double interval_ns = fsemu_mainloop_vertical_blank.published_interval_ns;
    SDL_UnlockSpinlock(&fsemu_mainloop_vertical_blank.lock);
    return interval_ns;
}

bool fsemu_mainloop_is_predicted_host_vertical_blank_settled(void)
{
    SDL_LockSpinlock(&fsemu_mainloop_vertical_blank.lock);
    bool settled = fsemu_mainloop_vertical_blank.published_settled;
    SDL_UnlockSpinlock(&fsemu_mainloop_vertical_blank.lock);
    return settled;
}

bool fsemu_mainloop_is_predicted_host_vertical_blank_steady(void)
{
    SDL_LockSpinlock(&fsemu_mainloop_vertical_blank.lock);
    bool steady = fsemu_mainloop_vertical_blank.published_steady;
    SDL_UnlockSpinlock(&fsemu_mainloop_vertical_blank.lock);
    return steady;
}

// The latest swap for the predicted host vertical blank nearest vsync_ns (the predicted host
// vertical blank minus the compositor's lead, as the swap deadlines are), once the prediction has
// settled, else 0 (drawing side). vsync_ns is a swap return, which is the lead before its host
// vertical blank.
static int64_t fsemu_mainloop_get_settled_prediction_near(int64_t vsync_ns)
{
    const fsemu_verticalblank_t *prediction = &fsemu_mainloop_vertical_blank.prediction;
    if (!fsemu_mainloop_vertical_blank.settled) {
        return 0;
    }
    int64_t lead_ns = fsemu_verticalblank_get_lead_ns(prediction);
    int64_t predicted_ns = fsemu_verticalblank_get_predicted_ns(
        prediction, vsync_ns + lead_ns - (int64_t)(prediction->interval_ns / 2));
    return predicted_ns != 0 ? predicted_ns - lead_ns : 0;
}

// The swapped hook (drawing side).
static void fsemu_mainloop_pacer_swapped(const fsapp_frame_record_t *record)
{
    // First, so that the frame's predicted host vertical blank below has this swap in it.
    fsemu_mainloop_predict_vertical_blank(record);
    if (fsemu_mainloop_pacer.last_new_content && fsemu_video_vsync()) {
        // With vsync on, the emulation thread starts a frame only once an earlier frame has been
        // swapped (fsemu_frame_wait_until_frame_start_vsync, while it follows the host vertical
        // blank and its slots are not planned yet), from the host vertical blank (observed with a
        // glFinish() after the swap while the emulation follows it, estimated otherwise), or the
        // swap's return if it is not known.
        // The host vertical blanks also measure the host refresh rate, by which the emulation
        // decides whether to follow them, so they are recorded whether it follows or not.
        // REVIEW: Was the swap's return before the frame record had the host vertical blank; on
        // llvmpipe the two are a few microseconds apart.
        int64_t vsync_ns = record->host_vertical_blank != FSAPP_FRAME_HOST_VERTICAL_BLANK_UNKNOWN
                               ? record->host_vertical_blank_ns
                               : record->swap_ns;
        // The start gate places the planned start from the prediction once it has settled, so that
        // a late swap (seen on macOS after a while without input) does not move the planned start.
        fsemu_video_set_frame_vsync(fsemu_frame_number_rendering, vsync_ns,
                                    fsemu_mainloop_get_settled_prediction_near(vsync_ns));
    }
    if (fsemu_mainloop_pacer.last_new_content) {
        // The swap that first presented the frame.
        fsemu_video_set_frame_rendered(fsemu_frame_number_rendering, record->rendering_finished_ns);
        fsemu_video_set_frame_swapped(fsemu_frame_number_rendering, record->swap_ns);
        if (fsemu_video_vsync() &&
            record->host_vertical_blank != FSAPP_FRAME_HOST_VERTICAL_BLANK_UNKNOWN) {
            fsemu_mainloop_measure_slot_margin(
                fsemu_frame_number_rendering, record, record->host_vertical_blank_ns,
                fsemu_mainloop_pacer.last_held, fsemu_mainloop_pacer.last_held_ns);
        }
    }
    // After the prediction and the margin have taken in this swap.
    fsemu_mainloop_publish_slot_timing();
    if (fsemu_mainloop_pacer.last_new_content && FS_IsTraceEnabled()) {
        FS_AddTraceInstant("emulated frame swapped", record->swap_ns, fsemu_frame_number_rendering,
                           "swap", record->swap_number);
    }
    // The performance graph: the emulated frame this swap presented, new or again (a repeat). A
    // swap of old content while pausing (the pacer no longer wanted), or after the pacer was set
    // again (resume) before new content came, presents no emulated frame of its own. The rows are
    // drawn and committed by the frame hook (host window side).
    bool presented =
        fsemu_mainloop_pacer.last_new_content ||
        (fsemu_mainloop_pacer.last_wanted && fsemu_mainloop_pacer.new_content_since_restart);
    fsemu_perfgui_on_swapped(record, presented ? fsemu_frame_number_rendering : -1);
}

// The swap interval fsemu wants (1 with vsync on, else 0), and whether it then follows the host
// vertical blank (with interval 1, as the emulation last decided).
static void fsemu_mainloop_pacer_swap_interval(int *swap_interval, bool *follows)
{
    *swap_interval = fsemu_video_initialized() && fsemu_video_vsync() > 0 ? 1 : 0;
    *follows = *swap_interval == 1 && fsemu_frame_is_following_host_vertical_blank();
}

void fsemu_mainloop_on_following_changed(void)
{
    int swap_interval;
    bool follows;
    fsemu_mainloop_pacer_swap_interval(&swap_interval, &follows);
    fsapp_mainthread_set_pacer_swap_interval(swap_interval, follows);
}

// Sets or clears the pacer when what fsemu wants has changed (host window side).
static void fsemu_mainloop_update_pacer(void)
{
    bool wanted = fsemu_mainloop_pacer_wanted();
    if (wanted == fsemu_mainloop_pacer.set) {
        return;
    }
    if (wanted) {
        fsapp_pacer_t pacer = {};
        pacer.wait = fsemu_mainloop_pacer_wait;
        pacer.swapped = fsemu_mainloop_pacer_swapped;
        pacer.surface = fsemu_mainloop_surface;
        fsemu_mainloop_pacer_swap_interval(&pacer.swap_interval,
                                           &pacer.follows_host_vertical_blank);
        SDL_SetAtomicInt(&fsemu_mainloop_pacer.restart, 1);
        fsapp_mainthread_set_pacer(&pacer);
    } else {
        fsapp_mainthread_set_pacer(NULL);
    }
    fsemu_mainloop_pacer.set = wanted;
}

void fsemu_mainloop_on_no_session_coming(void)
{
    fsemu_video_log("No emulation session is coming\n");
    SDL_SetAtomicInt(&fsemu_mainloop_pacer.no_session_coming, 1);
    // The frame hook takes the performance graph away, also while the main loop idles.
    fsapp_frame_wake();
}

void fsemu_mainloop_set_session(bool running)
{
    fsemu_video_log("Emulation session %s\n", running ? "started" : "ended");
    if (running) {
        // The predicted host vertical blank and the slot margin start afresh with the session.
        fsemu_mainloop_restart_slot_timing();
    }
    SDL_SetAtomicInt(&fsemu_mainloop_pacer.session,
                     running ? FSEMU_MAINLOOP_SESSION_RUNNING : FSEMU_MAINLOOP_SESSION_ENDED);
    // The wait hook looks again, and the frame hook sets or clears the pacer.
    fsapp_frame_wake();
}

void fsemu_mainloop_set_paused(bool paused)
{
    fsemu_video_log("Emulation %s\n", paused ? "paused" : "resumed");
    SDL_SetAtomicInt(&fsemu_mainloop_pacer.paused, paused);
    // The wait hook looks again, and the frame hook sets or clears the pacer.
    fsapp_frame_wake();
}

// ----------------------------------------------------------------------------
// The hooks registered with fsapp's main loop, called on the main thread.

// The frame hook (host window side): fsemu's per-frame work that touches input, and setting or
// clearing its pacer, from the first frame with the host window on.
static void fsemu_mainloop_frame_hook(void)
{
    if (fsapp_mainthread_has_render_thread()) {
        // The video setup (the drawing hook) runs on the render thread, and fsemu's options are
        // read on the main thread only, so they are read here first (in one-thread mode, the
        // video setup reads them itself, as before). In render thread mode the layout, frame and
        // screenshot modules are thus set up in the first frame hook, which comes before the
        // drawing side's first frame: the render thread draws only from a snapshot that the host
        // window side wrote after its frame hooks (in the same pass or a later one), handed over
        // under the snapshot slot's lock, which orders these writes before the drawing hook's
        // reads. (Reading the options before the main loop in both modes would be simpler, but
        // would move log lines in one-thread mode.)
        fsemu_video_read_options();
    }
    if (fsemu_input_reconfigure_needed) {
        fsemu_input_reconfigure();
    }
    fsemu_mainloop_update_pacer();
    // A click on the Amiga grabs only while a session runs: from its first emulated frame to its
    // end, paused or not.
    // REVIEW: Whether this is the right signal for "a session runs".
    fsapp_input_set_session(SDL_GetAtomicInt(&fsemu_mainloop_pacer.session) ==
                            FSEMU_MAINLOOP_SESSION_RUNNING);
    fsemu_testactions_run_due();
    // The performance graph is shown while a session is starting or running, and stays (standing
    // still, with no swapped hook to feed it) while paused; it goes when the session stops. The
    // rows that the swapped hook fed are drawn and committed here, before the snapshot is written,
    // so that the drawing side's next frame takes them in (in render thread mode too).
    fsemu_perfgui_update_adornments(fsemu_mainloop_session_starting_or_running());
}

// The drawing hook (drawing side, before drawing below): the host window exists by the drawing
// side's first frame, so the video is set up then (fsemu has no GL objects: fsapp owns the
// textures of the emulation surface's streamed content). In every frame, the picture is refitted
// to the emulation surface's rectangle in this frame, also while paused, and dropped once the
// session has stopped.
// REVIEW: The drawing side's first frame comes after the UI's first frame, so the emulation thread
// (which waits for fsemu_mainloop_video_initialized) now starts after the UI is up, not before.
static void fsemu_mainloop_drawing_hook(const fsapp_snapshot_window_t *window)
{
    if (!fsemu_mainloop_video_initialized()) {
        fsemu_mainloop_init_video(window);
        // The video setup decides vsync, and with it the pacer's swap interval, which fsapp
        // applies before this frame's swap.
        int swap_interval;
        bool follows;
        fsemu_mainloop_pacer_swap_interval(&swap_interval, &follows);
        fsapp_mainthread_set_pacer_swap_interval(swap_interval, follows);
    }
    fsemu_video_update_picture(SDL_GetAtomicInt(&fsemu_mainloop_pacer.session) ==
                               FSEMU_MAINLOOP_SESSION_ENDED);
}

// The display hook (host window side).
static void fsemu_mainloop_display_hook(double refresh_rate_hz, bool every_swap_waits)
{
    // TODO: fsemu's frame timing does not follow the host refresh rate yet; it is only logged.
    fsemu_video_log("Host refresh rate %0.3f Hz, every swap waits for vertical blank: %s\n",
                    refresh_rate_hz, every_swap_waits ? "yes" : "no");
}

static void fsemu_mainloop_event_hook(SDL_Event *event)
{
    fsemu_mainloop_handle_event(event);
    fsemu_mainloop_handle_mouse_event(event);
}

void fsemu_mainloop_init(void)
{
    // Called on the main thread before the UI thread starts, so the surface list is not in use.
    // The emulated picture is the surface's streamed content (fsemu_video_work commits it), drawn
    // by fsapp; with no content (before the first frame, after the session) its fill colour shows.
    fsemu_mainloop_surface = fsapp_surface_create(0, 0, true);
    fsapp_surface_attach_streamed_content(fsemu_mainloop_surface, FSAPP_SURFACE_STREAM_BGRA);
    // Its rectangle and picture rectangle are in device pixels.
    fsapp_surface_set_scale_support(fsemu_mainloop_surface, FSAPP_PIXELS_SCALE_ANY);
    // While it has keyboard focus, the keys go to the emulation (fsapp-input.h).
    fsapp_surface_set_focusable(fsemu_mainloop_surface, true);
    fsapp_input_set_emulation_surface(fsemu_mainloop_surface);

    // The performance graph, drawn as adornments: the swapped hook feeds its rows, and the frame
    // hook draws and commits them.
    fsemu_perfgui_init();
    fsemu_verticalblank_init(&fsemu_mainloop_vertical_blank.prediction);
    fsemu_mainloop_vertical_blank.steady = true;
    fsemu_mainloop_vertical_blank.published_steady = true;
    fsemu_slotplanner_follow_prediction(&fsemu_mainloop_vertical_blank.planning, NULL);
    fsemu_mainloop_init_slot_margin();
    fsemu_testactions_init();

    fsemu_mainloop_register_shortcuts();

    // The event hook comes first, so fsemu sees each event before the program's event hooks (if
    // any), and the frame hook before the program's frame hooks.
    fsapp_mainthread_add_event_hook(fsemu_mainloop_event_hook);
    fsapp_mainthread_add_frame_hook(fsemu_mainloop_frame_hook);
    fsapp_mainthread_add_drawing_hook(fsemu_mainloop_drawing_hook);
    fsapp_mainthread_add_display_hook(fsemu_mainloop_display_hook);
}

fsapp_surface_t *fsemu_mainloop_emulation_surface(void)
{
    return fsemu_mainloop_surface;
}
