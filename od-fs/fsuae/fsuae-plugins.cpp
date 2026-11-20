#define FSUAE_INTERNAL
#include "fsuae-plugins.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <SDL3/SDL.h>
#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <uae/uae.h>

#include <string>

#include "fs/path.h"
#include "fsemu-module.h"
#include "fsuae-init.h"
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

static GHashTable *provides;
#define MAX_PLUGINS 256
static char *g_plugin_ver_dirs[MAX_PLUGINS];
static int g_plugin_count;

#ifndef NEW_PLUGINS
static const char *os_arch_name()
{
    static const char *name = NULL;
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

static char *lookup_plugin_in_dir(const char *name, const std::string &module_name,
                                  const std::string &directory);

// Returns s, a string to be freed with SDL_free (from FS_JoinPath, for example), as a
// std::string, and frees it.
static std::string take_string(char *s)
{
    std::string result = s != NULL ? s : "";
    SDL_free(s);
    return result;
}

/**
 * @brief This function will be called from UAE it tries to load a plugin library.
 *
 * @param name Name of the library (without .dll or .so extension)
 * @return Returns the full path of the library (or NULL if not found)
 */
static const char *lookup_plugin(const char *name)
{
    char *result;

    std::string module_name = std::string(name) + LT_MODULE_EXT;
    SDL_Log("[PLUGINS] Looking up module \"%s\"\n", name);

    result = lookup_plugin_in_dir(name, module_name,
                                  take_string(FS_JoinPath(fsuae_init_get_data_dir(), "Plugins")));
    if (result) {
        return result;
    }

    // The plugins that come with FS-UAE: <root>/Plugins in the dist layout,
    // Contents/Resources/Plugins in a macOS app bundle (see fsuae-path.cpp and
    // build/dist/assemble.sh).
    const char *plugins_dir = fsuae_path_get_plugins_dir();
    if (plugins_dir != NULL) {
        result = lookup_plugin_in_dir(name, module_name, plugins_dir);
        if (result) {
            return result;
        }
    }

    // The application's directory (on macOS, the one containing the .app bundle).
    const char *application_dir = fsuae_path_get_application_dir();
    if (application_dir != NULL) {
        result = lookup_plugin_in_dir(name, module_name, application_dir);
        if (result) {
            return result;
        }
    }

    return NULL;
}

// Returns a copy of path (to be freed with free) if it exists, otherwise NULL.
static char *plugin_path_if_exists(const std::string &path)
{
    SDL_Log("Checking %s", path.c_str());
    if (SDL_GetPathInfo(path.c_str(), NULL)) {
        return strdup(path.c_str());
    }
    return NULL;
}

#if !defined(_WIN32) && !defined(MACOSX)
// Like plugin_path_if_exists, for path with the part from start lower-cased, if that is different
// from path (file systems on Windows and macOS are usually case insensitive).
static char *plugin_path_if_exists_lower_case(const std::string &path, size_t start)
{
    std::string lower = path;
    for (size_t i = start; i < lower.size(); i++) {
        lower[i] = SDL_tolower(lower[i]);
    }
    if (lower == path) {
        return NULL;
    }
    return plugin_path_if_exists(lower);
}
#endif

static char *lookup_plugin_in_dir(const char *name, const std::string &module_name,
                                  const std::string &directory)
{
    SDL_Log("[PLUGINS] Looking up module \"%s\" in \"%s\"", name, directory.c_str());
    char *result;

    // <directory>/<name>/<OS>/<arch>/
    std::string arch_dir = take_string(FS_JoinPath(directory.c_str(), name, OS_NAME_3, ARCH_NAME));

#ifdef MACOSX
    // <name>.framework/<name>
    std::string framework = std::string(name) + ".framework";
    result =
        plugin_path_if_exists(take_string(FS_JoinPath(arch_dir.c_str(), framework.c_str(), name)));
    if (result) {
        return result;
    }
#endif

    std::string path = take_string(FS_JoinPath(arch_dir.c_str(), module_name.c_str()));
    result = plugin_path_if_exists(path);
    if (result) {
        return result;
    }
#if !defined(_WIN32) && !defined(MACOSX)
    result = plugin_path_if_exists_lower_case(path, path.size() - module_name.size());
    if (result) {
        return result;
    }
#endif

#ifdef _WIN64
    // Special case for WinUAE compatibility (can share DLLs), prefer name_x64.dll on 64-bit
    // Windows (name.dll might be 32-bit if name_x64.dll is present).
    std::string dll_name = std::string(name) + "_x64.dll";
    result = plugin_path_if_exists(take_string(FS_JoinPath(directory.c_str(), dll_name.c_str())));
    if (result) {
        return result;
    }
#endif

    path = take_string(FS_JoinPath(directory.c_str(), module_name.c_str()));
    result = plugin_path_if_exists(path);
    if (result) {
        return result;
    }
#if !defined(_WIN32) && !defined(MACOSX)
    result = plugin_path_if_exists_lower_case(path, path.size() - module_name.size());
    if (result) {
        return result;
    }
#endif

    return NULL;
}

#ifndef NEW_PLUGINS
static void load_plugin_provides(const char *path, GKeyFile *key_file, const char *group_name,
                                 int scan)
{
    gchar **keys = g_key_file_get_keys(key_file, group_name, NULL, NULL);
    if (keys) {
        for (gchar *key = *keys; *key; key++) {
            gchar *value = g_key_file_get_string(key_file, group_name, key, NULL);
            if (value == NULL) {
                continue;
            }
            char *p = FS_JoinPath(path, value);
            if (SDL_GetPathInfo(p, NULL)) {
                g_hash_table_insert(provides, g_strdup(key), p);
            } else {
                SDL_free(p);
            }
        }
        g_strfreev(keys);
    }
    if (scan) {
        GDir *dir = g_dir_open(path, 0, NULL);
        if (dir) {
            const gchar *name;
            while ((name = g_dir_read_name(dir))) {
                char *p = FS_JoinPath(path, name);
                g_hash_table_insert(provides, g_strdup(name), p);
            }
            g_dir_close(dir);
        } else {
            SDL_Log("[PLUGINS] Could not open plugin dir %s\n", path);
        }
    }
}
#endif

static void load_plugin(const char *path, const char *ini_path)
{
    SDL_Log("[PLUGINS] Loading %s\n", path);
#if NEW_PLUGINS
    char *version_path = FS_JoinPath(path);
#else
    GKeyFile *key_file = g_key_file_new();
    if (!g_key_file_load_from_file(key_file, ini_path, G_KEY_FILE_NONE, NULL)) {
        SDL_Log("[PLUGINS] Could not load plugin.ini\n");
        return;
    }
    gchar *version = g_key_file_get_string(key_file, "plugin", "version", NULL);
    if (version == NULL) {
        SDL_Log("[PLUGINS] Could not read plugin version\n");
        return;
    }
    char *version_path = FS_JoinPath(path, version);
    SDL_Log("[PLUGINS] -> %s\n", version_path);
    load_plugin_provides(version_path, key_file, "provides", 0);

    load_plugin_provides(version_path, key_file, os_arch_name(), 1);
#endif
    if (g_plugin_count < MAX_PLUGINS) {
        g_plugin_ver_dirs[g_plugin_count] = version_path;
        g_plugin_count += 1;
    } else {
        SDL_free(version_path);
    }
#ifndef NEW_PLUGINS
    g_free(version);
    g_key_file_free(key_file);
#endif
}

static void load_plugins_from_dir(const char *plugins_dir)
{
    SDL_Log("[PLUGINS] Loading plugins from %s\n", plugins_dir);
    GDir *dir = g_dir_open(plugins_dir, 0, NULL);
    if (!dir) {
        SDL_Log("[PLUGINS] Could not open plugins dir %s\n", plugins_dir);
        return;
    }
    const gchar *name;
    while ((name = g_dir_read_name(dir))) {
        char *p = FS_JoinPath(plugins_dir, name);
        char *p2 = FS_JoinPath(p, "Plugin.ini");
        // Legacy check
        char *p3 = FS_JoinPath(p, "Version.txt");
        SDL_Log("[PLUGINS] Checking %s\n", p2);
        if (FS_IsFile(p2)) {
            load_plugin(p, NULL);
        } else if (FS_IsFile(p3)) {
            load_plugin(p, NULL);
        }
        SDL_free(p3);
        SDL_free(p2);
        SDL_free(p);
    }
    g_dir_close(dir);
}

static struct {
    bool initialized;
} module;

static void fsuae_plugins_quit(void)
{
    g_hash_table_destroy(provides);
}

void fsuae_plugins_init()
{
    if (module.initialized) {
        return;
    }
    module.initialized = true;
    SDL_Log("[PLUGINS] Initializing\n");
    fsemu_module_on_quit(fsuae_plugins_quit);

    provides = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
    amiga_set_plugin_lookup_function(lookup_plugin);

    // Then load side-by-side plugins
    char *path;
    const char *application_dir = fsuae_path_get_application_dir();
#if 0
    path = FS_JoinPath(application_dir, "..", "lib", "fs-uae", "plugins");
    if (FS_IsDirectory(path)) {
        load_plugins_from_dir(path);
    }
    SDL_free(path);
#endif
#ifdef MACOSX
    path = FS_JoinPath(application_dir, "..", "..", "..", "..", "..", "Plugin.ini");
#else
    path = FS_JoinPath(application_dir, "..", "..", "Plugin.ini");
#endif
    SDL_Log("[PLUGINS] Test %s\n", path);
    bool is_plugin = SDL_GetPathInfo(path, NULL);
    SDL_free(path);
    SDL_Log("[PLUGINS] Is plugin: %d\n", is_plugin);
    if (is_plugin) {
#ifdef MACOSX
        path = FS_JoinPath(application_dir, "..", "..", "..", "..", "..", "..");
#else
        path = FS_JoinPath(application_dir, "..", "..", "..");
#endif
        if (FS_IsDirectory(path)) {
            load_plugins_from_dir(path);
        }
        SDL_free(path);
    }
}
