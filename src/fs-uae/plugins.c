#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <fs/log.h>
#include <glib.h>
#include <string.h>
#include <uae/uae.h>
#include "paths.h"

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

static void load_plugin_provides(const char *path, GKeyFile *key_file, const char *group_name) {
    gchar **keys = g_key_file_get_keys(key_file, group_name, NULL, NULL);
    if (keys == NULL) {
        return;
    }
    for (gchar *key = *keys; *key; key++) {
        gchar *value = g_key_file_get_string(key_file, group_name, key, NULL);
        if (value == NULL) {
            continue;
        }
        if (strcmp(key, "libcapsimage") == 0) {
            fs_log("Found libcapsimage provide -> %s\n", value);
            gchar *p = g_build_filename(path, value, NULL);
            amiga_set_caps_library_path(p);
            g_free(p);
        }
    }
    g_strfreev(keys);
}

static void load_plugin(const char *path, const char *ini_path) {
    fs_log("Loading %s\n", path);
    GKeyFile *key_file = g_key_file_new();
    if (!g_key_file_load_from_file(key_file, ini_path, G_KEY_FILE_NONE, NULL)) {
        fs_log("Warning: Could not load plugin.ini\n");
        return;
    }
    gchar *version = g_key_file_get_string(key_file, "plugin", "version", NULL);
    if (version == NULL) {
        fs_log("Warning: Could not read plugin version\n");
        return;
    }
    gchar *version_path = g_build_filename(path, version, NULL);
    fs_log("-> %s\n", version_path);
    load_plugin_provides(version_path, key_file, "provides");
    load_plugin_provides(version_path, key_file, OS_NAME "-" ARCH_NAME);
    g_free(version_path);
    g_free(version);
    g_key_file_free(key_file);
}

void fs_uae_plugins_init() {
    fs_log("Initializing plugins\n");
    const char *plugins_dir = fs_uae_plugins_dir();
    GDir *dir = g_dir_open(plugins_dir, 0, NULL);
    if (!dir) {
        fs_log("Warning: could not open plugins dir\n");
        return;
    }
    const gchar *name;
    while ((name = g_dir_read_name(dir))) {
        char *p = g_build_filename(plugins_dir, name, NULL);
        char *p2 = g_build_filename(p, "plugin.ini", NULL);
        fs_log("Checking %s\n", p2);
        if (g_file_test(p2, G_FILE_TEST_IS_REGULAR)) {
            load_plugin(p, p2);
        }
        g_free(p2);
        g_free(p);
    }
    g_dir_close(dir);
}
