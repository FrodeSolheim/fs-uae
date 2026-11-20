#include "fsuae-mainthread.h"

#include <Python.h>
#include <glib.h>
#include <glib/genviron.h>
#include <glib/gstdio.h>

#include "fsapp-channel.h"
#include "fsapp-frame.h"
#include "fsapp-main.h"
#include "fsapp-mainthread.h"
#include "fsapp.h"
#include "fsemu-input.h"
#include "fsemu-sdlinput.h"

// The program's part of the main loop (fsapp_mainthread_run): its frame hook (the main channel and
// the input grab) and the quit flag. fsemu registers its own hooks in fsemu_mainloop_init.

// Set from any thread (the emulation thread when the emulation ends, the main thread on a quit
// event) and read by the main loop and the emulation thread, so atomic.
static SDL_AtomicInt g_quit;

void fsuae_mainthread_quit(void)
{
    SDL_SetAtomicInt(&g_quit, 1);
    // The main loop looks at the flag when it runs, also while it idles (from any thread).
    fsapp_frame_wake();
}

bool fsuae_mainthread_is_quitting(void)
{
    return SDL_GetAtomicInt(&g_quit) != 0;
}

static void handle_main_channel_messages(void)
{
    SDL_assert(g_fsapp_main_channel != NULL);

    int type;
    const char *data;
    while (fsapp_channel_get_next_message(g_fsapp_main_channel, &type, &data)) {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "[MAIN] Received message type %d", type);

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

            fsemu_inputport_t *port = fsemu_input_find_port_by_instance_id(port_instance_id);
            if (port == NULL) {
                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Did not find input port");
                break;
            }

            SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Find device: %d:%d", device_type,
                         device_instance_id);

            fsemu_inputdevice_t *device =
                fsemu_input_find_device_by_type_and_instance_id(device_type, device_instance_id);
            if (device == NULL) {
                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Did not find input device");
                break;
            }

            fsemu_inputport_set_device(port, device);

            break;
        }
    }
}

static void handle_mouse_grab(void)
{
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

            // float warp_to_x = g_window_width / 2;
            // float warp_to_y = g_window_height / 2;
            // SDL_WarpMouseInWindow(g_window, warp_to_x, warp_to_y);

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
        fsapp_main_set_relative_mouse(desired_state);
    }
}

// Called by fsapp's main loop once per frame on the host window side, once the host window exists,
// after the events and fsemu's frame hook.
static void frame_hook(void)
{
    // We could have used the SDL event queue to pass these messages, but an advantage of using a
    // separate fsapp_channel for this is that we can limit processing these types of
    // events/messages to this particular spot in the main loop. -Especially nice to be able to
    // avoid doing heavy custom event process during video wait loop.

    handle_main_channel_messages();

    // Decide if mouse cursor should be grabbed or not...

    handle_mouse_grab();
}

static bool quit_requested(void)
{
    return fsuae_mainthread_is_quitting();
}

void fsuae_mainthread_run(void)
{
    // fsemu's hooks are registered by fsemu_mainloop_init (init_fsemu), so they come before this.
    fsapp_mainthread_add_frame_hook(frame_hook);
    fsapp_mainthread_run(quit_requested);
}
