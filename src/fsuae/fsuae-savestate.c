#define FSUAE_INTERNAL
#include "fsuae-savestate.h"

#include "fs-uae.h"
#include "fsemu-glib.h"
#include "fsemu-savestate.h"
#include "fsemu-util.h"
#include "fsuae-path.h"

#define FSUAE_SAVESTATE_BASENAME "FS-UAE"

static char *fsuae_savestate_path(int slot)
{
    fsemu_assert(slot >= 0 && slot <= 9);

    char *state_base_name = g_build_filename(
        fsuae_path_state_dir(), FSUAE_SAVESTATE_BASENAME, NULL);
    char *result;
    if (slot > 0) {
        result = g_strdup_printf("%s_%d.uss", state_base_name, slot);
    } else {
        result = g_strdup_printf("%s.uss", state_base_name);
    }
    free(state_base_name);

    fsuae_log_debug("SAVESTATE SLOT %d -> %s\n", slot, result);
    return result;
}

#if 0
static bool fsuae_savestate_screenshot_path(void)
{
    
}
#endif

#define NO_ERROR 0

static void fsuae_savestate_rename(const char *src, const char *dst)
{
    if (fsemu_path_exists(src)) {
        fsemu_savestate_log("Renaming file %s to %s\n", src, dst);
        if (g_rename(src, dst) != NO_ERROR) {
            fsuae_log_warning("renamed failed\n");
        }
    }
}

static void fsuae_savestate_rename_old(void)
{
    char *src, *src_name, *dst;
    for (int i = 0; i < 10; i++) {
        dst = fsuae_savestate_path(i);

        src_name = g_strdup_printf("fs-uae-state_%d.uss", i);
        src = g_build_filename(fsuae_path_state_dir(), src_name, NULL);
        g_free(src_name);
        fsuae_savestate_rename(src, dst);
        g_free(src);

        src_name = g_strdup_printf("Saved State %d.uss", i);
        src = g_build_filename(fsuae_path_state_dir(), src_name, NULL);
        g_free(src_name);
        fsuae_savestate_rename(src, dst);
        g_free(src);

#if 0
        // FIXME: Was not in use
        src_name = g_strdup_printf("State_%d.uss", i);
        src = g_build_filename(fsuae_path_state_dir(), src_name, NULL);
        g_free(src_name);
        fsuae_savestate_rename(src, dst);
        g_free(src);
#endif

        g_free(dst);
    }
}

void fsuae_savestate_init(void)
{
    fsuae_log("[FSUAE] fsuae_savestate_init\n");
    // Force creation of state directories
    // fs_uae_flash_memory_dir();
    // fsuae_path_savestates_dir();
    // fs_uae_floppy_overlays_dir();
    fsuae_path_state_dir();

    fsuae_savestate_rename_old();

    char *state_base_name = g_build_filename(
        fsuae_path_state_dir(), FSUAE_SAVESTATE_BASENAME, NULL);
    // FIXME: Used???
    fs_uae_set_state_base_name(state_base_name);

    // uae_savestate_set_base_dir();
    // uae_savestate_set_home_dir();
    const char *run_dir = fs_config_get_const_string("run_dir");
    if (run_dir) {
        uae_savestate_set_run_dir(run_dir);
    }

#ifdef SAVESTATE
    char *path = g_strconcat(state_base_name, ".uss", NULL);
    amiga_set_option("statefile", path);
    free(path);
#endif
    free(state_base_name);

    fsemu_savestate_set_path_function(fsuae_savestate_path);
    fsemu_savestate_init();
}
