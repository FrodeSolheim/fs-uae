#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "inputdevice.h"
#include "custom.h"
#include "xwin.h"
#include "gui.h"
#include "moduleripper.h"
#include "uae/fs.h"

int tablet_log = 0;

static int g_joystick_port_autofire[4];

void amiga_set_joystick_port_autofire(int port, int autofire) {
    if (port < 0 || port > 3) {
        return;
    }
    g_joystick_port_autofire[port] = (autofire > 0);
}

static int handle_custom_action(int action, int state)
{
    write_log("handle_custom_action %d\n", action);
    if (action >= INPUTEVENT_SPC_DISKSWAPPER_0_0 &&
            action <= INPUTEVENT_SPC_DISKSWAPPER_3_19) {
        if (state) {
            int offset = action - INPUTEVENT_SPC_DISKSWAPPER_0_0;
            int drive = offset / AMIGA_FLOPPY_LIST_SIZE;
            int entry = offset % AMIGA_FLOPPY_LIST_SIZE;
            //disk_swap(drive, entry);
            amiga_floppy_set_from_list(drive, entry);
        }
        return 1;
    }

    if (action == INPUTEVENT_UAE_MODULE_RIPPER) {
        if (state) {
            moduleripper();
        }
        return 1;
    }

    /*
    if (state && action >= INPUTEVENT_AMIGA_JOYPORT_0_AUTOFIRE &&
            action <= INPUTEVENT_AMIGA_JOYPORT_3_AUTOFIRE) {
        int port = action - INPUTEVENT_AMIGA_JOYPORT_0_AUTOFIRE;
        g_joystick_port_autofire[port] = !g_joystick_port_autofire[port];
    }
    */

    return 0;
}

#include <fs/emu/hacks.h>

extern "C" {

int amiga_send_input_event(int input_event, int state) {
//#ifdef DEBUG_SYNC
    write_sync_log("apply action %d state=%d\n", input_event, state);
//#endif
    static int initialized = 0;
    if (!initialized) {
        initialized = 1;
    }

    if (g_fs_log_input) {
        write_log("amiga_send_input_event %d %d\n", input_event, state);
    }

    if (input_event > INPUTEVENT_PRIVATE_START) {
        return handle_custom_action(input_event, state);
    }
    // FIXME: is max = 1 always appropriate?
    int max = 1;
    bool magic_mouse = currprefs.input_magic_mouse;
    if (uae_deterministic_mode()) {
        magic_mouse = false;
    }

    switch (input_event) {
    case INPUTEVENT_MOUSE1_HORIZ:
    case INPUTEVENT_MOUSE1_VERT:
        //printf("magic mouse %d\n", currprefs.input_magic_mouse);
        if (magic_mouse) {
            //printf("magic mouse %d %d\n",
            //       fs_emu_mouse_absolute_x, fs_emu_mouse_absolute_y);
            uae_mousehack_helper(fs_emu_mouse_absolute_x,
                                 fs_emu_mouse_absolute_y);
            return 1;
        }
    case INPUTEVENT_MOUSE1_WHEEL:
    case INPUTEVENT_MOUSE2_HORIZ:
    case INPUTEVENT_MOUSE2_VERT:
        max = 0;
        break;
    case INPUTEVENT_KEY_CAPS_LOCK:
        // handled specially because of toggle mode
        // keyboard 0, using input event as scan code (correctly mapped
        // in keymap.cpp)
        inputdevice_translatekeycode(0, INPUTEVENT_KEY_CAPS_LOCK, state);
        return 1;
    }
    int autofire = 0;

    if (input_event == INPUTEVENT_JOY1_FIRE_BUTTON &&
            g_joystick_port_autofire[0]) {
        autofire = 1;
    }
    else if (input_event == INPUTEVENT_JOY2_FIRE_BUTTON &&
            g_joystick_port_autofire[1]) {
        autofire = 1;
    }
    else if (input_event == INPUTEVENT_PAR_JOY1_FIRE_BUTTON &&
            g_joystick_port_autofire[2]) {
        autofire = 1;
    }
    else if (input_event == INPUTEVENT_PAR_JOY2_FIRE_BUTTON &&
            g_joystick_port_autofire[3]) {
        autofire = 1;
    }

    bool canstopplayback = 1;
    bool playbackevent = 0;

    //amiga_configure_port_from_input_event(input_event);
    int result = amiga_handle_input_event (input_event, state, max, autofire,
            canstopplayback, playbackevent);
    if (result != 1) {
        write_log("amiga_handle_input_event(%d, %d, ...) failed with "
                "result %d", input_event, state, result);
    }
    return result;
}

} // extern "C"

bool handle_events (void) {
#ifdef DEBUG_SYNC
    static int count = 0;
    write_sync_log(" -- event handler for frame -- frame=%d\n", count);
    count++;
#endif

    if (g_libamiga_callbacks.event) {
        g_libamiga_callbacks.event(-1);
    }

    //frame_wait_for_filesys();
    //filesys_handle_events();
    return false;
}

int input_get_default_lightpen (struct uae_input_device *uid, int i, int port, int af, bool gp, bool joymouseswap) {
    return 0;
}

void gui_gameport_axis_change (int port, int axis, int state, int max) {

}

void gui_gameport_button_change (int port, int button, int onoff) {

}

#ifdef WINDOWS
#include <Windows.h>
#endif

int handle_msgpump (void) {
    if (g_libamiga_callbacks.event) {
        // g_libamiga_callbacks.event(hsync_counter);
        g_libamiga_callbacks.event(vpos);
    }
#ifdef WINDOWS
    // this message queue is used to dispatch messages for bsdsocket emulation
    // on Windows. Socket functions are run asynchronously with results posted
    // to a dummy Window.
	int got = 0;
	MSG msg;

	while (PeekMessage (&msg, 0, 0, 0, PM_REMOVE)) {
		got = 1;
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}
	return got;
#else
    return 0;
#endif
}

int is_tablet (void) {
    return 0;
}

bool target_can_autoswitchdevice(void)
{
    return false;
}
