#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <string.h>

#include <uae/uae.h>
#include <fs/emu.h>
#include <fs/i18n.h>
#include "fs-uae.h"

#include "options.h"
#include "config-accelerator.h"
#include "config-common.h"
#include "config-graphics.h"
#include "config-hardware.h"
#include "config-model.h"
#include "config-sound.h"

void fs_uae_configure_amiga_model()
{
    fs_emu_log("fs_uae_configure_amiga_model\n");
    fs_uae_init_configs();

    if (fs_config_get_boolean("ntsc_mode") == 1) {
        fs_emu_log("enabling NTSC mode (59.94Hz)\n");
        g_fs_uae_ntsc_mode = 1;
        fs_emu_set_video_frame_rate(59.94);
    } else {
        fs_emu_log("using PAL mode (50Hz)\n");
        fs_emu_set_video_frame_rate(50.0);
    }

    g_fs_uae_amiga_config = -1;
    const char *config_model = fs_config_get_const_string(OPTION_AMIGA_MODEL);
    if (config_model == NULL) {
        config_model = fs_config_get_const_string(OPTION_MODEL);
    }
    if (config_model == NULL) {
        config_model = "A500";
    }
    if (config_model) {
        int i = 0;
        for (amiga_config* c = g_fs_uae_amiga_configs; c->id; c++, i++) {
            if (fs_uae_values_matches(config_model, c->id)) {
                fs_emu_log("config match for \"%s\"\n", c->id);
                g_fs_uae_amiga_config = i;
                break;
            }

        }
    }
    if (g_fs_uae_amiga_config == -1) {
        fs_emu_warning("Unknown Amiga model - using A500\n");
        g_fs_uae_amiga_config = CONFIG_A500;
    }

    amiga_config *c = g_fs_uae_amiga_configs + g_fs_uae_amiga_config;
    g_fs_uae_config = c;
    g_fs_uae_amiga_model = c->model;

    if (!fs_emu_get_title()) {
        fs_emu_set_title(c->name);
    }
    if (!fs_emu_get_sub_title()) {
        fs_emu_set_sub_title("FS-UAE");
    }
    if (c->warning) {
        fs_emu_warning(c->warning);
    }
}

static void configure_memory(amiga_config *c)
{
    int chip_memory = fs_uae_read_memory_option_small(OPTION_CHIP_MEMORY);
    if (chip_memory != FS_CONFIG_NONE) {
        if (chip_memory == 128) {
            amiga_set_int_option("chipmem_size", -1);
        } else if (chip_memory == 256) {
            amiga_set_int_option("chipmem_size", 0);
        } else if (chip_memory % 512 == 0) {
            amiga_set_int_option("chipmem_size", chip_memory / 512);
        } else {
            fs_emu_warning(_("Option chip_memory must be a multiple of 512"));
            chip_memory = 0;
        }
    } else {
        chip_memory = 0;
    }

    int slow_memory = fs_uae_read_memory_option_small(OPTION_SLOW_MEMORY);
    if (slow_memory != FS_CONFIG_NONE) {
        if (slow_memory % 256 == 0) {
            amiga_set_int_option("bogomem_size", slow_memory / 256);
        } else {
            fs_emu_warning(_("Option slow_memory must be a multiple of 256"));
            slow_memory = 0;
        }
    } else {
        slow_memory = 0;
    }

    int fast_memory = fs_uae_read_memory_option(OPTION_FAST_MEMORY);
    if (fast_memory != FS_CONFIG_NONE) {
        if (fast_memory % 1024 == 0) {
            amiga_set_int_option("fastmem_size", fast_memory / 1024);
        } else {
            fs_emu_warning(_("Option fast_memory must be a multiple of 1024"));
            fast_memory = 0;
        }
    } else {
        fast_memory = 0;
    }

    int z3_memory = fs_uae_read_memory_option(OPTION_ZORRO_III_MEMORY);
    if (z3_memory != FS_CONFIG_NONE) {
        if (z3_memory && !c->allow_z3_memory) {
            fs_emu_warning(_("Option zorro_iii_memory needs a CPU "
                             "with 32-bit addressing"));
            z3_memory = 0;
        } else if (z3_memory % 1024 == 0) {
            amiga_set_int_option("z3mem_size", z3_memory / 1024);
        } else {
            fs_emu_warning(_("Option zorro_iii_memory must be a multiple "
                             "of 1024"));
            z3_memory = 0;
        }
    } else {
        z3_memory = 0;
    }

    int mb_ram = fs_uae_read_memory_option(OPTION_MOTHERBOARD_RAM);
    if (mb_ram != FS_CONFIG_NONE) {
        if (mb_ram && !c->allow_z3_memory) {
            fs_emu_warning(_("Option motherboard_ram needs a CPU "
                             "with 32-bit addressing"));
            mb_ram = 0;
        } else if (mb_ram % 1024 == 0) {
            amiga_set_int_option("a3000mem_size", mb_ram / 1024);
        } else {
            fs_emu_warning(_("Option motherboard_ram must be a multiple "
                             "of 1024"));
            mb_ram = 0;
        }
    } else {
        mb_ram = 0;
    }
}

