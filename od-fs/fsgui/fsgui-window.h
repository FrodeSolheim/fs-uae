#ifndef FSGUI_WINDOW_H
#define FSGUI_WINDOW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <SDL3/SDL.h>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} color_type;

// typedef struct {
//     //SDL_Window* sdl_window;
//     color_type background_color;
//     const char* title;
//     int width;
//     int height;
//     bool fullscreen;
//     // bool mousegrab;
//     bool relative_mouse;
// } window_type;

struct fsgui_window {
    SDL_Window* window;
    int width;
    int height;

    color_type background_color;
    const char* title;
    bool fullscreen;
    // bool mousegrab;
    bool relative_mouse;
};

typedef struct fsgui_window fsgui_window_t;

void fsgui_window_create(void);

#ifdef __cplusplus
}
#endif

#endif  // FSGUI_WINDOW_H
