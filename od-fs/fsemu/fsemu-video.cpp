#define FSEMU_INTERNAL
#include "fsemu-video.h"

#include "fsapp-bitmap.h"
#include "fsapp-frame.h"
#include "fsapp-surface.h"
#include "fsapp-time.h"
#include "fs/asyncqueue.h"
#include "fs/log.h"
#include "fs/trace.h"
#include "fsapp-util.h"
#include "fsemu-emuthread.h"
#include "fsemu-frame.h"
#include "fsemu-frameinfo.h"
#include "fsemu-layout.h"
#include "fsemu-mainloop.h"
#include "fsemu-module.h"
#include "fsemu-option.h"
#include "fsemu-perfgui.h"
#include "fsemu-screenshot.h"
#include "fsemu-sdl.h"
#include "fsemu-types.h"
#include "fsemu.h"

#include <algorithm>
#include <deque>
#include <vector>

int fsemu_video_log_level = SDL_LOG_PRIORITY_INFO;
// int fsemu_video_log_level = SDL_LOG_PRIORITY_DEBUG;

static struct {
    bool initialized;
    fsemu_video_format_t format;
    fsemu_rect_t rect;
    int ready;
    int vsync;
    // int rendered_frame;
    // 1 - disallow vsync, 2 - tried (and refused) to enable vsync
    int disallow_vsync;

    // Drawable size, used for layout and rendering. Not necessarily the same
    // as window size (for high-DPI windows on some platforms).
    fsemu_size_t drawable_size;

    bool must_render_frame;
    int64_t must_render_frame_until_ns;

    // The measured host refresh interval in nanoseconds, from the swapped hook (drawing side), also
    // read by the emulation thread (fsemu_video_get_host_refresh_interval_ns): written and read
    // under vsync_interval_lock. The intervals it is made from are the drawing side's
    // (fsemu_video_vsync_intervals_ns).
    double vsync_interval_avg_ns;
    SDL_SpinLock vsync_interval_lock;

    // Used with fsemu_video_frame_queue
    int last_posted_frame;
    int last_retrieved_frame;
    // fsemu_video_get_last_whole_posted_frame, plus one (so that 0 is none; emulation thread).
    int last_whole_posted_frame_plus_one;
    // FIXME: Move fsemu_video_frame_queue here

} fsemu_video;

static FS_AsyncQueue *fsemu_video_frame_queue;

fsemu_video_format_t fsemu_video_format(void)
{
    return fsemu_video.format;
}

void fsemu_video_set_format(fsemu_video_format_t format)
{
    fsemu_video.format = format;
}

static fsemu_video_format_t fsemu_video_default_format(void)
{
#ifdef FSEMU_LINUX_ARM
#warning not supprting RGB565 format anymore?
    // return FSEMU_VIDEO_FORMAT_RGB565;
    return FSEMU_VIDEO_FORMAT_BGRA;
#else
    return FSEMU_VIDEO_FORMAT_BGRA;
#endif
}

void fsemu_video_finalize_and_free_frame(fsemu_video_frame_t *frame)
{
    // printf("free\n");
    if (frame->finalize) {
        // printf("finalize\n");
        frame->finalize(frame);
    }
    free(frame);
}

// ----------------------------------------------------------------------------

void fsemu_video_drawable_size(fsemu_size_t *size)
{
    *size = fsemu_video.drawable_size;
}

void fsemu_video_set_drawable_size(fsemu_size_t *size)
{
    fsemu_video.drawable_size = *size;
    // Also automatically set layout size to drawable size?
    fsemu_layout_set_size(size);
}

// ----------------------------------------------------------------------------

void fsemu_video_set_size_2(int width, int height, int drawable_width, int drawable_height)
{
    fsemu_video_log_info("Window size %dx%d : drawable size %dx%d\n", width, height, drawable_width,
                         drawable_height);
    fsemu_size_t drawable_size = {drawable_width, drawable_height};
    fsemu_video_set_drawable_size(&drawable_size);
}

int fsemu_video_vsync(void)
{
    return fsemu_video.vsync;
}

void fsemu_video_disallow_vsync(int disallow_vsync)
{
    fsemu_video.disallow_vsync = disallow_vsync;
}

bool fsemu_video_vsync_prevented(void)
{
    return fsemu_video.disallow_vsync == 2;
}

