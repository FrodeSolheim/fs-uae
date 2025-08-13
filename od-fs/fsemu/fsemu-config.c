#define FSEMU_INTERNAL
#include "fsemu-config.h"

#include "fsemu-option.h"

int fsemu_config_read_bool_default(const char* name, bool* result, bool default_value) {
    return fsemu_option_read_bool_default(name, result, default_value);
}
