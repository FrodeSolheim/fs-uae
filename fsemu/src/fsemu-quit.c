#define FSEMU_INTERNAL
#include "fsemu-quit.h"

#include "fsemu-log.h"
#include "fsemu-thread.h"
#include "fsemu-window.h"

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
    fsemu_log("[FSE] [...] Quit (maybe)\n");
    fsemu_quit.quit = true;

#if 0
    if (!fsemu_thread_is_main()) {
        // SDL_Quit();
        printf("%d\n", 1 / 0);
    }
#endif
    fsemu_window_notify_quit();
}