void fsemu_video_set_vsync(int vsync)
{
    // SDL_Log("Set vsync=%d\n", vsync);
    if (vsync && fsemu_video.disallow_vsync) {
        // SDL_Log("Set vsync=%d disabled\n", vsync);
        fsemu_video_log("Wanted to enable vsync, but disabled by emulator");
        fsemu_video.disallow_vsync = 2;
        return;
    }
    fsemu_video.vsync = vsync;
}

void fsemu_video_toggle_vsync(void)
{
    fsemu_video_set_vsync(!fsemu_video_vsync());
}

// ----------------------------------------------------------------------------
// The emulated picture, handed to fsapp as the emulation surface's streamed content (fsapp owns
// the textures and draws it; fsemu makes no GL call). All on the main loop's drawing side: the
// pacer's wait hook takes in the slices (fsemu_video_work), uploading each and committing at frame
// ready, and the drawing hook refits the picture in every frame (fsemu_video_update_picture).

static struct {
    // The frame being uploaded, from its first slice until it is committed or abandoned.
    bool begun;
    int number;
    // Whether the frame was cropped under the auto crop policy (its first slice's mark).
    bool auto_crop;
    // The upload region in the frame's coordinates (the core's buffer): the crop with a fixed crop
    // policy, the whole frame with auto-crop.
    fsemu_rect_t region;
    // The frame's rows taken in so far (from row 0, in the frame's coordinates).
    int rows_taken;
    // Whether fsapp has content (committed, and not dropped since), and its source rectangle (in
    // the upload region).
    bool has_content;
    SDL_Rect source;
    // The picture rectangle last given to fsapp (the surface's coordinates).
    SDL_Rect picture;
    // The emulation surface's rectangle (window coordinates) as last seen by the drawing hook
    // (surface_known), and whether it is in the frame being drawn.
    bool surface_known;
    bool surface_visible;
    SDL_Rect surface_rect;
    // The window's pixels per window coordinate in the frame being drawn (from its snapshot), for
    // placing the sidebars: the drawing side does not ask the window itself.
    double pixel_scale;
    // The session has stopped: the content is dropped, and no frame is committed.
    bool stopped;
    // The filter last passed to fsapp (fsapp's default is linear).
    fsemu_layout_filter_t filter;
} fsemu_video_stream;

// The picture rectangle for the content shown (its source size) in the emulation surface's
// rectangle, by fsemu's layout code under the fit policy, in the surface's coordinates. Places the
// performance graph's sidebars from it when the surface is in this frame.
static SDL_Rect fsemu_video_fit_picture(void)
{
    SDL_Rect picture = {0, 0, 0, 0};
    if (!fsemu_video_stream.surface_known) {
        // The surface has not been in a frame yet: the drawing hook refits when it is.
        return picture;
    }
    const SDL_Rect *surface_rect = &fsemu_video_stream.surface_rect;
    const SDL_Rect *source = &fsemu_video_stream.source;
    // The layout is made in the emulation surface's rectangle (window coordinates).
    fsemu_layout_set_client_area(surface_rect);
    fsemu_layout_set_size_2(surface_rect->w, surface_rect->h);
    fsemu_layout_set_video_size(source->w, source->h);
    fsemu_layout_set_pixel_aspect(((double)source->w / source->h) / (4.0 / 3.0));
    fsemu_layout_video_rect(&picture);
    if (fsemu_video_stream.surface_visible) {
        fsemu_perfgui_place_sidebars(surface_rect, &picture, fsemu_video_stream.pixel_scale);
    }
    picture.x -= surface_rect->x;
    picture.y -= surface_rect->y;
    return picture;
}

