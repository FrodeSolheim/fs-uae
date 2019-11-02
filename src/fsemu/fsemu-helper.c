#define FSEMU_INTERNAL
#include "fsemu-helper.h"
#include "fsemu-hud.h"
#include "fsemu-perfgui.h"
#include "fsemu-startupinfo.h"
#include "fsemu-time.h"
#include "fsemu-titlebar.h"
#include "fsemu-video.h"
#include "fsemu-window.h"

// FIXME: Ideally, remove this dependency
#include "fsemu-sdlwindow.h"

void fsemu_helper_init_emulator(const char *emulator_name,
                                bool external_events,
                                int renderer,
                                int fullscreen,
                                int vsync)
{
    fsemu_set_emulator_name(emulator_name);
    fsemu_window_set_title(emulator_name);
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

    fsemu_window_init();
    fsemu_video_init();
    fsemu_titlebar_init();

    static fsemu_gui_item_t *gui;

    // This is somewhat strange, but it is important that we have a certain
    // delay here (either many frames rapidly, or fewer frames with longer
    // delays between them). If not, at least on Linux/X11 when tested - and
    // especially in fullscreen, nothing will be rendered on screen, and
    // flickering may appear (etc) especially if the emulation takes a good
    // while to get going (too long until the renderer starts again).

    int64_t t0 = fsemu_time_us();
    // for (int i = 0; i < 10; i++) {
    while (fsemu_time_us() - t0 < 100 * 1000) {
        // printf("%d\n", i);
        // glClearColor(0.0, 0.0, 0.0, 1.0);
        // glClear(GL_COLOR_BUFFER_BIT);
        // SDL_GL_SwapWindow(fsemu_sdlwindow_window());

        if (gui) {
            fsemu_video_render_gui(gui);
            fsemu_gui_free_snapshot(gui);
        }
        // fsemu_video_render();
        fsemu_video_display();
        // glFinish();
        // fsemu_perfgui_update();
        // fsemu_startupinfo_update();
        // fsemu_hud_update();
        fsemu_titlebar_update();
        gui = fsemu_gui_snapshot();
        fsemu_video_render_gui_early(gui);

        fsemu_window_work(0);

        fsemu_sleep_us(1 * 1000);

        // SDL_PumpEvents();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (fsemu_sdlwindow_handle_event(&event)) {
                printf("[FSEMU] Not passing on event to emulator\n");
                continue;
            }
        }
    }

    fsemu_perfgui_init();
    fsemu_startupinfo_init();
    fsemu_hud_init();
}
