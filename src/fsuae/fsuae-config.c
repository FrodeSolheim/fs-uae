#define FSUAE_INTERNAL
#include "fsuae-config.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/emu.h>
#include <fs/emu/render.h>
#include <fs/i18n.h>
#include <stdlib.h>
#include <string.h>
#include <uae/uae.h>

#include "fs-uae.h"
#include "fsemu-config.h"
#include "fsuae-accelerator.h"
#include "fsuae-graphics.h"
#include "fsuae-hardware.h"
#include "fsuae-model.h"
#include "fsuae-options.h"
#include "fsuae-path.h"
#include "fsuae-sound.h"

#define LEN_MAX 1024

static void strip(char *dstr, const char *sstr)
{
    char *d = dstr;
    int k = 0;
    for (const char *s = sstr; *s && k < LEN_MAX; s++, k++) {
        if (*s >= 'a' && *s <= 'z') {
        } else if (*s >= 'A' && *s <= 'Z') {
        } else if (*s >= '0' && *s <= '9') {
        } else if (*s == '+') {
        } else {
            continue;
        }
        *d++ = *s;
    }
    *d = '\0';
}

bool fs_uae_values_matches(const char *str1, const char *str2)
{
    if (str1 == NULL) {
        return str2 == NULL;
    } else if (str2 == NULL) {
        return str1 == NULL;
    }
    char nrm1[LEN_MAX + 1];
    char nrm2[LEN_MAX + 1];
    strip(nrm1, str1);
    strip(nrm2, str2);
    return strcasecmp(nrm1, nrm2) == 0;
}

int fs_uae_read_memory_option(const char *name)
{
    const char *s = fs_config_get_const_string(name);
    if (!s) {
        return FS_CONFIG_NONE;
    }
    return fs_uae_memory_kb_from_string(s, 0);
}

int fs_uae_read_memory_option_small(const char *name)
{
    const char *s = fs_config_get_const_string(name);
    if (!s) {
        return FS_CONFIG_NONE;
    }
    return fs_uae_memory_kb_from_string(s, 1);
}

int fs_uae_memory_kb_from_string(const char *str, bool smallmem)
{
    int len = strlen(str);
    if (len == 0) {
        return FS_CONFIG_NONE;
    }
    char u = str[len - 1];
    int factor = 1;
    bool factor_specified = false;
    int result = FS_CONFIG_NONE;
    if (u == 'K' || u == 'k' || u == 'M' || u == 'm') {
        factor_specified = true;
        if (u == 'M' || u == 'm') {
            factor = 1024;
        }
#if 0
        char *s2 = strdup(str);
        s2[len - 1] = '\0';
        result = atoi(s2);
        result = strtoll(s, NULL, 0);
    } else {
        result = atoi(s);
#endif
    }
    result = strtoll(str, NULL, 0);
    if (!factor_specified) {
        if (smallmem && result >= 32) {
            return result;
        }
        if (!smallmem && result < 1024) {
            return result * 1024;
        }
    }
    return result * factor;
}