// Takes in one slice (drawing side, in the pacer's wait): begins a frame at the first slice of a
// frame not begun, uploads the slice's rows of the upload region, and at the frame's last slice
// commits it (frame ready).
static void fsemu_video_take_in_slice(fsemu_video_frame_t *frame)
{
    fsemu_video_must_render_frame();
    fsapp_surface_t *surface = fsemu_mainloop_emulation_surface();
    // The posting side always posts BGRA (4 bytes per pixel), and the content is attached as BGRA.
    const int bytes_per_pixel = 4;

    // The rows of the frame drawn so far: partial is the rows drawn, the frame's height at its
    // last slice, or 0 for a whole frame at once (RTG).
    bool last = frame->partial == 0 || frame->partial == frame->height;
    int rows = frame->partial == 0 ? frame->height : SDL_min(frame->partial, frame->height);

    if (!fsemu_video_stream.begun || frame->number != fsemu_video_stream.number ||
        rows <= fsemu_video_stream.rows_taken) {
        // A new frame begins: at the first slice of a frame number not begun, and also at a slice
        // of the same number with no more rows than were taken in: the emulation has started
        // drawing that frame again from its top, so the rows taken in so far are stale. One half
        // uploaded (its later slices dropped from the queue, or a frame started again) is
        // abandoned: fsapp never shows it.
        fsemu_video_stream.begun = false;
        fsemu_rect_t frame_rect = {0, 0, frame->width, frame->height};
        fsemu_rect_t region;
        // The crop policy the frame was cropped under, read once at its first slice.
        bool auto_crop = (frame->flags & FSEMU_FRAME_FLAG_AUTO_CROP) != 0;
        if (auto_crop) {
            // The crop is only known when the frame is complete: the whole frame is uploaded.
            region = frame_rect;
        } else if (!SDL_GetRectIntersection(&frame->limits, &frame_rect, &region)) {
            region = {0, 0, 0, 0};
        }
        if (region.w <= 0 || region.h <= 0) {
            // Nothing to show (no crop yet): the frame is left out, as before.
            fsemu_video_log_debug("Empty upload region\n");
            return;
        }
        fsapp_surface_begin_streamed_frame(surface, region.w, region.h);
        fsemu_video_stream.begun = true;
        fsemu_video_stream.number = frame->number;
        fsemu_video_stream.auto_crop = auto_crop;
        fsemu_video_stream.region = region;
        fsemu_video_stream.rows_taken = 0;
    }

    // The slice's new rows inside the upload region.
    const fsemu_rect_t *region = &fsemu_video_stream.region;
    int first = SDL_max(fsemu_video_stream.rows_taken, region->y);
    int end = SDL_min(rows, region->y + region->h);
    if (first < end) {
        const uint8_t *pixels =
            frame->buffer + (size_t)first * frame->stride + (size_t)region->x * bytes_per_pixel;
        fsapp_surface_upload_streamed_slice(surface, first - region->y, end - first, pixels,
                                            frame->stride);
    }
    fsemu_video_stream.rows_taken = rows;
    if (FS_IsTraceEnabled()) {
        FS_AddTraceInstant("slice taken in", (int64_t)SDL_GetTicksNS(), frame->number, "lines",
                           rows);
    }
    if (!last) {
        return;
    }

    // Frame ready: the source rectangle is the whole region with a fixed crop, and the last
    // slice's crop (relative to the region) with auto-crop.
    fsemu_video_stream.begun = false;
    SDL_Rect source = {0, 0, region->w, region->h};
    if (fsemu_video_stream.auto_crop) {
        SDL_Rect crop;
        if (!SDL_GetRectIntersection(&frame->limits, region, &crop)) {
            fsemu_video_log_debug("Empty crop\n");
            return;
        }
        source = {crop.x - region->x, crop.y - region->y, crop.w, crop.h};
    }
    if (fsemu_video_stream.stopped) {
        // The session has stopped (its content dropped): its last frames, still in the queue, are
        // not shown, so that the next session starts from the fill colour, never this session's
        // picture (fsemu_video_update_picture).
        return;
    }
    fsemu_video_stream.source = source;
    fsemu_video_stream.has_content = true;
    fsemu_video_stream.picture = fsemu_video_fit_picture();
    fsapp_surface_commit_streamed_frame(surface, &source, &fsemu_video_stream.picture);

    fsemu_frame_number_posted = frame->number;
    fsemu_video_set_ready(true);
    int64_t ready_ns = (int64_t)SDL_GetTicksNS();
    // For the performance graph's frame ready mark, and the pacer's hold and the slot margin.
    FSEMU_FRAMEINFO(frame->number).frame_ready_ns = ready_ns;
    if (FS_IsTraceEnabled()) {
        FS_AddTraceInstant("frame ready", ready_ns, frame->number, FS_TRACE_NO_VALUE, 0);
    }
}

bool fsemu_video_work(int32_t timeout_ms)
{
    fsemu_video_frame_t *frame = fsemu_video_get_frame(timeout_ms);
    if (frame == NULL) {
        return false;
    }
    fsemu_video_take_in_slice(frame);
    fsemu_video_finalize_and_free_frame(frame);
    return true;
}

