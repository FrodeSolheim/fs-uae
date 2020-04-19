#define FSEMU_INTERNAL
#include "fsemu-mainmenu.h"

#include <stdlib.h>

#include "fsemu-control.h"
#include "fsemu-glib.h"
#include "fsemu-log.h"
#include "fsemu-menu.h"
#include "fsemu-savestate.h"

#define module fsemu_mainmenu

static struct fsemu_mainmenu {
    fsemu_menu_t *cached_menu;
    bool initialized;
} module;

// ----------------------------------------------------------------------------
// Emulator control
// ----------------------------------------------------------------------------

static fsemu_menu_t *fsemu_mainmenu_on_pause(fsemu_menu_item_t *item)
{
    // FIXME: Menu should update and menu item should be updated to "Resume".
    fsemu_control_toggle_paused();
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
    return NULL;
}

static fsemu_menu_t *fsemu_mainmenu_on_savestate_cancel_save(
    fsemu_menu_item_t *item)
{
    // FIXME: Return to previous menu;
    // return -1 ??
    // or just call pop menu and return NULL?
    return NULL;
}

static fsemu_menu_t *fsemu_mainmenu_on_savestate_confirm_save(
    fsemu_menu_item_t *item)
{
    int slot = GPOINTER_TO_INT(fsemu_menu_item_data(item));
    fsemu_savestate_save(slot);
    // FIXME: Also pop one menu...
    return NULL;
}

static fsemu_menu_t *fsemu_mainmenu_on_savestate_save(fsemu_menu_item_t *item)
{
    int slot = GPOINTER_TO_INT(fsemu_menu_item_data(item));
    if (!fsemu_savestate_has_state(slot)) {
        fsemu_savestate_save(slot);
        return NULL;
        // return fsemu_mainmenu_on_savestate_confirm_save(item);
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

static fsemu_menu_t *fsemu_mainmenu_on_savestate(fsemu_menu_item_t *item)
{
    printf("on_savestate\n");
    int slot = GPOINTER_TO_INT(fsemu_menu_item_data(item));

    fsemu_menu_t *newmenu;
    fsemu_menu_item_t *newitem;

    newmenu = fsemu_menu_new();

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
    fsemu_menu_item_set_enabled(newitem, fsemu_savestate_has_state(slot));
    fsemu_menu_add_item(newmenu, newitem);

    return newmenu;
}

static fsemu_menu_t *fsemu_mainmenu_on_savestates(fsemu_menu_item_t *item)
{
    printf("on_savestates\n");
    fsemu_menu_t *newmenu;
    fsemu_menu_item_t *newitem;

    newmenu = fsemu_menu_new();

    newitem = fsemu_menu_item_new_heading_with_title(_("Savestates"));
    fsemu_menu_add_item(newmenu, newitem);

    for (int i = 1; i <= fsemu_savestate_slots(); i++) {
        char *title = g_strdup_printf(_("%d: Empty"), i);
        newitem = fsemu_menu_item_new_with_title(title);
        free(title);
        fsemu_menu_item_set_data(newitem, GINT_TO_POINTER(i));
        fsemu_menu_item_on_activate(newitem, fsemu_mainmenu_on_savestate);
        fsemu_menu_add_item(newmenu, newitem);
    }

    return newmenu;
}

// ----------------------------------------------------------------------------
// Main menu
// ----------------------------------------------------------------------------

static fsemu_menu_t *fsemu_mainmenu_create_menu(void)
{
    printf("fsemu_mainmenu_create_menu\n");
    fsemu_menu_t *newmenu;
    fsemu_menu_item_t *newitem;

    newmenu = fsemu_menu_new();

    printf("0\n");

    // item = fsemu_menu_item_new();
    // fsemu_menu_add_item(menu, item);
    // fsemu_menu_item_set_title(item, _("Emulator Control"));
    // fsemu_menu_item_set_type(item, FSEMU_MENUITEM_TYPE_HEADING);

    newitem = fsemu_menu_item_new_heading_with_title(_("Emulator Control"));
    fsemu_menu_add_item(newmenu, newitem);

    // newitem = fsemu_menu_item_new_with_title(_("Resume"));
    // fsemu_menu_item_on_activate(newitem, fsemu_mainmenu_on_pause);
    // fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new_with_title(_("Pause"));
    fsemu_menu_item_on_activate(newitem, fsemu_mainmenu_on_pause);
    fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new_with_title(_("Fast Forward"));
    fsemu_menu_item_on_activate(newitem, fsemu_mainmenu_on_pause);
    fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new_with_title(_("Savestates"));
    fsemu_menu_add_item(newmenu, newitem);
    fsemu_menu_item_on_activate(newitem, fsemu_mainmenu_on_savestates);

    newitem = fsemu_menu_item_new_heading_with_title(_("Removable Media"));
    fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new_with_title(_("DF0: Empty"));
    fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new_with_title(_("DF1: Empty"));
    fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new_heading_with_title(_("Input Options"));
    fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new_with_title(_("Joystick port 1"));
    fsemu_menu_add_item(newmenu, newitem);

    newitem = fsemu_menu_item_new_with_title(_("Joystick port 0"));
    fsemu_menu_add_item(newmenu, newitem);

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
