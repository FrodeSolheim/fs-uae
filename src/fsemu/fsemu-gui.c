#define FSEMU_INTERNAL
#include "fsemu-gui.h"

#include "fsemu-thread.h"
#include "fsemu-titlebar.h"
#include "fsemu-types.h"
#include "fsemu-util.h"

#include "fsemu-glib.h"

static struct {
    GList *items;
    fsemu_mutex_t *mutex;
    bool locked;
} fsemu_gui;

void fsemu_gui_init(void)
{
    fsemu_thread_assert_main();
    fsemu_return_if_already_initialized();
    fsemu_gui.mutex = fsemu_mutex_create();
}

void fsemu_gui_lock(void)
{
    fsemu_mutex_lock(fsemu_gui.mutex);
    fsemu_gui.locked = true;
}

void fsemu_gui_unlock(void)
{
    fsemu_gui.locked = false;
    fsemu_mutex_unlock(fsemu_gui.mutex);
}

fsemu_gui_item_t *fsemu_gui_create_item(void)
{
    fsemu_thread_assert_main();
    // fsemu_gui_item_t *item = &fsemu_gui.items[fsemu_gui.num_items];
    // fsemu_gui.num_items += 1;
    // return item;
    fsemu_gui_item_t *item = malloc(sizeof(fsemu_gui_item_t));
    memset(item, 0, sizeof(fsemu_gui_item_t));
    return item;
}

void fsemu_gui_item_set_visible(fsemu_gui_item_t *item, bool visible)
{
    if (item->visible != visible) {
        item->visible = visible;
        item->dirty = true;
    }
}

#if 0
fsemu_gui_item_t *fsemu_gui_create_rectangle(int x, int y, int w, int h, int c)
{
    fsemu_gui_item_t *item = fsemu_gui_create_item();
    item->rect.x = x;
    item->rect.y = y;
    item->rect.w = w;
    item->rect.h = h;
    return item;
}
#endif

void fsemu_gui_rectangle(
    fsemu_gui_item_t *item, int x, int y, int w, int h, int c)
{
    fsemu_thread_assert_main();
    memset(item, 0, sizeof(fsemu_gui_item_t));
    item->rect.x = x;
    item->rect.y = y;
    item->rect.w = w;
    item->rect.h = h;
    item->color = c;
}

void fsemu_gui_image(
    fsemu_gui_item_t *item, int x, int y, int w, int h, fsemu_image_t *image)
{
    fsemu_thread_assert_main();
    memset(item, 0, sizeof(fsemu_gui_item_t));
    item->rect.x = x;
    item->rect.y = y;
    item->rect.w = w;
    item->rect.h = h;
    item->image = image;
}

static void fsemu_gui_assert_locked(void)
{
    fsemu_assert(fsemu_gui.locked);
}

void fsemu_gui_add_item(fsemu_gui_item_t *item)
{
    fsemu_thread_assert_main();
    fsemu_gui_assert_locked();
    fsemu_gui.items = g_list_append(fsemu_gui.items, item);
}

fsemu_gui_item_t *fsemu_gui_snapshot(void)
{
    // fsemu_gui_assert_locked();
    fsemu_gui_lock();
    fsemu_gui_item_t *snapshot = NULL;
    fsemu_gui_item_t *last_item = NULL;
    GList *items = fsemu_gui.items;
    while (items) {
        fsemu_gui_item_t *new_item = malloc(sizeof(fsemu_gui_item_t));
        memcpy(new_item, items->data, sizeof(fsemu_gui_item_t));

        if (snapshot == NULL) {
            snapshot = new_item;
        } else {
            last_item->next = new_item;
        }
        last_item = new_item;
        items = items->next;
    }

    fsemu_gui_unlock();
    return snapshot;
}

void fsemu_gui_free_snapshot(fsemu_gui_item_t *snapshot)
{
    // fsemu_gui_lock();
    fsemu_gui_item_t *item = snapshot;
    while (item) {
        fsemu_gui_item_t *next = item->next;
        free(item);
        item = next;
    }
    // fsemu_gui_unlock();
}
