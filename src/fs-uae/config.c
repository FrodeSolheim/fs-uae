#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <string.h>

#include <uae/uae.h>
#include <fs/emu.h>
#include <fs/i18n.h>
#include "fs-uae.h"

amiga_config g_fs_uae_amiga_configs[CONFIG_LAST + 1] = {};
int g_fs_uae_amiga_config = 0;
int g_fs_uae_ntsc_mode = 0;
int g_fs_uae_amiga_model = 0;
//char *g_fs_uae_default_dir = ".";

int g_fs_uae_fastest_possible = 0;
static int g_accuracy = 1;

#define NEW_ACCURACY_SYSTEM

void fs_uae_init_configs() {
    amiga_config *c;

    c = g_fs_uae_amiga_configs + CONFIG_A1000;
    c->id = "A1000";
    c->model = MODEL_A1000;
    c->name = "Amiga 1000";
    c->quickstart_model = 3;
    c->quickstart_config = 0;
    c->fast_on_accuracy_level = -999;

    c = g_fs_uae_amiga_configs + CONFIG_A500;
    c->id = "A500";
    c->model = MODEL_A500;
    c->name = "Amiga 500";
    //c->quickstart = "A500,,";
    c->quickstart_model = 0;
    c->fast_on_accuracy_level = -999;

    c = g_fs_uae_amiga_configs + CONFIG_A500P;
    c->id = "A500+";
    c->model = MODEL_A500P;
    c->name = "Amiga 500+";
    //c->quickstart = "A500+,,";
    c->quickstart_model = 1;
    c->fast_on_accuracy_level = -999;

    c = g_fs_uae_amiga_configs + CONFIG_A600;
    c->id = "A600";
    c->model = MODEL_A600;
    c->name = "Amiga 600";
    //c->quickstart = "A600,,";
    c->quickstart_model = 2;
    c->fast_on_accuracy_level = -999;

    c = g_fs_uae_amiga_configs + CONFIG_CDTV;
    c->id = "CDTV";
    c->model = MODEL_CDTV;
    c->name = "Commodore CDTV";
    //c->quickstart = "CDTV,,";
    c->quickstart_model = 9;
    c->fast_on_accuracy_level = -999;

    c = g_fs_uae_amiga_configs + CONFIG_CD32;
    c->id = "CD32";
    c->model = MODEL_CD32;
    c->name = "Amiga CD32";
    //c->quickstart = "CD32,,";
    c->quickstart_model = 8;
    c->quickstart_config = 0;
#ifdef NEW_ACCURACY_SYSTEM
    c->fast_on_accuracy_level = -999;
#else
    c->fast_on_accuracy_level = 0;
#endif

    c = g_fs_uae_amiga_configs + CONFIG_CD32_FMV;
    c->id = "CD32/FMV";
    c->model = MODEL_CD32;
    c->name = "Amiga CD32 + FMV ROM";
    //c->quickstart = "CD32,,";
    c->quickstart_model = 8;
    c->quickstart_config = 1;
#ifdef NEW_ACCURACY_SYSTEM
    c->fast_on_accuracy_level = -999;
#else
    c->fast_on_accuracy_level = 0;
#endif

    c = g_fs_uae_amiga_configs + CONFIG_A1200;
    c->id = "A1200";
    c->model = MODEL_A1200;
    c->name = "Amiga 1200";
    //c->quickstart = "A1200,,";
    c->quickstart_model = 4;
#ifdef NEW_ACCURACY_SYSTEM
    c->fast_on_accuracy_level = -999;
#else
    c->fast_on_accuracy_level = 0;
#endif
    c->enhanced_audio_filter = 1;

    c = g_fs_uae_amiga_configs + CONFIG_A1200_020;
    c->id = "A1200/020";
    c->model = MODEL_A1200;
    c->name = "Amiga 1200 (68020)";
    //c->quickstart = "A1200,,";
    c->quickstart_model = 4;
#ifdef NEW_ACCURACY_SYSTEM
    c->fast_on_accuracy_level = -999;
#else
    c->fast_on_accuracy_level = 0;
#endif
    c->enhanced_audio_filter = 1;
    c->cpu_model = "68020";
    c->cpu_32bit_addressing = 1;
    c->allow_z3_memory = 1;
    //c->z3mem_size = 64;

    c = g_fs_uae_amiga_configs + CONFIG_SUPER;
    c->id = "SUPER";
    // FIXME:
    c->model = MODEL_A1200;
    c->name = "Amiga (Super)";
    //c->quickstart = "A1200,,";
    c->quickstart_model = 11;
    c->fast_on_accuracy_level = 1;
    c->no_accuracy_adjustment = 1;
    //c->cpu_model = "68020";
    //c->cpu_32bit_addressing = 1;
    c->allow_z3_memory = 1;
    //c->z3mem_size = 64;
    c->warning = "SUPER is deprecated, use A4000/040 instead";

    c = g_fs_uae_amiga_configs + CONFIG_A4000;
    c->id = "A4000_DO_NOT_USE";
    c->model = MODEL_A4000;
    c->name = "Amiga 4000";
    c->quickstart_model = 6;
    c->fast_on_accuracy_level = 1;
    c->no_accuracy_adjustment = 1;
    c->allow_z3_memory = 1;

    c = g_fs_uae_amiga_configs + CONFIG_A4000_040;
    c->id = "A4000/040";
    c->model = MODEL_A4000;
    c->name = "Amiga 4000/040";
    c->quickstart_model = 6;
    c->quickstart_config = 1;
    c->fast_on_accuracy_level = 1;
    c->no_accuracy_adjustment = 1;
    c->allow_z3_memory = 1;
    c->enhanced_audio_filter = 1;

    c = g_fs_uae_amiga_configs + CONFIG_A3000;
    c->id = "A3000";
    c->model = MODEL_A3000;
    c->name = "Amiga 3000";
    c->quickstart_model = 5;
    c->quickstart_config = 2;
    c->fast_on_accuracy_level = 1;
    c->no_accuracy_adjustment = 1;
    c->allow_z3_memory = 1;

    /*
    c = g_amiga_configs + CONFIG_A1200_030;
    c->id = "A1200/030";
    c->model = MODEL_A1200;
    c->name = "Amiga 1200 (68030)";
    c->quickstart = "A1200,,";
    c->cpu_model = "68030";
    c->cpu_32bit_addressing = 1;
    c->allow_z3_memory = 1;
    c->z3mem_size = 64;

    c = g_amiga_configs + CONFIG_A1200_040;
    c->id = "A1200/040";
    c->model = MODEL_A1200;
    c->name = "Amiga 1200 (68040)";
    c->quickstart = "A1200,,";
    c->cpu_model = "68040";
    c->cpu_32bit_addressing = 1;
    c->allow_z3_memory = 1;
    c->z3mem_size = 64;
    c->fast = 1;
    */

    c = g_fs_uae_amiga_configs + CONFIG_LAST;
    c->id = NULL;
};

