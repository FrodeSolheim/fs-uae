#define FSUAE_INTERNAL
#include "fsuae-path.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define _GNU_SOURCE 1
#include <fs/base.h>
#include <fs/emu.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uae/uae.h>

#include "fs-uae.h"
#include "fsuae-config.h"
#include "fsuae-model.h"
#include "fsuae-options.h"
// #include "fsuae-path.h"

static struct {
    bool initialized;
} module;

struct multipath g_paths[5] = {};
static char *g_fs_uae_state_base_name;

static const char *fsuae_path_home_dir(void)
{
    static const char *path;
    if (!path) {
        path = fs_get_home_dir();
        if (path == NULL) {
            fsuae_log("WARNING: did not find home directory\n");
            path = g_strdup("");
        }
        fsuae_log("- using home directory \"%s\"\n", path);
    }
    return path;
}

const char *fsuae_path_exe_dir(void)
{
    static const char *path;
    if (!path) {
        char *app_dir = (char *) malloc(MAX_PATH);
        fs_get_application_exe_dir(app_dir, MAX_PATH);
        char *p = g_strdup(app_dir);
        g_free(app_dir);
        path = p;
        fsuae_log("- using $exe directory \"%s\"\n", path);
    }
    return path;
}

static const char *fsuae_path_app_dir()
{
    static const char *path;
    if (!path) {
        char *app_dir = (char *) malloc(MAX_PATH);
        fs_get_application_exe_dir(app_dir, MAX_PATH);
        char *p = g_strdup(app_dir);
        g_free(app_dir);
        path = p;
#ifdef MACOSX
        char *base_name = g_path_get_basename(p);
        if (strcmp(base_name, "MacOS") == 0) {
            char *temp;
            temp = p;
            p = g_path_get_dirname(p);
            g_free(temp);
            temp = p;
            p = g_path_get_dirname(p);
            g_free(temp);
            temp = p;
            p = g_path_get_dirname(p);
            g_free(temp);
            path = p;
        }
        free(base_name);
#endif
        fsuae_log("- using $app directory \"%s\"\n", path);
    }
    return path;
}

static const char *fsuae_path_documents_dir()
{
    static const char *path;
    if (!path) {
        path = fs_get_documents_dir();
        if (path == NULL) {
            fsuae_log("WARNING: did not find documents directory\n");
            path = fsuae_path_home_dir();
        }
        int result = g_mkdir_with_parents(path, 0755);
        if (result == -1) {
            fs_emu_warning("Documents directory does not exist: %s", path);
            path = fsuae_path_home_dir();
        }
        fsuae_log("- using documents directory \"%s\"\n", path);
    }
    return path;
}

static char *read_custom_path(const char *key)
{
    char *key_path =
        g_build_filename(fse_user_config_dir(), "fs-uae", key, NULL);
    fsuae_log("- checking %s\n", key_path);
    if (fs_path_is_file(key_path)) {
        FILE *f = fopen(key_path, "rb");
        g_free(key_path);
        if (f == NULL) {
            fsuae_log("- file exists but could not open\n");
            return NULL;
        }
        char *buffer = (char *) malloc(PATH_MAX + 1);
        int read_bytes = fread(buffer, 1, PATH_MAX, f);
        int eof = feof(f);
        fclose(f);
        if (!eof) {
            fsuae_log("- did not get EOF\n");
            free(buffer);
            return NULL;
        }
        buffer[read_bytes] = '\0';
        g_strchomp(buffer);
        fsuae_log("- read from file: %s\n", buffer);
        char *result = fsuae_path_expand(buffer);
        free(buffer);
        fsuae_log("- expanded path: %s\n", result);
        return result;
    } else {
        g_free(key_path);
    }
    return NULL;
}