void fsemu_video_update_picture(bool session_stopped, double pixel_scale)
{
    fsapp_surface_t *surface = fsemu_mainloop_emulation_surface();
    fsemu_video_stream.pixel_scale = pixel_scale;
    SDL_Rect surface_rect;
    fsemu_video_stream.surface_visible = fsapp_surface_get_frame_rectangle(surface, &surface_rect);
    if (fsemu_video_stream.surface_visible) {
        fsemu_video_stream.surface_rect = surface_rect;
        fsemu_video_stream.surface_known = true;
    }

    fsemu_layout_filter_t filter = fsemu_layout_get_filter();
    if (filter != fsemu_video_stream.filter) {
        fsapp_surface_set_streamed_filter(surface, filter == FSEMU_LAYOUT_FILTER_NEAREST
                                                       ? FSAPP_SURFACE_STREAM_FILTER_NEAREST
                                                       : FSAPP_SURFACE_STREAM_FILTER_LINEAR);
        fsemu_video_stream.filter = filter;
    }

    // TODO: A second session in one process (not possible yet) must clear stopped when it starts,
    // and must not take in slices the stopped session left in the queue.
    fsemu_video_stream.stopped = session_stopped;
    if (session_stopped && fsemu_video_stream.has_content) {
        // The next session starts from the fill colour, never the old picture.
        fsemu_video_log("Session stopped: dropping the emulated picture\n");
        fsapp_surface_drop_streamed_content(surface);
        fsemu_video_stream.has_content = false;
    }

    if (!fsemu_video_stream.surface_visible) {
        return;
    }
    if (!fsemu_video_stream.has_content) {
        // The fill shows in the whole surface, so no sidebars are shown.
        fsemu_perfgui_place_sidebars(&surface_rect, &surface_rect, pixel_scale);
        return;
    }
    // A refit to the surface's rectangle in this frame (a resize, Alt+S, also while paused).
    SDL_Rect picture = fsemu_video_fit_picture();
    if (!SDL_RectsEqual(&picture, &fsemu_video_stream.picture)) {
        fsapp_surface_set_streamed_picture(surface, &picture);
        fsemu_video_stream.picture = picture;
    }
    fsemu_frame_number_rendered = fsemu_frame_number_rendering;
}

void fsemu_video_take_frame(void)
{
    fsemu_frame_number_rendering = fsemu_frame_number_posted;
    fsemu_video_log_trace("fsemu_video_take_frame frame_number_rendering=%d\n",
                          fsemu_frame_number_rendering);
    fsemu_video.ready = false;
    // Reset this variable, will be set again if a new (actual) frame is
    // posted.
    // FIXME: Check if this is still needed / used
    fsemu_video.must_render_frame = false;
}

bool fsemu_video_initialized(void)
{
    return fsemu_video.initialized;
}

bool fsemu_video_ready(void)
{
    return fsemu_video.ready;
}

void fsemu_video_set_ready(bool ready)
{
    fsemu_video.ready = ready;
}

int fsemu_video_get_posting_frame_number(bool after_frame_end)
{
    // A picture posted after its frame's end comes when fsemu_frame_number_began has already moved
    // on. The first picture comes before any frame has begun (fsemu_frame_number_began is still
    // -1); it is part of frame 0.
    int number = after_frame_end ? fsemu_frame_number_began - 1 : fsemu_frame_number_began;
    return SDL_max(number, 0);
}

// The crop policy of the frame being posted, and its frame number (emulation thread only).
static struct {
    bool read;
    int number;
    fsemu_layout_crop_policy_t policy;
} fsemu_video_posting_crop;

fsemu_layout_crop_policy_t fsemu_video_get_frame_crop_policy(int number)
{
    if (!fsemu_video_posting_crop.read || number != fsemu_video_posting_crop.number) {
        fsemu_video_posting_crop.read = true;
        fsemu_video_posting_crop.number = number;
        fsemu_video_posting_crop.policy = fsemu_layout_get_crop_policy();
    }
    return fsemu_video_posting_crop.policy;
}

