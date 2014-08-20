#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <fs/string.h>
#include <fs/hashtable.h>
#include <fs/ml.h>

char *fs_ml_input_unique_device_name(char *name) {
    // fs_hash_table_destroy(device_counts);
    static fs_hash_table *device_counts = NULL;
    if (device_counts == NULL) {
        device_counts = fs_hash_table_new_full(
                fs_str_hash, fs_str_equal, free, NULL);
    }

    int count = FS_POINTER_TO_INT(fs_hash_table_lookup(
            device_counts, name));
    count++;
    fs_hash_table_replace(device_counts, fs_strdup(name),
            FS_INT_TO_POINTER(count));
    if (count > 1) {
        char *result = fs_strdup_printf("%s #%d", name, count);
        free(name);
        name = result;
    }
    return name;
}
