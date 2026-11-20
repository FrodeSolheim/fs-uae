/*
 * The performance graph's rows (see fsemu-graphrows.h).
 *
 * The inputs only update the rows' records; fsemu_graphrows_draw draws the changed rows from them.
 * Drawing a row is a series of steps over the row's pixels, each drawn over the one before: the
 * background, the slot margin's span, the background stripe, the guides, the marks and the flag
 * column.
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "fsemu-graphrows.h"

#include <math.h>
#include <string.h>

// ----------------------------------------------------------------------------
// The palettes.

// A colour from 0xRRGGBB and an alpha, premultiplied.
#define FSEMU_GRAPHROWS_COLOR(rgb, alpha)                                                          \
    {(uint8_t)((((rgb) >> 16) & 0xff) * (alpha) / 255),                                            \
     (uint8_t)((((rgb) >> 8) & 0xff) * (alpha) / 255), (uint8_t)(((rgb) & 0xff) * (alpha) / 255),  \
     (uint8_t)(alpha)}

// The sidebars' greys: white with the alpha as the grey level. The sidebars are opaque adornments,
// so fsapp ignores the alpha and the premultiplied white shows as the grey (transparent as black).
#define FSEMU_GRAPHROWS_GREY(level) FSEMU_GRAPHROWS_COLOR(0xffffff, level)

// The warnings' colours, the same in both palettes (the sidebars are grey but for them).
#define FSEMU_GRAPHROWS_WARNING_COLORS                                                             \
    {                                                                                              \
        FSEMU_GRAPHROWS_COLOR(0xff0000, 0xff), /* past_limit */                                    \
        FSEMU_GRAPHROWS_COLOR(0xff0000, 0xff), /* flag_missed */                                   \
        FSEMU_GRAPHROWS_COLOR(0xff8800, 0xff), /* flag_dropped */                                  \
        FSEMU_GRAPHROWS_COLOR(0xffff00, 0xff), /* flag_late_slot_start */                          \
        FSEMU_GRAPHROWS_COLOR(0xaa55ff, 0xff), /* flag_oversleep */                                \
        FSEMU_GRAPHROWS_COLOR(0x00ccff, 0xff), /* flag_extra_cpu_late */                           \
        FSEMU_GRAPHROWS_COLOR(0x00ff00, 0xff), /* flag_warp */                                     \
        FSEMU_GRAPHROWS_COLOR(0x3366ff, 0xff), /* flag_timeline */                                 \
        FSEMU_GRAPHROWS_COLOR(0xff55aa, 0xff), /* flag_event_pump */                               \
        FSEMU_GRAPHROWS_COLOR(0xffffff, 0xff), /* flag_late_presentation */                        \
    }

// The detailed graph: the marks in bright, saturated colours, with distinct hues for marks that are
// often close together (emulation done and frame ready, rendering finished, the swap and the host
// vertical blank), the emulation's actual start and end in white, the planned start dim; the
// background in dark colours, the emulation thread's work in blues (emulating blue, the work
// between frames muted cyan, extra CPU indigo), its line waits in darker blues (busy-waiting a
// little brighter than sleeping), its other waits in hues of their own, late in dark red; the
// drawing side's spans weak, those that end in a mark in weak versions of it.
const fsemu_graphrows_palette_t fsemu_graphrows_detailed_palette = {
    FSEMU_GRAPHROWS_COLOR(0x0c0c0c, 0xff), // background
    FSEMU_GRAPHROWS_COLOR(0xaaaaaa, 0xff), // cursor
    FSEMU_GRAPHROWS_COLOR(0x3a3a3a, 0xff), // guide
    FSEMU_GRAPHROWS_COLOR(0x707070, 0xff), // deadline
    FSEMU_GRAPHROWS_COLOR(0x24220e, 0xff), // slot_margin
    FSEMU_GRAPHROWS_COLOR(0x8c8c8c, 0xff), // planned start
    FSEMU_GRAPHROWS_COLOR(0xffffff, 0xff), // slot_start
    FSEMU_GRAPHROWS_COLOR(0x00ff40, 0xff), // frame_ready
    FSEMU_GRAPHROWS_COLOR(0xff8000, 0xff), // rendering_finished
    FSEMU_GRAPHROWS_COLOR(0xff00ff, 0xff), // swap
    FSEMU_GRAPHROWS_COLOR(0x8c8cff, 0xff), // host_vertical_blank
    FSEMU_GRAPHROWS_COLOR(0xffffff, 0xff), // emulation_done
    FSEMU_GRAPHROWS_COLOR(0xff55aa, 0xff), // event_pump
    FSEMU_GRAPHROWS_COLOR(0xffe000, 0xff), // presented
    FSEMU_GRAPHROWS_COLOR(0x40d8ff, 0xff), // display_refresh
    {
        FSEMU_GRAPHROWS_COLOR(0x0c0c0c, 0xff), // none (background)
        FSEMU_GRAPHROWS_COLOR(0x2468b0, 0xff), // emulating
        FSEMU_GRAPHROWS_COLOR(0x4c38a8, 0xff), // extra CPU
        FSEMU_GRAPHROWS_COLOR(0x2a7488, 0xff), // other work
        FSEMU_GRAPHROWS_COLOR(0x0c1a2e, 0xff), // sleeping (a line wait)
        FSEMU_GRAPHROWS_COLOR(0x14304e, 0xff), // busy-waiting (a line wait)
        FSEMU_GRAPHROWS_COLOR(0x221433, 0xff), // before the slot
        FSEMU_GRAPHROWS_COLOR(0x262626, 0xff), // end wait
        FSEMU_GRAPHROWS_COLOR(0x8c1010, 0xff), // oversleep
        FSEMU_GRAPHROWS_COLOR(0x781848, 0xff), // extra CPU late
    },
    true,                                  // drawing_side
    FSEMU_GRAPHROWS_COLOR(0x0e3a1a, 0xff), // hand_over
    FSEMU_GRAPHROWS_COLOR(0x444444, 0xff), // drawing
    FSEMU_GRAPHROWS_COLOR(0x5a2c00, 0xff), // rendering_wait
    FSEMU_GRAPHROWS_COLOR(0x380a38, 0xff), // swap_wait
    FSEMU_GRAPHROWS_WARNING_COLORS,
};

