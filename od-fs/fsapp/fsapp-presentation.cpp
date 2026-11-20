/*
 * Presentation feedback (see fsapp-presentation.h): the backend for SDL's video driver, and the
 * presentations and display refreshes reported, until they are taken.
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "fsapp-presentation.h"

#include <time.h>

#include "fsapp-presentation-backend.h"

// How many presentations, and display refreshes, can wait to be taken; when it is full, the oldest
// is dropped.
#define FSAPP_PRESENTATION_DONE_COUNT 32
#define FSAPP_PRESENTATION_REFRESH_COUNT 32

static struct {
    // Whether the first request has set up (or tried to set up) the feedback, and the backend that
    // set it up (NULL for none).
    bool tried;
    const fsapp_presentation_backend_t *backend;
    // The presentations reported, oldest first from first (wrapping), not taken yet.
    fsapp_frame_presented_t done[FSAPP_PRESENTATION_DONE_COUNT];
    int done_first;
    int done_count;
    // The display refreshes reported, oldest first from first (wrapping), not taken yet.
    fsapp_frame_display_refresh_t refreshes[FSAPP_PRESENTATION_REFRESH_COUNT];
    int refresh_first;
    int refresh_count;
} presentation;

void fsapp_presentation_add_done(const fsapp_frame_presented_t *presented)
{
    if (presentation.done_count == FSAPP_PRESENTATION_DONE_COUNT) {
        presentation.done_first = (presentation.done_first + 1) % FSAPP_PRESENTATION_DONE_COUNT;
        presentation.done_count--;
    }
    int i = (presentation.done_first + presentation.done_count) % FSAPP_PRESENTATION_DONE_COUNT;
    presentation.done[i] = *presented;
    presentation.done_count++;
}

void fsapp_presentation_add_display_refresh(const fsapp_frame_display_refresh_t *refresh)
{
    if (presentation.refresh_count == FSAPP_PRESENTATION_REFRESH_COUNT) {
        presentation.refresh_first =
            (presentation.refresh_first + 1) % FSAPP_PRESENTATION_REFRESH_COUNT;
        presentation.refresh_count--;
    }
    int i = (presentation.refresh_first + presentation.refresh_count) %
            FSAPP_PRESENTATION_REFRESH_COUNT;
    presentation.refreshes[i] = *refresh;
    presentation.refresh_count++;
}

int64_t fsapp_presentation_to_sdl_clock(int clock_id, int64_t time_ns)
{
#ifdef _WIN32
    (void)clock_id;
    (void)time_ns;
    return 0;
#else
    struct timespec ts;
    int64_t before_ns = (int64_t)SDL_GetTicksNS();
    if (clock_gettime((clockid_t)clock_id, &ts) != 0) {
        return 0;
    }
    int64_t after_ns = (int64_t)SDL_GetTicksNS();
    int64_t now_ns = (int64_t)ts.tv_sec * SDL_NS_PER_SECOND + ts.tv_nsec;
    return before_ns + (after_ns - before_ns) / 2 - (now_ns - time_ns);
#endif
}

// Sets up the feedback with the backend for SDL's video driver, and logs which, or why none.
static const fsapp_presentation_backend_t *fsapp_presentation_init(SDL_Window *window)
{
    const char *driver = SDL_GetCurrentVideoDriver();
    const fsapp_presentation_backend_t *backend = NULL;
    if (driver != NULL && SDL_strcmp(driver, "wayland") == 0) {
        backend = fsapp_presentation_get_wayland_backend();
    } else if (driver != NULL && SDL_strcmp(driver, "x11") == 0) {
        backend = fsapp_presentation_get_x11_backend();
    } else if (driver != NULL && SDL_strcmp(driver, "cocoa") == 0) {
        backend = fsapp_presentation_get_macos_backend();
    }
    if (backend == NULL) {
        SDL_Log("Presentation feedback: none for the video driver %s", driver ? driver : "(none)");
        return NULL;
    }
    if (!backend->init(window)) {
        backend->quit();
        return NULL;
    }
    return backend;
}

void fsapp_presentation_request(SDL_Window *window, int64_t swap_number)
{
    if (!presentation.tried) {
        presentation.tried = true;
        presentation.backend = window != NULL ? fsapp_presentation_init(window) : NULL;
    }
    if (presentation.backend != NULL && window != NULL) {
        presentation.backend->request(window, swap_number);
    }
}

int fsapp_presentation_take(fsapp_frame_presented_t *presented, int max)
{
    if (presentation.backend == NULL) {
        return 0;
    }
    presentation.backend->poll();
    int count = SDL_min(max, presentation.done_count);
    for (int i = 0; i < count; i++) {
        presented[i] = presentation.done[presentation.done_first];
        presentation.done_first = (presentation.done_first + 1) % FSAPP_PRESENTATION_DONE_COUNT;
    }
    presentation.done_count -= count;
    return count;
}

int fsapp_presentation_take_display_refreshes(fsapp_frame_display_refresh_t *refreshes, int max)
{
    // The newest max: the older ones are dropped.
    int dropped = SDL_max(0, presentation.refresh_count - max);
    presentation.refresh_first =
        (presentation.refresh_first + dropped) % FSAPP_PRESENTATION_REFRESH_COUNT;
    presentation.refresh_count -= dropped;
    int count = presentation.refresh_count;
    for (int i = 0; i < count; i++) {
        refreshes[i] = presentation.refreshes[presentation.refresh_first];
        presentation.refresh_first =
            (presentation.refresh_first + 1) % FSAPP_PRESENTATION_REFRESH_COUNT;
    }
    presentation.refresh_count = 0;
    return count;
}

int fsapp_presentation_get_waiting(void)
{
    if (presentation.backend == NULL) {
        return -1;
    }
    return presentation.backend->get_waiting();
}

bool fsapp_presentation_is_available(void)
{
    return presentation.backend != NULL;
}

void fsapp_presentation_quit(void)
{
    if (presentation.backend != NULL) {
        presentation.backend->quit();
    }
    presentation.tried = false;
    presentation.backend = NULL;
    presentation.done_first = 0;
    presentation.done_count = 0;
    presentation.refresh_first = 0;
    presentation.refresh_count = 0;
}
