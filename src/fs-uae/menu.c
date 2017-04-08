#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <string.h>
#include <uae/uae.h>
#include <fs/emu.h>
#include <fs/fs.h>
#include <fs/i18n.h>
#include <fs/time.h>
#include "fs-uae.h"
#include "config-common.h"
#include "options.h"

#define NUM_SAVE_SLOTS 9
/// TRANSLATORS: This is a menu entry and must not be too long: If you struggle
/// to get in short enough, just translate "No Device" instead.
/// The context is that no host device is inserted into the joystick port.
#define NO_HOST_DEVICE _("No Host Device")
/// TRANSLATORS: This is a menu entry and must not be too long: If you struggle
/// to get in short enough, just translate "No Device" instead.
/// The context is that nothing (no emulated device) is inserted into the
/// joystick port.
#define NO_AMIGA_DEVICE _("No Amiga Device")
#define PAUSE_ITEM_INDEX 1
#define INPUT_ITEM_INDEX 7
#define MEDIA_ITEM_INDEX 11

static int pause_function(fs_emu_menu_item *item, void **data) {
    fs_emu_log("pause_function\n");
    //return FS_EMU_MENU_RESULT_CLOSE;
    fs_emu_pause(!fs_emu_is_paused());
    return FS_EMU_MENU_RESULT_UPDATE;
}

static int hard_reset_function(fs_emu_menu_item *item, void **data) {
    fs_emu_log("hard_reset_function\n");
    fs_emu_queue_action(INPUTEVENT_SPC_HARDRESET, 1);
    return FS_EMU_MENU_RESULT_CLOSE | FS_EMU_MENU_RESULT_ROOT;
}

static int soft_reset_function(fs_emu_menu_item *item, void **data) {
    fs_emu_log("soft_reset_function\n");
    fs_emu_queue_action(INPUTEVENT_SPC_SOFTRESET, 1);
    return FS_EMU_MENU_RESULT_CLOSE | FS_EMU_MENU_RESULT_ROOT;
}

static int reset_menu_function(fs_emu_menu_item *menu_item,
        void **result_data) {
    fs_emu_menu_item *item;
    fs_emu_menu *menu = fs_emu_menu_new();

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    /// TRANSLATORS: Reset Amiga as in reboot the computer
    fs_emu_menu_item_set_title(item, _("Reset Amiga"));
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_HEADING);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    /// TRANSLATORS: Soft reset reboots the computer without a power cycle
    fs_emu_menu_item_set_title(item, _("Soft Reset"));
    fs_emu_menu_item_set_activate_function(item, soft_reset_function);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    /// TRANSLATORS: Hard reset is a computer reboot with an emulated power
    /// cycle
    fs_emu_menu_item_set_title(item, _("Hard Reset"));
    fs_emu_menu_item_set_activate_function(item, hard_reset_function);

    *result_data = menu;
    return FS_EMU_MENU_RESULT_MENU;
}

static char *get_floppy_label(const char* path) {
    if (!path || path[0] == '\0') {
        return g_strdup("");
    }
    char *name = g_path_get_basename(path);
#ifdef USE_GLIB
    GError *error = NULL;
    GRegex *re = g_regex_new(
            "([A-Za-z0-9_ ]*[Dd][Ii][Ss][Kk][A-Za-z0-9_ ]*)",
            0, 0, &error);
    if (error) {
        fs_emu_log(" *** error\n");
        return name;
    }
    GMatchInfo *mi = NULL;
    if (!g_regex_match(re, name, 0, &mi) || !g_match_info_matches(mi)) {
        //fs_emu_log(" *** false\n");
        g_match_info_free(mi);
        g_regex_unref(re);
        return name;
    }
    //fs_emu_log(" *** ok?\n");
    char *result = g_match_info_fetch(mi, 1);
    g_match_info_free(mi);
    g_regex_unref(re);
    if (!result) {
        return name;
    }
    g_free(name);
    return result;
#else
    return name;
#endif
}

static char g_input_desc[4][MAX_DEVICE_NAME_LEN + 1] = {};

static void update_input_item(fs_emu_menu_item *item, int port)
{
    int mode = g_fs_uae_input_ports[port].mode;
    g_input_desc[port][0] = '[';
    g_input_desc[port][2] = ']';
    g_input_desc[port][3] = ' ';
    if (mode == AMIGA_JOYPORT_NONE) {
        g_input_desc[port][1] = 'X';
    }
    else if (mode == AMIGA_JOYPORT_DJOY) {
        g_input_desc[port][1] = 'J';
    }
    else if (mode == AMIGA_JOYPORT_CD32JOY) {
        g_input_desc[port][1] = 'C';
    }
    else if (mode == AMIGA_JOYPORT_MOUSE) {
        g_input_desc[port][1] = 'M';
    }
    else {
        g_input_desc[port][1] = '?';
    }
    const char* s = g_fs_uae_input_ports[port].device;
    if (s[0] == '\0') {
        s = NO_HOST_DEVICE;
    }
    else if (strcmp(s, "KEYBOARD") == 0) {
        s = _("Keyboard");
    }
    else if (strcmp(s, "MOUSE") == 0) {
        s = _("Mouse");
    }

    strncpy(g_input_desc[port] + 4, s, MAX_DEVICE_NAME_LEN - 4);
    fs_emu_menu_item_set_title(item, g_input_desc[port]);
}