void fs_uae_configure_amiga_model()
{
    fs_emu_log("fs_uae_configure_amiga_model\n");
    fs_uae_init_configs();

#ifdef FSUAE_LEGACY
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
#endif

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
        for (amiga_config *c = g_fs_uae_amiga_configs; c->id; c++, i++) {
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

#ifdef FSUAE_LEGACY
    if (!fs_emu_get_title()) {
        fs_emu_set_title("FS-UAE");
    }
    if (!fs_emu_get_sub_title()) {
        fs_emu_set_sub_title(c->name);
    }
#endif
    if (c->warning) {
        fs_emu_warning(c->warning);
    }
}

static void configure_roms(amiga_config *c)
{
    char *path = fs_config_get_string("kickstart_file");
    if (path == NULL) {
    } else if (strcmp(path, "internal") == 0) {
        /* Do not load external kickstart */
        free(path);
    } else {
        path = fsuae_path_expand_and_free(path);
        path = fsuae_path_resolve_and_free(path, FS_UAE_ROM_PATHS);
        amiga_set_option("kickstart_rom_file", path);
        free(path);

        path = fs_config_get_string("kickstart_ext_file");
        if (path) {
            path = fsuae_path_expand_and_free(path);
            path = fsuae_path_resolve_and_free(path, FS_UAE_ROM_PATHS);
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

#ifdef WITH_MIDI

#include "portmidi.h"

#define MIDI_DEVICE_NAME_MAX 128

static bool check_midi_device(
    const char *name, bool partial, char *result, bool in, bool out)
{
    int count = Pm_CountDevices();
    for (int i = 0; i < count; i++) {
        const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
        if (info != NULL) {
            if (info->output != out) {
                continue;
            }
            if (partial) {
                int len = strlen(name);
                if (strncmp(info->name, name, len) != 0) {
                    continue;
                }
            } else {
                if (strcmp(info->name, name) != 0) {
                    continue;
                }
            }
            fs_emu_log("[MIDI] Found '%s' for '%s' (in: %d, out: %d)\n",
                       info->name,
                       name,
                       in,
                       out);
            strncpy(result, info->name, MIDI_DEVICE_NAME_MAX - 1);
            result[MIDI_DEVICE_NAME_MAX - 1] = '\0';
            return true;
        }
    }
    fs_emu_log("[MIDI] Did not find '%s' (out: %d, in: %d)\n", name, in, out);
    return false;
}

static void enable_midi(const char *in_option,
                        const char *out_option,
                        bool mt32)
{
    fs_emu_log("[MIDI] Calling Pm_Initialize\n");
    PmError error = Pm_Initialize();
    if (error) {
        fs_emu_log("[MIDI] PortMidi error %d\n", error);
        return;
    }
    int count = Pm_CountDevices();
    fs_emu_log("[MIDI] PortMidi reports %d devices:\n\n", count);
    for (int i = 0; i < count; i++) {
        const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
        if (info != NULL) {
            fs_emu_log("[MIDI] %i: %s\n\n", i, info->name);
        }
    }

    char in_device[MIDI_DEVICE_NAME_MAX];
    in_device[0] = '\0';
    if (in_option != NULL) {
        const char *value = fs_config_get_const_string(in_option);
        if (value != NULL) {
            if (strcmp(value, "none") == 0) {
                fs_emu_log("MIDI in device is 'none'\n");
            } else if (check_midi_device(
                           value, false, in_device, true, false)) {
                //
            }
        } else {
            fs_emu_log("Finding first MIDI input device\n");
            check_midi_device("", true, in_device, true, false);
        }
    }

    char out_device[MIDI_DEVICE_NAME_MAX];
    out_device[0] = '\0';
    if (out_option != NULL) {
        const char *value = fs_config_get_const_string(out_option);
        if (value != NULL) {
            if (strcmp(value, "none") == 0) {
                fs_emu_log("MIDI device is 'none'\n");
            } else if (check_midi_device(
                           value, false, out_device, false, true)) {
                //
            }
        } else {
            if (mt32) {
                if (false) {
                }
#if defined(FSEMU_OS_WINDOWS)
                else if (check_midi_device("MT-32 Synth Emulator",
                                           false,
                                           out_device,
                                           false,
                                           true)) {
                }
#elif defined(FSEMU_OS_MACOS)
                else if (check_midi_device(
                             "Mt32EmuPort", false, out_device, false, true)) {
                }
#else
                else if (check_midi_device(
                             "Standard", false, out_device, false, true)) {
                }
#endif
            } else {
                if (false) {
                }
#if defined(FSEMU_OS_WINDOWS)
#elif defined(FSEMU_OS_MACOS)
                else if (check_midi_device("FluidSynth virtual port (",
                                           true,
                                           out_device,
                                           false,
                                           true)) {
                }
#else
                else if (check_midi_device("Synth input port (",
                                           true,
                                           out_device,
                                           false,
                                           true)) {
                }
#endif
            }
        }
    }
    gchar *serial_port = NULL;
    if (in_device[0] && out_device[0]) {
        serial_port = g_strdup_printf("midi:%s,%s", out_device, in_device);
    } else if (in_device[0]) {
        serial_port = g_strdup_printf("midi:,%s", in_device);
    } else if (out_device[0]) {
        serial_port = g_strdup_printf("midi:%s", out_device);
    } else {
        fs_emu_log("Serial port cannot be configured for MIDI\n");
    }
    if (serial_port != NULL) {
        amiga_enable_serial_port(serial_port);
        g_free(serial_port);
    }
}

#endif

static void fsuae_configure_serial_port(void)
{
    const char *serial_port = fs_config_get_const_string(OPTION_SERIAL_PORT);
    if (serial_port == NULL) {
        // Nothing to configure
    } else if (g_ascii_strcasecmp(serial_port, "none") == 0) {
        // Specifically set to none
    }
#ifdef WITH_MIDI
    else if (g_ascii_strcasecmp(serial_port, "midi:") == 0) {
        enable_midi(OPTION_MIDI_IN_DEVICE, OPTION_MIDI_OUT_DEVICE, false);
    } else if (g_ascii_strcasecmp(serial_port, "midi://in") == 0) {
        enable_midi(OPTION_MIDI_IN_DEVICE, NULL, false);
    } else if (g_ascii_strcasecmp(serial_port, "midi://out") == 0) {
        enable_midi(NULL, OPTION_MIDI_OUT_DEVICE, false);
    } else if (g_ascii_strcasecmp(serial_port, "midi://mt32") == 0) {
        enable_midi(NULL, OPTION_MIDI_MT32_DEVICE, true);
    }
#endif
    else {
        amiga_enable_serial_port(serial_port);
    }
}

void fs_uae_configure_amiga_hardware(void)
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
        fs_uae_load_rom_files(fsuae_path_kickstarts_dir());
        gchar *scan_path = g_build_filename(fsuae_path_base_dir(),
                                            "AmigaForever",
                                            "Amiga Files",
                                            "Shared",
                                            "rom",
                                            NULL);
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
    fsuae_configure_serial_port();

    const char *parallel_port =
        fs_config_get_const_string(OPTION_PARALLEL_PORT);
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
        amiga_set_option("native_code", "yes");
    }

    if (fs_config_get_boolean(OPTION_LINE_DOUBLING) == 0) {
        fsuae_log("disabling line doubling\n");
        amiga_set_option("gfx_linemode", "none");
    }

    if (fs_config_get_boolean(OPTION_LOW_RESOLUTION) == 1) {
        fsuae_log("force low resolution\n");
        amiga_set_option("gfx_lores", "true");
    }

    int stereo_separation =
        fs_config_get_int_clamped(OPTION_STEREO_SEPARATION, 0, 100);
    if (stereo_separation == FS_CONFIG_NONE) {
        stereo_separation = 70;
    }
    stereo_separation = stereo_separation / 10;
    amiga_set_int_option("sound_stereo_separation", stereo_separation);

    // This must not be set higher than 96, that will interfere with sound
    // filter (= always on).
    amiga_set_option("power_led_dim", "96");
    if (c->enhanced_audio_filter) {
        // A 1200 type filter
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