const fsemu_graphrows_palette_t fsemu_graphrows_sidebars_palette = {
    FSEMU_GRAPHROWS_GREY(0x0c), // background
    FSEMU_GRAPHROWS_GREY(0x40), // cursor
    FSEMU_GRAPHROWS_GREY(0x1c), // guide
    FSEMU_GRAPHROWS_GREY(0x30), // deadline
    FSEMU_GRAPHROWS_GREY(0x14), // slot_margin
    FSEMU_GRAPHROWS_GREY(0x30), // planned start
    FSEMU_GRAPHROWS_GREY(0x48), // slot_start
    FSEMU_GRAPHROWS_GREY(0x40), // frame_ready
    FSEMU_GRAPHROWS_GREY(0x40), // rendering_finished
    FSEMU_GRAPHROWS_GREY(0x60), // swap
    FSEMU_GRAPHROWS_GREY(0x40), // host_vertical_blank
    FSEMU_GRAPHROWS_GREY(0x48), // emulation_done
    FSEMU_GRAPHROWS_GREY(0x38), // event_pump
    FSEMU_GRAPHROWS_GREY(0x60), // presented
    FSEMU_GRAPHROWS_GREY(0x28), // display_refresh
    {
        // The background stripe: greys, with the late group in dim red.
        FSEMU_GRAPHROWS_GREY(0x0c),            // none (background)
        FSEMU_GRAPHROWS_GREY(0x2c),            // emulating
        FSEMU_GRAPHROWS_GREY(0x24),            // extra CPU
        FSEMU_GRAPHROWS_GREY(0x20),            // other work
        FSEMU_GRAPHROWS_GREY(0x12),            // sleeping
        FSEMU_GRAPHROWS_GREY(0x16),            // busy-waiting
        FSEMU_GRAPHROWS_GREY(0x10),            // before the slot
        FSEMU_GRAPHROWS_GREY(0x10),            // end wait
        FSEMU_GRAPHROWS_COLOR(0xff2020, 0x58), // oversleep
        FSEMU_GRAPHROWS_COLOR(0xff2050, 0x48), // extra CPU late
    },
    // The sidebars leave out the drawing side's spans.
    false,
    FSEMU_GRAPHROWS_GREY(0x0c), // hand_over
    FSEMU_GRAPHROWS_GREY(0x0c), // drawing
    FSEMU_GRAPHROWS_GREY(0x0c), // rendering_wait
    FSEMU_GRAPHROWS_GREY(0x0c), // swap_wait
    FSEMU_GRAPHROWS_WARNING_COLORS,
};

static const fsemu_graphrows_color_t fsemu_graphrows_transparent = {0, 0, 0, 0};

// ----------------------------------------------------------------------------
// Damage: one span of image rows starting at damage_y, wrapping at the bottom.

static void fsemu_graphrows_damage_all(fsemu_graphrows_t *rows)
{
    rows->damage_y = 0;
    rows->damage_height = FSEMU_GRAPHROWS_HEIGHT;
}

static void fsemu_graphrows_damage_row(fsemu_graphrows_t *rows, int y)
{
    const int height = FSEMU_GRAPHROWS_HEIGHT;
    if (rows->damage_height == 0) {
        rows->damage_y = y;
        rows->damage_height = 1;
        return;
    }
    // How far after the span's first row the row is (wrapping).
    int after = (y - rows->damage_y + height) % height;
    if (after < rows->damage_height) {
        return;
    }
    if (after < height / 2) {
        // After the span: the span grows down to it.
        rows->damage_height = after + 1;
    } else {
        // Before the span (an older row): the span starts there instead.
        int before = height - after;
        rows->damage_y = y;
        rows->damage_height += before;
    }
    if (rows->damage_height > height) {
        fsemu_graphrows_damage_all(rows);
    }
}

// ----------------------------------------------------------------------------
// Writing rows.

// An events phase this many refresh periods before the host vertical blank still counts as after
// it: with vsync the swap before returns at the host vertical blank, so the events phase starts
// after it, and only the prediction's error can put it before.
#define FSEMU_GRAPHROWS_EVENT_PUMP_EARLY_PERIODS 0.1

// The frame's event pump: the first events phase of the main loop (of the swaps so far) after the
// host vertical blank before its swap deadline, 0 when it has no deadline or no such events phase
// is known yet.
static int64_t fsemu_graphrows_find_event_pump(const fsemu_graphrows_t *rows,
                                               const fsemu_graphrows_frame_t *frame)
{
    double interval = rows->axis.interval_ns;
    if (frame->deadline_ns == 0 || rows->axis.vertical_blank_ns == 0 || interval <= 0.0) {
        return 0;
    }
    double after_ns =
        (double)frame->deadline_ns - interval - FSEMU_GRAPHROWS_EVENT_PUMP_EARLY_PERIODS * interval;
    int64_t event_pump_ns = 0;
    for (int i = 0; i < FSEMU_GRAPHROWS_EVENTS_COUNT; i++) {
        int64_t events_ns = rows->events_ns[i];
        if (events_ns != 0 && (double)events_ns >= after_ns &&
            (event_pump_ns == 0 || events_ns < event_pump_ns)) {
            event_pump_ns = events_ns;
        }
    }
    return event_pump_ns;
}

static bool fsemu_graphrows_add_row_display_refresh(fsemu_graphrows_row_t *row, int64_t refresh_ns);

// Writes the frame's row (its fate is known) as the newest row, with the swap that presented it
// (NULL for a dropped frame). The row after it becomes the cursor.
static void fsemu_graphrows_write_row(fsemu_graphrows_t *rows, const fsemu_graphrows_frame_t *frame,
                                      unsigned int flags, const fsemu_graphrows_swap_t *swap)
{
    int y = rows->next_row;
    fsemu_graphrows_row_t *row = &rows->rows[y];
    memset(row, 0, sizeof(*row));
    row->written = true;
    row->frame = *frame;
    row->axis = rows->axis;
    row->flags = flags;
    row->event_pump_ns = fsemu_graphrows_find_event_pump(rows, frame);
    if (frame->warp) {
        row->flags |= FSEMU_GRAPHROWS_FLAG_WARP;
    }
    if (!rows->axis.settled || rows->axis.vertical_blank_ns == 0) {
        row->flags |= FSEMU_GRAPHROWS_FLAG_SETTLING;
    }
    if (swap) {
        row->swaps[0] = *swap;
        row->swap_count = 1;
        rows->presented_row = y;
    } else {
        row->flags |= FSEMU_GRAPHROWS_FLAG_DROPPED;
    }
    // The display refreshes that came before it, oldest first.
    for (int i = 0; i < FSEMU_GRAPHROWS_DISPLAY_REFRESHES; i++) {
        int j = (rows->display_refresh_next + i) % FSEMU_GRAPHROWS_DISPLAY_REFRESHES;
        if (rows->display_refresh_ns[j] != 0) {
            fsemu_graphrows_add_row_display_refresh(row, rows->display_refresh_ns[j]);
        }
    }
    rows->newest_row = y;
    rows->next_row = (y + 1) % FSEMU_GRAPHROWS_HEIGHT;
    // The cursor row: the oldest row goes.
    rows->rows[rows->next_row].written = false;
    if (rows->presented_row == rows->next_row) {
        rows->presented_row = -1;
    }
    fsemu_graphrows_damage_row(rows, y);
    fsemu_graphrows_damage_row(rows, rows->next_row);
}

