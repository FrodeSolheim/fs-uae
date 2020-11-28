#include "fsemu-internal.h"
#include "fsemu-option.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "fsemu-glib.h"
#include "fsemu-log.h"
#include "fsemu-thread.h"
#include "fsemu-util.h"

#ifdef FSUAE
// FIXME: REMOVE
#include <fs/conf.h>
#endif

static struct {
    bool initialized;
    GHashTable *hash_table;
} fsemu_option;

int fsemu_option_read_bool_default(const char *name,
                                   bool *result,
                                   bool default_value)
{
    fsemu_thread_assert_main();
    fsemu_assert(fsemu_option.initialized);

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
    fsemu_thread_assert_main();
    fsemu_assert(fsemu_option.initialized);

    const char *value = fsemu_read_env_option(name);
    if (value[0]) {
        sscanf(value, "%d", result);
        return 1;
    }
    return 0;
}

int fsemu_option_read_const_string(const char *name, const char **result)
{
    fsemu_thread_assert_main();
    fsemu_assert(fsemu_option.initialized);

    const char *value = fsemu_read_env_option(name);
    *result = value;
    if (value[0]) {
        return 1;
    }
    return 0;
}

const char *fsemu_option_const_string(const char *name)
{
    fsemu_thread_assert_main();
    fsemu_assert(fsemu_option.initialized);

    const char *value =
        (const char *) g_hash_table_lookup(fsemu_option.hash_table, name);
    if (value && !value[0]) {
        // An empty string is treated as non-existing (unset value).
        return NULL;
    }
    return value;
}

const char *fsemu_option_const_string_default(const char *name,
                                              const char *default_value)
{
    const char *value = fsemu_option_const_string(name);
    if (value == NULL) {
        return default_value;
    }
    return value;
}

int fsemu_option_int(const char *name)
{
    const char *value = fsemu_option_const_string(name);
    if (value == NULL) {
        return FSEMU_OPTION_NO_INT;
    }
    // FIXME: Use strtol to detect errors and return FS_OPTION_NO_INT?
    return atoi(value);
}

int fsemu_option_int_default(const char *name, int default_value)
{
    int value = fsemu_option_int(name);
    if (value == FSEMU_OPTION_NO_INT) {
        return default_value;
    }
    return value;
}

int fsemu_option_int_clamped(const char *name, int min, int max)
{
    int value = fsemu_option_int(name);
    if (value == FSEMU_OPTION_NO_INT) {
        return value;
    }
    if (value < min) {
        fsemu_option_log(
            "Clamping value %d for key %s to %d\n", value, name, min);
        return min;
    }
    if (value > max) {
        fsemu_option_log(
            "Clamping value %d for key %s to %d\n", value, name, max);
        return max;
    }
    return value;
}

int fsemu_option_int_clamped_default(const char *name,
                                     int min,
                                     int max,
                                     int default_value)
{
    int value = fsemu_option_int_clamped(name, min, max);
    if (value == FSEMU_OPTION_NO_INT) {
        return default_value;
    }
    return value;
}

static void fsemu_option_process_key_value(const char *key,
                                           char *value,
                                           int force)
{
    char *key_lower = g_ascii_strdown(key, -1);
    g_strdelimit(key_lower, "-", '_');
    // Using fsemu_option_const_string here instead of just
    // g_hash_table_lookup, since that also checks for empty strings, which
    // should be treated as non-existing keys.
    if (!force && fsemu_option_const_string(key_lower)) {
        fsemu_option_log("%s = %s (ignored)\n", key_lower, value);
        g_free(key_lower);
        g_free(value);
    } else {
        g_strstrip(value);
        fsemu_option_log("%s = %s\n", key_lower, value);
        // Hash table now owns both key_lower and value.
        g_hash_table_insert(fsemu_option.hash_table, key_lower, value);
    }
}

void fsemu_option_load_from_argv(int argc, char **argv)
{
    fsemu_thread_assert_main();
    int first = 1;
    for (int i = 0; i < argc; i++) {
        const char *arg = argv[i];
        if (!g_str_has_prefix(arg, "--")) {
            continue;
        }
        const char *key = arg + 2;
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
            fsemu_option_log("\n");
            fsemu_option_log("config (command line arguments)\n");
            fsemu_option_log("\n");
            first = 0;
        }
        fsemu_option_process_key_value(k, v, 0);
        g_free(k);
        /* v is owned by process_key_file, do not free here! */
    }
}

#if 0
#ifdef FSEMU_WINDOWS
// ...
#else
// extern char **environ;
#endif
#endif

static void fsemu_option_parse_env_string(char *string)
{
    /*
    printf("ENV: %s\n", string);
    char key[1024];
    char value[1024];
    // FIXME: Check if this is the correct way to do this without risking
    // buffer overflow.
    if (sscanf(string, "%s=%s", key, value) == 2) {
        fsemu_option_process_key_value(key, value, 0);
    }
    */

    for (char *p = string; *p; p++) {
        if (*p == '=') {
            // We can modify the environment string since we use a copy
            // allocated by glib.
            *p = '\0';
            if (strncmp(string, "FSEMU_", 6) == 0) {
                // fsemu_option_process_key_value takes ownership of value.
                fsemu_option_process_key_value(string + 6, g_strdup(p + 1), 0);
            }
            return;
        }
    }
}

static void fsemu_option_init_from_env(void)
{
    gchar **env = g_get_environ();
    for (char **p = env; *p; p++) {
        fsemu_option_parse_env_string(*p);
    }
    g_strfreev(env);
#if 0
#ifdef FSEMU_WINDOWS

#else
#endif
#endif
}

void fsemu_option_init(void)
{
    if (fsemu_option.initialized) {
        return;
    }
    fsemu_option.initialized = true;
    fsemu_thread_init();

    fsemu_option.hash_table =
        g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
    fsemu_option_init_from_env();
}

void fsemu_option_init_from_argv(int argc, char **argv)
{
    fsemu_option_init();
    fsemu_option_load_from_argv(argc, argv);
}
