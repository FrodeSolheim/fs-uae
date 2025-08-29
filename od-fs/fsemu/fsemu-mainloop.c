#include "fsemu-mainloop.h"

#include "fsapp-mainloop.h"
#include "fsemu-input.h"
#include "fsemu-sdlinput.h"

void fsemu_mainloop_handle_event(SDL_Event* event) {
    // fsapp_mainloop_handle_event(event);

    if (fsemu_sdlinput_handle_event(event)) {
        // return true;
    }

    // switch(event->type) {
    //     case EVENT_
    // }
}

void fsemu_mainloop_iteration(void) {
    if (fsemu_input_reconfigure_needed) {
        fsemu_input_reconfigure();
    }
}
