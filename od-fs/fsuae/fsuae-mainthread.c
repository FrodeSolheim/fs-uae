#include "fsuae-mainthread.h"

#include <Python.h>
#include <SDL3/SDL_opengl.h>
#include <glib.h>
#include <glib/genviron.h>
#include <glib/gstdio.h>

#include "fsapp-channel.h"
#include "fsapp-main.h"
#include "fsapp-mainloop.h"
#include "fsapp-pythonthread.h"
#include "fsapp-surface.h"
#include "fsapp.h"
#include "fsemu-input.h"
#include "fsemu-mainloop.h"
#include "fsemu-sdlinput.h"
#include "fsemu-sdlwindow.h"
#include "fsemu-thread.h"
#include "fsgui-windows.h"
#include "fslib-path.h"
#include "fsuae-bridge.h"
#include "fsuae-extras.h"
#include "fsuae-init.h"
#include "fsuae-messages.h"
#include "fsuae-path.h"
#include "fsuae-plugins.h"
#include "fsuae-renderer.h"

#define FSEMU_INTERNAL 1
#include "fsemu-glvideo.h"
#include "fsemu-video.h"

// FIXME: Rename to "fsapp-mainthread" or "fsemu-mainthread"??

// FIXME: static?
bool g_fsuae_main_quit = false;
volatile bool g_fsemu_video_initialized = false;

static void set_swap_interval(int interval) {
    static int old_interval = -999;  // "unset"
    if (interval != old_interval) {
        SDL_GL_SetSwapInterval(interval);
        old_interval = interval;
    }
}

static void handle_pending_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        fsemu_mainloop_handle_event(&event);
        // Or should we let fsemu give the event to fsapp?

        fsapp_mainloop_handle_event(&event);
    }
}

static void initialize_fsemu_video() {
    // FIXME: Move somewhere else...
    if (!g_fsemu_video_initialized) {
        // fsemu_sdlvideo_hack(g_renderer);
        // fsemu_sdlvideo_init();
        fsemu_sdlwindow_hack(g_window);
        // Must (currently) be called before fsemu_video_init
        fsemu_video_decide_driver();
        // fsemu_video_set_renderer(FSEMU_VIDEO_RENDERER_SDL);
        fsemu_video_set_renderer(FSEMU_VIDEO_RENDERER_OPENGL);
        fsemu_video_init();
        fsemu_glvideo_set_size_2(g_window_width, g_window_height);

        g_fsemu_video_initialized = true;
    }
}

static void handle_main_channel_messages(void) {
    SDL_assert(g_fsapp_main_channel != NULL);

    int type;
    const char* data;
    while (fsapp_channel_next_message(g_fsapp_main_channel, &type, &data)) {
        printf("[MAIN] Received message type %d\n", type);

        switch (type) {
            // FIXME: FSEMU_ message? Move to fsemu?
            case FSAPP_MESSAGE_INIT_SDL_GAMEPAD:
#if 0
            // long strtol(data, NULL, 10)
                int instance_id = atoi(data);

                SDL_Gamepad* gamepad = SDL_OpenGamepad(instance_id);
                if (gamepad != NULL) {
                    SDL_Log("Opened SDL gamepad instance ID %d\n", instance_id);
                    fsemu_sdlinput_add_controller(gamepad);
                } else {
                    SDL_LogWarn(
                        SDL_LOG_CATEGORY_APPLICATION,
                        "Could not open SDL gamepad %d (maybe it has already been disconnected)",
                        instance_id);
                }

                // exit(1);
#endif
                break;

            case FSEMU_MESSAGE_SET_PORT_DEVICE:
                int port_instance_id, device_type, device_instance_id;
                // printf("About to sscanf '%s'\n", data);
                int result =
                    sscanf(data, "%d:%d:%d", &port_instance_id, &device_type, &device_instance_id);
                // printf("result: %d\n", result);
                SDL_assert(result == 3);

                fsemu_inputport_t* port = fsemu_input_find_port_by_instance_id(port_instance_id);
                if (port == NULL) {
                    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Did not find input port");
                    break;
                }

                printf("\nFIND DEVICE: %d:%d\n", device_type, device_instance_id);

                fsemu_inputdevice_t* device = fsemu_input_find_device_by_type_and_instance_id(
                    device_type, device_instance_id);
                if (device == NULL) {
                    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Did not find input device");
                    break;
                }

                fsemu_inputport_set_device(port, device);

                break;
        }
    }
}

