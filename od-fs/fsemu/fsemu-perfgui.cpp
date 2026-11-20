// ----------------------------------------------------------------------------
// FSEMU library; Copyright (c) 2011-2021 Frode Solheim. See the file COPYING.
// ----------------------------------------------------------------------------

#define FSEMU_INTERNAL
#include "fsemu-perfgui.h"

#include <math.h>
#include <stdint.h>
#include <string.h>

#include "fsapp-adornment.h"
#include "fsapp-color.h"
#include "fsapp-main.h"
#include "fsapp-mainthread.h"
#include "fsapp-time.h"
#include "fs/trace.h"
#include "fsemu-audio.h"
#include "fsemu-audioring.h"
#include "fsemu-audiostrip.h"
#include "fsemu-frame.h"
#include "fsemu-frameinfo.h"
#include "fsemu-graphrows.h"
#include "fsemu-mainloop.h"
#include "fsemu-module.h"
#include "fsemu-option.h"
#include "fsemu-stripe.h"
#include "fsemu-video.h"

// The graph is one 480 x 360 image: the left half is the video part (the rows, fsemu-graphrows.h,
// one row per emulated frame), the right half the audio strip, with each frame's audio in the same
// row as its video. Each half is the content of an adornment (fsapp-adornment.h), drawn by fsapp
// from its own texture, so the halves do not blend into each other with the linear filter.
//
// The image is premultiplied, since the detailed graph's adornments are translucent. The sidebars'
// adornments are opaque, and fsapp ignores their alpha: the sidebars' premultiplied white with
// alpha a is then the grey a, which is how FS-UAE 4 drew them (blended over its black bars), and
// transparent is black.

#define FSEMU_PERFGUI_IMAGE_WIDTH (2 * FSEMU_GRAPHROWS_WIDTH)
#define FSEMU_PERFGUI_IMAGE_HEIGHT FSEMU_GRAPHROWS_HEIGHT
#define FSEMU_PERFGUI_STRIDE (FSEMU_PERFGUI_IMAGE_WIDTH * 4)

static_assert(FSEMU_AUDIOSTRIP_WIDTH == FSEMU_GRAPHROWS_WIDTH,
              "the audio strip is the image's half");

// The detailed graph is scaled by whole steps of the window's height in pixels (1 at 1080p, 2 at
// 4K), the image's columns one pixel wide at 1080p.
#define FSEMU_PERFGUI_STEP_HEIGHT 1080

// No last frame fed to the rows yet (since the graph was turned on).
#define FSEMU_PERFGUI_NO_FRAME INT32_MIN

// ----------------------------------------------------------------------------

int fsemu_perfgui_log_level = SDL_LOG_PRIORITY_INFO;

// The mode (fsemu_perfgui_mode_t), changed from any thread (Alt+O in the event hook, the UI
// thread's config), and read on the main loop's two sides.
static SDL_AtomicInt fsemu_perfgui_mode_value;

// The graph's adornments (left: video, right: audio), created, destroyed and committed to by the
// frame hook (host window side), behind the lock.
static SDL_Mutex *fsemu_perfgui_lock;
static fsapp_adornment_t *fsemu_perfgui_adornments[2];
// The mode the adornments were created for (sidebars or detailed), behind the lock.
static fsemu_perfgui_mode_t fsemu_perfgui_adornments_mode;

// The graph's state, behind the lock: the swapped hook (drawing side) feeds the rows, and the frame
// hook (host window side) draws the changed rows into the image and commits them. The commit is on
// the host window side, since fsapp takes committed content in with the next snapshot, which the
// host window side writes after its frame hooks: in render thread mode, content committed by the
// swapped hook would reach only the snapshot after the one the render thread takes at once after
// its swap, so it would be taken in a frame later, and the graph could commit only every other
// frame (until it was taken in, the image must not change).
static struct {
    bool initialized;
    // The audio strip's palette (fsemu-audiostrip.h) for the mode, NULL before the first.
    const fsemu_audiostrip_palette_t *audio_palette;
    // The rate feed-forward the audio strip's header shows (in tenths of a ppm, as it is shown),
    // INT64_MIN when the header has not been drawn.
    int64_t header_tenths;