const char *fsuae_path_base_dir(void)
{
    static const char *path;
    if (path) {
        return path;
    }

    path = fs_config_get_const_string("base_dir");
    if (path) {
        fsuae_log("base specified via base_dir option\n");
        path = fsuae_path_expand(path);
    }
    if (path == NULL) {
        // FIXME: deprecated
        const char *env_path = getenv("fsuae_path_base_dir");
        if (env_path && env_path[0]) {
            path = env_path;
            fsuae_log("base specified via fsuae_path_base_dir\n");
            fs_emu_deprecated("fsuae_path_base_dir is deprecated");
        }
    }
    if (path == NULL) {
        // check for portable dir
        char buffer[MAX_PATH];
        fs_get_application_exe_dir(buffer, MAX_PATH);
        char *next = g_strdup(buffer);
        char *orig = g_strdup(":INVALID;");
        while (strcmp(orig, next) != 0) {
            char *test = g_build_filename(next, "Portable.ini", NULL);
            fsuae_log("checking %s\n", test);
            if (fs_path_exists(test)) {
                path = next;
                fsuae_log("using portable base dir %s\n", path);
                g_free(orig);
                break;
            }
            g_free(test);
            g_free(orig);
            orig = next;
            next = g_path_get_dirname(next);
        }
    }
    if (path == NULL) {
        path = read_custom_path("base-dir");
    }
    if (path == NULL) {
        fsuae_log("- using base dir $DOCUMENTS/FS-UAE\n");
        path = g_build_filename(fsuae_path_documents_dir(), "FS-UAE", NULL);
    }

    int result = g_mkdir_with_parents(path, 0755);
    if (result == -1) {
        fs_emu_warning("Could not create base directory at %s", path);
        path = fsuae_path_documents_dir();
    }
    fsuae_log("- using base ($BASE / $FSUAE) directory \"%s\"\n", path);
    return path;
}

static char *get_or_create_default_dir(const char *name,
                                       const char *key1,
                                       const char *key2,
                                       const char *dashed_key,
                                       int create,
                                       int cache)
{
    char *path = NULL;

    if (path == NULL && key1 != NULL) {
        path = fs_config_get_string(key1);
    }
    if (path == NULL && key2 != NULL) {
        path = fs_config_get_string(key2);
    }
    if (path == NULL && dashed_key != NULL) {
        path = read_custom_path(dashed_key);
    }
    if (path == NULL) {
        if (cache) {
            path = g_build_filename(fsuae_path_cache_dir(), name, NULL);
        } else {
            path = g_build_filename(fsuae_path_base_dir(), name, NULL);
        }
    }
    char *expanded_path = fsuae_path_expand_and_free(path);
    path = fsuae_path_resolve(expanded_path, FS_UAE_DIR_PATHS);
    free(expanded_path);

    if (create) {
        int result = g_mkdir_with_parents(path, 0755);
        if (result == -1) {
            fs_emu_warning("Could not create %s directory", name);
            free(path);
            path = g_strdup(fsuae_path_base_dir());
        }
    }
    fsuae_log("- using \"%s\" directory \"%s\"\n", name, path);
    return path;
}

static char *create_default_cache_dir(const char *name,
                                      const char *key1,
                                      const char *key2,
                                      const char *dashed_key)
{
    return get_or_create_default_dir(name, key1, key2, dashed_key, 1, 1);
}

static char *create_default_dir(const char *name,
                                const char *key1,
                                const char *key2,
                                const char *dashed_key)
{
    return get_or_create_default_dir(name, key1, key2, dashed_key, 1, 0);
}

static char *get_default_dir(const char *name,
                             const char *key1,
                             const char *key2,
                             const char *dashed_key)
{
    return get_or_create_default_dir(name, key1, key2, dashed_key, 0, 0);
}

const char *fsuae_path_kickstarts_dir()
{
    static const char *path;
    if (!path) {
        path = create_default_dir(
            "Kickstarts", "kickstarts_dir", "roms_dir", "kickstarts-dir");
    }
    return path;
}

const char *fsuae_path_configs_dir()
{
    static const char *path;
    if (!path) {
        path = create_default_dir("Configurations",
                                  "configurations_dir",
                                  NULL,
                                  "configurations-dir");
    }
    return path;
}

static const char *fsuae_path_savestates_dir()
{
    static const char *path;
    if (!path) {
        path = create_default_dir(
            "Save States", "save_states_dir", NULL, "save-states-dir");
    }
    return path;
}

static const char *fsuae_path_state_dir_path()
{
    char *free_state_dir_name = NULL;
    static const char *path;
    if (!path) {
        fsuae_log("fsuae_path_state_dir:\n");
        path = fs_config_get_const_string("state_dir");
        if (path && path[0]) {
            fsuae_log("state_dir was explicitly set to: %s\n", path);
            char *expanded_path = fsuae_path_expand(path);
            path = fsuae_path_resolve(expanded_path, FS_UAE_DIR_PATHS);
            free(expanded_path);
            return path;
        }
        const char *base = fsuae_path_savestates_dir();
        fsuae_log("save_states_dir: %s\n", base);
        const char *state_dir_name =
            fs_config_get_const_string("state_dir_name");
        if (!state_dir_name || !state_dir_name[0]) {
            if (g_fs_uae_config_file_path) {
                char *n = g_path_get_basename(g_fs_uae_config_file_path);
                for (int i = strlen(n) - 1; i >= 0; i--) {
                    if (n[i] == '.') {
                        n[i] = '\0';
                        break;
                    }
                }
                free_state_dir_name = g_strdup(n);
                state_dir_name = free_state_dir_name;
                g_free(n);
            }
            if (!state_dir_name || !state_dir_name[0]) {
                state_dir_name = "Default";
            }
            fsuae_log("save_dir_name not set, using %s\n", state_dir_name);
        }
        path = g_build_filename(base, state_dir_name, NULL);
    }
    if (free_state_dir_name) {
        g_free(free_state_dir_name);
    }
    return path;
}

