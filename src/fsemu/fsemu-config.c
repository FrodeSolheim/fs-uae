#define FSEMU_INTERNAL
#include "fsemu-config.h"

#ifdef FSUAE
#include <fs/conf.h>
#endif

int fsemu_config_read_bool_default(const char *name,
                                   bool *result,
                                   bool default_value)
{
#ifdef FSUAE
    int res = fs_config_get_boolean(name);
    if (res == FS_CONFIG_NONE) {
        *result = default_value;
    } else {
        *result = res != 0;
    }
    return *result;
#else
    return 0;
#endif
}