    // The image (the content of both adornments) and the video part's rows.
    uint8_t image[FSEMU_PERFGUI_IMAGE_HEIGHT * FSEMU_PERFGUI_STRIDE];
    fsemu_graphrows_t rows;
    // Whether the rows are fed (the graph is on), whether they have been since the process started,
    // and whether they started then (with the session's first frames).
    bool active;
    bool started;
    bool from_session_start;
    // The newest frame fed to the rows, and whether the rows were told of a pause.
    int last_frame;
    bool paused;
    // The next frame whose stripe (fsemu-stripe.h) is given to its row, and room for its spans.
    int next_stripe_frame;
    fsemu_stripe_span_t stripe_spans[FSEMU_STRIPE_MAX_SPANS];
    // Set when new adornments are created: the whole image is committed to them.
    bool commit_all;
} fsemu_perfgui;

// Sets the rows' and the audio strip's palettes for the mode. Changing them draws all rows again.
static void fsemu_perfgui_set_colors(int color_set)
{
    bool sidebars = color_set == FSEMU_PERFGUI_MODE_SIDEBARS;
    fsemu_graphrows_set_palette(&fsemu_perfgui.rows, sidebars ? &fsemu_graphrows_sidebars_palette
                                                              : &fsemu_graphrows_detailed_palette);
    const fsemu_audiostrip_palette_t *audio_palette =
        sidebars ? &fsemu_audiostrip_sidebars_palette : &fsemu_audiostrip_detailed_palette;
    if (fsemu_perfgui.audio_palette != audio_palette) {
        fsemu_perfgui.audio_palette = audio_palette;
        fsemu_perfgui.header_tenths = INT64_MIN;
    }
}

// ----------------------------------------------------------------------------
// The audio strip.

static void fsemu_perfgui_fill_audio_row(uint8_t *row, const void *color)
{
    for (int x = 0; x < FSEMU_GRAPHROWS_WIDTH; x++) {
        memcpy(row + x * 4, color, 4);
    }
}

// Draws the audio row of frame into row (the start of the image row's right half) from the frame's
// audio statistics (fsemu-audiostrip.h).
static void fsemu_perfgui_draw_audio_row(int frame, uint8_t *row)
{
    fsemu_audio_frame_stats_t stats;
    fsemu_audio_frame_stats(frame, &stats);

    fsemu_audiostrip_frame_t strip = {};
    if (fsemu_audio_frequency()) {
        // The newly written part includes what the emulator has not written to the audio ring
        // buffer yet (Paula's partly filled buffer).
        strip.newly_written_ns = fsemu_audio_bytes_to_ns(stats.recent_bytes + stats.pending_bytes);
        strip.ring_buffer_ns = fsemu_audio_bytes_to_ns(stats.buffer_bytes - stats.recent_bytes);
        strip.in_flight_ns = fsemu_audio_bytes_to_ns(stats.in_flight_bytes);
        strip.minimum_ns = fsemu_audio_bytes_to_ns(stats.min_buffer_bytes);
        strip.average_ns = stats.avg_latency_ns;
        strip.target_ns = stats.target_latency_ns;
        strip.ceiling_ns = stats.target_latency_ns * FSEMU_AUDIORING_CEILING_FACTOR;
    }
    strip.drift_correction_ppb = stats.drift_correction_ppb;
    strip.frozen = stats.rate_adjustment_frozen;
    strip.underruns = stats.underruns;
    strip.overruns = stats.overruns;
    fsemu_audiostrip_draw_row(fsemu_perfgui.audio_palette, &strip, row);
}