void fsemu_video_post_frame(fsemu_video_frame_t *frame)
{
    // printf("Posted frame %d %d\n", frame->width, frame->height);

    // FIXME: There is currently an issue with fast-forward; the queue will
    // build up, and rendering might not be able to catch up. This is
    // especially bad if clients also allocate memory for each frame which
    // means fast-forwarding can use a lot of memory, quickly.
    // Should be able to throw away old frames here...

    // if (frame->partial == frame->height) {
    //     static int64_t last;
    //     int64_t t = (int64_t)SDL_NS_TO_US(SDL_GetTicksNS());
    //     printf("START_OR_POST post %d %d %d %d + %lld\n", frame->partial,
    //     frame->height, fsemu_frame_number_posted, fsemu_frame_number_began,
    //     lld((t - last) / 1000)); last = t;
    //     fsapp_assert(fsemu_frame_number_posted == fsemu_frame_number_began -
    //     1);
    // }

    // FIXME: fsemu_frame_counter conflict with new vars
    // frame->number = fsemu_frame_counter();

    int frame_number = frame->number;
    if (fsemu_video_get_frame_crop_policy(frame_number) == FSEMU_LAYOUT_CROP_POLICY_AUTO) {
        frame->flags |= FSEMU_FRAME_FLAG_AUTO_CROP;
    }

    FS_LockAsyncQueue(fsemu_video_frame_queue);

    static int last_skipped_frame;

    if (frame->number > fsemu_video.last_posted_frame &&
        frame->number > fsemu_video.last_retrieved_frame + 1) {
        // printf("-- frame number: %d last posted %d last retrieved %d\n",
        //        frame_number,
        //        fsemu_video.last_posted_frame,
        //        fsemu_video.last_retrieved_frame);
        std::vector<fsemu_video_frame_t *> keep;
        fsemu_video_frame_t *f;
        while ((f = (fsemu_video_frame_t *)FS_TryPopAsyncQueueUnlocked(fsemu_video_frame_queue))) {
            if (f->number == fsemu_video.last_retrieved_frame) {
                // Need to keep this
                keep.push_back(f);
            } else {
                fsemu_video_finalize_and_free_frame(f);
            }
        }
        for (size_t i = keep.size(); i > 0; i--) {
            FS_PushFrontAsyncQueueUnlocked(fsemu_video_frame_queue, keep[i - 1]);
        }
        // fsapp_assert(frame);

        int from = SDL_max(last_skipped_frame, fsemu_video.last_retrieved_frame + 1);
        for (int i = from; i < frame->number; i++) {
            // printf("SKIPPED FRAME %d\n", i);
            last_skipped_frame = i;
            FSEMU_FRAMEINFO(i).render_skipped = true;
            if (FS_IsTraceEnabled()) {
                FS_AddTraceInstant("frame dropped", (int64_t)SDL_GetTicksNS(), i, FS_TRACE_NO_VALUE,
                                   0);
            }
        }
    }

    // Read before the push: from then on the main thread may take the slice in and free it. The
    // time too, so that the slice is posted in the trace before it is taken in, and the last
    // slice's in the frame info (for the performance graph's hand-over) before the drawing side
    // reads it.
    bool last_slice = frame->partial == 0 || frame->partial == frame->height;
    int lines = frame->partial == 0 ? frame->height : frame->partial;
    int64_t posted_ns = FS_IsTraceEnabled() || last_slice ? (int64_t)SDL_GetTicksNS() : 0;
    if (last_slice) {
        FSEMU_FRAMEINFO(frame_number).last_slice_posted_ns = posted_ns;
    }
    FS_PushAsyncQueueUnlocked(fsemu_video_frame_queue, frame);
    fsemu_video.last_posted_frame = frame_number;
    FS_UnlockAsyncQueue(fsemu_video_frame_queue);
    if (last_slice) {
        fsemu_video.last_whole_posted_frame_plus_one = frame_number + 1;
    }
    if (FS_IsTraceEnabled()) {
        FS_AddTraceInstant(last_slice ? "last slice posted" : "slice posted", posted_ns,
                           frame_number, "lines", lines);
    }

    // fsemu's pacer takes the slices in on the main loop's drawing side: end its frame wait. Never
    // blocks.
    fsapp_frame_wake();

    // if (frame->partial == 0 || frame->partial == frame->height) {
    //     fsemu_frame_number_posted = fsemu_frame_number_began;
    // }
}

int fsemu_video_get_last_whole_posted_frame(void)
{
    return fsemu_video.last_whole_posted_frame_plus_one - 1;
}

