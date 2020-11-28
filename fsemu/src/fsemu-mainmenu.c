#include "fsemu-internal.h"
#include "fsemu-mainmenu.h"

#include <stdlib.h>

#include "fsemu-control.h"
#include "fsemu-glib.h"
#include "fsemu-input.h"
#include "fsemu-inputport.h"
#include "fsemu-log.h"
#include "fsemu-media.h"
#include "fsemu-menu.h"
#include "fsemu-savestate.h"
#include "fsemu-util.h"

#define module fsemu_mainmenu

static struct fsemu_mainmenu {
    fsemu_menu_t *cached_menu;

    // fsemu_mainmenu_reset_function_t soft_reset_function;
    // fsemu_mainmenu_reset_function_t hard_reset_function;

    // int first_drive_item_index;
    // int last_drive_item_index;

    bool initialized;
} module;

#if 0
typedef void (*fsemu_mainmenu_reset_function_t)(void);

void fsemu_mainmenu_set_soft_reset_function(
    fsemu_mainmenu_reset_function_t function)
{
    module.soft_reset_function = function;
}

void fsemu_mainmenu_set_hard_reset_function(
    fsemu_mainmenu_reset_function_t function)
{
    module.hard_reset_function = function;
}
#endif

// ----------------------------------------------------------------------------
// Emulator control
// ----------------------------------------------------------------------------

static fsemu_menu_t *fsemu_mainmenu_on_pause(fsemu_menu_item_t *item)
{
    // FIXME: Menu should update and menu item should be updated to "Resume".
    fsemu_control_toggle_paused();
    return NULL;
}

static fsemu_menu_t *fsemu_mainmenu_on_warp(fsemu_menu_item_t *item)
{
    // FIXME: Menu should update and menu item should be updated to "Resume".
    fsemu_control_toggle_warp();
    return NULL;
}

// ----------------------------------------------------------------------------
// Savestates
// ----------------------------------------------------------------------------

static fsemu_menu_t *fsemu_mainmenu_on_savestate_load(fsemu_menu_item_t *item)
{
    int slot = GPOINTER_TO_INT(fsemu_menu_item_data(item));
    // Probably don't need confirmation on load, since you need to navigate
    // down to reach the menu item.
    fsemu_savestate_load(slot);

    // FIXME: Close
    return FSEMU_MENU_RESULT_CLOSE;
}

static fsemu_menu_t *fsemu_mainmenu_on_savestate_cancel_save(
    fsemu_menu_item_t *item)
{
    // FIXME: Return to previous menu;
    // return -1 ??
    // or just call pop menu and return NULL?
    return FSEMU_MENU_RESULT_POP1;
}

static fsemu_menu_t *fsemu_mainmenu_on_savestate_confirm_save(
    fsemu_menu_item_t *item)
{
    int slot = GPOINTER_TO_INT(fsemu_menu_item_data(item));
    fsemu_savestate_save(slot);
    // FIXME: Also pop one menu...
    return FSEMU_MENU_RESULT_POP1_CLOSE;
}

static fsemu_menu_t *fsemu_mainmenu_on_savestate_save(fsemu_menu_item_t *item)
{
    int slot = GPOINTER_TO_INT(fsemu_menu_item_data(item));
    if (!fsemu_savestate_has_state(slot)) {
        fsemu_savestate_save(slot);
        return FSEMU_MENU_RESULT_CLOSE;
    }

    // Save exists in slot, need confirmation. Especially since the save
    // entry is the initially selected one in the menu.

    fsemu_menu_t *newmenu;
    fsemu_menu_item_t *newitem;

    newmenu = fsemu_menu_new();

    char *title = g_strdup_printf(_("Overwrite savestate %d?"), slot);
    newitem = fsemu_menu_item_new_heading_with_title(title);
    free(title);
    fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new_with_title("Confirm overwrite");
    fsemu_menu_item_set_data(newitem, GINT_TO_POINTER(slot));
    fsemu_menu_item_on_activate(newitem,
                                fsemu_mainmenu_on_savestate_confirm_save);
    fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new_with_title("Cancel");
    fsemu_menu_item_set_data(newitem, GINT_TO_POINTER(slot));
    fsemu_menu_item_on_activate(newitem,
                                fsemu_mainmenu_on_savestate_cancel_save);
    fsemu_menu_add_item(newmenu, newitem);

    return newmenu;
}

