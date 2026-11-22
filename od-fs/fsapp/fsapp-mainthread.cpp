/*
 * The main loop: the frames of the host window, on the main thread, with the drawing side on the
 * render thread in render thread mode.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#include "fsapp-mainthread.h"

#include <SDL3/SDL_opengl.h>

#include <utility>
#include <vector>

#include "fsapp-adornment.h"
#include "fsapp-bitmap.h"
#include "fsapp-compositor.h"
#include "fsapp-gl.h"
#include "fsapp-gldraw.h"
#include "fsapp-input.h"
#include "fsapp-main.h"
#include "fsapp-presentation.h"
#include "fsapp-pythonthread.h"
#include "fsapp-snapshot.h"
#include "fsapp-surface.h"
#include "fsapp-testaids.h"
#include "fsapp-time.h"
#include "fs/thread.h"
#include "fs/trace.h"
#include "fsapp-window.h"

namespace
{

// A snapshot with its own copies of the arrays it points to (the surface layout's and the
// adornments'), so that it can be kept, and handed from one side to the other, apart from the
// storage it was written from.
class SnapshotCopy
{
  public:
    fsapp_snapshot_t snapshot = {};

    // Copies the snapshot and its arrays.
    void assign(const fsapp_snapshot_t &from)
    {
        snapshot = from;
        fsapp_snapshot_layout_t *layout = &snapshot.layout;
        surfaces_.assign(layout->surfaces, layout->surfaces + layout->surface_count);
        contents_.assign(layout->contents, layout->contents + layout->content_count);
        destroyed_.assign(layout->destroyed, layout->destroyed + layout->destroyed_count);
        layout->surfaces = surfaces_.data();
        layout->contents = contents_.data();
        layout->destroyed = destroyed_.data();
        fsapp_snapshot_adornments_t *adornments = &snapshot.adornments;
        adornments_.assign(adornments->adornments, adornments->adornments + adornments->count);
        adornments_destroyed_.assign(adornments->destroyed,
                                     adornments->destroyed + adornments->destroyed_count);
        adornments->adornments = adornments_.data();
        adornments->destroyed = adornments_destroyed_.data();
    }

    // Exchanges two copies without copying the arrays: a vector's storage goes with it in the
    // swap, so each snapshot still points at its own arrays.
    void swap(SnapshotCopy &other)
    {
        std::swap(snapshot, other.snapshot);
        surfaces_.swap(other.surfaces_);
        contents_.swap(other.contents_);
        destroyed_.swap(other.destroyed_);
        adornments_.swap(other.adornments_);
        adornments_destroyed_.swap(other.adornments_destroyed_);
    }

    // Clears what the snapshot hands over only once (a record with its content and destroyed
    // surfaces, the adornments' new content and destroyed adornments, and their change), so that
    // a frame drawn from it again uploads, frees and takes in nothing a second time.
    void clear_handed_over_once()
    {
        snapshot.layout.takes_record = false;
        snapshot.layout.content_count = 0;
        snapshot.layout.destroyed_count = 0;
        for (fsapp_snapshot_adornment_t &adornment : adornments_) {
            adornment.content = NULL;
            adornment.content_damage_height = 0;
        }
        snapshot.adornments.destroyed_count = 0;
        snapshot.adornments.changed = false;
    }

  private:
    std::vector<fsapp_snapshot_surface_t> surfaces_;
    std::vector<fsapp_snapshot_content_t> contents_;
    std::vector<const void *> destroyed_;
    std::vector<fsapp_snapshot_adornment_t> adornments_;
    std::vector<struct fsapp_adornment *> adornments_destroyed_;
};

} // namespace

static struct {
    fsapp_mainthread_quit_requested_t quit_requested;
    // True once fsapp_mainthread_run has started; hooks are registered before.
    bool running;
    // True while the drawing side runs on the main thread, in one-thread mode (see
    // fsapp-mainthread.h). Only used on the main thread.
    bool drawing_side;
    // The snapshot the host window side writes for the next frame (the surface layout's storage is
    // the surface module's, fsapp_surface_write_snapshot).
    fsapp_snapshot_t snapshot;
    // The pacer as set or cleared on the host window side (fsapp_mainthread_set_pacer), copied
    // into the snapshot when the next frame starts.
    fsapp_snapshot_pacer_t pacer;
    // Log every Nth frame record (FSAPP_LOG_FRAME_RECORDS=N), or none when 0.
    int log_frame_records;
} fsapp_mainthread;

// The pacer's swap interval and whether it follows the host vertical blank
// (fsapp_mainthread_set_pacer_swap_interval), which the pacer may change from any thread. While it
// follows the host vertical blank, fsapp calls glFinish() after the swap to find it.
static SDL_AtomicInt g_pacer_swap_interval;
static SDL_AtomicInt g_pacer_follows_host_vertical_blank;

// The registered hooks (see fsapp-mainthread.h), each kind in the order registered.
static struct {
    std::vector<fsapp_mainthread_event_hook_t> event;
    std::vector<fsapp_mainthread_frame_hook_t> frame;
    std::vector<fsapp_mainthread_drawing_hook_t> drawing;
    std::vector<fsapp_mainthread_display_hook_t> display;
} hooks;

// The host display as last told to the display hooks (host window side).
static struct {
    // Whether the display hooks have been called (they are, once the host window exists).
    bool known;
    // Set by the events that can change the display (host window side), so it is looked at again.
    bool changed;
    double refresh_rate_hz;
    bool every_swap_waits;
    bool refresh_rate_known;
    bool host_vertical_blank_observed;
} display;

// The platforms (SDL video drivers, by name) where the return of a glFinish() after the swap is
// taken as the observed host vertical blank: those where the performance graph has shown it to
// line up with the refresh (steady spacing of one refresh period). Elsewhere it is only an
// estimate. None yet (the list ends with NULL).
static const char *const g_host_vertical_blank_observed_drivers[] = {
    NULL,
};

// The drawing side's own state.
static struct {
    // Whether the drawing side's first frame has tried to make its state (fsapp's GL state), and
    // whether it made it. It is tried once: after a failure the program is stopping, and the frames
    // until it stops are not drawn.
    bool init_tried;
    bool initialized;
    // The snapshot taken when the frame started, with its own copies of the surface layout (the
    // flat list, the record's content and destroyed surfaces) and the adornments.
    SnapshotCopy taken;
    // The swaps counted so far (the frame record's swap number, and the GL grab's).
    int64_t swap_number;
    // The simulated display's tick that the last swap was held until (the lead before it with
    // FSAPP_SIMULATED_DISPLAY_LEAD_US), or 0 when it was not held, and the tick itself.
    int64_t simulated_vertical_blank_ns;
    int64_t simulated_tick_ns;
    // The last record taken in (for the UI frame lag): the swap of the frame that took it in, and
    // when that frame's UI upload started; swap number 0 when none has been.
    int64_t ui_frame_swap_number;
    int64_t ui_frame_ns;
    // The GL grab (fsapp_mainthread_set_gl_grab): the swap numbers to grab, where to, the next
    // swap to grab (an index into gl_grab_swaps), and whether it holds keep drawing (while a grab
    // is still to come). Set before the main loop runs, and only used by the drawing side after
    // that.
    int gl_grab_swaps[32];
    int gl_grab_count;
    char *gl_grab_dir;
    int gl_grab_next;
    bool gl_grab_keeps_drawing;
    // Whether to ask to quit after the last grab, and how long after it
    // (fsapp_mainthread_set_quit_after_gl_grab), and the timer that asks then (0 when none).
    bool gl_grab_quits;
    int64_t gl_grab_quit_idle_ns;
    SDL_TimerID gl_grab_quit_timer;
} drawing;

// Drawing without a pacer (on demand, see fsapp-mainthread.h).
static struct {
    // Set by the events that change the window (host window side), so a frame is drawn.
    bool window_changed;
    // Whether the pacer was set for the last frame drawn (host window side), so a frame is drawn
    // when it is cleared (or set).
    bool pacer_was_set;
    // When the next frame is due without a pacer (host window side): one host refresh period after
    // the last one started, or 0 (at once) after a frame with a pacer.
    int64_t next_frame_due_ns;
} on_demand;

// Keep drawing (fsapp_frame_take_keep_drawing) is held while this is above 0, and a frame was
// requested (fsapp_frame_request) while the other is 1. Both are used from any thread.
static SDL_AtomicInt g_keep_drawing;
static SDL_AtomicInt g_frame_requested;

// The frame wake (fsapp_frame_wake): the SDL event type it pushes (0 until registered, when the
// main loop starts), and whether a wake event is already queued and not yet taken off the queue,
// so that many wakes in a row push only one event.
static SDL_AtomicU32 g_frame_wake_event_type;
static SDL_AtomicInt g_frame_wake_pending;

// The swaps made so far (fsapp_mainthread_get_swap_count): the drawing side's swap number, for
// any thread.
static SDL_AtomicInt g_swap_count;

// The render thread (render thread mode, see fsapp-mainthread.h).
static struct {
    // The mode, decided once on the main thread (fsapp_mainthread_has_render_thread).
    bool decided;
    bool on;
    // The thread from when the main loop starts it (once the host window exists) until it is
    // joined, the window and the GL context it makes current (main thread).
    SDL_Thread *thread;
    SDL_Window *window;
    SDL_GLContext context;
    // Whether the main loop has started it (or tried to), main thread.
    bool start_tried;
    // Set while the render thread runs (from before it is created until it has been joined), so
    // that fsapp_frame_wake signals it.
    SDL_AtomicInt running;
    // Set when the main loop has ended: the render thread ends its frame and stops.
    SDL_AtomicInt stop;
    // Set by the render thread when its function returns, so that the main thread can run the main
    // queue (macOS) while it waits to join it (render_thread_stop).
    SDL_AtomicInt ended;
    // Set by fsapp_frame_wake, and cleared by the render thread's frame wait that returns for it.
    SDL_AtomicInt woken;
    // The lock and condition the render thread waits on, for a wanted frame and in its frame
    // wait. The lock is held only to copy a snapshot in or out, set a flag and signal; never while
    // drawing, uploading, waiting or running hooks. They are never destroyed, since other threads
    // may wake the render thread at any time.
    SDL_Mutex *mutex;
    SDL_Condition *condition;
    // Under the lock: the next snapshot, whether it has been written and not taken yet, and
    // whether a frame is wanted (one flag, cleared when the render thread starts a frame: wanted
    // frames do not queue).
    SnapshotCopy next;
    bool next_written;
    bool wanted;
    // The host window side's own storage: the snapshot written, copied before it is handed over,
    // and an untaken next snapshot taken back, to carry over what it hands over only once.
    SnapshotCopy written;
    SnapshotCopy untaken;
} render;

// True on the render thread (which sets it when it starts).
static thread_local bool t_render_thread;

// ----------------------------------------------------------------------------
// Registrations.

static void assert_can_register(void)
{
    SDL_assert(SDL_IsMainThread());
    // Registrations are made before the loop runs, so the hook lists never change while they are
    // being called.
    SDL_assert(!fsapp_mainthread.running);
}

void fsapp_mainthread_add_event_hook(fsapp_mainthread_event_hook_t hook)
{
    assert_can_register();
    hooks.event.push_back(hook);
}

void fsapp_mainthread_call_event_hooks(SDL_Event *event)
{
    FSAPP_MAINTHREAD_ASSERT_HOST_WINDOW_SIDE();
    for (fsapp_mainthread_event_hook_t hook : hooks.event) {
        hook(event);
    }
}

void fsapp_mainthread_add_frame_hook(fsapp_mainthread_frame_hook_t hook)
{
    assert_can_register();
    hooks.frame.push_back(hook);
}

void fsapp_mainthread_add_drawing_hook(fsapp_mainthread_drawing_hook_t hook)
{
    assert_can_register();
    hooks.drawing.push_back(hook);
}

void fsapp_mainthread_add_display_hook(fsapp_mainthread_display_hook_t hook)
{
    assert_can_register();
    hooks.display.push_back(hook);
}

// ----------------------------------------------------------------------------
// The pacer.

void fsapp_mainthread_set_pacer(const fsapp_pacer_t *pacer)
{
    FSAPP_MAINTHREAD_ASSERT_HOST_WINDOW_SIDE();
    if (pacer) {
        SDL_assert(pacer->wait);
        SDL_Log("Pacer set (swap interval %d)", pacer->swap_interval);
        fsapp_mainthread.pacer.set = true;
        fsapp_mainthread.pacer.pacer = *pacer;
        fsapp_mainthread_set_pacer_swap_interval(pacer->swap_interval,
                                                 pacer->follows_host_vertical_blank);
    } else {
        if (fsapp_mainthread.pacer.set) {
            SDL_Log("Pacer cleared");
        }
        fsapp_mainthread.pacer.set = false;
        SDL_zero(fsapp_mainthread.pacer.pacer);
    }
}

void fsapp_mainthread_set_pacer_swap_interval(int swap_interval, bool follows_host_vertical_blank)
{
    SDL_SetAtomicInt(&g_pacer_swap_interval, swap_interval);
    SDL_SetAtomicInt(&g_pacer_follows_host_vertical_blank, follows_host_vertical_blank ? 1 : 0);
    if (FS_IsTraceEnabled()) {
        FS_TraceValue values[] = {{"interval", swap_interval},
                                  {"follows", follows_host_vertical_blank ? 1 : 0}};
        FS_AddTraceInstantWithValues("pacer swap interval", (int64_t)SDL_GetTicksNS(),
                                     FS_TRACE_NO_FRAME, values, 2);
    }
}

// ----------------------------------------------------------------------------
// The mode.

// Render thread mode is off by default on every platform until it has been tested more
// (FSAPP_RENDER_THREAD=1 turns it on). It works on Linux and macOS, and was measured on Linux and
// Windows to be no worse than one-thread mode (and input waits much less for the main thread when
// it would block in the swap or draw slowly).
#define RENDER_THREAD_DEFAULT false

bool fsapp_mainthread_has_render_thread(void)
{
    if (render.decided) {
        return render.on;
    }
    SDL_assert(SDL_IsMainThread());
    render.decided = true;
    // X11 (Xlib) is used from both threads in render thread mode (GLX's swap on the render
    // thread, the events on the main thread), which needs XInitThreads before any other Xlib call.
    // SDL 3's X11 driver calls it itself, before it opens the display (X11_CreateDevice), so
    // render thread mode needs no check of its own under X11.
    const char *value = SDL_getenv("FSAPP_RENDER_THREAD");
    if (value && (SDL_strcmp(value, "1") == 0 || SDL_strcmp(value, "0") == 0)) {
        render.on = value[0] == '1';
        SDL_Log("Render thread: %s (FSAPP_RENDER_THREAD=%s)", render.on ? "on" : "off", value);
    } else {
        render.on = RENDER_THREAD_DEFAULT;
        SDL_Log("Render thread: %s (the default%s)", render.on ? "on" : "off",
                value ? ", FSAPP_RENDER_THREAD is neither 1 nor 0" : "");
    }
    return render.on;
}

// ----------------------------------------------------------------------------
// The two sides.

bool fsapp_mainthread_is_drawing_side(void)
{
    if (t_render_thread) {
        return true;
    }
    return SDL_IsMainThread() && fsapp_mainthread.drawing_side;
}

bool fsapp_mainthread_is_host_window_side(void)
{
    return SDL_IsMainThread() && !fsapp_mainthread.drawing_side;
}

// ----------------------------------------------------------------------------
// The events.

// True for the frame wake's event (fsapp_frame_wake), which only ends the frame wait: nothing else
// handles it. Taking it off the queue lets the next wake push one again.
static bool is_frame_wake_event(const SDL_Event *event)
{
    Uint32 type = SDL_GetAtomicU32(&g_frame_wake_event_type);
    if (type == 0 || event->type != type) {
        return false;
    }
    SDL_SetAtomicInt(&g_frame_wake_pending, 0);
    return true;
}

// The trace's name for an input event, or NULL for other events.
static const char *trace_input_event_name(const SDL_Event *event)
{
    switch (event->type) {
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        return "input: key";
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
    case SDL_EVENT_MOUSE_WHEEL:
        return "input: mouse button";
    case SDL_EVENT_MOUSE_MOTION:
        return "input: mouse motion";
    case SDL_EVENT_JOYSTICK_AXIS_MOTION:
    case SDL_EVENT_JOYSTICK_HAT_MOTION:
    case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
    case SDL_EVENT_JOYSTICK_BUTTON_UP:
        return "input: joystick";
    case SDL_EVENT_GAMEPAD_AXIS_MOTION:
    case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
    case SDL_EVENT_GAMEPAD_BUTTON_UP:
        return "input: gamepad";
    default:
        return NULL;
    }
}

// When SDL received an event (its timestamp, which is on the clock the timing code reads), and how
// long it has waited since then (waited_ns). A timestamp after now (which SDL should not give)
// counts as now.
static int64_t get_event_received_ns(const SDL_Event *event, int64_t *waited_ns)
{
    int64_t now_ns = (int64_t)SDL_GetTicksNS();
    int64_t received_ns = SDL_min((int64_t)event->common.timestamp, now_ns);
    *waited_ns = now_ns - received_ns;
    return received_ns;
}

// Traces an input event at the time SDL received it, with how long it waited until it was handled.
static void trace_input_event(const SDL_Event *event)
{
    const char *name = trace_input_event_name(event);
    if (name == NULL) {
        return;
    }
    int64_t waited_ns;
    int64_t received_ns = get_event_received_ns(event, &waited_ns);
    FS_AddTraceInstant(name, received_ns, FS_TRACE_NO_FRAME, "handled_after_us", waited_ns);
}

// The trace's name for a change of the host window or its display, and whether its data1 and data2
// are a size (width and height) or a position (x and y); NULL for other events.
static const char *trace_window_event_name(const SDL_Event *event, bool *size, bool *position)
{
    *size = false;
    *position = false;
    switch (event->type) {
    case SDL_EVENT_WINDOW_SHOWN:
        return "window: shown";
    case SDL_EVENT_WINDOW_HIDDEN:
        return "window: hidden";
    case SDL_EVENT_WINDOW_MOVED:
        *position = true;
        return "window: moved";
    case SDL_EVENT_WINDOW_RESIZED:
        *size = true;
        return "window: resized";
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
        *size = true;
        return "window: pixel size changed";
    case SDL_EVENT_WINDOW_MINIMIZED:
        return "window: minimized";
    case SDL_EVENT_WINDOW_MAXIMIZED:
        return "window: maximized";
    case SDL_EVENT_WINDOW_RESTORED:
        return "window: restored";
    case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
        return "window: entered fullscreen";
    case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
        return "window: left fullscreen";
    case SDL_EVENT_WINDOW_FOCUS_GAINED:
        return "window: focus gained";
    case SDL_EVENT_WINDOW_FOCUS_LOST:
        return "window: focus lost";
    case SDL_EVENT_WINDOW_OCCLUDED:
        return "window: occluded";
    case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
        return "window: display changed";
    case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
        return "window: display scale changed";
    case SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED:
        return "display: mode changed";
    default:
        return NULL;
    }
}

// Traces a change of the host window or its display at the time SDL received it, with its size or
// position when it has one, and how long it waited until it was handled.
static void trace_window_event(const SDL_Event *event)
{
    bool size, position;
    const char *name = trace_window_event_name(event, &size, &position);
    if (name == NULL) {
        return;
    }
    int64_t waited_ns;
    int64_t received_ns = get_event_received_ns(event, &waited_ns);
    FS_TraceValue values[3] = {{"handled_after_us", waited_ns}};
    int count = 1;
    if (size || position) {
        values[1] = {size ? "width" : "x", event->window.data1};
        values[2] = {size ? "height" : "y", event->window.data2};
        count = 3;
    }
    FS_AddTraceInstantWithValues(name, received_ns, FS_TRACE_NO_FRAME, values, count);
}

static void handle_event(SDL_Event *event)
{
    if (FS_IsTraceEnabled()) {
        trace_input_event(event);
        trace_window_event(event);
    }
    // The events are handled on the host window side, also when they are pumped in the drawing
    // side's wait: the record switches to the host window side for them and back.
    bool drawing_side = fsapp_mainthread.drawing_side;
    fsapp_mainthread.drawing_side = false;
    // Mouse positions become pixels before anything sees them (routing's hit test, the event hooks
    // and the copy for the UI).
    fsapp_main_convert_event_to_pixels(event);
    // The shortcuts first: a key event of a shortcut goes nowhere else. Then routing, which decides
    // whether the event goes to the event hooks (the emulation's input) and to the copy for the UI
    // thread, then fsapp, which also makes that copy.
    if (!fsapp_main_handle_shortcut_event(event)) {
        int destination = fsapp_input_handle_event(event);
        if (destination & FSAPP_INPUT_TO_EVENT_HOOKS) {
            fsapp_mainthread_call_event_hooks(event);
        }
        fsapp_main_handle_event(event, (destination & FSAPP_INPUT_TO_UI_COPY) != 0);
    }
    if (event->type == SDL_EVENT_WINDOW_DISPLAY_CHANGED ||
        event->type == SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED) {
        display.changed = true;
    }
    switch (event->type) {
    case SDL_EVENT_WINDOW_SHOWN:
    case SDL_EVENT_WINDOW_EXPOSED:
    case SDL_EVENT_WINDOW_RESIZED:
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
    case SDL_EVENT_WINDOW_MINIMIZED:
    case SDL_EVENT_WINDOW_MAXIMIZED:
    case SDL_EVENT_WINDOW_RESTORED:
    case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
    case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
    case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
    case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
        // The window changed, so a frame is needed (also without a pacer).
        on_demand.window_changed = true;
        break;
    default:
        break;
    }
    fsapp_mainthread.drawing_side = drawing_side;
}

static void handle_pending_events(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // A wake outside the frame wait has nothing to end (whoever waits checks first).
        if (!is_frame_wake_event(&event)) {
            handle_event(&event);
        }
    }
}

// Returns true if the main loop should end: quit was requested (also by SIGTERM or SIGINT, which
// SDL turns into a quit event, so events must be handled for it to be seen), or the Python thread
// has ended because of an error. Without the UI thread, the program cannot be used (and it never
// becomes ready for the first render), so this asks to quit then, and main shows the error.
static bool check_quit(void)
{
    if (!fsapp_mainthread.quit_requested() && fsapp_pythonthread_has_failed()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "The UI thread has failed, quitting");
        fsapp_main_quit();
    }
    return fsapp_mainthread.quit_requested();
}

// Gives the navigation repeats that are due (fsapp-input.h) on the host window side, also while the
// drawing side waits in one-thread mode, as handle_event does.
static void handle_navigation_repeats(void)
{
    bool drawing_side = fsapp_mainthread.drawing_side;
    fsapp_mainthread.drawing_side = false;
    fsapp_input_handle_repeats((int64_t)SDL_GetTicksNS());
    fsapp_mainthread.drawing_side = drawing_side;
}

// Handles the pending events and the navigation repeats due, then returns true if the main loop
// should end (check_quit).
static bool handle_events_and_check_quit(void)
{
    handle_pending_events();
    handle_navigation_repeats();
    return check_quit();
}

// ----------------------------------------------------------------------------
// The frame wait and wake.

// The frame wait (fsapp_frame_wait), also used by the loop's own waits on the host window side,
// which only look again after a while (before the host window exists, and shutting down): for
// those, precise is false, and the end is not busy-waited (the wait may then end up to 1 ms late).
// With with_quit false, quit is not checked (after the main loop has ended, it is always
// requested).
static fsapp_frame_wait_result_t frame_wait(int64_t until_ns, bool precise, bool with_quit)
{
    while (true) {
        int64_t remaining_ns = until_ns - (int64_t)SDL_GetTicksNS();
        SDL_Event event;
        bool have_event;
        if (precise ? remaining_ns >= (int64_t)SDL_MS_TO_NS(1) : remaining_ns > 0) {
            // Each SDL wait is capped at 1 ms, which bounds the wake a push can miss just as SDL
            // starts its driver wait. SDL's wait takes whole milliseconds, so (when precise) it is
            // only used while at least one is left.
            have_event = SDL_WaitEventTimeout(&event, 1);
        } else {
            // The last fraction of a millisecond (or the events pending when the time has come):
            // busy-wait, pumping the events.
            have_event = SDL_PollEvent(&event);
        }
        if (have_event) {
            if (is_frame_wake_event(&event)) {
                return FSAPP_FRAME_WAIT_WOKEN;
            }
            handle_event(&event);
        }
        // The repeats are given when due, also while a frame waits.
        handle_navigation_repeats();
        if (with_quit && check_quit()) {
            return FSAPP_FRAME_WAIT_QUIT;
        }
        // The events pending when the time has come are handled before returning.
        if (!have_event && remaining_ns <= 0) {
            return FSAPP_FRAME_WAIT_TIME_REACHED;
        }
    }
}

// The frame wait on the render thread (render thread mode): blocks on the render thread's
// condition, without pumping events (the main thread does), until woken, until the time, or until
// the render thread is asked to stop (quit). Like the frame wait on the main thread, it waits
// coarsely while enough time is left and busy-waits the last fraction of a millisecond or so: the
// condition's timeout is in whole milliseconds, so it waits until about half a millisecond before
// the time.
static fsapp_frame_wait_result_t render_frame_wait(int64_t until_ns)
{
    while (true) {
        if (SDL_GetAtomicInt(&render.stop)) {
            return FSAPP_FRAME_WAIT_QUIT;
        }
        if (SDL_CompareAndSwapAtomicInt(&render.woken, 1, 0)) {
            return FSAPP_FRAME_WAIT_WOKEN;
        }
        int64_t remaining_ns = until_ns - (int64_t)SDL_GetTicksNS();
        if (remaining_ns <= 0) {
            return FSAPP_FRAME_WAIT_TIME_REACHED;
        }
        if (remaining_ns >= (int64_t)SDL_US_TO_NS(1500)) {
            // A waker sets its flag before it takes the lock to signal, and the flags are looked
            // at again under the lock, so no wake is lost.
            SDL_LockMutex(render.mutex);
            if (!SDL_GetAtomicInt(&render.stop) && !SDL_GetAtomicInt(&render.woken)) {
                // Rounded down: the rest is busy-waited.
                SDL_WaitConditionTimeout(
                    render.condition, render.mutex,
                    (Sint32)SDL_NS_TO_MS(remaining_ns - (int64_t)SDL_US_TO_NS(500)));
            }
            SDL_UnlockMutex(render.mutex);
        } else {
            SDL_CPUPauseInstruction();
        }
    }
}

fsapp_frame_wait_result_t fsapp_frame_wait(int64_t until_ns)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    if (t_render_thread) {
        return render_frame_wait(until_ns);
    }
    return frame_wait(until_ns, true, true);
}

void fsapp_frame_wake(void)
{
    // Both sides are woken: the render thread's wait (in render thread mode), and the main thread
    // with an event, below. Waking a side that does not wait costs a signal, or an event.
    if (SDL_GetAtomicInt(&render.running)) {
        SDL_SetAtomicInt(&render.woken, 1);
        SDL_LockMutex(render.mutex);
        SDL_SignalCondition(render.condition);
        SDL_UnlockMutex(render.mutex);
    }
    Uint32 type = SDL_GetAtomicU32(&g_frame_wake_event_type);
    if (type == 0) {
        // The main loop has not started, so nothing waits yet.
        return;
    }
    if (!SDL_CompareAndSwapAtomicInt(&g_frame_wake_pending, 0, 1)) {
        // A wake event is already queued.
        return;
    }
    SDL_Event event;
    SDL_zero(event);
    event.type = type;
    if (!SDL_PushEvent(&event)) {
        SDL_SetAtomicInt(&g_frame_wake_pending, 0);
    }
}

// ----------------------------------------------------------------------------
// Keep drawing and the request for one frame (any thread).

void fsapp_frame_take_keep_drawing(void)
{
    SDL_AddAtomicInt(&g_keep_drawing, 1);
    fsapp_frame_wake();
}

void fsapp_frame_release_keep_drawing(void)
{
    int previous = SDL_AddAtomicInt(&g_keep_drawing, -1);
    SDL_assert(previous > 0);
    (void)previous;
    fsapp_frame_wake();
}

void fsapp_frame_request(void)
{
    SDL_SetAtomicInt(&g_frame_requested, 1);
    fsapp_frame_wake();
}

// Registers the frame wake's event type, when the main loop starts. Not earlier: the program
// registers its own event types first, and FS-UAE's Python side relies on the first of them being
// SDL_EVENT_USER.
static void frame_wake_init(void)
{
    if (SDL_GetAtomicU32(&g_frame_wake_event_type) != 0) {
        return;
    }
    Uint32 type = SDL_RegisterEvents(1);
    if (type == 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not register the frame wake event: %s",
                     SDL_GetError());
        return;
    }
    SDL_SetAtomicU32(&g_frame_wake_event_type, type);
}

// ----------------------------------------------------------------------------
// The host display (host window side).

// Tells the display hooks the host refresh rate and whether every swap waits for a host vertical
// blank: when the host window exists, and again when an event says the display may have changed
// and either did.
static void frame_update_display(void)
{
    if (display.known && !display.changed) {
        return;
    }
    display.changed = false;
    double refresh_rate_hz = 0.0;
    const SDL_DisplayMode *mode = SDL_GetCurrentDisplayMode(SDL_GetDisplayForWindow(g_window));
    if (fsapp_testaids_is_display_simulated()) {
        refresh_rate_hz = fsapp_testaids_get_simulated_refresh_rate_hz();
    } else if (mode) {
        refresh_rate_hz = mode->refresh_rate;
    }
    bool refresh_rate_known = refresh_rate_hz > 0.0;
    if (!refresh_rate_known) {
        // SDL cannot tell (for example with the offscreen driver).
        refresh_rate_hz = 60.0;
    }
    // With SDL's KMSDRM driver, every swap waits for a page flip, which happens at the host
    // vertical blank.
    // REVIEW: Whether the Raspberry Pi takes SDL's atomic KMSDRM path (where this may not hold) is
    // still to be checked on a Pi.
    const char *driver = SDL_GetCurrentVideoDriver();
    bool every_swap_waits = driver && SDL_strcmp(driver, "kmsdrm") == 0;
    bool observed = false;
    for (int i = 0; driver && g_host_vertical_blank_observed_drivers[i]; i++) {
        if (SDL_strcmp(driver, g_host_vertical_blank_observed_drivers[i]) == 0) {
            observed = true;
        }
    }
    display.host_vertical_blank_observed = observed;
    if (display.known && refresh_rate_hz == display.refresh_rate_hz &&
        refresh_rate_known == display.refresh_rate_known &&
        every_swap_waits == display.every_swap_waits) {
        return;
    }
    display.known = true;
    display.refresh_rate_hz = refresh_rate_hz;
    display.refresh_rate_known = refresh_rate_known;
    display.every_swap_waits = every_swap_waits;
    SDL_Log("Host display: refresh rate %.3f Hz%s, every swap waits for vertical blank: %s",
            refresh_rate_hz, refresh_rate_known ? "" : " (not known, assumed)",
            every_swap_waits ? "yes" : "no");
    for (fsapp_mainthread_display_hook_t hook : hooks.display) {
        hook(refresh_rate_hz, every_swap_waits);
    }
}

// ----------------------------------------------------------------------------
// Drawing on demand without a pacer (host window side).

// Whether the drawing side is to draw a frame now: always while a pacer is set (it paces the loop
// in its wait hook), and without one only when a frame is needed. But no frame is drawn before the
// UI's first record has been taken in, pacer or keep drawing notwithstanding, so the first frame
// swapped already has the whole UI in it; the host window side keeps running until then. (A failed
// UI thread ends the program, so there is no timeout.)
static bool frame_is_needed(void)
{
    if (!fsapp_surface_has_taken_first_record()) {
        // The record handing over wakes the loop.
        return fsapp_surface_has_pending_record();
    }
    if (fsapp_mainthread.pacer.set || fsapp_mainthread.pacer.set != on_demand.pacer_was_set) {
        // A pacer is set, or it was just cleared.
        return true;
    }
    if (SDL_GetAtomicInt(&g_keep_drawing) > 0 || SDL_GetAtomicInt(&g_frame_requested) ||
        on_demand.window_changed) {
        return true;
    }
    // An adornment changed (each change also wakes the loop).
    if (fsapp_adornment_has_changed()) {
        return true;
    }
    // The UI has handed over a record (it wakes the loop when it does).
    return fsapp_surface_has_pending_record();
}

// How long the idle wait blocks at most (frame_wait_idle).
// REVIEW: Not for ever, since SDL can lose a wake. SDL_PushEvent wakes a driver wait in progress
// (SDL_SendWakeupEvent), but SDL 3.4's SDL_WaitEventTimeout_Device looks at the queue for the last
// time before it sets the window to wake, so an event pushed in between wakes nothing, and the
// driver wait blocks with it queued (the frame wait caps its waits at 1 ms for the same reason).
// So a lost wake (rare: the push must land in that gap) leaves the loop late by up to this, and so
// does state that another thread changes without fsapp_frame_wake. Ten looks a second cost next to
// nothing, also on Windows, where each wait of the frame wait's 1 ms was a real wake (with
// timeBeginPeriod(1)).
#define IDLE_WAIT_NS SDL_MS_TO_NS(100)

// No frame is needed: wait until woken, until an event comes (it is handled) or until quit (an
// event, or a wake), so the host window side runs again then, but no longer than timeout_ns. It
// blocks in SDL's wait for events (the driver's, where it has one), with no 1 ms cap as in the
// frame wait, so an idle host window costs next to nothing: what makes a frame needed, or the loop
// end, from another thread wakes the loop (fsapp_frame_wake), and the rest comes as events.
static void frame_wait_idle(int64_t timeout_ns)
{
    // A navigation repeat ends the wait when it is due (fsapp-input.h), so that a held direction
    // repeats without a pacer and with the host window idle.
    int64_t repeat_ns;
    if (fsapp_input_get_repeat_deadline_ns(&repeat_ns)) {
        timeout_ns = SDL_min(timeout_ns, repeat_ns - (int64_t)SDL_GetTicksNS());
    }
    SDL_Event event;
    // SDL waits whole milliseconds: rounded down (the host window side looks again a little early
    // rather than late), and at least 1.
    Sint32 timeout_ms =
        (Sint32)SDL_clamp(SDL_NS_TO_MS(timeout_ns), (int64_t)1, (int64_t)SDL_MAX_SINT32);
    if (!SDL_WaitEventTimeout(&event, timeout_ms)) {
        // Nothing came in time (or a wake was lost, see IDLE_WAIT_NS): the host window side looks
        // again.
        return;
    }
    // The wake's event has done its work by ending the wait.
    if (!is_frame_wake_event(&event)) {
        handle_event(&event);
    }
}

// How long the host window side's idle wait blocks at most with a pacer in render thread mode:
// one host refresh period (60 Hz when not known), at least 1 ms.
static int64_t paced_idle_wait_ns(void)
{
    double refresh_rate_hz = display.refresh_rate_hz > 0.0 ? display.refresh_rate_hz : 60.0;
    return SDL_max((int64_t)SDL_MS_TO_NS(1),
                   (int64_t)((double)SDL_NS_PER_SECOND / refresh_rate_hz));
}

// Whether a needed frame is due: always while a pacer is set (it paces the loop in its wait hook)
// or when it was just cleared (the next frame is then due at once), and without one from when it
// is due.
static bool frame_is_due(void)
{
    if (fsapp_mainthread.pacer.set || on_demand.pacer_was_set) {
        return true;
    }
    return (int64_t)SDL_GetTicksNS() >= on_demand.next_frame_due_ns;
}

// A frame is needed but not due yet (without a pacer): wait until it is due, pumping the events,
// or until woken or quit. This is the pacing without a pacer, after the previous frame's swap: the
// host window side then runs again from the events, so the frame's phases (and its record) start
// after the wait.
static void frame_wait_until_due(void)
{
    frame_wait(on_demand.next_frame_due_ns, true, true);
}

// The frame is about to be drawn: what made it needed is taken, and when the next one is due is
// set. Without a pacer, frames are drawn at up to the host refresh rate (60 Hz when not known).
static void frame_take_need(void)
{
    SDL_SetAtomicInt(&g_frame_requested, 0);
    on_demand.window_changed = false;
    on_demand.pacer_was_set = fsapp_mainthread.pacer.set;
    if (fsapp_mainthread.pacer.set) {
        // Should the pacer be cleared, the next frame is due at once.
        on_demand.next_frame_due_ns = 0;
        return;
    }
    // REVIEW: Counted from the last swap (one host refresh period after it), every frame would
    // start late by its own drawing time, so the frames would drift below the refresh rate (and
    // with swap interval 1, miss vertical blanks now and then). So it is counted from when this
    // frame was due, as the fixed 60 Hz pacing did before, while it starts less than a period
    // late; otherwise (after idling, or with no due time, 0) from now.
    double refresh_rate_hz = display.refresh_rate_hz > 0.0 ? display.refresh_rate_hz : 60.0;
    int64_t period_ns = (int64_t)((double)SDL_NS_PER_SECOND / refresh_rate_hz);
    int64_t now_ns = (int64_t)SDL_GetTicksNS();
    int64_t due_ns = on_demand.next_frame_due_ns;
    bool from_due = due_ns != 0 && now_ns - due_ns < period_ns;
    on_demand.next_frame_due_ns = (from_due ? due_ns : now_ns) + period_ns;
}

// ----------------------------------------------------------------------------
// The snapshot (host window side).

// Writes the snapshot (in fsapp_mainthread.snapshot, its arrays in the modules' storage). untaken
// is the next snapshot when the drawing side has not taken it yet (render thread mode), whose parts
// that are handed over only once are carried over into this one by the modules that own them;
// else NULL.
static void frame_write_snapshot(const fsapp_snapshot_t *untaken)
{
    fsapp_snapshot_window_t *window = &fsapp_mainthread.snapshot.window;
    window->sdl_window = g_window;
    // The size in window coordinates is kept up to date from the window events (fsapp-main.cpp);
    // the size in pixels is read now.
    window->width = g_window_width;
    window->height = g_window_height;
    if (!SDL_GetWindowSizeInPixels(g_window, &window->pixel_width, &window->pixel_height)) {
        window->pixel_width = window->width;
        window->pixel_height = window->height;
    }
    window->scale = SDL_GetWindowPixelDensity(g_window);
    if (window->scale <= 0.0f) {
        window->scale = 1.0f;
    }
    window->background_color = fsapp_main_get_background_color();
    window->refresh_rate_hz = display.refresh_rate_hz;
    window->every_swap_waits = display.every_swap_waits;
    window->refresh_rate_known = display.refresh_rate_known;
    window->host_vertical_blank_observed = display.host_vertical_blank_observed;
    // The surface layout: the pending record (if any) taken and applied, the flat list, and the
    // record's content and destroyed surfaces (fsapp-surface.h).
    fsapp_surface_write_snapshot(&fsapp_mainthread.snapshot.layout,
                                 untaken ? &untaken->layout : NULL);
    fsapp_adornment_write_snapshot(&fsapp_mainthread.snapshot.adornments,
                                   untaken ? &untaken->adornments : NULL);
    fsapp_mainthread.snapshot.pacer = fsapp_mainthread.pacer;
}

// The drawing side takes the snapshot (in one-thread mode), with copies of the surface layout and
// the adornments, so that it reads none of the host window side's storage during its frame.
static void take_snapshot(void)
{
    drawing.taken.assign(fsapp_mainthread.snapshot);
}

// The host window side writes the next snapshot in render thread mode and wants a frame (which the
// render thread draws now without a pacer; with one, it draws frame after frame anyway and takes
// the snapshot at its next frame's start). An untaken next snapshot is first taken back, so that
// the render thread cannot take it meanwhile (it then draws from the one it has), and what it
// hands over only once is carried over into the new one. The lock is held only to swap the copies.
// Taking it back whole (rather than merging into the slot under the lock) keeps the lock short and
// leaves each carry-over rule to the one module that owns it: the surface module carries over its
// record and takes no new one meanwhile, the adornment module its destroyed adornments, new content
// and change, as frame_write_snapshot hands each its part of the untaken snapshot.
static void render_write_next_snapshot(void)
{
    SDL_LockMutex(render.mutex);
    bool untaken = render.next_written;
    if (untaken) {
        render.untaken.swap(render.next);
        render.next_written = false;
    }
    SDL_UnlockMutex(render.mutex);
    frame_write_snapshot(untaken ? &render.untaken.snapshot : NULL);
    render.written.assign(fsapp_mainthread.snapshot);
    SDL_LockMutex(render.mutex);
    render.next.swap(render.written);
    render.next_written = true;
    render.wanted = true;
    SDL_SignalCondition(render.condition);
    SDL_UnlockMutex(render.mutex);
}

// ----------------------------------------------------------------------------
// Main thread calls from the drawing side.

static void SDLCALL stop_with_error_on_main_thread(void *userdata)
{
    char *message = (char *)userdata;
    fsapp_main_stop_with_error(message);
    SDL_free(message);
}

// Stops the program with an error (fsapp_main_stop_with_error, main thread only), from the drawing
// side: on the render thread, the message is handed to the main thread (SDL_RunOnMainThread,
// without waiting), which stops when it next pumps the events; the render thread draws nothing
// more meanwhile, since its state was not made.
static void drawing_stop_with_error(const char *message)
{
    if (!t_render_thread) {
        fsapp_main_stop_with_error(message);
        return;
    }
    char *copy = SDL_strdup(message);
    if (!SDL_RunOnMainThread(stop_with_error_on_main_thread, copy, false)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s (could not hand it to the main thread: %s)",
                     message, SDL_GetError());
        SDL_free(copy);
    }
}

static void SDLCALL quit_on_main_thread(void *userdata)
{
    (void)userdata;
    fsapp_main_quit();
}

// Asks to quit (fsapp_main_quit) from the drawing side: on the render thread, through the main
// thread (SDL_RunOnMainThread, without waiting), where the quit function is called.
static void drawing_quit(void)
{
    if (!t_render_thread) {
        fsapp_main_quit();
        return;
    }
    if (!SDL_RunOnMainThread(quit_on_main_thread, NULL, false)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not ask the main thread to quit: %s",
                     SDL_GetError());
    }
}

// ----------------------------------------------------------------------------
// The drawing side's first frame.

// Makes the drawing side's state when its first frame starts: everything fsapp does with GL is done
// on the drawing side, none of it on the host window side, which only creates the window and the GL
// context. It logs the OpenGL information, checks the OpenGL version, loads fsapp's GL functions
// (fsapp-gl.h) and makes the compositor's programs and vertex buffer (fsapp-gldraw.h); when the
// version is below 2.1, a function is missing or a program cannot be made, it stops the program
// with an error and returns false, and the frame is not drawn.
// (The compositor's textures are made as the surfaces are first uploaded, and the first swap
// interval is applied before the first swap, after the drawing hooks, so the emulation's video
// setup in its drawing hook has decided it.)
static bool drawing_init(void)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    fsapp_gl_log_information();
    char *version_error = fsapp_gl_check_version();
    if (version_error != NULL) {
        drawing_stop_with_error(version_error);
        SDL_free(version_error);
        return false;
    }
    const char *missing = fsapp_gl_load_functions();
    if (missing != NULL) {
        char *message = NULL;
        if (SDL_asprintf(&message, "OpenGL 2.1 is needed, but the OpenGL function %s is missing",
                         missing) < 0) {
            message = NULL;
        }
        drawing_stop_with_error(message ? message : "OpenGL 2.1 is needed");
        SDL_free(message);
        return false;
    }
    const char *error = fsapp_gldraw_init();
    if (error != NULL) {
        drawing_stop_with_error(error);
        return false;
    }
    drawing.swap_number = 0;
    SDL_SetAtomicInt(&g_swap_count, 0);
    drawing.ui_frame_swap_number = 0;
    drawing.ui_frame_ns = 0;
    drawing.gl_grab_next = 0;
    return true;
}

const fsapp_snapshot_t *fsapp_mainthread_get_drawing_snapshot(void)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    return &drawing.taken.snapshot;
}

// ----------------------------------------------------------------------------
// The drawing hooks (drawing side).

static void frame_drawing_hooks(void)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    for (fsapp_mainthread_drawing_hook_t hook : hooks.drawing) {
        hook(&drawing.taken.snapshot.window);
    }
}

// ----------------------------------------------------------------------------
// The UI upload (drawing side).

// Clears the window; when the snapshot carries a record, deletes the textures of its destroyed
// surfaces and uploads its content; uploads the adornments' content committed since the previous
// frame; and then marks the record taken in, which lets the UI thread start its next UI frame.
// Without a record, nothing of the UI's is uploaded and the old textures are drawn again: the
// drawing side never waits for the UI thread.
static void frame_ui_upload(void)
{
    fsapp_compositor_clear_window(&drawing.taken.snapshot);
    fsapp_compositor_update_textures(&drawing.taken.snapshot);
    fsapp_compositor_update_adornment_textures(&drawing.taken.snapshot);
    fsapp_adornment_on_uploaded(&drawing.taken.snapshot.adornments);
    fsapp_surface_on_uploaded(&drawing.taken.snapshot.layout);
}

// ----------------------------------------------------------------------------
// The wait (drawing side): the pacer's wait hook, or nothing without a pacer.

static fsapp_pacer_wait_result_t frame_wait_for_pacer(void)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    if (!drawing.taken.snapshot.pacer.set) {
        // Without a pacer, the paced surface (if any) is drawn with its old content, and the loop
        // is paced after the swap.
        return FSAPP_PACER_NO_NEW_CONTENT;
    }
    return drawing.taken.snapshot.pacer.pacer.wait();
}

// ----------------------------------------------------------------------------
// The swap.

static void set_swap_interval(int swap_interval)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    // The cached value starts unset, so the first wanted swap interval is applied even when it is 0
    // (SDL does not say what the driver's default is).
    static bool set = false;
    static int current = 0;
    if (set && swap_interval == current) {
        return;
    }
    SDL_Log("SDL_GL_SetSwapInterval(%d)", swap_interval);
    if (!SDL_GL_SetSwapInterval(swap_interval)) {
        SDL_Log("SDL_GL_SetSwapInterval(%d) failed: %s", swap_interval, SDL_GetError());
    }
    set = true;
    current = swap_interval;
}

void fsapp_mainthread_set_gl_grab(const char *swaps, const char *dir)
{
    // The GL grab's state is the drawing side's: it is set here before the main loop runs (as the
    // hooks are registered), and nothing on the host window side reads or changes it afterwards.
    assert_can_register();
    drawing.gl_grab_count = 0;
    while (swaps && *swaps && drawing.gl_grab_count < 32) {
        char *end = NULL;
        long value = strtol(swaps, &end, 10);
        if (end == swaps) {
            break;
        }
        drawing.gl_grab_swaps[drawing.gl_grab_count++] = (int)value;
        swaps = (*end == ',') ? end + 1 : end;
    }
    SDL_free(drawing.gl_grab_dir);
    drawing.gl_grab_dir = SDL_strdup((dir && dir[0]) ? dir : ".");
    // The GL grab counts swaps, so it keeps the loop drawing while a grab is still to come, and
    // headless runs and tests get their grabs at the same swaps as when the loop always drew.
    bool keep_drawing = drawing.gl_grab_count > 0;
    if (keep_drawing != drawing.gl_grab_keeps_drawing) {
        drawing.gl_grab_keeps_drawing = keep_drawing;
        if (keep_drawing) {
            fsapp_frame_take_keep_drawing();
        } else {
            fsapp_frame_release_keep_drawing();
        }
    }
}

int fsapp_mainthread_get_swap_count(void)
{
    return SDL_GetAtomicInt(&g_swap_count);
}

void fsapp_mainthread_set_quit_after_gl_grab(bool quit, int64_t idle_ns)
{
    assert_can_register();
    drawing.gl_grab_quits = quit;
    drawing.gl_grab_quit_idle_ns = SDL_max(idle_ns, (int64_t)0);
}

// Asks to quit on the main thread, where the quit function is called (fsapp_main_quit).
static void gl_grab_quit_on_main_thread(void *userdata)
{
    (void)userdata;
    SDL_Log("GL grab: idled after the last grab, quitting");
    fsapp_main_quit();
}

// The GL grab's quit timer (SDL's timer thread): the main loop runs the quit when it pumps the
// events, also while it idles.
static Uint64 SDLCALL gl_grab_quit_timer_callback(void *userdata, SDL_TimerID timer_id,
                                                  Uint64 interval_ns)
{
    (void)userdata;
    (void)timer_id;
    (void)interval_ns;
    SDL_RunOnMainThread(gl_grab_quit_on_main_thread, NULL, false);
    return 0;
}

static void gl_grab_write(int64_t swap_number, int w, int h);

// The GL grab, a developer and test aid for headless runs (see fsapp_mainthread_set_gl_grab):
// saves the rendered back buffer (the emulation surface and the UI, as composited) as a PNG just
// before the given swap numbers.
static void gl_grab_maybe(int64_t swap_number)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    if (drawing.gl_grab_next >= drawing.gl_grab_count ||
        swap_number < drawing.gl_grab_swaps[drawing.gl_grab_next]) {
        return;
    }
    drawing.gl_grab_next++;
    if (drawing.gl_grab_next >= drawing.gl_grab_count && drawing.gl_grab_keeps_drawing) {
        // The last grab: no grab is still to come.
        drawing.gl_grab_keeps_drawing = false;
        fsapp_frame_release_keep_drawing();
    }
    int w = drawing.taken.snapshot.window.pixel_width;
    int h = drawing.taken.snapshot.window.pixel_height;
    if (w <= 0 || h <= 0) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "GL grab: invalid window size %dx%d", w, h);
    } else {
        gl_grab_write(swap_number, w, h);
    }
    if (drawing.gl_grab_next >= drawing.gl_grab_count && drawing.gl_grab_quits) {
        if (drawing.gl_grab_quit_idle_ns > 0) {
            // The main loop goes on (and idles, if nothing changes) until the timer asks to quit.
            SDL_Log("GL grab: the last grab is done, quitting in %lld ms",
                    (long long)SDL_NS_TO_MS(drawing.gl_grab_quit_idle_ns));
            drawing.gl_grab_quit_timer = SDL_AddTimerNS((Uint64)drawing.gl_grab_quit_idle_ns,
                                                        gl_grab_quit_timer_callback, NULL);
        } else {
            // The last grab is done: the frame still swaps, and the main loop ends at its next
            // quit check.
            SDL_Log("GL grab: the last grab is done, quitting");
            drawing_quit();
        }
    }
}

// Writes the GL grab of the back buffer (w x h pixels) for the swap, with its alpha as drawn (the
// window is opaque, so alpha below 255 in a grab is a fault worth seeing).
static void gl_grab_write(int64_t swap_number, int w, int h)
{
    uint8_t *pixels = (uint8_t *)malloc((size_t)w * h * 4);
    uint8_t *flipped = (uint8_t *)malloc((size_t)w * h * 4);
    // The pack state in full, whatever was set before.
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ROW_LENGTH, 0);
    glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_PACK_SKIP_ROWS, 0);
    glReadBuffer(GL_BACK);
    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    for (int y = 0; y < h; y++) {
        memcpy(flipped + (size_t)y * w * 4, pixels + (size_t)(h - 1 - y) * w * 4, (size_t)w * 4);
    }
    char path[1024];
    snprintf(path, sizeof(path), "%s/GLGrab_%05d.png", drawing.gl_grab_dir, (int)swap_number);
    int err = fsapp_bitmap_save_png_file_from_rgba_data(path, flipped, w, h);
    SDL_Log("GL grab: %dx%d -> %s (result %d, glGetError %d)", w, h, path, err, (int)glGetError());
    free(pixels);
    free(flipped);
}

// The swap interval belongs to whoever paces: the pacer's own while one is set, else 1.
static int swap_interval_to_apply(void)
{
    if (drawing.taken.snapshot.pacer.set) {
        return SDL_GetAtomicInt(&g_pacer_swap_interval);
    }
    return 1;
}

// Rendering finished: waits until the frame's rendering is done, and records when. With the
// simulated drawing delay (FSAPP_SIMULATED_RENDER_US), rendering finished comes that much later.
static void frame_rendering_finished(fsapp_frame_record_t *record)
{
    // This glFinish() stays, for two reasons: its return is the frame's rendering finished in the
    // record, and the slot margin (how long before the swap deadline rendering must start) will
    // adapt from it. Without it the time rendering takes would be hidden in the swap. (No fence
    // instead until there is a render thread.)
    glFinish();
    int64_t now_ns = (int64_t)SDL_GetTicksNS();
    // The swap about to be made gets the next swap number (frame_swap).
    int64_t delay_ns = fsapp_testaids_get_simulated_render_delay_ns(drawing.swap_number + 1);
    if (delay_ns > 0) {
        now_ns = fsapp_time_wait_until_ns(now_ns + delay_ns);
    }
    record->rendering_finished_ns = now_ns;
}

// Applies the swap interval, then swaps, and fills in the swap in the frame's record. With the
// simulated display, the swap is then held until its next tick, unless the swap interval is 0 (and
// on some ticks a little longer, with FSAPP_SIMULATED_DISPLAY_LATE_US, as if it woke up late).
static void frame_swap(fsapp_frame_record_t *record)
{
    int swap_interval = swap_interval_to_apply();
    set_swap_interval(swap_interval);
    record->swap_number = ++drawing.swap_number;
    gl_grab_maybe(record->swap_number);
    // Feedback on when this swap is shown, where the windowing system has it (Wayland, X11; on
    // macOS only the display's refreshes are reported).
    fsapp_presentation_request(drawing.taken.snapshot.window.sdl_window, record->swap_number);
    SDL_GL_SwapWindow(drawing.taken.snapshot.window.sdl_window);
    drawing.simulated_vertical_blank_ns = 0;
    drawing.simulated_tick_ns = 0;
    if (fsapp_testaids_is_display_simulated() && swap_interval != 0) {
        // With a lead, the swap returns that long before its tick (the next one it can return
        // before), as at a compositor's frame callback.
        int64_t lead_ns = fsapp_testaids_get_simulated_lead_ns();
        int64_t tick_ns =
            fsapp_testaids_get_next_simulated_tick_ns((int64_t)SDL_GetTicksNS() + lead_ns);
        // A late swap returns after the tick, and its host vertical blank is taken then, as when
        // it is observed from the swap's return.
        int64_t returns_ns = tick_ns - lead_ns + fsapp_testaids_get_simulated_late_swap_ns(tick_ns);
        fsapp_time_wait_until_ns(returns_ns);
        drawing.simulated_vertical_blank_ns = returns_ns;
        drawing.simulated_tick_ns = tick_ns;
    }
    record->swap_ns = (int64_t)SDL_GetTicksNS();
    SDL_SetAtomicInt(&g_swap_count, (int)drawing.swap_number);
}

// ----------------------------------------------------------------------------
// After the swap.

static const char *const g_frame_phase_names[FSAPP_FRAME_PHASE_COUNT] = {
    "events",        "frame_work",    "ui_upload",          "drawing_below", "wait",
    "drawing_paced", "drawing_above", "rendering_finished", "swap",          "after_swap",
};

const char *fsapp_frame_get_phase_name(fsapp_frame_phase_t phase)
{
    if (phase < 0 || phase >= FSAPP_FRAME_PHASE_COUNT) {
        return "unknown";
    }
    return g_frame_phase_names[phase];
}

static const char *host_vertical_blank_name(fsapp_frame_host_vertical_blank_t host_vertical_blank)
{
    switch (host_vertical_blank) {
    case FSAPP_FRAME_HOST_VERTICAL_BLANK_OBSERVED:
        return "observed";
    case FSAPP_FRAME_HOST_VERTICAL_BLANK_ESTIMATED:
        return "estimated";
    default:
        return "unknown";
    }
}

// The host vertical blank in the frame's record. When the simulated display held the swap, it is
// the tick the swap was held until (estimated). While the pacer follows the host vertical blank,
// a glFinish() after the swap returns when the swap is done, which is taken as the host vertical
// blank: observed on the platforms listed (g_host_vertical_blank_observed_drivers), estimated
// elsewhere. Otherwise (no glFinish(), so it costs nothing) it is estimated as the swap plus one
// host refresh period, or unknown when the host refresh rate is not known.
static void frame_host_vertical_blank(fsapp_frame_record_t *record)
{
    const fsapp_snapshot_window_t *window = &drawing.taken.snapshot.window;
    if (drawing.simulated_vertical_blank_ns != 0) {
        record->host_vertical_blank_ns = drawing.simulated_vertical_blank_ns;
        record->host_vertical_blank = FSAPP_FRAME_HOST_VERTICAL_BLANK_ESTIMATED;
    } else if (drawing.taken.snapshot.pacer.set &&
               SDL_GetAtomicInt(&g_pacer_follows_host_vertical_blank)) {
        glFinish();
        record->host_vertical_blank_ns = (int64_t)SDL_GetTicksNS();
        record->host_vertical_blank = window->host_vertical_blank_observed
                                          ? FSAPP_FRAME_HOST_VERTICAL_BLANK_OBSERVED
                                          : FSAPP_FRAME_HOST_VERTICAL_BLANK_ESTIMATED;
    } else if (window->refresh_rate_known && window->refresh_rate_hz > 0.0) {
        record->host_vertical_blank_ns =
            record->swap_ns +
            (int64_t)SDL_round((double)SDL_NS_PER_SECOND / window->refresh_rate_hz);
        record->host_vertical_blank = FSAPP_FRAME_HOST_VERTICAL_BLANK_ESTIMATED;
    } else {
        record->host_vertical_blank_ns = 0;
        record->host_vertical_blank = FSAPP_FRAME_HOST_VERTICAL_BLANK_UNKNOWN;
    }
}

// The presentations the windowing system has reported since the frame before, the swaps still
// waiting for theirs (this one included) and the display's refreshes, in the frame's record
// (fsapp-presentation.h); with the simulated display's lead (FSAPP_SIMULATED_DISPLAY_LEAD_US), the
// swap held before this one, presented at its tick, instead of the presentations. The pacer feeds
// the presentations to its predicted host vertical blank; the display's refreshes are only
// recorded, and the host vertical blank in the record is found as before (the swap return).
static void frame_presented(fsapp_frame_record_t *record)
{
    record->display_refresh_count = fsapp_presentation_take_display_refreshes(
        record->display_refreshes, FSAPP_FRAME_MAX_DISPLAY_REFRESHES);
    if (fsapp_testaids_get_simulated_lead_ns() != 0) {
        record->presented_count =
            fsapp_testaids_take_simulated_presentation(
                record->swap_number, drawing.simulated_tick_ns, &record->presented[0])
                ? 1
                : 0;
        record->presentation_waiting = 1;
        return;
    }
    record->presented_count = fsapp_presentation_take(record->presented, FSAPP_FRAME_MAX_PRESENTED);
    record->presentation_waiting = fsapp_presentation_get_waiting();
}

// The UI frame lag in the frame's record.
static void frame_ui_frame_lag(fsapp_frame_record_t *record)
{
    if (drawing.ui_frame_swap_number > 0) {
        record->ui_frame_lag_swaps = record->swap_number - drawing.ui_frame_swap_number;
        record->ui_frame_lag_ns = record->swap_ns - drawing.ui_frame_ns;
    }
}

// Logs every Nth frame record (FSAPP_LOG_FRAME_RECORDS=N), in microseconds: the phases by name
// with their start times relative to the first (the events, logged as is), then rendering finished,
// when the swap returned, the host vertical blank (and how it is known) and the UI frame lag.
static void log_frame_record(const fsapp_frame_record_t *record)
{
    int every = fsapp_mainthread.log_frame_records;
    if (every <= 0 || record->swap_number % every != 0) {
        return;
    }
    char text[1024];
    size_t size = sizeof(text);
    int64_t start_ns = record->phase_ns[FSAPP_FRAME_PHASE_EVENTS];
    int n = SDL_snprintf(text, size, "Frame record %lld: %s=%lld", (long long)record->swap_number,
                         fsapp_frame_get_phase_name(FSAPP_FRAME_PHASE_EVENTS),
                         (long long)SDL_NS_TO_US(start_ns));
    for (int phase = FSAPP_FRAME_PHASE_EVENTS + 1; phase < FSAPP_FRAME_PHASE_COUNT; phase++) {
        n += SDL_snprintf(text + n, size - n, " %s=%+lld",
                          fsapp_frame_get_phase_name((fsapp_frame_phase_t)phase),
                          (long long)SDL_NS_TO_US(record->phase_ns[phase] - start_ns));
    }
    n += SDL_snprintf(text + n, size - n, " rendering_finished_at=%+lld swap_returned_at=%+lld",
                      (long long)SDL_NS_TO_US(record->rendering_finished_ns - start_ns),
                      (long long)SDL_NS_TO_US(record->swap_ns - start_ns));
    if (record->host_vertical_blank == FSAPP_FRAME_HOST_VERTICAL_BLANK_UNKNOWN) {
        n += SDL_snprintf(text + n, size - n, " host_vertical_blank=unknown");
    } else {
        n += SDL_snprintf(text + n, size - n, " host_vertical_blank=%+lld (%s)",
                          (long long)SDL_NS_TO_US(record->host_vertical_blank_ns - start_ns),
                          host_vertical_blank_name(record->host_vertical_blank));
    }
    n += SDL_snprintf(text + n, size - n, " ui_frame_lag_swaps=%lld ui_frame_lag_us=%lld",
                      (long long)record->ui_frame_lag_swaps,
                      (long long)SDL_NS_TO_US(record->ui_frame_lag_ns));
    if (record->presentation_waiting >= 0) {
        n += SDL_snprintf(text + n, size - n, " presentation_waiting=%d",
                          record->presentation_waiting);
    }
    for (int i = 0; i < record->presented_count; i++) {
        const fsapp_frame_presented_t *presented = &record->presented[i];
        if (presented->presented_ns == 0) {
            n += SDL_snprintf(text + n, size - n, " swap_%lld=discarded",
                              (long long)presented->swap_number);
        } else {
            n += SDL_snprintf(text + n, size - n, " swap_%lld_presented_at=%+lld",
                              (long long)presented->swap_number,
                              (long long)SDL_NS_TO_US(presented->presented_ns - start_ns));
        }
    }
    for (int i = 0; i < record->display_refresh_count; i++) {
        n += SDL_snprintf(
            text + n, size - n, " display_refresh_at=%+lld",
            (long long)SDL_NS_TO_US(record->display_refreshes[i].refresh_ns - start_ns));
    }
    SDL_Log("%s", text);
}

// Traces the frame's phases as spans (each until the next starts, the last until end_ns) and the
// host vertical blank, with the swap number. In render thread mode, the events and the per-frame
// work's host window side are the times of the main thread's pass that wrote the snapshot, which
// that pass traced on its own track (trace_host_window_pass): on the render thread's track, the
// frame starts with the per-frame work's drawing side (the drawing hooks), from drawing_ns.
static void trace_frame_record(const fsapp_frame_record_t *record, int64_t drawing_ns,
                               int64_t end_ns)
{
    int first_phase = FSAPP_FRAME_PHASE_EVENTS;
    if (t_render_thread) {
        FS_AddTraceSpan(fsapp_frame_get_phase_name(FSAPP_FRAME_PHASE_FRAME_WORK), drawing_ns,
                        record->phase_ns[FSAPP_FRAME_PHASE_UI_UPLOAD], FS_TRACE_NO_FRAME, "swap",
                        record->swap_number);
        first_phase = FSAPP_FRAME_PHASE_UI_UPLOAD;
    }
    for (int phase = first_phase; phase < FSAPP_FRAME_PHASE_COUNT; phase++) {
        int64_t phase_end_ns =
            phase + 1 < FSAPP_FRAME_PHASE_COUNT ? record->phase_ns[phase + 1] : end_ns;
        FS_AddTraceSpan(fsapp_frame_get_phase_name((fsapp_frame_phase_t)phase),
                        record->phase_ns[phase], phase_end_ns, FS_TRACE_NO_FRAME, "swap",
                        record->swap_number);
    }
    if (record->host_vertical_blank != FSAPP_FRAME_HOST_VERTICAL_BLANK_UNKNOWN) {
        FS_AddTraceInstant(record->host_vertical_blank == FSAPP_FRAME_HOST_VERTICAL_BLANK_OBSERVED
                               ? "host vertical blank (observed)"
                               : "host vertical blank (estimated)",
                           record->host_vertical_blank_ns, FS_TRACE_NO_FRAME, "swap",
                           record->swap_number);
    }
    // The presentations reported since the frame before, at the times they were shown (earlier
    // than the frame's own events), and those discarded when they were reported: with Wayland's
    // kind bits and refresh period, or X11 Present's completion mode.
    for (int i = 0; i < record->presented_count; i++) {
        const fsapp_frame_presented_t *presented = &record->presented[i];
        if (presented->presented_ns == 0) {
            FS_AddTraceInstant("presentation discarded", record->swap_ns, FS_TRACE_NO_FRAME, "swap",
                               presented->swap_number);
            continue;
        }
        bool x11 = presented->mode != FSAPP_FRAME_PRESENTED_MODE_NONE;
        FS_TraceValue values[] = {
            {"swap", presented->swap_number},
            {"sequence", (int64_t)presented->sequence},
            {x11 ? "mode" : "flags", x11 ? presented->mode : (int64_t)presented->flags},
            {"refresh_us", presented->refresh_ns},
        };
        // X11 reports no refresh period.
        int count = (int)SDL_arraysize(values) - (x11 ? 1 : 0);
        FS_AddTraceInstantWithValues("presented", presented->presented_ns, FS_TRACE_NO_FRAME,
                                     values, count);
    }
    // The swaps still waiting for their presentation, this one included.
    if (record->presentation_waiting >= 0) {
        FS_TraceValue waiting = {"swaps", record->presentation_waiting};
        FS_AddTraceCounter("swaps waiting for presentation", record->swap_ns, &waiting, 1);
    }
    // The display's refreshes reported since the frame before, at their times, with the period
    // and how long after the refresh the windowing system expected a frame drawn then to be shown.
    for (int i = 0; i < record->display_refresh_count; i++) {
        const fsapp_frame_display_refresh_t *refresh = &record->display_refreshes[i];
        FS_TraceValue values[2];
        int count = 0;
        if (refresh->period_ns > 0) {
            values[count++] = {"period_us", refresh->period_ns};
        }
        if (refresh->output_ns != 0) {
            values[count++] = {"output_after_us", refresh->output_ns - refresh->refresh_ns};
        }
        FS_AddTraceInstantWithValues("display refresh", refresh->refresh_ns, FS_TRACE_NO_FRAME,
                                     values, count);
    }
}

static void frame_after_swap(const fsapp_frame_record_t *record)
{
    FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE();
    // The pacer paces the loop in its wait hook, and gets the frame's record (complete by now).
    // Without a pacer, the loop waits until the next frame is due (frame_wait_until_due) before
    // the next frame's host window side runs again, so that it still runs on every wake.
    if (drawing.taken.snapshot.pacer.set && drawing.taken.snapshot.pacer.pacer.swapped) {
        drawing.taken.snapshot.pacer.pacer.swapped(record);
    }
}

// ----------------------------------------------------------------------------

// The drawing side of a frame (see fsapp-mainthread.h), from the snapshot it has taken (with the
// start times of the host window side's phases, the events and the per-frame work's frame hooks,
// for the frame's record).
static void fsapp_mainthread_draw_frame(void)
{
    if (!drawing.init_tried) {
        drawing.init_tried = true;
        drawing.initialized = drawing_init();
    }
    if (!drawing.initialized) {
        return;
    }
    fsapp_frame_record_t record = {};
    record.phase_ns[FSAPP_FRAME_PHASE_EVENTS] = drawing.taken.snapshot.phases.events_ns;
    record.phase_ns[FSAPP_FRAME_PHASE_FRAME_WORK] = drawing.taken.snapshot.phases.frame_work_ns;

    // The per-frame work's drawing side: the drawing hooks (traced on the render thread's track
    // from here in render thread mode).

    int64_t drawing_ns = FS_IsTraceEnabled() ? (int64_t)SDL_GetTicksNS() : 0;
    frame_drawing_hooks();

    // The UI upload.

    record.phase_ns[FSAPP_FRAME_PHASE_UI_UPLOAD] = (int64_t)SDL_GetTicksNS();
    frame_ui_upload();
    if (drawing.taken.snapshot.layout.takes_record) {
        // The UI frame lag is counted from the last frame that took a record in.
        drawing.ui_frame_swap_number = drawing.swap_number + 1;
        drawing.ui_frame_ns = record.phase_ns[FSAPP_FRAME_PHASE_UI_UPLOAD];
    }

    // Drawing below: the frame plan, then its opaque entries, front to back, and its translucent
    // entries behind the emulation surface (fsapp-compositor.h).

    record.phase_ns[FSAPP_FRAME_PHASE_DRAWING_BELOW] = (int64_t)SDL_GetTicksNS();
    fsapp_compositor_draw_below(&drawing.taken.snapshot);

    // The wait: the pacer's wait hook, which ends the frame without drawing or swapping on quit
    // (the emulation may already have stopped, so no new content would come).

    record.phase_ns[FSAPP_FRAME_PHASE_WAIT] = (int64_t)SDL_GetTicksNS();
    if (frame_wait_for_pacer() == FSAPP_PACER_QUIT) {
        return;
    }

    // Drawing the paced surface (the frame plan's emulation surface, at its depth), from new
    // content or its old content, and also without a pacer.

    record.phase_ns[FSAPP_FRAME_PHASE_DRAWING_PACED] = (int64_t)SDL_GetTicksNS();
    fsapp_compositor_draw_paced_surface(&drawing.taken.snapshot);

    // Drawing above: the frame plan's translucent entries in front of the emulation surface.

    record.phase_ns[FSAPP_FRAME_PHASE_DRAWING_ABOVE] = (int64_t)SDL_GetTicksNS();
    fsapp_compositor_draw_above(&drawing.taken.snapshot);

    // Rendering finished.

    record.phase_ns[FSAPP_FRAME_PHASE_RENDERING_FINISHED] = (int64_t)SDL_GetTicksNS();
    frame_rendering_finished(&record);

    // The swap.

    record.phase_ns[FSAPP_FRAME_PHASE_SWAP] = (int64_t)SDL_GetTicksNS();
    frame_swap(&record);

    // After the swap: the host vertical blank and the UI frame lag complete the frame's record,
    // then the pacer's swapped hook, or pacing without a pacer.

    record.phase_ns[FSAPP_FRAME_PHASE_AFTER_SWAP] = record.swap_ns;
    frame_host_vertical_blank(&record);
    frame_presented(&record);
    frame_ui_frame_lag(&record);
    log_frame_record(&record);
    frame_after_swap(&record);
    if (FS_IsTraceEnabled()) {
        trace_frame_record(&record, drawing_ns, (int64_t)SDL_GetTicksNS());
    }
}

// ----------------------------------------------------------------------------
// The render thread (render thread mode).

// Waits until the render thread is to start a frame (render thread), and takes the next snapshot
// if one was written since its last frame; else what the snapshot it has hands over only once is
// cleared, so that the frame draws it again without uploading, freeing or taking in anything twice.
// With a pacer in the snapshot it has, a frame starts at once (frame after frame, paced by the
// pacer's wait); without one, when a frame is wanted. Returns false when the render thread is to
// stop.
static bool render_wait_for_frame(void)
{
    SDL_LockMutex(render.mutex);
    // A pacer only paces once the drawing side's state is made (else its frames are not drawn,
    // and would follow each other at once).
    bool paced = drawing.initialized && drawing.taken.snapshot.pacer.set;
    while (!SDL_GetAtomicInt(&render.stop) && !render.wanted && !paced) {
        SDL_WaitCondition(render.condition, render.mutex);
    }
    bool stop = SDL_GetAtomicInt(&render.stop) != 0;
    bool took = false;
    if (!stop) {
        render.wanted = false;
        if (render.next_written) {
            drawing.taken.swap(render.next);
            render.next_written = false;
            took = true;
        }
    }
    SDL_UnlockMutex(render.mutex);
    if (!stop && !took) {
        drawing.taken.clear_handed_over_once();
    }
    return !stop;
}

static int SDLCALL render_thread_function(void *data)
{
    (void)data;
    t_render_thread = true;
    // The render thread draws and swaps each frame (on macOS, kept on the fast cores and clocks).
    if (!FS_SetCurrentThreadInteractive()) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "[THREAD] Could not give the thread the user-interactive QoS class: %s",
                    SDL_GetError());
    }
    if (!SDL_GL_MakeCurrent(render.window, render.context)) {
        char *message = NULL;
        if (SDL_asprintf(&message,
                         "OpenGL 2.1 is needed, but the OpenGL context could not be made current "
                         "on the render thread: %s",
                         SDL_GetError()) < 0) {
            message = NULL;
        }
        drawing_stop_with_error(message ? message : "OpenGL 2.1 is needed");
        SDL_free(message);
        // No frame is drawn: the drawing side's state is not made.
        drawing.init_tried = true;
    }
    while (render_wait_for_frame()) {
        // The frame starts: a UI frame requested for it runs now (fsapp-pythonthread.h).
        fsapp_pythonthread_on_frame_started();
        fsapp_mainthread_draw_frame();
    }
    // The main thread makes the context current again once it has joined this thread.
    SDL_GL_MakeCurrent(render.window, NULL);
    SDL_SetAtomicInt(&render.ended, 1);
    return 0;
}

// Starts the render thread (render thread mode), once the host window and its GL context exist:
// the main thread releases the context, which the render thread makes current.
static void render_thread_start(void)
{
    SDL_assert(SDL_IsMainThread());
    SDL_assert(render.thread == NULL);
    render.window = g_window;
    render.context = SDL_GL_GetCurrentContext();
    if (render.mutex == NULL) {
        render.mutex = SDL_CreateMutex();
        render.condition = SDL_CreateCondition();
    }
    if (!SDL_GL_MakeCurrent(g_window, NULL)) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Could not release the GL context: %s",
                    SDL_GetError());
    }
    SDL_SetAtomicInt(&render.ended, 0);
    SDL_SetAtomicInt(&render.running, 1);
    render.thread = SDL_CreateThread(render_thread_function, "render", NULL);
    if (render.thread == NULL) {
        SDL_SetAtomicInt(&render.running, 0);
        SDL_GL_MakeCurrent(g_window, render.context);
        char *message = NULL;
        if (SDL_asprintf(&message, "The render thread could not be started: %s", SDL_GetError()) <
            0) {
            message = NULL;
        }
        fsapp_main_stop_with_error(message ? message : "The render thread could not be started");
        SDL_free(message);
        return;
    }
    SDL_Log("Render thread started");
}

// Stops the render thread when the main loop has ended (main thread): it is asked to stop and
// woken, ends its frame (its frame wait, and with it the pacer's wait, returns quit), releases the
// context and ends, and is joined. The only time the main thread waits for it, bounded by one
// frame (a swap, a glFinish() or the pacer's bounded wait). The render thread waits for the main
// thread only on macOS, in a swap after the host window changed (a resize, a fullscreen switch),
// until the main thread has run the GL context's update that SDL sends to the main queue (see
// create_window in fsapp-main.cpp); so the main thread pumps the events, which runs the main
// queue, until the render thread has ended. The context is then current on the main thread again,
// as in one-thread mode after the loop.
static void render_thread_stop(void)
{
    if (render.thread == NULL) {
        return;
    }
    SDL_SetAtomicInt(&render.stop, 1);
    SDL_LockMutex(render.mutex);
    SDL_BroadcastCondition(render.condition);
    SDL_UnlockMutex(render.mutex);
#ifdef __APPLE__
    while (!SDL_GetAtomicInt(&render.ended)) {
        SDL_PumpEvents();
        SDL_DelayNS(SDL_MS_TO_NS(1));
    }
#endif
    SDL_WaitThread(render.thread, NULL);
    render.thread = NULL;
    SDL_SetAtomicInt(&render.running, 0);
    SDL_Log("Render thread ended");
    if (!SDL_GL_MakeCurrent(render.window, render.context)) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Could not make the GL context current: %s",
                    SDL_GetError());
    }
}

// ----------------------------------------------------------------------------

// Traces a pass of the host window side in render thread mode, on the main thread's track: a span
// from the event pump to the end of the pass (before it waits), with whether it wrote the next
// snapshot, and in it the pass's events and per-frame work (the display, the frame hooks and, when
// it wrote one, writing the snapshot), named as the frame record's phases. In one-thread mode the
// main thread's phases are traced from the frame record instead (trace_frame_record).
static void trace_host_window_pass(int64_t events_ns, int64_t frame_work_ns, bool wrote_snapshot)
{
    if (!render.on || !FS_IsTraceEnabled()) {
        return;
    }
    int64_t end_ns = (int64_t)SDL_GetTicksNS();
    FS_AddTraceSpan("host window side", events_ns, end_ns, FS_TRACE_NO_FRAME, "snapshot",
                    wrote_snapshot ? 1 : 0);
    FS_AddTraceSpan(fsapp_frame_get_phase_name(FSAPP_FRAME_PHASE_EVENTS), events_ns, frame_work_ns,
                    FS_TRACE_NO_FRAME, FS_TRACE_NO_VALUE, 0);
    FS_AddTraceSpan(fsapp_frame_get_phase_name(FSAPP_FRAME_PHASE_FRAME_WORK), frame_work_ns, end_ns,
                    FS_TRACE_NO_FRAME, FS_TRACE_NO_VALUE, 0);
}

static void fsapp_mainthread_frame(void)
{
    // The host window side.

    FSAPP_MAINTHREAD_ASSERT_HOST_WINDOW_SIDE();

    // The events.

    int64_t events_ns = (int64_t)SDL_GetTicksNS();
    if (handle_events_and_check_quit()) {
        return;
    }

    // If no window yet, abort loop and try again (short delay to avoid excessive CPU usage)

    if (g_window == NULL) {
        frame_wait((int64_t)SDL_GetTicksNS() + (int64_t)SDL_MS_TO_NS(1), false, true);
        return;
    }

    // In render thread mode, the render thread starts once the host window exists.

    if (render.on && !render.start_tried) {
        render.start_tried = true;
        render_thread_start();
    }

    // The per-frame work's host window side: the host display, told to the display hooks when the
    // window first exists and when it changes.

    int64_t frame_work_ns = (int64_t)SDL_GetTicksNS();
    frame_update_display();

    // The frame hooks.

    for (fsapp_mainthread_frame_hook_t hook : hooks.frame) {
        hook();
    }

    // Without a pacer, the drawing side only runs when a frame is needed, and when it is due: until
    // then the loop waits, and the host window side runs again after the wait (on a wake, after an
    // event or when the frame is due), so the waiting is not part of the frame. No frame is needed
    // before the UI's first record (frame_is_needed).

    if (!frame_is_needed()) {
        trace_host_window_pass(events_ns, frame_work_ns, false);
        frame_wait_idle(IDLE_WAIT_NS);
        return;
    }
    if (!frame_is_due()) {
        trace_host_window_pass(events_ns, frame_work_ns, false);
        frame_wait_until_due();
        return;
    }
    frame_take_need();

    fsapp_mainthread.snapshot.phases.events_ns = events_ns;
    fsapp_mainthread.snapshot.phases.frame_work_ns = frame_work_ns;

    if (render.on) {
        // Render thread mode: where one-thread mode would run the drawing side, the host window
        // side writes the next snapshot and wants a frame, which the render thread draws (it
        // calls fsapp_pythonthread_on_frame_started when it starts the frame). It never waits
        // for the render thread. Without a pacer, the next pass waits until the next frame is
        // due, or idles, as above. With one, every pass writes the snapshot, so it is never older
        // than the last thing that changed, and then waits for a wake or an event: the render
        // thread draws frame after frame, paced by the pacer's wait.
        render_write_next_snapshot();
        trace_host_window_pass(events_ns, frame_work_ns, true);
        if (fsapp_mainthread.pacer.set) {
            // The idle wait can miss a wake (see IDLE_WAIT_NS), and the next snapshot is written
            // only by the next pass: what changed meanwhile (a UI frame's record, whose UI thread
            // waits for it to be taken in) would reach the render thread late by up to the wait.
            // With a pacer the wait is bounded by one host refresh period instead, so a lost wake
            // costs at most a frame; the wakes come with every slice posted anyway, so this adds
            // few passes.
            frame_wait_idle(paced_idle_wait_ns());
        }
        return;
    }

    // The frame starts: a UI frame requested for it runs now (fsapp-pythonthread.h).

    fsapp_pythonthread_on_frame_started();

    // Write the snapshot for the drawing side.

    frame_write_snapshot(NULL);

    // The drawing side.

    fsapp_mainthread.drawing_side = true;
    take_snapshot();
    fsapp_mainthread_draw_frame();
    fsapp_mainthread.drawing_side = false;
}

void fsapp_mainthread_run(fsapp_mainthread_quit_requested_t quit_requested)
{
    fsapp_mainthread.quit_requested = quit_requested;
    fsapp_mainthread.running = true;
    // A developer aid: log every Nth frame record (for example 100), to check the record without
    // the performance graph.
    const char *log_frame_records = SDL_getenv("FSAPP_LOG_FRAME_RECORDS");
    fsapp_mainthread.log_frame_records = log_frame_records ? SDL_atoi(log_frame_records) : 0;
    fsapp_testaids_init();
    frame_wake_init();
    // The mode is decided before the host window is created (fsapp-main.cpp), or here at the
    // latest, and recorded at the start of the trace.
    bool render_thread = fsapp_mainthread_has_render_thread();
    if (FS_IsTraceEnabled()) {
        FS_AddTraceInstant("main loop mode", (int64_t)SDL_GetTicksNS(), FS_TRACE_NO_FRAME,
                           "render_thread", render_thread ? 1 : 0);
    }
    // At the same time as the trace's instant, so that the log and the trace can be matched (the
    // log test checks it).
    SDL_Log("Main loop started (%s mode)", render_thread ? "render thread" : "one-thread");
    while (!fsapp_mainthread.quit_requested()) {
        fsapp_mainthread_frame();
    }
    render_thread_stop();
    // The drawing side has ended (the render thread has, or it ran on this thread).
    fsapp_presentation_quit();
    fsapp_testaids_quit();
    if (drawing.gl_grab_quit_timer != 0) {
        SDL_RemoveTimer(drawing.gl_grab_quit_timer);
        drawing.gl_grab_quit_timer = 0;
    }
    SDL_Log("Main loop ended after %lld swaps", (long long)drawing.swap_number);
    // No frame takes in the adornments' changes or the surfaces destroyed outside a UI frame any
    // more.
    // A next snapshot the render thread never took may still hand over destroyed adornments.
    fsapp_adornment_quit(render.next_written ? &render.next.snapshot.adornments : NULL);
    fsapp_surface_quit();
}

bool fsapp_mainthread_run_until_flag(SDL_AtomicInt *flag, int64_t timeout_ns)
{
    // Only the events are pumped: nothing that ends here waits for the main thread (a pacer's
    // content producer never blocks on it). The events are pumped with the frame wait, for up to
    // 1 ms at a time (or until woken), without checking for quit (which has been requested).
    int64_t until_ns = (int64_t)SDL_GetTicksNS() + timeout_ns;
    while (!SDL_GetAtomicInt(flag)) {
        int64_t now_ns = (int64_t)SDL_GetTicksNS();
        if (now_ns >= until_ns) {
            return false;
        }
        frame_wait(SDL_min(until_ns, now_ns + (int64_t)SDL_MS_TO_NS(1)), false, false);
    }
    return true;
}
