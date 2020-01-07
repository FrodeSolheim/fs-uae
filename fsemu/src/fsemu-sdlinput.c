#define FSEMU_INTERNAL
#include "fsemu-sdlinput.h"

#include "fsemu-input.h"

#ifdef FSEMU_SDL

void fsemu_sdlinput_init(void)
{
    fsemu_return_if_already_initialized();
}

void fsemu_sdlinput_work(void)
{
}

static bool fsemu_sdlinput_handle_key_event(SDL_Event *event)
{
    if (event->key.repeat) {
        fsemu_input_log("(Ignoring repeated key press scancode=%d)\n",
                        event->key.keysym.scancode);
        return false;
    }
    fsemu_input_log("Key %s scancode=%d\n",
                    event->key.state ? "press" : "release",
                    event->key.keysym.scancode);
    fsemu_scancode_t scancode = event->key.keysym.scancode;
    fsemu_input_handle_keyboard(scancode, event->key.state != 0);
    return false;
}

bool fsemu_sdlinput_handle_event(SDL_Event *event)
{
    if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
        return fsemu_sdlinput_handle_key_event(event);
    }
    return false;
}

#endif  // FSEMU_SDL
