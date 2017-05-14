#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <uae/uae.h>
#include <fs/conf.h>
#include <fs/emu.h>
#include <stdlib.h>
#include <fs/i18n.h>
#include <glib.h>
#include "options.h"
#include "config-drives.h"
#include "fs-uae.h"

static int next_uaehfi(void)
{
    static int hfi = 0;
    return hfi++;
}

static bool is_cdtv_cd32(void)
{
    return g_fs_uae_amiga_model == MODEL_CDTV ||
           g_fs_uae_amiga_model == MODEL_CD32;
}

static const char *get_default_hd_controller(void)
{
    if (cfg->default_hd_controller) {
        return cfg->default_hd_controller;
    }
    return "uae";
}

static const char *get_default_cd_controller(void)
{
    if (cfg->default_cd_controller) {
        return cfg->default_cd_controller;
    }
    return "uae";
}

static char *resolve_controller(const char *controller)
{
    static GHashTable *in_use;
    if (in_use == NULL) {
        in_use = g_hash_table_new(g_str_hash, g_str_equal);
    }

    if (strcasecmp(controller, "ide") == 0) {
        controller = "ide%d";
    } else if (strcasecmp(controller, "scsi") == 0) {
        controller = "scsi%d";
    } else if (strcasecmp(controller, "scsi_cpuboard") == 0) {
        controller = "scsi%d_cpuboard";
    } else {
        fs_log("resolve_controller: \"%s\" not known\n", controller);
        if (g_hash_table_lookup(in_use, controller)) {
            /* Only uae controllers can be used more than once */
            if (strcasecmp(controller, "uae") != 0) {
                fs_log("controller already used: %s\n", controller);
            }
        } else {
            g_hash_table_add(in_use, g_strdup(controller));
        }
        return g_strdup(controller);
    }

    for (int i = 0;; i++) {
        char *uae_controller = g_strdup_printf(controller, i);
        if (g_hash_table_lookup(in_use, uae_controller)) {
            g_free(uae_controller);
            continue;
        }
        g_hash_table_add(in_use, uae_controller);
        return g_strdup(uae_controller);
    }
}

void fs_uae_configure_cdrom(void)
{
    fs_emu_log("configure_cdrom\n");

    int auto_num_drives = 0;
    char *path = fs_config_get_string("cdrom_drive_0");
    if (path) {
        path = fs_uae_expand_path_and_free(path);
        path = fs_uae_resolve_path_and_free(path, FS_UAE_CD_PATHS);
        //set_default_dirs_from_file_path(path);
        // FIXME: can possibly remove temp / ,image now
        char *temp;
        if (fs_config_true("cdrom_drive_0_delay")) {
            temp = g_strconcat(path, ",delay:image", NULL);
        } else {
            temp = g_strconcat(path, ",image", NULL);
        }
        amiga_set_option("cdimage0", temp);
        g_free(temp);
        g_free(path);
        auto_num_drives = 1;
    } else if (fs_config_get_const_string("cdrom_image_0")) {
        auto_num_drives = 1;
    }

    int num_drives = auto_num_drives;
    const char *value = fs_config_get_const_string("cdrom_drive_count");
    if (value) {
        if (g_ascii_strcasecmp(value, "auto") == 0) {
            // auto
        } else {
            num_drives = atoi(value);
        }
    }

    if (num_drives == 0) {
        // do nothing
        return;
    } else if (num_drives > 1) {
        fs_emu_warning(_("Max 1 CD-ROM drive supported currently"));
        return;
    }

    if (is_cdtv_cd32()) {
        /* CDTV / CD32 models automatically get a CD-ROM drive */
    } else {
        const char *controller = fs_config_get_const_string(
            "cdrom_drive_0_controller");
        if (controller == NULL) {
            controller = get_default_cd_controller();
        }
        char *uae_controller = resolve_controller(controller);

        int hfi = next_uaehfi();
        char *uaekey = g_strdup_printf("uaehf%d", hfi);
        char *uaeval = g_strdup_printf("cd0,ro,:,0,0,0,2048,0,,%s",
                                       uae_controller);
        amiga_set_option(uaekey, uaeval);
        g_free(uaekey);
        g_free(uaeval);

        if (g_ascii_strcasecmp(uae_controller, "uae") == 0) {
            /* Setting scsi option enables uaescsi.device */
            amiga_set_option("scsi", "true");
            if (fs_config_get_boolean(OPTION_CDFS) == 0) {
                fs_log("CDROM: CDFS option was excplicitly disabled\n");
            } else {
                /* Enables win32_automount_cddrives */
                amiga_map_cd_drives(1);
            }
        }
        g_free(uae_controller);
    }

    if (auto_num_drives == 0) {
        // set cdimage0 to force a CD-ROM drive
        amiga_set_option("cdimage0", "");
    }
}

