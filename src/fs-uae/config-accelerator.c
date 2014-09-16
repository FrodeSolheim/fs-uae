#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <uae/uae.h>
#include <fs/conf.h>
#include <fs/glib.h>
#include "options.h"
#include "fs-uae.h"
#include "config-model.h"
#include "config-accelerator.h"

static void configure_accelerator_rom(void)
{
    char *path = fs_config_get_string(OPTION_ACCELERATOR_ROM);
    if (!path) {
        path = fs_config_get_string("cpuboard_flash_file");
    }
    if (path) {
        path = fs_uae_expand_path_and_free(path);
        path = fs_uae_resolve_path_and_free(path, FS_UAE_ROM_PATHS);
        amiga_set_option("cpuboard_rom_file", path);
        g_free(path);
    }

    path = fs_config_get_string("cpuboard_flash_ext_file");
    if (path) {
        path = fs_uae_expand_path_and_free(path);
        path = fs_uae_resolve_path_and_free(path, FS_UAE_ROM_PATHS);
        amiga_set_option("cpuboard_ext_rom_file", path);
        g_free(path);
    }
}

void fs_uae_configure_accelerator(void)
{
    int memory = 128;
    const char *card = fs_config_get_const_string(OPTION_ACCELERATOR);
    if (card == NULL) {
        card = cfg->accelerator;
    }

    if (card != NULL) {
        if (strcasecmp(card, "cyberstorm-ppc") == 0) {
            card = "CyberstormPPC";
        }
    }

    if (fs_config_get_const_string(OPTION_ACCELERATOR_MEMORY)) {
        // FIXME: use memory function
        memory = fs_config_get_int(OPTION_ACCELERATOR_MEMORY);
    }

    if (card != NULL) {
        if (memory != 0) {
            amiga_set_option("cpuboard_type", card);
            amiga_set_int_option("cpuboardmem1_size", memory);
        }
    }

    configure_accelerator_rom();
}
