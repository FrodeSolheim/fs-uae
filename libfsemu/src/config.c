#include <stdlib.h>
#include <string.h>
#include <fs/base.h>
#include <fs/config.h>
#include <fs/filesys.h>
#include <fs/hashtable.h>
#include <fs/inifile.h>
#include <fs/log.h>
#include <fs/string.h>

#if 0
#ifdef USE_GLIB
#include <glib.h>
#endif
#endif

static int g_initialized = 0;
fs_hash_table *g_hash_table = NULL;

#define LOG_LINE "---------------------------------------------------------" \
        "-------------------\n"

static void initialize() {
    g_hash_table = fs_hash_table_new_full(fs_str_hash, fs_str_equal,
            free, free);
    g_initialized = 1;
}

const char *fs_config_get_const_string(const char *key) {
    if (!g_initialized) {
        initialize();
    }
    const char *value = (const char *) fs_hash_table_lookup(g_hash_table, key);
    if (value && !value[0]) {
        // an empty string is treated as non-existing (unset value)
        return NULL;
    }
    return value;
}

char *fs_config_get_string(const char *key) {
    const char* value = fs_config_get_const_string(key);
    if (value) {
        return fs_strdup(value);
    }
    return NULL;
}

static void process_key_value(const char *key, char *value, int force) {
    char *key_lower = fs_ascii_strdown(key, -1);
    // using fs_config_get_const_string here instead of just
    // fs_hash_table_lookup, since that also checks for empty strings, which
    // should be treated as non-existing keys
    if (!force && fs_config_get_const_string(key_lower)) {
        fs_log("%s = %s (ignored)\n", key_lower, value);
        free(key_lower);
        free(value);
    }
    else {
        fs_strstrip(value);
        fs_log("%s = %s\n", key_lower, value);
        // hash table now owns both key_lower and value
        fs_hash_table_insert(g_hash_table, key_lower, value);
    }
}

void fs_config_set_string(const char *key, const char *value) {
    process_key_value(key, fs_strdup(value), 1);
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

    if (!fs_path_is_file(path)) {
        fs_log("config file %s does not exist\n", path);
        return 0;
    }

    fs_ini_file *ini_file = fs_ini_file_open(path);
    if (ini_file == NULL) {
        fs_log("error loading config file\n");
        return 0;
    }

    char **groups = fs_ini_file_get_groups(ini_file, NULL);
    for (char **group = groups; *group; group++) {
        const char *prefix = "";
        if (strcmp(*group, "theme") == 0) {
            prefix = "theme_";
        }
        char **keys = fs_ini_file_get_keys(ini_file, *group, NULL);
        for (char **key = keys; *key; key++) {
            char *value = fs_ini_file_get_value(ini_file, *group, *key);
            if (value) {
                char *key2 = fs_strconcat(prefix, *key, NULL);
                process_key_value(key2, value, 0);
                free(key2);
            }
        }
        fs_strfreev(keys);
    }
    fs_strfreev(groups);
    fs_ini_file_destroy(ini_file);

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
    return fs_ascii_strtod(value, NULL);
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
        if (!fs_str_has_prefix(arg, "--")) {
            continue;
        }
        char *key = arg + 2;
        char *value = strchr(arg, '=');
        char *k, *v;
        if (value) {
            k = fs_strndup(key, value - key);
            v = fs_strdup(value + 1);
        }
        else {
            if (fs_str_has_prefix(key, "no-")) {
                k = fs_strdup(key + 3);
                v = fs_strdup("0");
            }
            else {
                k = fs_strdup(key);
                v = fs_strdup("1");
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
        fs_strdelimit (k, "-", '_');
        process_key_value(k, v, 0);
        free(k);
        // v is owned by process_key_file, do not free here
    }
}