// Draws the audio half of count image rows from first, wrapping from the last row to the first:
// each frame's audio in its video row, the cursor after the newest row, and nothing in the others.
// The header's rows (at the top) are drawn too, and the header must then be drawn over them. The
// audio statistics are kept for the last 256 frames, so older rows (drawn again after a palette
// change) get none.
static void fsemu_perfgui_draw_audio_rows(int first, int count)
{
    fsemu_graphrows_t *rows = &fsemu_perfgui.rows;
    int newest_row = fsemu_graphrows_get_newest_row(rows);
    int newest_frame = fsemu_graphrows_get_row_frame(rows, newest_row);
    int cursor_row = newest_row >= 0 ? (newest_row + 1) % FSEMU_PERFGUI_IMAGE_HEIGHT : -1;
    for (int i = 0; i < count; i++) {
        int y = (first + i) % FSEMU_PERFGUI_IMAGE_HEIGHT;
        uint8_t *row = fsemu_perfgui.image + y * FSEMU_PERFGUI_STRIDE + FSEMU_GRAPHROWS_WIDTH * 4;
        int frame = fsemu_graphrows_get_row_frame(rows, y);
        if (frame >= 0 && frame > newest_frame - (FSEMU_FRAMEINFO_COUNT - 16)) {
            fsemu_perfgui_draw_audio_row(frame, row);
        } else if (frame >= 0) {
            fsemu_perfgui_fill_audio_row(row, &fsemu_perfgui.audio_palette->background);
        } else if (y == cursor_row) {
            fsemu_perfgui_fill_audio_row(row, &rows->palette->cursor);
        } else {
            static const uint8_t transparent[4] = {0, 0, 0, 0};
            fsemu_perfgui_fill_audio_row(row, transparent);
        }
    }
}

// The audio strip's header shows the rate feed-forward of the newest frame, over the audio of the
// image's top rows: draws those rows and it again when what it shows has changed (or when forced:
// after those rows were drawn), and returns whether it did.
static bool fsemu_perfgui_draw_audio_header(bool force)
{
    fsemu_graphrows_t *rows = &fsemu_perfgui.rows;
    int newest_frame = fsemu_graphrows_get_row_frame(rows, fsemu_graphrows_get_newest_row(rows));
    int ppb = 0;
    if (newest_frame >= 0) {
        fsemu_audio_frame_stats_t stats;
        fsemu_audio_frame_stats(newest_frame, &stats);
        ppb = stats.rate_feed_forward_ppb;
    }
    int64_t tenths = fsemu_audiostrip_get_rate_feed_forward_tenths(ppb);
    if (!force && tenths == fsemu_perfgui.header_tenths) {
        return false;
    }
    // The rows first: a shorter text's box leaves part of a longer one's.
    fsemu_perfgui_draw_audio_rows(0, FSEMU_AUDIOSTRIP_HEADER_HEIGHT);
    fsemu_audiostrip_draw_header(fsemu_perfgui.audio_palette, ppb,
                                 fsemu_perfgui.image + FSEMU_GRAPHROWS_WIDTH * 4,
                                 FSEMU_PERFGUI_STRIDE);
    fsemu_perfgui.header_tenths = tenths;
    return true;
}

// ----------------------------------------------------------------------------
// Feeding the rows.

// The axis for the rows written at this swap: the predicted host vertical blank with vsync on.
static void fsemu_perfgui_set_axis(const fsapp_frame_record_t *record, int frame)
{
    fsemu_graphrows_axis_t axis = {};
    if (fsemu_video_vsync()) {
        double interval_ns = fsemu_mainloop_get_predicted_host_refresh_interval_ns();
        if (interval_ns > 0.0) {
            axis.vertical_blank_ns =
                fsemu_mainloop_get_predicted_host_vertical_blank_ns(record->swap_ns);
            axis.interval_ns = interval_ns;
            axis.settled = fsemu_mainloop_is_predicted_host_vertical_blank_settled();
        }
    } else {
        // Without vsync, an axis of emulated frame periods through the presented frame's origin
        // (each row's x = 0 is its own frame's origin).
        double hz = fsemu_frame_rate_hz();
        int64_t origin_ns = FSEMU_FRAMEINFO(frame).origin_ns;
        if (hz >= 1.0 && origin_ns != 0) {
            axis.vertical_blank_ns = origin_ns;
            axis.interval_ns = (double)SDL_NS_PER_SECOND / hz;
            axis.settled = true;
        }
    }
    fsemu_graphrows_set_axis(&fsemu_perfgui.rows, &axis);
}