static void fsemu_mainmenu_update_savestate(fsemu_menu_t *menu)
{
    // printf("----> UPDATE SAVESTATE MENU\n");
    int slot = GPOINTER_TO_INT(fsemu_menu_data(menu));
    fsemu_menu_item_t *item = fsemu_menu_get_item(menu, 2);
    fsemu_menu_item_set_enabled(item, fsemu_savestate_has_state(slot));
}

static fsemu_menu_t *fsemu_mainmenu_on_savestate(fsemu_menu_item_t *item)
{
    printf("on_savestate\n");
    int slot = GPOINTER_TO_INT(fsemu_menu_item_data(item));

    fsemu_menu_t *newmenu;
    fsemu_menu_item_t *newitem;

    newmenu = fsemu_menu_new();
    fsemu_menu_set_update_function(newmenu, fsemu_mainmenu_update_savestate);
    fsemu_menu_set_data(newmenu, GINT_TO_POINTER(slot));

    char *title = g_strdup_printf(_("Savestate %d"), slot);
    newitem = fsemu_menu_item_new_heading_with_title(title);
    free(title);
    fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new_with_title("Save");
    fsemu_menu_item_set_data(newitem, GINT_TO_POINTER(slot));
    fsemu_menu_item_on_activate(newitem, fsemu_mainmenu_on_savestate_save);
    fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new_with_title("Load");
    fsemu_menu_item_set_data(newitem, GINT_TO_POINTER(slot));
    fsemu_menu_item_on_activate(newitem, fsemu_mainmenu_on_savestate_load);
    fsemu_menu_add_item(newmenu, newitem);

    fsemu_mainmenu_update_savestate(newmenu);
    return newmenu;
}

static void fsemu_mainmenu_update_savestates(fsemu_menu_t *menu)
{
    // printf("UPDATE SAVESTATES MENU\n");

    for (int i = 1; i <= fsemu_savestate_slots(); i++) {
        fsemu_menu_item_t *item = fsemu_menu_get_item(menu, i);
        char *title = fsemu_savestate_description(i);
        fsemu_menu_item_set_title(item, title);
        free(title);
        // fsemu_menu_item_set_data(item, GINT_TO_POINTER(i));
        // fsemu_menu_item_on_activate(item, fsemu_mainmenu_on_savestate);
        // fsemu_menu_add_item(newmenu, newitem);
    }
}

static fsemu_menu_t *fsemu_mainmenu_on_savestates(fsemu_menu_item_t *item)
{
    printf("on_savestates\n");
    fsemu_menu_t *newmenu;
    fsemu_menu_item_t *newitem;

    newmenu = fsemu_menu_new();
    fsemu_menu_set_update_function(newmenu, fsemu_mainmenu_update_savestates);

    newitem = fsemu_menu_item_new_heading_with_title(_("Savestates"));
    fsemu_menu_add_item(newmenu, newitem);

    for (int i = 1; i <= fsemu_savestate_slots(); i++) {
        /*
        char *title;
        if (fsemu_savestate_has_state(i)) {
            title = g_strdup_printf(_("%d: ..."), i);
        } else {
            title = g_strdup_printf(_("%d: Empty"), i);
        }
        */
        // char *title = fsemu_savestate_description(i);
        newitem = fsemu_menu_item_new();
        // free(title);
        fsemu_menu_item_set_data(newitem, GINT_TO_POINTER(i));
        fsemu_menu_item_on_activate(newitem, fsemu_mainmenu_on_savestate);
        fsemu_menu_add_item(newmenu, newitem);
    }

    fsemu_menu_update(newmenu);
    return newmenu;
}

// ----------------------------------------------------------------------------
// Reset
// ----------------------------------------------------------------------------

static fsemu_menu_t *fsemu_mainmenu_on_soft_reset(fsemu_menu_item_t *item)
{
    // FIXME: Maybe use actions instead
    //  if (module.soft_reset_function) {
    //     module.soft_reset_function();
    // }

    fsemu_action_post_from_main(FSEMU_ACTION_SOFTRESET);
    return FSEMU_MENU_RESULT_POP1_CLOSE;
}

static fsemu_menu_t *fsemu_mainmenu_on_hard_reset(fsemu_menu_item_t *item)
{
    // FIXME: Maybe use actions instead
    // if (module.hard_reset_function) {
    //     module.hard_reset_function();
    // }

    fsemu_action_post_from_main(FSEMU_ACTION_HARDRESET);
    return FSEMU_MENU_RESULT_POP1_CLOSE;
}

