#include "fs/base.h"
#include <string.h>

#ifndef USE_GLIB

void *fs_malloc0(size_t n_bytes)
{
    /* use real malloc if FS_DEBUG_MALLOC is enabled */
    void *data = malloc(n_bytes);
    if (data) {
        memset(data, 0, n_bytes);
    }
    return data;
}

#endif