// Removes the first count pending frames.
static void fsemu_graphrows_remove_pending(fsemu_graphrows_t *rows, int count)
{
    rows->pending_count -= count;
    memmove(rows->pending, rows->pending + count, rows->pending_count * sizeof(rows->pending[0]));
    memmove(rows->pending_flags, rows->pending_flags + count,
            rows->pending_count * sizeof(rows->pending_flags[0]));
}

void fsemu_graphrows_init(fsemu_graphrows_t *rows, uint8_t *pixels, int stride)
{
    memset(rows, 0, sizeof(*rows));
    rows->pixels = pixels;
    rows->stride = stride;
    rows->palette = &fsemu_graphrows_detailed_palette;
    rows->newest_row = -1;
    rows->presented_row = -1;
    rows->last_frame = INT32_MIN;
    // The whole image is cleared at the first draw.
    fsemu_graphrows_damage_all(rows);
}

void fsemu_graphrows_set_palette(fsemu_graphrows_t *rows, const fsemu_graphrows_palette_t *palette)
{
    if (palette != rows->palette) {
        rows->palette = palette;
        fsemu_graphrows_damage_all(rows);
    }
}

void fsemu_graphrows_set_axis(fsemu_graphrows_t *rows, const fsemu_graphrows_axis_t *axis)
{
    rows->axis = *axis;
}

void fsemu_graphrows_add_frame(fsemu_graphrows_t *rows, const fsemu_graphrows_frame_t *frame)
{
    if (rows->paused || frame->number <= rows->last_frame) {
        return;
    }
    rows->last_frame = frame->number;
    if (rows->pending_count == FSEMU_GRAPHROWS_PENDING_COUNT) {
        // Too many frames without a swap (warp): the oldest is dropped (a later frame will be
        // swapped before it).
        fsemu_graphrows_write_row(rows, &rows->pending[0], rows->pending_flags[0], NULL);
        fsemu_graphrows_remove_pending(rows, 1);
    }
    rows->pending[rows->pending_count] = *frame;
    rows->pending_flags[rows->pending_count] = rows->next_flags;
    rows->pending_count += 1;
    rows->next_flags = 0;
}

void fsemu_graphrows_on_swapped(fsemu_graphrows_t *rows, int frame_number,
                                const fsemu_graphrows_swap_t *swap)
{
    if (rows->paused) {
        return;
    }
    // Every swap's events phase, a repeat's too, for the frames' event pumps.
    rows->events_ns[rows->events_next] = swap->events_ns;
    rows->events_next = (rows->events_next + 1) % FSEMU_GRAPHROWS_EVENTS_COUNT;
    if (rows->presented_row >= 0 && rows->rows[rows->presented_row].frame.number == frame_number) {
        // Swapped again: a repeat, drawn as a second swap mark in the frame's row.
        fsemu_graphrows_row_t *row = &rows->rows[rows->presented_row];
        row->flags |= FSEMU_GRAPHROWS_FLAG_REPEATED;
        if (row->swap_count < 2) {
            row->swaps[1] = *swap;
            row->swap_count = 2;
            fsemu_graphrows_damage_row(rows, rows->presented_row);
        }
        return;
    }
    // The frames waiting before it are dropped, and it is presented.
    int count = 0;
    while (count < rows->pending_count && rows->pending[count].number <= frame_number) {
        const fsemu_graphrows_frame_t *frame = &rows->pending[count];
        bool presented = frame->number == frame_number;
        fsemu_graphrows_write_row(rows, frame, rows->pending_flags[count], presented ? swap : NULL);
        count++;
    }
    fsemu_graphrows_remove_pending(rows, count);
}

void fsemu_graphrows_on_pause(fsemu_graphrows_t *rows)
{
    rows->paused = true;
}

void fsemu_graphrows_on_resume(fsemu_graphrows_t *rows)
{
    if (rows->paused) {
        rows->paused = false;
        rows->next_flags |= FSEMU_GRAPHROWS_FLAG_AFTER_PAUSE;
    }
}

void fsemu_graphrows_on_session_start(fsemu_graphrows_t *rows)
{
    rows->next_flags |= FSEMU_GRAPHROWS_FLAG_AFTER_SESSION_START;
}

void fsemu_graphrows_on_timeline_reset(fsemu_graphrows_t *rows)
{
    rows->next_flags |= FSEMU_GRAPHROWS_FLAG_AFTER_TIMELINE_RESET;
}

// ----------------------------------------------------------------------------
// Drawing a row: the steps.

// A row's pixels, and what it is drawn from.
typedef struct {
    uint8_t *pixels;
    const fsemu_graphrows_row_t *row;
    const fsemu_graphrows_palette_t *palette;
} fsemu_graphrows_drawing_t;

static void fsemu_graphrows_put(fsemu_graphrows_drawing_t *d, int x, fsemu_graphrows_color_t color)
{
    memcpy(d->pixels + x * 4, &color, 4);
}

static void fsemu_graphrows_fill(fsemu_graphrows_drawing_t *d, fsemu_graphrows_color_t color)
{
    for (int x = 0; x < FSEMU_GRAPHROWS_WIDTH; x++) {
        fsemu_graphrows_put(d, x, color);
    }
}

// Whether the row has an axis to draw on: a prediction when it was written.
static bool fsemu_graphrows_has_axis(const fsemu_graphrows_row_t *row)
{
    return row->axis.vertical_blank_ns != 0 && row->axis.interval_ns > 0.0;
}

// x = 0 for the row's frame: its origin, or when that is not known, the predicted host vertical
// blank one refresh before the target vertical blank, which is the first predicted one at or after
// the frame's planned end. False when the row has no axis, or the frame neither an origin nor a
// planned end.
static bool fsemu_graphrows_get_zero_ns(const fsemu_graphrows_row_t *row, double *zero_ns)
{
    if (!fsemu_graphrows_has_axis(row)) {
        return false;
    }
    if (row->frame.origin_ns != 0) {
        *zero_ns = (double)row->frame.origin_ns;
        return true;
    }
    if (row->frame.planned_end_ns == 0) {
        return false;
    }
    double interval = row->axis.interval_ns;
    double periods =
        ceil((double)(row->frame.planned_end_ns - row->axis.vertical_blank_ns) / interval);
    double target_ns = (double)row->axis.vertical_blank_ns + periods * interval;
    *zero_ns = target_ns - interval;
    return true;
}

// The column of time_ns on the row's axis (with x = 0 at zero_ns), drawn in the edge columns when
// it is beyond them: before the first column after the flag column (which is within -0.5 periods)
// in that column, at or past +1.5 periods in the last column. *beyond says whether it was beyond
// the edges (and is not drawn where it is).
static int fsemu_graphrows_get_column(const fsemu_graphrows_row_t *row, double zero_ns,
                                      int64_t time_ns, bool *beyond)
{
    double column = FSEMU_GRAPHROWS_ZERO_COLUMN +
                    floor(((double)time_ns - zero_ns) * FSEMU_GRAPHROWS_COLUMNS_PER_PERIOD /
                          row->axis.interval_ns);
    *beyond = column < FSEMU_GRAPHROWS_FLAG_COLUMNS || column >= FSEMU_GRAPHROWS_WIDTH;
    if (column < FSEMU_GRAPHROWS_FLAG_COLUMNS) {
        return FSEMU_GRAPHROWS_FLAG_COLUMNS;
    }
    if (column >= FSEMU_GRAPHROWS_WIDTH) {
        return FSEMU_GRAPHROWS_WIDTH - 1;
    }
    return (int)column;
}

