#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ml_internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fs/lazyness.h>
#include <fs/log.h>
#include <fs/thread.h>

#include "manymouse.h"

static fs_thread *g_manymouse_thread = NULL;
static int g_first_manymouse_index = 0;
static volatile int g_manymouse_last_index = -1;

#define db_log(name, format, ...) \
    if (g_fs_log_ ## name) { \
        fs_log(format, ## __VA_ARGS__); \
    }

static void *manymouse_thread(void* data)
{
    fs_log("[MANYMOUSE] Thread running\n");

    int k = g_fs_ml_input_device_count;
    g_first_manymouse_index = k;
    int mouse_count = ManyMouse_Init();
    if (mouse_count < 0) {
        fs_log("[MANYMOUSE] Initialization failed (%d)\n", mouse_count);
    }
    else if (mouse_count == 0) {
        fs_log("MANYMOUSE: no mice found\n");
        // no mice found, so we just quit using the library
    }

    for (int i = 0; i < mouse_count; i++) {
        const char *device = ManyMouse_DeviceName(i);
        const char *driver = ManyMouse_DriverName();

        char *name;
        if (device[0] == 0 || g_ascii_strcasecmp(device, "mouse") == 0) {
            name = g_strdup("Mouse: Unnamed Mouse");
        } else {
            name = g_strdup_printf("Mouse: %s", device);
        }
        // fs_ml_input_unique_device_name either returns name, or frees it
        // and return another name, so name must be malloced and owned by
        // caller
        name = fs_ml_input_unique_device_name(name);
        fs_log("MANYMOUSE: Adding %s (%s)\n", name, driver);

        g_fs_ml_input_devices[k].type = FS_ML_MOUSE;
        g_fs_ml_input_devices[k].index = k;
        g_fs_ml_input_devices[k].name = name;
        g_fs_ml_input_devices[k].alias = g_strdup_printf("MOUSE #%d", i + 2);
        k += 1;
    }

    // when done like this, I believe no memory barrier is needed when the
    // other thread polls g_manymouse_last_index
    g_manymouse_last_index = k;

    if (mouse_count < 0) {
        // ManyMouse library was not initialized
        return NULL;
    }

    ManyMouseEvent event;
    fs_ml_event *new_event;
    while (!fs_ml_is_quitting()) {
        // printf("..\n");
        while (ManyMouse_PollEvent(&event)) {
            // printf(" -- event type %d -- \n", event.type);
            if (event.type == MANYMOUSE_EVENT_RELMOTION) {
                // printf("MANYMOUSE_EVENT_RELMOTION\n");
                new_event = fs_ml_alloc_event();
                new_event->type = FS_ML_MOUSEMOTION;
                new_event->motion.device = g_first_manymouse_index + \
                        event.device;
                if (event.item == 0) {
                    new_event->motion.xrel = event.value;
                    new_event->motion.yrel = 0;
                }
                else if (event.item == 1) {
                    new_event->motion.xrel = 0;
                    new_event->motion.yrel = event.value;
                }
                new_event->motion.x = FS_ML_NO_ABSOLUTE_MOUSE_POS;
                new_event->motion.y = FS_ML_NO_ABSOLUTE_MOUSE_POS;

                fs_ml_post_event(new_event);
                // ManyMouseEventType type;
                // unsigned int device;
                // unsigned int item;
                // int value;
                // int minval;
                // int maxval;
            }
            else if (event.type == MANYMOUSE_EVENT_BUTTON) {
                db_log(input, "MANYMOUSE: EVENT_BUTTON "
                       "device %d item %d value %d\n",
                       event.device, event.item, event.value);
                new_event = fs_ml_alloc_event();
                new_event->type = event.value ? FS_ML_MOUSEBUTTONDOWN :
                        FS_ML_MOUSEBUTTONUP;
                new_event->button.state = event.value != 0;
                new_event->button.device = g_first_manymouse_index + \
                        event.device;
                if (event.item == 0) {
                    new_event->button.button = FS_ML_BUTTON_LEFT;
                }
                else if (event.item == 1) {
                    new_event->button.button = FS_ML_BUTTON_RIGHT;
                }
                else if (event.item == 2) {
                    new_event->button.button = FS_ML_BUTTON_MIDDLE;
                }
                else {
                    new_event->button.button = 0;
                }
                fs_ml_post_event(new_event);
            }
            else if (event.type == MANYMOUSE_EVENT_ABSMOTION) {
                // printf("MANYMOUSE_EVENT_ABSMOTION\n");
            }
            else if (event.type == MANYMOUSE_EVENT_SCROLL) {
                db_log(input, "MANYMOUSE: EVENT_SCROLL "
                       "device %d item %d value %d\n",
                       event.device, event.item, event.value);
                new_event = fs_ml_alloc_event();
                new_event->type = FS_ML_MOUSEBUTTONDOWN;
                new_event->button.state = 1;
                new_event->button.device = g_first_manymouse_index + \
                        event.device;
                new_event->button.button = 0;
                if (event.item == 0) {
                    if (event.value == 1) {
                        new_event->button.button = FS_ML_BUTTON_WHEELUP;
                    }
                    else {
                        new_event->button.button = FS_ML_BUTTON_WHEELDOWN;
                    }
                }
                fs_ml_post_event(new_event);
            }
        }
        fs_ml_usleep(1000);
    }

    ManyMouse_Quit();
    return NULL;
}

static void init_manymouse(void)
{
    /* On OS X with HIDManager driver at least, the mice must be polled from
     * the same thread as the one which called ManyMouse_Init, so we do
     * everything (also enumeration in a worker thread) and wait for
     * enumeration to complete.*/

    fs_log("[MANYMOUSE] Copyright (c) 2005-2012 Ryan C. Gordon\n");
    g_manymouse_thread = fs_thread_create("manymouse", manymouse_thread, NULL);
    if (g_manymouse_thread == NULL) {
        fs_log("[MANYMOUSE] Error - could not create ManyMouse thread\n");
        // ManyMouse_Quit();
    } else {
        while (g_manymouse_last_index < 0) {
            fs_ml_usleep(1000);
        }
        g_fs_ml_input_device_count = g_manymouse_last_index;
    }
}

void fs_ml_mouse_init(void)
{
    FS_ML_INIT_ONCE;
    fs_log("[INPUT] fs_ml_mouse_init\n");

    g_fs_ml_first_mouse_index = g_fs_ml_input_device_count;
    int k = g_fs_ml_input_device_count;

    fs_log("[INPUT] Adding system mouse\n");
    g_fs_ml_input_devices[k].type = FS_ML_MOUSE;
    g_fs_ml_input_devices[k].index = k;
    g_fs_ml_input_devices[k].name = g_strdup("MOUSE");
    g_fs_ml_input_devices[k].alias = g_strdup("MOUSE");
    k += 1;
    g_fs_ml_input_device_count = k;
}