static fsemu_menu_t *fsemu_mainmenu_on_reset(fsemu_menu_item_t *item)
{
    printf("on_reset\n");
    fsemu_menu_t *newmenu;
    fsemu_menu_item_t *newitem;

    newmenu = fsemu_menu_new();
    // fsemu_menu_set_update_function(newmenu,
    // fsemu_mainmenu_update_savestates);

    newitem = fsemu_menu_item_new_heading_with_title(_("Confirm reset"));
    fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new_with_title(_("Soft reset"));
    fsemu_menu_item_on_activate(newitem, fsemu_mainmenu_on_soft_reset);
    fsemu_menu_item_set_enabled(newitem, fsemu_control_soft_reset_allowed());
    fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new_with_title(_("Hard reset"));
    fsemu_menu_item_on_activate(newitem, fsemu_mainmenu_on_hard_reset);
    fsemu_menu_item_set_enabled(newitem, fsemu_control_hard_reset_allowed());
    fsemu_menu_add_item(newmenu, newitem);

    fsemu_menu_update(newmenu);
    return newmenu;
}

// ----------------------------------------------------------------------------
// Media drives
// ----------------------------------------------------------------------------

static fsemu_menu_t *fsemu_mainmenu_on_drive_eject(fsemu_menu_item_t *item)
{
    fsemu_media_drive_t *drive = fsemu_menu_item_data(item);
    fsemu_assert(drive);

    int drive_index = fsemu_media_drive_index(drive);
    fsemu_assert(drive_index >= 0 &&
                 drive_index < FSEMU_MEDIA_DRIVE_MAX_COUNT);

    fsemu_action_post_from_main(FSEMU_ACTION_DRIVE0EJECT + drive_index);

    return NULL;
}

static fsemu_menu_t *fsemu_mainmenu_on_drive_insert(fsemu_menu_item_t *item)
{
    fsemu_media_drive_t *drive = fsemu_menu_item_data(item);
    fsemu_assert(drive);

    int drive_index = fsemu_media_drive_index(drive);
    fsemu_assert(drive_index >= 0 &&
                 drive_index < FSEMU_MEDIA_DRIVE_MAX_COUNT);

    int media_index = fsemu_menu_item_int(item);
    fsemu_assert(media_index >= 0 && media_index < FSEMU_MEDIA_MAX_FILE_COUNT);

    fsemu_action_post_from_main(FSEMU_ACTION_DRIVE0INSERT0 +
                                drive_index * FSEMU_MEDIA_MAX_FILE_COUNT +
                                media_index);

    return NULL;
}

static bool fsemu_mainmenu_compare_media(const char *a, const char *b)
{
    // printf("compare %s %s\n", a, b);
    if (a == NULL && b == NULL) {
        return true;
    } else if (a && b) {
        // FIXME: May have to do a better comparison, account for expansion of
        // media directories (etc)
        // FIXME: Yes, need to...
        return strcmp(a, b) == 0;
    } else {
        return false;
    }
}

static void fsemu_mainmenu_update_drive(fsemu_menu_t *menu)
{
    fsemu_media_drive_t *drive = fsemu_menu_data(menu);
    fsemu_assert(drive);

    fsemu_menu_item_t *item = fsemu_menu_get_item(menu, 1);

    const char *label = fsemu_media_label(fsemu_media_drive_file(drive));
    if (label) {
        const char *old_label = fsemu_menu_item_title(item);
        if (!old_label || strcmp(label, old_label) != 0) {
            fsemu_menu_item_set_title(item, label);
        }
    } else {
        // FIXME: A bit inefficient to create a new name for each update.
        // Perhaps cache the empty status for the item somehow, so we can
        // check if the title needs to be updated.
        const char *name = fsemu_media_drive_name(drive);
        char *title = g_strdup_printf(_("%s: Empty"), name);
        fsemu_menu_item_assign_title(item, title);
    }

    int type = fsemu_media_drive_type(drive);
    int count = fsemu_media_count(type);
    for (int i = 0; i < count; i++) {
        // const char *label = fsemu_media_label(fsemu_media_file(type, i));
        item = fsemu_menu_get_item(menu, 6 + i);
        fsemu_menu_item_set_enabled(
            item,
            fsemu_mainmenu_compare_media(fsemu_media_file(type, i),
                                         fsemu_media_drive_file(drive)) ==
                false);
    }
}