// ----------------------------------------------------------------------------
// The background stripe.

// Reduces the frame's spans to a category per column on the row's axis, by error diffusion along
// the real timeline: for each column (1/120 of a period) from the first after the flag column, each
// category's time within the column is added to its remainder (time outside the spans to NONE's),
// the category with the largest remainder is drawn and one column's worth is subtracted from its
// remainder. Each category's column count is then within one of its time, every transition is
// drawn within about a column of where it was, and runs much shorter than a column (line sync's
// 64 us) do not flicker but add up to their share of the columns.
static void fsemu_graphrows_reduce_stripe(fsemu_graphrows_row_t *row,
                                          const fsemu_stripe_frame_t *stripe)
{
    memset(row->stripe, FSEMU_STRIPE_CATEGORY_NONE, sizeof(row->stripe));
    double zero_ns;
    if (!fsemu_graphrows_get_zero_ns(row, &zero_ns)) {
        return;
    }
    const int first = FSEMU_GRAPHROWS_FLAG_COLUMNS;
    const int count = FSEMU_GRAPHROWS_WIDTH - first;
    const double column_ns = row->axis.interval_ns / FSEMU_GRAPHROWS_COLUMNS_PER_PERIOD;
    const double first_ns = zero_ns + (first - FSEMU_GRAPHROWS_ZERO_COLUMN) * column_ns;

    // Each category's time in each column, in columns.
    double times[FSEMU_GRAPHROWS_WIDTH][FSEMU_STRIPE_CATEGORY_COUNT];
    memset(times, 0, sizeof(times));
    for (int i = 0; i < stripe->span_count; i++) {
        const fsemu_stripe_span_t *span = &stripe->spans[i];
        fsemu_stripe_category_t category = span->category;
        if (category <= FSEMU_STRIPE_CATEGORY_NONE || category >= FSEMU_STRIPE_CATEGORY_COUNT) {
            continue;
        }
        double a = fmax(((double)span->start_ns - first_ns) / column_ns, 0.0);
        double b = fmin(((double)span->end_ns - first_ns) / column_ns, (double)count);
        for (int c = (int)floor(a); c < b; c++) {
            times[c][category] += fmin(b, c + 1.0) - fmax(a, (double)c);
        }
    }

    double remainders[FSEMU_STRIPE_CATEGORY_COUNT] = {};
    for (int c = 0; c < count; c++) {
        double covered = 0.0;
        for (int k = 1; k < FSEMU_STRIPE_CATEGORY_COUNT; k++) {
            covered += times[c][k];
        }
        times[c][FSEMU_STRIPE_CATEGORY_NONE] = fmax(1.0 - covered, 0.0);
        int best = 0;
        for (int k = 0; k < FSEMU_STRIPE_CATEGORY_COUNT; k++) {
            remainders[k] += times[c][k];
            if (remainders[k] > remainders[best]) {
                best = k;
            }
        }
        remainders[best] -= 1.0;
        row->stripe[first + c] = (uint8_t)best;
    }
}

// A frame is late (oversleep, extra CPU late) when its late time of that category adds up to this
// many refresh periods or more: a column, so that it shows in the stripe.
#define FSEMU_GRAPHROWS_LATE_PERIODS (1.0 / FSEMU_GRAPHROWS_COLUMNS_PER_PERIOD)

// Of each late span, only the time after this many nanoseconds (16 us) counts: with "fastest
// possible" CPU speed every block of extra CPU time ends a few us late (5 to 8 us, the median), and
// hundreds of such blocks in a frame must not make it late. Plain sleeps also wake tens of us late,
// which counts only when it adds up.
#define FSEMU_GRAPHROWS_LATE_NOISE_NS 16000

// Sets the row's frame's oversleep and extra CPU late from its spans: each category's late time
// summed over the frame, against a share of the row's refresh period (none without an axis).
static void fsemu_graphrows_set_late(fsemu_graphrows_row_t *row, const fsemu_stripe_frame_t *stripe)
{
    int64_t oversleep_ns = 0;
    int64_t extra_cpu_late_ns = 0;
    for (int i = 0; i < stripe->span_count; i++) {
        const fsemu_stripe_span_t *span = &stripe->spans[i];
        int64_t late_ns = span->end_ns - span->start_ns - FSEMU_GRAPHROWS_LATE_NOISE_NS;
        if (late_ns <= 0) {
            continue;
        }
        if (span->category == FSEMU_STRIPE_CATEGORY_OVERSLEEP) {
            oversleep_ns += late_ns;
        } else if (span->category == FSEMU_STRIPE_CATEGORY_EXTRA_CPU_LATE) {
            extra_cpu_late_ns += late_ns;
        }
    }
    bool has_axis = fsemu_graphrows_has_axis(row);
    double limit_ns = FSEMU_GRAPHROWS_LATE_PERIODS * row->axis.interval_ns;
    row->frame.oversleep = has_axis && (double)oversleep_ns >= limit_ns;
    row->frame.extra_cpu_late = has_axis && (double)extra_cpu_late_ns >= limit_ns;
}

// The row of the frame, -1 when it has none (not written yet, or gone). Rows are in frame number
// order, so the search goes back from the newest row.
static int fsemu_graphrows_find_row(const fsemu_graphrows_t *rows, int frame_number)
{
    int y = rows->newest_row;
    for (int i = 0; y >= 0 && i < FSEMU_GRAPHROWS_HEIGHT - 1; i++) {
        const fsemu_graphrows_row_t *row = &rows->rows[y];
        if (!row->written || row->frame.number < frame_number) {
            break;
        }
        if (row->frame.number == frame_number) {
            return y;
        }
        y = (y + FSEMU_GRAPHROWS_HEIGHT - 1) % FSEMU_GRAPHROWS_HEIGHT;
    }
    return -1;
}

void fsemu_graphrows_on_presented(fsemu_graphrows_t *rows, int64_t swap_number,
                                  int64_t presented_ns)
{
    if (swap_number <= 0) {
        return;
    }
    int y = rows->newest_row;
    for (int i = 0; y >= 0 && i < FSEMU_GRAPHROWS_PRESENTED_ROWS; i++) {
        fsemu_graphrows_row_t *row = &rows->rows[y];
        if (!row->written) {
            break;
        }
        for (int j = 0; j < row->swap_count; j++) {
            if (row->swaps[j].swap_number == swap_number) {
                row->swaps[j].presented_ns = presented_ns;
                fsemu_graphrows_damage_row(rows, y);
                return;
            }
        }
        y = (y + FSEMU_GRAPHROWS_HEIGHT - 1) % FSEMU_GRAPHROWS_HEIGHT;
    }
}

