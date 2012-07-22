#include <uae/uae.h>
#include <fs/emu.h>
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
    char *path;

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
        if (chip_memory % 512 == 0) {
            amiga_set_int_option("chipmem_size", chip_memory / 512);
        }
        else {
            fs_emu_warning("chip_memory must be a multiple of 512");
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
            fs_emu_warning("slow_memory must be a multiple of 256");
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
            fs_emu_warning("fast_memory must be a multiple of 1024");
            fast_memory = 0;
        }
    }
    else {
        fast_memory = 0;
    }
    int z3_memory = fs_config_get_int("zorro_iii_memory");
    if (z3_memory != FS_CONFIG_NONE) {
        if (z3_memory && !c->allow_z3_memory) {
            fs_emu_warning("Zorro III fast memory needs a processor "
                    "with 32-bit addressing");
            z3_memory = 0;
        }
        else if (z3_memory % 1024 == 0) {
            amiga_set_int_option("z3mem_size", z3_memory / 1024);
        }
        else {
            fs_emu_warning("zorro_iii_memory must be a multiple of 1024");
            z3_memory = 0;
        }
    }
    else {
        z3_memory = 0;
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

    amiga_enable_serial_port();
    configure_accuracy(c);

    if (fs_config_get_boolean("line_doubling") == 0) {
        fs_log("disabling line doubling\n");
        amiga_set_option("gfx_linemode", "none");
    }

    if (fs_config_get_boolean("low_resolution") == 1) {
        fs_log("force low resolution\n");
        amiga_set_option("gfx_lores", "true");
    }

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
    char *path = fs_config_get_string("cdrom_drive_0");
    if (path) {
        path = fs_uae_expand_path_and_free(path);
        path = fs_uae_resolve_path_and_free(path, FS_UAE_CD_PATHS);
        //set_default_dirs_from_file_path(path);
        char* temp = fs_strconcat(path, ",", NULL);
        amiga_set_option("cdimage0", temp);
        free(temp);
        free(path);

        if (g_fs_uae_amiga_model != MODEL_CDTV &&
                g_fs_uae_amiga_model != MODEL_CD32) {
            amiga_set_option("scsi", "true");
            amiga_map_cd_drives(1);
        }
    }
}