const char *fsuae_path_state_dir()
{
    static const char *path;
    if (!path) {
        path = fsuae_path_state_dir_path();
        if (!path || !path[0]) {
            path = fsuae_path_base_dir();
            fsuae_log("reverting state dir to: %s\n", path);
        }
        fsuae_log("- using state dir %s\n", path);
        int result = g_mkdir_with_parents(path, 0755);
        if (result == -1) {
            fs_emu_warning("Could not create state directory");
            path = fsuae_path_base_dir();
        }
        fsuae_log("final state dir path: %s\n", path);
    }
    return path;
}

const char *fsuae_path_cdroms_dir()
{
    static const char *path;
    if (!path) {
        path = create_default_dir("CD-ROMs", "cdroms_dir", NULL, "cdroms-dir");
    }
    return path;
}

const char *fsuae_path_harddrives_dir()
{
    static const char *path;
    if (!path) {
        path = create_default_dir(
            "Hard Drives", "hard_drives_dir", NULL, "hard-drives-dir");
    }
    return path;
}

const char *fsuae_path_floppies_dir()
{
    static const char *path;
    if (!path) {
        path = create_default_dir(
            "Floppies", "floppies_dir", NULL, "floppies-dir");
    }
    return path;
}

const char *fsuae_path_controllers_dir()
{
    static const char *path;
    if (!path) {
        path = create_default_dir(
            "Controllers", "controllers_dir", NULL, "controllers-dir");
    }
    return path;
}

const char *fsuae_path_logs_dir()
{
    static const char *path;
    if (!path) {
        path = create_default_cache_dir("Logs", "logs_dir", NULL, "logs-dir");
    }
    return path;
}

const char *fsuae_path_moduleripper_dir()
{
    static const char *path;
    if (!path) {
        path = create_default_cache_dir(
            "Modules", "module_ripper_dir", NULL, "module-ripper-dir");
    }
    return path;
}

const char *fsuae_path_cache_dir(void)
{
    static const char *path;
    if (!path) {
        path = create_default_dir("Cache", "cache_dir", NULL, "cache-dir");
    }
    return path;
}

const char *fsuae_path_data_dir(void)
{
    static const char *path;
    if (!path) {
        path = create_default_dir("Data", "data_dir", NULL, "data-dir");
    }
    return path;
}

const char *fsuae_path_kickstartcache_dir()
{
    static const char *path;
    if (!path) {
        path = g_build_filename(fsuae_path_cache_dir(), "Kickstarts", NULL);
        int result = g_mkdir_with_parents(path, 0755);
        if (result == -1) {
            fs_emu_warning("Could not create kickstarts cache directory");
            path = fsuae_path_base_dir();
        }
    }
    return path;
}

const char *fsuae_path_themes_dir()
{
    static const char *path;
    if (!path) {
        path = create_default_dir("Themes", "themes_dir", NULL, "themes-dir");
    }
    return path;
}

const char *fsuae_path_plugins_dir(void)
{
    static const char *path;
    if (!path) {
        path = get_default_dir("Plugins", "plugins_dir", NULL, "plugins-dir");
    }
    return path;
}

const char *fsuae_path_system_dir(void)
{
    static const char *path;
    if (!path) {
        path = get_default_dir("System", "system_dir", NULL, "system-dir");
    }
    return path;
}

static const char *fsuae_path_temp_dir(void)
{
    static const char *path = NULL;
    if (path == NULL) {
        path = g_dir_make_tmp("fs-uae-TEMP-XXXXXX", NULL);
        fsuae_log("FS-UAE $TEMP = %s\n", path);
    }
    return path;
}

