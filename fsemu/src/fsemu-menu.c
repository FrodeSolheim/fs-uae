#define FSEMU_INTERNAL
#include "fsemu-menu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fsemu-log.h"
#include "fsemu-refable.h"
#include "fsemu-util.h"

struct fsemu_menu {
    FSEMU_REFABLE;
    // fsemu_menu_item_t *first_item;
    // fsemu_menu_item_t *last_item;
    int allocated_items;
    fsemu_menu_item_t **items;
    int length;
};

struct fsemu_menu_item {
    FSEMU_REFABLE;
    // Store any kind of item data in this pointer. Note, it will not be freed
    // when the menu item is unrefed.
    // FIXME: Is this good enough? Do we need some kind of finalization here?
    // FIXME: Maybe set_data_with_finalizer?
    void *data;
    uint32_t flags;
    // fsemu_menu_item_t *next;
    // fsemu_menu_item_activate_f on_activate;
    fsemu_menu_item_on_activate_f on_activate;
    fsemu_menu_t *parent;
    char *title;
};

static void fsemu_menu_finalize(void *object)
{
    fsemu_menu_t *menu = (fsemu_menu_t *) object;
    for (int i = 0; i < menu->length; i++) {
        fsemu_menu_item_unref(menu->items[i]);
    }
    // It is safe to call free on a NULL pointer, if no items were ever added.
    free(menu->items);
    free(menu);
}

fsemu_menu_t *fsemu_menu_new(void)
{
    fsemu_menu_t *menu = FSEMU_UTIL_MALLOC0(fsemu_menu_t);
    fsemu_refable_init_with_finalizer(menu, fsemu_menu_finalize);
    return menu;
}

void fsemu_menu_ref(fsemu_menu_t *menu)
{
    fsemu_refable_ref(menu);
}

void fsemu_menu_unref(fsemu_menu_t *menu)
{
    fsemu_refable_unref(menu);
}

void fsemu_menu_add_item(fsemu_menu_t *menu, fsemu_menu_item_t *item)
{
    // printf("aa %d (length %d)\n", menu->allocated_items, menu->length);
    if (menu->length + 1 > menu->allocated_items) {
        int allocate = menu->allocated_items ? menu->allocated_items * 2 : 16;
        // It is safe to call realloc with NULL pointer for the initial
        // allocation.
        // printf("realloc %d %d\n", allocate, sizeof(fsemu_menu_item_t *));
        menu->items =
            realloc(menu->items, allocate * sizeof(fsemu_menu_item_t *));
        menu->allocated_items = allocate;
    }
    printf("bb\n");
    menu->items[menu->length] = item;
    menu->length += 1;
    item->parent = menu;
    printf("cc\n");

    // FIXME: Or should reference be assigned over?
    fsemu_menu_item_ref(item);
    printf("dd\n");

#if 0
    if (menu->last_item) {
        menu->last_item->next = item;
        menu->last_item = item;
    } else {
        menu->first_item = item;
        menu->last_item = item;
    }
#endif
}

int fsemu_menu_count_items(fsemu_menu_t *menu)
{
    return menu->length;
}

fsemu_menu_item_t *fsemu_menu_get_item(fsemu_menu_t *menu, int index)
{
    if (index < 0 || index >= menu->length) {
        fsemu_error("%s: index %d out of bounds\n", __func__, index);
        return NULL;
    }
    return menu->items[index];
}

static void fsemu_menu_item_finalize(void *object)
{
    fsemu_menu_item_t *menu_item = (fsemu_menu_item_t *) object;
    free(menu_item);
}

fsemu_menu_item_t *fsemu_menu_item_new(void)
{
    fsemu_menu_item_t *item = FSEMU_UTIL_MALLOC0(fsemu_menu_item_t);
    fsemu_refable_init_with_finalizer(item, fsemu_menu_finalize);
    return item;
}

void fsemu_menu_item_ref(fsemu_menu_item_t *item)
{
    fsemu_refable_ref(item);
}

void fsemu_menu_item_unref(fsemu_menu_item_t *item)
{
    fsemu_refable_unref(item);
}

fsemu_menu_item_t *fsemu_menu_item_new_with_title(const char *title)
{
    fsemu_menu_item_t *item = fsemu_menu_item_new();
    fsemu_menu_item_set_title(item, title);
    return item;
}

fsemu_menu_item_t *fsemu_menu_item_new_heading_with_title(const char *title)
{
    fsemu_menu_item_t *item = fsemu_menu_item_new_with_title(title);
    fsemu_menu_item_set_flag(item, FSEMU_MENU_ITEM_FLAG_HEADING);
    return item;
}

const char *fsemu_menu_item_title(fsemu_menu_item_t *item)
{
    if (item->title == NULL) {
        return "";
    }
    return item->title;
}

void fsemu_menu_item_set_title(fsemu_menu_item_t *item, const char *title)
{
    if (item->title) {
        free(item->title);
    }
    item->title = strdup(title);
}

bool fsemu_menu_item_check_flag(fsemu_menu_item_t *item, uint32_t flag)
{
    return (item->flags & flag) != 0;
}

void fsemu_menu_item_clear_flag(fsemu_menu_item_t *item, uint32_t flag)
{
    item->flags &= ~flag;
}

void fsemu_menu_item_set_flag(fsemu_menu_item_t *item, uint32_t flag)
{
    item->flags |= flag;
}

bool fsemu_menu_item_heading(fsemu_menu_item_t *item)
{
    return fsemu_menu_item_check_flag(item, FSEMU_MENU_ITEM_FLAG_HEADING);
}

void fsemu_menu_item_set_heading(fsemu_menu_item_t *item, bool heading)
{
    if (heading) {
        fsemu_menu_item_set_flag(item, FSEMU_MENU_ITEM_FLAG_HEADING);
    } else {
        fsemu_menu_item_clear_flag(item, FSEMU_MENU_ITEM_FLAG_HEADING);
    }
}

bool fsemu_menu_item_enabled(fsemu_menu_item_t *item)
{
    return !fsemu_menu_item_check_flag(item, FSEMU_MENU_ITEM_FLAG_DISABLED);
}

void fsemu_menu_item_set_enabled(fsemu_menu_item_t *item, bool enabled)
{
    if (enabled) {
        fsemu_menu_item_clear_flag(item, FSEMU_MENU_ITEM_FLAG_DISABLED);
    } else {
        fsemu_menu_item_set_flag(item, FSEMU_MENU_ITEM_FLAG_DISABLED);
    }
}

void *fsemu_menu_item_data(fsemu_menu_item_t *item)
{
    return item->data;
}

void fsemu_menu_item_set_data(fsemu_menu_item_t *item, void *data)
{
    item->data = data;
}

bool fsemu_menu_item_selectable(fsemu_menu_item_t *item)
{
    return !fsemu_menu_item_heading(item) && fsemu_menu_item_enabled(item);
}

#if 0
void fsemu_menu_item_set_on_activate(fsemu_menu_item_t *item,
                                     fsemu_menu_item_activate_f on_activate)
{
    item->on_activate = on_activate;
}
#endif

void fsemu_menu_item_on_activate(fsemu_menu_item_t *item,
                                 fsemu_menu_item_on_activate_f on_activate)
{
    item->on_activate = on_activate;
}

fsemu_menu_t *fsemu_menu_item_activate(fsemu_menu_item_t *item)
{
    if (item->on_activate == NULL) {
        fsemu_warning("Tried to activate a menu item with function\n");
        return NULL;
    }
    fsemu_menu_t *new_menu = item->on_activate(item);
    return new_menu;
}
