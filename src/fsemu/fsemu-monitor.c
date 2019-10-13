#define FSEMU_INTERNAL
#include "fsemu-monitor.h"

#include "fsemu-glib.h"
#include "fsemu-sdl.h"
#include "fsemu-window.h"

static int g_fs_emu_monitor_count;
// static fsemu_monitor_t g_fs_emu_monitors[FS_EMU_MONITOR_MAX_COUNT];
static GArray *g_fs_emu_monitors;

static gint fsemu_monitor_compare(gconstpointer a, gconstpointer b)
{
    fsemu_monitor_t *am = (fsemu_monitor_t *) a;
    fsemu_monitor_t *bm = (fsemu_monitor_t *) b;

    return am->rect.x - bm->rect.x;
}

void fsemu_monitor_init(void)
{
    fsemu_return_if_already_initialized();

    g_fs_emu_monitors = g_array_new(false, true, sizeof(fsemu_monitor_t));

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
        fsemu_window_log("[DISPLAY] %d: %dx%d+%d+%d @%d\n",
                         display_index,
                         monitor.rect.w,
                         monitor.rect.h,
                         monitor.rect.x,
                         monitor.rect.y,
                         monitor.refresh_rate);
        g_array_append_val(g_fs_emu_monitors, monitor);
        display_index += 1;
    }
    g_fs_emu_monitor_count = display_index;

    g_array_sort(g_fs_emu_monitors, fsemu_monitor_compare);
    for (int i = 0; i < g_fs_emu_monitor_count; i++) {
        g_array_index(g_fs_emu_monitors, fsemu_monitor_t, i).index = i;
        /* Set physical position flags (left, m-left, m-right, right) */
        int flags = 0;
        for (int j = 0; j < 4; j++) {
            int pos = (g_fs_emu_monitor_count - 1.0) * j / 3.0 + 0.5;
            fsemu_window_log("Monitor - j %d pos %d\n", j, pos);
            if (pos == i) {
                flags |= (1 << j);
            }
        }
        fsemu_window_log("Monitor index %d flags %d\n", i, flags);
        g_array_index(g_fs_emu_monitors, fsemu_monitor_t, i).flags = flags;
    }
}

int fsemu_monitor_count()
{
    return g_fs_emu_monitor_count;
}

bool fsemu_monitor_get_by_index(int index, fsemu_monitor_t *monitor)
{
    if (index < 0 || index >= g_fs_emu_monitor_count) {
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
           &g_array_index(g_fs_emu_monitors, fsemu_monitor_t, index),
           sizeof(fsemu_monitor_t));
    return true;
}

bool fsemu_monitor_get_by_flag(int flag, fsemu_monitor_t *monitor)
{
    for (int i = 0; i < g_fs_emu_monitor_count; i++) {
        if ((g_array_index(g_fs_emu_monitors, fsemu_monitor_t, i).flags &
             flag) == flag) {
            fsemu_window_log("Monitor: found index %d for flag %d\n", i, flag);
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