// Developer/test aid: FSEMU_AUTO_SCREENSHOT_FRAMES="250,500,1000" requests a screenshot (saved to
// FSEMU_SCREENSHOTS_DIR) once each listed frame number has been reached. Allows capturing output
// when running headless, where keyboard shortcuts cannot be used.
static void fsemu_video_auto_screenshot(const fsemu_video_frame_t *frame)
{
    static bool initialized = false;
    static int frames[32];
    static int count = 0;
    static int next = 0;
    if (!initialized) {
        initialized = true;
        const char *env = getenv("FSEMU_AUTO_SCREENSHOT_FRAMES");
        while (env && *env && count < 32) {
            char *end = NULL;
            long value = strtol(env, &end, 10);
            if (end == env) {
                break;
            }
            frames[count++] = (int)value;
            env = (*end == ',') ? end + 1 : end;
        }
    }
    if (next < count && (int)frame->number >= frames[next]) {
        next++;
        fsemu_screenshot_capture();
    }
}

fsemu_video_frame_t *fsemu_video_get_frame(int32_t timeout_ms)
{
    fsapp_assert(timeout_ms >= -1);
    fsemu_video_frame_t *frame;

    FS_LockAsyncQueue(fsemu_video_frame_queue);
    if (timeout_ms == 0) {
        frame = (fsemu_video_frame_t *)FS_TryPopAsyncQueueUnlocked(fsemu_video_frame_queue);
    } else {
        frame = (fsemu_video_frame_t *)FS_PopAsyncQueueUnlockedTimeout(fsemu_video_frame_queue,
                                                                       timeout_ms);
    }

    if (frame == NULL) {
        FS_UnlockAsyncQueue(fsemu_video_frame_queue);
        return NULL;
    }

    // if (frame->number > fsemu_video.last_retrieved_frame) {
    //     // We want to start processing a new frame, but check if we are too
    //     // far behind
    //     while (frame->number < fsemu_video.last_posted_frame) {

    //     }
    // }

    fsemu_video.last_retrieved_frame = frame->number;

    FS_UnlockAsyncQueue(fsemu_video_frame_queue);

#if 0
    // See if there are more events, and only consider the last one.
    fsemu_video_frame_t *frame2;
    while ((frame2 = FS_TryPopAsyncQueue(fsemu_video_frame_queue))) {
        // FIXME: Can also check if an entire frame has been skipped here.
        printf("WARNING: Had to skip frame event\n");
        fsemu_video_free_frame(frame);
        frame = frame2;
    }
#endif
    if (frame->partial == 0 || frame->partial == frame->height) {
        fsemu_video_auto_screenshot(frame);
        if (fsemu_screenshot_should_capture()) {
            fsemu_screenshot_capture_video_frame(frame);
        }
    }

    return frame;
}

// FIXME: COPY WINDOW / LAYOUT SIZE on main <-> video sync ??
/*
static void fsemu_video_convert_coordinates(SDL_Rect *out,
                                            fsemu_rect *in,
                                            int coordinates)
{
    // FIXME: Use copy of window size? Layout?
    fsemu_size_t window_size;
    fsemu_window_size(&window_size);

    if (coordinates == FSEMU_COORD_REAL) {
        out->w = in->w;
        out->h = in->h;
        out->x = in->x;
        out->y = in->y;
    } else if (coordinates == FSEMU_COORD_1080P_LEFT) {
        double scale = window_size.h / 1080.0;

        out->w = in->w * scale;
        out->h = in->h * scale;
        out->x = in->x * scale;
        out->y = in->y * scale;

    } else if (coordinates == FSEMU_COORD_1080P_RIGHT) {
        double scale = window_size.h / 1080.0;

        out->w = in->w * scale;
        out->h = in->h * scale;
        out->x = window_size.w - (1920 - in->x) * scale;
        out->y = in->y * scale;
    }
}
*/

#if 1

void fsemu_video_set_frame_swapped(int frame, int64_t swapped_ns)
{
    FSEMU_FRAMEINFO(frame).swapped_ns = swapped_ns;
}

void fsemu_video_set_frame_rendered(int frame, int64_t rendered_ns)
{
    FSEMU_FRAMEINFO(frame).rendered_ns = rendered_ns;
}

// The host refresh interval is the mean of the middle half of the last intervals measured: the
// intervals in the order they came, and sorted (drawing side).
#define FSEMU_VIDEO_VSYNC_INTERVALS 512
static std::deque<int64_t> fsemu_video_vsync_intervals_ns;
static std::vector<int64_t> fsemu_video_vsync_intervals_sorted_ns;

