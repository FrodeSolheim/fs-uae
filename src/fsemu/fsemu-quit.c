#define FSEMU_INTERNAL
#include "fsemu/fsemu-quit.h"

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
    fsemu_log("[FSEMU] Quit (maybe)\n");
    fsemu_quit.quit = true;
}