static fsemu_menu_t *fsemu_mainmenu_on_drive(fsemu_menu_item_t *item)
{
    printf("on_drive\n");
    fsemu_media_drive_t *drive = fsemu_menu_item_data(item);
    fsemu_assert(drive);

    fsemu_menu_t *newmenu;
    fsemu_menu_item_t *newitem;

    newmenu = fsemu_menu_new();
    fsemu_menu_set_data(newmenu, drive);
    fsemu_menu_set_update_function(newmenu, fsemu_mainmenu_update_drive);

    newitem =
        fsemu_menu_item_new_heading_with_title(fsemu_media_drive_title(drive));
    fsemu_menu_add_item(newmenu, newitem);

    // Index 1 - inserted media item
    // FIXME: Maybe move eject into a submenu of this...
    newitem = fsemu_menu_item_new();
    fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new_heading_with_title(_("Commands"));
    fsemu_menu_add_item(newmenu, newitem);

    // _("Browse")
    newitem = fsemu_menu_item_new_with_title("Browse (Coming later?)");
    // fsemu_menu_item_on_activate(newitem, fsemu_mainmenu_on_drive_browse);
    fsemu_menu_item_set_data(newitem, drive);
    fsemu_menu_item_set_enabled(newitem, false);
    fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new_with_title(_("Eject"));
    fsemu_menu_item_on_activate(newitem, fsemu_mainmenu_on_drive_eject);
    fsemu_menu_item_set_data(newitem, drive);
    fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new_heading_with_title(_("Media list"));
    fsemu_menu_add_item(newmenu, newitem);

    int type = fsemu_media_drive_type(drive);
    int count = fsemu_media_count(type);
    for (int i = 0; i < count; i++) {
        const char *label = fsemu_media_label(fsemu_media_file(type, i));
        newitem = fsemu_menu_item_new_with_title(label);
        fsemu_menu_item_on_activate(newitem, fsemu_mainmenu_on_drive_insert);
        fsemu_menu_item_set_data_with_int(newitem, drive, i);
        fsemu_menu_add_item(newmenu, newitem);
    }

    fsemu_menu_update(newmenu);
    return newmenu;
}

// ----------------------------------------------------------------------------
// Input ports
// ----------------------------------------------------------------------------

static fsemu_menu_t *fsemu_mainmenu_set_port_type(fsemu_menu_item_t *item)
{
    fsemu_inputport_t *port = fsemu_menu_item_data(item);
    fsemu_assert(port);

    int port_index = fsemu_inputport_index(port);
    fsemu_assert(port_index >= 0 && port_index < FSEMU_INPUT_MAX_PORTS);

    int mode_index = fsemu_menu_item_int(item);
    fsemu_assert(mode_index >= 0 && mode_index < FSEMU_INPUTPORT_MAX_MODES);

    fsemu_input_log("Sending action to change port %d -> mode %d\n",
                    port_index,
                    mode_index);
    fsemu_action_post_from_main(FSEMU_ACTION_PORT0TYPE0 +
                                port_index * FSEMU_INPUTPORT_MAX_MODES +
                                mode_index);

    return FSEMU_MENU_RESULT_POP1;
}

static fsemu_menu_t *fsemu_mainmenu_on_port_type(fsemu_menu_item_t *activated)
{
    fsemu_inputport_t *port = fsemu_menu_item_data(activated);
    fsemu_assert(port);

    int port_index = fsemu_inputport_index(port);
    fsemu_assert(port_index >= 0 && port_index < FSEMU_INPUT_MAX_PORTS);

    fsemu_menu_t *menu;
    fsemu_menu_item_t *item;

    menu = fsemu_menu_new();
    fsemu_menu_set_data(menu, port);

    // FIXME: type or mode?
    char *title = g_strdup_printf("%s mode", fsemu_inputport_name(port));
    item = fsemu_menu_item_new_heading_with_title(title);
    free(title);
    fsemu_menu_add_item(menu, item);

    int count = fsemu_inputport_mode_count(port);
    for (int i = 0; i < count; i++) {
        fsemu_inputmode_t *mode = fsemu_inputport_mode_by_index(port, i);
        item = fsemu_menu_item_new_with_title(fsemu_inputmode_title(mode));
        fsemu_menu_item_on_activate(item, fsemu_mainmenu_set_port_type);
        fsemu_menu_item_set_data_with_int(item, port, i);
        fsemu_menu_add_item(menu, item);

        if (mode == fsemu_inputport_mode(port)) {
            fsemu_menu_item_set_selected_initially(item, true);
        }
    }
    // fsemu_inputmode_t *mode = fsemu_inputport_mode(port);
    // int count fsemu_inputmode_

    // fsemu_menu_select();

    fsemu_menu_update(menu);
    return menu;
}

