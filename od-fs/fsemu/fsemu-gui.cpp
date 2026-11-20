#define FSEMU_INTERNAL
#include "fsemu-gui.h"

#include "fsemu-frame.h"
#include "fsemu-glib.h"
#include "fsemu-mutex.h"
#include "fsemu-thread.h"
#include "fsemu-time.h"
#include "fsemu-titlebar.h"
#include "fsemu-types.h"
#include "fsemu-util.h"
#include "fsemu-video.h"

/*
Z-indices:
...  - -10000 :
-9999 - -1000 : Reserved for FSEMU under the game display
-1            : Game display
0-999         :
1000 - 9999   : Reserved for FSEMU overlaying the game display
10000 - ...   :

-9xxx: performance gui

5xxx : notifications
7xxx : game information
9xxx : titlebar
*/

static struct {
    GList* items;
    fsemu_mutex_t* mutex;
    bool locked;
} fsemu_gui;

void fsemu_gui_init(void) {
    fsemu_return_if_already_initialized();

    fsemu_thread_init();
    fsemu_thread_assert_main();
    fsemu_gui.mutex = fsemu_mutex_create();
}

void fsemu_gui_lock(void) {
    fsemu_mutex_lock(fsemu_gui.mutex);
    fsemu_gui.locked = true;
}

void fsemu_gui_unlock(void) {
    fsemu_gui.locked = false;
    fsemu_mutex_unlock(fsemu_gui.mutex);
}

fsemu_gui_item_t* fsemu_gui_create_item(void) {
    fsemu_thread_assert_main();
    // fsemu_gui_item_t *item = &fsemu_gui.items[fsemu_gui.num_items];
    // fsemu_gui.num_items += 1;
    // return item;
    fsemu_gui_item_t* item = (fsemu_gui_item_t*)malloc(sizeof(fsemu_gui_item_t));
    memset(item, 0, sizeof(fsemu_gui_item_t));
    return item;
}

void fsemu_gui_item_set_visible(fsemu_gui_item_t* item, bool visible) {
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

void fsemu_gui_rectangle(fsemu_gui_item_t* item, int x, int y, int w, int h, uint32_t c) {
    fsemu_thread_assert_main();

    // memset(item, 0, sizeof(fsemu_gui_item_t));
    item->rect.x = x;
    item->rect.y = y;
    item->rect.w = w;
    item->rect.h = h;
    item->color = c;

    // Hack, remove when fully merged into fsemu-widget
    item->first_child = NULL;
    item->last_child = NULL;
    item->name = NULL;
    item->next = NULL;
}

void fsemu_gui_image(fsemu_gui_item_t* item, int x, int y, int w, int h, fsemu_image_t* image) {
    fsemu_thread_assert_main();

    // memset(item, 0, sizeof(fsemu_gui_item_t));
    item->color = FSEMU_RGBA(0xffffffff);
    item->rect.x = x;
    item->rect.y = y;
    item->rect.w = w;
    item->rect.h = h;
    item->image = image;

    // Hack, remove when fully merged into fsemu-widget
    item->first_child = NULL;
    item->last_child = NULL;
    item->name = NULL;
    item->next = NULL;
}

static void fsemu_gui_assert_locked(void) {
    fsemu_assert(fsemu_gui.locked);
}

void fsemu_gui_add_item(fsemu_gui_item_t* item) {
    fsemu_gui_lock();
    fsemu_gui_add_item_unlocked(item);
    fsemu_gui_unlock();
}

void fsemu_gui_add_item_unlocked(fsemu_gui_item_t* item) {
    fsemu_thread_assert_main();
    fsemu_gui_assert_locked();

    fsemu_gui.items = g_list_append(fsemu_gui.items, item);
}

static gint fsemu_gui_item_compare(gconstpointer a, gconstpointer b) {
    const fsemu_gui_item_t* a_item = (const fsemu_gui_item_t*)a;
    const fsemu_gui_item_t* b_item = (const fsemu_gui_item_t*)b;
    return a_item->z_index - b_item->z_index;
}

static void fsemu_gui_snapshot_children(fsemu_gui_item_t* new_item, fsemu_gui_item_t* old_item) {
    new_item->first_child = NULL;
    new_item->last_child = NULL;

    for (fsemu_gui_item_t* old_child = old_item->first_child; old_child;
         old_child = old_child->next) {
        fsemu_gui_item_t* new_child = (fsemu_gui_item_t*)malloc(sizeof(fsemu_gui_item_t));
        memcpy(new_child, old_child, sizeof(fsemu_gui_item_t));
        new_child->parent = new_item;
        new_child->next = NULL;
        if (new_item->last_child) {
            new_item->last_child->next = new_child;
            new_item->last_child = new_child;
        } else {
            new_item->first_child = new_child;
            new_item->last_child = new_child;
        }

        fsemu_gui_snapshot_children(new_child, old_child);
    }
}

fsemu_gui_item_t* fsemu_gui_snapshot(void) {
    fsemu_frame_log_epoch("Snapshot GUI\n");
    // fsemu_gui_assert_locked();
    fsemu_gui_lock();
    fsemu_gui_item_t* snapshot = NULL;
    fsemu_gui_item_t* last_item = NULL;
    GList* items = fsemu_gui.items;
    items = g_list_sort(fsemu_gui.items, fsemu_gui_item_compare);
    fsemu_gui.items = items;

    // GList *items = fsemu_gui.items;

    while (items) {
        fsemu_gui_item_t* old_item = items->data;
        // FIXME: Also need proper widget copy function, properly doing
        // string copies.
        fsemu_gui_item_t* new_item = (fsemu_gui_item_t*)malloc(sizeof(fsemu_gui_item_t));
        memcpy(new_item, old_item, sizeof(fsemu_gui_item_t));

        fsemu_gui_snapshot_children(new_item, old_item);

        if (snapshot == NULL) {
            snapshot = new_item;
        } else {
            last_item->next = new_item;
        }
        last_item = new_item;
        items = items->next;
    }

    fsemu_gui_unlock();
#if 0
    printf("created gui snapshot %p\n", snapshot);
#endif
    return snapshot;
}

static void fsemu_gui_free_recursive(fsemu_gui_item_t* item) {
    fsemu_gui_item_t* child = item->first_child;
    while (child) {
        fsemu_gui_item_t* next = child->next;
        fsemu_gui_free_recursive(child);
        // FIXME: Need proper free function
        // free(child);
        child = next;
    }
    free(item);
}

void fsemu_gui_free_snapshot(fsemu_gui_item_t* snapshot) {
#if 0
    printf("fsemu_gui_free_snapshot %p\n", snapshot);
#endif
    // fsemu_gui_lock();
    fsemu_gui_item_t* item = snapshot;
    while (item) {
        fsemu_gui_item_t* next = item->next;
        fsemu_gui_free_recursive(item);
        item = next;
    }
    // fsemu_gui_unlock();
}
