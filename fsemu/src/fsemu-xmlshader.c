#define FSEMU_INTERNAL
#include "fsemu-xmlshader.h"

#include "fsemu-log.h"

void fsemu_xmlshader_module_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_log("[FSEMU][SHADE] Initializing XML shader module\n");
}