static void fsemu_video_update_vsync_interval(int64_t vsync_interval_ns)
{
    std::deque<int64_t> &intervals = fsemu_video_vsync_intervals_ns;
    std::vector<int64_t> &sorted = fsemu_video_vsync_intervals_sorted_ns;
    if (intervals.size() == FSEMU_VIDEO_VSYNC_INTERVALS) {
        // The oldest interval goes (one of its value from the sorted ones).
        sorted.erase(std::lower_bound(sorted.begin(), sorted.end(), intervals.front()));
        intervals.pop_front();
    }
    intervals.push_back(vsync_interval_ns);
    sorted.insert(std::upper_bound(sorted.begin(), sorted.end(), vsync_interval_ns),
                  vsync_interval_ns);

    // Only consider the middle part of the sorted list to remove noise
    size_t start_index = sorted.size() / 4;
    size_t include_count = sorted.size() / 2;
    int64_t sum_ns = 0;
    for (size_t i = start_index; i < start_index + include_count; i++) {
        sum_ns += sorted[i];
    }

    // FIXME: Calculate from monitor refresh or just use dummy value?
    double vsync_interval_avg_ns =
        include_count == 0 ? 16667000.0 : (double)sum_ns / (double)include_count;
    SDL_LockSpinlock(&fsemu_video.vsync_interval_lock);
    fsemu_video.vsync_interval_avg_ns = vsync_interval_avg_ns;
    SDL_UnlockSpinlock(&fsemu_video.vsync_interval_lock);
}

int64_t fsemu_video_get_vsync_interval_ns(void)
{
    return (int64_t)fsemu_video_get_host_refresh_interval_ns();
}

double fsemu_video_get_host_refresh_interval_ns(void)
{
    SDL_LockSpinlock(&fsemu_video.vsync_interval_lock);
    double interval_ns = fsemu_video.vsync_interval_avg_ns;
    SDL_UnlockSpinlock(&fsemu_video.vsync_interval_lock);
    return interval_ns;
}

int fsemu_video_vsync_frequency(void)
{
    return (int)((double)SDL_NS_PER_SECOND / fsemu_video_get_host_refresh_interval_ns());
}

void fsemu_video_set_frame_vsync(int frame, int64_t vsync_ns, int64_t vsync_predicted_ns)
{
    static int64_t last_vsync_ns = 0;

    // The start gate reads vsync_ns first, as the sign that the frame was swapped, and then the
    // prediction (fsemu_frame_can_start_frame_vsync), so the prediction is written first.
    FSEMU_FRAMEINFO(frame).vsync_predicted_ns = vsync_predicted_ns;
    SDL_MemoryBarrierRelease();
    FSEMU_FRAMEINFO(frame).vsync_ns = vsync_ns;

    FSEMU_FRAMEINFO(frame + 3).vsync_allow_start_ns = 0;
    FSEMU_FRAMEINFO(frame + 2).vsync_allow_start_ns = vsync_ns + SDL_US_TO_NS(9000);
    if (last_vsync_ns > 0) {
        int64_t vsync_interval_ns = vsync_ns - last_vsync_ns;
        // Small hack here; assume we have missed a vblank, so we divide by
        // two in order to try avoiding a feedback effect where emulation
        // length is increased, thereby missing more vblank intervals.
        if (vsync_interval_ns > SDL_MS_TO_NS(30)) {
            vsync_interval_ns = vsync_interval_ns / 2;
        }
        fsemu_video_update_vsync_interval(vsync_interval_ns);
    }
    last_vsync_ns = vsync_ns;
}

#endif

void fsemu_video_end_frame(void)
{
    // FIXME
    // int next_frame = (fsemu_frame_counter() + 1) %
    // FSEMU_VIDEO_MAX_FRAME_STATS;

    // FIXME
    // fsemu_video.stats[next_frame].rendered_at = 0;
    // fsemu_video.stats[next_frame].swapped_at = 0;
}

#if 0
void fsemu_video_fix_right_edge(uint8_t *pixels, int vy, int vw, int vh, int tw, int wh)
{
    if (vw >= tw) {
        if (vw > tw) {
            printf("WARNING: vw > tw\n");
        }
        return;
    }
    // int pitch = tw * 4;
    pixels += (tw * 4) * vy + vw * 4;
    for (int i = vy; i < vh; i++) {
        pixels[4] = pixels[0];
        pixels[5] = pixels[1];
        pixels[6] = pixels[2];
        pixels[7] = pixels[3];
        pixels += tw * 4;
    }
}

