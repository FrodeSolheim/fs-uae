#define FSEMU_INTERNAL 1
#include "fsemu-shader.h"

#include "fsemu-log.h"
#include "fsemu-xmlshader.h"

void fsemu_shader_module_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_xmlshader_module_init();
    fsemu_log("[FSE] [SHD] Initializing shader module\n");
}
