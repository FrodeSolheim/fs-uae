#define FSEMU_INTERNAL
#include "fsemu/fsemu-image.h"

void fsemu_image_init(void)
{
    fsemu_return_if_already_initialized();

    fsemu_image_log("Initialize\n");
}
