#define FSEMU_INTERNAL
#include "fsemu-option.h"

#include <stdio.h>
#include <strings.h>

#include "fsemu-util.h"
#ifdef FSUAE
#include <fs/conf.h>
#endif

int fsemu_option_read_bool_default(const char *name,
                                   bool *result,
                                   bool default_value)
{
    const char *value = fsemu_read_env_option(name);
    if (value[0]) {
        if (strcasecmp(value, "1") == 0 || strcasecmp(value, "true") == 0) {
            *result = true;
            return 1;
        }
        if (strcasecmp(value, "0") == 0 || strcasecmp(value, "false") == 0) {
            *result = false;
            return 1;
        }
    }
#ifdef FSUAE
    int res = fs_config_get_boolean(name);
    if (res == FS_CONFIG_NONE) {
        *result = default_value;
    } else {
        *result = res != 0;
    }
    return *result;
#else
    *result = default_value;
    return 0;
#endif
}

int fsemu_option_read_int(const char *name, int *result)
{
    const char *value = fsemu_read_env_option(name);
    if (value[0]) {
        sscanf(value, "%d", result);
        return 1;
    }
    return 0;
}

int fsemu_option_read_const_string(const char *name, const char **result)
{
    const char *value = fsemu_read_env_option(name);
    *result = value;
    if (value[0]) {
        return 1;
    }
    return 0;
}
