#ifndef FSAPP_MAIN_H
#define FSAPP_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdint.h>

extern uint32_t SDL_EVENT_FSAPP_WINDOW;

bool fsemu_init(int argc, char* argv[], int* error);
bool fsapp_init(int argc, char* argv[], int* error);

void fsemu_main_handle_one_frame(void);
bool fsapp_main_is_alive(void);
void fsapp_main_handle_event(SDL_Event* event);
bool fsapp_main_handle_events(void);
void fsapp_check_and_handle_dialog_result(void);

void fsapp_main_quit(void);

extern SDL_Window* g_window;
extern int g_window_width;
extern int g_window_height;

extern bool g_fsapp_force_ui_cursor;
extern bool g_fsapp_mouse_grab;

#ifdef __cplusplus
}
#endif

#endif  // FSAPP_MAIN_H
