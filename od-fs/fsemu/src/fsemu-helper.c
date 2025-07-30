#define FSEMU_INTERNAL
#include "fsemu-helper.h"

#include "fsemu-action.h"
#include "fsemu-application.h"
#include "fsemu-background.h"
#include "fsemu-controller.h"
#include "fsemu-fade.h"
#include "fsemu-frame.h"
#include "fsemu-gamemode.h"
#include "fsemu-hud.h"
#include "fsemu-input.h"
#include "fsemu-leds.h"
#include "fsemu-log.h"
#include "fsemu-option.h"
#include "fsemu-oskeyboard.h"
#include "fsemu-osmenu.h"
#include "fsemu-perfgui.h"
#include "fsemu.h"
#include "fsemu-screenshot.h"
// FIXME: Ideally, remove this dependency
#include "fsemu-sdlwindow.h"
#include "fsemu-startupinfo.h"
#include "fsemu-theme.h"
#include "fsemu-thread.h"
#include "fsemu-time.h"
#include "fsemu-titlebar.h"
#include "fsemu-video.h"
#include "fsemu-videothread.h"
#include "fsemu-window.h"

static struct {
    fsemu_gui_item_t *gui;
} fsemu_helper;

void fsemu_helper_init_emulator(const char *emulator_name,
                                bool external_events,
                                int renderer,
                                int fullscreen,
                                int vsync)
{
    fsemu_boot_log("fsemu_init_with_args");
    fsemu_log("[FSE] [HLP] Init emulator vsync=%d\n", vsync);

    // Make sure warnings can be safely logged and scheduled for later
    // displaying via an async queue.
    fsemu_hud_init_early();

    fsemu_warning_2("Early development preview",
                    "Some features are not fully developed");

    fsemu_boot_log("before fsemu_thread_init");
    // This call will also register the main thread
    fsemu_thread_init();
    // Register main thread as video thread also
    fsemu_thread_set_video();

    fsemu_boot_log("before fsemu_log_setup");
    fsemu_log_setup();

    fsemu_boot_log("before fsemu_option_init");
    fsemu_option_init();

    // fsemu_set_emulator_name(emulator_name);
    const char *env_title = fsemu_read_env_option("WINDOW_TITLE");
    // printf("%s\n", env_title);
    // exit(1);
    if (env_title && env_title[0]) {
        fsemu_window_set_title(env_title);
    } else {
        fsemu_window_set_title(emulator_name);
    }

    if (external_events) {
        fsemu_sdlwindow_set_no_event_polling(true);
    }
    fsemu_video_set_renderer(renderer);
    fsemu_startupinfo_set_emulator_name(emulator_name);
    // fsemu_startupinfo_set_emulator_fork_info("Mednafen");

    // FIXME: Review
    if (fullscreen == 1) {
        fsemu_window_set_fullscreen(true);
    }
    // FIXME: Review
    if (vsync == 1) {
        fsemu_video_set_vsync(1);
    }

    fsemu_boot_log("before fsemu_window_init");
    fsemu_window_init();
    fsemu_boot_log("before fsemu_video_init");
    fsemu_video_init();
    fsemu_boot_log("before fsemu_fade_init");
    fsemu_fade_init();
    fsemu_boot_log("before fsemu_titlebar_init");
    fsemu_titlebar_init();
    // Hmm, necessary?
    fsemu_boot_log("before fsemu_input_init");
    fsemu_input_init();

    // Now, open the window and render the decorations

    fsemu_boot_log("before fsemu_helper_startup_loop");
    fsemu_helper_startup_loop();

    // Continue with initialization

    fsemu_application_init();
    fsemu_screenshot_init();

    fsemu_boot_log("before fsemu_action_init");
    fsemu_action_init();
    fsemu_boot_log("before fsemu_background_init");
    fsemu_background_init();

    fsemu_boot_log("before fsemu_hud_init");
    fsemu_hud_init();

    // FIXME: Postpone this until after the window is shown?
    fsemu_boot_log("before fsemu_gamemode_init");
    fsemu_gamemode_init();
    // FIXME: Check (on Linux) if CPU governor is now set to performance

    fsemu_boot_log("before fsemu_leds_init");
    fsemu_leds_init();
    fsemu_boot_log("before fsemu_oskeyboard_init");
    fsemu_oskeyboard_init();
    fsemu_boot_log("before fsemu_osmenu_init");
    fsemu_osmenu_init();
    fsemu_boot_log("before fsemu_perfgui_init");
    fsemu_perfgui_init();
    fsemu_boot_log("before fsemu_startupinfo_init");
    fsemu_startupinfo_init();
    fsemu_boot_log("before fsemu_theme_module_init");
    fsemu_theme_module_init();

    // FIXME: Maybe temporary
    fsemu_boot_log("before fsemu_controller_init");
    fsemu_controller_init();
}

