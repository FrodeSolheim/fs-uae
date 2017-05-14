#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <string.h>

#include <uae/uae.h>
#include <fs/emu.h>
#include <fs/emu/render.h>
#include <fs/i18n.h>
#include "fs-uae.h"

#include "options.h"
#include "paths.h"
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
        fse_set_pixel_aspect((4.0 / 3.0) / (640.0 / 400.0));
    } else {
        fs_emu_log("using PAL mode (50Hz)\n");
        fs_emu_set_video_frame_rate(50.0);
        /* Pixel aspect is close enough to 1:1 what we just go with that
         * for improved scaling quality. */
        // fse_set_pixel_aspect((4.0 / 3.0) / (640.0 / 512.0));
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
        fs_emu_set_title("FS-UAE");
    }
    if (!fs_emu_get_sub_title()) {
        fs_emu_set_sub_title(c->name);
    }
    if (c->warning) {
        fs_emu_warning(c->warning);
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

static void fs_uae_configure_network_card(amiga_config *c)
{
    const char *card = fs_config_get_const_string(OPTION_NETWORK_CARD);
    if (card != NULL) {
        if (strcmp(card, "0") == 0) {
        } else if (fs_uae_values_matches(card, "a2065")) {
            amiga_set_option("a2065", "slirp");
        } else {
            fs_emu_warning("Unrecognized network card");
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
        gchar *scan_path = g_build_filename(
            fs_uae_base_dir(), "AmigaForever", "Amiga Files", "Shared",
            "rom", NULL);
        if (g_file_test(scan_path, G_FILE_TEST_IS_DIR)) {
            fs_uae_load_rom_files(scan_path);
        }
        g_free(scan_path);
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

    /* Accelerator must be configured before CPU */
    fs_uae_configure_accelerator();
    fs_uae_configure_hardware();
    fs_uae_configure_graphics_card(c);
    fs_uae_configure_sound_card(c);
    fs_uae_configure_network_card(c);

    const char *serial_port = fs_config_get_const_string(OPTION_SERIAL_PORT);
    if (serial_port && g_ascii_strcasecmp(serial_port, "none") != 0) {
        amiga_enable_serial_port(serial_port);
    }

    const char *parallel_port = fs_config_get_const_string(
                OPTION_PARALLEL_PORT);
    if (parallel_port && g_ascii_strcasecmp(parallel_port, "none") != 0) {
        amiga_enable_parallel_port(parallel_port);
    }

    const char *dongle_type = fs_config_get_const_string(OPTION_DONGLE_TYPE);
    if (dongle_type) {
        int dongle_index = 0;
        if (strcmp(dongle_type, "0") == 0) {
        } else if (strcmp(dongle_type, "robocop 3") == 0) {
            dongle_index = 1;
        } else if (strcmp(dongle_type, "leaderboard") == 0) {
            dongle_index = 2;
        } else if (strcmp(dongle_type, "b.a.t. ii") == 0) {
            dongle_index = 3;
        } else if (strcmp(dongle_type, "italy'90 soccer") == 0) {
            dongle_index = 4;
        } else if (strcmp(dongle_type, "dames grand maitre") == 0) {
            dongle_index = 5;
        } else if (strcmp(dongle_type, "rugby coach") == 0) {
            dongle_index = 6;
        } else if (strcmp(dongle_type, "cricket captain") == 0) {
            dongle_index = 7;
        } else if (strcmp(dongle_type, "leviathan") == 0) {
            dongle_index = 8;
        } else {
            fs_emu_warning("Unrecognized dongle type");
        }
        if (dongle_index > 0) {
            amiga_set_int_option("dongle", dongle_index);
        }
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

    const char *freezer = fs_config_get_const_string(OPTION_FREEZER_CARTRIDGE);
    if (freezer) {
        if (strcmp(freezer, "0") == 0) {
            /* No freezer cartridge */
        } else if (strcmp(freezer, "hrtmon") == 0) {
            amiga_set_option("cart_file", ":HRTMon");
        } else if (strcmp(freezer, "action-replay-2") == 0) {
            amiga_set_option("cart", "Freezer: Action Replay Mk II v2.14");
        } else if (strcmp(freezer, "action-replay-3") == 0) {
            amiga_set_option("cart", "Freezer: Action Replay Mk III v3.17");
        } else {
            fs_emu_warning("Unrecognized cartridge");
        }
    }
}
