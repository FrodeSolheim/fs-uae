#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/base.h>
#include <fs/log.h>
#include <uae/uae.h>
#include "paths.h"
#include "plugins.h"

#include <string.h>
#include <stdlib.h>
#include <glib.h>

#define NEW_PLUGINS 1

// FIXME: move to configure.ac / config.h
#if defined(__x86_64__) || defined(_M_AMD64) || defined(_M_X64)
    #define ARCH_NAME "x86-64"
#elif defined(__i386__) || defined (_M_IX86)
    #define ARCH_NAME "x86"
#elif defined(__ppc__)
    #define ARCH_NAME "ppc"
#else
    #define ARCH_NAME "unknown"
#endif

static GHashTable *provides;
#define MAX_PLUGINS 256
static gchar* g_plugin_ver_dirs[MAX_PLUGINS];
static int g_plugin_count;

#ifndef NEW_PLUGINS
static const char *os_arch_name()
{
    static const char *name = NULL;
    if (name == NULL) {
#ifdef LINUX
#if 0
        if (getenv("STEAM_RUNTIME") && getenv("STEAM_RUNTIME")[0]) {
            fs_log("os_arch_name: detected STEAM_RUNTIME, using steamos\n");
            name = "steamos_" ARCH_NAME;
            return name;
        }
#endif
#endif
        name = OS_NAME "_" ARCH_NAME;
    }
    return name;
}
#endif

static const char *lookup_plugin(const char *name)
{
    fs_log("[PLUGINS] Looking up \"%s\"\n", name);
    gchar *module_name = g_strconcat(name, LT_MODULE_EXT, NULL);
    char executable_dir[FS_PATH_MAX];
    fs_get_application_exe_dir(executable_dir, FS_PATH_MAX);
    gchar *path;

    path = g_build_filename(executable_dir, module_name, NULL);
    fs_log("[PLUGINS] Checking \"%s\"\n", path);
    if (g_file_test(path, G_FILE_TEST_EXISTS)) {
        g_free(module_name);
        // FIXME: resource leak if called more than once for the same
        // plugin, should cache the path
        return (const char*) path;
    }
    g_free(path);

    path = g_build_filename(executable_dir, "..", name, module_name, NULL);
    fs_log("[PLUGINS] Checking \"%s\"\n", path);
    if (g_file_test(path, G_FILE_TEST_EXISTS)) {
        g_free(module_name);
        // FIXME: resource leak, should cache the path
        return (const char*) path;
    }
    g_free(path);

    path = g_build_filename(fs_uae_plugins_dir(), module_name, NULL);
    fs_log("[PLUGINS] Checking \"%s\"\n", path);
    if (g_file_test(path, G_FILE_TEST_EXISTS)) {
        g_free(module_name);
        // FIXME: resource leak, should cache the path
        return (const char*) path;
    }
    g_free(path);

#ifndef NEW_PLUGINS
    void *data = g_hash_table_lookup(provides, module_name);
    if (!data) {
        data = g_hash_table_lookup(provides, name);
    }
    if (data) {
        g_free(module_name);
        return (const char *) data;
    }
#endif

    /* First check within plugin os/arch directories */

    for (int i = 0; i < g_plugin_count; i++) {
#ifdef NEW_PLUGINS
        path = g_build_filename(g_plugin_ver_dirs[i], OS_NAME_3, ARCH_NAME,
                                module_name, NULL);
#else
        path = g_build_filename(g_plugin_ver_dirs[i], os_arch_name(),
                                module_name, NULL);
#endif
        fs_log("[PLUGINS] Checking \"%s\"\n", path);
        if (g_file_test(path, G_FILE_TEST_EXISTS)) {
            g_free(module_name);
            // FIXME: resource leak, should cache the path
            return (const char*) path;
        }
        g_free(path);
    }

    /* Check outside arch directories */

    for (int i = 0; i < g_plugin_count; i++) {
        path = g_build_filename(g_plugin_ver_dirs[i], module_name, NULL);
        fs_log("[PLUGINS] Checking \"%s\"\n", path);
        if (g_file_test(path, G_FILE_TEST_EXISTS)) {
            g_free(module_name);
            // FIXME: resource leak, should cache the path
            return (const char*) path;
        }
        g_free(path);
    }

    g_free(module_name);
    return NULL;
}