static void get_drive_for_index(int index, int *type, int *drive) {
    int count = 0;
    int num_floppy_drives = amiga_get_num_floppy_drives();
    int num_cdrom_drives = amiga_get_num_cdrom_drives();
    if (g_fs_uae_amiga_model == MODEL_CD32 ||
            g_fs_uae_amiga_model == MODEL_CDTV) {
        if (num_cdrom_drives < 1) {
            num_cdrom_drives = 1;
        }
    }
    //printf("num drives: floppy %d cd-rom %d\n", num_floppy_drives,
    //        num_cdrom_drives);

    for (int i = 0; i < num_cdrom_drives; i++) {
        if (index == count) {
            *type = 1;
            *drive = i;
            return;
        }
        count++;
    }
    for (int i = 0; i < num_floppy_drives; i++) {
        if (index == count) {
            *type = 0;
            *drive = i;
            return;
        }
        count++;
    }
    *type = -1;
    *drive = 0;
}

static void update_main_menu(fs_emu_menu *menu) {
    fs_emu_log("update_main_menu\n");
    fs_emu_menu_item *item = menu->items[PAUSE_ITEM_INDEX];
    if (fs_emu_is_paused()) {
        fs_emu_menu_item_set_title(item, _("Resume"));
    }
    else {
        fs_emu_menu_item_set_title(item, _("Pause"));
    }
    update_input_item(menu->items[INPUT_ITEM_INDEX], 1);
    update_input_item(menu->items[INPUT_ITEM_INDEX + 1], 0);
    int media_item_first_index = MEDIA_ITEM_INDEX;

    int drive, type;
    for (int i = 0; i < 4; i++) {
        item = menu->items[media_item_first_index + i];
        get_drive_for_index(i, &type, &drive);
        //printf("index %d => %d %d\n", i, type, drive);
        if (type == 0) { // floppy
            if (amiga_floppy_get_drive_type(
                    drive) == AMIGA_FLOPPY_DRIVE_NONE) {
                fs_emu_menu_item_set_title(item, _("Disabled"));
                fs_emu_menu_item_set_enabled(item, 0);
            }
            else {
                const char *path = amiga_floppy_get_file(drive);
                fs_emu_log("floppy in %d: %s\n", drive, path);
                if (path == NULL || *path == 0) {
                    /// TRANSLATORS: Empty or nothing
                    char *title = g_strdup_printf(_("DF%d: Empty"), drive);
                    fs_emu_menu_item_set_title(item, title);
                    free(title);
                }
                else {
                    char *label = get_floppy_label(path);
                    fs_emu_menu_item_set_title(item, label);
                    free(label);
                }
                fs_emu_menu_item_set_enabled(item, 1);
            }
        }
        else if (type == 1) { // CD
            const char *path = amiga_cdrom_get_file(drive);
            fs_emu_log("CD in %d: %s\n", drive, path);
            if (path == NULL || *path == 0) {
                /// TRANSLATORS: Empty or nothing
                char *title = g_strdup_printf(_("CD%d: Empty"), drive);
                fs_emu_menu_item_set_title(item, title);
                free(title);
            }
            else {
                // FIXME: not really that useful for CDs
                char *label = get_floppy_label(path);
                fs_emu_menu_item_set_title(item, label);
                free(label);
            }
            fs_emu_menu_item_set_enabled(item, 1);
        }
        else {
            fs_emu_menu_item_set_title(item, _("Disabled"));
            fs_emu_menu_item_set_enabled(item, 0);
        }
    }
}

static int g_last_save_slot = 0;

static int load_function(fs_emu_menu_item *item, void **data) {
    int slot = item->idata;
    fs_emu_log("load_function slot = %d\n", slot);
    g_last_save_slot = slot;

    //amiga_state_load(slot);
    fs_emu_queue_action(INPUTEVENT_SPC_STATERESTORE1 + slot, 1);
    return FS_EMU_MENU_RESULT_CLOSE | FS_EMU_MENU_RESULT_ROOT;
}

static int save_function(fs_emu_menu_item *item, void **data) {
    int slot = item->idata;
    fs_emu_log("save_function slot = %d\n", slot);
    g_last_save_slot = slot;

    //amiga_state_save(slot);
    fs_emu_queue_action(INPUTEVENT_SPC_STATESAVE1 + slot, 1);
    return FS_EMU_MENU_RESULT_CLOSE | FS_EMU_MENU_RESULT_ROOT;
}

static char *get_state_file(int slot, const char *ext) {
    char *path = g_strdup_printf("%s %d.%s", fs_uae_get_state_base_name(),
            slot + 1, ext);
    return path;
}

