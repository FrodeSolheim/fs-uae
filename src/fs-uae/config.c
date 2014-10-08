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

//char *g_fs_uae_default_dir = ".";

int g_fs_uae_fastest_possible = 0;
static int g_accuracy = 1;

void fs_uae_configure_amiga_model()
{
    fs_emu_log("fs_uae_configure_amiga_model\n");
    fs_uae_init_configs();
    //amiga_set_option("kickstart_rom_file", "aa:AROS");

    if (fs_config_get_boolean("ntsc_mode") == 1) {
        fs_emu_log("enabling NTSC mode (59.94Hz)\n");
        g_fs_uae_ntsc_mode = 1;
        fs_emu_set_video_frame_rate(59.94);
    } else {
        fs_emu_log("using PAL mode (50Hz)\n");
        fs_emu_set_video_frame_rate(50.0);
    }

    g_fs_uae_amiga_config = -1;

    const char *config_model = fs_config_get_const_string("amiga_model");
    if (config_model == NULL) {
        config_model = fs_config_get_const_string("model");
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
        fs_emu_log("WARNING: unknown amiga config specified, using A500\n");
        g_fs_uae_amiga_config = CONFIG_A500;
    }

    amiga_config *c = g_fs_uae_amiga_configs + g_fs_uae_amiga_config;
    g_fs_uae_config = c;
    g_fs_uae_amiga_model = c->model;
    g_accuracy = fs_config_get_int_clamped("accuracy", -2, 1);

    if (c->no_accuracy_adjustment) {
        fs_emu_log("ignoring accuracy for this model\n");
        g_accuracy = 1;
    }

    if (g_accuracy == FS_CONFIG_NONE) {
        g_accuracy = 1;
    }
    if (g_accuracy <= c->fast_on_accuracy_level) {
        fs_emu_log("this model / accuracy combination is a "
                   "\"fastest possibly\" mode\n");
        g_fs_uae_fastest_possible = 1;
    }

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

static void configure_accuracy(amiga_config *c)
{
#if 0
#ifdef NEW_ACCURACY_SYSTEM
    int cpu_accuracy = fs_config_get_int("cpu_accuracy");
    if (cpu_accuracy == FS_CONFIG_NONE) {
        cpu_accuracy = fs_config_get_int("accuracy");
    }
    if (cpu_accuracy != FS_CONFIG_NONE) {
        if (cpu_accuracy <= 0) {
            amiga_set_option("cpu_cycle_exact", "false");
            if (g_fs_uae_amiga_model == MODEL_A1200
                || g_fs_uae_amiga_model == MODEL_CD32) {
                // FIXME: test this setting some more
                //amiga_set_option("cpu_speed", "2");
                amiga_set_option("cpu_speed", "0");
            }
        }
        if (cpu_accuracy <= -1) {
            amiga_set_option("cpu_compatible", "false");
        }
    }
    int blitter_accuracy = fs_config_get_int("blitter_accuracy");
    if (blitter_accuracy == FS_CONFIG_NONE) {
        blitter_accuracy = fs_config_get_int("accuracy");
    }
    if (blitter_accuracy != FS_CONFIG_NONE) {
        if (blitter_accuracy <= 0) {
            amiga_set_option("blitter_cycle_exact", "false");
        }
        if (blitter_accuracy <= -1) {
            amiga_set_option("waiting_blits", "false");
            amiga_set_option("immediate_blits", "true");
        }
    }
    int audio_accuracy = fs_config_get_int("audio_accuracy");
    if (audio_accuracy == FS_CONFIG_NONE) {
        // we don't want to use interrupts unless we specifically requested
        // audio_accuracy of -1
        audio_accuracy = fs_config_get_int_clamped("accuracy", 0, 1);
    }
    if (audio_accuracy != FS_CONFIG_NONE) {
        if (audio_accuracy == 0) {
            amiga_set_option("sound_output", "normal");
        } else if (audio_accuracy <= -1) {
            amiga_set_option("sound_output", "interrupts");
        }
    }
#endif
#endif
}

static void configure_memory(amiga_config *c)
{
    int chip_memory = fs_uae_read_memory_option_small("chip_memory");
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

    int slow_memory = fs_uae_read_memory_option_small("slow_memory");
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

    int fast_memory = fs_uae_read_memory_option("fast_memory");
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

    int z3_memory = fs_uae_read_memory_option("zorro_iii_memory");
    if (z3_memory != FS_CONFIG_NONE) {
        if (z3_memory && !c->allow_z3_memory) {
            fs_emu_warning(_("Options zorro_iii_memory needs a CPU "
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
}

static void configure_roms(amiga_config *c)
{
    char *path = fs_config_get_string("kickstart_file");
    if (path) {
        if (strcmp(path, "internal") == 0) {
            // do not load external kickstart
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

#ifdef NEW_ACCURACY_SYSTEM
    g_accuracy = 1;
#endif
    fs_emu_log("configuring \"%s\", accuracy=%d\n", c->name, g_accuracy);

    amiga_quickstart(c->quickstart_model, c->quickstart_config, g_accuracy);
    amiga_set_option("cachesize", "0");
    amiga_set_option("comp_trustbyte", "indirect");
    amiga_set_option("comp_trustword", "indirect");
    amiga_set_option("comp_trustlong", "indirect");
    amiga_set_option("comp_trustnaddr", "indirect");

#if 0
    if (cfg->cpu_model) {
        amiga_set_option("cpu_model", cfg->cpu_model);
        if (strcmp(cfg->cpu_model, "68040") == 0) {
            amiga_set_option("fpu_model", "68040");
        } else if (strcmp(cfg->cpu_model, "68060") == 0) {
            amiga_set_option("fpu_model", "68060");
        }
    }
#endif

#if 0
    if (c->z3mem_size) {
        amiga_set_int_option("z3mem_size", c->z3mem_size);
    }
#endif
#if 0
    if (c->cpu_32bit_addressing) {
        amiga_set_option("cpu_24bit_addressing", "false");
    }
#endif

    if (cfg->z3realmapping != FS_CONFIG_NONE) {
        amiga_set_int_option("z3realmapping", cfg->z3realmapping);
    }

    if (g_fs_uae_ntsc_mode) {
        // FIXME: ciiatod on some Amiga models?
        amiga_set_option("ntsc", "true");
    }

    configure_roms(c);
    configure_memory(c);

    fs_uae_configure_hardware();
    fs_uae_configure_accelerator();
    fs_uae_configure_graphics_card(c);

    configure_accuracy(c);

    const char *serial_port = fs_config_get_const_string("serial_port");
    if (serial_port && g_ascii_strcasecmp(serial_port, "none") != 0) {
        amiga_enable_serial_port(fs_config_get_const_string("serial_port"));
    }

    if (fs_config_get_const_string("dongle_type")) {
        amiga_set_option("dongle", fs_config_get_const_string("dongle_type"));
    }

    if (fs_config_get_boolean("bsdsocket_library") == 1) {
        amiga_set_option("bsdsocket_emu", "yes");
    }

    if (fs_config_get_boolean("line_doubling") == 0) {
        fs_log("disabling line doubling\n");
        amiga_set_option("gfx_linemode", "none");
    }

    if (fs_config_get_boolean("low_resolution") == 1) {
        fs_log("force low resolution\n");
        amiga_set_option("gfx_lores", "true");
    }

    int stereo_separation = fs_config_get_int_clamped(
        "stereo_separation", 0, 100);
    if (stereo_separation == FS_CONFIG_NONE) {
        stereo_separation = 100;
    }
    stereo_separation = stereo_separation / 10;
    amiga_set_option_and_free("sound_stereo_separation",
                              g_strdup_printf("%d", stereo_separation), free);
    if (c->enhanced_audio_filter) {
        amiga_set_option("sound_filter_type", "enhanced");
    }
}
