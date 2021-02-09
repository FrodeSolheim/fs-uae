#define FSUAE_INTERNAL
#include "fsuae-accelerator.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/conf.h>
#include <fs/glib.h>
#include <uae/uae.h>

#include "fs-uae.h"
#include "fsuae-config.h"
#include "fsuae-model.h"
#include "fsuae-options.h"
#include "fsuae-path.h"

static void configure_accelerator_rom(const char *default_rom)
{
    char *path = fs_config_get_string(OPTION_ACCELERATOR_ROM);
    if (!path) {
        path = fs_config_get_string("cpuboard_flash_file");
    }
    if (path) {
        path = fsuae_path_expand_and_free(path);
        path = fsuae_path_resolve_and_free(path, FS_UAE_ROM_PATHS);
        amiga_set_option("cpuboard_rom_file", path);
        g_free(path);
    } else if (default_rom) {
        amiga_set_option("cpuboard_rom_file", default_rom);
    }
    path = fs_config_get_string("cpuboard_flash_ext_file");
    if (path) {
        path = fsuae_path_expand_and_free(path);
        path = fsuae_path_resolve_and_free(path, FS_UAE_ROM_PATHS);
        amiga_set_option("cpuboard_ext_rom_file", path);
        g_free(path);
    }
}

void fs_uae_configure_accelerator(void)
{
    int memory = 128;
    const char *card = fs_config_get_const_string(OPTION_ACCELERATOR);
    const char *rom = NULL;
    bool allow_scsi_kit = false;
    if (card == NULL) {
        card = cfg->accelerator;
    }

    if (card != NULL) {
        if (fs_uae_values_matches(card, "blizzard-1230-iv")) {
            card = "Blizzard1230IV";
            memory = 32;
            allow_scsi_kit = true;
            cfg->accelerator_cpu = "68EC030";
        } else if (fs_uae_values_matches(card, "blizzard-1240")) {
            card = "Blizzard1260";
            memory = 32;
            allow_scsi_kit = true;
            cfg->accelerator_cpu = "68040-NOMMU";
        } else if (fs_uae_values_matches(card, "blizzard-1260")) {
            card = "Blizzard1260";
            memory = 32;
            allow_scsi_kit = true;
            cfg->accelerator_cpu = "68060-NOMMU";
        } else if (fs_uae_values_matches(card, "blizzard-ppc")) {
            card = "BlizzardPPC";
            rom = "blizzardppc_060.rom";
            memory = 256;
            cfg->accelerator_cpu = "68060-NOMMU";
        } else if (fs_uae_values_matches(card, "cyberstorm-ppc")) {
            card = "CyberstormPPC";
            gchar *csppc_rom_path = g_build_filename(
                fsuae_path_kickstarts_dir(), "cyberstormppc.rom", NULL);
            if (fs_path_exists(csppc_rom_path)) {
                rom = "cyberstormppc.rom";
                fsuae_log("[ROM] Found cyberstormppc.rom\n");
            } else {
                fsuae_log("[ROM] Did not find cyberstormppc.rom\n");
                fsuae_log("[ROM] Trying ralphschmidt-cyberstorm-ppc-4471.rom\n");
                rom = "ralphschmidt-cyberstorm-ppc-4471.rom";
            }
            /* FIXME: TODO: Also try to find ROM in Amiga Forever roms dir.
             * (Or maybe just implement that for FS-UAE Launcher) */
            memory = 128;
            cfg->accelerator_cpu = "68060-NOMMU";
        }
    }

    if (fs_config_get_const_string(OPTION_ACCELERATOR_MEMORY)) {
        memory = fs_uae_read_memory_option(OPTION_ACCELERATOR_MEMORY);
        memory /= 1024;
        fsuae_log("CONFIG: Overriding accelerator memory: %d MB\n", memory);
    }

    if (fs_config_get_boolean(OPTION_BLIZZARD_SCSI_KIT) == 1) {
        if (allow_scsi_kit) {
            amiga_set_option("cpuboard_ext_rom_file",
                             "Blizzard_SCSI_Kit_IV_v8.5.bin");
        } else {
            fs_emu_warning(
                "Blizzard SCSI Kit cannot be used with this accelerator");
        }
    }

    if (card != NULL) {
        if (memory != 0) {
            amiga_set_option("cpuboard_type", card);
            amiga_set_int_option("cpuboardmem1_size", memory);
        }
    }

    configure_accelerator_rom(rom);
}
