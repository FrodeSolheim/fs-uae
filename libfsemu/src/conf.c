#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/base.h>
#include <fs/conf.h>
#include <fs/filesys.h>
#include <fs/glib.h>
#include <fs/inifile.h>
#include <fs/log.h>
#include <stdlib.h>
#include <string.h>

static int g_initialized = 0;
GHashTable *g_hash_table = NULL;

#define LOG_LINE \
"----------------------------------------------------------------------------\n"

static void initialize(void)
{
    if (g_initialized) {
        return;
    }
    g_initialized = 1;
    g_hash_table = g_hash_table_new_full(
        g_str_hash, g_str_equal, g_free, g_free);
}

void fse_init_conf(void)
{
    initialize();
}

bool fs_config_exists(const char *key)
{
    return fs_config_get_const_string(key) != NULL;
}

bool fs_config_check_auto(const char *key, const char *value)
{
    const char *s = fs_config_get_const_string(key);
    if (s == NULL) {
        return true;
    }
    if (value && strcasecmp(s, value) == 0) {
        return true;
    }
    return false;
}

bool fs_config_check_enabled(const char *key, const char *value)
{
    const char *s = fs_config_get_const_string(key);
    if (s == NULL) {
        return false;
    }
    if (value && strcasecmp(s, value) == 0) {
        return true;
    }
    if (strcasecmp(s, "1") == 0) {
        return true;
    }
    return false;
}

bool fs_config_check_disabled(const char *key, const char *value)
{
    const char *s = fs_config_get_const_string(key);
    if (s == NULL) {
        return false;
    }
    if (value && strcasecmp(s, value) == 0) {
        return true;
    }
    if (strcasecmp(s, "0") == 0) {
        return true;
    }
    return false;
}

const char *fs_config_get_const_string(const char *key)
{
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

char *fs_config_get_string(const char *key)
{
    const char* value = fs_config_get_const_string(key);
    if (value) {
        return g_strdup(value);
    }
    return NULL;
}

static void process_key_value(const char *key, char *value, int force)
{
    char *key_lower = g_ascii_strdown(key, -1);
    g_strdelimit (key_lower, "-", '_');
    /* Using fs_config_get_const_string here instead of just
     * g_hash_table_lookup, since that also checks for empty strings, which
     * should be treated as non-existing keys. */
    if (!force && fs_config_get_const_string(key_lower)) {
        fs_log("%s = %s (ignored)\n", key_lower, value);
        g_free(key_lower);
        g_free(value);
    } else {
        g_strstrip(value);
        fs_log("%s = %s\n", key_lower, value);
        /* Hash table now owns both key_lower and value. */
        g_hash_table_insert(g_hash_table, key_lower, value);
    }
}

void fs_config_set_string(const char *key, const char *value)
{
    process_key_value(key, g_strdup(value), 1);
}

void fs_config_set_string_if_unset(const char *key, const char *value)
{
    if (fs_config_get_const_string(key) == NULL) {
        fs_config_set_string(key, value);
    }
}

void fs_config_parse_ini_file(fs_ini_file *ini_file, int force)
{
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
                char *key2 = g_strconcat(prefix, *key, NULL);
                process_key_value(key2, value, force);
                g_free(key2);
            }
        }
        g_strfreev(keys);
    }
    g_strfreev(groups);
}


int fs_config_read_file(const char *path, int force)
{
    if (!g_initialized) {
        initialize();
    }
    fs_log("\n");
    fs_log(LOG_LINE);
    fs_log("config (%s)\n", path);
    fs_log(LOG_LINE);
    fs_log("\n");

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
    fs_config_parse_ini_file(ini_file, force);
    fs_ini_file_destroy(ini_file);
    return 1;
}

int fs_config_get_boolean(const char *key)
{
    return fs_config_get_int(key);
}

int fs_config_get_int(const char *key)
{
    const char *value = fs_config_get_const_string(key);
    if (value == NULL) {
        return FS_CONFIG_NONE;
    }
    return atoi(value);
}

int fs_config_get_int_clamped(const char *key, int min, int max)
{
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

double fs_config_get_double(const char *key)
{
    const char *value = fs_config_get_const_string(key);
    if (value == NULL) {
        return FS_CONFIG_NONE;
    }
    return g_ascii_strtod(value, NULL);
}

double fs_config_get_double_clamped(const char *key, double min, double max)
{
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

void fs_config_parse_options(int argc, char **argv)
{
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
        char *k, *v;
        if (value) {
            k = g_strndup(key, value - key);
            v = g_strdup(value + 1);
        } else {
            if (g_str_has_prefix(key, "no-")) {
                k = g_strdup(key + 3);
                v = g_strdup("0");
            } else {
                k = g_strdup(key);
                v = g_strdup("1");
            }
        }

        if (first) {
            fs_log("\n");
            fs_log(LOG_LINE);
            fs_log("config (command line arguments)\n");
            fs_log(LOG_LINE);
            fs_log("\n");
            first = 0;
        }
        process_key_value(k, v, 0);
        g_free(k);
        /* v is owned by process_key_file, do not free here! */
    }
}