// Adds the display refresh to the row when it is between the row's edges (on its axis), not there
// already, and the row has room for it. Returns whether it was added.
static bool fsemu_graphrows_add_row_display_refresh(fsemu_graphrows_row_t *row, int64_t refresh_ns)
{
    double zero_ns;
    if (row->display_refresh_count == FSEMU_GRAPHROWS_ROW_DISPLAY_REFRESHES ||
        !fsemu_graphrows_get_zero_ns(row, &zero_ns)) {
        return false;
    }
    bool beyond;
    fsemu_graphrows_get_column(row, zero_ns, refresh_ns, &beyond);
    if (beyond) {
        return false;
    }
    for (int i = 0; i < row->display_refresh_count; i++) {
        if (row->display_refresh_ns[i] == refresh_ns) {
            return false;
        }
    }
    row->display_refresh_ns[row->display_refresh_count++] = refresh_ns;
    return true;
}

void fsemu_graphrows_on_display_refresh(fsemu_graphrows_t *rows, int64_t refresh_ns)
{
    if (refresh_ns == 0) {
        return;
    }
    rows->display_refresh_ns[rows->display_refresh_next] = refresh_ns;
    rows->display_refresh_next =
        (rows->display_refresh_next + 1) % FSEMU_GRAPHROWS_DISPLAY_REFRESHES;
    int y = rows->newest_row;
    for (int i = 0; y >= 0 && i < FSEMU_GRAPHROWS_DISPLAY_REFRESH_ROWS; i++) {
        fsemu_graphrows_row_t *row = &rows->rows[y];
        if (!row->written) {
            break;
        }
        if (fsemu_graphrows_add_row_display_refresh(row, refresh_ns)) {
            fsemu_graphrows_damage_row(rows, y);
        }
        y = (y + FSEMU_GRAPHROWS_HEIGHT - 1) % FSEMU_GRAPHROWS_HEIGHT;
    }
}

void fsemu_graphrows_on_frame_ended(fsemu_graphrows_t *rows, int frame_number,
                                    const fsemu_stripe_frame_t *stripe)
{
    int y = fsemu_graphrows_find_row(rows, frame_number);
    if (y < 0) {
        return;
    }
    fsemu_graphrows_row_t *row = &rows->rows[y];
    row->frame.emulation_done_ns = stripe->emulation_done_ns;
    fsemu_graphrows_set_late(row, stripe);
    fsemu_graphrows_reduce_stripe(row, stripe);
    fsemu_graphrows_damage_row(rows, y);
}

// ----------------------------------------------------------------------------
// Limits: past them, a mark turns red and the row is flagged.

// A swap returned this many refresh periods after the target vertical blank or later: it missed it.
// With vsync, a swap returns at a host vertical blank, so a swap this late returned at a later one
// (a whole refresh late).
#define FSEMU_GRAPHROWS_LATE_SWAP_PERIODS 0.5

// The emulation slot actually started this many refresh periods after the planned start or later.
#define FSEMU_GRAPHROWS_LATE_SLOT_START_PERIODS 0.1

// Whether the swap returned a whole refresh late for the row's target vertical blank (one period
// after zero_ns).
static bool fsemu_graphrows_is_swap_late(const fsemu_graphrows_row_t *row, double zero_ns,
                                         const fsemu_graphrows_swap_t *swap)
{
    if (swap->swap_ns == 0) {
        return false;
    }
    double interval = row->axis.interval_ns;
    double target_ns = zero_ns + interval;
    return (double)swap->swap_ns >= target_ns + FSEMU_GRAPHROWS_LATE_SWAP_PERIODS * interval;
}

// A swap that was on time was reported shown this many refresh periods after the target vertical
// blank or later: a late presentation. With vsync it is then shown at a later host vertical blank
// than the one FS-UAE took it to be shown at.
#define FSEMU_GRAPHROWS_LATE_PRESENTATION_PERIODS 0.5

// Whether the row's first swap was on time but reported shown a whole host vertical blank (or more)
// after the target vertical blank: latency that the host vertical blank (from the glFinish() after
// the swap) does not show, as when swaps are queued ahead of the display. A swap that was late
// itself is flagged as missed instead.
static bool fsemu_graphrows_is_presentation_late(const fsemu_graphrows_row_t *row)
{
    double zero_ns;
    if (row->swap_count < 1 || row->swaps[0].presented_ns == 0 ||
        !fsemu_graphrows_get_zero_ns(row, &zero_ns) ||
        fsemu_graphrows_is_swap_late(row, zero_ns, &row->swaps[0])) {
        return false;
    }
    double interval = row->axis.interval_ns;
    double target_ns = zero_ns + interval;
    return (double)row->swaps[0].presented_ns >=
           target_ns + FSEMU_GRAPHROWS_LATE_PRESENTATION_PERIODS * interval;
}

// Whether time_ns (frame ready, rendering finished) is too late for the row's target vertical blank
// (one period after zero_ns).
static bool fsemu_graphrows_is_too_late_for_target(const fsemu_graphrows_row_t *row, double zero_ns,
                                                   int64_t time_ns)
{
    return time_ns != 0 && (double)time_ns >= zero_ns + row->axis.interval_ns;
}

// Whether the frame's slot is a planned whole-frame slot: it has a swap deadline, and was planned
// to start before the host vertical blank one refresh before it (where the previous slot ended).
static bool fsemu_graphrows_is_whole_frame_slot(const fsemu_graphrows_row_t *row)
{
    const fsemu_graphrows_frame_t *frame = &row->frame;
    return fsemu_graphrows_has_axis(row) && frame->deadline_ns != 0 &&
           frame->planned_start_ns != 0 &&
           (double)frame->planned_start_ns < (double)frame->deadline_ns - row->axis.interval_ns;
}

// Whether the frame's emulation slot started noticeably later than its planned start. Not for a
// whole-frame slot: it starts after the emulation thread's work between frames, always a little
// after its planned start, and catches up; it is late when it ends late
// (fsemu_graphrows_is_slot_end_late).
static bool fsemu_graphrows_is_slot_start_late(const fsemu_graphrows_row_t *row)
{
    const fsemu_graphrows_frame_t *frame = &row->frame;
    if (!fsemu_graphrows_has_axis(row) || frame->slot_start_ns == 0 ||
        frame->planned_start_ns == 0 || fsemu_graphrows_is_whole_frame_slot(row)) {
        return false;
    }
    double late_ns = (double)(frame->slot_start_ns - frame->planned_start_ns);
    return late_ns >= FSEMU_GRAPHROWS_LATE_SLOT_START_PERIODS * row->axis.interval_ns;
}

