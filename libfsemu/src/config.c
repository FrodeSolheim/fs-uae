#include <fs/config.h>
#include <fs/log.h>
#include <glib.h>
#include <stdlib.h>
#include <string.h>

static int g_initialized = 0;
GHashTable *g_hash_table = NULL;

#define LOG_LINE "---------------------------------------------------------" \
        "-------------------\n"

static void initialize() {
    g_hash_table = g_hash_table_new_full(g_str_hash, g_str_equal,
            g_free, g_free);
    g_initialized = 1;
}

const char *fs_config_get_const_string(const char *key) {
    if (!g_initialized) {
        initialize();
    }
    const char *value = (const char *) g_hash_table_lookup(g_hash_table, key);
    if (value && !value[0]) {
        // an empty string is treated as non-existing (unset value)
        return NULL;
    }
    return value;
}

char *fs_config_get_string(const char *key) {
    const gchar* value = fs_config_get_const_string(key);
    if (value) {
        return g_strdup(value);
    }
    return NULL;
}

static void process_key_value(const char *key, char *value, int force) {
    gchar *key_lower = g_ascii_strdown(key, -1);
    if (!force && g_hash_table_lookup(g_hash_table, key_lower)) {
        fs_log("%s = %s (ignored)\n", key_lower, value);
        g_free(key_lower);
        g_free(value);
    }
    else {
        g_strstrip(value);
        fs_log("%s = %s\n", key_lower, value);
        // hash table now owns both key_lower and value
        g_hash_table_insert(g_hash_table, key_lower, value);
    }
}

void fs_config_set_string(const char *key, const char *value) {
    process_key_value(key, g_strdup(value), 1);
}

void fs_config_set_string_if_unset(const char *key, const char *value) {
    if (fs_config_get_const_string(key) == NULL) {
        fs_config_set_string(key, value);
    }
}

int fs_config_read_file(const char *path, int force) {
    if (!g_initialized) {
        initialize();
    }
    fs_log("\n");
    fs_log(LOG_LINE);
    fs_log("config (%s)\n", path);
    fs_log(LOG_LINE);
    fs_log("\n");

    // FIXME: support checking a config key "end_config", which causes
    // later calls to fs_config_read_config_file to be ignored
    if (!force && fs_config_get_boolean("end_config") == 1) {
        fs_log("end_config is set, ignoring this config file\n");
        return 1;
    }

    if (g_file_test(path, G_FILE_TEST_IS_REGULAR)) {
        //fs_log("reading config file %s\n", path);
    }
    else {
        fs_log("config file %s does not exist\n", path);
        return 0;
    }

    GKeyFile *key_file = g_key_file_new();
    if (!g_key_file_load_from_file(key_file, path, G_KEY_FILE_NONE, NULL)) {
        fs_log("error loading config file\n");
        g_key_file_free(key_file);
        return 0;
    }

    gchar **groups = g_key_file_get_groups(key_file, NULL);
    for (char **group = groups; *group; group++) {
        gchar **keys = g_key_file_get_keys(key_file, *group, NULL, NULL);
        for (char **key = keys; *key; key++) {
            gchar *value = g_key_file_get_string(key_file, *group, *key, NULL);
            if (value) {
                process_key_value(*key, value, 0);
            }
        }
        g_strfreev(keys);
    }
    g_strfreev(groups);
    return 1;
}

int fs_config_get_boolean(const char *key) {
    return fs_config_get_int(key);
}

int fs_config_get_int(const char *key) {
    const char *value = fs_config_get_const_string(key);
    if (value == NULL) {
        return FS_CONFIG_NONE;
    }
    return atoi(value);
}

int fs_config_get_int_clamped(const char *key, int min, int max) {
    int value = fs_config_get_int(key);
    if (value == FS_CONFIG_NONE) {
        return value;
    }
    if (value < min) {
        fs_log("clamping value %d for key %s to %d\n", value, key, min);
        return min;
    }
    if (value > max) {
        fs_log("clamping value %d for key %s to %d\n", value, key, max);
        return max;
    }
    return value;
}

double fs_config_get_double(const char *key) {
    const char *value = fs_config_get_const_string(key);
    if (value == NULL) {
        return FS_CONFIG_NONE;
    }
    return g_ascii_strtod(value, NULL);
}

double fs_config_get_double_clamped(const char *key, double min, double max) {
    double value = fs_config_get_double(key);
    if (value == FS_CONFIG_NONE) {
        return value;
    }
    if (value < min) {
        fs_log("clamping value %d for key %s to %d\n", value, key, min);
        return min;
    }
    if (value > max) {
        fs_log("clamping value %d for key %s to %d\n", value, key, max);
        return max;
    }
    return value;
}

void fs_config_parse_options(int argc, char **argv) {
    if (!g_initialized) {
        initialize();
    }
    int first = 1;
    for (int i = 0; i < argc; i++) {
        char *arg = argv[i];
        if (!g_str_has_prefix(arg, "--")) {
            continue;
        }
        char *key = arg + 2;
        char *value = strchr(arg, '=');
        gchar *k, *v;
        if (value) {
            k = g_strndup(key, value - key);
            v = g_strdup(value + 1);
        }
        else {
            if (g_str_has_prefix(key, "no-")) {
                k = g_strdup(key + 3);
                v = g_strdup("0");
            }
            else {
                k = g_strdup(key);
                v = g_strdup("1");
            }
        }

        //if (value) {
        if (first) {
            fs_log("\n");
            fs_log(LOG_LINE);
            fs_log("config (command line arguments)\n");
            fs_log(LOG_LINE);
            fs_log("\n");
            first = 0;
        }
        g_strdelimit (k, "-", '_');
        process_key_value(k, v, 0);
        g_free(k);
        // v is owned by process_key_file, do not free here
    }
}