static char *check_save_state(int slot) {
    char *title = NULL;
    char *state_file = get_state_file(slot, "uss");
    fs_emu_log("check %s\n", state_file);
    if (fs_path_exists(state_file)) {
        fs_emu_log("exists\n");
        struct fs_stat buf;
        if (fs_stat(state_file, &buf) == 0) {
            //GDate date;
            //g_date_clear(&date, 1);
            //g_date_set_time_t(&date, buf.mtime);
            struct tm tm_struct;
            fs_localtime_r(&buf.mtime, &tm_struct);
            char strbuf[32];
            //g_date_strftime(strbuf, 32, "");
            strftime(strbuf, 32, "%Y-%m-%d %H:%M:%S", &tm_struct);
            //title = fs_strdup_printf("%d", buf.mtime);
            title = g_strdup(strbuf);
        }
    }
    g_free(state_file);
    return title;
}

static int check_save_state_exists(int slot) {
    char *state_file = get_state_file(slot, "uss");
    fs_emu_log("check %s\n", state_file);
    int result = fs_path_exists(state_file);
    free(state_file);
    return result;
}

static void update_save_state_item(fs_emu_menu_item* item, int slot,
        int save) {
    char *title = check_save_state(slot);
    if (title) {
        fs_emu_menu_item_set_title(item, title);
        free(title);
    }
    else {
        fs_emu_menu_item_set_title(item, _("Empty"));
    }
}

static void update_save_states_menu(fs_emu_menu *menu) {
    for (int i = 0; i < NUM_SAVE_SLOTS; i++) {
        fs_emu_menu_item *item = menu->items[1 + i];
        update_save_state_item(item, i, 1);
    }
}

#if 0
static int save_state_menu_function(fs_emu_menu_item *menu_item,
        void **result_data) {

    int slot = menu_item->idata;
    fs_emu_menu_item *item;
    fs_emu_menu *menu = fs_emu_menu_new();
    //fs_emu_menu_set_update_function(menu, update_save_states_menu);

    char *title = check_save_state(slot);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    if (title) {
        fs_emu_menu_item_set_title(item, title);
    }
    else {
        fs_emu_menu_item_set_title(item, _("Empty"));
    }
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_HEADING);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, _("Load"));
    fs_emu_menu_item_set_idata(item, slot);
    fs_emu_menu_item_set_enabled(item, title != NULL);
    fs_emu_menu_item_set_activate_function(item, load_function);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, _("Save"));
    fs_emu_menu_item_set_idata(item, slot);
    fs_emu_menu_item_set_activate_function(item, save_function);

    if (title) {
        free(title);
    }

    *result_data = menu;
    return FS_EMU_MENU_RESULT_MENU;
}
#endif

static int load_states_menu_function(fs_emu_menu_item *unused,
        void **result_data) {
    fs_emu_log("load_states_menu_function\n");
    fs_emu_menu_item *item;
    fs_emu_menu *menu = fs_emu_menu_new();
    fs_emu_menu_set_update_function(menu, update_save_states_menu);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, _("Load State"));
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_HEADING);

    for (int i = 0; i < NUM_SAVE_SLOTS; i++) {
        item = fs_emu_menu_item_new();
        fs_emu_menu_append_item(menu, item);
        fs_emu_menu_item_set_idata(item, i);
        fs_emu_menu_item_set_enabled(item, check_save_state_exists(i));
        fs_emu_menu_item_set_activate_function(item,
                //save_state_menu_function);
                load_function);
    }

    // focus on the last used slot
    menu->index = g_last_save_slot + 1;

    //create_save_state_menu(menu, 1);
    *result_data = menu;
    return FS_EMU_MENU_RESULT_MENU;
}

static int save_states_menu_function(fs_emu_menu_item *unused,
        void **result_data) {
    fs_emu_log("save_states_menu_function\n");
    fs_emu_menu_item *item;
    fs_emu_menu *menu = fs_emu_menu_new();
    fs_emu_menu_set_update_function(menu, update_save_states_menu);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, _("Save State"));
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_HEADING);

    for (int i = 0; i < NUM_SAVE_SLOTS; i++) {
        item = fs_emu_menu_item_new();
        fs_emu_menu_append_item(menu, item);
        fs_emu_menu_item_set_idata(item, i);
        fs_emu_menu_item_set_activate_function(item,
                //save_state_menu_function);
                save_function);
    }

    // focus on the last used slot
    menu->index = g_last_save_slot + 1;

    //create_save_state_menu(menu, 1);
    *result_data = menu;
    return FS_EMU_MENU_RESULT_MENU;
}

static void insert_disk(int drive_index, int disk_index) {
    if (disk_index == -1) {
        fs_emu_log("menu: eject disk from drive %d\n", drive_index);
        int action = INPUTEVENT_SPC_EFLOPPY0 + drive_index;
        fs_emu_queue_action(action, 1);
        //fs_emu_queue_action(action, 0);
        return;
    }
    fs_emu_log("menu: insert disk index %d into df%d\n", disk_index,
            drive_index);
    int action = INPUTEVENT_SPC_DISKSWAPPER_0_0;
    action += drive_index * AMIGA_FLOPPY_LIST_SIZE + disk_index;
    fs_emu_queue_action(action, 1);
}