static void configure_roms(amiga_config *c)
{
    char *path = fs_config_get_string("kickstart_file");
    if (path == NULL ) {

    } else if (strcmp(path, "internal") == 0) {
        /* Do not load external kickstart */
        free(path);
    } else {
        path = fs_uae_expand_path_and_free(path);
        path = fs_uae_resolve_path_and_free(path, FS_UAE_ROM_PATHS);
        amiga_set_option("kickstart_rom_file", path);
        free(path);

        path = fs_config_get_string("kickstart_ext_file");
        if (path) {
            path = fs_uae_expand_path_and_free(path);
            path = fs_uae_resolve_path_and_free(path, FS_UAE_ROM_PATHS);
            amiga_set_option("kickstart_ext_rom_file", path);
            free(path);
        }
    }
}

void fs_uae_configure_amiga_hardware()
{
    amiga_config *c = g_fs_uae_amiga_configs + g_fs_uae_amiga_config;
    char *path;

    fs_emu_log("fs_uae_configure_amiga_hardware\n");

    int scan_kickstarts = 1;
    path = fs_config_get_string("kickstart_file");
    if (path) {
        if (strcmp(path, "internal") == 0) {
            scan_kickstarts = 0;
        }
        free(path);
    }
    if (scan_kickstarts) {
        fs_uae_load_rom_files(fs_uae_kickstarts_dir());
    }

    fs_emu_log("configuring \"%s\", accuracy=%d\n", c->name, 1);

    amiga_quickstart(c->quickstart_model, c->quickstart_config, 1);

    if (fs_config_get_boolean(OPTION_JIT_COMPILER) == 1) {
        amiga_set_option("cachesize", "8192");
    } else {
        amiga_set_option("cachesize", "0");
    }
    const char *jit_memory = fs_config_get_const_string(OPTION_JIT_MEMORY);
    if (jit_memory && strcmp(jit_memory, "indirect") == 0) {
        amiga_set_option("comp_trustbyte", "indirect");
        amiga_set_option("comp_trustword", "indirect");
        amiga_set_option("comp_trustlong", "indirect");
        amiga_set_option("comp_trustnaddr", "indirect");
    }

#if 1
    if (cfg->z3realmapping == 0) {
        amiga_set_option("z3mapping", "real");
    }
#else
    if (cfg->z3realmapping != FS_CONFIG_NONE) {
        amiga_set_int_option("z3realmapping", cfg->z3realmapping);
    }
#endif

    if (g_fs_uae_ntsc_mode) {
        // FIXME: ciiatod on some Amiga models?
        amiga_set_option("ntsc", "true");
    }

    configure_roms(c);
    configure_memory(c);

    fs_uae_configure_hardware();
    fs_uae_configure_accelerator();
    fs_uae_configure_graphics_card(c);
    fs_uae_configure_sound_card(c);

    const char *serial_port = fs_config_get_const_string(OPTION_SERIAL_PORT);
    if (serial_port && g_ascii_strcasecmp(serial_port, "none") != 0) {
        amiga_enable_serial_port(serial_port);
    }

    const char *dongle_type = fs_config_get_const_string(OPTION_DONGLE_TYPE);
    if (dongle_type) {
        amiga_set_option("dongle", dongle_type);
    }

    if (fs_config_get_boolean(OPTION_BSDSOCKET_LIBRARY) == 1) {
        amiga_set_option("bsdsocket_emu", "yes");
    }

    if (fs_config_get_boolean(OPTION_UAENATIVE_LIBRARY) == 1) {
        amiga_set_option("bsdsocket_emu", "yes");
    }

    if (fs_config_get_boolean(OPTION_LINE_DOUBLING) == 0) {
        fs_log("disabling line doubling\n");
        amiga_set_option("gfx_linemode", "none");
    }

    if (fs_config_get_boolean(OPTION_LOW_RESOLUTION) == 1) {
        fs_log("force low resolution\n");
        amiga_set_option("gfx_lores", "true");
    }

    int stereo_separation = fs_config_get_int_clamped(
        OPTION_STEREO_SEPARATION, 0, 100);
    if (stereo_separation == FS_CONFIG_NONE) {
        stereo_separation = 70;
    }
    stereo_separation = stereo_separation / 10;
    amiga_set_int_option("sound_stereo_separation", stereo_separation);

    if (c->enhanced_audio_filter) {
        amiga_set_option("sound_filter_type", "enhanced");
    }
}