static void configure_hard_drive_directory(
    int index, const char *path, const char *device, int read_only,
    int boot_priority)
{
    char *type = g_strdup("dir");

    char *mount_name;
    char *label_option_name = g_strdup_printf(
        "hard_drive_%d_label", index);
    char *label_option = fs_config_get_string(label_option_name);
    if (label_option) {
        mount_name = label_option;
    } else {
        mount_name = g_path_get_basename(path);
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

    char *filesystem2_value = g_strdup_printf(
        "%s,%s:%s:%s,%d", read_only ? "ro" : "rw", device, mount_name, path,
        boot_priority);
    amiga_set_option("filesystem2", filesystem2_value);

    int hfi = next_uaehfi();
    char *uaekey = g_strdup_printf("uaehf%d", hfi);
    char *uaeval = g_strdup_printf("dir,%s", filesystem2_value);
    amiga_set_option(uaekey, uaeval);
    g_free(uaekey);
    g_free(uaeval);

    g_free(filesystem2_value);
    g_free(mount_name);
    g_free(type);
}

static void configure_hard_drive_image(
    int index, const char *path, const char *device, int read_only,
    int boot_priority)
{
    int rdb_mode = 0;
    char *key = NULL;

    FILE *f = g_fopen(path, "rb");
    if (f == NULL) {
        fs_emu_log("WARNING: could not open %s\n", path);
    }

    key = g_strdup_printf("hard_drive_%d_type", index);
    const char *hd_type = fs_config_get_const_string(key);
    free(key);
    if (hd_type && g_ascii_strcasecmp(hd_type, "rdb") == 0) {
        fs_emu_log("hard drive type explicitly set to rdb\n");
        rdb_mode = 1;
    }
#if 0
    else if (enable_rdb_mode == 0) {
        rdb_mode = 0;
    }
#endif
    else if (f != NULL) {
        // autodetect rdb mode
        char buffer[5];
        int read = fread(buffer, 1, 4, f);
        buffer[4] = '\0';
        if (read == 4) {
            if (strcmp(buffer, "RDSK") == 0) {
                rdb_mode = 1;
            } else if (strcmp(buffer, "rdsk") == 0) {
                // this is a unformatted disk file prepared by for example
                // FS-UAE Launcher, using rdsk to indicate that the file
                // is intended to be used as an RDB file
                rdb_mode = 1;
            }
        } else {
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

    key = g_strdup_printf("hard_drive_%d_file_system", index);
    char *file_system = fs_config_get_string(key);
    if (file_system == NULL) {
        file_system = g_strdup("");
    }
    g_free(key);
    file_system = fs_uae_expand_path_and_free(file_system);
    file_system = fs_uae_resolve_path_and_free(file_system,
                                               FS_UAE_HD_PATHS);
    if (file_system[0] && !fs_path_exists(file_system)) {
        fs_emu_warning("file system handler \"%s\" not found", file_system);
        return;
    }

    key = g_strdup_printf("hard_drive_%d_controller", index);
    const char *controller = fs_config_get_const_string(key);
    if (controller == NULL) {
        controller = get_default_hd_controller();
    }
    free(key);

    char *uae_controller = resolve_controller(controller);

    fs_emu_log("hard drive file: %s\n", path);
    fs_emu_log("rdb mode: %d\n", rdb_mode);
    fs_emu_log("device: %s\n", device);
    fs_emu_log("read only: %d\n", read_only);
    fs_emu_log("boot priority: %d\n", boot_priority);
    fs_emu_log("surfaces: %d\n", surfaces);
    fs_emu_log("reserved: %d\n", reserved);
    fs_emu_log("hd controller: %s\n", uae_controller);
    fs_emu_log("sectors: %d\n", sectors);
    fs_emu_log("block size: %d\n", block_size);
    fs_emu_log("file system: %s\n", file_system);

    char *hardfile2_value = g_strdup_printf(
        "%s,%s:%s,%d,%d,%d,%d,%d,%s,%s", read_only ? "ro" : "rw", device, path,
        sectors, surfaces, reserved, block_size, boot_priority, file_system,
        uae_controller);
    amiga_set_option("hardfile2", hardfile2_value);

    int hfi = next_uaehfi();
    char *uaekey = g_strdup_printf("uaehf%d", hfi);
    char *uaeval = g_strdup_printf("hdf,%s", hardfile2_value);
    amiga_set_option(uaekey, uaeval);
    g_free(uaekey);
    g_free(uaeval);

    free(hardfile2_value);
    free(uae_controller);
    free(file_system);
    //free(type);
}

void fs_uae_configure_hard_drives()
{
    fs_emu_log("fs_uae_configure_hard_drives\n");

    const char *flags = fs_config_get_const_string("uaem_write_flags");
    if (flags != NULL) {
        uae_set_uaem_write_flags_from_string(flags);
    }

    for (int i = 0; i < 10; i++) {
        char *key = g_strdup_printf("hard_drive_%d", i);
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
            fs_emu_warning(_("HD not found: %s"), path);
            g_free(path);
            continue;
        }
        key = g_strdup_printf("hard_drive_%d_priority", i);
        int boot_priority = fs_config_get_int(key);
        free(key);
        if (boot_priority == FS_CONFIG_NONE) {
            boot_priority = 0;
        }

        char *device = g_strdup_printf("DH%d", i);

        int read_only = 0;
        int virtual_hd = 0;
        if (fs_path_is_dir(path)) {
            virtual_hd = 1;
        } else if (g_str_has_suffix(path, ".zip")) {
            virtual_hd = 1;
            //read_write = ro_string;
            read_only = 1;
        }

        key = g_strdup_printf("hard_drive_%d_read_only", i);
        if (fs_config_get_boolean(key) == 1) {
            //read_write = ro_string;
            read_only = 1;
        }
        g_free(key);

        if (virtual_hd) {
            configure_hard_drive_directory(i, path, device, read_only,
                                           boot_priority);
        } else {
            configure_hard_drive_image(i, path, device, read_only,
                                       boot_priority);
        }

        g_free(device);
    }
}

void fs_uae_configure_floppies()
{
    if (is_cdtv_cd32()) {
        // FIXME: instead, just set default number of floppy drives to 0
        return;
    }
    fs_emu_log("configure_floppies\n");

    if (fs_config_get_int(OPTION_WORKBENCH_DISK) == 1) {
        if (cfg->wb_disk) {
            fs_config_set_string("floppy_drive_0", cfg->wb_disk);
        } else {
            fs_emu_warning("No default WB disk defined for this model");
        }
    }

    char option_floppy_drive_x[] = "floppy_drive_0";
    char option_floppy_drive_x_sounds[] = "floppy_drive_0_sounds";
    char option_floppyx[] = "floppy0";
    char option_floppyxtype[] = "floppy0type";
    char option_floppyxsound[] = "floppy0sound";
    char option_floppyxsoundext[] = "floppy0soundext";
    int auto_num_drives = cfg->default_floppy_drive_count;
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
            path = g_strdup("");
        }

        if (path[0] != '\0') {
            if (g_str_has_prefix(path, "dat://")) {

            } else {
                path = fs_uae_expand_path_and_free(path);
                path = fs_uae_resolve_path_and_free(path, FS_UAE_FLOPPY_PATHS);
                if (!g_file_test(path, G_FILE_TEST_EXISTS)) {
                    fs_emu_warning("Not found: %s", fs_config_get_const_string(
                                       option_floppy_drive_x));
                }
            }
            auto_num_drives = MAX(auto_num_drives, i + 1);
        }
        amiga_set_option(option_floppyx, path);
        amiga_set_int_option(option_floppyxtype,
                             cfg->default_floppy_drive_type);
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
        if (g_ascii_strcasecmp(value, "auto") == 0) {
            // auto
        } else {
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

    int volume = fs_config_get_int_clamped(
                OPTION_FLOPPY_DRIVE_VOLUME, 0, 100);
    if (volume == FS_CONFIG_NONE) {
        volume = 20;
    }
    if (volume == 0) {
        for (int i = 0; i < 4; i++) {
            char *key = g_strdup_printf("floppy%dsound", i);
            amiga_set_option(key, "0");
            g_free(key);
        }
    }
    amiga_set_int_option("floppy_volume", 100 - volume);

    volume = fs_config_get_int_clamped(
                OPTION_FLOPPY_DRIVE_VOLUME_EMPTY, 0, 100);
    if (volume != FS_CONFIG_NONE) {
        for (int i = 0; i < 4; i++) {
            char *key = g_strdup_printf("floppy%dsoundvolume_empty", i);
            amiga_set_int_option(key, 100 - volume);
            g_free(key);
        }
    }

    int count = 0;
    int k = 0;
    while (count < 20) {
        char *config_key = g_strdup_printf("floppy_image_%d", k);
        const char *config_value = fs_config_get_const_string(config_key);
        if (config_value) {
            char *option = g_strdup_printf("diskimage%d", count);
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
        char *option = g_strdup_printf("diskimage%d", count);
        amiga_set_option(option, "");
        free(option);
        count++;
    }

    if (fs_config_get_boolean("writable_floppy_images") == 1) {
        amiga_floppy_set_writable_images(1);
    }
}
