#define FSEMU_INTERNAL
#include "fsemu-quit.h"

#include "fs/log.h"
#include "fsemu-frame.h"
#include "fsemu-sdl.h"

static struct {
    volatile bool quit;
} fsemu_quit;

bool fsemu_quit_check(void)
{
    return fsemu_quit.quit;
}

void fsemu_quit_abort(void)
{
    fsemu_quit.quit = false;
}

void fsemu_quit_maybe(void)
{
    SDL_Log("[FSE] [...] Quit (maybe)\n");
    fsemu_quit.quit = true;
    // A paused emulation thread sleeps until woken; it then sees the quit request.
    fsemu_frame_wake_paused();

#if 0
    if (!SDL_IsMainThread()) {
        // SDL_Quit();
        printf("%d\n", 1 / 0);
    }
#endif
    // FIXME: This pushes an SDL event of type 0 (SDL_EVENT_FIRST); nothing handles it. FS-UAE 4's
    // host window code registered the type that this was meant to have (fsemu_sdlwindow_init),
    // which FS-UAE 5 never ran. Remove it.
    SDL_Event user_event;
    SDL_zero(user_event);
    SDL_PushEvent(&user_event);
}