static void insert_cdrom(int drive_index, int disk_index) {
    if (disk_index == -1) {
        fs_emu_log("menu: eject CD from drive %d\n", drive_index);
        amiga_cdrom_set_file(drive_index, "");
        return;
    }
    fs_emu_log("menu: insert CD index %d into drive %d\n", disk_index,
            drive_index);
    char *key = g_strdup_printf("cdrom_image_%d", disk_index);
    char* path = fs_config_get_string(key);
    free(key);
    if (path == NULL) {
        fs_emu_log("no CD at this index in CD-ROM list\n");
    }
    path = fs_uae_expand_path_and_free(path);
    path = fs_uae_resolve_path_and_free(path, FS_UAE_CD_PATHS);
    amiga_cdrom_set_file(drive_index, path);
    free(path);
}

static int df0_function(fs_emu_menu_item *menu_item, void **result_data) {
    insert_disk(0, fs_emu_menu_item_get_idata(menu_item));
    return FS_EMU_MENU_RESULT_CLOSE;
}

static int df1_function(fs_emu_menu_item *menu_item, void **result_data) {
    insert_disk(1, fs_emu_menu_item_get_idata(menu_item));
    return FS_EMU_MENU_RESULT_CLOSE;
}

static int df2_function(fs_emu_menu_item *menu_item, void **result_data) {
    insert_disk(2, fs_emu_menu_item_get_idata(menu_item));
    return FS_EMU_MENU_RESULT_CLOSE;
}

static int df3_function(fs_emu_menu_item *menu_item, void **result_data) {
    insert_disk(3, fs_emu_menu_item_get_idata(menu_item));
    return FS_EMU_MENU_RESULT_CLOSE;
}

static int cd0_function(fs_emu_menu_item *menu_item, void **result_data) {
    insert_cdrom(0, fs_emu_menu_item_get_idata(menu_item));
    return FS_EMU_MENU_RESULT_CLOSE;
}

static void update_disk_menu(fs_emu_menu *menu) {
    fs_emu_menu_item *item = menu->items[1];
    const char *inserted_path = amiga_floppy_get_file(menu->idata);
    fs_emu_menu_item_set_enabled(item, inserted_path[0] != '\0');
    for (int i = 0; i < menu->count - 2; i++) {
        item = menu->items[i + 2];
        //const char *path = amiga_floppy_get_list_entry(i - 1);
        const char *path = amiga_floppy_get_list_entry(i);
        //printf("inserted in %d: %s\n", menu->idata, inserted_path);
        //printf("at          %d: %s\n", i, path);
        //char *key = fs_strdup_printf("floppy_image_%d", i);
        //char* path = fs_config_get_string(key);
        //free(key);
        if (path == NULL) {
            fs_emu_menu_item_set_enabled(item, 0);
            continue;
        }
        if (strcmp(path, "") == 0) {
            fs_emu_menu_item_set_enabled(item, 0);
        }
        else if (strcmp(inserted_path, path) == 0) {
            fs_emu_menu_item_set_enabled(item, 0);
            // FIXME: move somewhere else?
            menu->index = 2 + i;
        }
        else {
            fs_emu_menu_item_set_enabled(item, 1);
        }
        //free(path);
    }
}

static void update_cd_menu(fs_emu_menu *menu) {
    fs_emu_menu_item *item = menu->items[1];
    const char *inserted_path = amiga_cdrom_get_file(menu->idata);
    // FIXME: enable later
    //fs_emu_menu_item_set_enabled(item, inserted_path[0] != '\0');
    for (int i = 0; i < menu->count - 2; i++) {
        item = menu->items[i + 2];
        char *key = g_strdup_printf("cdrom_image_%d", i);
        char* path = fs_config_get_string(key);
        free(key);
        if (path == NULL) {
            fs_emu_menu_item_set_enabled(item, 0);
            continue;
        }
        if (strcmp(inserted_path, path) == 0) {
            fs_emu_menu_item_set_enabled(item, 0);
            // FIXME: move somewhere else?
            menu->index = 2 + i;
        }
        else {
            fs_emu_menu_item_set_enabled(item, 1);
        }
        free(path);
    }
}