void fsemu_video_fix_bottom_edge(uint8_t *pixels, int vw, int vh, int tw, int wh)
{

}

#endif

bool fsemu_video_can_skip_rendering_this_frame(void)
{
    if (fsemu_video.must_render_frame) {
        // printf("must render frame\n");
        return false;
    }
    if ((int64_t)SDL_GetTicksNS() < fsemu_video.must_render_frame_until_ns) {
        printf("now < fsemu_video.must_render_frame_until_ns\n");
        return false;
    }
    // FIXME: Possible check state such as adaptive sync / v-sync / fullscreen
    // window mode / focused / minimized in order to determine whether
    // rendering is necessary.
    printf("can skip!\n");
    return true;
}

void fsemu_video_must_render_frame(void)
{
    fsemu_video.must_render_frame = true;
}

void fsemu_video_must_render_frame_until(int64_t until_ns)
{
    fsemu_video.must_render_frame_until_ns = until_ns;
}

// ----------------------------------------------------------------------------

static void fsemu_video_quit(void)
{
}

// ----------------------------------------------------------------------------

// The options the video setup reads (fsemu_video_read_options).
static struct {
    // Whether they have been read. Written on the main thread only; in render thread mode the
    // drawing side reads it (and the options) only after the host window side has handed it a
    // snapshot written after they were read, under the snapshot slot's lock, so a plain bool.
    bool read;
    bool vsync;
    bool video_sync;
} fsemu_video_options;

void fsemu_video_read_options(void)
{
    if (fsemu_video_options.read) {
        return;
    }
    SDL_assert(SDL_IsMainThread());
    fsemu_video_options.read = true;
    fsemu_layout_init();
    fsemu_frame_init();
    fsemu_screenshot_init();
    fsemu_video_options.vsync = fsemu_option_enabled(FSEMU_OPTION_VSYNC);
    fsemu_video_options.video_sync = fsemu_option_enabled(FSEMU_OPTION_VIDEO_SYNC);
}

void fsemu_video_init(void)
{
    if (FSEMU_MODULE_INIT(video)) {
        return;
    }

    printf("\n\n\nfsemu_video_init\n");

    fsemu_video_read_options();

    // FIXME:
    // fsemu_option_read_bool_default(
    //     FSEMU_OPTION_LOG_VIDEO, &fsemu_video_log_enabled, false);

    fsemu_video_frame_queue = FS_CreateAsyncQueue();
    if (fsemu_video_frame_queue == NULL) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Could not create the video frame queue: %s",
                        SDL_GetError());
        FS_FlushLog();
        abort();
    }

    if (fsemu_video.format == FSEMU_VIDEO_FORMAT_UNKNOWN) {
        fsemu_video_format_t default_format = fsemu_video_default_format();
        int format_flags = fsemu_video_format_flags();
        if (format_flags == 0) {
            fsemu_video_log("No format flags, setting default format\n");
            fsemu_video.format = default_format;
        } else {
            if (format_flags & (1 << default_format)) {
                fsemu_video_log("Default format allowed in format flags\n");
                fsemu_video.format = default_format;
            } else if (format_flags & FSEMU_VIDEO_FORMAT_FLAG_BGRA) {
                fsemu_video_log("Format flags allow BGRA\n");
                fsemu_video.format = FSEMU_VIDEO_FORMAT_BGRA;
            } else if (format_flags & FSEMU_VIDEO_FORMAT_FLAG_RGB565) {
                fsemu_video_log("Format flags allow RGB565\n");
                fsemu_video.format = FSEMU_VIDEO_FORMAT_RGB565;
            }
        }
    }

    // The picture is the emulation surface's streamed content (attached as BGRA): fsemu makes no GL
    // objects of its own.
    if (fsemu_video.format != FSEMU_VIDEO_FORMAT_BGRA) {
        // TODO: RGB565 needs the posting side's 565 path and the content attached as RGB565.
        fsemu_video_log_warning("Only the BGRA video format is shown\n");
    }

    if (fsemu_video_options.vsync) {
        fsemu_video_log("Enabling vsync (via vsync option)\n");
        fsemu_video_set_vsync(true);
    } else if (fsemu_video_options.video_sync) {
        fsemu_video_log("Enabling vsync (via video_sync option)\n");
        fsemu_video_set_vsync(true);
    }

    // The swap interval that the vsync setting asks for is set by fsapp's main loop, which asks
    // fsemu's swap interval hook (fsemu-mainloop.cpp).
}