// Whether a whole-frame slot's emulation was done noticeably later than its planned end (it did
// not catch up), by the same share of a period as a late slot start.
static bool fsemu_graphrows_is_slot_end_late(const fsemu_graphrows_row_t *row)
{
    const fsemu_graphrows_frame_t *frame = &row->frame;
    if (!fsemu_graphrows_is_whole_frame_slot(row) || frame->emulation_done_ns == 0 ||
        frame->planned_end_ns == 0) {
        return false;
    }
    double late_ns = (double)(frame->emulation_done_ns - frame->planned_end_ns);
    return late_ns >= FSEMU_GRAPHROWS_LATE_SLOT_START_PERIODS * row->axis.interval_ns;
}

// Whether the frame's emulation slot started before the event pump ran: before the first events
// phase of the main loop after the host vertical blank before its swap deadline, so that input
// pumped after the swap before returned came too late for its first lines (its vertical blank
// interrupt). Only for a slot planned to start after that host vertical blank (not a whole-frame
// slot, which starts before it by design: its event pump mark still shows how late the input
// comes). Never for a frame without a deadline, or whose event pump was not known when its row was
// written.
static bool fsemu_graphrows_is_slot_start_before_event_pump(const fsemu_graphrows_row_t *row)
{
    const fsemu_graphrows_frame_t *frame = &row->frame;
    if (row->event_pump_ns == 0 || frame->slot_start_ns == 0 || frame->planned_start_ns == 0 ||
        !fsemu_graphrows_has_axis(row) || fsemu_graphrows_is_whole_frame_slot(row)) {
        return false;
    }
    return frame->slot_start_ns < row->event_pump_ns;
}

// Draws a mark at time_ns (nothing when it is not known), in red when it is late (past its limit)
// or beyond the edges, and returns its column (-1 for none).
static int fsemu_graphrows_draw_mark(fsemu_graphrows_drawing_t *d, double zero_ns, int64_t time_ns,
                                     fsemu_graphrows_color_t color, bool late)
{
    if (time_ns == 0) {
        return -1;
    }
    bool beyond;
    int x = fsemu_graphrows_get_column(d->row, zero_ns, time_ns, &beyond);
    fsemu_graphrows_put(d, x, late || beyond ? d->palette->warnings.past_limit : color);
    return x;
}

// Draws a swap's marks: the swap's return (red when late), the host vertical blank where it is in
// another column, and on top of them the presentation, when the windowing system reported it. The
// presentation has no limit (it is not a problem in itself): beyond an edge it is drawn in the
// edge column in its own colour, not red.
static void fsemu_graphrows_draw_swap_marks(fsemu_graphrows_drawing_t *d, double zero_ns,
                                            const fsemu_graphrows_swap_t *swap, bool late)
{
    bool beyond;
    int swap_x =
        swap->swap_ns ? fsemu_graphrows_get_column(d->row, zero_ns, swap->swap_ns, &beyond) : -1;
    if (swap->host_vertical_blank_ns != 0) {
        int x = fsemu_graphrows_get_column(d->row, zero_ns, swap->host_vertical_blank_ns, &beyond);
        if (x != swap_x) {
            fsemu_graphrows_put(
                d, x, beyond ? d->palette->warnings.past_limit : d->palette->host_vertical_blank);
        }
    }
    fsemu_graphrows_draw_mark(d, zero_ns, swap->swap_ns, d->palette->swap, late);
    if (swap->presented_ns != 0) {
        int x = fsemu_graphrows_get_column(d->row, zero_ns, swap->presented_ns, &beyond);
        fsemu_graphrows_put(d, x, d->palette->presented);
    }
}

static void fsemu_graphrows_draw_background(fsemu_graphrows_drawing_t *d)
{
    fsemu_graphrows_fill(d, d->palette->background);
}

// The background stripe, from the row's category per column (also in a warped frame's row).
static void fsemu_graphrows_draw_stripe(fsemu_graphrows_drawing_t *d)
{
    for (int x = FSEMU_GRAPHROWS_FLAG_COLUMNS; x < FSEMU_GRAPHROWS_WIDTH; x++) {
        fsemu_stripe_category_t category = (fsemu_stripe_category_t)d->row->stripe[x];
        if (category != FSEMU_STRIPE_CATEGORY_NONE) {
            fsemu_graphrows_put(d, x, d->palette->stripe[category]);
        }
    }
}

// Whether the drawing side's spans are drawn over the stripe's category: over the emulation
// thread's waits and time outside its records, not over its work or its late time.
static bool fsemu_graphrows_is_under_drawing_side(fsemu_stripe_category_t category)
{
    switch (category) {
    case FSEMU_STRIPE_CATEGORY_NONE:
    case FSEMU_STRIPE_CATEGORY_SLEEPING:
    case FSEMU_STRIPE_CATEGORY_BUSY_WAITING:
    case FSEMU_STRIPE_CATEGORY_BEFORE_SLOT:
    case FSEMU_STRIPE_CATEGORY_END_WAIT:
        return true;
    default:
        return false;
    }
}

// A drawing side's span from start_ns to end_ns (nothing when either is not known, or it is empty),
// in the columns from start_ns's to the one before end_ns's (at least start_ns's column), where the
// stripe lets it show. Columns beyond the edges are left out.
static void fsemu_graphrows_draw_drawing_span(fsemu_graphrows_drawing_t *d, double zero_ns,
                                              int64_t start_ns, int64_t end_ns,
                                              fsemu_graphrows_color_t color)
{
    if (start_ns == 0 || end_ns == 0 || end_ns <= start_ns) {
        return;
    }
    bool start_beyond;
    bool end_beyond;
    int start_x = fsemu_graphrows_get_column(d->row, zero_ns, start_ns, &start_beyond);
    int end_x = fsemu_graphrows_get_column(d->row, zero_ns, end_ns, &end_beyond);
    if ((start_beyond && start_x == FSEMU_GRAPHROWS_WIDTH - 1) ||
        (end_beyond && end_x == FSEMU_GRAPHROWS_FLAG_COLUMNS)) {
        return; // Wholly past the right edge, or before the left one.
    }
    if (end_beyond && end_x == FSEMU_GRAPHROWS_WIDTH - 1) {
        end_x = FSEMU_GRAPHROWS_WIDTH;
    }
    for (int x = start_x; x < end_x || x == start_x; x++) {
        if (fsemu_graphrows_is_under_drawing_side((fsemu_stripe_category_t)d->row->stripe[x])) {
            fsemu_graphrows_put(d, x, color);
        }
    }
}

