/*
 * The host window.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSAPP_WINDOW_H
#define FSAPP_WINDOW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <SDL3/SDL.h>

struct fsapp_window {
    SDL_Window *window;
    // The size asked for, in pixels, until the window is created; then its size in window
    // coordinates.
    int width;
    int height;

    SDL_Color background_color;
    const char *title;
    bool fullscreen;
    // bool mousegrab;
    bool relative_mouse;
};

typedef struct fsapp_window fsapp_window_t;

// The host window, once the main thread has created it (NULL before).
extern fsapp_window_t *g_fsapp_window;

#ifdef __cplusplus
}
#endif

#endif /* FSAPP_WINDOW_H */
