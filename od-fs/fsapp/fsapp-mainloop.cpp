#include "fsapp-mainloop.h"

#include "fsapp-main.h"

void fsapp_mainloop_handle_event(SDL_Event* event) {
    // ...
    // FIXME: Old?
    fsapp_main_handle_event(event);
}

void fsapp_mainloop_iteration(void) {
}
