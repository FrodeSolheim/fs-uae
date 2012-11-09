#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <uae/uae.h>
#include <fs/base.h>
#include <fs/emu.h>
#include <glib.h>
#include "fs-uae.h"

#define MAX_PATHS 8

struct multipath {
    const char *path[MAX_PATHS];
};

static struct multipath g_paths[5] = {};

const char* fs_uae_home_dir() {
    static const char* path = NULL;
    if (path == NULL) {
        path = fs_get_home_dir();
        if (path == NULL) {
            fs_log("WARNING: did not find home directory\n");
            path = fs_strdup("");
        }
        fs_log("using home directory \"%s\"\n", path);
    }
    return path;
}

const char* fs_uae_exe_dir() {
    static const char* path = NULL;
    if (path == NULL) {
        char *app_dir = (char*) malloc(MAX_PATH);
        fs_get_application_exe_dir(app_dir, MAX_PATH);
        char *p = fs_strdup(app_dir);
        free(app_dir);
        path = p;
        fs_log("using $exe directory \"%s\"\n", path);
    }
    return path;
}

const char* fs_uae_app_dir() {
    static const char* path = NULL;
    if (path == NULL) {
        char *app_dir = (char*) malloc(MAX_PATH);
        fs_get_application_exe_dir(app_dir, MAX_PATH);
        char *p = fs_strdup(app_dir);
        free(app_dir);
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
        g_free(base_name);
#endif
        fs_log("using $app directory \"%s\"\n", path);
    }
    return path;
}

const char* fs_uae_documents_dir() {
    static const char* path = NULL;
    if (path == NULL) {
        path = fs_get_documents_dir();
        if (path == NULL) {
            fs_log("WARNING: did not find documents directory\n");
            path = fs_uae_home_dir();
        }
        int result = fs_mkdir_with_parents(path, 0755);
        if (result == -1) {
            char *msg = fs_strdup_printf("Documents directory does not "
                    "exist: %s", path);
            fs_emu_warning(msg);
            g_free(msg);
            path = fs_uae_home_dir();
        }
        fs_log("using documents directory \"%s\"\n", path);
    }
    return path;
}

const char* fs_uae_base_dir() {
    static const char* path = NULL;
    char *p = fs_config_get_string("base_dir");
    if (p) {
        p = fs_uae_expand_path_and_free(p);
        int result = fs_mkdir_with_parents(p, 0755);
        if (result == -1) {
            char *msg = fs_strdup_printf("Could not create specified "
                    "base dir");
            fs_emu_warning(msg);
            free(msg);
            free(p);
            p = NULL;
        }
        else {
            path = p;
        }
    }
    if (path == NULL) {
        path = fs_path_join(fs_uae_documents_dir(), "FS-UAE", NULL);
        int result = fs_mkdir_with_parents(path, 0755);
        if (result == -1) {
            char *msg = fs_strdup_printf("Could not create base directory "
                    "at %s", path);
            fs_emu_warning(msg);
            g_free(msg);
            path = fs_uae_documents_dir();
        }
        fs_log("using base ($fsuae) directory \"%s\"\n", path);
    }
    return path;
}

static char *create_default_dir(const char *name, const char *key1,
        const char *key2) {
    char *path = NULL;

    if (path == NULL && key1 != NULL) {
        path = fs_config_get_string(key1);
    }
    if (path == NULL && key2 != NULL) {
        path = fs_config_get_string(key2);
    }
    if (path == NULL) {
        path = fs_path_join(fs_uae_base_dir(), name, NULL);
    }
    char *expanded_path = fs_uae_expand_path_and_free(path);
    path = fs_uae_resolve_path(expanded_path, FS_UAE_DIR_PATHS);
    free(expanded_path);

    int result = fs_mkdir_with_parents(path, 0755);
    if (result == -1) {
        char *msg = fs_strdup_printf("Could not create %s directory", name);
        fs_emu_warning(msg);
        free(msg);
        path = fs_strdup(fs_uae_base_dir());
    }
    fs_log("using \"%s\" directory \"%s\"\n", name, path);
    return path;
}

const char *fs_uae_kickstarts_dir() {
    static const char *path = NULL;
    if (path == NULL) {
        path = create_default_dir("Kickstarts", "kickstarts_dir",
                "roms_dir");
    }
    return path;
}

