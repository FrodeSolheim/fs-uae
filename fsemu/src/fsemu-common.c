#define FSEMU_INTERNAL
#include "fsemu-common.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

static struct {
    char *emulator_name;
    // const char *fork_info;
} fsemu_common;

const char *fsemu_emulator_name(void)
{
    if (!fsemu_common.emulator_name) {
        return "FSEMU";
    }
    return fsemu_common.emulator_name;
}

void fsemu_set_emulator_name(const char *emulator_name)
{
    if (fsemu_common.emulator_name) {
        free(fsemu_common.emulator_name);
    }
    fsemu_common.emulator_name = strdup(emulator_name);
}

void fsemu_error_2(const char *msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    vprintf(msg, ap);
    va_end(ap);
}
