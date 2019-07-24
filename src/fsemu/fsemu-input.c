#define FSEMU_INTERNAL
#include "fsemu/fsemu-input.h"

#include "fsemu/fsemu-mouse.h"
#include "fsemu/fsemu-sdlinput.h"

#ifdef FSUAE
#include <fs/ml.h>
#endif

static struct {
} fsemu_input;

void fsemu_input_init(int flags)
{
    fsemu_return_if_already_initialized();

    fsemu_mouse_init();
    fsemu_sdlinput_init();

#ifdef FSUAE
    fs_ml_input_init();
#endif
}

void fsemu_input_work(int timeout)
{
    fsemu_sdlinput_work();
}