const char *fs_uae_configurations_dir() {
    static const char *path = NULL;
    if (path == NULL) {
        path = create_default_dir("Configurations", NULL, NULL);
    }
    return path;
}

/*
const char *fs_uae_flash_memory_dir() {
    static const char *path = NULL;
    if (path == NULL) {
        path = create_default_dir("Flash Memory", "flash_memory_dir",
                "state_dir");
    }
    return path;
}
*/

const char *fs_uae_save_states_dir() {
    static const char *path = NULL;
    if (path == NULL) {
        path = create_default_dir("Save States", "save_states_dir",
                "state_dir");
    }
    return path;
}

const char *fs_uae_state_dir_path() {
    static const char *path = NULL;
    if (path == NULL) {
        fs_log("fs_uae_state_dir:\n");
        path = fs_config_get_const_string("state_dir");
        if (path && path[0]) {
            fs_log("state_dir was explicitly set to: %s\n", path);
            char *expanded_path = fs_uae_expand_path(path);
            path = fs_uae_resolve_path(expanded_path, FS_UAE_DIR_PATHS);
            free(expanded_path);
            return path;
        }
        const char *base = fs_uae_save_states_dir();
        fs_log("save_states_dir: %s\n", base);
        const char *state_dir_name = fs_config_get_const_string(
                "state_dir_name");
        if (!state_dir_name || !state_dir_name[0]) {
            if (g_fs_uae_config_file_path) {
                char *n = fs_path_get_basename(g_fs_uae_config_file_path);
                for (int i = strlen(n) - 1; i >= 0; i--) {
                    if (n[i] == '.') {
                        n[i] = '\0';
                        break;
                    }
                }
                state_dir_name = fs_strdup(n);
                free(n);
            }
            if (!state_dir_name || !state_dir_name[0]) {
                state_dir_name = "Default";
            }
            fs_log("save_dir_name not set, using %s\n", state_dir_name);
        }
        path = fs_path_join(base, state_dir_name, NULL);
    }
    return path;
}

const char *fs_uae_state_dir() {
    static const char *path = NULL;
    if (path == NULL) {
        path = fs_uae_state_dir_path();
        if (!path || !path[0]) {
            path = fs_uae_base_dir();
            fs_log("reverting state dir to: %s\n", path);
        }
        fs_log("using state dir %s\n", path);
        int result = fs_mkdir_with_parents(path, 0755);
        if (result == -1) {
            fs_emu_warning("Could not create state directory");
            path = fs_uae_base_dir();
        }
        fs_log("final state dir path: %s\n", path);
    }
    return path;
}

const char *fs_uae_cdroms_dir() {
    static const char *path = NULL;
    if (path == NULL) {
        path = create_default_dir("CD-ROMs", "cdroms_dir", NULL);
    }
    return path;
}

const char *fs_uae_hard_drives_dir() {
    static const char *path = NULL;
    if (path == NULL) {
        path = create_default_dir("Hard Drives", "hard_drives_dir", NULL);
    }
    return path;
}

const char *fs_uae_floppies_dir() {
    static const char *path = NULL;
    if (path == NULL) {
        path = create_default_dir("Floppies", "floppies_dir", NULL);
    }
    return path;
}

/*
const char *fs_uae_floppy_overlays_dir() {
    static const char *path = NULL;
    if (path == NULL) {
        path = create_default_dir("Floppy Overlays", "floppy_overlays_dir",
                "state_dir");
    }
    return path;
}
*/

const char *fs_uae_controllers_dir() {
    static const char *path = NULL;
    if (path == NULL) {
        path = create_default_dir("Controllers", "controllers_dir", NULL);
    }
    return path;
}

const char *fs_uae_logs_dir() {
    static const char *path = NULL;
    if (path == NULL) {
        path = create_default_dir("Logs", "logs_dir", NULL);
    }
    return path;
}

const char *fs_uae_themes_dir() {
    static const char *path = NULL;
    if (path == NULL) {
        path = create_default_dir("Themes", "themes_dir", NULL);
    }
    return path;
}