#ifndef NEW_PLUGINS
static void load_plugin_provides(const char *path, GKeyFile *key_file,
                                 const char *group_name, int scan)
{
    gchar **keys = g_key_file_get_keys(key_file, group_name, NULL, NULL);
    if (keys) {
        for (gchar *key = *keys; *key; key++) {
            gchar *value = g_key_file_get_string(key_file, group_name,
                                                 key, NULL);
            if (value == NULL) {
                continue;
            }
            gchar *p = g_build_filename(path, value, NULL);
            if (g_file_test(p, G_FILE_TEST_EXISTS)) {
                g_hash_table_insert(provides, g_strdup(key), p);
            }
            else {
                g_free(p);
            }
        }
        g_strfreev(keys);
    }
    if (scan) {
        GDir *dir = g_dir_open(path, 0, NULL);
        if (dir) {
            const gchar *name;
            while ((name = g_dir_read_name(dir))) {
                gchar *p = g_build_filename(path, name, NULL);
                g_hash_table_insert(provides, g_strdup(name), p);
            }
            g_dir_close(dir);
        } else {
            fs_log("[PLUGINS] Could not open plugin dir %s\n", path);
        }
    }
}
#endif

static void load_plugin(const char *path, const char *ini_path)
{
    fs_log("[PLUGINS] Loading %s\n", path);
#if NEW_PLUGINS
    gchar *version_path = g_build_filename(path, NULL);
#else
    GKeyFile *key_file = g_key_file_new();
    if (!g_key_file_load_from_file(key_file, ini_path, G_KEY_FILE_NONE, NULL)) {
        fs_log("[PLUGINS] Could not load plugin.ini\n");
        return;
    }
    gchar *version = g_key_file_get_string(key_file, "plugin", "version", NULL);
    if (version == NULL) {
        fs_log("[PLUGINS] Could not read plugin version\n");
        return;
    }
    gchar *version_path = g_build_filename(path, version, NULL);
    fs_log("[PLUGINS] -> %s\n", version_path);
    load_plugin_provides(version_path, key_file, "provides", 0);

    load_plugin_provides(version_path, key_file, os_arch_name(), 1);
#endif
    if (g_plugin_count < MAX_PLUGINS) {
        g_plugin_ver_dirs[g_plugin_count] = version_path;
        g_plugin_count += 1;
    } else {
        g_free(version_path);
    }
#ifndef NEW_PLUGINS
    g_free(version);
    g_key_file_free(key_file);
#endif
}

static void load_plugins_from_dir(const char *plugins_dir)
{
    fs_log("[PLUGINS] Loading plugins from %s\n", plugins_dir);
    GDir *dir = g_dir_open(plugins_dir, 0, NULL);
    if (!dir) {
        fs_log("[PLUGINS] Could not open plugins dir %s\n", plugins_dir);
        return;
    }
    const gchar *name;
    while ((name = g_dir_read_name(dir))) {
        gchar *p = g_build_filename(plugins_dir, name, NULL);
#if NEW_PLUGINS
        gchar *p2 = g_build_filename(p, "Version.txt", NULL);
#else
        gchar *p2 = g_build_filename(p, "Plugin.ini", NULL);
#endif
        fs_log("[PLUGINS] Checking %s\n", p2);
        if (g_file_test(p2, G_FILE_TEST_IS_REGULAR)) {
            load_plugin(p, p2);
        }
        g_free(p2);
        g_free(p);
    }
    g_dir_close(dir);
}

void fs_uae_plugins_init()
{
    fs_log("[PLUGINS] Initializing\n");
    provides = g_hash_table_new(g_str_hash, g_str_equal);
    amiga_set_plugin_lookup_function(lookup_plugin);

    const char *plugins_dir = fs_uae_plugins_dir();
    load_plugins_from_dir(plugins_dir);

    char executable_dir[FS_PATH_MAX];
    fs_get_application_exe_dir(executable_dir, FS_PATH_MAX);
    gchar *path = g_build_filename(executable_dir, "..", "lib",
                                   "fs-uae", "plugins", NULL);
    if (g_file_test(path, G_FILE_TEST_IS_DIR)) {
        load_plugins_from_dir(path);
    }
    g_free(path);
}