static int media_menu_function(fs_emu_menu_item *menu_item,
        void **result_data) {
    int index = fs_emu_menu_item_get_idata(menu_item);
    int drive, type;
    get_drive_for_index(index, &type, &drive);

    fs_emu_menu_item *item;
    fs_emu_menu *menu = fs_emu_menu_new();

    if (type == 0) { // floppy
        fs_emu_log("disk_menu_function for df%d\n");
        char *str;
        menu->idata = drive;
        fs_emu_menu_set_update_function(menu, update_disk_menu);
        item = fs_emu_menu_item_new();
        fs_emu_menu_append_item(menu, item);
        /// TRANSLATORS: This is a menu entry and must not be too long
        str = g_strdup_printf(_("Insert Into DF%d"), drive);
        fs_emu_menu_item_set_title(item, str);
        free(str);
        fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_HEADING);

        for (int i = -1; i < AMIGA_FLOPPY_LIST_SIZE; i++) {
            item = fs_emu_menu_item_new();
            fs_emu_menu_append_item(menu, item);
            if (i == -1) {
                fs_emu_menu_item_set_title(item, _("Eject"));
            }
            else {
                const char *path = amiga_floppy_get_list_entry(i);
                str = get_floppy_label(path);
                fs_emu_menu_item_set_title(item, str);
                free(str);
            }

            //fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_ITEM);
            fs_emu_menu_item_set_idata(item, i);
            if (drive == 0) {
                fs_emu_menu_item_set_activate_function(item, df0_function);
            }
            else if (drive == 1) {
                fs_emu_menu_item_set_activate_function(item, df1_function);
            }
            else if (drive == 2) {
                fs_emu_menu_item_set_activate_function(item, df2_function);
            }
            else if (drive == 3) {
                fs_emu_menu_item_set_activate_function(item, df3_function);
            }
        }
    }
    else if (type == 1) {
        fs_emu_log("cd_menu_function for CD %d\n");
        char *str;
        menu->idata = drive;
        fs_emu_menu_set_update_function(menu, update_cd_menu);
        item = fs_emu_menu_item_new();
        fs_emu_menu_append_item(menu, item);
        /// TRANSLATORS: This is a menu entry and must not be too long
        str = g_strdup_printf(_("Insert Into CD%d"), drive);
        fs_emu_menu_item_set_title(item, str);
        g_free(str);
        fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_HEADING);

        item = fs_emu_menu_item_new();
        fs_emu_menu_append_item(menu, item);
        fs_emu_menu_item_set_title(item, _("Eject"));
        fs_emu_menu_item_set_idata(item, -1);
        fs_emu_menu_item_set_activate_function(item, cd0_function);
        // FIXME: Disabled because inserting nothing seems to make it crash,
        // currently -needs to be investigated
        //fs_emu_menu_item_set_enabled(item, 0);

        for (int i = 0; ; i++) {
            // FIXME: GET FROM UAE OPTIONS...?
            char *key = g_strdup_printf("cdrom_image_%d", i);
            char *path = fs_config_get_string(key);
            free(key);
            if (path == NULL) {
                if (i == 0) {
                    // try starting from index 1
                    continue;
                }
                break;
            }
            item = fs_emu_menu_item_new();
            fs_emu_menu_append_item(menu, item);
            str = get_floppy_label(path);
            fs_emu_menu_item_set_title(item, str);
            free(str);
            //fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_ITEM);
            fs_emu_menu_item_set_idata(item, i);
            if (drive == 0) {
                fs_emu_menu_item_set_activate_function(item, cd0_function);
            }
            free(path);
        }
    }
    *result_data = menu;
    return FS_EMU_MENU_RESULT_MENU;
}

static void update_input_menu(fs_emu_menu *menu) {
    int port = menu->idata;
    fs_emu_menu_item *item;
    item = menu->items[1];
    const char* s = g_fs_uae_input_ports[port].device;
    if (s[0] == '\0') {
        s = NO_HOST_DEVICE;
    }
    else if (strcmp(s, "KEYBOARD") == 0) {
        s = _("Keyboard");
    }
    else if (strcmp(s, "MOUSE") == 0) {
        s = _("Mouse");
    }
    fs_emu_menu_item_set_title(item, s);

    if (port < 4) {
        item = menu->items[3];
        if (g_fs_uae_input_ports[port].mode == AMIGA_JOYPORT_NONE) {
            fs_emu_menu_item_set_title(item, NO_AMIGA_DEVICE);
        }
        else if (g_fs_uae_input_ports[port].mode == AMIGA_JOYPORT_MOUSE) {
            fs_emu_menu_item_set_title(item, _("Mouse Mode"));
        }
        else if (g_fs_uae_input_ports[port].mode == AMIGA_JOYPORT_DJOY) {
            fs_emu_menu_item_set_title(item, _("Joystick Mode"));
        }
        else if (g_fs_uae_input_ports[port].mode == AMIGA_JOYPORT_CD32JOY) {
            fs_emu_menu_item_set_title(item, _("CD32 Pad Mode"));
        }
        else {
            fs_emu_menu_item_set_title(item, "???");
        }

        item = menu->items[4];
        if (g_fs_uae_input_ports[port].autofire_mode) {
            fs_emu_menu_item_set_title(item, _("Auto-Fire is On"));
        }
        else {
            fs_emu_menu_item_set_title(item, _("Auto-Fire is Off"));
        }
    }
}

static void set_input_port(int port, const char *device, int remove_other)
{
    strncpy(g_fs_uae_input_ports[port].device, device, MAX_DEVICE_NAME_LEN);
    if (remove_other) {
        for (int i = 0; i < FS_UAE_NUM_INPUT_PORTS; i++) {
            if (i == port) {
                continue;
            }
            if (strcmp(g_fs_uae_input_ports[i].device, device) == 0) {
                strcpy(g_fs_uae_input_ports[i].device, "");
            }
        }
    }
}