static void handle_mouse_grab(void) {
    // Assuming a single main window for now (will probably be true for a looong time)

    fsgui_window_t* window = (fsgui_window_t*)g_fsgui_windows_list->window;

    bool desired_state;
    if (g_fsapp_force_ui_cursor) {
        desired_state = !g_fsapp_force_ui_cursor;
    } else {
        desired_state = g_fsapp_mouse_grab;
    }

    static float last_mouse_x, last_mouse_y;

    bool existing_state = SDL_GetWindowRelativeMouseMode(g_window);
    if (desired_state != existing_state) {
        // g_window->relative_mouse = desired_state;
        SDL_Log("Set mouse grab = %d", desired_state);

        if (desired_state) {
            SDL_GetMouseState(&last_mouse_x, &last_mouse_y);
            SDL_Log("Storing mouse coordinates %f,%f", last_mouse_x, last_mouse_y);
        } else {
            // When ungrabbing in fullscreen, it is possible that the mouse pointer is at the top
            // of the screen, and the UI might briefly see a y 0 position, causing e.g. title
            // bar to flash before the mouse pointer is moved by SDL.
            SDL_Log("Restoring mouse coordinates %f,%f", last_mouse_x, last_mouse_y);
            // A hack seems to be to manually warp the mouse cursor to e.g. the center.
            // SDL_WarpMouseInWindow(g_window, g_window_width / 2, g_window_height / 2);
            float warp_to_x = last_mouse_x;
            float warp_to_y = last_mouse_y;
            if (warp_to_x < 0 || warp_to_x >= g_window_width || warp_to_y < 0 ||
                warp_to_y >= g_window_height) {
                SDL_Log("Mouse coordinates out of bounds, centering instead");
                warp_to_x = g_window_width / 2;
                warp_to_y = g_window_height / 2;
            }
            SDL_WarpMouseInWindow(g_window, warp_to_x, warp_to_y);
        }

        SDL_SetWindowRelativeMouseMode(g_window, desired_state);
        window->relative_mouse = desired_state;
    }
}

static void fsuae_mainloop_iteration(void) {
    static bool first_render = true;

    // Handle pending events

    handle_pending_events();

    // If no window yet, abort loop and try again (short delay to avoid excessive CPU usage)

    if (g_window == NULL) {
        SDL_Delay(1);
        return;
    }

    // Window and OpenGL is initialized, now it is time to initialize FSEMU the first time

    if (!g_fsemu_video_initialized) {
        initialize_fsemu_video();
    }

    fsemu_mainloop_iteration();
    fsapp_mainloop_iteration();

    // We could have used the SDL event queue to pass these messages, but an advantage of using a
    // separate fsapp_channel for this is that we can limit processing these types of
    // events/messages to this particular spot in the main loop. -Especially nice to be able to
    // avoid doing heavy custom event process during video wait loop.

    handle_main_channel_messages();

    // Decide if mouse cursor should be grabbed or not...

    handle_mouse_grab();

    // Wayland/Linux only sees one mouse?
    // int count;
    // SDL_GetMice(&count);
    // printf("%d\n", count);

    // fsgui_window_t* window = (fsgui_window_t*)g_windows->data;
    // window->relative_mouse = !window->relative_mouse;

    // We want python surfaces to be ready for rendering for the very first render, to avoid
    // flickering, so we handle first render a bit differently (actually wait)!

    // printf("main lock!\n");
    if (first_render) {
        SDL_Log("First render, wait on UI this time!");
        // FIXME: Somehow handle here or otherwise what happens if the UI thread never becomes
        // ready???
        SDL_LockMutex(g_python_mutex);
        // printf("main %d\n", g_python_ready);
        while (!g_python_ready) {
            SDL_UnlockMutex(g_python_mutex);
            SDL_Delay(1);
            SDL_LockMutex(g_python_mutex);
            // printf("main %d\n", g_python_ready);
        }
        first_render = false;
    } else {
        SDL_LockMutex(g_python_mutex);
    }
    // printf("main OK!\n");

    // Clear OpenGL buffers (let OpenGL do that in the background?) while we go on...

    fsuae_renderer_clear_window();

    // Create/update textures from surfaces

    if (g_python_ready) {
        fsuae_renderer_update_textures();
    } else {
        // Textures not ready in time, skipping - FIXME: Debug log?
    }

    // When we now signal the python thread to continue, the UI code will be interested in getting
    // information about window state, keys pressed, etc

    g_python_ready = false;
    // printf("main signal!\n");
    SDL_SignalCondition(g_python_condition);
    SDL_UnlockMutex(g_python_mutex);
    // printf("main unlocked!\n");

    // Render solid surfaces to screen - front to back, with z values

    fsuae_renderer_render_textures();

    // Process emulation video updates

    while (!fsemu_video_ready()) {
        handle_pending_events();
        fsemu_video_work(1000);
    }

    // Render emulator video to screen (with correct z value determined earlier)

    fsuae_renderer_render_video();

    // Render transparent surfaces (shadows)

    // Swap!

    glFinish();  // FIXME: Why is glFinish needed here?
    SDL_GL_SwapWindow(g_window);
}

void fsuae_mainthread_run(void) {
    while (!g_fsuae_main_quit) {
        fsuae_mainloop_iteration();
    }
}
