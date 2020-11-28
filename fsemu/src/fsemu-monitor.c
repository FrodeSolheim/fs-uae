#include "fsemu-internal.h"
#include "fsemu-monitor.h"

#include "fsemu-glib.h"
#include "fsemu-sdl.h"
#include "fsemu-util.h"
#include "fsemu-window.h"

#ifdef FSEMU_LINUX
#include <dlfcn.h>
#endif

typedef struct {
    int x;
    int y;
    int w;
    int h;
    double scale;
} fsemu_monitor_scale;

static struct {
    int count;
    GArray *list;
    fsemu_monitor_scale scales[FSEMU_MONITOR_MAX_COUNT];
} fsemu_monitor;

static gint fsemu_monitor_compare(gconstpointer a, gconstpointer b)
{
    const fsemu_monitor_t *am = (const fsemu_monitor_t *) a;
    const fsemu_monitor_t *bm = (const fsemu_monitor_t *) b;

    return am->rect.x - bm->rect.x;
}

#ifdef FSEMU_LINUX

typedef struct {
    int x, y;
    int width, height;
} fsemu_monitor_GdkRectangle;

static void *fsemu_monitor_dlsym(void *handle, const char *name)
{
    void *result = dlsym(handle, name);
    if (!result) {
        printf("[DSPLY] [MONITOR] Could not load %s\n", name);
        dlclose(handle);
        return NULL;
    }
    return result;
}

static void fsemu_monitor_read_scale_via_gdk(void)
{
#if 1
    // GDK allocates some stuff that will be counted as memory leaks, so when
    // running the leak check, might as well skip using GDK.
    if (strcmp(fsemu_getenv("FSEMU_LEAK_CHECK"), "1") == 0) {
        return;
    }
#endif
    bool (*gdk_init_check)(int *argc, char ***argv);
    void *(*gdk_display_get_default)(void);
    int (*gdk_display_get_n_monitors)(void *display);
    void *(*gdk_display_get_monitor)(void *display, int monitor_num);
    void (*gdk_monitor_get_geometry)(void *monitor,
                                     fsemu_monitor_GdkRectangle *geometry);
    int (*gdk_monitor_get_scale_factor)(void *monitor);

    void (*gdk_display_close)(void *display);
    void *handle = dlopen("libgdk-3.so.0", RTLD_GLOBAL | RTLD_LAZY);
    if (!handle) {
        printf("[DSPLY] [MONITOR] Could not open libgdk-3.so.0: %s\n",
               dlerror());
        return;
    }
    if (!(gdk_init_check = fsemu_monitor_dlsym(handle, "gdk_init_check"))) {
        dlclose(handle);
        return;
    }
    if (!(gdk_display_get_default =
              fsemu_monitor_dlsym(handle, "gdk_display_get_default"))) {
        dlclose(handle);
        return;
    }
    if (!(gdk_display_get_n_monitors =
              fsemu_monitor_dlsym(handle, "gdk_display_get_n_monitors"))) {
        dlclose(handle);
        return;
    }
    if (!(gdk_display_get_monitor =
              fsemu_monitor_dlsym(handle, "gdk_display_get_monitor"))) {
        dlclose(handle);
        return;
    }
    if (!(gdk_monitor_get_geometry =
              fsemu_monitor_dlsym(handle, "gdk_monitor_get_geometry"))) {
        dlclose(handle);
        return;
    }
    if (!(gdk_monitor_get_scale_factor =
              fsemu_monitor_dlsym(handle, "gdk_monitor_get_scale_factor"))) {
        dlclose(handle);
        return;
    }
    if (!(gdk_display_close =
              fsemu_monitor_dlsym(handle, "gdk_display_close"))) {
        dlclose(handle);
        return;
    }
    char *argv[] = {NULL};
    char **args = argv;
    gdk_init_check(0, &args);
    void *display = gdk_display_get_default();
    if (display) {
        int monitor_count = gdk_display_get_n_monitors(display);
        for (int i = 0; i < monitor_count; i++) {
            void *monitor = gdk_display_get_monitor(display, i);
            if (monitor == NULL) {
                continue;
            }
            fsemu_monitor_GdkRectangle rect;
            gdk_monitor_get_geometry(monitor, &rect);
            int scale = gdk_monitor_get_scale_factor(monitor);
            printf("[DSPLY] %d %d %d %d: scale %d\n",
                   rect.x,
                   rect.y,
                   rect.width,
                   rect.height,
                   scale);
            if (i < FSEMU_MONITOR_MAX_COUNT) {
                fsemu_monitor.scales[i].x = rect.x * scale;
                fsemu_monitor.scales[i].y = rect.y * scale;
                fsemu_monitor.scales[i].w = rect.width * scale;
                fsemu_monitor.scales[i].h = rect.height * scale;
                fsemu_monitor.scales[i].scale = scale;
            }
        }
        // gdk_display_close(display);
    } else {
        printf("[DSPLY] [MONITOR] Could not get default display\n");
    }
    dlclose(handle);
}

#endif

