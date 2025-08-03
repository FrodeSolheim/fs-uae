#define FSEMU_INTERNAL
#include "fsemu-main.h"

#include "fsemu-fade.h"
#include "fsemu-frame.h"
#include "fsemu-gui.h"
#include "fsemu-helpbar.h"
#include "fsemu-hud.h"
#include "fsemu-input.h"
#include "fsemu-leds.h"
#include "fsemu-oskeyboard.h"
#include "fsemu-osmenu.h"
#include "fsemu-perfgui.h"
#include "fsemu-quit.h"
#include "fsemu-startupinfo.h"
#include "fsemu-titlebar.h"
#include "fsemu-video.h"

/*
static struct {
    bool initialized;
} fsemu_main;
*/

void fsemu_main_init_module(void)
{
}

// ----------------------------------------------------------------------------

#if 0

bool fsemu_main_is_running(void)
{
    return !fsemu_quit_check();
}

void fsemu_main_update(void)
{
    fsemu_action_update();
    fsemu_fade_update();
    fsemu_helpbar_update();
    fsemu_hud_update();
    fsemu_leds_update();
    fsemu_oskeyboard_update();
    fsemu_osmenu_update();
    fsemu_perfgui_update();
    fsemu_startupinfo_update();
    fsemu_titlebar_update();
}

void fsemu_main_update_and_snapshot_gui(void)
{
    fsemu_main_update();

    fsemu_gui_item_t *snapshot = fsemu_gui_snapshot();
    fsemu_video_set_gui_snapshot_mt(snapshot);
}

static void fsemu_main_handle_events_until_next_frame_in_main(void)
{
    fsemu_gui_item_t *snapshot = fsemu_video_get_gui_snapshot_vt();
    fsemu_video_render_gui_early(snapshot);

    while (true) {
        // The SDL window subsystem will put input events into a queue which
        // can be read by the SDL input system right afterwards. So the order
        // here matters.
        fsemu_window_work(0);
        // But note, right now, fsemu_input_work is a no-op. Everything is
        // dispatched from fsemu_window_work.
        fsemu_input_work(0);

        fsemu_video_work(1000);
        if (fsemu_video_ready()) {
            // fsemu_video_render();
            SDL_assert(false); // render disabled
            fsemu_video_render_gui(snapshot);
            fsemu_gui_free_snapshot(snapshot);
            snapshot = NULL;

            fsemu_video_display();

            // new_host_frame = true;
            break;
        }
    }
}

static void fsemu_main_handle_events_until_next_frame_threaded(void)
{
    static int64_t old_frame = 0;

    while (true) {
        // FIXME: Rework event handling to wait for events instead

        // The SDL window subsystem will put input events into a queue which
        // can be read by the SDL input system right afterwards. So the order
        // here matters.
        fsemu_window_work(-1);
        // But note, right now, fsemu_input_work is a no-op. Everything is
        // dispatched from fsemu_window_work.
        fsemu_input_work(0);

        // Handle via custom event to the SDL event queue instead
        int64_t frame = fsemu_frame_counter();
        if (frame != old_frame) {
            old_frame = frame;
            return;
        }

        // Sleeping a bit here fixes video stuttering on Linux/X11. Maybe this
        // is caused by CPU starvation or something. Anyway, will be fixed
        // better when waiting for events is implemented.
        // Fixed via timeout fo fsemu_window_work
        // fsemu_time_sleep_ms(1);
    }
}

void fsemu_main_handle_events_until_next_frame(void)
{
    if (fsemu_video_is_threaded()) {
        fsemu_main_handle_events_until_next_frame_threaded();
    } else {
        fsemu_main_handle_events_until_next_frame_in_main();
    }
}

// ----------------------------------------------------------------------------
/*
void fsemu_main_init(void)
{
    fsemu_init_once(&fsemu_main.initialized);
    fsemu_log("[MAIN ] Initializing main module\n");
}
*/

#endif
