/*
 * The performance graph's audio strip: each emulated frame's audio as one row of the graph's right
 * half, next to the frame's video row (fsemu-graphrows.h), and the strip's header. The terms are
 * those of GLOSSARY.md (audio buffer level, audio ring buffer, audio in flight, target, rate
 * feed-forward, drift correction, audio underrun, audio overrun).
 *
 * A row, from the outer (right) edge of the image inwards:
 *
 * - The flag column (FSEMU_AUDIOSTRIP_FLAG_COLUMNS): an audio underrun in the frame in the
 *   outermost columns, an audio overrun in the ones inside them, each class in columns of its own,
 *   like the video part's flag column at the image's other edge. Not in the video part's flag
 *   column: the two halves are separate adornments, on either side of the picture in the
 *   sidebars, and the video part's flag column has no free columns.
 * - The levels, on a fixed scale from 0 (at FSEMU_AUDIOSTRIP_LEVEL_ZERO_COLUMN) to half the
 *   overrun ceiling (twice the target, FSEMU_AUDIOSTRIP_LEVEL_COLUMNS further in), so that the
 *   scale never jumps: from 0 outwards in, what was newly written, the audio ring buffer's part and
 *   the audio in flight, filled, with marks at the audio buffer level (their sum), the minimum the
 *   audio callback saw in the audio ring buffer, the average and the target. A level at or past the
 *   scale's end is drawn in the last column (and an audio overrun is flagged).
 * - The drift correction band (FSEMU_AUDIOSTRIP_DRIFT_COLUMNS): a centre line and a mark at the
 *   drift correction, the band's full width being the drift correction's limit both ways (±0.1 %),
 *   positive to the right. The mark is dim while the controller is frozen.
 *
 * The header is a box of background in the strip's first FSEMU_AUDIOSTRIP_HEADER_HEIGHT image rows,
 * just wide enough for its text, over those rows' frames' audio. It shows the rate feed-forward as
 * a number in ppm, which changes only when the timeline's rate does.
 *
 * Pure drawing with no dependency on the GPU, SDL or other modules, so that it can be tested alone
 * (od-fs/tests/fsemu). The performance graph (fsemu-perfgui.cpp) gives it each frame's audio
 * statistics.
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef FSEMU_AUDIOSTRIP_H
#define FSEMU_AUDIOSTRIP_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// The strip's columns (the graph image's right half).
#define FSEMU_AUDIOSTRIP_WIDTH 240

// The flag column at the right edge: an audio underrun in the last two columns, an audio overrun
// in the two before them.
#define FSEMU_AUDIOSTRIP_FLAG_COLUMNS 4
#define FSEMU_AUDIOSTRIP_UNDERRUN_COLUMN (FSEMU_AUDIOSTRIP_WIDTH - 2)
#define FSEMU_AUDIOSTRIP_OVERRUN_COLUMN (FSEMU_AUDIOSTRIP_WIDTH - 4)

// The levels: 0 at the zero column (inside the flag column and a gap), the scale's end (half the
// overrun ceiling) FSEMU_AUDIOSTRIP_LEVEL_COLUMNS columns to the left of it (a level at or past it
// is drawn in the last column, FSEMU_AUDIOSTRIP_LEVEL_COLUMNS - 1 to the left).
#define FSEMU_AUDIOSTRIP_LEVEL_ZERO_COLUMN                                                         \
    (FSEMU_AUDIOSTRIP_WIDTH - FSEMU_AUDIOSTRIP_FLAG_COLUMNS - 2)
#define FSEMU_AUDIOSTRIP_LEVEL_COLUMNS 200
#define FSEMU_AUDIOSTRIP_LEVEL_SCALE_DIVISOR 2

// The drift correction band, left of the levels and a gap: its centre column, and the columns each
// side of it (the band's full width being the drift correction's limit, 0.1 %, so 100 ppm per
// column).
#define FSEMU_AUDIOSTRIP_DRIFT_HALF_COLUMNS 10
#define FSEMU_AUDIOSTRIP_DRIFT_COLUMNS (2 * FSEMU_AUDIOSTRIP_DRIFT_HALF_COLUMNS + 1)
#define FSEMU_AUDIOSTRIP_DRIFT_CENTRE_COLUMN                                                       \
    (FSEMU_AUDIOSTRIP_LEVEL_ZERO_COLUMN - FSEMU_AUDIOSTRIP_LEVEL_COLUMNS -                         \
     FSEMU_AUDIOSTRIP_DRIFT_HALF_COLUMNS - 1)
#define FSEMU_AUDIOSTRIP_DRIFT_LIMIT_PPB 1000000

// The header's image rows at the top of the strip.
#define FSEMU_AUDIOSTRIP_HEADER_HEIGHT 7

// A colour as it is written to the image: bytes R, G, B and A, premultiplied.
typedef struct fsemu_audiostrip_color_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} fsemu_audiostrip_color_t;

// The colours of one mode (detailed or sidebars).
typedef struct fsemu_audiostrip_palette_t {
    fsemu_audiostrip_color_t background;
    // The levels' parts, filled.
    fsemu_audiostrip_color_t newly_written;
    fsemu_audiostrip_color_t ring_buffer;
    fsemu_audiostrip_color_t in_flight;
    // The marks: the audio buffer level, the minimum seen by the callback, the average, the target.
    fsemu_audiostrip_color_t level;
    fsemu_audiostrip_color_t minimum;
    fsemu_audiostrip_color_t average;
    fsemu_audiostrip_color_t target;
    // The drift correction band's background and centre line, and its mark (and the mark while the
    // controller is frozen).
    fsemu_audiostrip_color_t drift_band;
    fsemu_audiostrip_color_t drift_centre;
    fsemu_audiostrip_color_t drift_correction;
    fsemu_audiostrip_color_t drift_correction_frozen;
    // The header's text.
    fsemu_audiostrip_color_t header_text;
    // The flag column (the same in both modes).
    fsemu_audiostrip_color_t flag_underrun;
    fsemu_audiostrip_color_t flag_overrun;
} fsemu_audiostrip_palette_t;

// The detailed graph's colours, and the sidebars' greys.
extern const fsemu_audiostrip_palette_t fsemu_audiostrip_detailed_palette;
extern const fsemu_audiostrip_palette_t fsemu_audiostrip_sidebars_palette;

// An emulated frame's audio, measured at its end. Levels in nanoseconds.
typedef struct fsemu_audiostrip_frame_t {
    // The audio buffer level's parts: what was newly written (including what the emulator has not
    // written to the audio ring buffer yet), the audio ring buffer's part, and the audio in flight
    // (below 0 when the audio device ran dry).
    int64_t newly_written_ns;
    int64_t ring_buffer_ns;
    int64_t in_flight_ns;
    // The least the audio callback left in the audio ring buffer during the frame.
    int64_t minimum_ns;
    // The audio buffer level's average over recent frames.
    int64_t average_ns;
    // The target and the overrun ceiling (the scale ends at half of it). No levels are drawn
    // without a target.
    int64_t target_ns;
    int64_t ceiling_ns;
    // The drift correction (ppb), and whether the controller was frozen.
    int drift_correction_ppb;
    bool frozen;
    // The audio underruns and audio overruns in the frame.
    int underruns;
    int overruns;
} fsemu_audiostrip_frame_t;

// Draws the frame's row into pixels (FSEMU_AUDIOSTRIP_WIDTH pixels of 4 bytes).
void fsemu_audiostrip_draw_row(const fsemu_audiostrip_palette_t *palette,
                               const fsemu_audiostrip_frame_t *frame, uint8_t *pixels);

// The rate feed-forward as the header shows it, in tenths of a ppm rounded to the nearest (halves
// away from 0): the header changes when this does.
int64_t fsemu_audiostrip_get_rate_feed_forward_tenths(int rate_feed_forward_ppb);

// The rate feed-forward as the header shows it, in ppm with one decimal and a sign ("+5.6",
// "-9895.5", "0.0"), into text (at least 16 bytes).
void fsemu_audiostrip_format_rate_feed_forward(int rate_feed_forward_ppb, char *text, int size);

// Draws the header into pixels (FSEMU_AUDIOSTRIP_HEADER_HEIGHT rows of stride bytes, each starting
// with the strip's FSEMU_AUDIOSTRIP_WIDTH pixels): the rate feed-forward ("FF", the number and
// "PPM") on a box of background with a column and a row around the text, leaving the rest of the
// rows as they are (the rows' audio). A shorter text leaves part of a longer one's box: draw the
// rows again first.
void fsemu_audiostrip_draw_header(const fsemu_audiostrip_palette_t *palette,
                                  int rate_feed_forward_ppb, uint8_t *pixels, int stride);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSEMU_AUDIOSTRIP_H */
