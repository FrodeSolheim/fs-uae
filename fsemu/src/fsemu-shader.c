#define FSEMU_INTERNAL
#include "fsemu-shader.h"

#include "fsemu-log.h"
#include "fsemu-xmlshader.h"

void fsemu_shader_module_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_xmlshader_module_init();
    fsemu_log("[FSEMU][SHADE] Initializing shader module\n");
}
