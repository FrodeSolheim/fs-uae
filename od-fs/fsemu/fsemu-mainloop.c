#include "fsemu-mainloop.h"

#include "fsapp-mainloop.h"
#include "fsemu-input.h"
#include "fsemu-sdlinput.h"
#include "fsuae-hack.h"

void fsemu_mainloop_handle_event(SDL_Event* event) {
    // fsapp_mainloop_handle_event(event);

    if (fsemu_sdlinput_handle_event(event)) {
        // return true;
    }

    switch (event->type) {
        case SDL_EVENT_KEY_DOWN:
#ifdef FSLIB_OS_MACOS
            if (event->key.mod & SDL_KMOD_LGUI) {
#else
            if (event->key.mod & SDL_KMOD_LALT) {
#endif
                if (event->key.key == SDLK_S) {
                    hack_integer_scaling_mode = !hack_integer_scaling_mode;
                }
                if (event->key.key == SDLK_C) {
                    hack_auto_crop_mode = !hack_auto_crop_mode;
                }
            }
            break;
    }
}

void fsemu_mainloop_iteration(void) {
    if (fsemu_input_reconfigure_needed) {
        fsemu_input_reconfigure();
    }
}
