/*
 * The performance graph's rows: the video part's history, one row per emulated frame on a
 * timeline with x = 0 at the frame's origin, drawn into an RGBA image.
 *
 * Fed each emulated frame's points in time, each swap and the events (pause, resume, session
 * start, timeline reset), it works out each frame's fate (presented, dropped, repeated) and writes
 * the frame's row once the fate is known. It has no dependency on the GPU, SDL or other modules,
 * so that it can be tested alone (od-fs/tests/fsemu). The performance graph adornment
 * (fsemu-perfgui.h) feeds it and uploads the rows it changed. The terms are those of GLOSSARY.md;
 * the background stripe's categories and spans are the stripe recorder's (fsemu-stripe.h).
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef FSEMU_GRAPHROWS_H
#define FSEMU_GRAPHROWS_H

#include <stdbool.h>
#include <stdint.h>

#include "fsemu-stripe.h"

#ifdef __cplusplus
extern "C" {
#endif

// The video part of the image: columns and rows (one row per emulated frame).
#define FSEMU_GRAPHROWS_WIDTH 240
#define FSEMU_GRAPHROWS_HEIGHT 360

// The x axis: 120 columns per predicted host refresh period, from -0.5 to +1.5 periods, with x = 0
// (the predicted host vertical blank one refresh before the frame's target vertical blank) at
// column 60 and the target vertical blank at column 180.
#define FSEMU_GRAPHROWS_COLUMNS_PER_PERIOD 120
#define FSEMU_GRAPHROWS_ZERO_COLUMN 60
#define FSEMU_GRAPHROWS_TARGET_COLUMN 180

// The flag column: the strip of columns at the left edge that flags problems, each class in columns
// of its own, so that a row shows all of its problems: from the left, the frame's fate in two
// columns (a missed vertical blank, where the swap returned half a refresh or more after the target
// vertical blank, or a repeated frame; or a dropped frame), a late slot (it started a tenth of a
// period or more after the planned start, or for a whole-frame slot, which always starts a little
// late and catches up, its emulation was done as late after its planned end), a slot that started
// before the event pump ran (before the first events phase of the main loop after the host vertical
// blank before its swap deadline; not for whole-frame slots, which start before it), an
// oversleep, extra CPU late (each when the frame's late time adds up to enough), a late
// presentation (the windowing system reported the swap shown half a refresh or more after the
// target vertical blank, though the swap itself was on time: frames of latency the host vertical
// blank does not show, such as swaps queued ahead of the display), and in the last column what the
// row's timing must be read with: warp (a warped row shows no problem), or the
// timeline (the first row after a pause, a session start or a timeline reset, and the rows written
// while the prediction settles). A mark before the first column after it is drawn red in that
// column, a mark at or past +1.5 periods red in the last column; the marks past their limits (the
// late slot start, a whole-frame slot's late emulation done, frame ready or rendering finished at
// or after the target vertical blank, the late swap) are red too.
#define FSEMU_GRAPHROWS_FLAG_COLUMNS 8

// How many of the newest swaps' events phases are kept, to find a frame's event pump when its row
// is written (a dropped frame's row is written at a later swap).
#define FSEMU_GRAPHROWS_EVENTS_COUNT 8

// How many of the newest rows a presentation's swap is looked for in.
#define FSEMU_GRAPHROWS_PRESENTED_ROWS 32

// The display's refreshes (fsemu_graphrows_on_display_refresh): how many of the newest are kept for
// the rows written after them, how many of the newest rows a refresh is added to (each row whose
// axis has it between its edges), and the most one row draws.
#define FSEMU_GRAPHROWS_DISPLAY_REFRESHES 8
#define FSEMU_GRAPHROWS_DISPLAY_REFRESH_ROWS 8
#define FSEMU_GRAPHROWS_ROW_DISPLAY_REFRESHES 4

// How many frames can wait for their fate (added, not yet swapped or dropped). When more are
// added, the oldest is dropped.
#define FSEMU_GRAPHROWS_PENDING_COUNT 64

// A colour as it is written to the image: bytes R, G, B and A, premultiplied.
typedef struct fsemu_graphrows_color_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} fsemu_graphrows_color_t;

// The warnings' colours, the same in both modes: a mark past its limit, and the flag column's
// classes.
typedef struct fsemu_graphrows_warning_colors_t {
    // A mark past its limit, or beyond the edges (drawn in the edge column).
    fsemu_graphrows_color_t past_limit;
    // The flag column, one colour per class: the problems (a missed vertical blank or a repeated
    // frame, a dropped frame, a late slot start, an oversleep, extra CPU late) and the contexts
    // (warp, and the timeline: a timeline reset, a pause, a session start, or the prediction still
    // settling).
    fsemu_graphrows_color_t flag_missed;
    fsemu_graphrows_color_t flag_dropped;
    fsemu_graphrows_color_t flag_late_slot_start;
    fsemu_graphrows_color_t flag_oversleep;
    fsemu_graphrows_color_t flag_extra_cpu_late;
    fsemu_graphrows_color_t flag_warp;
    fsemu_graphrows_color_t flag_timeline;
    // A slot that started before the event pump ran.
    fsemu_graphrows_color_t flag_event_pump;
    // A late presentation.
    fsemu_graphrows_color_t flag_late_presentation;
} fsemu_graphrows_warning_colors_t;

// The colours of one mode (detailed or sidebars).
typedef struct fsemu_graphrows_palette_t {
    // A written row's background, and the row after the newest one (the cursor).
    fsemu_graphrows_color_t background;
    fsemu_graphrows_color_t cursor;
    // The faint guides at x = 0 and the target vertical blank, the swap deadline's guide, and the
    // slot margin's faint span before it.
    fsemu_graphrows_color_t guide;
    fsemu_graphrows_color_t deadline;
    fsemu_graphrows_color_t slot_margin;
    // The marks: the emulation's actual start (the slot start) and end (emulation done) alike, the
    // planned start dim (it shows only where the slot start is not in its column), and the event
    // pump (the input the frame's first lines see was read there), and when the windowing system
    // reported that the swap was shown (presented). The display's refreshes (the real vertical
    // blanks, where the windowing system reports them) under all marks, as a guide.
    fsemu_graphrows_color_t planned_start;
    fsemu_graphrows_color_t slot_start;
    fsemu_graphrows_color_t frame_ready;
    fsemu_graphrows_color_t rendering_finished;
    fsemu_graphrows_color_t swap;
    fsemu_graphrows_color_t host_vertical_blank;
    fsemu_graphrows_color_t emulation_done;
    fsemu_graphrows_color_t event_pump;
    fsemu_graphrows_color_t presented;
    fsemu_graphrows_color_t display_refresh;
    // The background stripe, by category (the background for NONE).
    fsemu_graphrows_color_t stripe[FSEMU_STRIPE_CATEGORY_COUNT];
    // Whether the drawing side's spans are drawn, and their colours: the hand-over (from the last
    // slice posted until it was taken in, up to frame ready), drawing the emulated frame (from the
    // start of drawing the paced surface to the glFinish() before the swap), the rendering wait
    // (that glFinish(), up to rendering finished) and the swap wait (the swap phase, up to the
    // swap's return). The spans that end in a mark are weak versions of it.
    bool drawing_side;
    fsemu_graphrows_color_t hand_over;
    fsemu_graphrows_color_t drawing;
    fsemu_graphrows_color_t rendering_wait;
    fsemu_graphrows_color_t swap_wait;
    // The warnings.
    fsemu_graphrows_warning_colors_t warnings;
} fsemu_graphrows_palette_t;

// The detailed graph's colours, and the sidebars' greys.
extern const fsemu_graphrows_palette_t fsemu_graphrows_detailed_palette;
extern const fsemu_graphrows_palette_t fsemu_graphrows_sidebars_palette;

// An emulated frame, as the emulation thread recorded it. Times are on SDL's clock, in
// nanoseconds, 0 when not known.
typedef struct fsemu_graphrows_frame_t {
    // The frame number (fsemu_frame_get_number), counting up by one per frame.
    int number;
    // A warped frame gets a row with no marks.
    bool warp;
    // Whether the frame's waits overslept, and whether its extra CPU blocks ran past their lines'
    // planned times (extra CPU late), by enough time in all to show (a column of the row): flagged
    // in the frame's row. Set by fsemu_graphrows_on_frame_ended from the frame's spans.
    bool oversleep;
    bool extra_cpu_late;
    // The frame's origin: x = 0 on its row. 0 when not known; x = 0 is then taken from the axis
    // and the frame's planned end.
    int64_t origin_ns;
    // The frame's planned start, when its emulation slot actually started (the overshoot after the
    // planned start), its planned end (which gives the target vertical blank), when its last slice
    // (the whole picture) was posted, and frame ready (the last slice was taken in and committed on
    // the drawing side).
    int64_t planned_start_ns;
    int64_t slot_start_ns;
    int64_t planned_end_ns;
    int64_t last_slice_posted_ns;
    int64_t frame_ready_ns;
    // Emulation done: when the frame's last line was emulated, before any end wait.
    int64_t emulation_done_ns;
    // The frame's swap deadline, and the slot margin its emulation slot was planned with (the slot
    // ends that long before the deadline): only for frames in planned emulation slots, 0 for
    // others, which draw neither.
    int64_t deadline_ns;
    int64_t slot_margin_ns;
} fsemu_graphrows_frame_t;

// A swap, from its frame record. Times as above, 0 when not known.
typedef struct fsemu_graphrows_swap_t {
    // The swap's number (the frame record's, counting from 1; 0 when not known), which a later
    // presentation (fsemu_graphrows_on_presented) is matched by.
    int64_t swap_number;
    // When the main loop frame that ended with this swap started its events phase: the event pump
    // after the swap before returned (0 when not known; the performance graph gives it only in
    // one-thread mode, where the events are not pumped while drawing and swapping).
    int64_t events_ns;
    // When the drawing side started drawing the paced surface (the pacer's wait had returned), when
    // it called the glFinish() before the swap (the rendering finished phase) and when that
    // returned (rendering finished), and when it started the swap (the swap phase) and when the
    // swap returned.
    int64_t drawing_paced_ns;
    int64_t rendering_wait_ns;
    int64_t rendering_finished_ns;
    int64_t swap_wait_ns;
    int64_t swap_ns;
    int64_t host_vertical_blank_ns;
    // When the windowing system reported that the swap was shown (given later, by
    // fsemu_graphrows_on_presented; 0 until then, and where it reports none).
    int64_t presented_ns;
} fsemu_graphrows_swap_t;

// The axis: the predicted host vertical blank, as one of its predicted times (0 when there is no
// prediction) and the refresh interval it predicts with, and whether it has settled. Rows written
// without a prediction show no marks or guides.
typedef struct fsemu_graphrows_axis_t {
    int64_t vertical_blank_ns;
    double interval_ns;
    bool settled;
} fsemu_graphrows_axis_t;

// What a row says besides its marks, for the flag column (which also flags the marks past their
// limits).
typedef enum {
    // The frame was emulated but never shown: a later frame was swapped before it.
    FSEMU_GRAPHROWS_FLAG_DROPPED = 1 << 0,
    // The frame was swapped again (it has a second swap mark).
    FSEMU_GRAPHROWS_FLAG_REPEATED = 1 << 1,
    FSEMU_GRAPHROWS_FLAG_WARP = 1 << 2,
    // The first row after a pause, a session start or a timeline reset.
    FSEMU_GRAPHROWS_FLAG_AFTER_PAUSE = 1 << 3,
    FSEMU_GRAPHROWS_FLAG_AFTER_SESSION_START = 1 << 4,
    FSEMU_GRAPHROWS_FLAG_AFTER_TIMELINE_RESET = 1 << 5,
    // Written while the predicted host vertical blank had not settled (or there was none).
    FSEMU_GRAPHROWS_FLAG_SETTLING = 1 << 6,
} fsemu_graphrows_flag_t;

// A written row: the frame, the swaps that presented it (the first, and a repeat), the axis it was
// drawn on, its flags, the first event pump after the host vertical blank before its swap deadline
// (drawn as a mark; 0 when it has no deadline or the event pump was not known when it was
// written), the display's refreshes between its edges, and its background stripe as a category
// per column (from the frame's spans, on the row's axis).
typedef struct fsemu_graphrows_row_t {
    bool written;
    fsemu_graphrows_frame_t frame;
    fsemu_graphrows_swap_t swaps[2];
    int swap_count;
    fsemu_graphrows_axis_t axis;
    unsigned int flags;
    int64_t event_pump_ns;
    int64_t display_refresh_ns[FSEMU_GRAPHROWS_ROW_DISPLAY_REFRESHES];
    int display_refresh_count;
    uint8_t stripe[FSEMU_GRAPHROWS_WIDTH];
} fsemu_graphrows_row_t;

// The history (caller-owned; set up with fsemu_graphrows_init). Not thread safe: fed and drawn on
// one thread.
typedef struct fsemu_graphrows_t {
    uint8_t *pixels;
    int stride;
    const fsemu_graphrows_palette_t *palette;
    fsemu_graphrows_axis_t axis;
    fsemu_graphrows_row_t rows[FSEMU_GRAPHROWS_HEIGHT];
    // The image row the next frame's row is written to, and the newest written one (-1 for none).
    int next_row;
    int newest_row;
    // The row of the newest presented frame (which a repeat amends), -1 for none.
    int presented_row;
    // Frames waiting for their fate, oldest first.
    fsemu_graphrows_frame_t pending[FSEMU_GRAPHROWS_PENDING_COUNT];
    unsigned int pending_flags[FSEMU_GRAPHROWS_PENDING_COUNT];
    int pending_count;
    // The newest swaps' events phases (0 for none), the newest at events_next - 1 (wrapping).
    int64_t events_ns[FSEMU_GRAPHROWS_EVENTS_COUNT];
    int events_next;
    // The newest display refreshes (0 for none), the newest at display_refresh_next - 1 (wrapping).
    int64_t display_refresh_ns[FSEMU_GRAPHROWS_DISPLAY_REFRESHES];
    int display_refresh_next;
    // The newest frame number added (frames must come in order), INT32_MIN before the first.
    int last_frame;
    bool paused;
    // Event flags for the next frame added.
    unsigned int next_flags;
    // The rows to draw (and upload): a span starting at damage_y, wrapping at the bottom.
    int damage_y;
    int damage_height;
} fsemu_graphrows_t;

// Sets up an empty history drawing into pixels (FSEMU_GRAPHROWS_HEIGHT rows of stride bytes, of
// which the first FSEMU_GRAPHROWS_WIDTH * 4 bytes are the video part), with the detailed palette.
// The image is cleared to transparent when first drawn.
void fsemu_graphrows_init(fsemu_graphrows_t *rows, uint8_t *pixels, int stride);

// Changes the palette (one of the two above); all rows are drawn again.
void fsemu_graphrows_set_palette(fsemu_graphrows_t *rows, const fsemu_graphrows_palette_t *palette);

// The prediction the next rows are written on (each row keeps the axis it was written on).
void fsemu_graphrows_set_axis(fsemu_graphrows_t *rows, const fsemu_graphrows_axis_t *axis);

// An emulated frame, in frame-number order, before its swap. Ignored while paused: no rows are
// written for the frame numbers a pause uses up.
void fsemu_graphrows_add_frame(fsemu_graphrows_t *rows, const fsemu_graphrows_frame_t *frame);

// A swap of the emulated frame frame_number (added before): the frame is presented, and the frames
// added before it that are still waiting are dropped. A swap of the frame presented last is a
// repeat, which amends its row. Ignored while paused.
void fsemu_graphrows_on_swapped(fsemu_graphrows_t *rows, int frame_number,
                                const fsemu_graphrows_swap_t *swap);

// Swap swap_number was shown at presented_ns, as the windowing system reported it (some swaps
// later): amends the row of the frame it presented (the row is drawn again). Ignored when no row
// among the newest FSEMU_GRAPHROWS_PRESENTED_ROWS has the swap (written too long ago, or not
// written).
void fsemu_graphrows_on_presented(fsemu_graphrows_t *rows, int64_t swap_number,
                                  int64_t presented_ns);

// The display refreshed at refresh_ns, as the windowing system reported it (the real vertical
// blank, not tied to a swap; only on macOS so far): drawn in each of the newest
// FSEMU_GRAPHROWS_DISPLAY_REFRESH_ROWS rows that has it between its edges (amending them), and kept
// for the rows written after it. Refreshes come in time order.
void fsemu_graphrows_on_display_refresh(fsemu_graphrows_t *rows, int64_t refresh_ns);

// The frame frame_number has ended: what the stripe recorder recorded of it (emulation done and the
// background stripe's spans), for its written row, which it amends (the row is drawn again). A
// frame's row is often written at its swap, before the frame has ended, so this comes after
// fsemu_graphrows_on_swapped has written the row. Ignored when the frame has no written row (not
// written yet, or gone).
void fsemu_graphrows_on_frame_ended(fsemu_graphrows_t *rows, int frame_number,
                                    const fsemu_stripe_frame_t *frame);

// The events. From pause until resume no rows are written. The first row after a resume, a session
// start or a timeline reset is flagged.
void fsemu_graphrows_on_pause(fsemu_graphrows_t *rows);
void fsemu_graphrows_on_resume(fsemu_graphrows_t *rows);
void fsemu_graphrows_on_session_start(fsemu_graphrows_t *rows);
void fsemu_graphrows_on_timeline_reset(fsemu_graphrows_t *rows);

// Draws the rows changed since the last draw into the image, and gives the span of image rows it
// changed: *damage_height rows from *damage_y, wrapping from the last row to the first (row
// (*damage_y + i) % FSEMU_GRAPHROWS_HEIGHT). Returns false, with an empty span, when nothing
// changed.
bool fsemu_graphrows_draw(fsemu_graphrows_t *rows, int *damage_y, int *damage_height);

// The frame number written in image row y, or -1 when the row has no frame (not written yet, or the
// cursor after the newest row).
int fsemu_graphrows_get_row_frame(const fsemu_graphrows_t *rows, int y);

// The image row of the newest written frame, -1 before the first. The row after it is the cursor.
int fsemu_graphrows_get_newest_row(const fsemu_graphrows_t *rows);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSEMU_GRAPHROWS_H */