static bool check_path_prefix(const char *path, const char *prefix, int *r)
{
    if (g_str_has_prefix(path, prefix)) {
        *r = strlen(prefix);
        if (path[*r] == '/' || path[*r] == '\\') {
            *r += 1;
            return true;
        } else if (path[*r] == '\0') {
            return true;
        }
    }
    return false;
}

char *fsuae_path_expand(const char *path)
{
    char *lower = g_ascii_strdown(path, -1);
    int replace = 0;
    const char *replace_with = NULL;

    if (check_path_prefix(lower, "~", &replace)) {
        replace_with = fsuae_path_home_dir();
    } else if (check_path_prefix(lower, "$home", &replace)) {
        replace_with = fsuae_path_home_dir();
    } else if (check_path_prefix(lower, "$app", &replace)) {
        replace_with = fsuae_path_app_dir();
    } else if (check_path_prefix(lower, "$exe", &replace)) {
        replace_with = fsuae_path_exe_dir();
    } else if (check_path_prefix(lower, "$fsuae", &replace)) {
        replace_with = fsuae_path_base_dir();
    } else if (check_path_prefix(lower, "$base", &replace)) {
        replace_with = fsuae_path_base_dir();
    } else if (check_path_prefix(lower, "$documents", &replace)) {
        replace_with = fsuae_path_documents_dir();
    } else if (check_path_prefix(lower, "$config", &replace)) {
        replace_with = g_fs_uae_config_dir_path;
    } else if (check_path_prefix(lower, "$temp", &replace)) {
        replace_with = fsuae_path_temp_dir();
    }

    free(lower);
    if (replace_with) {
        const char *src = path + replace;
        return g_build_filename(replace_with, src, NULL);
    } else {
        return g_strdup(path);
    }
}

char *fsuae_path_expand_and_free(char *path)
{
    char *p = fsuae_path_expand(path);
    free(path);
    return p;
}

const char *fs_uae_get_state_base_name()
{
    return g_fs_uae_state_base_name;
}

void fs_uae_set_state_base_name(const char *base_name)
{
    g_fs_uae_state_base_name = g_strdup(base_name);
}

static void fix_separators(char *path)
{
#ifdef WINDOWS
    char *p = path;
    while (*p) {
        if (*p == '\\') {
            *p = '/';
        }
        p++;
    }
#endif
}

char *fsuae_path_resolve(const char *name, int type)
{
    if (name[0] == '\0') {
        fsuae_log("resolve_path (empty string)\n");
        return g_strdup("");
    } else if (g_path_is_absolute(name)) {
        fsuae_log("resolve_path %s (absolute)\n", name);
        char *path = g_strdup(name);
        fix_separators(path);
        return path;
    } else {
        fsuae_log("resolve_path %s (relative)\n", name);
        for (int i = 0; i < MAX_PATHS; i++) {
            if (!g_paths[type].path[i] || g_paths[type].path[i][0] == '\0') {
                continue;
            }
            char *path = g_build_filename(g_paths[type].path[i], name, NULL);
            fsuae_log("checking %s\n", path);
            if (fs_path_exists(path)) {
                fsuae_log("- found %s\n", path);
                fix_separators(path);
                return path;
            }
            free(path);
        }
    }
    fsuae_log("WARNING: did not find path %s\n", name);
    char *path = g_strdup(name);
    fix_separators(path);
    return path;
}

char *fsuae_path_resolve_and_free(char *name, int type)
{
    char *result = fsuae_path_resolve(name, type);
    free(name);
    return result;
}