static fsemu_menu_t *fsemu_mainmenu_set_port_device(fsemu_menu_item_t *item)
{
    fsemu_inputport_t *port = fsemu_menu_item_data(item);
    fsemu_assert(port);

    int port_index = fsemu_inputport_index(port);
    fsemu_assert(port_index >= 0 && port_index < FSEMU_INPUT_MAX_PORTS);

    int device_index = fsemu_menu_item_int(item);
    fsemu_assert(device_index >= -1 && device_index < FSEMU_INPUT_MAX_DEVICES);

    fsemu_input_log("Change port %d -> device %d\n", port_index, device_index);
    // fsemu_action_post_from_main(FSEMU_ACTION_PORT0TYPE0 +
    //                             port_index * FSEMU_INPUTPORT_MAX_MODES +
    //                             mode_index);

    fsemu_inputport_set_device_by_index(port, device_index);

    return FSEMU_MENU_RESULT_POP1;
}

static void fsemu_mainmenu_update_port_device(fsemu_menu_t *menu)
{
    fsemu_inputport_t *port = fsemu_menu_data(menu);
    fsemu_assert(port);

    fsemu_menu_item_t *item;
    item = fsemu_menu_get_item(menu, 1);

    for (int i = 0; i < FSEMU_INPUT_MAX_DEVICES; i++) {
        item = fsemu_menu_get_item(menu, 2 + i);
        fsemu_inputdevice_t *device = fsemu_input_get_device(i);
        // printf("%i %p\n", i, device);
        if (device) {
            fsemu_menu_item_set_title(item, fsemu_inputdevice_name(device));
            fsemu_menu_item_set_enabled(item, true);
        } else {
            fsemu_menu_item_set_title(item, NULL);
            fsemu_menu_item_set_enabled(item, false);
        }
    }

    // FIXME... complete..
}

static fsemu_menu_t *fsemu_mainmenu_on_port_device(
    fsemu_menu_item_t *activated)
{
    fsemu_inputport_t *port = fsemu_menu_item_data(activated);
    fsemu_assert(port);

    int port_index = fsemu_inputport_index(port);
    fsemu_assert(port_index >= 0 && port_index < FSEMU_INPUT_MAX_PORTS);

    fsemu_menu_t *menu;
    fsemu_menu_item_t *item;

    menu = fsemu_menu_new();
    fsemu_menu_set_data(menu, port);
    fsemu_menu_set_update_function(menu, fsemu_mainmenu_update_port_device);

    char *title = g_strdup_printf("%s device", fsemu_inputport_name(port));
    item = fsemu_menu_item_new_heading_with_title(title);
    free(title);
    fsemu_menu_add_item(menu, item);

    item = fsemu_menu_item_new_with_title(_("No host device"));
    fsemu_menu_item_on_activate(item, fsemu_mainmenu_set_port_device);
    fsemu_menu_item_set_data_with_int(item, port, -1);
    fsemu_menu_add_item(menu, item);

    // fsemu_inputdevice

    for (int i = 0; i < FSEMU_INPUT_MAX_DEVICES; i++) {
        fsemu_inputdevice_t *device = fsemu_input_get_device(i);
        // Note; device can be NULL here. Holes are allowed in the device list.

        // if (device == NULL) {
        //     continue;
        // }
        /*
        if (mouse && device->type != FSEMU_INPUTDEVICE_TYPE_MOUSE) {
            continue;
        }
        if (!mouse && device->type == FSEMU_INPUTDEVICE_TYPE_MOUSE) {
            continue;
        }
        if (device->port_index == -1) {
            return device;
        }
        */

        item = fsemu_menu_item_new();
        fsemu_menu_item_on_activate(item, fsemu_mainmenu_set_port_device);
        fsemu_menu_item_set_data_with_int(item, port, i);
        fsemu_menu_add_item(menu, item);

        if (device == fsemu_inputport_device(port)) {
            fsemu_menu_item_set_selected_initially(item, true);
        }
    }

    // int count = fsemu_inputport_mode_count(port);
    // for (int i = 0; i < count; i++) {
    //     fsemu_inputmode_t *mode = fsemu_inputport_mode_by_index(port, i);
    //     item = fsemu_menu_item_new_with_title(fsemu_inputmode_title(mode));
    //     fsemu_menu_item_on_activate(item, fsemu_mainmenu_set_port_type);
    //     fsemu_menu_item_set_data_with_int(item, port, i);
    //     fsemu_menu_add_item(menu, item);

    //     if (mode == fsemu_inputport_mode(port)) {
    //         fsemu_menu_item_set_selected_initially(item, true);
    //     }
    // }
    // fsemu_inputmode_t *mode = fsemu_inputport_mode(port);
    // int count fsemu_inputmode_

    // fsemu_menu_select();

    fsemu_menu_update(menu);
    return menu;
}