// Adds emulated frame n to the rows, with the events the emulation thread recorded for it.
static void fsemu_perfgui_add_frame(int n)
{
    fsemu_graphrows_t *rows = &fsemu_perfgui.rows;
    const fsemu_frameinfo_t *info = &FSEMU_FRAMEINFO(n);
    if (info->session_start) {
        fsemu_graphrows_on_session_start(rows);
    }
    if (info->paused) {
        // A frame number used up by a pause, with no picture.
        if (!fsemu_perfgui.paused) {
            fsemu_graphrows_on_pause(rows);
            fsemu_perfgui.paused = true;
        }
        return;
    }
    if (fsemu_perfgui.paused) {
        fsemu_graphrows_on_resume(rows);
        fsemu_perfgui.paused = false;
    }
    if (info->timeline_reset) {
        fsemu_graphrows_on_timeline_reset(rows);
    }
    fsemu_graphrows_frame_t frame = {};
    frame.number = n;
    frame.warp = info->frame_warp;
    frame.origin_ns = info->origin_ns;
    frame.planned_start_ns = info->planned_start_ns;
    frame.slot_start_ns = info->slot_started_ns;
    frame.planned_end_ns = info->ends_ns;
    frame.last_slice_posted_ns = info->last_slice_posted_ns;
    frame.frame_ready_ns = info->frame_ready_ns;
    frame.deadline_ns = info->deadline_ns;
    frame.slot_margin_ns = info->slot_margin_ns;
    fsemu_graphrows_add_frame(rows, &frame);
}

// Feeds the rows the frames since the last swap up to frame, and the swap that presented it.
static void fsemu_perfgui_feed(const fsapp_frame_record_t *record, int frame)
{
    fsemu_graphrows_t *rows = &fsemu_perfgui.rows;
    // Frames older than this have left the frame info.
    int oldest = frame - (FSEMU_FRAMEINFO_COUNT - 16);
    if (fsemu_perfgui.last_frame == FSEMU_PERFGUI_NO_FRAME) {
        // From the session's first frame when the graph was on from the start, else from this one.
        fsemu_perfgui.last_frame = fsemu_perfgui.from_session_start && oldest < 0 ? -1 : frame - 1;
    }
    fsemu_perfgui_set_axis(record, frame);
    for (int n = fsemu_perfgui.last_frame + 1; n <= frame; n++) {
        if (n < oldest) {
            // Left the frame info (a long warp between two swaps): a row without marks.
            fsemu_graphrows_frame_t bare = {};
            bare.number = n;
            fsemu_graphrows_add_frame(rows, &bare);
        } else {
            fsemu_perfgui_add_frame(n);
        }
    }
    if (frame > fsemu_perfgui.last_frame) {
        fsemu_perfgui.last_frame = frame;
    }
    fsemu_graphrows_swap_t swap = {};
    swap.swap_number = record->swap_number;
    // The event pump (the graph's mark and flag) only in one-thread mode: in render thread mode
    // the main thread pumps events all the time, also during the swap, and the record's events
    // phase is only when it wrote the snapshot this swap drew.
    if (!fsapp_mainthread_has_render_thread()) {
        swap.events_ns = record->phase_ns[FSAPP_FRAME_PHASE_EVENTS];
    }
    swap.drawing_paced_ns = record->phase_ns[FSAPP_FRAME_PHASE_DRAWING_PACED];
    swap.rendering_wait_ns = record->phase_ns[FSAPP_FRAME_PHASE_RENDERING_FINISHED];
    swap.rendering_finished_ns = record->rendering_finished_ns;
    swap.swap_wait_ns = record->phase_ns[FSAPP_FRAME_PHASE_SWAP];
    swap.swap_ns = record->swap_ns;
    if (record->host_vertical_blank != FSAPP_FRAME_HOST_VERTICAL_BLANK_UNKNOWN) {
        swap.host_vertical_blank_ns = record->host_vertical_blank_ns;
    }
    // The display's refreshes reported since the swap before (all before this swap's return), for
    // the rows written and the row this swap writes.
    for (int i = 0; i < record->display_refresh_count; i++) {
        fsemu_graphrows_on_display_refresh(rows, record->display_refreshes[i].refresh_ns);
    }
    fsemu_graphrows_on_swapped(rows, frame, &swap);
    // The presentations of earlier swaps (or this one) reported since the swap before.
    for (int i = 0; i < record->presented_count; i++) {
        const fsapp_frame_presented_t *presented = &record->presented[i];
        if (presented->presented_ns != 0) {
            fsemu_graphrows_on_presented(rows, presented->swap_number, presented->presented_ns);
        }
    }
}