static int input_device_function(fs_emu_menu_item *menu_item,
        void **result_data) {
    int port = fs_emu_menu_item_get_idata(menu_item);
    int index = port & 0xff;
    port = port >> 8;

    if (index == 255) {
        fs_log("[menu] port %d set device to \"%s\"\n", port, "");
        set_input_port(port, "", 0);
    }
#if 0
    else if (index == 254) {
        fs_log("[menu] port %d set device to \"%s\"\n", port, "MOUSE");
        set_input_port(port, "MOUSE", 1);
    }
    else if (index == 253) {
        fs_log("[menu] port %d set device to \"%s\"\n", port, "KEYBOARD");
        set_input_port(port, "KEYBOARD", 1);
    }
#endif
    else {
        fs_emu_input_device device;
        if (!fs_ml_input_device_get(index, &device)) {
            return FS_EMU_MENU_RESULT_NONE;
        }
        fs_log("[menu] port %d set device to %s\n", port, device.name);
        set_input_port(port, device.name, 1);
    }
    fs_uae_reconfigure_input_ports_host();
    return FS_EMU_MENU_RESULT_BACK;
}

static int input_host_menu_function(fs_emu_menu_item *menu_item,
        void **result_data) {
    int port = fs_emu_menu_item_get_idata(menu_item);
    char *str;
    fs_emu_menu_item *item;
    fs_emu_menu *menu = fs_emu_menu_new();
    menu->idata = port;

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    /// TRANSLATORS: This is a menu entry and must not be too long
    str = g_strdup_printf(_("Joystick Port %d Device"), port);
    fs_emu_menu_item_set_title(item, str);
    g_free(str);
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_HEADING);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, NO_HOST_DEVICE);
    fs_emu_menu_item_set_idata(item, (port << 8) | 255);
    fs_emu_menu_item_set_activate_function(item, input_device_function);
    if (strcmp(g_fs_uae_input_ports[port].device, "") == 0) {
        menu->index = 1;
    }

#if 0
    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, _("Mouse"));
    fs_emu_menu_item_set_idata(item, (port << 8) | 254);
    fs_emu_menu_item_set_activate_function(item, input_device_function);
    if (strcmp(g_fs_uae_input_ports[port].device, "MOUSE") == 0) {
        menu->index = 2;
    }

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, _("Keyboard"));
    fs_emu_menu_item_set_idata(item, (port << 8) | 253);
    fs_emu_menu_item_set_activate_function(item, input_device_function);
    if (strcmp(g_fs_uae_input_ports[port].device,
            "KEYBOARD") == 0) {
        menu->index = 3;
    }
#endif

    fs_emu_input_device device;
    for (int i = 0; i < FS_ML_INPUT_DEVICES_MAX; i++) {
        if (!fs_ml_input_device_get(i, &device)) {
            continue;
        }
        /*
        if (strcmp(device.name, "KEYBOARD") == 0) {
            continue;
        }
        */
        if (strcmp(g_fs_uae_input_ports[port].device,
                device.name) == 0) {
            // menu->index = 4 + i;
            menu->index = 2 + i;
        }
        item = fs_emu_menu_item_new();
        fs_emu_menu_append_item(menu, item);
        const char* s = device.name;
        if (strcmp(s, "KEYBOARD") == 0) {
            s = _("Keyboard");
        }
        else if (strcmp(s, "MOUSE") == 0) {
            s = _("Mouse");
        }
        fs_emu_menu_item_set_title(item, s);
        fs_emu_menu_item_set_idata(item, (port << 8) | i);
        fs_emu_menu_item_set_activate_function(item, input_device_function);
    }

    *result_data = menu;
    return FS_EMU_MENU_RESULT_MENU;
}

static int input_type_function(fs_emu_menu_item *menu_item,
        void **result_data) {
    int port = fs_emu_menu_item_get_idata(menu_item);
    int mode = port & 0xff;
    port = port >> 8;
    if (port < 4) {
        fs_log("[menu] port %d set mode to %d\n", port, mode);
        g_fs_uae_input_ports[port].new_mode = mode;
        fs_uae_reconfigure_input_ports_amiga();
    }
    else {
        // this is set directly, locally, for custom joystick ("port 4")
        g_fs_uae_input_ports[port].mode = mode;
        g_fs_uae_input_ports[port].new_mode = mode;
        fs_uae_reconfigure_input_ports_host();
    }
    return FS_EMU_MENU_RESULT_BACK;
}

