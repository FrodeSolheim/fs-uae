// ----------------------------------------------------------------------------
// FSEMU library; Copyright (c) 2011-2021 Frode Solheim. See the file COPYING.
// ----------------------------------------------------------------------------

#ifndef FSEMU_PERFGUI_H
#define FSEMU_PERFGUI_H

#include <SDL3/SDL.h>

#include "fsapp-frame.h"
#include "fsemu-config.h"

#ifdef __cplusplus
extern "C" {
#endif

// The performance graph, drawn by fsapp as adornments (fsapp-adornment.h) that fsemu fills, as two
// strips from one 480 x 360 image: video on the left (its rows, fsemu-graphrows.h: one row per
// emulated frame), audio on the right (each frame's audio in the same row). The sidebars are opaque
// grey strips attached in front of the emulation surface, each filling the bar beside the picture
// (shown only when its bar is at least 240 pixels wide). The detailed graph is translucent colour
// strips in front of everything at the window's left and right edges, 240 pixels wide scaled by
// whole steps. Both are filtered linearly. Its mode can be changed from any thread.

// Reads the performance_gui option (the environment's FSEMU_PERFORMANCE_GUI). Called at fsemu's
// init, on the main thread.
void fsemu_perfgui_init(void);

// Sets the mode from a performance_gui value: off (or 0), sidebars (or 1) or detailed (or 2).
// Returns false, and leaves the mode as it was, for another value. Any thread.
bool fsemu_perfgui_set_mode_from_string(const char *value);

// Alt+O: toggles the mode between off and detailed (from sidebars, turns it off). Any thread.
int fsemu_perfgui_toggle_mode(void);

#ifdef FSEMU_INTERNAL

typedef enum {
    FSEMU_PERFGUI_MODE_OFF,
    // The sidebars (performance_gui=sidebars or 1).
    FSEMU_PERFGUI_MODE_SIDEBARS,
    // The detailed graph (performance_gui=detailed or 2).
    FSEMU_PERFGUI_MODE_DETAILED
} fsemu_perfgui_mode_t;

fsemu_perfgui_mode_t fsemu_perfgui_mode(void);

// From fsemu's frame hook (host window side): creates the graph's adornments when it is on and a
// session is running (or starting), places them in the window, and destroys them otherwise (once
// fsapp has taken in their content); then draws the rows that changed since the last commit and
// commits them, once fsapp has taken in the previous content (so once per swap, in both modes of
// the main loop).
void fsemu_perfgui_update_adornments(bool session);

// From where fsemu works out the picture rectangle (drawing side: its drawing hook and the commit
// at frame ready): places the sidebars in the bars beside the picture, both rectangles in pixels.
void fsemu_perfgui_place_sidebars(const SDL_Rect *surface_rect, const SDL_Rect *picture_rect);

// From fsemu's swapped hook (drawing side), after each swap: the swap's frame record, and the
// emulated frame it presented (the same one again for a repeat), or -1 when it presented none of
// its own (old content while pausing). Feeds the graph's rows with the frames up to it and the
// swap; the frame hook draws and commits them (fsemu_perfgui_update_adornments).
void fsemu_perfgui_on_swapped(const fsapp_frame_record_t *record, int frame);

#endif // FSEMU_INTERNAL

extern int fsemu_perfgui_log_level;

#define fsemu_perfgui_log(format, ...)                                                             \
    FS_LOG_INFO(fsemu_perfgui_log_level, "[FSE] [PFG]", format, ##__VA_ARGS__)

#define fsemu_perfgui_log_debug(format, ...)                                                       \
    FS_LOG_DEBUG(fsemu_perfgui_log_level, "[FSE] [PFG]", format, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* FSEMU_PERFGUI_H */