// Gives the written rows the stripes (and emulation done) of their frames that have ended. A
// frame's row is written at its swap, which can come before the frame has ended (its end wait); its
// stripe then amends the row at a later swap. The recorder keeps as many frames' stripes as the
// frame info keeps frames (fewer only when they are long), so also the frames that warp makes
// between two swaps get theirs.
static void fsemu_perfgui_add_stripes(void)
{
    fsemu_graphrows_t *rows = &fsemu_perfgui.rows;
    int newest = fsemu_graphrows_get_row_frame(rows, fsemu_graphrows_get_newest_row(rows));
    int last = SDL_min(newest, fsemu_stripe_get_ended_frame());
    // Older ones have left the recorder (and the frame info) anyway.
    int first = SDL_max(fsemu_perfgui.next_stripe_frame, last - FSEMU_FRAMEINFO_COUNT);
    for (int n = first; n <= last; n++) {
        fsemu_stripe_frame_t ended;
        if (fsemu_stripe_read(n, fsemu_perfgui.stripe_spans, FSEMU_STRIPE_MAX_SPANS, &ended)) {
            fsemu_graphrows_on_frame_ended(rows, n, &ended);
        }
    }
    if (last >= fsemu_perfgui.next_stripe_frame) {
        fsemu_perfgui.next_stripe_frame = last + 1;
    }
}

// ----------------------------------------------------------------------------

static const char *fsemu_perfgui_mode_name(int mode)
{
    switch (mode) {
    case FSEMU_PERFGUI_MODE_SIDEBARS:
        return "sidebars";
    case FSEMU_PERFGUI_MODE_DETAILED:
        return "detailed";
    default:
        return "off";
    }
}

int fsemu_perfgui_cycle_mode(void)
{
    // Off, sidebars, detailed, off: the enum's order.
    int mode;
    int old_mode;
    do {
        old_mode = SDL_GetAtomicInt(&fsemu_perfgui_mode_value);
        mode = (old_mode + 1) % (FSEMU_PERFGUI_MODE_DETAILED + 1);
    } while (!SDL_CompareAndSwapAtomicInt(&fsemu_perfgui_mode_value, old_mode, mode));
    fsemu_perfgui_log("Performance graph: %s\n", fsemu_perfgui_mode_name(mode));
    return mode;
}

fsemu_perfgui_mode_t fsemu_perfgui_mode(void)
{
    return (fsemu_perfgui_mode_t)SDL_GetAtomicInt(&fsemu_perfgui_mode_value);
}

bool fsemu_perfgui_set_mode_from_string(const char *value)
{
    int mode;
    if (SDL_strcasecmp(value, "off") == 0 || SDL_strcmp(value, "0") == 0) {
        mode = FSEMU_PERFGUI_MODE_OFF;
    } else if (SDL_strcasecmp(value, "sidebars") == 0 || SDL_strcmp(value, "1") == 0) {
        mode = FSEMU_PERFGUI_MODE_SIDEBARS;
    } else if (SDL_strcasecmp(value, "detailed") == 0 || SDL_strcmp(value, "2") == 0) {
        mode = FSEMU_PERFGUI_MODE_DETAILED;
    } else {
        fsemu_perfgui_log("WARNING: Invalid value for " FSEMU_OPTION_PERFORMANCE_GUI
                          ": %s (off, sidebars or detailed)\n",
                          value);
        return false;
    }
    SDL_SetAtomicInt(&fsemu_perfgui_mode_value, mode);
    fsemu_perfgui_log(FSEMU_OPTION_PERFORMANCE_GUI " = %s\n", fsemu_perfgui_mode_name(mode));
    return true;
}

// ----------------------------------------------------------------------------
// The adornments.