static void fsemu_helper_startup_update(void)
{
    // fsemu_perfgui_update();
    // fsemu_startupinfo_update();
    // fsemu_hud_update();

    fsemu_titlebar_update();
    // We do not want to start the "fade timer", so we do not run the update
    // function from the startup part of the helper.
    // fsemu_fade_update();
}

static void fsemu_helper_poll_and_sleep(void)
{
    fsemu_window_work(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (fsemu_sdlwindow_handle_event(&event)) {
            // printf("[FSE] Not passing on event to emulator\n");
            continue;
        }
    }
    fsemu_sleep_us(1 * 1000);
    // fsemu_sleep_us(1000 * 1000);
}

void fsemu_helper_startup_loop(void)
{
    int64_t t0;
    int64_t duration_us;

    // if (fsemu_video_is_threaded()) {
    //     printf("VIDEO IS THREADED, SKIPPING fsemu_helper_startup_loop\n");
    //     return;
    // }

    // fsemu_fade_force(true);

    // When opening directly to fullscreen, we run the rendering loop
    // (rendering only a black screen) to avoid flickering. For example,
    // transitioning to unredirected fullscreen and/or enabling G-SYNC
    // mode can cause some occasional flickering otherwise.
    if (fsemu_window_fullscreen()) {
        // fsemu_fade_set_color(FSEMU_RGB(0x000000));
        duration_us = 500 * 1000;
#if 0
        fsemu_log("[FSE] Entering fullscreen clear loop");
        t0 = fsemu_time_us();
        // The fade function also renders to full black for the first frames
        // to avoid flickering from the emulated machine booting. So we don't
        // need to overdo the delay here...
        while (fsemu_time_us() - t0 < 500 * 1000) {
            // FIXME: Clear to black
            fsemu_video_display();
            fsemu_helper_poll_and_sleep();
        }
        fsemu_log("[FSE] Leave fullscreen clear loop");
#endif
    } else {
        // fsemu_fade_set_color(FSEMU_RGB(0x808080));
        // fsemu_fade_set_color(fsemu_titlebar_background_color());

        // FIXME: Get bezel background color?
        fsemu_fade_set_color(FSEMU_RGB(0x0c0c0c));
        duration_us = 150 * 1000;
    }

    // Create initial GUI for the first rendered frame
    fsemu_helper_startup_update();
    // fsemu_helper.gui = fsemu_gui_snapshot();
    // fsemu_video_render_gui_early(fsemu_helper.gui);
    fsemu_helper_set_gui(fsemu_gui_snapshot());
    fsemu_video_render_gui_early(fsemu_helper_gui());

#if 0
    if (fsemu_window_fullscreen()) {
        fsemu_log("[FSE] Leave fullscreen clear loop");
        return;
    }
#endif

    // This is somewhat strange, but it is important that we have a certain
    // delay here (either many frames rapidly, or fewer frames with longer
    // delays between them). If not, at least on Linux/X11 when tested - and
    // especially in fullscreen, nothing will be rendered on screen, and
    // flickering may appear (etc) especially if the emulation takes a good
    // while to get going (too long until the renderer starts again).

    t0 = fsemu_time_us();
    // for (int i = 0; i < 10; i++) {

#if 0
    (void) t0;
    (void) duration_us;
#else
    // Disabled due to v-sync testing

    while (fsemu_time_us() - t0 < duration_us) {
        // if (fsemu_helper.gui) {
        fsemu_video_render_gui(fsemu_helper.gui);
        fsemu_gui_free_snapshot(fsemu_helper.gui);
        // }
        // fsemu_video_render();
        // fsemu_video_set_did_render();
        // fsemu_video_display();
        fsemu_video_force_display();

        fsemu_helper_startup_update();
        // fsemu_helper.gui = fsemu_gui_snapshot();
        // fsemu_video_render_gui_early(fsemu_helper.gui);
        fsemu_helper_set_gui(fsemu_gui_snapshot());
        fsemu_video_render_gui_early(fsemu_helper_gui());

        fsemu_helper_poll_and_sleep();
    }

    // Resetting these to avoid confusion the frame timing system
    fsemu_frame_number_rendering = -1;
    fsemu_frame_number_rendered = -1;
    fsemu_frame_number_displaying = -1;
    fsemu_frame_number_displayed = -1;

#endif
    // FIXME: Should not be necessary to get / render early gui from emulators
    // since this is done here!

    // fsemu_fade_force(false);

    // FIXME:
    // if (fsemu_video_is_threaded()) {
    //     fsemu_videothread_start();
    // }
    fsemu_video_set_startup_done_mt();
}