// The drawing side's spans of the row's frame: the hand-over (from its last slice posted to frame
// ready), and for each swap drawing it, the rendering wait and the swap wait, each up to the next
// (the marks at their ends are drawn over them).
static void fsemu_graphrows_draw_drawing_side(fsemu_graphrows_drawing_t *d)
{
    const fsemu_graphrows_row_t *row = d->row;
    double zero_ns;
    if (!d->palette->drawing_side || row->frame.warp ||
        !fsemu_graphrows_get_zero_ns(row, &zero_ns)) {
        return;
    }
    fsemu_graphrows_draw_drawing_span(d, zero_ns, row->frame.last_slice_posted_ns,
                                      row->frame.frame_ready_ns, d->palette->hand_over);
    for (int i = 0; i < row->swap_count; i++) {
        const fsemu_graphrows_swap_t *swap = &row->swaps[i];
        fsemu_graphrows_draw_drawing_span(d, zero_ns, swap->drawing_paced_ns,
                                          swap->rendering_wait_ns, d->palette->drawing);
        fsemu_graphrows_draw_drawing_span(d, zero_ns, swap->rendering_wait_ns,
                                          swap->rendering_finished_ns, d->palette->rendering_wait);
        fsemu_graphrows_draw_drawing_span(d, zero_ns, swap->swap_wait_ns, swap->swap_ns,
                                          d->palette->swap_wait);
    }
}

// The column of the frame's swap deadline, false when it has none (or it is beyond the edges).
static bool fsemu_graphrows_get_deadline_column(const fsemu_graphrows_row_t *row, double zero_ns,
                                                int *x)
{
    if (row->frame.deadline_ns == 0) {
        return false;
    }
    bool beyond;
    *x = fsemu_graphrows_get_column(row, zero_ns, row->frame.deadline_ns, &beyond);
    return !beyond;
}

// The slot margin's faint span, in the columns from the slot's planned end (the margin before the
// swap deadline) to the one before the deadline's. Drawn under the background stripe, which
// covers it where the frame's work ran into the margin.
static void fsemu_graphrows_draw_slot_margin(fsemu_graphrows_drawing_t *d)
{
    const fsemu_graphrows_row_t *row = d->row;
    double zero_ns;
    int deadline_x;
    if (!fsemu_graphrows_get_zero_ns(row, &zero_ns) ||
        !fsemu_graphrows_get_deadline_column(row, zero_ns, &deadline_x) ||
        row->frame.slot_margin_ns <= 0) {
        return;
    }
    bool beyond;
    int x = fsemu_graphrows_get_column(row, zero_ns,
                                       row->frame.deadline_ns - row->frame.slot_margin_ns, &beyond);
    for (; x < deadline_x; x++) {
        fsemu_graphrows_put(d, x, d->palette->slot_margin);
    }
}

// The faint guides at x = 0 and the target vertical blank, and the frame's swap deadline (the
// target vertical blank while the emulation follows the host vertical blank).
static void fsemu_graphrows_draw_guides(fsemu_graphrows_drawing_t *d)
{
    if (!fsemu_graphrows_has_axis(d->row)) {
        return;
    }
    fsemu_graphrows_put(d, FSEMU_GRAPHROWS_ZERO_COLUMN, d->palette->guide);
    fsemu_graphrows_put(d, FSEMU_GRAPHROWS_TARGET_COLUMN, d->palette->guide);
    double zero_ns;
    int x;
    if (fsemu_graphrows_get_zero_ns(d->row, &zero_ns) &&
        fsemu_graphrows_get_deadline_column(d->row, zero_ns, &x)) {
        fsemu_graphrows_put(d, x, d->palette->deadline);
    }
}

// The display's refreshes, under the marks, like the guides (not on a warped row, which has no
// marks).
static void fsemu_graphrows_draw_display_refreshes(fsemu_graphrows_drawing_t *d)
{
    double zero_ns;
    if (d->row->frame.warp || !fsemu_graphrows_get_zero_ns(d->row, &zero_ns)) {
        return;
    }
    for (int i = 0; i < d->row->display_refresh_count; i++) {
        bool beyond;
        int x = fsemu_graphrows_get_column(d->row, zero_ns, d->row->display_refresh_ns[i], &beyond);
        fsemu_graphrows_put(d, x, d->palette->display_refresh);
    }
}

// The marks, later ones over earlier ones in the same column: the planned start under the event
// pump, and both under the slot start (the planned start shows when the overshoot is at least a
// column), the swaps last (each swap's presentation over its other marks).
static void fsemu_graphrows_draw_marks(fsemu_graphrows_drawing_t *d)
{
    const fsemu_graphrows_row_t *row = d->row;
    double zero_ns;
    if (row->frame.warp || !fsemu_graphrows_get_zero_ns(row, &zero_ns)) {
        return;
    }
    const fsemu_graphrows_palette_t *palette = d->palette;
    const fsemu_graphrows_frame_t *frame = &row->frame;
    fsemu_graphrows_draw_mark(d, zero_ns, frame->planned_start_ns, palette->planned_start, false);
    fsemu_graphrows_draw_mark(d, zero_ns, row->event_pump_ns, palette->event_pump, false);
    fsemu_graphrows_draw_mark(d, zero_ns, frame->slot_start_ns, palette->slot_start,
                              fsemu_graphrows_is_slot_start_late(row));
    fsemu_graphrows_draw_mark(d, zero_ns, frame->emulation_done_ns, palette->emulation_done,
                              fsemu_graphrows_is_slot_end_late(row));
    fsemu_graphrows_draw_mark(
        d, zero_ns, frame->frame_ready_ns, palette->frame_ready,
        fsemu_graphrows_is_too_late_for_target(row, zero_ns, frame->frame_ready_ns));
    if (row->swap_count >= 1) {
        int64_t rendering_finished_ns = row->swaps[0].rendering_finished_ns;
        fsemu_graphrows_draw_mark(
            d, zero_ns, rendering_finished_ns, palette->rendering_finished,
            fsemu_graphrows_is_too_late_for_target(row, zero_ns, rendering_finished_ns));
    }
    // A repeat's swap is a refresh after the first by its nature: only the first has a limit.
    for (int i = 0; i < row->swap_count; i++) {
        bool late = i == 0 && fsemu_graphrows_is_swap_late(row, zero_ns, &row->swaps[i]);
        fsemu_graphrows_draw_swap_marks(d, zero_ns, &row->swaps[i], late);
    }
}

// The flag column's layout: the columns of each class (from the left edge), so that a row shows all
// of its problems at once. The fate takes two columns, since a stutter is what is looked for first.
#define FSEMU_GRAPHROWS_FATE_FLAG_COLUMN 0
#define FSEMU_GRAPHROWS_FATE_FLAG_WIDTH 2
#define FSEMU_GRAPHROWS_LATE_SLOT_START_FLAG_COLUMN 2
#define FSEMU_GRAPHROWS_EVENT_PUMP_FLAG_COLUMN 3
#define FSEMU_GRAPHROWS_OVERSLEEP_FLAG_COLUMN 4
#define FSEMU_GRAPHROWS_EXTRA_CPU_LATE_FLAG_COLUMN 5
#define FSEMU_GRAPHROWS_LATE_PRESENTATION_FLAG_COLUMN 6
#define FSEMU_GRAPHROWS_CONTEXT_FLAG_COLUMN 7

