/*
 * The test scene: a scene of known surfaces, adornments and an emulation surface that fsapp draws
 * in place of the UI, from a file, for tests (the compositor test, od-fs/tests/compositor).
 *
 * FSAPP_TEST_SCENE=<path> makes fsapp_pythonthread_start (fsapp-pythonthread.h) load the scene and
 * start its stand-in UI thread instead of the Python UI thread, so no session and no Python UI is
 * started; the host window and its GL context are created and drawn as always. The stand-in UI
 * thread runs UI frames as the Python UI does (fsapp-pythonthread.h): it creates the host window
 * at its start (fsapp_main_create_window); its first UI frame creates, draws, shows and commits the
 * surfaces with the calls fsgui uses (fsapp-surface.h) and creates the adornments with fsapp's
 * adornment calls; and later UI frames make the scene's changes (at, below) with the same calls,
 * committing only what the scene's commit lines say, until it is stopped. It sleeps between UI
 * frames, with no timers. The GL grab writes the grabs the scene names next to the scene file, with
 * its usual names (GLGrab_<swap>.png, replacing FSUAE_GL_GRAB_FRAMES), keeps the main loop drawing
 * until the last one, and then asks the program to quit (at once, or after idle), so FS-UAE exits
 * by itself with exit code 0. A scene with no grab is drawn until the program is quit.
 *
 * The line format, one instruction per line (blank lines and lines starting with # are ignored;
 * numbers are decimal integers, colour components 0 to 255):
 *
 *   window <w> <h>
 *       The host window's size (640 x 480 by default).
 *   background <r> <g> <b> [<a>]
 *       The host window's background colour (alpha 255 when left out; opaque black by default).
 *   fill <r> <g> <b>
 *       The emulation surface's fill colour (black by default).
 *   surface <x> <y> <w> <h> <r> <g> <b> <a> [translucent] [z <z>]
 *       A surface filled with one straight RGBA colour (fsapp's filled rectangle), opaque unless
 *       translucent. Its z index is 0 unless given, so the surfaces are stacked in the order of the
 *       file (ties in creation order), later ones in front.
 *   emulation <x> <y> <w> <h> [z <z>] [linear|nearest] [rgb565]
 *       The emulation surface, at this place in the stacking order: a surface with streamed
 *       content in BGRA (or RGB565 with rgb565), drawn with linear (the default) or nearest
 *       filtering, with no content until a frame line commits one, so until then its whole
 *       rectangle shows the fill colour. At most one.
 *   adornment <surface index|all> <x> <y> <w> <h> <r> <g> <b> <a> [translucent]
 *       An adornment attached to the surface with that index (counted from 0 in the file's order,
 *       the emulation surface included; one earlier in the file) or in front of everything
 *       ("all"), opaque unless translucent, with content of one colour (straight RGBA, which the
 *       scene premultiplies).
 *   grab <swap>...
 *       Swaps to grab, in increasing order (at most 32 in all, over all grab lines).
 *   idle <ms>
 *       Keeps the scene running for that long (milliseconds) after its last grab, with nothing
 *       changing, before it quits, instead of quitting at the last grab. When the main loop ends,
 *       fsapp logs the number of swaps made ("Main loop ended after <n> swaps"): an idle host
 *       window makes none after the last grab (or one, already needed when the grab let go of
 *       keep drawing). No effect without a grab.
 *   at <swap>
 *       Starts a group of changes (the lines below, up to the next at line), which the stand-in
 *       UI thread makes in its first UI frame after that swap has been made, so they go with that
 *       UI frame's record and show from the frame that takes it in (a swap or two later); at 0,
 *       in the first UI frame, right after it makes the scene. Swaps in increasing order, over
 *       all at lines. The UI thread gets there by requesting a UI frame
 *       for each host window frame (fsapp_pythonthread_request_ui_frame) until it has made the
 *       last group; it sleeps between UI frames, as the Python UI does.
 *
 * The changes, each in the group of the at line before it, to the surface with that index (as for
 * adornments, the emulation surface included):
 *
 *   move <surface> <x> <y>
 *       Moves the surface to x, y.
 *   show <surface>
 *   hide <surface>
 *       Shows or hides the surface.
 *   translucent <surface> on|off
 *       Makes the surface translucent or opaque.
 *   draw <surface> <x> <y> <w> <h> <r> <g> <b> <a>
 *       Fills a rectangle of the surface's buffer (surface pixels) with a straight RGBA colour
 *       (fsapp's filled rectangle), without committing it: it shows only where a commit's damage
 *       covers it. Not the emulation surface (it has no buffer).
 *   commit <surface> [<x> <y> <w> <h>]...
 *       Commits the surface with these damaged rectangles (surface pixels, any numbers: fsapp
 *       clips them), or the whole surface without any. Not the emulation surface.
 *   resize <surface> <w> <h> <r> <g> <b> <a>
 *       Resizes the surface and fills its new buffer with a straight RGBA colour, without
 *       committing it: the new size shows with the surface's next commit (whole, whatever damage
 *       it names), and until then its old content at its old size. The emulation surface (no
 *       buffer, the colour is not used) changes size with the UI frame's record.
 *   delete <surface>
 *       Destroys the surface. No later line may change it.
 *
 * Stream instructions, also in the groups of at lines, for the emulation surface's streamed content
 * (an emulation surface earlier in the file). A scene with any sets a stand-in pacer for the
 * emulation surface (from a frame hook on the host window side, as fsemu does), whose wait hook
 * carries out the frame instructions due with fsapp's calls for streamed content (fsapp-surface.h)
 * on the drawing side, in the first frame after the at line's swap, and returns new content after a
 * commit; otherwise it waits about a host refresh period in the frame wait and returns no new
 * content. The picture and drop instructions are carried out by a drawing hook of the scene, before
 * drawing below in the first frame after the swap (also while paused), as fsemu refits and drops
 * its content, so in a group with a frame line they come first, whatever the order in the file. A
 * scene without stream instructions runs without a pacer. Not made by the stand-in UI thread.
 *
 *   frame <w> <h> crop <x> <y> <w> <h> picture <x> <y> <w> <h> <outside r g b> <top r g b>
 *         <bottom r g b> [slices <n>] [stop <k>]
 *       A generated frame (in the content's format) with an upload region of w x h pixels, in the
 *       outside colour except the crop (inside the region), whose top half (its first h / 2 rows)
 *       and bottom half are in the top and bottom colours. Its rows are further apart than the
 *       region is wide, with pixels in another colour after each row. It is begun, uploaded in n
 *       slices of whole rows (4 by default) and committed with the crop as the source rectangle
 *       and the picture rectangle (in the emulation surface's coordinates). With stop k, only the
 *       first k slices are uploaded, and it is never committed (the next frame abandons it).
 *   picture <x> <y> <w> <h>
 *       Sets the picture rectangle (in the emulation surface's coordinates) of the content shown,
 *       without a commit.
 *   drop
 *       Drops the content: the emulation surface shows its fill colour until the next commit.
 *   pause
 *   resume
 *       Clears the stand-in pacer (the content is drawn again without one), and sets it again,
 *       from the scene's frame hook in the first frame after the swap.
 *
 * A line that is not one of these stops the program with an error naming the file, the line number
 * and the line.
 *
 * Copyright (c) 2026 Frode Solheim
 */

