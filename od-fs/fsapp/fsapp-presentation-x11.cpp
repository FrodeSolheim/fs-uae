/*
 * Presentation feedback on X11, also under XWayland (see fsapp-presentation.h and
 * fsapp-presentation-backend.h): the Present extension's completion events.
 *
 * The GL driver (Mesa, with DRI3) shows each swap with a PresentPixmap request on the window, and
 * the X server sends a PresentCompleteNotify to every client that selected the window's completion
 * events: when the swap was shown (its UST, in microseconds on CLOCK_MONOTONIC, and the refresh
 * counter, MSC), and how (copied, flipped, or skipped and never shown). fsapp selects them with an
 * event context of its own on SDL's X connection, registered as a special event queue (as Mesa does
 * for its own), so that the events go only to that queue, never to SDL's event loop, and are read
 * here without waiting.
 *
 * The events name the driver's present by its serial, which counts the window's swaps; the first
 * one is matched to the oldest swap asked about, and the serials after it by their difference.
 * Under Xorg the UST is the kernel's vertical blank; under XWayland it is what XWayland makes of
 * the compositor's reports, so only as good as those.
 *
 * Only where the GL driver uses Present: libxcb-present must already be loaded (by the driver).
 * When no event has come after a while, the feedback is turned off. The few xcb declarations used
 * are written out here (as in xcb/present.h), and the libraries are those already loaded.
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "fsapp-presentation-backend.h"

#ifdef __linux__
#define FSAPP_PRESENTATION_X11
#endif

#ifdef FSAPP_PRESENTATION_X11

#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ----------------------------------------------------------------------------
// xcb, Xlib's xcb connection and the Present extension, as loaded.

// The connection, the special event queue and the extension are opaque here.
typedef struct fsapp_xcb_void_cookie {
    unsigned int sequence;
} fsapp_xcb_void_cookie_t;

static struct {
    void *(*get_xcb_connection)(void *display);
    uint32_t (*generate_id)(void *connection);
    void *(*register_for_special_xge)(void *connection, void *extension, uint32_t eid,
                                      uint32_t *stamp);
    void (*unregister_for_special_event)(void *connection, void *special);
    void *(*poll_for_special_event)(void *connection, void *special);
    int (*flush)(void *connection);
    fsapp_xcb_void_cookie_t (*present_select_input)(void *connection, uint32_t eid, uint32_t window,
                                                    uint32_t event_mask);
    void *present_id;
} xcb;

// The Present extension's values used (present.xml): the completion event's type and mask, and the
// completion's kinds.
#define FSAPP_PRESENTATION_XCB_GE_GENERIC 35
#define FSAPP_PRESENTATION_PRESENT_COMPLETE_NOTIFY 1
#define FSAPP_PRESENTATION_PRESENT_EVENT_MASK_COMPLETE_NOTIFY 2
#define FSAPP_PRESENTATION_PRESENT_COMPLETE_KIND_PIXMAP 0

// Where a PresentCompleteNotify's fields are, in the event as xcb gives it (the 32 bytes of the
// generic event, xcb's full sequence number, then the rest; xcb_present_complete_notify_event_t is
// packed).
#define FSAPP_PRESENTATION_EVENT_TYPE_OFFSET 8
#define FSAPP_PRESENTATION_KIND_OFFSET 10
#define FSAPP_PRESENTATION_MODE_OFFSET 11
#define FSAPP_PRESENTATION_SERIAL_OFFSET 20
#define FSAPP_PRESENTATION_UST_OFFSET 24
#define FSAPP_PRESENTATION_MSC_OFFSET 36

// Opens a library: the copy already loaded, or with load, loads it if it is not.
static void *fsapp_presentation_x11_open(const char *name, bool load)
{
    void *library = dlopen(name, RTLD_LAZY | RTLD_NOLOAD);
    if (library == NULL && load) {
        library = dlopen(name, RTLD_LAZY);
    }
    return library;
}

static bool fsapp_presentation_x11_load(void)
{
    // libX11-xcb is a small part of Xlib that the GL driver usually has loaded already; libxcb is
    // loaded with Xlib. libxcb-present is loaded only by a GL driver that presents with Present.
    void *x11_xcb = fsapp_presentation_x11_open("libX11-xcb.so.1", true);
    void *libxcb = fsapp_presentation_x11_open("libxcb.so.1", false);
    void *present = fsapp_presentation_x11_open("libxcb-present.so.0", false);
    if (x11_xcb == NULL || libxcb == NULL) {
        SDL_Log("Presentation feedback: none (libX11-xcb or libxcb is not loaded)");
        return false;
    }
    if (present == NULL) {
        SDL_Log("Presentation feedback: none (libxcb-present is not loaded: the GL driver does "
                "not present with the Present extension)");
        return false;
    }
    struct {
        void *library;
        const char *name;
        void **address;
    } symbols[] = {
        {x11_xcb, "XGetXCBConnection", (void **)&xcb.get_xcb_connection},
        {libxcb, "xcb_generate_id", (void **)&xcb.generate_id},
        {libxcb, "xcb_register_for_special_xge", (void **)&xcb.register_for_special_xge},
        {libxcb, "xcb_unregister_for_special_event", (void **)&xcb.unregister_for_special_event},
        {libxcb, "xcb_poll_for_special_event", (void **)&xcb.poll_for_special_event},
        {libxcb, "xcb_flush", (void **)&xcb.flush},
        {present, "xcb_present_select_input", (void **)&xcb.present_select_input},
        {present, "xcb_present_id", &xcb.present_id},
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
// The X11 state.

// How many swaps can wait for their completion; the oldest is dropped when more are asked about.
#define FSAPP_PRESENTATION_X11_PENDING_COUNT 64

// The swaps after which, with no completion event at all, the feedback is turned off (the GL
// driver does not present this window with Present).
#define FSAPP_PRESENTATION_X11_SILENT_SWAPS 120

static struct {
    void *connection;
    uint32_t window;
    uint32_t eid;
    void *special;
    uint32_t stamp;
    // The swaps asked about and not yet reported, oldest first from first (wrapping).
    int64_t pending[FSAPP_PRESENTATION_X11_PENDING_COUNT];
    int pending_first;
    int pending_count;
    // Whether a completion has been matched to a swap, and what to add to a serial to get its swap.
    bool matched;
    int64_t serial_offset;
    // Swaps asked about, and whether the feedback was turned off for want of events.
    int64_t requests;
    bool silent;
} x11;

static bool fsapp_presentation_x11_init(SDL_Window *window)
{
    SDL_PropertiesID properties = SDL_GetWindowProperties(window);
    void *display = SDL_GetPointerProperty(properties, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, NULL);
    x11.window = (uint32_t)SDL_GetNumberProperty(properties, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
    if (display == NULL || x11.window == 0) {
        SDL_Log("Presentation feedback: none (the window has no X11 display or window)");
        return false;
    }
    if (!fsapp_presentation_x11_load()) {
        return false;
    }
    x11.connection = xcb.get_xcb_connection(display);
    if (x11.connection == NULL) {
        SDL_Log("Presentation feedback: none (no xcb connection)");
        return false;
    }
    x11.eid = xcb.generate_id(x11.connection);
    x11.special = xcb.register_for_special_xge(x11.connection, xcb.present_id, x11.eid, &x11.stamp);
    if (x11.special == NULL) {
        SDL_Log("Presentation feedback: none (the X server has no Present extension)");
        return false;
    }
    xcb.present_select_input(x11.connection, x11.eid, x11.window,
                             FSAPP_PRESENTATION_PRESENT_EVENT_MASK_COMPLETE_NOTIFY);
    xcb.flush(x11.connection);
    SDL_Log("Presentation feedback: X11 Present (window 0x%x)", (unsigned int)x11.window);
    return true;
}

static void fsapp_presentation_x11_quit(void)
{
    if (x11.special != NULL) {
        xcb.present_select_input(x11.connection, x11.eid, x11.window, 0);
        xcb.unregister_for_special_event(x11.connection, x11.special);
        xcb.flush(x11.connection);
    }
    memset(&x11, 0, sizeof(x11));
}

static void fsapp_presentation_x11_request(SDL_Window *window, int64_t swap_number)
{
    (void)window;
    if (x11.silent) {
        return;
    }
    x11.requests++;
    if (!x11.matched && x11.requests > FSAPP_PRESENTATION_X11_SILENT_SWAPS) {
        SDL_Log("Presentation feedback: no X11 Present completion after %d swaps (the GL driver "
                "does not present this window with Present), turned off",
                FSAPP_PRESENTATION_X11_SILENT_SWAPS);
        x11.silent = true;
        x11.pending_count = 0;
        return;
    }
    if (x11.pending_count == FSAPP_PRESENTATION_X11_PENDING_COUNT) {
        x11.pending_first = (x11.pending_first + 1) % FSAPP_PRESENTATION_X11_PENDING_COUNT;
        x11.pending_count--;
    }
    int i = (x11.pending_first + x11.pending_count) % FSAPP_PRESENTATION_X11_PENDING_COUNT;
    x11.pending[i] = swap_number;
    x11.pending_count++;
}

// A completion of the driver's present with this serial: matched to its swap, which is reported
// (and the swaps asked about before it, which got no completion, are dropped).
static void fsapp_presentation_x11_on_complete(uint32_t serial, int mode, uint64_t ust,
                                               uint64_t msc)
{
    if (x11.pending_count == 0) {
        return;
    }
    if (!x11.matched) {
        x11.serial_offset = x11.pending[x11.pending_first] - (int64_t)serial;
        x11.matched = true;
    }
    int64_t swap_number = (int64_t)serial + x11.serial_offset;
    bool found = false;
    while (x11.pending_count > 0 && x11.pending[x11.pending_first] <= swap_number) {
        found = found || x11.pending[x11.pending_first] == swap_number;
        x11.pending_first = (x11.pending_first + 1) % FSAPP_PRESENTATION_X11_PENDING_COUNT;
        x11.pending_count--;
    }
    if (!found) {
        return;
    }
    fsapp_frame_presented_t presented = {};
    presented.swap_number = swap_number;
    if (mode != FSAPP_FRAME_PRESENTED_MODE_SKIP) {
        presented.presented_ns =
            fsapp_presentation_to_sdl_clock(CLOCK_MONOTONIC, (int64_t)ust * 1000);
    }
    presented.sequence = msc;
    presented.mode = mode;
    fsapp_presentation_add_done(&presented);
}

static void fsapp_presentation_x11_poll(void)
{
    if (x11.silent) {
        return;
    }
    uint8_t *event;
    while ((event = (uint8_t *)xcb.poll_for_special_event(x11.connection, x11.special)) != NULL) {
        uint16_t event_type;
        memcpy(&event_type, event + FSAPP_PRESENTATION_EVENT_TYPE_OFFSET, sizeof(event_type));
        if ((event[0] & 0x7f) == FSAPP_PRESENTATION_XCB_GE_GENERIC &&
            event_type == FSAPP_PRESENTATION_PRESENT_COMPLETE_NOTIFY &&
            event[FSAPP_PRESENTATION_KIND_OFFSET] ==
                FSAPP_PRESENTATION_PRESENT_COMPLETE_KIND_PIXMAP) {
            uint32_t serial;
            uint64_t ust, msc;
            memcpy(&serial, event + FSAPP_PRESENTATION_SERIAL_OFFSET, sizeof(serial));
            memcpy(&ust, event + FSAPP_PRESENTATION_UST_OFFSET, sizeof(ust));
            memcpy(&msc, event + FSAPP_PRESENTATION_MSC_OFFSET, sizeof(msc));
            fsapp_presentation_x11_on_complete(serial, event[FSAPP_PRESENTATION_MODE_OFFSET], ust,
                                               msc);
        }
        free(event);
    }
}

static int fsapp_presentation_x11_get_waiting(void)
{
    return x11.silent ? -1 : x11.pending_count;
}

static const fsapp_presentation_backend_t fsapp_presentation_x11_backend = {
    fsapp_presentation_x11_init,        fsapp_presentation_x11_request, fsapp_presentation_x11_poll,
    fsapp_presentation_x11_get_waiting, fsapp_presentation_x11_quit,
};

const fsapp_presentation_backend_t *fsapp_presentation_get_x11_backend(void)
{
    return &fsapp_presentation_x11_backend;
}

#else

const fsapp_presentation_backend_t *fsapp_presentation_get_x11_backend(void)
{
    return NULL;
}

#endif // FSAPP_PRESENTATION_X11