// FIXME: Remove: deprecated
fsemu_gui_item_t *fsemu_helper_startup_gui(void)
{
    return fsemu_helper.gui;
}

fsemu_gui_item_t *fsemu_helper_gui(void)
{
    return fsemu_helper.gui;
}

void fsemu_helper_set_gui(fsemu_gui_item_t *gui)
{
#if 0
    if (fsemu_helper.gui) {
        free
    }
#endif
    fsemu_helper.gui = gui;
}

void fsemu_helper_sleep_display_end_start(double hz)
{
    int64_t now = fsemu_time_wait_until_us(fsemu_frame_end_at);
    fsemu_frame_add_sleep_time(now);

    fsemu_video_display();
    // fsemu_video_set_frame_displayed_at(current_frame_no, fsemu_time_us());

    // printf("WARNING WARNING WARNING: REMEMBER TO REMOVE SLEEP\n");
    // fsemu_sleep_us(g_random_int_range(1000, 2000));

    fsemu_frame_end();

    fsemu_frame_start(hz);

#ifdef FSEMU_SAMPLERATE
    // FIXME: Move?
    fsemu_audiobuffer_set_adjustment(fsemu_audiobuffer_calculate_adjustment());
#endif

    fsemu_frame_log_epoch("Frame begin\n");
}

void fsemu_helper_render_sleep_display_end_start(double hz)
{
    // printf("fsemu_helper_render_sleep_display_end_start\n");
    fsemu_video_render();
    fsemu_video_render_gui(fsemu_helper_gui());
    fsemu_gui_free_snapshot(fsemu_helper_gui());
    fsemu_helper_set_gui(NULL);
    fsemu_frame_log_epoch("Rendered\n");

    // Add time spent uploading, rendering and rendering the UI
    // (FIXME: Should we glFinish, or similar here?)
    // FIXME: At least glFlush?

    fsemu_frame_add_render_time(0);

    static int current_frame_no = 0;
    fsemu_video_set_frame_rendered_at(current_frame_no, fsemu_time_us());
    current_frame_no += 1;

    fsemu_helper_sleep_display_end_start(hz);
}

void fsemu_helper_update(void)
{
    fsemu_frame_log_epoch("Update (helper)\n");
    fsemu_fade_update();
    fsemu_hud_update();
    fsemu_oskeyboard_update();
    fsemu_osmenu_update();
    fsemu_perfgui_update();
    fsemu_startupinfo_update();
    fsemu_titlebar_update();

    // FIXME: Maybe temporary
    fsemu_controller_update();

    fsemu_helper_set_gui(fsemu_gui_snapshot());
    fsemu_frame_add_gui_time(0);
}

void fsemu_helper_update_render_early(void)
{
    fsemu_helper_update();

    fsemu_video_render_gui_early(fsemu_helper_gui());
    fsemu_frame_add_render_time(0);
}

void fsemu_helper_framewait(void)
{
    int64_t now = fsemu_time_us();
    int dt = fsemu_frame_begin_at - now;
    fsemu_frame_log_epoch("Wait %d us until emulation begins\n", dt);
    if (dt > 0) {
        // fsemu_frame_log_epoch("Wait %d us until emulation begins", dt);
        fsemu_time_wait_until_us_2(fsemu_frame_begin_at, now);
        fsemu_frame_add_framewait_time(0);
    } else {
        // fsemu_frame_log_epoch("Wait 0 us until emulation begins");
    }
}