void fs_uae_configure_hard_drives() {
    static const char *ro_string = "ro";
    static const char *rw_string = "rw";
    const char *read_write = rw_string;
    fs_emu_log("fs_uae_configure_hard_drives\n");
    for(int i = 0; i < 10; i++) {
        char *fs_uae_option = fs_strdup_printf("hard_drive_%d", i);
        char *path = fs_config_get_string(fs_uae_option);
        free(fs_uae_option);
        if (path == NULL) {
            continue;
        }
        if (path[0] == '\0') {
            continue;
        }
        path = fs_uae_expand_path_and_free(path);
        path = fs_uae_resolve_path_and_free(path, FS_UAE_HD_PATHS);
        if (!fs_path_exists(path)) {
            char *msg = fs_strdup_printf("HD path \"%s\" does not exist",
                    path);
            fs_emu_warning(msg);
            free(msg);
            continue;
        }
        int boot_priority = -i;
        read_write = rw_string;
        char *device = fs_strdup_printf("DH%d", i);

        int virtual = 0;
        if (fs_path_is_dir(path)) {
            virtual = 1;
        }
        else if (fs_str_has_suffix(path, ".zip")) {
            virtual = 1;
            read_write = ro_string;
        }

        fs_uae_option = fs_strdup_printf("hard_drive_%d_read_only", i);
        if (fs_config_get_boolean(fs_uae_option) == 1) {
            read_write = ro_string;
        }
        free(fs_uae_option);

        if (virtual) {
            char *type = fs_strdup("dir");
            int surfaces = 1;
            int reserved = 2;
            //char *hd_controller = g_strdup("ide0");
            int sectors = 32;
            int block_size = 512;

            char *mount_name;
            char *label_option_name = fs_strdup_printf(
                    "hard_drive_%d_label", i);
            char *label_option = fs_config_get_string(label_option_name);
            if (label_option) {
                mount_name = label_option;
            }
            else {
                mount_name = fs_path_get_basename(path);
            }

            fs_emu_log("hard drive mount: %s\n", path);
            fs_emu_log("device: %s\n", device);
            fs_emu_log("mount name: %s\n", mount_name);
            fs_emu_log("read/write: %s\n", read_write);
            fs_emu_log("boot priority: %d\n", boot_priority);
            /*
            char *option = fs_strdup_printf("uaehf%d", i);
            char *value = fs_strdup_printf("%s,%s,%s:%s:%s,%d",
                    type, read_write, device, mount_name, path,
                    boot_priority);
            amiga_set_option(option, value);
            */

            char *option2 = fs_strdup("filesystem2");
            char *value2 = fs_strdup_printf("%s,%s:%s:%s,%d",
                    read_write, device, mount_name, path,
                    boot_priority);
            amiga_set_option(option2, value2);
            free(option2);
            free(value2);

            // uaehf0=hdf,rw,DH0:path.hdf,32,1,2,512,0,,uae;

            //free(option);
            //free(value);
            free(device);
            free(type);
            free(mount_name);
            continue;
        }
        else if (fs_path_exists(path)) {
            int rdb_mode = 0;

            FILE *f = fs_fopen(path, "rb");
            if (f == NULL) {
                fs_emu_log("WARNING: could not open %s\n", path);
            }
            else {
                char buffer[5];
                int read = fread(buffer, 1, 4, f);
                buffer[4] = '\0';
                //printf("read %d bytes %s\n", read, buffer);
                if (read == 4) {
                    if (strcmp(buffer, "RDSK") == 0) {
                        rdb_mode = 1;
                    }
                }
                else {
                    fs_emu_log("WARNING: error reading 4 bytes from HD "
                            "file\n");
                }
                fclose(f);
            }

            char *type = fs_strdup("hdf");
            int surfaces = 1;
            int reserved = 2;
            char *hd_controller = fs_strdup("uae");
            //char *hd_controller = g_strdup("ide0");
            int sectors = 32;
            int block_size = 512;

            if (rdb_mode) {
                surfaces = 0;
                reserved = 0;
                sectors = 0;
            }

            fs_emu_log("hard drive file: %s\n", path);
            fs_emu_log("rdb mode: %d\n", rdb_mode);
            fs_emu_log("device: %s\n", device);
            fs_emu_log("read/write: %s\n", read_write);
            fs_emu_log("boot priority: %d\n", boot_priority);
            fs_emu_log("surfaces: %d\n", surfaces);
            fs_emu_log("reserved: %d\n", reserved);
            fs_emu_log("hd controller: %s\n", hd_controller);
            fs_emu_log("sectors: %d\n", sectors);
            fs_emu_log("block size: %d\n", block_size);

            /*
            char *option = fs_strdup_printf("uaehf%d", i);
            char *value = fs_strdup_printf("%s,%s,%s:%s,%d,%d,%d,%d,%d,%s,%s",
                    type, read_write, device, path, sectors, surfaces,
                    reserved, block_size, boot_priority, "", hd_controller);
            amiga_set_option(option, value);
            */

            char *option2 = fs_strdup("hardfile2");
            char *value2 = fs_strdup_printf("%s,%s:%s,%d,%d,%d,%d,%d,%s,%s",
                    read_write, device, path, sectors, surfaces,
                    reserved, block_size, boot_priority, "", hd_controller);
            amiga_set_option(option2, value2);
            free(option2);
            free(value2);

            //free(option);
            //free(value);
            free(device);
            free(type);
            free(hd_controller);
        }
        else {
            // FIXME: GUI warning
            fs_emu_warning("Hard drive path does not exist");
            fs_emu_log("hard drive path does not exist: %s\n", path);
        }
    }
}

void fs_uae_configure_floppies() {
    if (g_fs_uae_amiga_model == MODEL_CDTV || g_fs_uae_amiga_model == MODEL_CD32) {
        return;
    }
    fs_emu_log("configure_floppies\n");
    char option_dfx[] = "floppy_drive_0";
    char option_floppyx[] = "floppy0";
    char option_floppyxtype[] = "floppy0type";
    int auto_num_drives = 1;
    for(int i = 0; i < 4; i++) {
        option_dfx[13] = '0' + i;
        option_floppyx[6] = '0' + i;
        char *path = fs_config_get_string(option_dfx);
        fs_emu_log("value for option %s: %s\n", option_dfx, path);
        if (path) {
            //if (i == 0) {
            //    set_default_dirs_from_file_path(path);
            //}
            if (path[0] != '\0') {
                path = fs_uae_expand_path_and_free(path);
                path = fs_uae_resolve_path_and_free(path, FS_UAE_FLOPPY_PATHS);
            }
            amiga_set_option(option_floppyx, path);
            free(path);
            auto_num_drives = i + 1;
            option_floppyxtype[6] = '0' + i;
            amiga_set_option(option_floppyxtype, "0");
        }
    }
    char *value;
    value = fs_config_get_string("floppy_drive_speed");
    if (value) {
        amiga_set_option("floppy_speed", value);
        free(value);
    }
    value = fs_config_get_string("floppy_drive_count");
    if (!value || fs_ascii_strcasecmp(value, "auto") == 0) {
        fs_emu_log("automatically set num_drives to %d\n", auto_num_drives);
        value = fs_strdup_printf("%d", auto_num_drives);
    }
    int num_drives = atoi(value);
    amiga_set_int_option("nr_floppies", num_drives);
    // set remaining floppy drive types to -1
    for (int i = num_drives; i < 4; i++) {
        option_floppyxtype[6] = '0' + i;
        amiga_set_option(option_floppyxtype, "-1");
    }
    free(value);

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
        const char *config_value = fs_config_get_string(config_key);
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
}