static void fsemu_mainmenu_update_port(fsemu_menu_t *menu)
{
    fsemu_inputport_t *port = fsemu_menu_data(menu);
    fsemu_assert(port);

    fsemu_menu_item_t *item;

    item = fsemu_menu_get_item(menu, 1);

    fsemu_inputdevice_t *device = fsemu_inputport_device(port);
    if (device) {
        fsemu_menu_item_set_title(item, fsemu_inputdevice_name(device));
    } else {
        fsemu_menu_item_set_title(item, _("No host device"));
    }

    item = fsemu_menu_get_item(menu, 3);

    fsemu_menu_item_set_title(
        item, fsemu_inputmode_title(fsemu_inputport_mode(port)));

#if 0
    // Disconnect item
    item = fsemu_menu_get_item(menu, 5);
    fsemu_menu_item_set_enabled(item, fsemu_inputport_device(port) != NULL);
#endif
}

#if 0
static fsemu_menu_t *fsemu_mainmenu_on_port_disconnect(fsemu_menu_item_t *item)
{
    fsemu_inputport_t *port = fsemu_menu_item_data(item);
    fsemu_assert(port);

    int port_index = fsemu_inputport_index(port);
    fsemu_assert(port_index >= 0 && port_index < FSEMU_INPUT_MAX_PORTS);

    // fsemu_action_post_from_main(FSEMU_ACTION_PORT0DISCONNECT + port_index);
    fsemu_inputport_set_device_by_index(port, -1);

    return NULL;
}
#endif

static fsemu_menu_t *fsemu_mainmenu_on_port(fsemu_menu_item_t *activated)
{
    printf("on_port\n");
    fsemu_inputport_t *port = fsemu_menu_item_data(activated);
    fsemu_assert(port);

    fsemu_menu_t *menu;
    fsemu_menu_item_t *item;

    menu = fsemu_menu_new();
    fsemu_menu_set_data(menu, port);
    fsemu_menu_set_update_function(menu, fsemu_mainmenu_update_port);

    item = fsemu_menu_item_new_heading_with_title(fsemu_inputport_name(port));
    fsemu_menu_add_item(menu, item);

    // Index 1 - inserted media item
    // FIXME: Maybe move eject into a submenu of this...
    // item = fsemu_menu_item_new();
    // fsemu_menu_add_item(menu, item);

    // item = fsemu_menu_item_new_heading_with_title(_("Host device"));
    // fsemu_menu_add_item(menu, item);

    item = fsemu_menu_item_new();
    fsemu_menu_item_on_activate(item, fsemu_mainmenu_on_port_device);
    fsemu_menu_item_set_data(item, port);
    fsemu_menu_add_item(menu, item);

    item = fsemu_menu_item_new_heading_with_title(_("Port mode"));
    fsemu_menu_add_item(menu, item);

    item = fsemu_menu_item_new();
    fsemu_menu_item_on_activate(item, fsemu_mainmenu_on_port_type);
    fsemu_menu_item_set_data(item, port);
    fsemu_menu_add_item(menu, item);

#if 0
    item = fsemu_menu_item_new_heading_with_title(_("Commands"));
    fsemu_menu_add_item(menu, item);

    item = fsemu_menu_item_new_with_title(_("Disconnect"));
    fsemu_menu_item_on_activate(item, fsemu_mainmenu_on_port_disconnect);
    fsemu_menu_item_set_data(item, port);
    fsemu_menu_add_item(menu, item);

    item = fsemu_menu_item_new_with_title(_("Swap?"));
    // fsemu_menu_item_on_activate(item, fsemu_mainmenu_on_port_disconnect);
    // fsemu_menu_item_set_data(item, port);
    fsemu_menu_item_set_enabled(item, false);
    fsemu_menu_add_item(menu, item);
#endif

#if 0
    // _("Browse")
    item = fsemu_menu_item_new_with_title("Browse (Coming later)");
    // fsemu_menu_item_on_activate(newitem, fsemu_mainmenu_on_drive_browse);
    fsemu_menu_item_set_data(item, port);
    fsemu_menu_item_set_enabled(item, false);
    fsemu_menu_add_item(menu, item);

    item = fsemu_menu_item_new_with_title(_("Eject"));
    fsemu_menu_item_on_activate(item, fsemu_mainmenu_on_drive_eject);
    fsemu_menu_item_set_data(item, drive);
    fsemu_menu_add_item(menu, item);

    item = fsemu_menu_item_new_heading_with_title(_("Media list"));
    fsemu_menu_add_item(menu, item);

    int type = fsemu_media_drive_type(drive);
    int count = fsemu_media_count(type);
    for (int i = 0; i < count; i++) {
        const char *label = fsemu_media_label(fsemu_media_file(type, i));
        item = fsemu_menu_item_new_with_title(label);
        fsemu_menu_item_on_activate(item, fsemu_mainmenu_on_drive_insert);
        fsemu_menu_item_set_data_with_int(item, drive, i);
        fsemu_menu_add_item(menu, item);
    }
#endif

    fsemu_menu_update(menu);
    return menu;
}

