#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/ml.h>
#include <fs/glib.h>

char *fs_ml_input_unique_device_name(char *name)
{
    static GHashTable *device_counts = NULL;
    if (device_counts == NULL) {
        device_counts = g_hash_table_new_full(
                g_str_hash, g_str_equal, g_free, NULL);
    }

    int count = GPOINTER_TO_INT(g_hash_table_lookup(
                                device_counts, name));
    count++;
    g_hash_table_replace(device_counts, g_strdup(name),
                         GINT_TO_POINTER(count));
    if (count > 1) {
        char *result = g_strdup_printf("%s #%d", name, count);
        g_free(name);
        name = result;
    }
    return name;
}