static int input_amiga_menu_function(fs_emu_menu_item *menu_item,
        void **result_data) {
    int port = fs_emu_menu_item_get_idata(menu_item);
    char *str;
    fs_emu_menu_item *item;
    fs_emu_menu *menu = fs_emu_menu_new();
    menu->idata = port;

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    /// TRANSLATORS: This is a menu entry and must not be too long
    str = g_strdup_printf(_("Joystick Port %d Mode"), port);
    fs_emu_menu_item_set_title(item, str);
    g_free(str);
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_HEADING);

    int index = 0;

    item = fs_emu_menu_item_new();
    index++;
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, NO_AMIGA_DEVICE);
    fs_emu_menu_item_set_idata(item, (port << 8) | 0);
    fs_emu_menu_item_set_activate_function(item, input_type_function);
    if (g_fs_uae_input_ports[port].mode == 0) {
        menu->index = index;
    }

    if (port < 2) {
        item = fs_emu_menu_item_new();
        index++;
        fs_emu_menu_append_item(menu, item);
        fs_emu_menu_item_set_title(item, _("Mouse Mode"));
        fs_emu_menu_item_set_idata(item, (port << 8) | AMIGA_JOYPORT_MOUSE);
        fs_emu_menu_item_set_activate_function(item, input_type_function);
        if (g_fs_uae_input_ports[port].mode == AMIGA_JOYPORT_MOUSE) {
            menu->index = index;
        }
    }

    if (port < 4) {
        item = fs_emu_menu_item_new();
        index++;
        fs_emu_menu_append_item(menu, item);
        fs_emu_menu_item_set_title(item, _("Joystick Mode"));
        fs_emu_menu_item_set_idata(item, (port << 8) | AMIGA_JOYPORT_DJOY);
        fs_emu_menu_item_set_activate_function(item, input_type_function);
        if (g_fs_uae_input_ports[port].mode == AMIGA_JOYPORT_DJOY) {
            menu->index = index;
        }
    }

    if (port < 2) {
        item = fs_emu_menu_item_new();
        index++;
        fs_emu_menu_append_item(menu, item);
        fs_emu_menu_item_set_title(item, _("CD32 Pad Mode"));
        fs_emu_menu_item_set_idata(item, (port << 8) | AMIGA_JOYPORT_CD32JOY);
        fs_emu_menu_item_set_activate_function(item, input_type_function);
        if (g_fs_uae_input_ports[port].mode == AMIGA_JOYPORT_CD32JOY) {
            menu->index = index;
        }
    }

    if (port >= 4) {
        item = fs_emu_menu_item_new();
        index++;
        fs_emu_menu_append_item(menu, item);
        fs_emu_menu_item_set_title(item, _("Custom Mode"));
        fs_emu_menu_item_set_idata(item, (port << 8) | AMIGA_JOYPORT_DJOY);
        fs_emu_menu_item_set_activate_function(item, input_type_function);
        if (g_fs_uae_input_ports[port].mode == AMIGA_JOYPORT_DJOY) {
            menu->index = index;
        }
    }

    *result_data = menu;
    return FS_EMU_MENU_RESULT_MENU;
}

static int input_autofire_function(fs_emu_menu_item *menu_item,
        void **result_data) {
    int port = fs_emu_menu_item_get_idata(menu_item);
    g_fs_uae_input_ports[port].new_autofire_mode = \
            !g_fs_uae_input_ports[port].new_autofire_mode;
    fs_log("[menu] port %d toggle autofire mode %d\n", port,
            g_fs_uae_input_ports[port].new_autofire_mode);
    fs_uae_reconfigure_input_ports_amiga();
    return FS_EMU_MENU_RESULT_NONE;
}

static int input_menu_function(
        fs_emu_menu_item *menu_item, void **result_data)
{
    int port = fs_emu_menu_item_get_idata(menu_item);
    fs_emu_log("input_menu_function for port %d\n", port);

    char *str;
    fs_emu_menu_item *item;
    fs_emu_menu *menu = fs_emu_menu_new();
    menu->idata = port;
    fs_emu_menu_set_update_function(menu, update_input_menu);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    /// TRANSLATORS: This is a menu entry and must not be too long
    if (port == 0) {
        str = g_strdup_printf(_("Mouse Port"));
    }
    else if (port == 1) {
        str = g_strdup_printf(_("Joystick Port"));
    }
    else {
        str = g_strdup_printf(_("Joystick Port %d"), port);
    }
    fs_emu_menu_item_set_title(item, str);
    g_free(str);
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_HEADING);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_MENU);
    fs_emu_menu_item_set_idata(item, port);
    fs_emu_menu_item_set_activate_function(item, input_host_menu_function);

    if (port < 4) {
        item = fs_emu_menu_item_new();
        fs_emu_menu_append_item(menu, item);
        fs_emu_menu_item_set_title(item, _("Port Settings"));
        fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_HEADING);

        item = fs_emu_menu_item_new();
        fs_emu_menu_append_item(menu, item);
        fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_MENU);
        fs_emu_menu_item_set_idata(item, port);
        fs_emu_menu_item_set_activate_function(item, input_amiga_menu_function);

        // autofire option
        item = fs_emu_menu_item_new();
        fs_emu_menu_append_item(menu, item);
        fs_emu_menu_item_set_idata(item, port);
        fs_emu_menu_item_set_activate_function(item, input_autofire_function);
    }

    *result_data = menu;
    return FS_EMU_MENU_RESULT_MENU;
}