// ----------------------------------------------------------------------------
// Main menu
// ----------------------------------------------------------------------------

static void fsemu_mainmenu_update_main(fsemu_menu_t *menu)
{
    fsemu_menu_item_t *item;
    bool paused = fsemu_control_paused();
    item = fsemu_menu_get_item(menu, 1);
    if (paused) {
        fsemu_menu_item_set_title(item, _("Resume"));
    } else {
        fsemu_menu_item_set_title(item, _("Pause"));
    }
    item = fsemu_menu_get_item(menu, 2);
    if (fsemu_control_warp()) {
        fsemu_menu_item_set_title(item, _("Normal speed"));
    } else {
        fsemu_menu_item_set_title(item, _("Fast forward"));
    }
    // fsemu_menu_item_set_enabled(item, !paused);

    int item_index = 5;
    int drive_count = fsemu_media_drive_count();
    for (int i = 0; i < drive_count; i++) {
        item = fsemu_menu_get_item(menu, item_index++);
        fsemu_media_drive_t *drive = fsemu_media_drive_at_index(i);
        const char *label = fsemu_media_label(fsemu_media_drive_file(drive));
        if (label) {
            const char *old_label = fsemu_menu_item_title(item);
            if (!old_label || strcmp(label, old_label) != 0) {
                fsemu_menu_item_set_title(item, label);
            }
        } else {
            // FIXME: A bit inefficient to create a new name for each update.
            // Perhaps cache the empty status for the item somehow, so we can
            // check if the title needs to be updated.
            const char *name = fsemu_media_drive_name(drive);
            char *title = g_strdup_printf(_("%s: Empty"), name);
            fsemu_menu_item_assign_title(item, title);
        }
    }

    item_index += 1;
    int port_count = fsemu_input_port_count();
    for (int i = 0; i < port_count; i++) {
        item = fsemu_menu_get_item(menu, item_index++);
        fsemu_inputport_t *port = fsemu_input_port_by_index(i);
        const char *mode_name = fsemu_inputport_mode_name(port);
        fsemu_assert_release(mode_name);

        fsemu_inputdevice_t *device = fsemu_inputport_device(port);
        const char *device_name = "";
        if (device) {
            device_name = fsemu_inputdevice_name(device);
        }
        if (device_name == NULL) {
            device_name = "No host device";
        }
        // fsemu_menu_item_set_title(item, fsemu_inputdevice_name(device));
#if 0
        const char *mode_desc = "";
        if (mode_name[0] == 'J') {
            mode_desc = "[\xef\xbc\xaa] ";
        } else if (mode_name[0] == 'M') {
            mode_desc = "[\xef\xbc\xad] ";
        } else if (mode_name[0] == 'C') {
            mode_desc = "[\xef\xbc\xa3] ";
        } else if (mode_name[0] == 'N') {
            mode_desc = "[\xef\xbc\xae] ";
        }
        char *title = g_strdup_printf("%s%s", mode_desc, device_name);
#endif
        char *title = g_strdup_printf("[ %c ] %s", mode_name[0], device_name);
        fsemu_menu_item_set_title(item, title);
        free(title);
    }
}

