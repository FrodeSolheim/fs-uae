#ifndef FSEMU_OPTION_H_
#define FSEMU_OPTION_H_

#include "fsemu-common.h"

// #define FSEMU_CONFIG_NONE 0x12345678

#ifdef __cplusplus
extern "C" {
#endif

static inline int fsemu_option_boolean(const char *name)
{
    return FSEMU_CONFIG_NONE;
}

int fsemu_option_read_bool_default(const char *name,
                                   bool *result,
                                   bool default_value);
int fsemu_option_read_int(const char *name, int *result);
// int fsemu_option_read_string(const char *name, int *result, int max_len);
int fsemu_option_read_const_string(const char *name, const char **result);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_OPTION_H_
