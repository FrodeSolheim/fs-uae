#define FSEMU_INTERNAL
#include "fsemu/fsemu-mouse.h"

#include "fsemu/fsemu-option.h"
#include "fsemu/fsemu-thread.h"
#include "fsemu/fsemu-titlebar.h"

// ---------------------------------------------------------------------------

static struct fsemu_mouse {
    // True if the mouse cursor is (or will be) captured.
    bool captured;
    // True if emulation should grab the mouse on startup
    bool initial_grab;
    // True if emulation should grab the mouse on mouse click
    bool automatic_grab;
    // True if middle clicking should grab/ungrab input
    bool middle_click_ungrab;
} fsemu_mouse;

// ---------------------------------------------------------------------------

void fsemu_mouse_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_thread_init();
    fsemu_titlebar_init();

    fsemu_mouse_log("Initializing mouse subsystem\n");
#if 0
    fsemu_mouse.initial_grab = true;
    fsemu_config_read_bool(
        FSEMU_OPTION_AUTOMATIC_INPUT_GRAB, &fsemu_mouse.initial_grab);
    fsemu_mouse.automatic_grab = true;
    fsemu_config_read_bool(
        FSEMU_OPTION_AUTOMATIC_INPUT_GRAB, &fsemu_mouse.automatic_grab);
#endif

    fsemu_config_read_bool_default(
        FSEMU_OPTION_AUTOMATIC_INPUT_GRAB, &fsemu_mouse.automatic_grab, true);
    fsemu_config_read_bool_default(FSEMU_OPTION_INITIAL_INPUT_GRAB,
                                   &fsemu_mouse.initial_grab,
                                   fsemu_mouse.automatic_grab);
    fsemu_config_read_bool_default(FSEMU_OPTION_MIDDLE_CLICK_UNGRAB,
                                   &fsemu_mouse.middle_click_ungrab,
                                   true);

    fsemu_mouse_log("Initial grab:        %d\n", fsemu_mouse.initial_grab);
    fsemu_mouse_log("Automatic grab:      %d\n", fsemu_mouse.automatic_grab);
    fsemu_mouse_log("Middle-click ungrab: %d\n",
                    fsemu_mouse.middle_click_ungrab);

    // Set the desired state of mouse capture (to be handled by window update
    // or creation function
    fsemu_mouse.captured = fsemu_mouse.initial_grab;
}

// ---------------------------------------------------------------------------

bool fsemu_mouse_captured(void)
{
    return fsemu_mouse.captured;
}

// ---------------------------------------------------------------------------

void fsemu_mouse_set_captured(bool captured)
{
    fsemu_mouse.captured = captured;
}

// ---------------------------------------------------------------------------
#if 0
void fsemu_mouse_handle_position(int window_x, int window_y)
{
    fsemu_thread_assert_main();

    fsemu_mouse_log("%d,%d\n", window_x, window_y);
}

// ---------------------------------------------------------------------------

void fsemu_mouse_handle_click(int button,
                              int state,
                              int window_x,
                              int window_y)
{
    fsemu_thread_assert_main();

    fsemu_mouse_log("Button %d clicked (%d) at %d,%d\n",
                    button,
                    state,
                    window_x,
                    window_y);

    if (state) {
        if (button == 2) {
            fsemu_mouse_set_captured(!fsemu_mouse_captured());
        } else {
            if (fsemu_mouse.automatic_grab && !fsemu_mouse_captured()) {
                fsemu_mouse_set_captured(true);
            }
        }
    }
}
#endif
// ---------------------------------------------------------------------------

// void fsemu_mouse_handle_mouse(int x, int y, int button, int state)
bool fsemu_mouse_handle_mouse(fsemu_mouse_event_t *event)
{
    fsemu_thread_assert_main();

    if (fsemu_titlebar_handle_mouse(event)) {
        return true;
    }

    if (event->button >= 0) {
        fsemu_mouse_log("Button %d clicked (%d) at %d,%d\n",
                        event->button,
                        event->state,
                        event->x,
                        event->y);

        if (event->state) {
            if (event->button == 2) {
                fsemu_mouse_set_captured(!fsemu_mouse_captured());
            } else {
                if (fsemu_mouse.automatic_grab && !fsemu_mouse_captured()) {
                    fsemu_mouse_set_captured(true);
                }
            }
        }
    } else {
        fsemu_mouse_log("%d,%d\n", event->x, event->y);
#if 0
        if (y == 0) { // FIXME: CHECK GRAB?
            fsemu_titlebar_handle_mouse(x, y, button, state);
        }
#endif
    }

    return false;
}

// ---------------------------------------------------------------------------
