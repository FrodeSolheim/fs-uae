/*
 * The main loop: the frames of the host window, on the main thread, with the drawing side on a
 * render thread of its own in render thread mode.
 *
 * A frame has two sides. In one-thread mode they run one after the other on the main thread; in
 * render thread mode the main thread runs the host window side and the render thread the drawing
 * side (below, "The two modes"):
 *
 * - The host window side: the events (the shortcuts, fsapp-main.h, which consume their key events;
 *   then routing, fsapp-input.h, which gives input to the event hooks or to the focused surface;
 *   then fsapp's handling, which carries out window operations such as resizing and fullscreen at
 *   once, and copies the events that routing leaves to the UI for the UI thread), the display hooks
 *   when the host display changes and the frame hooks. It then writes the frame's snapshot
 *   (fsapp-snapshot.h): the window's size in pixels and in window coordinates, its scale, its
 *   background colour, the host refresh rate and whether every swap waits for a host vertical
 *   blank, the surface layout (the pending record of a UI frame taken and applied, the flat list,
 *   and the record's content and destroyed surfaces), the adornments, and the pacer.
 * - The drawing side: everything with GL, the wait and the swap. It takes the snapshot when its
 *   frame starts and reads none of the host window side's state but that, and calls no window
 *   functions (but the swap and the swap interval). Its first frame makes fsapp's GL state (the
 *   GL grab's). In one-thread mode, events pumped in the wait are handled on the host window side,
 *   which runs for each of them and then hands back to the drawing side; window operations then
 *   take effect on the screen at the next frame.
 *
 * The two modes. The mode is chosen once, before the host window is created, with
 * FSAPP_RENDER_THREAD (1 on, 0 off; otherwise the default: off on every platform), and logged
 * with whether the variable or the default chose it (fsapp_mainthread_has_render_thread):
 *
 * - One-thread mode: the main thread runs both sides. The drawing side runs right after the host
 *   window side has written the snapshot, and the frame wait pumps the events while it blocks.
 * - Render thread mode: once the host window exists, the main thread releases the GL context and
 *   starts the render thread, which makes it current and runs the drawing side's frames; the main
 *   thread runs only the host window side and never waits for the render thread (but to join it
 *   when the loop ends). The host window side writes the next snapshot into one slot under a short
 *   lock (fsapp-snapshot.h), writing over one not taken yet, and carrying over what that one hands
 *   over only once (a record, destroyed surfaces and adornments, new adornment content: each by the
 *   module that owns it); the render thread takes the newest when its frame starts, or else draws
 *   from the one it has, without handing anything over twice. With a pacer in its snapshot the
 *   render thread draws frame after frame, paced by the pacer's wait, and the host window side
 *   writes the snapshot in every pass (each wake or event); without one, the host window side
 *   keeps the on-demand rules below, and where one-thread mode would run the drawing side, writes
 *   the next snapshot and wants a frame, which wakes the render thread to draw one (wanted frames
 *   do not queue). The frame wait on the render thread does not pump events (the main thread
 *   does). What the drawing side must have the main thread do (stopping with an error, quitting
 *   after the last GL grab) is handed to it (SDL_RunOnMainThread). When the loop ends, the render
 *   thread is asked to stop, its frame wait returns quit, and it is joined.
 *
 * Without a pacer the loop draws on demand: the host window side runs on every wake (and after
 * every event), but the drawing side only runs when a frame is needed: the UI handed over a
 * record, the window changed (resized, exposed, ...), an adornment changed (fsapp-adornment.h), the
 * pacer was set or cleared, a frame was requested (fsapp_frame_request), or keep drawing is held
 * (fsapp_frame_take_keep_drawing). It is drawn at up to the host refresh rate (60 Hz when not
 * known): a frame is due one refresh period after the previous one was (or now, when later), and
 * the loop waits for it in the frame wait, then runs the host window side again, which starts the
 * frame; a wake ends that wait too, so the host window side runs again first. When the pacer is
 * cleared, the next frame is due at once, and swap interval 1 applies before its swap.
 *
 * The phases of a frame, named the same in code (fsapp_frame_phase_t in fsapp-frame.h), the log
 * and the performance graph:
 *
 *  1. the events (events): the host window side's event pump and quit check;
 *  2. per-frame work (frame_work): the display hooks and the frame hooks on the host window side,
 *     and the snapshot, then the drawing hooks, which start the drawing side;
 *  3. the UI upload (ui_upload): clearing the window, uploading the content of the record the
 *     frame takes in, if it has one, and the adornments' new content (fsapp-adornment.h), and then
 *     marking the record taken in;
 *  4. drawing below (drawing_below): working out the frame plan, then its opaque surfaces and
 *     adornments and the emulation surface's fill, front to back, and the translucent ones behind
 *     the emulation surface (fsapp-compositor.h);
 *  5. the wait (wait): the pacer's wait hook (below), or nothing without a pacer;
 *  6. drawing the paced surface (drawing_paced): the surface the pacer names (the emulation
 *     surface), from its streamed content, whenever it is visible; without a pacer, a visible
 *     surface with streamed content all the same;
 *  7. drawing above (drawing_above): the translucent surfaces and adornments in front of the
 *     emulation surface;
 *  8. rendering finished (rendering_finished): the glFinish() before the swap;
 *  9. the swap (swap): applying the swap interval, the GL grab and the swap (held until the next
 *     tick of the simulated display, FSAPP_SIMULATED_DISPLAY_HZ, unless the swap interval is 0);
 * 10. after the swap (after_swap): the glFinish() after the swap while the pacer follows the host
 *     vertical blank, the pacer's swapped hook with the frame's record (fsapp_frame_record_t: the
 *     start of each phase, rendering finished, the swap, the host vertical blank and the UI frame
 *     lag). FSAPP_LOG_FRAME_RECORDS=N logs every Nth record. Without a pacer, the loop then
 *     waits until the next frame is due (below), before the next frame's phases start.
 *
 * A program with content on its own clock (FS-UAE's emulation, a video player) paces the loop with
 * a pacer (fsapp_pacer_t in fsapp-pacer.h), set and cleared on the host window side at any time
 * and taking effect when the next frame starts (through the snapshot). Its wait hook returns when
 * the frame should be finished and swapped: with new content, with no new content (the paced
 * surface's old content is drawn again), or on quit (the frame ends without drawing or swapping).
 * It waits with the frame wait (in one-thread mode the events are pumped while it blocks), and
 * whoever produces its content wakes the wait. Its wait and swapped hooks run on the drawing side
 * (the render thread in render thread mode), its other hooks on the host window side. The swap
 * interval belongs to whoever paces: the pacer's own while one is set, else 1.
 *
 * The drawing side's waits use the frame wait (fsapp_frame_wait in fsapp-frame.h) on SDL's clock,
 * which ends when woken from any thread (fsapp_frame_wake, which wakes both sides), at a time or
 * on quit; sending on the main channel wakes it. The host window side's own waits (until a frame
 * is due, idle) pump the events in both modes.
 *
 * The host window side creates the window and the GL context and makes no GL calls. The loop
 * records which side is running (in render thread mode, the thread), and debug builds assert it
 * in each side's entry points (FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE and
 * FSAPP_MAINTHREAD_ASSERT_HOST_WINDOW_SIDE).
 *
 * Everything a UI frame changes reaches the loop through one record (fsapp-surface.h), which the UI
 * thread hands over in a pending slot under a short lock at the end of the UI frame, waking the
 * loop; a record makes a frame needed. The host window side takes the pending record when it writes
 * the snapshot and applies it to its applied state (the live surfaces the flat list is built from,
 * rebuilt only when the layout changed); the snapshot carries the flat list in every frame, and the
 * record's content (each committed surface's buffer and size) and destroyed surfaces in the frame
 * that takes it. The drawing side uploads the content in the UI upload and then marks the record
 * taken in, which lets the UI thread start its next UI frame. The UI thread sleeps between UI
 * frames until it has a wake reason (fsapp-pythonthread.h): the host window side wakes it when it
 * copies events for it, and a frame that starts (on the host window side in one-thread mode, on
 * the render thread in render thread mode) after a UI frame was requested
 * (fsapp_pythonthread_request_ui_frame, which also requests the frame). Without a record the old
 * textures are drawn again: the main thread never waits for the UI thread. No frame is drawn or
 * swapped before the first record has been taken in, pacer or keep drawing notwithstanding (the
 * host window side keeps running), so the first swap already has the whole UI in it. The frame
 * record's UI frame lag is counted from the last frame that took a record in.
 *
 * The program and the modules it is built on add their work to the loop by registering hooks
 * (below), each kind called in the order registered.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#ifndef FSAPP_MAINTHREAD_H
#define FSAPP_MAINTHREAD_H

#include <SDL3/SDL.h>
#include <stdbool.h>

#include "fsapp-frame.h"
#include "fsapp-pacer.h"

#ifdef __cplusplus
extern "C" {
#endif

// ----------------------------------------------------------------------------
// Registrations: hooks that the program and the modules it is built on add to the main loop, on the
// main thread before fsapp_mainthread_run. Each kind of hook is called in the order registered.

struct fsapp_snapshot;
struct fsapp_snapshot_window;

// Called for the input events that routing gives the emulation (fsapp-input.h: keys and gamepad
// buttons and axes while the emulation surface has keyboard focus or the input grab is applied,
// mouse input while the grab is applied; never the menu input, but a release of Start made by fsapp
// when Start and Select become the menu input), and for every hotplug event (keyboards, mice,
// joysticks and gamepads added or removed); for no other event (host window side). Called before
// fsapp handles the event. Also called for the events pumped while the drawing side waits in
// one-thread mode, so it may run between the drawing side's own calls, and in render thread mode at
// the same time as them: what an event hook shares with drawing-side code must be atomic or locked.
typedef void (*fsapp_mainthread_event_hook_t)(SDL_Event *event);

void fsapp_mainthread_add_event_hook(fsapp_mainthread_event_hook_t hook);

// Calls the event hooks with an event that fsapp makes itself for the emulation (fsapp-input.h:
// the release of Start when Start and Select become the menu input). Host window side.
void fsapp_mainthread_call_event_hooks(SDL_Event *event);

// Called once per frame on the host window side, once the host window exists: after the events
// and the display hooks, and before the snapshot (also before the UI's first record, when no frame
// is drawn yet). In render thread mode, once per pass of the host window side (with a pacer, every
// wake or event). Must not make GL calls.
typedef void (*fsapp_mainthread_frame_hook_t)(void);

void fsapp_mainthread_add_frame_hook(fsapp_mainthread_frame_hook_t hook);

// Called once per frame at the start of the drawing side (the render thread in render thread mode),
// with the GL context current, after fsapp's own first-frame state is made and before the UI
// upload. window is the frame's
// snapshot of the host window's state (the drawing side must not ask the window itself).
typedef void (*fsapp_mainthread_drawing_hook_t)(const struct fsapp_snapshot_window *window);

void fsapp_mainthread_add_drawing_hook(fsapp_mainthread_drawing_hook_t hook);

// Called on the host window side when the host window exists (before the first frame hooks) and
// whenever the host refresh rate or the video driver's swap behaviour changes (the window moved to
// another display, a mode change). refresh_rate_hz is 60 when SDL cannot tell (as with the
// offscreen driver). every_swap_waits is true when every swap waits for a host vertical blank,
// whatever the swap interval (SDL's KMSDRM driver).
typedef void (*fsapp_mainthread_display_hook_t)(double refresh_rate_hz, bool every_swap_waits);

void fsapp_mainthread_add_display_hook(fsapp_mainthread_display_hook_t hook);

// ----------------------------------------------------------------------------
// The pacer (fsapp-pacer.h, and see the top of this file).

// Set the pacer (copied), or clear it with NULL, on the host window side (the main thread outside
// the drawing side, for example in a frame hook) at any time. Takes effect when the next frame
// starts: when it is set, that frame calls its wait hook and applies its swap interval before the
// swap; when it is cleared, the next frame is due at once and swap interval 1 applies before its
// swap.
void fsapp_mainthread_set_pacer(const fsapp_pacer_t *pacer);

// The pacer changes its swap interval and whether it follows the host vertical blank, from any
// thread, only when it wants another one. fsapp applies the swap interval before the next swap
// while the pacer is set, calling SDL_GL_SetSwapInterval only when it differs from the one last
// applied.
void fsapp_mainthread_set_pacer_swap_interval(int swap_interval, bool follows_host_vertical_blank);

// Returns true when the program wants the main loop to end. Called on the main thread after every
// event pump, also while waiting (never on the render thread).
typedef bool (*fsapp_mainthread_quit_requested_t)(void);

// Run the main loop until quit_requested returns true (or the UI thread has failed, which asks to
// quit with fsapp_main_quit). When it ends, it logs the number of swaps made
// ("Main loop ended after <n> swaps").
void fsapp_mainthread_run(fsapp_mainthread_quit_requested_t quit_requested);

// The GL grab, a developer and test aid for headless runs: saves the rendered back buffer (the
// emulation surface and the UI, as composited) just before the given swaps, as GLGrab_<swap>.png
// in dir (the current directory if NULL or empty). swaps is a comma-separated list of swap numbers
// in increasing order (at most 32), counted from the first swap; NULL or empty grabs nothing. Set
// on the main thread before fsapp_mainthread_run: from then on its state is the drawing side's.
void fsapp_mainthread_set_gl_grab(const char *swaps, const char *dir);

// Sets whether the GL grab asks the program to quit (fsapp_main_quit) once its last grab is
// written (false by default), for tests that end by themselves (the test scene, fsapp-testscene.h):
// at once, or idle_ns nanoseconds later (above 0), so that a test can see whether the main loop
// idles after the last grab (it logs the number of swaps made when it ends). Set on the main thread
// before fsapp_mainthread_run, as the GL grab.
void fsapp_mainthread_set_quit_after_gl_grab(bool quit, int64_t idle_ns);

// The number of swaps the main loop has made so far (the last swap's number, counted as the GL
// grab counts them; 0 before the first). Any thread: for the test scene's stand-in UI thread
// (fsapp-testscene.h), which makes its changes after given swaps.
int fsapp_mainthread_get_swap_count(void);

// The snapshot of the frame being drawn (fsapp-snapshot.h), for fsapp's own code on the drawing
// side (fsapp_surface_get_frame_rectangle). Drawing side, from the start of its frame.
const struct fsapp_snapshot *fsapp_mainthread_get_drawing_snapshot(void);

// Whether the render thread is on (render thread mode, see the top of this file). Decided and
// logged the first time it is called, which is on the main thread before the host window is
// created (or when the main loop starts, at the latest); the same for the rest of the process.
bool fsapp_mainthread_has_render_thread(void);

// Which side of the main loop's frame is running (see the top of this file). The host window side
// is the main thread outside the drawing side, so it is also the side before and after the main
// loop. In one-thread mode these are just a record kept on the main thread; in render thread mode
// the drawing side is the render thread, and the host window side the main thread. For the
// assertions below, which only fire in debug builds.
bool fsapp_mainthread_is_drawing_side(void);
bool fsapp_mainthread_is_host_window_side(void);

// Use in a function that belongs to one side: the drawing side's are fsapp's GL calls (the
// compositor, the GL grab, the swap interval), the host window side's are window operations and
// event dispatch.
#define FSAPP_MAINTHREAD_ASSERT_DRAWING_SIDE() SDL_assert(fsapp_mainthread_is_drawing_side())
#define FSAPP_MAINTHREAD_ASSERT_HOST_WINDOW_SIDE()                                                 \
    SDL_assert(fsapp_mainthread_is_host_window_side())

// ----------------------------------------------------------------------------
// Shutting down.

// Pump the events with the frame wait (nothing is drawn) until flag is non-zero or timeout_ns has
// passed. Returns true if the flag was set. Used when shutting down, after fsapp_mainthread_run has
// returned, so the window stays responsive while the program waits for its threads to end. Whoever
// sets the flag can call fsapp_frame_wake to end the wait at once.
bool fsapp_mainthread_run_until_flag(SDL_AtomicInt *flag, int64_t timeout_ns);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSAPP_MAINTHREAD_H */