static void add_input_item(fs_emu_menu *menu, int index)
{
    fs_emu_menu_item *item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    gchar *title;
    if (index == 0) {
        title = g_strdup_printf(_("Mouse Port"));
    } else if (index == 1) {
        title = g_strdup_printf(_("Joystick Port"));
    } else {
        title = g_strdup_printf(_("Joystick Port %d"), index);
    }
    fs_emu_menu_item_set_title(item, title);
    g_free(title);
    if (index >= 4) {
        if (g_fs_uae_input_ports[index].mode == AMIGA_JOYPORT_NONE) {
            fs_emu_menu_item_set_enabled(item, 0);
        }
    }
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_MENU);
    fs_emu_menu_item_set_idata(item, index);
    fs_emu_menu_item_set_activate_function(item, input_menu_function);
}

static void update_input_options_menu(fs_emu_menu *menu)
{
    update_input_item(menu->items[1], 1);
    update_input_item(menu->items[3], 0);
    update_input_item(menu->items[5], 2);
    update_input_item(menu->items[6], 3);
    for (int i = 4; i < FS_UAE_NUM_INPUT_PORTS; i++) {
        update_input_item(menu->items[4 + i], i);
    }
}

static int input_options_menu_function(
        fs_emu_menu_item *menu_item, void **result_data)
{
    fs_emu_log("input_options_menu_function\n");

    fs_emu_menu_item *item;
    fs_emu_menu *menu = fs_emu_menu_new();
    fs_emu_menu_set_update_function(menu, update_input_options_menu);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, _("Joystick Port"));
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_HEADING);

    add_input_item(menu, 1);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, _("Mouse Port"));
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_HEADING);

    add_input_item(menu, 0);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    /// TRANSLATORS: This is a menu entry and must not be too long
    fs_emu_menu_item_set_title(item, _("Parallel Joystick Ports"));
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_HEADING);

    add_input_item(menu, 2);
    add_input_item(menu, 3);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    /// TRANSLATORS: This is a menu entry and must not be too long
    fs_emu_menu_item_set_title(item, _("Custom Joystick Ports"));
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_HEADING);

    for (int i = 4; i < FS_UAE_NUM_INPUT_PORTS; i++) {
        add_input_item(menu, i);
    }

    *result_data = menu;
    return FS_EMU_MENU_RESULT_MENU;
}

void fs_uae_configure_menu() {
    fs_emu_log("fs_uae_configure_menu\n");
    fs_emu_menu_item *item;
    fs_emu_menu *menu = fs_emu_menu_new();
    fs_emu_menu_set_update_function(menu, update_main_menu);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, _("Emulator Control"));
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_HEADING);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, _("Pause"));
    fs_emu_menu_item_set_activate_function(item, pause_function);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, _("Load State"));
    fs_emu_menu_item_set_activate_function(item, load_states_menu_function);
    if (fs_config_false(OPTION_SAVE_STATES)) {
        fs_emu_menu_item_set_enabled(item, 0);
    }

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, _("Save State"));
    fs_emu_menu_item_set_activate_function(item, save_states_menu_function);
    if (fs_config_false(OPTION_SAVE_STATES)) {
        fs_emu_menu_item_set_enabled(item, 0);
    }
/*
    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, _("More..."));
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_MENU);
    fs_emu_menu_item_set_enabled(item, 0);
*/
    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, _("Amiga Control"));
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_HEADING);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, _("Reset Amiga"));
    fs_emu_menu_item_set_activate_function(item, reset_menu_function);
/*
    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, _("More..."));
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_MENU);
    fs_emu_menu_item_set_enabled(item, 0);
*/
    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, _("Input Options"));
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_HEADING);

    add_input_item(menu, 1);
    add_input_item(menu, 0);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, _("More..."));
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_MENU);
    //fs_emu_menu_item_set_enabled(item, 0);
    fs_emu_menu_item_set_activate_function(item, input_options_menu_function);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, _("Removable Media"));
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_HEADING);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, "");
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_MENU);
    fs_emu_menu_item_set_idata(item, 0);
    fs_emu_menu_item_set_activate_function(item, media_menu_function);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, "");
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_MENU);
    fs_emu_menu_item_set_idata(item, 1);
    fs_emu_menu_item_set_activate_function(item, media_menu_function);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, "");
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_MENU);
    fs_emu_menu_item_set_idata(item, 2);
    fs_emu_menu_item_set_activate_function(item, media_menu_function);

    item = fs_emu_menu_item_new();
    fs_emu_menu_append_item(menu, item);
    fs_emu_menu_item_set_title(item, "");
    fs_emu_menu_item_set_type(item, FS_EMU_MENU_ITEM_TYPE_MENU);
    fs_emu_menu_item_set_idata(item, 3);
    fs_emu_menu_item_set_activate_function(item, media_menu_function);

    fs_emu_menu_set_current(menu);
}