void fs_uae_configure_amiga_model() {
    fs_emu_log("fs_uae_configure_amiga_model\n");
    fs_uae_init_configs();
    //amiga_set_option("kickstart_rom_file", "aa:AROS");

    if (fs_config_get_boolean("ntsc_mode") == 1) {
        fs_emu_log("enabling NTSC mode (60Hz)\n");
        g_fs_uae_ntsc_mode = 1;
        fs_emu_set_video_frame_rate(60);
    }
    else {
        fs_emu_log("using PAL mode (50Hz)\n");
        fs_emu_set_video_frame_rate(50);
    }

    g_fs_uae_amiga_config = -1;

    const char *config_model = fs_config_get_const_string("amiga_model");
    if (config_model == NULL) {
        config_model = fs_config_get_const_string("model");
    }
    if (config_model) {
        int i = 0;
        for (amiga_config* c = g_fs_uae_amiga_configs; c->id; c++, i++) {
            if (fs_ascii_strcasecmp(config_model, c->id) == 0) {
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

static void configure_accuracy(amiga_config *c) {
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
        }
        else if (audio_accuracy <= -1) {
            amiga_set_option("sound_output", "interrupts");
        }
    }
#endif
}

static void configure_memory(amiga_config *c) {
    int chip_memory = fs_config_get_int("chip_memory");
    if (chip_memory != FS_CONFIG_NONE) {
        if (chip_memory == 128) {
            amiga_set_int_option("chipmem_size", -1);
        }
        else if (chip_memory == 256) {
            amiga_set_int_option("chipmem_size", 0);
        }
        else if (chip_memory % 512 == 0) {
            amiga_set_int_option("chipmem_size", chip_memory / 512);
        }
        else {
            fs_emu_warning(_("Option chip_memory must be a multiple of 512"));
            chip_memory = 0;
        }
    }
    else {
        chip_memory = 0;
    }
    int slow_memory = fs_config_get_int("slow_memory");
    if (slow_memory != FS_CONFIG_NONE) {
        if (slow_memory % 256 == 0) {
            amiga_set_int_option("bogomem_size", slow_memory / 256);
        }
        else {
            fs_emu_warning(_("Option slow_memory must be a multiple of 256"));
            slow_memory = 0;
        }
    }
    else {
        slow_memory = 0;
    }
    int fast_memory = fs_config_get_int("fast_memory");
    if (fast_memory != FS_CONFIG_NONE) {
        if (fast_memory % 1024 == 0) {
            amiga_set_int_option("fastmem_size", fast_memory / 1024);
        }
        else {
            fs_emu_warning(_("Option fast_memory must be a multiple of 1024"));
            fast_memory = 0;
        }
    }
    else {
        fast_memory = 0;
    }
    int z3_memory = fs_config_get_int("zorro_iii_memory");
    if (z3_memory != FS_CONFIG_NONE) {
        if (z3_memory && !c->allow_z3_memory) {
            fs_emu_warning(_("Options zorro_iii_memory needs a CPU "
                    "with 32-bit addressing"));
            z3_memory = 0;
        }
        else if (z3_memory % 1024 == 0) {
            amiga_set_int_option("z3mem_size", z3_memory / 1024);
        }
        else {
            fs_emu_warning(_("Option zorro_iii_memory must be a multiple "
                    "of 1024"));
            z3_memory = 0;
        }
    }
    else {
        z3_memory = 0;
    }
}

static void configure_cpuboard(void)
{
    char *path;

    path = fs_config_get_string("cpuboard_flash_file");
    if (path) {
        path = fs_uae_expand_path_and_free(path);
        path = fs_uae_resolve_path_and_free(path, FS_UAE_ROM_PATHS);
        amiga_set_option("cpuboard_rom_file", path);
        free(path);
    }

    path = fs_config_get_string("cpuboard_flash_ext_file");
    if (path) {
        path = fs_uae_expand_path_and_free(path);
        path = fs_uae_resolve_path_and_free(path, FS_UAE_ROM_PATHS);
        amiga_set_option("cpuboard_ext_rom_file", path);
        free(path);
    }
}

void fs_uae_configure_amiga_hardware() {
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

    if (c->cpu_model) {
        amiga_set_option("cpu_model", c->cpu_model);
    }
    if (c->z3mem_size) {
        amiga_set_int_option("z3mem_size", c->z3mem_size);
    }
    if (c->cpu_32bit_addressing) {
        amiga_set_option("cpu_24bit_addressing", "false");
    }
    if (c->fast) {
        amiga_set_option("cpu_speed", "max");
        amiga_set_option("blitter_cycle_exact", "false");
        amiga_set_option("cpu_cycle_exact", "false");

        amiga_set_option("cpu_compatible", "false");
        amiga_set_option("immediate_blits", "true");


    }

    //if (g_fs_uae_fastest_possible) {
        amiga_set_cpu_idle(2);
    //}

    if (g_fs_uae_ntsc_mode) {
        // FIXME: ciiatod on some Amiga models?
        amiga_set_option("ntsc", "true");
    }

    path = fs_config_get_string("kickstart_file");
    if (path) {
        if (strcmp(path, "internal") == 0) {
            // do not load external kickstart
            free(path);
        }
        else {
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

    configure_memory(c);

    if (fs_config_get_boolean("bsdsocket_library") == 1) {
        amiga_set_option("bsdsocket_emu", "yes");
    }

    int uaegfx_card = fs_config_get_boolean("uaegfx_card");
    if (uaegfx_card == 1) {
        if (!c->allow_z3_memory) {
            fs_emu_warning(_("Option uaegfx.card needs a CPU with 32-bit "
                    "addressing"));
        }
        else {
            amiga_set_option("gfxcard_size", "32");
        }
    }

    const char *serial_port = fs_config_get_const_string("serial_port");
    if (!serial_port) {
    }
    else if (fs_ascii_strcasecmp(serial_port, "none") == 0) {
    }
    else {
        amiga_enable_serial_port(fs_config_get_const_string("serial_port"));
    }

    configure_accuracy(c);

    if (fs_config_get_boolean("line_doubling") == 0) {
        fs_log("disabling line doubling\n");
        amiga_set_option("gfx_linemode", "none");
    }

    if (fs_config_get_boolean("low_resolution") == 1) {
        fs_log("force low resolution\n");
        amiga_set_option("gfx_lores", "true");
    }

    if (fs_config_get_const_string("dongle_type")) {
        amiga_set_option("dongle", fs_config_get_const_string("dongle_type"));
    }

    int stereo_separation = fs_config_get_int_clamped(
        "stereo_separation", 0, 100);
    if (stereo_separation == FS_CONFIG_NONE) {
        stereo_separation = 100;
    }
    stereo_separation = stereo_separation / 10;
    amiga_set_option_and_free("sound_stereo_separation",
        fs_strdup_printf("%d", stereo_separation), free);
    if (c->enhanced_audio_filter) {
        amiga_set_option("sound_filter_type", "enhanced");
    }

	configure_cpuboard();

    /*
    if (g_fs_uae_amiga_model == MODEL_A500) {
        if (slow_memory || fast_memory || chip_memory > 512) {
            fs_log("using A500 and memory expansions, "
                    "enabling real-time clock");
            amiga_set_option("rtc", "MSM6242B");
        }
    }
    */

    /*
    char **keys = g_key_file_get_keys(g_fs_uae_config, "uae", NULL, NULL);
    if (keys) {
        for (char **key = keys; *key; key++) {
            char *value = g_key_file_get_string(g_fs_uae_config, "uae",
                    *key, NULL);
            if (value != NULL) {
                amiga_set_option(*key, value);
                free(value);
            }
        }
        g_strfreev(keys);
    }
    */
}

/*
static void set_default_dirs_from_file_path(const char *path) {
    fs_emu_log("set_default_dirs_from_file_path %s\n", path);
    char *dir = g_path_get_dirname(path);
    fs_emu_log("default dir = %s\n", dir);
    g_fs_uae_default_dir = dir;
}
*/

void fs_uae_configure_cdrom() {
    /*
    if (g_fs_uae_amiga_model != MODEL_CDTV && g_fs_uae_amiga_model != MODEL_CD32) {
        return;
    }
    */
    fs_emu_log("configure_cdrom\n");
    int auto_num_drives = 0;
    char *path = fs_config_get_string("cdrom_drive_0");
    if (path) {
        path = fs_uae_expand_path_and_free(path);
        path = fs_uae_resolve_path_and_free(path, FS_UAE_CD_PATHS);
        //set_default_dirs_from_file_path(path);
        char* temp = fs_strconcat(path, ",image", NULL);
        amiga_set_option("cdimage0", temp);
        free(temp);
        free(path);
        auto_num_drives = 1;
    }

    int num_drives = auto_num_drives;
    const char *value = fs_config_get_const_string("cdrom_drive_count");
    if (value) {
        if (fs_ascii_strcasecmp(value, "auto") == 0) {
            // auto
        }
        else {
            num_drives = atoi(value);
        }
    }

    if (num_drives == 0) {
        // do nothing
    }
    else if (num_drives == 1) {
        if (g_fs_uae_amiga_model != MODEL_CDTV &&
                g_fs_uae_amiga_model != MODEL_CD32) {
            amiga_set_option("scsi", "true");
            amiga_map_cd_drives(1);
        }
        if (auto_num_drives == 0) {
            // set cdimage0 to force a CD-ROM drive
            amiga_set_option("cdimage0", "");
        }
    }
    else {
        fs_emu_warning(_("Invalid number of CD-ROM drives specified"));
    }
}

static void configure_hard_drive_directory (int index, const char *path,
        const char *device, int read_only, int boot_priority) {
    char *type = fs_strdup("dir");

    char *mount_name;
    char *label_option_name = fs_strdup_printf(
            "hard_drive_%d_label", index);
    char *label_option = fs_config_get_string(label_option_name);
    if (label_option) {
        mount_name = label_option;
    }
    else {
        mount_name = fs_path_get_basename(path);
        int stop = 0;
        for(int i = 0; mount_name[i]; i++) {
            if (mount_name[i] == '(') {
                stop = i;
                break;
            }
        }
        while (stop > 0 && mount_name[stop - 1] == ' ') {
            stop--;
        }
        if (stop > 0) {
            mount_name[stop] = '\0';
        }
    }

    fs_emu_log("hard drive mount: %s\n", path);
    fs_emu_log("device: %s\n", device);
    fs_emu_log("mount name: %s\n", mount_name);
    fs_emu_log("read only: %d\n", read_only);
    fs_emu_log("boot priority: %d\n", boot_priority);

    char *filesystem2_value = fs_strdup_printf("%s,%s:%s:%s,%d",
            read_only ? "ro" : "rw", device, mount_name, path,
            boot_priority);
    amiga_set_option("filesystem2", filesystem2_value);

    free(filesystem2_value);
    free(mount_name);
    free(type);
}

static void configure_hard_drive_image (int index, const char *path,
        const char *device, int read_only, int boot_priority) {
    int rdb_mode = 0;
    char *key = NULL;

    FILE *f = fs_fopen(path, "rb");
    if (f == NULL) {
        fs_emu_log("WARNING: could not open %s\n", path);
    }

    key = fs_strdup_printf("hard_drive_%d_type", index);
    const char *hd_type = fs_config_get_const_string(key);
    free(key);
    if (hd_type && fs_ascii_strcasecmp(hd_type, "rdb") == 0) {
        fs_emu_log("hard drive type explicitly set to rdb\n");
        rdb_mode = 1;
    }
    /*
    else if (enable_rdb_mode == 0) {
        rdb_mode = 0;
    }
    */
    else if (f != NULL) {
        // autodetect rdb mode
        char buffer[5];
        int read = fread(buffer, 1, 4, f);
        buffer[4] = '\0';
        if (read == 4) {
            if (strcmp(buffer, "RDSK") == 0) {
                rdb_mode = 1;
            }
            else if (strcmp(buffer, "rdsk") == 0) {
                // this is a unformatted disk file prepared by for example
                // FS-UAE Launcher, using rdsk to indicate that the file
                // is intended to be used as an RDB file
                rdb_mode = 1;
            }
        }
        else {
            fs_emu_log("WARNING: error reading 4 bytes from HD "
                    "file\n");
        }
    }
    if (f != NULL) {
        fclose(f);
    }

    //char *type = fs_strdup("hdf");
    int sectors = 32;
    int surfaces = 1;
    int reserved = 2;
    int block_size = 512;

    if (rdb_mode) {
        sectors = 0;
        surfaces = 0;
        reserved = 0;
    }

    key = fs_strdup_printf("hard_drive_%d_file_system", index);
    char *file_system = fs_config_get_string(key);
    if (file_system == NULL) {
        file_system = fs_strdup("");
    }
    free(key);
    file_system = fs_uae_expand_path_and_free(file_system);
    file_system = fs_uae_resolve_path_and_free(file_system,
            FS_UAE_HD_PATHS);
    if (file_system[0] && !fs_path_exists(file_system)) {
        char *msg = fs_strdup_printf(
                "file system handler \"%s\" not found", file_system);
        fs_emu_warning(msg);
        free(msg);
        return;
    }

    key = fs_strdup_printf("hard_drive_%d_controller", index);
    char *hd_controller = fs_config_get_string(key);
    if (hd_controller == NULL) {
        hd_controller = fs_strdup("uae");
    }
    free(key);

    fs_emu_log("hard drive file: %s\n", path);
    fs_emu_log("rdb mode: %d\n", rdb_mode);
    fs_emu_log("device: %s\n", device);
    fs_emu_log("read only: %d\n", read_only);
    fs_emu_log("boot priority: %d\n", boot_priority);
    fs_emu_log("surfaces: %d\n", surfaces);
    fs_emu_log("reserved: %d\n", reserved);
    fs_emu_log("hd controller: %s\n", hd_controller);
    fs_emu_log("sectors: %d\n", sectors);
    fs_emu_log("block size: %d\n", block_size);
    fs_emu_log("file system: %s\n", file_system);

    char *hardfile2_value = fs_strdup_printf(
            "%s,%s:%s,%d,%d,%d,%d,%d,%s,%s",
            read_only ? "ro" : "rw", device, path,
            sectors, surfaces, reserved,
            block_size, boot_priority, file_system, hd_controller);
    amiga_set_option("hardfile2", hardfile2_value);

    free(hardfile2_value);
    free(hd_controller);
    free(file_system);
    //free(type);
}

void fs_uae_configure_hard_drives() {
    fs_emu_log("fs_uae_configure_hard_drives\n");

    const char *flags = fs_config_get_const_string("uaem_write_flags");
    if (flags != NULL) {
        uae_set_uaem_write_flags_from_string(flags);
    }

    for (int i = 0; i < 10; i++) {
        char *key = fs_strdup_printf("hard_drive_%d", i);
        char *path = fs_config_get_string(key);
        free(key);
        if (path == NULL) {
            continue;
        }
        /* fs_config_get_string never returns an empty string, NULL is
         * returned if value is empty or key does not exist. */
        path = fs_uae_expand_path_and_free(path);
        path = fs_uae_resolve_path_and_free(path, FS_UAE_HD_PATHS);
        if (!fs_path_exists(path)) {
            char *msg = fs_strdup_printf(_("HD not found: %s"),
                    path);
            fs_emu_warning(msg);
            free(msg);
            free(path);
            continue;
        }
        key = fs_strdup_printf("hard_drive_%d_priority", i);
        int boot_priority = fs_config_get_int(key);
        free(key);
        if (boot_priority == FS_CONFIG_NONE) {
            boot_priority = 0;
        }

        char *device = fs_strdup_printf("DH%d", i);

        int read_only = 0;
        int virtual_hd = 0;
        if (fs_path_is_dir(path)) {
            virtual_hd = 1;
        }
        else if (fs_str_has_suffix(path, ".zip")) {
            virtual_hd = 1;
            //read_write = ro_string;
            read_only = 1;
        }

        key = fs_strdup_printf("hard_drive_%d_read_only", i);
        if (fs_config_get_boolean(key) == 1) {
            //read_write = ro_string;
            read_only = 1;
        }
        free(key);

        if (virtual_hd) {
            configure_hard_drive_directory(i, path, device, read_only,
                    boot_priority);
        }
        else {
            configure_hard_drive_image(i, path, device, read_only,
                    boot_priority);
        }

        free(device);
    }
}

void fs_uae_configure_floppies() {
    if (g_fs_uae_amiga_model == MODEL_CDTV || g_fs_uae_amiga_model == MODEL_CD32) {
        return;
    }
    fs_emu_log("configure_floppies\n");
    char option_floppy_drive_x[] = "floppy_drive_0";
    char option_floppy_drive_x_sounds[] = "floppy_drive_0_sounds";
    char option_floppyx[] = "floppy0";
    char option_floppyxtype[] = "floppy0type";
    char option_floppyxsound[] = "floppy0sound";
    char option_floppyxsoundext[] = "floppy0soundext";
    int auto_num_drives = 1;
    for(int i = 0; i < 4; i++) {
        option_floppy_drive_x[13] = '0' + i;
        option_floppy_drive_x_sounds[13] = '0' + i;
        option_floppyx[6] = '0' + i;
        option_floppyxtype[6] = '0' + i;
        option_floppyxsound[6] = '0' + i;
        option_floppyxsoundext[6] = '0' + i;

        char *path = fs_config_get_string(option_floppy_drive_x);
        fs_emu_log("value for option %s: %s\n", option_floppy_drive_x, path);
        if (!path) {
            path = fs_strdup("");
        }

        if (path[0] != '\0') {
            path = fs_uae_expand_path_and_free(path);
            path = fs_uae_resolve_path_and_free(path, FS_UAE_FLOPPY_PATHS);
            auto_num_drives = i + 1;
        }
        amiga_set_option(option_floppyx, path);
        amiga_set_option(option_floppyxtype, "0");
        free(path);

        const char *floppy_sounds = fs_config_get_const_string(
                option_floppy_drive_x_sounds);
        if (floppy_sounds) {
            fs_log("custom floppy sounds for drive %d: %s\n", i,
                    floppy_sounds);
            amiga_set_option(option_floppyxsound, "-1");
            amiga_set_option(option_floppyxsoundext, floppy_sounds);
        }
    }
    const char *value;
    value = fs_config_get_const_string("floppy_drive_speed");
    if (value) {
        amiga_set_option("floppy_speed", value);
    }

    int num_drives = auto_num_drives;
    value = fs_config_get_const_string("floppy_drive_count");
    if (value) {
        if (fs_ascii_strcasecmp(value, "auto") == 0) {
            // auto
        }
        else {
            num_drives = atoi(value);
        }
    }
    amiga_set_int_option("nr_floppies", num_drives);

    // set remaining floppy drive types to -1
    for (int i = num_drives; i < 4; i++) {
        option_floppyx[6] = '0' + i;
        option_floppyxtype[6] = '0' + i;
        amiga_set_option(option_floppyx, "");
        amiga_set_option(option_floppyxtype, "-1");
    }

    int volume = fs_config_get_int_clamped("floppy_drive_volume", 0, 100);
    if (volume != FS_CONFIG_NONE) {
        if (volume == 0) {
            for (int i = 0; i < 4; i++) {
                char *key = fs_strdup_printf("floppy%dsound", i);
                amiga_set_option(key, "0");
                free(key);
            }
        }
        amiga_set_int_option("floppy_volume", 100 - volume);
    }

    int count = 0;
    int k = 0;
    while (count < 20) {
        char *config_key = fs_strdup_printf("floppy_image_%d", k);
        const char *config_value = fs_config_get_const_string(config_key);
        if (config_value) {
            char *option = fs_strdup_printf("diskimage%d", count);
            char *path = fs_uae_expand_path(config_value);
            path = fs_uae_resolve_path_and_free(path, FS_UAE_FLOPPY_PATHS);
            amiga_set_option(option, path);
            free(path);
            free(option);
            count++;
        }
        free(config_key);
        if (k > 0 && !config_value) {
            // allow to start at floppy_image_0 or floppy_image_1
            break;
        }
        k++;
    }
    while (count < 20) {
        // set remaining floppy list entries to the empty string
        char *option = fs_strdup_printf("diskimage%d", count);
        amiga_set_option(option, "");
        free(option);
        count++;
    }

    if (fs_config_get_boolean("writable_floppy_images") == 1) {
        amiga_floppy_set_writable_images(1);
    }
}
