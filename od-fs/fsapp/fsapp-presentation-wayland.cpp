/*
 * Presentation feedback on Wayland (see fsapp-presentation.h and fsapp-presentation-backend.h):
 * the presentation-time protocol (wp_presentation).
 *
 * The wp_presentation global is bound on an event queue of fsapp's own, so its events and those of
 * the feedback objects made from it are dispatched only here, on the drawing side, and never by
 * SDL. Each poll reads what the connection has without waiting (the usual prepare, read or cancel
 * sequence, safe alongside SDL's own reads), then dispatches the queue. A presentation's time is on
 * the clock the compositor names (wp_presentation.clock_id, usually CLOCK_MONOTONIC). The
 * protocol's two interfaces are written out here as wayland-scanner would write them, and
 * libwayland-client is the copy SDL loaded.
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "fsapp-presentation-backend.h"

#if defined(__linux__) && __has_include(<wayland-client-core.h>)
#define FSAPP_PRESENTATION_WAYLAND
#endif

#ifdef FSAPP_PRESENTATION_WAYLAND

#include <dlfcn.h>
#include <poll.h>
#include <wayland-client-core.h>

// ----------------------------------------------------------------------------
// libwayland-client, as SDL loaded it.

static struct {
    struct wl_proxy *(*proxy_marshal_flags)(struct wl_proxy *proxy, uint32_t opcode,
                                            const struct wl_interface *interface, uint32_t version,
                                            uint32_t flags, ...);
    int (*proxy_add_listener)(struct wl_proxy *proxy, void (**implementation)(void), void *data);
    void (*proxy_destroy)(struct wl_proxy *proxy);
    uint32_t (*proxy_get_version)(struct wl_proxy *proxy);
    void *(*proxy_create_wrapper)(void *proxy);
    void (*proxy_wrapper_destroy)(void *proxy_wrapper);
    struct wl_event_queue *(*display_create_queue)(struct wl_display *display);
    void (*event_queue_destroy)(struct wl_event_queue *queue);
    void (*proxy_set_queue)(struct wl_proxy *proxy, struct wl_event_queue *queue);
    int (*display_roundtrip_queue)(struct wl_display *display, struct wl_event_queue *queue);
    int (*display_dispatch_queue_pending)(struct wl_display *display, struct wl_event_queue *queue);
    int (*display_prepare_read_queue)(struct wl_display *display, struct wl_event_queue *queue);
    int (*display_read_events)(struct wl_display *display);
    void (*display_cancel_read)(struct wl_display *display);
    int (*display_get_fd)(struct wl_display *display);
    const struct wl_interface *registry_interface;
    const struct wl_interface *surface_interface;
    const struct wl_interface *output_interface;
} wl;

// Finds libwayland-client's functions and interfaces in the copy already loaded (by SDL), and
// returns false when it is not loaded or one is missing (wl_proxy_marshal_flags is from 1.20).
static bool fsapp_presentation_load_wayland(void)
{
    void *library = dlopen("libwayland-client.so.0", RTLD_LAZY | RTLD_NOLOAD);
    if (library == NULL) {
        SDL_Log("Presentation feedback: none (libwayland-client is not loaded)");
        return false;
    }
    struct {
        const char *name;
        void **address;
    } symbols[] = {
        {"wl_proxy_marshal_flags", (void **)&wl.proxy_marshal_flags},
        {"wl_proxy_add_listener", (void **)&wl.proxy_add_listener},
        {"wl_proxy_destroy", (void **)&wl.proxy_destroy},
        {"wl_proxy_get_version", (void **)&wl.proxy_get_version},
        {"wl_proxy_create_wrapper", (void **)&wl.proxy_create_wrapper},
        {"wl_proxy_wrapper_destroy", (void **)&wl.proxy_wrapper_destroy},
        {"wl_display_create_queue", (void **)&wl.display_create_queue},
        {"wl_event_queue_destroy", (void **)&wl.event_queue_destroy},
        {"wl_proxy_set_queue", (void **)&wl.proxy_set_queue},
        {"wl_display_roundtrip_queue", (void **)&wl.display_roundtrip_queue},
        {"wl_display_dispatch_queue_pending", (void **)&wl.display_dispatch_queue_pending},
        {"wl_display_prepare_read_queue", (void **)&wl.display_prepare_read_queue},
        {"wl_display_read_events", (void **)&wl.display_read_events},
        {"wl_display_cancel_read", (void **)&wl.display_cancel_read},
        {"wl_display_get_fd", (void **)&wl.display_get_fd},
        {"wl_registry_interface", (void **)&wl.registry_interface},
        {"wl_surface_interface", (void **)&wl.surface_interface},
        {"wl_output_interface", (void **)&wl.output_interface},
    };
    for (size_t i = 0; i < SDL_arraysize(symbols); i++) {
        *symbols[i].address = dlsym(library, symbols[i].name);
        if (*symbols[i].address == NULL) {
            SDL_Log("Presentation feedback: none (libwayland-client has no %s)", symbols[i].name);
            // The library stays loaded by SDL; this only drops the reference taken here.
            dlclose(library);
            return false;
        }
    }
    // SDL holds the library for as long as the connection exists; the reference taken here is
    // kept too, for the functions found.
    return true;
}

// ----------------------------------------------------------------------------
// The presentation-time protocol's interfaces (stable/presentation-time/presentation-time.xml in
// wayland-protocols), as wayland-scanner would write them, with the interfaces of libwayland-client
// that they refer to filled in when it is loaded.

// The argument types of the messages: none for the integers, then the feedback request's surface
// and new feedback object, and the sync_output event's output.
static const struct wl_interface *fsapp_presentation_types[10];

static const struct wl_message fsapp_presentation_requests[] = {
    {"destroy", "", fsapp_presentation_types + 0},
    {"feedback", "on", fsapp_presentation_types + 7},
};

static const struct wl_message fsapp_presentation_events[] = {
    {"clock_id", "u", fsapp_presentation_types + 0},
};

static const struct wl_interface fsapp_presentation_interface = {
    "wp_presentation", 1, 2, fsapp_presentation_requests, 1, fsapp_presentation_events,
};

static const struct wl_message fsapp_presentation_feedback_events[] = {
    {"sync_output", "o", fsapp_presentation_types + 9},
    {"presented", "uuuuuuu", fsapp_presentation_types + 0},
    {"discarded", "", fsapp_presentation_types + 0},
};

static const struct wl_interface fsapp_presentation_feedback_interface = {
    "wp_presentation_feedback", 1, 0, NULL, 3, fsapp_presentation_feedback_events,
};

// The opcodes used: wl_display.get_registry, wl_registry.bind, wp_presentation.destroy and
// wp_presentation.feedback.
#define FSAPP_PRESENTATION_WL_DISPLAY_GET_REGISTRY 1
#define FSAPP_PRESENTATION_WL_REGISTRY_BIND 0
#define FSAPP_PRESENTATION_DESTROY 0
#define FSAPP_PRESENTATION_FEEDBACK 1

// ----------------------------------------------------------------------------
// The Wayland state.

// How many feedback objects can wait for their event; a swap is left without feedback when all
// are waiting (the compositor reports each within a few refreshes).
#define FSAPP_PRESENTATION_PENDING_COUNT 16

typedef struct fsapp_presentation_pending_t {
    struct wl_proxy *feedback;
    int64_t swap_number;
} fsapp_presentation_pending_t;

static struct {
    struct wl_display *display;
    struct wl_event_queue *queue;
    struct wl_proxy *presentation;
    // The clock of the presentation times (clockid_t), -1 until the compositor has named it.
    int clock_id;
    // The window the surface is from, and its wl_surface.
    SDL_Window *window;
    struct wl_proxy *surface;
    fsapp_presentation_pending_t pending[FSAPP_PRESENTATION_PENDING_COUNT];
    // Requests left without feedback since the last log of it (all feedback objects were waiting).
    int64_t skipped;
} wayland = {NULL, NULL, NULL, -1};

static void fsapp_presentation_on_global(void *data, struct wl_proxy *registry, uint32_t name,
                                         const char *interface, uint32_t version)
{
    (void)data;
    if (SDL_strcmp(interface, "wp_presentation") == 0 && wayland.presentation == NULL) {
        uint32_t bound = SDL_min(version, (uint32_t)1);
        wayland.presentation = wl.proxy_marshal_flags(
            registry, FSAPP_PRESENTATION_WL_REGISTRY_BIND, &fsapp_presentation_interface, bound, 0,
            name, fsapp_presentation_interface.name, bound, NULL);
    }
}

static void fsapp_presentation_on_global_remove(void *data, struct wl_proxy *registry,
                                                uint32_t name)
{
    (void)data;
    (void)registry;
    (void)name;
}

static const struct {
    void (*global)(void *data, struct wl_proxy *registry, uint32_t name, const char *interface,
                   uint32_t version);
    void (*global_remove)(void *data, struct wl_proxy *registry, uint32_t name);
} fsapp_presentation_registry_listener = {
    fsapp_presentation_on_global,
    fsapp_presentation_on_global_remove,
};

static void fsapp_presentation_on_clock_id(void *data, struct wl_proxy *presentation,
                                           uint32_t clock_id)
{
    (void)data;
    (void)presentation;
    wayland.clock_id = (int)clock_id;
}

static const struct {
    void (*clock_id)(void *data, struct wl_proxy *presentation, uint32_t clock_id);
} fsapp_presentation_listener = {
    fsapp_presentation_on_clock_id,
};

// Ends a pending feedback (its object is gone after its presented or discarded event).
static void fsapp_presentation_end_pending(fsapp_presentation_pending_t *pending)
{
    wl.proxy_destroy(pending->feedback);
    pending->feedback = NULL;
}

static void fsapp_presentation_on_sync_output(void *data, struct wl_proxy *feedback,
                                              struct wl_proxy *output)
{
    (void)data;
    (void)feedback;
    (void)output;
}

static void fsapp_presentation_on_presented(void *data, struct wl_proxy *feedback,
                                            uint32_t tv_sec_hi, uint32_t tv_sec_lo,
                                            uint32_t tv_nsec, uint32_t refresh, uint32_t seq_hi,
                                            uint32_t seq_lo, uint32_t flags)
{
    (void)feedback;
    fsapp_presentation_pending_t *pending = (fsapp_presentation_pending_t *)data;
    int64_t seconds = (int64_t)(((uint64_t)tv_sec_hi << 32) | tv_sec_lo);
    fsapp_frame_presented_t presented = {};
    presented.swap_number = pending->swap_number;
    presented.presented_ns = fsapp_presentation_to_sdl_clock(
        wayland.clock_id, seconds * SDL_NS_PER_SECOND + (int64_t)tv_nsec);
    presented.refresh_ns = refresh;
    presented.sequence = ((uint64_t)seq_hi << 32) | seq_lo;
    presented.flags = flags;
    presented.mode = FSAPP_FRAME_PRESENTED_MODE_NONE;
    fsapp_presentation_add_done(&presented);
    fsapp_presentation_end_pending(pending);
}

static void fsapp_presentation_on_discarded(void *data, struct wl_proxy *feedback)
{
    (void)feedback;
    fsapp_presentation_pending_t *pending = (fsapp_presentation_pending_t *)data;
    fsapp_frame_presented_t presented = {};
    presented.swap_number = pending->swap_number;
    presented.mode = FSAPP_FRAME_PRESENTED_MODE_NONE;
    fsapp_presentation_add_done(&presented);
    fsapp_presentation_end_pending(pending);
}

static const struct {
    void (*sync_output)(void *data, struct wl_proxy *feedback, struct wl_proxy *output);
    void (*presented)(void *data, struct wl_proxy *feedback, uint32_t tv_sec_hi, uint32_t tv_sec_lo,
                      uint32_t tv_nsec, uint32_t refresh, uint32_t seq_hi, uint32_t seq_lo,
                      uint32_t flags);
    void (*discarded)(void *data, struct wl_proxy *feedback);
} fsapp_presentation_feedback_listener = {
    fsapp_presentation_on_sync_output,
    fsapp_presentation_on_presented,
    fsapp_presentation_on_discarded,
};

// Binds wp_presentation on a queue of its own, from the window's Wayland display, and waits for
// its clock. Returns false (and logs why) when the compositor does not have it.
static bool fsapp_presentation_wayland_init(SDL_Window *window)
{
    SDL_PropertiesID properties = SDL_GetWindowProperties(window);
    wayland.display = (struct wl_display *)SDL_GetPointerProperty(
        properties, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, NULL);
    if (wayland.display == NULL) {
        SDL_Log("Presentation feedback: none (the window has no Wayland display)");
        return false;
    }
    if (!fsapp_presentation_load_wayland()) {
        return false;
    }
    fsapp_presentation_types[7] = wl.surface_interface;
    fsapp_presentation_types[8] = &fsapp_presentation_feedback_interface;
    fsapp_presentation_types[9] = wl.output_interface;

    wayland.queue = wl.display_create_queue(wayland.display);
    if (wayland.queue == NULL) {
        SDL_Log("Presentation feedback: none (could not create a Wayland event queue)");
        return false;
    }
    // The registry is made from a wrapper of the display on the queue, so that its events (and
    // those of the objects bound from it) go to the queue.
    struct wl_proxy *display = (struct wl_proxy *)wl.proxy_create_wrapper((void *)wayland.display);
    wl.proxy_set_queue(display, wayland.queue);
    struct wl_proxy *registry =
        wl.proxy_marshal_flags(display, FSAPP_PRESENTATION_WL_DISPLAY_GET_REGISTRY,
                               wl.registry_interface, wl.proxy_get_version(display), 0, NULL);
    wl.proxy_wrapper_destroy(display);
    wl.proxy_add_listener(registry, (void (**)(void))&fsapp_presentation_registry_listener, NULL);
    // The globals, then the presentation's clock.
    wl.display_roundtrip_queue(wayland.display, wayland.queue);
    wl.proxy_destroy(registry);
    if (wayland.presentation == NULL) {
        SDL_Log("Presentation feedback: none (the compositor has no wp_presentation)");
        return false;
    }
    wl.proxy_add_listener(wayland.presentation, (void (**)(void))&fsapp_presentation_listener,
                          NULL);
    wl.display_roundtrip_queue(wayland.display, wayland.queue);
    if (wayland.clock_id < 0) {
        SDL_Log("Presentation feedback: none (the compositor did not name its clock)");
        return false;
    }
    SDL_Log("Presentation feedback: Wayland wp_presentation (clock %d)", wayland.clock_id);
    return true;
}

// Frees what fsapp_presentation_init_wayland made, and the feedback still to come.
static void fsapp_presentation_wayland_quit(void)
{
    for (int i = 0; i < FSAPP_PRESENTATION_PENDING_COUNT; i++) {
        if (wayland.pending[i].feedback != NULL) {
            fsapp_presentation_end_pending(&wayland.pending[i]);
        }
    }
    if (wayland.presentation != NULL) {
        wl.proxy_marshal_flags(wayland.presentation, FSAPP_PRESENTATION_DESTROY, NULL,
                               wl.proxy_get_version(wayland.presentation), WL_MARSHAL_FLAG_DESTROY);
        wayland.presentation = NULL;
    }
    if (wayland.queue != NULL) {
        wl.event_queue_destroy(wayland.queue);
        wayland.queue = NULL;
    }
    wayland.display = NULL;
    wayland.window = NULL;
    wayland.surface = NULL;
    wayland.clock_id = -1;
}

static void fsapp_presentation_wayland_request(SDL_Window *window, int64_t swap_number)
{
    if (window != wayland.window) {
        wayland.window = window;
        wayland.surface = (struct wl_proxy *)SDL_GetPointerProperty(
            SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, NULL);
    }
    if (wayland.surface == NULL) {
        return;
    }
    fsapp_presentation_pending_t *pending = NULL;
    for (int i = 0; i < FSAPP_PRESENTATION_PENDING_COUNT; i++) {
        if (wayland.pending[i].feedback == NULL) {
            pending = &wayland.pending[i];
            break;
        }
    }
    if (pending == NULL) {
        // REVIEW: a compositor that never answers (a hidden window may get no presentations until
        // it is shown again) leaves every feedback object waiting; the swaps go without feedback
        // until some are answered.
        if (wayland.skipped++ % 1000 == 0) {
            SDL_Log("Presentation feedback: %d swaps waiting for it, swap %lld left without",
                    FSAPP_PRESENTATION_PENDING_COUNT, (long long)swap_number);
        }
        return;
    }
    pending->swap_number = swap_number;
    pending->feedback = wl.proxy_marshal_flags(
        wayland.presentation, FSAPP_PRESENTATION_FEEDBACK, &fsapp_presentation_feedback_interface,
        wl.proxy_get_version(wayland.presentation), 0, wayland.surface, NULL);
    if (pending->feedback == NULL) {
        return;
    }
    wl.proxy_add_listener(pending->feedback, (void (**)(void))&fsapp_presentation_feedback_listener,
                          pending);
}

// Reads what the connection has for any queue, without waiting (so that a presentation is seen as
// soon as it has come, also when SDL has not pumped its events since), then dispatches the queue.
static void fsapp_presentation_wayland_poll(void)
{
    while (wl.display_prepare_read_queue(wayland.display, wayland.queue) != 0) {
        wl.display_dispatch_queue_pending(wayland.display, wayland.queue);
    }
    struct pollfd fd = {wl.display_get_fd(wayland.display), POLLIN, 0};
    if (poll(&fd, 1, 0) > 0) {
        wl.display_read_events(wayland.display);
    } else {
        wl.display_cancel_read(wayland.display);
    }
    wl.display_dispatch_queue_pending(wayland.display, wayland.queue);
}

static int fsapp_presentation_wayland_get_waiting(void)
{
    int waiting = 0;
    for (int i = 0; i < FSAPP_PRESENTATION_PENDING_COUNT; i++) {
        if (wayland.pending[i].feedback != NULL) {
            waiting++;
        }
    }
    return waiting;
}

static const fsapp_presentation_backend_t fsapp_presentation_wayland_backend = {
    fsapp_presentation_wayland_init, fsapp_presentation_wayland_request,
    fsapp_presentation_wayland_poll, fsapp_presentation_wayland_get_waiting,
    fsapp_presentation_wayland_quit,
};

const fsapp_presentation_backend_t *fsapp_presentation_get_wayland_backend(void)
{
    return &fsapp_presentation_wayland_backend;
}

#else

const fsapp_presentation_backend_t *fsapp_presentation_get_wayland_backend(void)
{
    return NULL;
}

#endif // FSAPP_PRESENTATION_WAYLAND
