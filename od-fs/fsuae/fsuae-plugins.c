#define FSUAE_INTERNAL
#include "fsuae-plugins.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <SDL3/SDL.h>
#include <fs/base.h>
#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <uae/uae.h>

#include "FS/FS.h"
#include "fsemu-module.h"
#include "fslib-path.h"
#include "fsuae-path.h"

#define NEW_PLUGINS 1

// FIXME: move to configure.ac / config.h
#if defined(__x86_64__) || defined(_M_AMD64) || defined(_M_X64)
#define ARCH_NAME "x86-64"
#elif defined(__i386__) || defined(_M_IX86)
#define ARCH_NAME "x86"
#elif defined(__ppc__)
#define ARCH_NAME "PPC"
#elif defined(__aarch64__)
#define ARCH_NAME "ARM64"
#elif defined(__ARM_ARCH_8A__)
#define ARCH_NAME "ARMv8"
#elif defined(__arm__)
#define ARCH_NAME "ARM"
#else
#define ARCH_NAME "unknown"
#endif

static GHashTable* provides;
#define MAX_PLUGINS 256
static gchar* g_plugin_ver_dirs[MAX_PLUGINS];
static int g_plugin_count;

#ifndef NEW_PLUGINS
static const char* os_arch_name() {
    static const char* name = NULL;
    if (name == NULL) {
#ifdef LINUX
#if 0
        if (getenv("STEAM_RUNTIME") && getenv("STEAM_RUNTIME")[0]) {
            SDL_Log("os_arch_name: detected STEAM_RUNTIME, using steamos\n");
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

static char* lookup_plugin_in_dir(const char* name, const char* module_name,
                                        const char* path);

/**
 * @brief This function will be called from UAE it tries to load a plugin library.
 *
 * @param name Name of the library (without .dll or .so extension)
 * @return Returns the full path of the library (or NULL if not found)
 */
static const char* lookup_plugin(const char* name) {
    char* result;

    char module_name[FS_MAX_PATH];
    FS_CopyPath(module_name, name);
    FS_AppendPath(module_name, LT_MODULE_EXT);
    SDL_Log("[PLUGINS] Looking up module \"%s\"\n", name);

    char path[FS_MAX_PATH];
    FS_CopyPath(path, fsapp_data_directory);
    FS_AppendDirName(path, "Plugins");
    result = lookup_plugin_in_dir(name, module_name, path);
    if (result) {
        return result;
    }

    FS_CopyPath(path, FS_GetApplicationDir());
    result = lookup_plugin_in_dir(name, module_name, path);
    if (result) {
        return result;
    }

#if 0
    // gchar* module_name = g_strconcat(name, LT_MODULE_EXT, NULL);
    char executable_dir[FS_PATH_MAX];
    // fs_get_application_exe_dir(executable_dir, FS_PATH_MAX);
    fslib_path_application_dir(executable_dir, FS_PATH_MAX);
    gchar* path2;

#if 0
    // Check side-by-side development directory
    if (fsapp_development_dir) {
        path = g_build_filename(executable_dir, "..", name, module_name, NULL);
        SDL_Log("[PLUGINS] Checking \"%s\"\n", path);
        if (g_file_test(path, G_FILE_TEST_EXISTS)) {
            g_free(module_name);
            // FIXME: resource leak, should cache the path
            return (const char*)path;
        }
        g_free(path);
    }
#endif

#ifndef NEW_PLUGINS
    void* data = g_hash_table_lookup(provides, module_name);
    if (!data) {
        data = g_hash_table_lookup(provides, name);
    }
    if (data) {
        g_free(module_name);
        return (const char*)data;
    }
#endif

#ifdef MACOSX
    // First check for framework
    gchar* framework_name = g_strconcat(name, ".framework", NULL);
    for (int i = 0; i < g_plugin_count; i++) {
        path = g_build_filename(g_plugin_ver_dirs[i], OS_NAME_3, ARCH_NAME, framework_name, name,
                                NULL);
        SDL_Log("[PLUGINS] Checking \"%s\"", path);
        if (g_file_test(path, G_FILE_TEST_EXISTS)) {
            g_free(module_name);
            g_free(framework_name);
            // FIXME: resource leak, should cache the path
            return (const char*)path;
        }
        g_free(path);
    }
    g_free(framework_name);
#endif

    // First check within plugin os/arch directories
    for (int i = 0; i < g_plugin_count; i++) {
#ifdef NEW_PLUGINS
        path2 = g_build_filename(g_plugin_ver_dirs[i], OS_NAME_3, ARCH_NAME, module_name, NULL);
#else
        path2 = g_build_filename(g_plugin_ver_dirs[i], os_arch_name(), module_name, NULL);
#endif
        SDL_Log("[PLUGINS] Checking \"%s\"\n", path2);
        if (g_file_test(path2, G_FILE_TEST_EXISTS)) {
            // g_free(module_name);
            //  FIXME: resource leak, should cache the path
            return (const char*)path2;
        }
        g_free(path2);
    }

    // Check outside arch directories
    for (int i = 0; i < g_plugin_count; i++) {
        path2 = g_build_filename(g_plugin_ver_dirs[i], module_name, NULL);
        SDL_Log("[PLUGINS] Checking \"%s\"\n", path2);
        if (g_file_test(path2, G_FILE_TEST_EXISTS)) {
            // g_free(module_name);
            //  FIXME: resource leak, should cache the path
            return (const char*)path2;
        }
        g_free(path2);
    }

    // Directly in old plugins dir
    // path = g_build_filename(fsuae_path_plugins_dir(), module_name, NULL);
    // SDL_Log("[PLUGINS] Checking \"%s\"\n", path);
    // if (g_file_test(path, G_FILE_TEST_EXISTS)) {
    //     g_free(module_name);
    //     // FIXME: resource leak, should cache the path
    //     return (const char*)path;
    // }
    // g_free(path);

    // Directly in (System)/Plugins/ (FIXME)
    path2 = g_build_filename(executable_dir, "Plugins", module_name, NULL);
    SDL_Log("[PLUGINS] Checking \"%s\"\n", path2);
    if (g_file_test(path2, G_FILE_TEST_EXISTS)) {
        // g_free(module_name);
        //  FIXME: resource leak if called more than once for the same
        //  plugin, should cache the path
        return (const char*)path2;
    }
    g_free(path2);

    // Sideloaded with executable
    path2 = g_build_filename(executable_dir, module_name, NULL);
    SDL_Log("[PLUGINS] Checking \"%s\"\n", path);
    if (g_file_test(path2, G_FILE_TEST_EXISTS)) {
        // g_free(module_name);
        //  FIXME: resource leak if called more than once for the same
        //  plugin, should cache the path
        return (const char*)path2;
    }
    g_free(path2);

    // g_free(module_name);
#endif

    // printf("\nEXIT\n");
    // exit(1);
    return NULL;
}

static char* lookup_plugin_in_dir(const char* name, const char* module_name,
                                  const char* directory) {
    SDL_Log("[PLUGINS] Looking up module \"%s\" in \"%s\"", name, directory);

    char path[FS_MAX_PATH];
    FS_CopyPath(path, directory);
    int reset = strlen(path);
    FS_AppendDirName(path, name);
    FS_AppendDirName(path, OS_NAME_3);
    FS_AppendDirName(path, ARCH_NAME);
    int reset2 = strlen(path);
    FS_AppendFileName(path, module_name);

    SDL_Log("Checking %s", path);
    if (FS_PathExists(path)) {
        return strdup(path);
    }

#if defined(_WIN32) || defined(__DARWIN__)
    // Case insensitive
#else
    char *c = path + reset2;
    while (*c) {
        *c = SDL_tolower(*c);
        c++;
    }
    SDL_Log("Checking %s", path);
    if (FS_PathExists(path)) {
        return strdup(path);
    }

#endif

    path[reset] = '\0';

#ifdef _WIN64
    // Special case for WinUAE compatibility (can share DLLs), prefer name_x64.dll on 64-bit
    // Windows (name.dll might be 32-bit if name_x64.dll is present).
    FS_AppendFileName(path, name);
    FS_AppendPath(path, "_x64.dll");
    SDL_Log("Checking %s", path);
    if (FS_PathExists(path)) {
        return strdup(path);
    }
    path[reset] = '\0';
#endif

    FS_AppendFileName(path, module_name);
    SDL_Log("Checking %s", path);
    if (FS_PathExists(path)) {
        return strdup(path);
    }

#if defined(_WIN32) || defined(__DARWIN__)
    // Case insensitive
#else
    c = path + reset;
    while (*c) {
        *c = SDL_tolower(*c);
        c++;
    }

    SDL_Log("Checking %s", path);
    if (FS_PathExists(path)) {
        return strdup(path);
    }

#endif

    return NULL;
}

#ifndef NEW_PLUGINS
static void load_plugin_provides(const char* path, GKeyFile* key_file, const char* group_name,
                                 int scan) {
    gchar** keys = g_key_file_get_keys(key_file, group_name, NULL, NULL);
    if (keys) {
        for (gchar* key = *keys; *key; key++) {
            gchar* value = g_key_file_get_string(key_file, group_name, key, NULL);
            if (value == NULL) {
                continue;
            }
            gchar* p = g_build_filename(path, value, NULL);
            if (g_file_test(p, G_FILE_TEST_EXISTS)) {
                g_hash_table_insert(provides, g_strdup(key), p);
            } else {
                g_free(p);
            }
        }
        g_strfreev(keys);
    }
    if (scan) {
        GDir* dir = g_dir_open(path, 0, NULL);
        if (dir) {
            const gchar* name;
            while ((name = g_dir_read_name(dir))) {
                gchar* p = g_build_filename(path, name, NULL);
                g_hash_table_insert(provides, g_strdup(name), p);
            }
            g_dir_close(dir);
        } else {
            SDL_Log("[PLUGINS] Could not open plugin dir %s\n", path);
        }
    }
}
#endif

static void load_plugin(const char* path, const char* ini_path) {
    SDL_Log("[PLUGINS] Loading %s\n", path);
#if NEW_PLUGINS
    gchar* version_path = g_build_filename(path, NULL);
#else
    GKeyFile* key_file = g_key_file_new();
    if (!g_key_file_load_from_file(key_file, ini_path, G_KEY_FILE_NONE, NULL)) {
        SDL_Log("[PLUGINS] Could not load plugin.ini\n");
        return;
    }
    gchar* version = g_key_file_get_string(key_file, "plugin", "version", NULL);
    if (version == NULL) {
        SDL_Log("[PLUGINS] Could not read plugin version\n");
        return;
    }
    gchar* version_path = g_build_filename(path, version, NULL);
    SDL_Log("[PLUGINS] -> %s\n", version_path);
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

static void load_plugins_from_dir(const char* plugins_dir) {
    SDL_Log("[PLUGINS] Loading plugins from %s\n", plugins_dir);
    GDir* dir = g_dir_open(plugins_dir, 0, NULL);
    if (!dir) {
        SDL_Log("[PLUGINS] Could not open plugins dir %s\n", plugins_dir);
        return;
    }
    const gchar* name;
    while ((name = g_dir_read_name(dir))) {
        gchar* p = g_build_filename(plugins_dir, name, NULL);
        gchar* p2 = g_build_filename(p, "Plugin.ini", NULL);
        // Legacy check
        gchar* p3 = g_build_filename(p, "Version.txt", NULL);
        SDL_Log("[PLUGINS] Checking %s\n", p2);
        if (g_file_test(p2, G_FILE_TEST_IS_REGULAR)) {
            load_plugin(p, NULL);
        } else if (g_file_test(p3, G_FILE_TEST_IS_REGULAR)) {
            load_plugin(p, NULL);
        }
        g_free(p3);
        g_free(p2);
        g_free(p);
    }
    g_dir_close(dir);
}

static struct {
    bool initialized;
} module;

static void fsuae_plugins_quit(void) {
    g_hash_table_destroy(provides);
}

void fsuae_plugins_init() {
    if (module.initialized) {
        return;
    }
    module.initialized = true;
    SDL_Log("[PLUGINS] Initializing\n");
    fsemu_module_on_quit(fsuae_plugins_quit);

    provides = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
    amiga_set_plugin_lookup_function(lookup_plugin);

    // First check FS-UAE/System
    load_plugins_from_dir(fsuae_path_system_dir());

    // Then the older FS-UAE/Plugins directory
    // load_plugins_from_dir(fsuae_path_plugins_dir());

    // Then load side-by-side plugins
    gchar* path;
    char executable_dir[FS_PATH_MAX];
    // fs_get_application_exe_dir(executable_dir, FS_PATH_MAX);
    fslib_path_application_dir(executable_dir, FS_PATH_MAX);
#if 0
    path = g_build_filename(
        executable_dir, "..", "lib", "fs-uae", "plugins", NULL);
    if (g_file_test(path, G_FILE_TEST_IS_DIR)) {
        load_plugins_from_dir(path);
    }
    g_free(path);
#endif
#ifdef MACOSX
    path = g_build_filename(executable_dir, "..", "..", "..", "..", "..", "Plugin.ini", NULL);
#else
    path = g_build_filename(executable_dir, "..", "..", "Plugin.ini", NULL);
#endif
    SDL_Log("[PLUGINS] Test %s\n", path);
    bool is_plugin = g_file_test(path, G_FILE_TEST_EXISTS);
    g_free(path);
    SDL_Log("[PLUGINS] Is plugin: %d\n", is_plugin);
    if (is_plugin) {
#ifdef MACOSX
        path = g_build_filename(executable_dir, "..", "..", "..", "..", "..", "..", NULL);
#else
        path = g_build_filename(executable_dir, "..", "..", "..", NULL);
#endif
        if (g_file_test(path, G_FILE_TEST_IS_DIR)) {
            load_plugins_from_dir(path);
        }
        g_free(path);
    }
}