void fs_uae_init_path_resolver(void)
{
    bool relative_paths = true;
    if (fs_config_false(OPTION_RELATIVE_PATHS)) {
        relative_paths = false;
    }

    int k;
    k = 0;
    if (relative_paths) {
        /* Current working directory should always come first (index 0) */
        g_paths[FS_UAE_DIR_PATHS].path[k++] = g_strdup(".");
    }
    if (g_fs_uae_config_dir_path) {
        g_paths[FS_UAE_DIR_PATHS].path[k++] =
            g_strdup(g_fs_uae_config_dir_path);
    }

    k = 0;
    if (relative_paths) {
        g_paths[FS_UAE_FLOPPY_PATHS].path[k++] = g_strdup(".");
    }
    if (g_fs_uae_config_dir_path) {
        g_paths[FS_UAE_FLOPPY_PATHS].path[k++] =
            g_strdup(g_fs_uae_config_dir_path);
    }
    if (fsuae_path_floppies_dir()) {
        g_paths[FS_UAE_FLOPPY_PATHS].path[k++] =
            g_strdup(fsuae_path_floppies_dir());
    }

    k = 0;
    if (relative_paths) {
        g_paths[FS_UAE_CD_PATHS].path[k++] = g_strdup(".");
    }
    if (g_fs_uae_config_dir_path) {
        g_paths[FS_UAE_CD_PATHS].path[k++] =
            g_strdup(g_fs_uae_config_dir_path);
    }
    if (fsuae_path_cdroms_dir()) {
        g_paths[FS_UAE_CD_PATHS].path[k++] = g_strdup(fsuae_path_cdroms_dir());
    }

    k = 0;
    if (relative_paths) {
        g_paths[FS_UAE_HD_PATHS].path[k++] = g_strdup(".");
    }
    if (g_fs_uae_config_dir_path) {
        g_paths[FS_UAE_HD_PATHS].path[k++] =
            g_strdup(g_fs_uae_config_dir_path);
    }
    if (fsuae_path_harddrives_dir()) {
        g_paths[FS_UAE_HD_PATHS].path[k++] =
            g_strdup(fsuae_path_harddrives_dir());
    }

    k = 0;
    if (relative_paths) {
        g_paths[FS_UAE_ROM_PATHS].path[k++] = g_strdup(".");
    }
    if (g_fs_uae_config_dir_path) {
        g_paths[FS_UAE_ROM_PATHS].path[k++] =
            g_strdup(g_fs_uae_config_dir_path);
    }
    if (fsuae_path_kickstarts_dir()) {
        g_paths[FS_UAE_ROM_PATHS].path[k++] =
            g_strdup(fsuae_path_kickstarts_dir());
    }
}

#ifndef DEVICE_HELPER

void fs_uae_configure_directories(void)
{
    char *path;

    /*
    path = g_build_filename(fsuae_path_state_dir(), "Autoload State.uss",
    NULL); if (fs_path_exists(path)) { fsuae_log("found autoload state at %s\n",
    path); amiga_set_option("statefile", path);
    }
    free(path);
    */

    /*
    path = fs_strconcat(state_base_name, ".uss", NULL);
    amiga_set_option("statefile_name", path);
    free(path);
    */

    if (g_fs_uae_amiga_model == MODEL_CD32) {
        // legacy file name
        path = g_build_filename(fsuae_path_state_dir(), "cd32.nvr", NULL);
        if (!fs_path_exists(path)) {
            free(path);
            // new file name
            path = g_build_filename(
                fsuae_path_state_dir(), "CD32 Storage.nvr", NULL);
        }
        amiga_set_option("flash_file", path);
        free(path);
    } else if (g_fs_uae_amiga_model == MODEL_CDTV) {
        // legacy file name
        path = g_build_filename(fsuae_path_state_dir(), "cdtv.nvr", NULL);
        if (!fs_path_exists(path)) {
            free(path);
            // new file name
            path = g_build_filename(
                fsuae_path_state_dir(), "CDTV Storage.nvr", NULL);
        }
        amiga_set_option("flash_file", path);
        free(path);
    }

    amiga_set_save_image_dir(fsuae_path_state_dir());
    amiga_set_module_ripper_dir(fsuae_path_moduleripper_dir());
}

void fs_uae_set_uae_paths(void)
{
    fsuae_log("fs_uae_set_uae_paths\n");
    amiga_set_paths(g_paths[FS_UAE_ROM_PATHS].path,
                    g_paths[FS_UAE_FLOPPY_PATHS].path,
                    g_paths[FS_UAE_CD_PATHS].path,
                    g_paths[FS_UAE_HD_PATHS].path);

    static const char *library_dirs[2];
    library_dirs[0] = fsuae_path_plugins_dir();
    library_dirs[1] = NULL;  // terminates the list
    amiga_set_native_library_dirs(library_dirs);

#if 0
    // find path for built-in drive sounds
    char *path = fs_get_program_data_file("floppy_sounds");
    if (path) {
        fsuae_log("found \"built-in\" driveclick directory at %s\n", path);
        amiga_set_builtin_driveclick_path(path);
        free(path);
    } else {
        fsuae_log("did not find \"built-in\" driveclick directory\n");
    }
#endif

    // find path for custom drive sounds
    char *path =
        g_build_filename(fsuae_path_base_dir(), "Floppy Sounds", NULL);
    amiga_set_floppy_sounds_dir(path);
    free(path);
}

#endif

void fsuae_path_init(void)
{
    if (module.initialized) {
        return;
    }
    module.initialized = true;
}
