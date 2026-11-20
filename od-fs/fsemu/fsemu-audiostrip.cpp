/*
 * The performance graph's audio strip (see fsemu-audiostrip.h).
 *
 * A row is drawn as a line of colours, one per column, each step over the one before: the
 * background, the levels and their marks, the drift correction band and the flag column.
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "fsemu-audiostrip.h"

#include <stdio.h>
#include <string.h>

// ----------------------------------------------------------------------------
// The palettes.

// A colour from 0xRRGGBB and an alpha, premultiplied.
#define FSEMU_AUDIOSTRIP_COLOR(rgb, alpha)                                                         \
    {(uint8_t)((((rgb) >> 16) & 0xff) * (alpha) / 255),                                            \
     (uint8_t)((((rgb) >> 8) & 0xff) * (alpha) / 255), (uint8_t)(((rgb) & 0xff) * (alpha) / 255),  \
     (uint8_t)(alpha)}

// The sidebars' greys: white with the alpha as the grey level (as for the video part's rows, the
// sidebars are opaque adornments, so the premultiplied white shows as the grey).
#define FSEMU_AUDIOSTRIP_GREY(level) FSEMU_AUDIOSTRIP_COLOR(0xffffff, level)

const fsemu_audiostrip_palette_t fsemu_audiostrip_detailed_palette = {
    FSEMU_AUDIOSTRIP_COLOR(0x0c0c0c, 0xff), // background
    FSEMU_AUDIOSTRIP_COLOR(0x4a5868, 0xff), // newly_written
    FSEMU_AUDIOSTRIP_COLOR(0x343e4a, 0xff), // ring_buffer
    FSEMU_AUDIOSTRIP_COLOR(0x22282e, 0xff), // in_flight
    FSEMU_AUDIOSTRIP_COLOR(0xcccccc, 0xff), // level
    FSEMU_AUDIOSTRIP_COLOR(0x0099ff, 0xff), // minimum
    FSEMU_AUDIOSTRIP_COLOR(0x00ff00, 0xff), // average
    FSEMU_AUDIOSTRIP_COLOR(0xee8800, 0xff), // target
    FSEMU_AUDIOSTRIP_COLOR(0x161616, 0xff), // drift_band
    FSEMU_AUDIOSTRIP_COLOR(0x444444, 0xff), // drift_centre
    FSEMU_AUDIOSTRIP_COLOR(0xffff00, 0xff), // drift_correction
    FSEMU_AUDIOSTRIP_COLOR(0x777700, 0xff), // drift_correction_frozen
    FSEMU_AUDIOSTRIP_COLOR(0xaaaaaa, 0xff), // header_text
    FSEMU_AUDIOSTRIP_COLOR(0xff0000, 0xff), // flag_underrun
    FSEMU_AUDIOSTRIP_COLOR(0xff00ff, 0xff), // flag_overrun
};

const fsemu_audiostrip_palette_t fsemu_audiostrip_sidebars_palette = {
    FSEMU_AUDIOSTRIP_GREY(0x0c),            // background
    FSEMU_AUDIOSTRIP_GREY(0x20),            // newly_written
    FSEMU_AUDIOSTRIP_GREY(0x18),            // ring_buffer
    FSEMU_AUDIOSTRIP_GREY(0x12),            // in_flight
    FSEMU_AUDIOSTRIP_GREY(0x40),            // level
    FSEMU_AUDIOSTRIP_GREY(0x30),            // minimum
    FSEMU_AUDIOSTRIP_GREY(0x38),            // average
    FSEMU_AUDIOSTRIP_GREY(0x28),            // target
    FSEMU_AUDIOSTRIP_GREY(0x10),            // drift_band
    FSEMU_AUDIOSTRIP_GREY(0x1c),            // drift_centre
    FSEMU_AUDIOSTRIP_GREY(0x48),            // drift_correction
    FSEMU_AUDIOSTRIP_GREY(0x24),            // drift_correction_frozen
    FSEMU_AUDIOSTRIP_GREY(0x40),            // header_text
    FSEMU_AUDIOSTRIP_COLOR(0xff0000, 0xff), // flag_underrun
    FSEMU_AUDIOSTRIP_COLOR(0xff00ff, 0xff), // flag_overrun
};

// ----------------------------------------------------------------------------
// The row.

// How many columns from 0 the level is on the scale (its end, scale_ns, at
// FSEMU_AUDIOSTRIP_LEVEL_COLUMNS), negative below 0 and clamped to the last column.
static int fsemu_audiostrip_level_distance(int64_t level_ns, int64_t scale_ns)
{
    int64_t d = level_ns * FSEMU_AUDIOSTRIP_LEVEL_COLUMNS / scale_ns;
    if (level_ns < 0) {
        // Rounded towards minus infinity, so that -1 ns is not drawn at 0.
        d = (level_ns * FSEMU_AUDIOSTRIP_LEVEL_COLUMNS - scale_ns + 1) / scale_ns;
    }
    if (d > FSEMU_AUDIOSTRIP_LEVEL_COLUMNS - 1) {
        d = FSEMU_AUDIOSTRIP_LEVEL_COLUMNS - 1;
    }
    return (int)d;
}

static void fsemu_audiostrip_draw_levels(const fsemu_audiostrip_palette_t *palette,
                                         const fsemu_audiostrip_frame_t *frame,
                                         fsemu_audiostrip_color_t *line)
{
    int64_t scale_ns = frame->ceiling_ns / FSEMU_AUDIOSTRIP_LEVEL_SCALE_DIVISOR;
    if (frame->target_ns <= 0 || scale_ns <= 0) {
        return;
    }
    int64_t newly_written_ns = frame->newly_written_ns;
    int64_t ring_buffer_ns = newly_written_ns + frame->ring_buffer_ns;
    int64_t level_ns = ring_buffer_ns + frame->in_flight_ns;
    int newly_written = fsemu_audiostrip_level_distance(newly_written_ns, scale_ns);
    int ring_buffer = fsemu_audiostrip_level_distance(ring_buffer_ns, scale_ns);
    int level = fsemu_audiostrip_level_distance(level_ns, scale_ns);
    for (int d = 0; d < FSEMU_AUDIOSTRIP_LEVEL_COLUMNS; d++) {
        fsemu_audiostrip_color_t *color = &line[FSEMU_AUDIOSTRIP_LEVEL_ZERO_COLUMN - d];
        if (d < newly_written) {
            *color = palette->newly_written;
        } else if (d < ring_buffer) {
            *color = palette->ring_buffer;
        } else if (d < level) {
            *color = palette->in_flight;
        }
    }
    // The marks, each over the ones before: the level, the minimum, the target, the average.
    struct {
        int64_t level_ns;
        const fsemu_audiostrip_color_t *color;
    } marks[] = {
        {level_ns, &palette->level},
        {frame->minimum_ns, &palette->minimum},
        {frame->target_ns, &palette->target},
        {frame->average_ns, &palette->average},
    };
    for (size_t i = 0; i < sizeof(marks) / sizeof(marks[0]); i++) {
        int d = fsemu_audiostrip_level_distance(marks[i].level_ns, scale_ns);
        if (d >= 0) {
            line[FSEMU_AUDIOSTRIP_LEVEL_ZERO_COLUMN - d] = *marks[i].color;
        }
    }
}

static void fsemu_audiostrip_draw_drift_correction(const fsemu_audiostrip_palette_t *palette,
                                                   const fsemu_audiostrip_frame_t *frame,
                                                   fsemu_audiostrip_color_t *line)
{
    const int centre = FSEMU_AUDIOSTRIP_DRIFT_CENTRE_COLUMN;
    const int half = FSEMU_AUDIOSTRIP_DRIFT_HALF_COLUMNS;
    for (int x = centre - half; x <= centre + half; x++) {
        line[x] = palette->drift_band;
    }
    line[centre] = palette->drift_centre;
    // To the nearest column, clamped to the band's edges.
    const int64_t ppb_per_column = FSEMU_AUDIOSTRIP_DRIFT_LIMIT_PPB / half;
    int64_t ppb = frame->drift_correction_ppb;
    int64_t columns =
        (ppb >= 0 ? ppb + ppb_per_column / 2 : ppb - ppb_per_column / 2) / ppb_per_column;
    if (columns < -half) {
        columns = -half;
    } else if (columns > half) {
        columns = half;
    }
    line[centre + columns] =
        frame->frozen ? palette->drift_correction_frozen : palette->drift_correction;
}

static void fsemu_audiostrip_draw_flags(const fsemu_audiostrip_palette_t *palette,
                                        const fsemu_audiostrip_frame_t *frame,
                                        fsemu_audiostrip_color_t *line)
{
    if (frame->underruns > 0) {
        line[FSEMU_AUDIOSTRIP_UNDERRUN_COLUMN] = palette->flag_underrun;
        line[FSEMU_AUDIOSTRIP_UNDERRUN_COLUMN + 1] = palette->flag_underrun;
    }
    if (frame->overruns > 0) {
        line[FSEMU_AUDIOSTRIP_OVERRUN_COLUMN] = palette->flag_overrun;
        line[FSEMU_AUDIOSTRIP_OVERRUN_COLUMN + 1] = palette->flag_overrun;
    }
}

void fsemu_audiostrip_draw_row(const fsemu_audiostrip_palette_t *palette,
                               const fsemu_audiostrip_frame_t *frame, uint8_t *pixels)
{
    fsemu_audiostrip_color_t line[FSEMU_AUDIOSTRIP_WIDTH];
    for (int x = 0; x < FSEMU_AUDIOSTRIP_WIDTH; x++) {
        line[x] = palette->background;
    }
    fsemu_audiostrip_draw_levels(palette, frame, line);
    fsemu_audiostrip_draw_drift_correction(palette, frame, line);
    fsemu_audiostrip_draw_flags(palette, frame, line);
    memcpy(pixels, line, sizeof(line));
}

// ----------------------------------------------------------------------------
// The header.

// The header's font: 3 x 5 pixels per glyph, a row per byte with the left column in bit 2, and a
// column between glyphs.
#define FSEMU_AUDIOSTRIP_GLYPH_WIDTH 3
#define FSEMU_AUDIOSTRIP_GLYPH_HEIGHT 5

typedef struct fsemu_audiostrip_glyph_t {
    char c;
    uint8_t rows[FSEMU_AUDIOSTRIP_GLYPH_HEIGHT];
} fsemu_audiostrip_glyph_t;

static const fsemu_audiostrip_glyph_t fsemu_audiostrip_glyphs[] = {
    {'0', {7, 5, 5, 5, 7}}, {'1', {2, 6, 2, 2, 7}}, {'2', {7, 1, 7, 4, 7}}, {'3', {7, 1, 7, 1, 7}},
    {'4', {5, 5, 7, 1, 1}}, {'5', {7, 4, 7, 1, 7}}, {'6', {7, 4, 7, 5, 7}}, {'7', {7, 1, 1, 1, 1}},
    {'8', {7, 5, 7, 5, 7}}, {'9', {7, 5, 7, 1, 7}}, {'+', {0, 2, 7, 2, 0}}, {'-', {0, 0, 7, 0, 0}},
    {'.', {0, 0, 0, 0, 2}}, {'F', {7, 4, 6, 4, 4}}, {'P', {7, 5, 7, 4, 4}}, {'M', {5, 7, 5, 5, 5}},
};

// The text's first column: in line with the drift correction band.
#define FSEMU_AUDIOSTRIP_HEADER_TEXT_COLUMN                                                        \
    (FSEMU_AUDIOSTRIP_DRIFT_CENTRE_COLUMN - FSEMU_AUDIOSTRIP_DRIFT_HALF_COLUMNS)

int64_t fsemu_audiostrip_get_rate_feed_forward_tenths(int rate_feed_forward_ppb)
{
    int64_t ppb = rate_feed_forward_ppb;
    return (ppb >= 0 ? ppb + 50 : ppb - 50) / 100;
}

void fsemu_audiostrip_format_rate_feed_forward(int rate_feed_forward_ppb, char *text, int size)
{
    int64_t tenths = fsemu_audiostrip_get_rate_feed_forward_tenths(rate_feed_forward_ppb);
    if (tenths == 0) {
        snprintf(text, size, "0.0");
        return;
    }
    int64_t magnitude = tenths < 0 ? -tenths : tenths;
    snprintf(text, size, "%c%lld.%lld", tenths < 0 ? '-' : '+', (long long)(magnitude / 10),
             (long long)(magnitude % 10));
}

void fsemu_audiostrip_draw_header(const fsemu_audiostrip_palette_t *palette,
                                  int rate_feed_forward_ppb, uint8_t *pixels, int stride)
{
    char number[16];
    fsemu_audiostrip_format_rate_feed_forward(rate_feed_forward_ppb, number, sizeof(number));
    char text[32];
    snprintf(text, sizeof(text), "FF %s PPM", number);

    // The box: a column before the text, and the glyphs' columns with the column after each (the
    // last one's the column after the text).
    const int box_start = FSEMU_AUDIOSTRIP_HEADER_TEXT_COLUMN - 1;
    int box_end = FSEMU_AUDIOSTRIP_HEADER_TEXT_COLUMN +
                  (int)strlen(text) * (FSEMU_AUDIOSTRIP_GLYPH_WIDTH + 1);
    if (box_end > FSEMU_AUDIOSTRIP_WIDTH) {
        box_end = FSEMU_AUDIOSTRIP_WIDTH;
    }
    for (int y = 0; y < FSEMU_AUDIOSTRIP_HEADER_HEIGHT; y++) {
        fsemu_audiostrip_color_t *row = (fsemu_audiostrip_color_t *)(pixels + y * stride);
        for (int x = box_start; x < box_end; x++) {
            row[x] = palette->background;
        }
    }
    int x0 = FSEMU_AUDIOSTRIP_HEADER_TEXT_COLUMN;
    for (const char *c = text; *c; c++, x0 += FSEMU_AUDIOSTRIP_GLYPH_WIDTH + 1) {
        const fsemu_audiostrip_glyph_t *glyph = NULL;
        for (size_t i = 0; i < sizeof(fsemu_audiostrip_glyphs) / sizeof(fsemu_audiostrip_glyphs[0]);
             i++) {
            if (fsemu_audiostrip_glyphs[i].c == *c) {
                glyph = &fsemu_audiostrip_glyphs[i];
                break;
            }
        }
        if (glyph == NULL) {
            // A space.
            continue;
        }
        // A row of background above the text (and below it).
        for (int gy = 0; gy < FSEMU_AUDIOSTRIP_GLYPH_HEIGHT; gy++) {
            fsemu_audiostrip_color_t *row =
                (fsemu_audiostrip_color_t *)(pixels + (gy + 1) * stride);
            for (int gx = 0; gx < FSEMU_AUDIOSTRIP_GLYPH_WIDTH; gx++) {
                int x = x0 + gx;
                if (x < FSEMU_AUDIOSTRIP_WIDTH &&
                    (glyph->rows[gy] & (1 << (FSEMU_AUDIOSTRIP_GLYPH_WIDTH - 1 - gx)))) {
                    row[x] = palette->header_text;
                }
            }
        }
    }
}