void fsemu_monitor_init(void)
{
    fsemu_return_if_already_initialized();

#ifdef FSEMU_LINUX
    fsemu_monitor_read_scale_via_gdk();
#endif
    fsemu_monitor.list = g_array_new(false, true, sizeof(fsemu_monitor_t));

    // FIXME: Move SDL-specific code to sdlwindow module (later)

    int display_index = 0;
    while (true) {
        SDL_DisplayMode mode;
        int error = SDL_GetDesktopDisplayMode(display_index, &mode);
        if (error) {
            break;
        }

        fsemu_monitor_t monitor;
        monitor.index = display_index;
        SDL_Rect rect;
        error = SDL_GetDisplayBounds(display_index, &rect);
        if (error) {
            fsemu_window_log(
                "Error retrieving display bounds for display %d: %s\n",
                display_index,
                SDL_GetError());
            monitor.rect.x = 0;
            monitor.rect.y = 0;
            monitor.rect.w = 1024;
            monitor.rect.h = 768;
            monitor.refresh_rate = 1;
        } else {
            monitor.rect.x = rect.x;
            monitor.rect.y = rect.y;
            monitor.rect.w = rect.w;
            monitor.rect.h = rect.h;
            monitor.refresh_rate = mode.refresh_rate;
        }

        monitor.scale = 1.0;
#ifdef FSEMU_LINUX
        for (int i = 0; i < FSEMU_MONITOR_MAX_COUNT; i++) {
            if (monitor.rect.x == fsemu_monitor.scales[i].x &&
                monitor.rect.y == fsemu_monitor.scales[i].y &&
                monitor.rect.w == fsemu_monitor.scales[i].w &&
                monitor.rect.h == fsemu_monitor.scales[i].h) {
                monitor.scale = fsemu_monitor.scales[i].scale;
                fsemu_window_log(
                    "[DSPLY] Found scale factor for monitor: %0.2f\n",
                    monitor.scale);
                break;
            }
        }
#endif
#if 0
        float ddpi, hdpi, vdpi;
        if (SDL_GetDisplayDPI(display_index, &ddpi, &hdpi, &vdpi) == 0) {
            fsemu_window_log("[DSPLY] %d: %f %f %f\n",
                            display_index,
                            ddpi, hdpi, vdpi);
        } else {

        }
#endif

        fsemu_window_log("[DSPLY] %d: %dx%d+%d+%d %d Hz Scale %0.2f\n",
                         display_index,
                         monitor.rect.w,
                         monitor.rect.h,
                         monitor.rect.x,
                         monitor.rect.y,
                         monitor.refresh_rate,
                         monitor.scale);
        g_array_append_val(fsemu_monitor.list, monitor);
        display_index += 1;
    }
    fsemu_monitor.count = display_index;

    g_array_sort(fsemu_monitor.list, fsemu_monitor_compare);
    for (int i = 0; i < fsemu_monitor.count; i++) {
        g_array_index(fsemu_monitor.list, fsemu_monitor_t, i).index = i;
        /* Set physical position flags (left, m-left, m-right, right) */
        int flags = 0;
        for (int j = 0; j < 4; j++) {
            int pos = (fsemu_monitor.count - 1.0) * j / 3.0 + 0.5;
            fsemu_window_log("Monitor - j %d pos %d\n", j, pos);
            if (pos == i) {
                flags |= (1 << j);
            }
        }
        fsemu_window_log("Monitor index %d flags %d\n", i, flags);
        g_array_index(fsemu_monitor.list, fsemu_monitor_t, i).flags = flags;
    }
}

int fsemu_monitor_count(void)
{
    return fsemu_monitor.count;
}

bool fsemu_monitor_get_by_index(int index, fsemu_monitor_t *monitor)
{
    if (index < 0 || index >= fsemu_monitor.count) {
        monitor->index = -1;
        monitor->flags = 0;
        monitor->rect.x = 0;
        monitor->rect.y = 0;
        monitor->rect.w = 1024;
        monitor->rect.h = 768;
        monitor->refresh_rate = 1;
        return false;
    }
    SDL_assert(monitor != NULL);
    memcpy(monitor,
           &g_array_index(fsemu_monitor.list, fsemu_monitor_t, index),
           sizeof(fsemu_monitor_t));
    return true;
}

bool fsemu_monitor_get_by_flag(int flag, fsemu_monitor_t *monitor)
{
    for (int i = 0; i < fsemu_monitor.count; i++) {
        if ((g_array_index(fsemu_monitor.list, fsemu_monitor_t, i).flags &
             flag) == flag) {
            fsemu_window_log("Monitor: found index %d for flag %d\n", i, flag);
            return fsemu_monitor_get_by_index(i, monitor);
        }
    }
    fsemu_monitor_get_by_index(0, monitor);
    return false;
}

bool fsemu_monitor_get_by_rect(fsemu_rect_t *rect, fsemu_monitor_t *monitor)
{
    for (int i = 0; i < fsemu_monitor.count; i++) {
        fsemu_monitor_t *m =
            &g_array_index(fsemu_monitor.list, fsemu_monitor_t, i);
        if (m->rect.x == rect->x && m->rect.y == rect->y &&
            m->rect.w == rect->w && m->rect.h == rect->h) {
            fsemu_window_log("Monitor: found index %d for rect %d,%d,%d,%d\n",
                             i,
                             rect->x,
                             rect->y,
                             rect->w,
                             rect->h);
            return fsemu_monitor_get_by_index(i, monitor);
        }
    }
    fsemu_monitor_get_by_index(0, monitor);
    return false;
}

#if 0
int fs_ml_video_mode_get_current(fs_ml_video_mode *mode)
{
    mode->width = 0;
    mode->height = 0;
    mode->fps = 0;
    mode->bpp = 0;
    mode->flags = 0;

    fsemu_monitor_t monitor;
    fs_emu_monitor_get_by_index(g_display, &monitor);
    mode->width = monitor.rect.w;
    mode->height = monitor.rect.h;
    mode->fps = monitor.refresh_rate;

    if (mode->fps == 0) {
        fsemu_window_log("WARNING: refresh rate was not detected\n");
        fsemu_window_log("full video sync will not be enabled automatically, but can "
                "be forced\n");
    }
    return 0;
}
#endif