// The window's pixels per window coordinate (host window side).
static double fsemu_perfgui_pixel_scale(void)
{
    int window_width, window_height, pixel_width, pixel_height;
    fsapp_main_get_window_size(&window_width, &window_height);
    fsapp_main_get_window_size_in_pixels(&pixel_width, &pixel_height);
    if (window_width <= 0 || pixel_width <= 0) {
        return 1.0;
    }
    return (double)pixel_width / window_width;
}

// Whether fsapp has taken in the content last committed to both adornments (also true before the
// first commit), so that the image may change again.
static bool fsemu_perfgui_is_content_taken(void)
{
    return fsapp_adornment_is_content_taken(fsemu_perfgui_adornments[0]) &&
           fsapp_adornment_is_content_taken(fsemu_perfgui_adornments[1]);
}

// From the frame hook (host window side), with the lock held: draws the rows changed since the last
// commit and commits them to both adornments of the mode, once fsapp has taken in the previous
// content (it uploads it in the UI upload of the frame whose snapshot has it; the content must not
// change until then), and only when something changed. In one-thread mode that is once per frame
// (the content is taken in by the frame of the same pass); in render thread mode, where the frame
// hook runs in every pass of the host window side (several per frame), it is once per swap too:
// the rows change at each swap, and the content committed after the swap before is taken in by the
// render thread's next frame, early in it. The rows changed meanwhile are drawn next time.
static void fsemu_perfgui_commit(fsemu_perfgui_mode_t mode)
{
    if (!fsemu_perfgui.active || fsemu_perfgui_adornments[0] == NULL ||
        fsemu_perfgui_adornments_mode != mode || !fsemu_perfgui_is_content_taken()) {
        return;
    }
    fsemu_perfgui_set_colors(fsemu_perfgui_adornments_mode);
    int damage_y, damage_height;
    bool changed = fsemu_graphrows_draw(&fsemu_perfgui.rows, &damage_y, &damage_height);
    bool commit_all = fsemu_perfgui.commit_all;
    if (commit_all) {
        changed = true;
        damage_y = 0;
        damage_height = FSEMU_PERFGUI_IMAGE_HEIGHT;
        fsemu_perfgui.commit_all = false;
    }
    // The header's rows were drawn again when the damage covers them (it may wrap to the top).
    bool header_rows_drawn = false;
    if (changed) {
        fsemu_perfgui_draw_audio_rows(damage_y, damage_height);
        header_rows_drawn = damage_y < FSEMU_AUDIOSTRIP_HEADER_HEIGHT ||
                            damage_y + damage_height > FSEMU_PERFGUI_IMAGE_HEIGHT;
    }
    // The audio strip's header, when what it shows changed (it then joins the audio half's
    // damage), and whenever its rows were drawn again.
    bool header_changed = fsemu_perfgui_draw_audio_header(header_rows_drawn);
    if (!changed && !header_changed) {
        return;
    }
    if (damage_y + damage_height > FSEMU_PERFGUI_IMAGE_HEIGHT) {
        // The rows wrap from the last row to the first: one span for the commit, the whole image.
        // REVIEW: Once every 360 frames; two commits would be combined into one span by fsapp.
        damage_y = 0;
        damage_height = FSEMU_PERFGUI_IMAGE_HEIGHT;
    }
    if (FS_IsTraceEnabled()) {
        fsemu_graphrows_t *rows = &fsemu_perfgui.rows;
        FS_TraceValue values[] = {{"first_row", changed ? damage_y : 0},
                                  {"rows", changed ? damage_height : 0},
                                  {"header", header_changed ? 1 : 0}};
        FS_AddTraceInstantWithValues(
            "performance graph committed", (int64_t)SDL_GetTicksNS(),
            fsemu_graphrows_get_row_frame(rows, fsemu_graphrows_get_newest_row(rows)), values, 3);
    }
    // Both halves change in the same rows; fsapp uploads only those (the whole image into a new
    // adornment's texture). The audio half's header is a commit of its own, which fsapp combines
    // with the rows' into one span.
    for (int i = 0; i < 2; i++) {
        uint8_t *pixels = fsemu_perfgui.image + i * FSEMU_GRAPHROWS_WIDTH * 4;
        if (changed) {
            fsapp_adornment_commit(fsemu_perfgui_adornments[i], pixels, FSEMU_GRAPHROWS_WIDTH,
                                   FSEMU_PERFGUI_IMAGE_HEIGHT, FSEMU_PERFGUI_STRIDE, damage_y,
                                   damage_height);
        }
        if (i == 1 && header_changed) {
            fsapp_adornment_commit(fsemu_perfgui_adornments[i], pixels, FSEMU_GRAPHROWS_WIDTH,
                                   FSEMU_PERFGUI_IMAGE_HEIGHT, FSEMU_PERFGUI_STRIDE, 0,
                                   FSEMU_AUDIOSTRIP_HEADER_HEIGHT);
        }
    }
}

