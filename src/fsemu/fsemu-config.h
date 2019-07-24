#ifndef FSEMU_CONFIG_H_
#define FSEMU_CONFIG_H_

#include "fsemu/fsemu-common.h"

#define FSEMU_CONFIG_NONE 0x12345678

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

static inline int fsemu_config_boolean(const char *name)
{
    return FSEMU_CONFIG_NONE;
}

int fsemu_config_read_bool_default(const char *name,
                                   bool *result,
                                   bool default_value);

#endif  // FSEMU_CONFIG_H_