char *fs_uae_expand_path(const char* path) {
    char* lower = g_ascii_strdown(path, -1);
    int replace = 0;
    const char *replace_with = NULL;

    if (fs_str_has_prefix(lower, "~/") || g_str_has_prefix(lower, "~\\")) {
        replace = 2;
        replace_with = fs_uae_home_dir();
    }
    if (fs_str_has_prefix(lower, "$home/") ||
            fs_str_has_prefix(lower, "$home\\")) {
        replace = 6;
        replace_with = fs_uae_home_dir();
    }
    if (fs_str_has_prefix(lower, "$app/") ||
            fs_str_has_prefix(lower, "$app\\")) {
        replace = 5;
        replace_with = fs_uae_app_dir();
    }
    if (fs_str_has_prefix(lower, "$exe/") ||
            fs_str_has_prefix(lower, "$exe\\")) {
        replace = 5;
        replace_with = fs_uae_exe_dir();
    }
    if (fs_str_has_prefix(lower, "$fsuae/") ||
            fs_str_has_prefix(lower, "$fsuae\\")) {
        replace = 7;
        replace_with = fs_uae_base_dir();
    }
    if (fs_str_has_prefix(lower, "$base/") ||
            fs_str_has_prefix(lower, "$base\\")) {
        replace = 7;
        replace_with = fs_uae_base_dir();
    }
    if (fs_str_has_prefix(lower, "$documents/") ||
            fs_str_has_prefix(lower, "$documents\\")) {
        replace = 11;
        replace_with = fs_uae_documents_dir();
    }
    if (fs_str_has_prefix(lower, "$config/") ||
            fs_str_has_prefix(lower, "$config\\")) {
        replace = 8;
        replace_with = g_fs_uae_config_dir_path;
    }

    free(lower);
    if (replace && replace_with) {
        const char *src = path + replace;
        return fs_path_join(replace_with, src, NULL);
    }
    else {
        return fs_strdup(path);
    }
}

char *fs_uae_expand_path_and_free(char *path) {
    char *p = fs_uae_expand_path(path);
    free(path);
    return p;
}

static char* g_fs_uae_state_base_name = NULL;

const char *fs_uae_get_state_base_name() {
    return g_fs_uae_state_base_name;
}

void fs_uae_set_state_base_name(const char *base_name) {
    g_fs_uae_state_base_name = fs_strdup(base_name);
}

void fs_uae_configure_directories() {
    char *path;

    for (int i = 0; i < 10; i++) {
        char *src_name = fs_strdup_printf("fs-uae-state_%d.uss", i);
        char *src = fs_path_join(fs_uae_state_dir(), src_name, NULL);
        free(src_name);
        char *dst_name = fs_strdup_printf("Saved State %d.uss", i);
        char *dst = fs_path_join(fs_uae_state_dir(), src_name, NULL);
        free(dst_name);
        if (fs_path_exists(src)) {
            fs_log("renaming file %s to %s\n", src, dst);
            fs_rename(src, dst);
        }
        free(src);
        free(dst);
    }
    char *state_base_name = fs_path_join(fs_uae_state_dir(),
            "Saved State", NULL);
    fs_uae_set_state_base_name(state_base_name);

    /*
    path = fs_path_join(fs_uae_state_dir(), "Autoload State.uss", NULL);
    if (fs_path_exists(path)) {
        fs_log("found autoload state at %s\n", path);
        amiga_set_option("statefile", path);
    }
    free(path);
    */

    /*
    path = fs_strconcat(state_base_name, ".uss", NULL);
    amiga_set_option("statefile_name", path);
    free(path);
    */

    path = fs_strconcat(state_base_name, ".uss", NULL);
    amiga_set_option("statefile", path);
    free(path);

    if (g_fs_uae_amiga_model == MODEL_CD32) {
        // legacy file name
        path = fs_path_join(fs_uae_state_dir(), "cd32.nvr", NULL);
        if (!fs_path_exists(path)) {
            free(path);
            // new file name
            path = fs_path_join(fs_uae_state_dir(), "CD32 Storage.nvr", NULL);
        }
        amiga_set_option("flash_file", path);
        free(path);
    }
    else if (g_fs_uae_amiga_model == MODEL_CDTV) {
        // legacy file name
        path = fs_path_join(fs_uae_state_dir(), "cdtv.nvr", NULL);
        if (!fs_path_exists(path)) {
            free(path);
            // new file name
            path = fs_path_join(fs_uae_state_dir(), "CDTV Storage.nvr", NULL);
        }
        amiga_set_option("flash_file", path);
        free(path);
    }

    amiga_set_save_image_dir(fs_uae_state_dir());
    free(state_base_name);
}

