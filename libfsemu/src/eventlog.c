#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/eventlog.h>
#include <stdio.h>
#include <stdlib.h>
#include <fs/base.h>
#include <fs/filesys.h>
#include <fs/glib.h>
#include <fs/init.h>
#include <fs/thread.h>

#ifdef NOT_USED

static int g_log_events = 0;
static fs_mutex *g_mutex = NULL;
static GList *g_event_list = NULL;
static volatile int g_event = 0;
static volatile int g_event_count = 0;
static FILE *g_event_file;

FS_INIT_FUNCTION(module) {
    g_log_events = getenv("FS_DEBUG_EVENTS") && \
            getenv("FS_DEBUG_EVENTS")[0] == '1';
    if (g_log_events) {
        g_mutex = fs_mutex_create();
        g_event_file = g_fopen("events.dat", "wb");
        if (g_event_file == NULL) {
            printf("error opening events.dat for writing\n");
            g_log_events = 0;
        }
    }
}

static void flush_events()
{
    GList *link = g_event_list;
    while (link) {
        fwrite(link->data, 13, 1, g_event_file);
        free(link->data);
        GList *temp = link;
        link = link->next;
        free(temp);
    }
    g_event_list = NULL;
    g_event_count = 0;
}

static void fs_eventlog_new_event(
        int64_t *event_time, int *event, uint8_t event_type)
{
    FS_INIT(module);
    if (!g_log_events) {
        return;
    }

    int64_t t;
    if (event_time == NULL) {
        t = fs_get_monotonic_time();
    }
    else {
        if (*event_time == 0) {
            *event_time = fs_get_monotonic_time();
        }
        t = *event_time;
    }

    void *data = malloc(13);
    *((uint8_t *) data) = event_type;
    *((int64_t *) data + 5) = t;

    fs_mutex_lock(g_mutex);
    int e = ++g_event;
    *((int32_t *) data + 1) = e;
    g_event_list = g_list_append(g_event_list, data);
    g_event_count++;
    if (g_event_count > 1000000) {
        flush_events();
    }
    fs_mutex_unlock(g_mutex);
    if (event) {
        *event = e;
    }
}

static void fs_eventlog_update_event(
        int64_t event_time, int event, int64_t t1, int64_t t2)
{
    FS_INIT(module);
    if (!g_log_events) {
        return;
    }

    if (t1 == 0) {
        t1 = fs_get_monotonic_time();
    }
    if (t2 == 0) {
        t2 = t1;
    }
    t1 = t1 - event_time;
    t2 = t2 - event_time;

    void *data = malloc(13);
    *((uint8_t *) data) = 255; // event update
    *((int32_t *) data + 1) = event;
    *((uint32_t *) data + 5) = t1;
    *((uint32_t *) data + 9) = t2;

    fs_mutex_lock(g_mutex);
    g_event_list = g_list_append(g_event_list, data);
    g_event_count++;
    fs_mutex_unlock(g_mutex);
}

#endif
