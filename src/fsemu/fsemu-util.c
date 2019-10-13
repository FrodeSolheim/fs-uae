#define FSEMU_INTERNAL
#include "fsemu-util.h"

#include "fsemu-glib.h"

const char *fsemu_read_env_option(const char *name)
{
    gchar *key = g_strdup_printf("FSEMU_%s", name);
    char *c = key;
    while (*c) {
        *c = g_ascii_toupper(*c);
        c++;
    }
    const char *value = getenv(key);
    g_free(key);
    if (value != NULL) {
        return value;
    }
    /*
    key = g_strdup_printf("FSGS_%s", name);
    value = getenv(key);
    g_free(key);
    if (value != NULL) {
        return value;
    }
    */
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