static fsemu_menu_t *fsemu_mainmenu_create_menu(void)
{
    printf("fsemu_mainmenu_create_menu\n");
    fsemu_menu_t *newmenu;
    fsemu_menu_item_t *newitem;

    newmenu = fsemu_menu_new();
    fsemu_menu_set_update_function(newmenu, fsemu_mainmenu_update_main);

    printf("0\n");

    // item = fsemu_menu_item_new();
    // fsemu_menu_add_item(menu, item);
    // fsemu_menu_item_set_title(item, _("Emulator Control"));
    // fsemu_menu_item_set_type(item, FSEMU_MENUITEM_TYPE_HEADING);

    newitem = fsemu_menu_item_new_heading_with_title(_("Emulator control"));
    fsemu_menu_add_item(newmenu, newitem);

    // newitem = fsemu_menu_item_new_with_title(_("Resume"));
    // fsemu_menu_item_on_activate(newitem, fsemu_mainmenu_on_pause);
    // fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new();  //_with_title(_("Pause"));
    fsemu_menu_item_on_activate(newitem, fsemu_mainmenu_on_pause);
    fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new();  // _with_title(_("Fast Forward"));
    fsemu_menu_item_on_activate(newitem, fsemu_mainmenu_on_warp);
    fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new_with_title(_("Savestates"));
    fsemu_menu_add_item(newmenu, newitem);
    fsemu_menu_item_on_activate(newitem, fsemu_mainmenu_on_savestates);

    newitem = fsemu_menu_item_new_heading_with_title(_("Removable media"));
    fsemu_menu_add_item(newmenu, newitem);

    int drive_count = fsemu_media_drive_count();
    // module.first_drive_item_index = fsemu_menu_count_items(newmenu);
    for (int i = 0; i < drive_count; i++) {
        newitem = fsemu_menu_item_new();
        fsemu_menu_item_on_activate(newitem, fsemu_mainmenu_on_drive);
        fsemu_menu_item_set_data(newitem, fsemu_media_drive_at_index(i));
        fsemu_menu_add_item(newmenu, newitem);
    }
    // module.last_drive_item_index = fsemu_menu_count_items(newmenu);

    /*
    newitem = fsemu_menu_item_new_with_title(_("DF0: Empty"));
    fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new_with_title(_("DF1: Empty"));
    fsemu_menu_add_item(newmenu, newitem);
    */

    newitem = fsemu_menu_item_new_heading_with_title(_("Input ports"));
    fsemu_menu_add_item(newmenu, newitem);

    int port_count = fsemu_input_port_count();
    // module.first_drive_item_index = fsemu_menu_count_items(newmenu);
    for (int i = 0; i < port_count; i++) {
        newitem = fsemu_menu_item_new();
        fsemu_menu_item_on_activate(newitem, fsemu_mainmenu_on_port);
        fsemu_menu_item_set_data(newitem, fsemu_input_port_by_index(i));
        fsemu_menu_add_item(newmenu, newitem);
    }

#if 0
    newitem = fsemu_menu_item_new_with_title(_("Joystick port 1"));
    fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new_with_title(_("Joystick port 0"));
    fsemu_menu_add_item(newmenu, newitem);
#endif

    newitem = fsemu_menu_item_new_heading_with_title(_("Machine control"));
    fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new_with_title(_("Reset"));
    fsemu_menu_add_item(newmenu, newitem);
    fsemu_menu_item_on_activate(newitem, fsemu_mainmenu_on_reset);

    fsemu_menu_item_set_enabled(newitem,
                                fsemu_control_soft_reset_allowed() ||
                                    fsemu_control_hard_reset_allowed());

    fsemu_menu_update(newmenu);
    return newmenu;
}

fsemu_menu_t *fsemu_mainmenu_get_menu(void)
{
    if (!module.cached_menu) {
        module.cached_menu = fsemu_mainmenu_create_menu();
    }
    return module.cached_menu;
}

// ----------------------------------------------------------------------------
// Module
// ----------------------------------------------------------------------------

void fsemu_mainmenu_init(void)
{
    if (module.initialized) {
        return;
    }
    module.initialized = true;
    fsemu_log("Initializing mainmenu module\n");
}

#undef module