static void fix_separators(char *path) {
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

char *fs_uae_resolve_path(const char *name, int type) {
    if (name[0] == '\0') {
        fs_log("resolve_path (empty string)\n");
        return fs_strdup("");
    }
    else if (fs_path_is_absolute(name)) {
        fs_log("resolve_path %s (absolute)\n", name);
        char *path = fs_strdup(name);
        fix_separators(path);
        return path;
    }
    else {
        fs_log("resolve_path %s (relative)\n", name);
        for (int i = 0; i < MAX_PATHS; i++) {
            if (!g_paths[type].path[i] || g_paths[type].path[i][0] == '\0') {
                continue;
            }
            char *path = fs_path_join(g_paths[type].path[i], name, NULL);
            fs_log("checking %s\n", path);
            if (fs_path_exists(path)) {
                fs_log("- found %s\n", path);
                fix_separators(path);
                return path;
            }
            g_free(path);
        }
    }
    fs_log("WARNING: did not find path\n", name);
    char *path = fs_strdup(name);
    fix_separators(path);
    return path;
}

char *fs_uae_resolve_path_and_free(char *name, int type) {
    char *result = fs_uae_resolve_path(name, type);
    free(name);
    return result;
}

void fs_uae_set_uae_paths() {
    fs_log("fs_uae_set_uae_paths\n");
    amiga_set_paths(g_paths[FS_UAE_ROM_PATHS].path,
            g_paths[FS_UAE_FLOPPY_PATHS].path,
            g_paths[FS_UAE_CD_PATHS].path,
            g_paths[FS_UAE_HD_PATHS].path);
}

void fs_uae_init_path_resolver() {
    int k;
    // current working directory should always come first (index 0)
    k = 0;
    g_paths[FS_UAE_DIR_PATHS].path[k++] = fs_strdup(".");
    if (g_fs_uae_config_dir_path) {
        g_paths[FS_UAE_DIR_PATHS].path[k++] = fs_strdup(
                g_fs_uae_config_dir_path);
    }
    // current working directory should always come first (index 0)
    k = 0;
    g_paths[FS_UAE_FLOPPY_PATHS].path[k++] = fs_strdup(".");
    if (g_fs_uae_config_dir_path) {
        g_paths[FS_UAE_FLOPPY_PATHS].path[k++] = fs_strdup(
                g_fs_uae_config_dir_path);
    }
    // then add the default floppies dir
    if (fs_uae_floppies_dir()) {
        g_paths[FS_UAE_FLOPPY_PATHS].path[k++] = fs_strdup(
                fs_uae_floppies_dir());
    }
    // similarly for other path types
    k = 0;
    g_paths[FS_UAE_CD_PATHS].path[k++] = fs_strdup(".");
    if (g_fs_uae_config_dir_path) {
        g_paths[FS_UAE_CD_PATHS].path[k++] = fs_strdup(
                g_fs_uae_config_dir_path);
    }
    if (fs_uae_cdroms_dir()) {
        g_paths[FS_UAE_CD_PATHS].path[k++] = fs_strdup(
                fs_uae_cdroms_dir());
    }
    k = 0;
    g_paths[FS_UAE_HD_PATHS].path[k++] = fs_strdup(".");
    if (g_fs_uae_config_dir_path) {
        g_paths[FS_UAE_HD_PATHS].path[k++] = fs_strdup(
                g_fs_uae_config_dir_path);
    }
    if (fs_uae_hard_drives_dir()) {
        g_paths[FS_UAE_HD_PATHS].path[k++] = fs_strdup(
                fs_uae_hard_drives_dir());

    }
    k = 0;
    g_paths[FS_UAE_ROM_PATHS].path[k++] = fs_strdup(".");
    if (g_fs_uae_config_dir_path) {
        g_paths[FS_UAE_ROM_PATHS].path[k++] = fs_strdup(
                g_fs_uae_config_dir_path);
    }
    if (fs_uae_kickstarts_dir()) {
        g_paths[FS_UAE_ROM_PATHS].path[k++] = fs_strdup(
                fs_uae_kickstarts_dir());
    }
}
