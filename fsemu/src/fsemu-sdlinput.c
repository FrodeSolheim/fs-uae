#define FSEMU_INTERNAL
#include "fsemu-sdlinput.h"

#ifdef FSEMU_SDL

void fsemu_sdlinput_init(void)
{
    fsemu_return_if_already_initialized();
}

void fsemu_sdlinput_work(void)
{
}

#endif  // FSEMU_SDL