#ifndef FSAPP_TESTSCENE_H
#define FSAPP_TESTSCENE_H

#ifdef __cplusplus
extern "C" {
#endif

struct fsapp_testscene;
typedef struct fsapp_testscene fsapp_testscene_t;

// Reads and checks the scene file. Returns the scene, or NULL with *error set to a message (freed
// with SDL_free) naming the file, and the line and what is wrong with it. Any thread.
fsapp_testscene_t *fsapp_testscene_load(const char *path, char **error);

// Sets up the GL grab for the scene's grabs, next to the scene file, and the quit after the last
// one (fsapp_mainthread_set_gl_grab, fsapp_mainthread_set_quit_after_gl_grab). Call on the main
// thread before the main loop runs.
void fsapp_testscene_set_up_gl_grab(const fsapp_testscene_t *scene);

// Sets up the stand-in pacer when the scene has stream instructions (registers its frame hook).
// Call on the main thread before the main loop runs, as fsapp_testscene_set_up_gl_grab.
void fsapp_testscene_set_up_pacer(fsapp_testscene_t *scene);

// The stand-in UI thread's start-up, before its first UI frame: creates the host window.
void fsapp_testscene_start(fsapp_testscene_t *scene);

// One UI frame of the stand-in UI thread: the first creates the surfaces (committed whole) and the
// adornments, and each makes the changes of the groups whose swap has been made (at) and requests
// the next UI frame while a group is still to come. The UI thread ends the UI frame
// (fsapp-pythonthread.cpp).
void fsapp_testscene_run_ui_frame(fsapp_testscene_t *scene);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSAPP_TESTSCENE_H */