void fsemu_perfgui_update_adornments(bool session)
{
    fsemu_perfgui_mode_t mode = session ? fsemu_perfgui_mode() : FSEMU_PERFGUI_MODE_OFF;
    SDL_LockMutex(fsemu_perfgui_lock);
    if (fsemu_perfgui_adornments[0] != NULL && mode != fsemu_perfgui_adornments_mode &&
        fsemu_perfgui_is_content_taken()) {
        // The graph is off, the session has stopped, or the mode changed (between sidebars and
        // detailed, which are different adornments). Only once fsapp has taken in their content
        // (else a later frame hook does it: in one-thread mode always the next, in render thread
        // mode one after the render thread's next frame has taken it in), since the image is
        // drawn again for new adornments at once, and fsapp could still be reading it for these.
        // REVIEW: Without a pacer (the session has stopped), a pass of the host window side after
        // that frame can be up to the idle wait (100 ms) later in render thread mode, so the graph
        // can stay that long after the session.
        for (int i = 0; i < 2; i++) {
            fsapp_adornment_destroy(fsemu_perfgui_adornments[i]);
            fsemu_perfgui_adornments[i] = NULL;
        }
    }
    if (mode != FSEMU_PERFGUI_MODE_OFF && fsemu_perfgui_adornments[0] == NULL) {
        for (int i = 0; i < 2; i++) {
            fsapp_adornment_t *adornment;
            if (mode == FSEMU_PERFGUI_MODE_SIDEBARS) {
                // Opaque (the default), placed by fsemu_perfgui_place_sidebars once the picture's
                // rectangle is known (until then the rectangle is empty and nothing is drawn).
                adornment = fsapp_adornment_create_in_front_of(fsemu_mainloop_emulation_surface());
            } else {
                adornment = fsapp_adornment_create_in_front();
                fsapp_adornment_set_opaque(adornment, false);
            }
            // The image is stretched to the strips (the rows to the window's or the surface's
            // height, the sidebars' columns to the bar's width), with the linear filter, so that no
            // column or row comes out one pixel and its neighbour two.
            fsapp_adornment_set_filter(adornment, FSAPP_ADORNMENT_FILTER_LINEAR);
            fsemu_perfgui_adornments[i] = adornment;
        }
        fsemu_perfgui_adornments_mode = mode;
        // The whole image is committed with the next commit (once the rows are fed).
        fsemu_perfgui.commit_all = true;
    }
    if (mode == FSEMU_PERFGUI_MODE_DETAILED && fsemu_perfgui_adornments_mode == mode &&
        fsemu_perfgui_adornments[0] != NULL) {
        // Each strip is the image's half, 240 pixels wide at 1080p and scaled by whole steps of
        // the window's height (2x at 4K), at the window's left and right edges, full height.
        int window_width, window_height, pixel_width, pixel_height;
        fsapp_main_get_window_size(&window_width, &window_height);
        fsapp_main_get_window_size_in_pixels(&pixel_width, &pixel_height);
        int step = SDL_max(1, pixel_height / FSEMU_PERFGUI_STEP_HEIGHT);
        int strip_width = (int)lround(FSEMU_GRAPHROWS_WIDTH * step / fsemu_perfgui_pixel_scale());
        fsapp_adornment_set_rect(fsemu_perfgui_adornments[0], 0, 0, strip_width, window_height);
        fsapp_adornment_set_rect(fsemu_perfgui_adornments[1], window_width - strip_width, 0,
                                 strip_width, window_height);
    }
    // Before the snapshot is written (after the frame hooks), so that a commit is taken in by the
    // drawing side's next frame.
    fsemu_perfgui_commit(mode);
    SDL_UnlockMutex(fsemu_perfgui_lock);
}

