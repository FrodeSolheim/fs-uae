/*
 * The display's refreshes on macOS (see fsapp-presentation.h and fsapp-presentation-backend.h),
 * from CVDisplayLink.
 *
 * macOS reports no presentation for OpenGL: neither NSOpenGLContext nor CGL says when a swap was
 * shown (only Metal's drawables do). What it reports is the display's refreshes. A display link
 * calls back once per refresh, on a thread of its own, with the time of the refresh (the hardware's
 * vertical blank, in host time: mach_absolute_time's units), the refresh period, and when it
 * expects a frame drawn now to be shown (its output time). This backend reports those refreshes,
 * not presentations.
 *
 * SDL's cocoa driver makes the swap interval with a display link of its own: the swap waits for
 * that link's next callback, then flushes. SDL sets its link to the display of the GL context's
 * renderer (CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext). This link is set the same way, so
 * it reports the refreshes that SDL's swaps wait for.
 *
 * The callback queues each refresh under a mutex; poll converts them to SDL's clock and adds them.
 * CoreVideo and CGL (both deprecated, CVDisplayLink since macOS 15 in favour of CADisplayLink,
 * which needs macOS 14) are called through dlsym, so their deprecation gives no warnings; only
 * CoreVideo's types come from its headers.
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "fsapp-presentation-backend.h"

#ifdef __APPLE__
#define FSAPP_PRESENTATION_MACOS
#endif

#ifdef FSAPP_PRESENTATION_MACOS

#include <CoreVideo/CVBase.h>
#include <CoreVideo/CVReturn.h>
#include <dlfcn.h>
#include <mach/mach_time.h>
#include <string.h>
#include <time.h>

// ----------------------------------------------------------------------------
// CoreVideo's display link and CGL, as loaded.

// The display link, the GL context and its pixel format are opaque here (CVDisplayLinkRef,
// CGLContextObj, CGLPixelFormatObj); a display is a CGDirectDisplayID.
typedef void *fsapp_cv_display_link_t;
typedef CVReturn (*fsapp_cv_output_callback_t)(fsapp_cv_display_link_t link, const CVTimeStamp *now,
                                               const CVTimeStamp *output_time,
                                               CVOptionFlags flags_in, CVOptionFlags *flags_out,
                                               void *context);

static struct {
    CVReturn (*create_with_active_cg_displays)(fsapp_cv_display_link_t *link);
    CVReturn (*set_output_callback)(fsapp_cv_display_link_t link,
                                    fsapp_cv_output_callback_t callback, void *context);
    CVReturn (*set_current_cg_display_from_opengl_context)(fsapp_cv_display_link_t link,
                                                           void *context, void *pixel_format);
    uint32_t (*get_current_cg_display)(fsapp_cv_display_link_t link);
    CVReturn (*start)(fsapp_cv_display_link_t link);
    CVReturn (*stop)(fsapp_cv_display_link_t link);
    void (*release)(fsapp_cv_display_link_t link);
    void *(*cgl_get_current_context)(void);
    void *(*cgl_get_pixel_format)(void *context);
} cv;

#define FSAPP_PRESENTATION_MACOS_CORE_VIDEO                                                        \
    "/System/Library/Frameworks/CoreVideo.framework/CoreVideo"
#define FSAPP_PRESENTATION_MACOS_OPENGL "/System/Library/Frameworks/OpenGL.framework/OpenGL"

static bool fsapp_presentation_macos_load(void)
{
    // System frameworks: SDL (CoreVideo) and FS-UAE (OpenGL) have them loaded already.
    void *core_video = dlopen(FSAPP_PRESENTATION_MACOS_CORE_VIDEO, RTLD_LAZY);
    void *opengl = dlopen(FSAPP_PRESENTATION_MACOS_OPENGL, RTLD_LAZY);
    if (core_video == NULL || opengl == NULL) {
        SDL_Log("Presentation feedback: none (CoreVideo or OpenGL could not be loaded)");
        return false;
    }
    struct {
        void *library;
        const char *name;
        void **address;
    } symbols[] = {
        {core_video, "CVDisplayLinkCreateWithActiveCGDisplays",
         (void **)&cv.create_with_active_cg_displays},
        {core_video, "CVDisplayLinkSetOutputCallback", (void **)&cv.set_output_callback},
        {core_video, "CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext",
         (void **)&cv.set_current_cg_display_from_opengl_context},
        {core_video, "CVDisplayLinkGetCurrentCGDisplay", (void **)&cv.get_current_cg_display},
        {core_video, "CVDisplayLinkStart", (void **)&cv.start},
        {core_video, "CVDisplayLinkStop", (void **)&cv.stop},
        {core_video, "CVDisplayLinkRelease", (void **)&cv.release},
        {opengl, "CGLGetCurrentContext", (void **)&cv.cgl_get_current_context},
        {opengl, "CGLGetPixelFormat", (void **)&cv.cgl_get_pixel_format},
    };
    for (size_t i = 0; i < SDL_arraysize(symbols); i++) {
        *symbols[i].address = dlsym(symbols[i].library, symbols[i].name);
        if (*symbols[i].address == NULL) {
            SDL_Log("Presentation feedback: none (%s is missing)", symbols[i].name);
            return false;
        }
    }
    // The references taken here are kept, for the functions found.
    return true;
}

// ----------------------------------------------------------------------------
// The refreshes the display link's thread has queued.

// How many refreshes can wait for poll; the oldest is dropped when more come.
#define FSAPP_PRESENTATION_MACOS_QUEUE_COUNT 32

// A refresh as the callback queues it: host times (0 when not valid) and the period.
typedef struct fsapp_presentation_macos_refresh {
    uint64_t refresh_host;
    uint64_t output_host;
    int64_t period_ns;
} fsapp_presentation_macos_refresh_t;

// Shared with the display link's thread, under the mutex. The mutex is made once and kept, since a
// callback can still be running while the link is stopped; while not running, the callback queues
// nothing.
static struct {
    SDL_Mutex *mutex;
    bool running;
    fsapp_presentation_macos_refresh_t refreshes[FSAPP_PRESENTATION_MACOS_QUEUE_COUNT];
    int first;
    int count;
} queue;

// On the display link's thread: queues the refresh.
static CVReturn fsapp_presentation_macos_callback(fsapp_cv_display_link_t link,
                                                  const CVTimeStamp *now,
                                                  const CVTimeStamp *output_time,
                                                  CVOptionFlags flags_in, CVOptionFlags *flags_out,
                                                  void *context)
{
    (void)link;
    (void)flags_in;
    (void)flags_out;
    (void)context;
    if ((now->flags & kCVTimeStampHostTimeValid) == 0) {
        return kCVReturnSuccess;
    }
    fsapp_presentation_macos_refresh_t refresh = {};
    refresh.refresh_host = now->hostTime;
    if ((output_time->flags & kCVTimeStampHostTimeValid) != 0) {
        refresh.output_host = output_time->hostTime;
    }
    if ((now->flags & kCVTimeStampVideoRefreshPeriodValid) != 0 && now->videoTimeScale > 0) {
        refresh.period_ns = (int64_t)SDL_round((double)now->videoRefreshPeriod * SDL_NS_PER_SECOND /
                                               now->videoTimeScale);
    }
    SDL_LockMutex(queue.mutex);
    if (queue.running) {
        if (queue.count == FSAPP_PRESENTATION_MACOS_QUEUE_COUNT) {
            queue.first = (queue.first + 1) % FSAPP_PRESENTATION_MACOS_QUEUE_COUNT;
            queue.count--;
        }
        queue.refreshes[(queue.first + queue.count) % FSAPP_PRESENTATION_MACOS_QUEUE_COUNT] =
            refresh;
        queue.count++;
    }
    SDL_UnlockMutex(queue.mutex);
    return kCVReturnSuccess;
}

// ----------------------------------------------------------------------------
// The macOS state (the drawing side's).

// How often (in swaps) the link is set to the GL context's display again, to follow the window to
// another display (as SDL's own link does when the window moves to another screen).
#define FSAPP_PRESENTATION_MACOS_DISPLAY_CHECK_SWAPS 120

static struct {
    fsapp_cv_display_link_t link;
    uint32_t display;
    // Swaps since the display was last checked.
    int swaps;
    // Host time to nanoseconds: times numer, divided by denom.
    mach_timebase_info_data_t timebase;
} macos;

// Sets the link to the display of the current GL context's renderer, as SDL sets its own.
static void fsapp_presentation_macos_follow_context(void *context)
{
    cv.set_current_cg_display_from_opengl_context(macos.link, context,
                                                  cv.cgl_get_pixel_format(context));
}

static bool fsapp_presentation_macos_init(SDL_Window *window)
{
    (void)window;
    if (!fsapp_presentation_macos_load()) {
        return false;
    }
    // Called just before a swap, with the window's GL context current.
    void *context = cv.cgl_get_current_context();
    if (context == NULL) {
        SDL_Log("Presentation feedback: none (no current OpenGL context)");
        return false;
    }
    if (queue.mutex == NULL) {
        queue.mutex = SDL_CreateMutex();
        if (queue.mutex == NULL) {
            SDL_Log("Presentation feedback: none (%s)", SDL_GetError());
            return false;
        }
    }
    if (mach_timebase_info(&macos.timebase) != KERN_SUCCESS || macos.timebase.denom == 0) {
        SDL_Log("Presentation feedback: none (no host time base)");
        return false;
    }
    if (cv.create_with_active_cg_displays(&macos.link) != kCVReturnSuccess || macos.link == NULL) {
        macos.link = NULL;
        SDL_Log("Presentation feedback: none (no display link)");
        return false;
    }
    cv.set_output_callback(macos.link, fsapp_presentation_macos_callback, NULL);
    fsapp_presentation_macos_follow_context(context);
    SDL_LockMutex(queue.mutex);
    queue.running = true;
    queue.first = 0;
    queue.count = 0;
    SDL_UnlockMutex(queue.mutex);
    if (cv.start(macos.link) != kCVReturnSuccess) {
        SDL_Log("Presentation feedback: none (the display link did not start)");
        return false;
    }
    macos.display = cv.get_current_cg_display(macos.link);
    SDL_Log("Presentation feedback: none for OpenGL on macOS; the display's refreshes instead, "
            "from CVDisplayLink (display %u)",
            (unsigned int)macos.display);
    return true;
}

static void fsapp_presentation_macos_quit(void)
{
    if (macos.link != NULL) {
        cv.stop(macos.link);
        cv.release(macos.link);
    }
    if (queue.mutex != NULL) {
        SDL_LockMutex(queue.mutex);
        queue.running = false;
        queue.count = 0;
        SDL_UnlockMutex(queue.mutex);
    }
    memset(&macos, 0, sizeof(macos));
}

static void fsapp_presentation_macos_request(SDL_Window *window, int64_t swap_number)
{
    (void)window;
    (void)swap_number;
    if (++macos.swaps < FSAPP_PRESENTATION_MACOS_DISPLAY_CHECK_SWAPS) {
        return;
    }
    macos.swaps = 0;
    void *context = cv.cgl_get_current_context();
    if (context == NULL) {
        return;
    }
    // REVIEW: SDL sets its link again only when the window moves to another screen; this sets it
    // every DISPLAY_CHECK_SWAPS swaps, assuming that setting the same display again changes
    // nothing.
    fsapp_presentation_macos_follow_context(context);
    uint32_t display = cv.get_current_cg_display(macos.link);
    if (display != macos.display) {
        SDL_Log("Presentation feedback: the display's refreshes now from display %u",
                (unsigned int)display);
        macos.display = display;
    }
}

// A host time in nanoseconds on SDL's clock (0 for 0). Host time is mach_absolute_time's, which
// CLOCK_UPTIME_RAW gives in nanoseconds.
static int64_t fsapp_presentation_macos_to_sdl_clock(uint64_t host)
{
    if (host == 0) {
        return 0;
    }
    int64_t ns = (int64_t)((unsigned __int128)host * macos.timebase.numer / macos.timebase.denom);
    return fsapp_presentation_to_sdl_clock(CLOCK_UPTIME_RAW, ns);
}

static void fsapp_presentation_macos_poll(void)
{
    fsapp_presentation_macos_refresh_t refreshes[FSAPP_PRESENTATION_MACOS_QUEUE_COUNT];
    SDL_LockMutex(queue.mutex);
    int count = queue.count;
    for (int i = 0; i < count; i++) {
        refreshes[i] = queue.refreshes[(queue.first + i) % FSAPP_PRESENTATION_MACOS_QUEUE_COUNT];
    }
    queue.first = 0;
    queue.count = 0;
    SDL_UnlockMutex(queue.mutex);
    for (int i = 0; i < count; i++) {
        fsapp_frame_display_refresh_t refresh = {};
        refresh.refresh_ns = fsapp_presentation_macos_to_sdl_clock(refreshes[i].refresh_host);
        refresh.output_ns = fsapp_presentation_macos_to_sdl_clock(refreshes[i].output_host);
        refresh.period_ns = refreshes[i].period_ns;
        if (refresh.refresh_ns != 0) {
            fsapp_presentation_add_display_refresh(&refresh);
        }
    }
}

// The swaps' presentations are not reported.
static int fsapp_presentation_macos_get_waiting(void)
{
    return -1;
}

static const fsapp_presentation_backend_t fsapp_presentation_macos_backend = {
    fsapp_presentation_macos_init, fsapp_presentation_macos_request,
    fsapp_presentation_macos_poll, fsapp_presentation_macos_get_waiting,
    fsapp_presentation_macos_quit,
};

const fsapp_presentation_backend_t *fsapp_presentation_get_macos_backend(void)
{
    return &fsapp_presentation_macos_backend;
}

#else

const fsapp_presentation_backend_t *fsapp_presentation_get_macos_backend(void)
{
    return NULL;
}

#endif // FSAPP_PRESENTATION_MACOS
