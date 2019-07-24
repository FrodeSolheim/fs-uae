#define FSEMU_INTERNAL
#include "fsemu/fsemu-util.h"

#include <glib.h>

const char *fsemu_read_env(const char *name)
{
    gchar *key = g_strdup_printf("FSE_%s", name);
    const char *value = getenv(key);
    g_free(key);
    if (value != NULL) {
        return value;
    }
    key = g_strdup_printf("FSGS_%s", name);
    value = getenv(key);
    g_free(key);
    if (value != NULL) {
        return value;
    }
    return "";
}

const char *fsemu_getenv(const char *name)
{
    const char *value = getenv(name);
    if (value != NULL) {
        return value;
    }
    return "";
}
