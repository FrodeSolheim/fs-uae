#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define _GNU_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <uae/uae.h>
#include <fs/base.h>
#include <fs/emu.h>
#include "fs-uae.h"
#include "config-common.h"
#include "paths.h"
#include "options.h"

void fs_uae_configure_directories(void)
{
    char *path;

    for (int i = 0; i < 10; i++) {
        char *src_name = g_strdup_printf("fs-uae-state_%d.uss", i);
        char *src = g_build_filename(fs_uae_state_dir(), src_name, NULL);
        g_free(src_name);
        char *dst_name = g_strdup_printf("Saved State %d.uss", i);
        char *dst = g_build_filename(fs_uae_state_dir(), dst_name, NULL);
        g_free(dst_name);
        if (fs_path_exists(src)) {
            fs_log("renaming file %s to %s\n", src, dst);
            if (g_rename(src, dst) != ZERO_SUCCESS) {
                fs_log("WARNING: renamed failed\n");
            }
        }
        g_free(src);
        g_free(dst);
    }
    char *state_base_name = g_build_filename(fs_uae_state_dir(),
            "Saved State", NULL);
    fs_uae_set_state_base_name(state_base_name);

    /*
    path = g_build_filename(fs_uae_state_dir(), "Autoload State.uss", NULL);
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

#ifdef SAVESTATE
    path = g_strconcat(state_base_name, ".uss", NULL);
    amiga_set_option("statefile", path);
    free(path);
#endif

    if (g_fs_uae_amiga_model == MODEL_CD32) {
        // legacy file name
        path = g_build_filename(fs_uae_state_dir(), "cd32.nvr", NULL);
        if (!fs_path_exists(path)) {
            free(path);
            // new file name
            path = g_build_filename(fs_uae_state_dir(), "CD32 Storage.nvr", NULL);
        }
        amiga_set_option("flash_file", path);
        free(path);
    }
    else if (g_fs_uae_amiga_model == MODEL_CDTV) {
        // legacy file name
        path = g_build_filename(fs_uae_state_dir(), "cdtv.nvr", NULL);
        if (!fs_path_exists(path)) {
            free(path);
            // new file name
            path = g_build_filename(fs_uae_state_dir(), "CDTV Storage.nvr", NULL);
        }
        amiga_set_option("flash_file", path);
        free(path);
    }
    free(state_base_name);

    amiga_set_save_image_dir(fs_uae_state_dir());
    amiga_set_module_ripper_dir(fs_uae_module_ripper_dir());
}

void fs_uae_set_uae_paths(void)
{
    fs_log("fs_uae_set_uae_paths\n");
    amiga_set_paths(g_paths[FS_UAE_ROM_PATHS].path,
            g_paths[FS_UAE_FLOPPY_PATHS].path,
            g_paths[FS_UAE_CD_PATHS].path,
            g_paths[FS_UAE_HD_PATHS].path);

    static const char *library_dirs[2];
    library_dirs[0] = fs_uae_plugins_dir();
    library_dirs[1] = NULL; // terminates the list
    amiga_set_native_library_dirs(library_dirs);

    // find path for built-in drive sounds
    char *path = fs_get_program_data_file("floppy_sounds");
    if (path) {
        fs_log("found \"built-in\" driveclick directory at %s\n", path);
        amiga_set_builtin_driveclick_path(path);
        free(path);
    }
    else {
        fs_log("did not find \"built-in\" driveclick directory\n");
    }

    // find path for custom drive sounds
    path = g_build_filename(fs_uae_base_dir(), "Floppy Sounds", NULL);
    amiga_set_floppy_sounds_dir(path);
    free(path);
}