void fsemu_perfgui_place_sidebars(const SDL_Rect *surface_rect, const SDL_Rect *picture_rect,
                                  double pixel_scale)
{
    SDL_LockMutex(fsemu_perfgui_lock);
    if (fsemu_perfgui_adornments[0] != NULL &&
        fsemu_perfgui_adornments_mode == FSEMU_PERFGUI_MODE_SIDEBARS) {
        // Each strip fills the bar beside the picture, the surface's full height, and is not shown
        // when its bar is narrower than the image's half (240 pixels), so it never covers the
        // picture.
        // REVIEW: Placed on the drawing side (fsemu's drawing hook, and the commit in the wait),
        // but adornment rectangles are taken in with the next frame's snapshot: the strips follow
        // a change of the picture (resizing, Alt+S, Alt+C, the emulated video's size) one frame
        // later, so for that frame a strip can be drawn where the picture has grown into its bar
        // (in front of it).
        int min_bar = (int)ceil(FSEMU_GRAPHROWS_WIDTH / (pixel_scale > 0.0 ? pixel_scale : 1.0));
        int left_bar = picture_rect->x - surface_rect->x;
        int right_bar = (surface_rect->x + surface_rect->w) - (picture_rect->x + picture_rect->w);
        if (left_bar >= min_bar) {
            fsapp_adornment_set_rect(fsemu_perfgui_adornments[0], surface_rect->x, surface_rect->y,
                                     left_bar, surface_rect->h);
        } else {
            fsapp_adornment_set_rect(fsemu_perfgui_adornments[0], 0, 0, 0, 0);
        }
        if (right_bar >= min_bar) {
            fsapp_adornment_set_rect(fsemu_perfgui_adornments[1], picture_rect->x + picture_rect->w,
                                     surface_rect->y, right_bar, surface_rect->h);
        } else {
            fsapp_adornment_set_rect(fsemu_perfgui_adornments[1], 0, 0, 0, 0);
        }
    }
    SDL_UnlockMutex(fsemu_perfgui_lock);
}

void fsemu_perfgui_on_swapped(const fsapp_frame_record_t *record, int frame)
{
    SDL_LockMutex(fsemu_perfgui_lock);
    if (fsemu_perfgui_mode() == FSEMU_PERFGUI_MODE_OFF) {
        // The rows start afresh when the graph is turned on again.
        fsemu_perfgui.active = false;
        SDL_UnlockMutex(fsemu_perfgui_lock);
        return;
    }
    if (!fsemu_perfgui.active) {
        fsemu_graphrows_init(&fsemu_perfgui.rows, fsemu_perfgui.image, FSEMU_PERFGUI_STRIDE);
        fsemu_perfgui.from_session_start = !fsemu_perfgui.started;
        fsemu_perfgui.last_frame = FSEMU_PERFGUI_NO_FRAME;
        fsemu_perfgui.next_stripe_frame = 0;
        fsemu_perfgui.paused = false;
        fsemu_perfgui.header_tenths = INT64_MIN;
        fsemu_perfgui.active = true;
        fsemu_perfgui.started = true;
    }
    if (frame >= 0) {
        fsemu_perfgui_feed(record, frame);
    }
    fsemu_perfgui_add_stripes();
    // The frame hook draws and commits the changed rows (fsemu_perfgui_commit).
    SDL_UnlockMutex(fsemu_perfgui_lock);
}

// ----------------------------------------------------------------------------

static void fsemu_perfgui_quit(void)
{
}

// ----------------------------------------------------------------------------
// Init
// ----------------------------------------------------------------------------

void fsemu_perfgui_init(void)
{
    if (FSEMU_MODULE_INIT(perfgui)) {
        return;
    }
    fsemu_perfgui_lock = SDL_CreateMutex();

    const char *value = fsemu_option_const_string(FSEMU_OPTION_PERFORMANCE_GUI);
    if (value) {
        fsemu_perfgui_set_mode_from_string(value);
    }
}