// Fills width columns of the flag column from x with a class's colour.
static void fsemu_graphrows_draw_flag(fsemu_graphrows_drawing_t *d, int x, int width,
                                      fsemu_graphrows_color_t color)
{
    for (int i = 0; i < width; i++) {
        fsemu_graphrows_put(d, x + i, color);
    }
}

// The flag column's fate class: a missed vertical blank or a repeated frame, or a dropped frame
// (NULL for neither).
static const fsemu_graphrows_color_t *
fsemu_graphrows_get_fate_flag(const fsemu_graphrows_drawing_t *d)
{
    const fsemu_graphrows_row_t *row = d->row;
    double zero_ns;
    bool has_zero = fsemu_graphrows_get_zero_ns(row, &zero_ns);
    if ((row->flags & FSEMU_GRAPHROWS_FLAG_REPEATED) ||
        (has_zero && row->swap_count >= 1 &&
         fsemu_graphrows_is_swap_late(row, zero_ns, &row->swaps[0]))) {
        return &d->palette->warnings.flag_missed;
    }
    if (row->flags & FSEMU_GRAPHROWS_FLAG_DROPPED) {
        return &d->palette->warnings.flag_dropped;
    }
    return NULL;
}

// The flag column's context class: what the row's timing must be read with, or NULL for nothing.
static const fsemu_graphrows_color_t *
fsemu_graphrows_get_context_flag(const fsemu_graphrows_drawing_t *d)
{
    const fsemu_graphrows_row_t *row = d->row;
    if (row->flags & FSEMU_GRAPHROWS_FLAG_WARP) {
        return &d->palette->warnings.flag_warp;
    }
    const unsigned int timeline =
        FSEMU_GRAPHROWS_FLAG_AFTER_PAUSE | FSEMU_GRAPHROWS_FLAG_AFTER_SESSION_START |
        FSEMU_GRAPHROWS_FLAG_AFTER_TIMELINE_RESET | FSEMU_GRAPHROWS_FLAG_SETTLING;
    if (row->flags & timeline) {
        return &d->palette->warnings.flag_timeline;
    }
    return NULL;
}

// The flag column: each of the row's problems (its fate, a late slot start, a slot started before
// the event pump, an oversleep, extra CPU late, a late presentation) in its class's columns, and
// the context its timing must be read with in the last column. A warped row shows only warp: its
// frames are not paced, so their timing is no problem.
static void fsemu_graphrows_draw_flags(fsemu_graphrows_drawing_t *d)
{
    const fsemu_graphrows_row_t *row = d->row;
    const fsemu_graphrows_warning_colors_t *warnings = &d->palette->warnings;
    const fsemu_graphrows_color_t *context = fsemu_graphrows_get_context_flag(d);
    if (context) {
        fsemu_graphrows_draw_flag(d, FSEMU_GRAPHROWS_CONTEXT_FLAG_COLUMN, 1, *context);
    }
    if (row->flags & FSEMU_GRAPHROWS_FLAG_WARP) {
        return;
    }
    const fsemu_graphrows_color_t *fate = fsemu_graphrows_get_fate_flag(d);
    if (fate) {
        fsemu_graphrows_draw_flag(d, FSEMU_GRAPHROWS_FATE_FLAG_COLUMN,
                                  FSEMU_GRAPHROWS_FATE_FLAG_WIDTH, *fate);
    }
    if (fsemu_graphrows_is_slot_start_late(row) || fsemu_graphrows_is_slot_end_late(row)) {
        fsemu_graphrows_draw_flag(d, FSEMU_GRAPHROWS_LATE_SLOT_START_FLAG_COLUMN, 1,
                                  warnings->flag_late_slot_start);
    }
    if (fsemu_graphrows_is_slot_start_before_event_pump(row)) {
        fsemu_graphrows_draw_flag(d, FSEMU_GRAPHROWS_EVENT_PUMP_FLAG_COLUMN, 1,
                                  warnings->flag_event_pump);
    }
    if (row->frame.oversleep) {
        fsemu_graphrows_draw_flag(d, FSEMU_GRAPHROWS_OVERSLEEP_FLAG_COLUMN, 1,
                                  warnings->flag_oversleep);
    }
    if (row->frame.extra_cpu_late) {
        fsemu_graphrows_draw_flag(d, FSEMU_GRAPHROWS_EXTRA_CPU_LATE_FLAG_COLUMN, 1,
                                  warnings->flag_extra_cpu_late);
    }
    if (fsemu_graphrows_is_presentation_late(row)) {
        fsemu_graphrows_draw_flag(d, FSEMU_GRAPHROWS_LATE_PRESENTATION_FLAG_COLUMN, 1,
                                  warnings->flag_late_presentation);
    }
}

static void fsemu_graphrows_draw_row(fsemu_graphrows_t *rows, int y)
{
    fsemu_graphrows_drawing_t d;
    d.pixels = rows->pixels + y * rows->stride;
    d.row = &rows->rows[y];
    d.palette = rows->palette;
    if (!d.row->written) {
        bool cursor = rows->newest_row >= 0 && y == (rows->newest_row + 1) % FSEMU_GRAPHROWS_HEIGHT;
        fsemu_graphrows_fill(&d, cursor ? d.palette->cursor : fsemu_graphrows_transparent);
        return;
    }
    fsemu_graphrows_draw_background(&d);
    fsemu_graphrows_draw_slot_margin(&d);
    fsemu_graphrows_draw_stripe(&d);
    fsemu_graphrows_draw_drawing_side(&d);
    fsemu_graphrows_draw_guides(&d);
    fsemu_graphrows_draw_display_refreshes(&d);
    fsemu_graphrows_draw_marks(&d);
    fsemu_graphrows_draw_flags(&d);
}

bool fsemu_graphrows_draw(fsemu_graphrows_t *rows, int *damage_y, int *damage_height)
{
    if (rows->damage_height == 0) {
        *damage_y = 0;
        *damage_height = 0;
        return false;
    }
    for (int i = 0; i < rows->damage_height; i++) {
        fsemu_graphrows_draw_row(rows, (rows->damage_y + i) % FSEMU_GRAPHROWS_HEIGHT);
    }
    *damage_y = rows->damage_y;
    *damage_height = rows->damage_height;
    rows->damage_y = 0;
    rows->damage_height = 0;
    return true;
}

// ----------------------------------------------------------------------------

int fsemu_graphrows_get_row_frame(const fsemu_graphrows_t *rows, int y)
{
    if (y < 0 || y >= FSEMU_GRAPHROWS_HEIGHT || !rows->rows[y].written) {
        return -1;
    }
    return rows->rows[y].frame.number;
}

int fsemu_graphrows_get_newest_row(const fsemu_graphrows_t *rows)
{
    return rows->newest_row;
}
