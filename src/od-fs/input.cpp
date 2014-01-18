#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "inputdevice.h"
#include "custom.h"

#if 0
int sdl_key_to_dik (int keycode);
#endif

//static void disk_swap(int drive, int entry) {
//  amiga_floppy_set_frm
//}

int tablet_log = 0;

static int g_joystick_port_autofire[4];

void amiga_set_joystick_port_autofire(int port, int autofire) {
    if (port < 0 || port > 3) {
        return;
    }
    g_joystick_port_autofire[port] = (autofire > 0);
}

static int handle_custom_action(int action, int state) {
    write_log("handle_custom_action %d\n", action);
    if (action >= INPUTEVENT_SPC_DISKSWAPPER_0_0 &&
            action <= INPUTEVENT_SPC_DISKSWAPPER_3_19) {
        int offset = action - INPUTEVENT_SPC_DISKSWAPPER_0_0;
        int drive = offset / AMIGA_FLOPPY_LIST_SIZE;
        int entry = offset % AMIGA_FLOPPY_LIST_SIZE;
        //disk_swap(drive, entry);
        amiga_floppy_set_from_list(drive, entry);
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

#define MAX_MICE 2
#define MAX_MICE_AXES 2

extern "C" {

int amiga_send_input_event(int input_event, int state) {
//#ifdef DEBUG_SYNC
    write_sync_log("apply action %d state=%d\n", input_event, state);
//#endif

    //printf("amiga_send_input_event %d %d\n", input_event, state);
    int isabs = 0;

    if (input_event > INPUTEVENT_PRIVATE_START) {
        return handle_custom_action(input_event, state);
    }
    // FIXME: is max = 1 always appropriate?
    int max = 1;

    switch (input_event) {
    case INPUTEVENT_MOUSE1_HORIZ:
    case INPUTEVENT_MOUSE1_VERT:
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

extern void uae_quit (void);

void setid (struct uae_input_device *uid, int i, int slot, int sub, int port, int evt)
{
        uid[i].eventid[slot][sub] = evt;
        uid[i].port[slot][sub] = port + 1;
}

void setid_af (struct uae_input_device *uid, int i, int slot, int sub, int port, int evt, int af)
{
        setid (uid, i, slot, sub, port, evt);
        uid[i].flags[slot][sub] &= ~(ID_FLAG_AUTOFIRE | ID_FLAG_TOGGLE);
        if (af >= JPORT_AF_NORMAL)
                uid[i].flags[slot][sub] |= ID_FLAG_AUTOFIRE;
        if (af == JPORT_AF_TOGGLE)
                uid[i].flags[slot][sub] |= ID_FLAG_TOGGLE;
}

/*
 * Default inputdevice config for SDL mouse
 */
int input_get_default_mouse (struct uae_input_device *uid, int i, int port, int af, bool gp, bool wheel)
//int input_get_default_mouse (struct uae_input_device *uid, int i, int port, int af, bool gp)
{
    write_log("input_get_default_mouse\n");
    /* SDL supports only one mouse */
    setid (uid, i, ID_AXIS_OFFSET + 0, 0, port, port ? INPUTEVENT_MOUSE2_HORIZ : INPUTEVENT_MOUSE1_HORIZ);
    setid (uid, i, ID_AXIS_OFFSET + 1, 0, port, port ? INPUTEVENT_MOUSE2_VERT : INPUTEVENT_MOUSE1_VERT);
    setid (uid, i, ID_AXIS_OFFSET + 2, 0, port, port ? 0 : INPUTEVENT_MOUSE1_WHEEL);
    setid_af (uid, i, ID_BUTTON_OFFSET + 0, 0, port, port ? INPUTEVENT_JOY2_FIRE_BUTTON : INPUTEVENT_JOY1_FIRE_BUTTON, af);
    setid (uid, i, ID_BUTTON_OFFSET + 1, 0, port, port ? INPUTEVENT_JOY2_2ND_BUTTON : INPUTEVENT_JOY1_2ND_BUTTON);
    setid (uid, i, ID_BUTTON_OFFSET + 2, 0, port, port ? INPUTEVENT_JOY2_3RD_BUTTON : INPUTEVENT_JOY1_3RD_BUTTON);
    if (port == 0) { /* map back and forward to ALT+LCUR and ALT+RCUR */
//                if (isrealbutton (did, 3)) {
                        setid (uid, i, ID_BUTTON_OFFSET + 3, 0, port, INPUTEVENT_KEY_ALT_LEFT);
                        setid (uid, i, ID_BUTTON_OFFSET + 3, 1, port, INPUTEVENT_KEY_CURSOR_LEFT);
//                        if (isrealbutton (did, 4)) {
                                setid (uid, i, ID_BUTTON_OFFSET + 4, 0, port, INPUTEVENT_KEY_ALT_LEFT);
                                setid (uid, i, ID_BUTTON_OFFSET + 4, 1, port, INPUTEVENT_KEY_CURSOR_RIGHT);
//                        }
//                }
    }
    if (i == 0)
        return 1;
    return 0;
}

void handle_events (void) {
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
}

void toggle_mousegrab (void) {
    STUB("");
}

void setmouseactivexy (int x, int y, int dir) {
    STUB("");
}

void setmouseactive (int active) {
    STUB("");
}

static int get_mouse_num (void) {
    return 2;
}

static TCHAR *get_mouse_friendlyname (int mouse) {
    //VERBOSE_STUB("(possible memory leak here)");
    if (mouse == 0) {
        return (char*) "Mouse 0";
    }
    else if (mouse == 1) {
        return (char*) "Mouse 1";
    }
    else {
        return (char*) "Mouse";
    }
}

static TCHAR *get_mouse_uniquename (int mouse) {
    //VERBOSE_STUB("(possible memory leak here)");
    if (mouse == 0) {
        return (char*) "MOUSE0";
    }
    else if (mouse == 1) {
        return (char*) "MOUSE1";
    }
    else {
        return (char*) "MOUSE";
    }
}

static int get_mouse_widget_num (int mouse) {
    return 0;
}

static int get_mouse_widget_first (int mouse, int type) {
    return 0;
}

static int get_mouse_widget_type (int mouse, int num, TCHAR *name, uae_u32 *code) {
    return 0;
}

static int init_mouse (void) {
    return 1;
}

static void close_mouse (void) {

}

static int acquire_mouse (int num, int flags) {
    STUB("num=%d flags=%d", num, flags);
    return 0;
}

static void unacquire_mouse (int num) {
    STUB("num=%d", num);
}

static void read_mouse (void) {

}

static int get_mouse_flags (int num) {
    return 1;
}

struct inputdevice_functions inputdevicefunc_mouse = {
    init_mouse, close_mouse, acquire_mouse, unacquire_mouse, read_mouse,
    get_mouse_num, get_mouse_friendlyname, get_mouse_uniquename,
    get_mouse_widget_num, get_mouse_widget_type,
    get_mouse_widget_first,
    get_mouse_flags
};


static int init_kb (void) {
    LOG_STUB("");
    return 1;
}

static void close_kb (void) {
    LOG_STUB("");
}

static int acquire_kb (int num, int flags) {
    STUB("");
    return 1;
}

static void unacquire_kb (int num) {
    STUB("");
}

static void read_kb (void) {

}

static int get_kb_num (void) {
    return 1;
}

static TCHAR *get_kb_friendlyname (int kb) {
    VERBOSE_STUB("(possible memory leak here)");
    return strdup("Default Keyboard");
}

static TCHAR *get_kb_uniquename (int kb) {
    VERBOSE_STUB("(possible memory leak here)");
    return strdup("KEYBOARD0");
}

static int get_kb_widget_num (int kb) {
    // FIXME: WHAT is this?
    return 255;
}

static int get_kb_widget_first (int kb, int type) {
    //STUB("");
    return 0;
}

static int get_kb_widget_type (int kb, int num, TCHAR *name, uae_u32 *code) {
    // FIXME: What is this?
    //*code = num;
    return IDEV_WIDGET_KEY;
}

static int get_kb_flags (int num) {
    return 0;
}

struct inputdevice_functions inputdevicefunc_keyboard = {
    init_kb, close_kb, acquire_kb, unacquire_kb, read_kb,
    get_kb_num, get_kb_friendlyname, get_kb_uniquename,
    get_kb_widget_num, get_kb_widget_type,
    get_kb_widget_first,
    get_kb_flags
};

int target_checkcapslock (int scancode, int *state) {
    return 0;
}

int getcapslockstate (void) {
    return 0;
}

void setcapslockstate (int state) {

}

int input_get_default_keyboard (int num) {
    if (num == 0) {
        return 1;
    }
    return 0;
}

int input_get_default_joystick_analog (struct uae_input_device *uid, int num, int port, int af, bool gp) {
    return 0;
}

int input_get_default_lightpen (struct uae_input_device *uid, int i, int port, int af, bool gp) {
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


/*
 * SDL Joystick code
 *
 * Copyright 1997 Bernd Schmidt
 * Copyright 1998 Krister Walfridsson
 * Copyright 2003-2005 Richard Drummond
 */
/*
#include <SDL.h>

#define MAX_MAPPINGS 256

static unsigned int nr_joysticks;
static int initialized;

struct joyinfo {
    SDL_Joystick *joy;
    unsigned int axles;
    unsigned int buttons;
};

static struct joyinfo joys[MAX_INPUT_DEVICES];
*/
/*
static void read_joy (unsigned int nr) {
    unsigned int num, i, axes, axis;
    SDL_Joystick *joy;

    if (currprefs.input_selected_setting == 0) {
        if (jsem_isjoy (0, &currprefs) != (int)nr && jsem_isjoy (1, &currprefs) != (int)nr)
            return;
    }
    joy = joys[nr].joy;
    axes = SDL_JoystickNumAxes (joy);
    for (i = 0; i < axes; i++) {
        axis = SDL_JoystickGetAxis (joy, i);
        setjoystickstate (nr, i, axis, 32767);
    }

    num = SDL_JoystickNumButtons (joy);
    for (i = 0; i < num; i++) {
        int bs = SDL_JoystickGetButton (joy, i) ? 1 : 0;
        setjoybuttonstate (nr, i, bs);
    }
}
*/

static int get_joystick_num (void) {
    return 4;
}

static int get_joystick_widget_num (int joy) {
    write_log("get_joystick_widget_num joy=%d\n", joy);
    return 0;
}

static int get_joystick_widget_type (int joy, int num, char *name,
        uae_u32 *code) {
    return IDEV_WIDGET_NONE;
}

static int get_joystick_widget_first (int joy, int type) {
    return -1;
}

static char *get_joystick_friendlyname (int joy) {
    if (joy == 0) {
        return (char*) "Joystick 1";
    }
    else if (joy == 1) {
        return (char*) "Joystick 2";
    }
    else if (joy == 2) {
        return (char*) "Joystick 3";
    }
    else if (joy == 3) {
        return (char*) "Joystick 4";
    }
    else {
        return (char*) "Joystick";
    }
}

static char *get_joystick_uniquename (int joy) {
    if (joy == 0) {
        return (char*) "JOYSTICK1";
    }
    else if (joy == 1) {
        return (char*) "JOYSTICK2";
    }
    else if (joy == 2) {
        return (char*) "JOYSTICK3";
    }
    else if (joy == 3) {
        return (char*) "JOYSTICK4";
    }
    else {
        return (char*) "JOYSTICK";
    }
}

static void read_joystick (void) {
    VERBOSE_STUB("");
}

static int init_joystick (void) {
    write_log("init_joystick\n");
    return 1;
}

static void close_joystick (void) {

}

static int acquire_joystick (int num, int flags)
{
    //return num < get_joystick_num ();
    return 1;
}

static void unacquire_joystick (int num)
{
}

static int get_joystick_flags (int num)
{
    return 0;
}

struct inputdevice_functions inputdevicefunc_joystick = {
    init_joystick,
    close_joystick,
    acquire_joystick,
    unacquire_joystick,
    read_joystick,
    get_joystick_num,
    get_joystick_friendlyname,
    get_joystick_uniquename,
    get_joystick_widget_num,
    get_joystick_widget_type,
    get_joystick_widget_first,
    get_joystick_flags
};

int input_get_default_joystick (struct uae_input_device *uid, int num, int port, int af, int mode, bool gp)
{
    int h,v;
    unsigned int j;
    struct didata *did;
    //SDL_Joystick *joy;
    //joy = joys[i].joy;

    write_log("input_get_default_joystick num=%d port=%d af=%d mode=%d\n", num, port, af, mode);

    /*
    if (i >= get_joystick_num ())
        return 0;
    */

    if (mode == JSEM_MODE_MOUSE_CDTV) {
        h = INPUTEVENT_MOUSE_CDTV_HORIZ;
        v = INPUTEVENT_MOUSE_CDTV_VERT;
    } else if (port >= 2) {
        h = port == 3 ? INPUTEVENT_PAR_JOY2_HORIZ : INPUTEVENT_PAR_JOY1_HORIZ;
        v = port == 3 ? INPUTEVENT_PAR_JOY2_VERT : INPUTEVENT_PAR_JOY1_VERT;
    } else {
        h = port ? INPUTEVENT_JOY2_HORIZ : INPUTEVENT_JOY1_HORIZ;;
        v = port ? INPUTEVENT_JOY2_VERT : INPUTEVENT_JOY1_VERT;
    }
    setid (uid, num, ID_AXIS_OFFSET + 0, 0, port, h);
    setid (uid, num, ID_AXIS_OFFSET + 1, 0, port, v);

    if (port >= 2) {
        setid_af (uid, num, ID_BUTTON_OFFSET + 0, 0, port, port == 3 ? INPUTEVENT_PAR_JOY2_FIRE_BUTTON : INPUTEVENT_PAR_JOY1_FIRE_BUTTON, af);
    } else {
        setid_af (uid, num, ID_BUTTON_OFFSET + 0, 0, port, port ? INPUTEVENT_JOY2_FIRE_BUTTON : INPUTEVENT_JOY1_FIRE_BUTTON, af);
        setid (uid, num, ID_BUTTON_OFFSET + 1, 0, port, port ? INPUTEVENT_JOY2_2ND_BUTTON : INPUTEVENT_JOY1_2ND_BUTTON);
        setid (uid, num, ID_BUTTON_OFFSET + 2, 0, port, port ? INPUTEVENT_JOY2_3RD_BUTTON : INPUTEVENT_JOY1_3RD_BUTTON);
    }

    if (mode == JSEM_MODE_JOYSTICK_CD32) {
        setid_af (uid, num, ID_BUTTON_OFFSET + 0, 0, port, port ? INPUTEVENT_JOY2_CD32_RED : INPUTEVENT_JOY1_CD32_RED, af);
        setid_af (uid, num, ID_BUTTON_OFFSET + 0, 1, port, port ? INPUTEVENT_JOY2_FIRE_BUTTON : INPUTEVENT_JOY1_FIRE_BUTTON, af);
        setid (uid, num, ID_BUTTON_OFFSET + 1, 0, port, port ? INPUTEVENT_JOY2_CD32_BLUE : INPUTEVENT_JOY1_CD32_BLUE);
        setid (uid, num, ID_BUTTON_OFFSET + 1, 1, port,  port ? INPUTEVENT_JOY2_2ND_BUTTON : INPUTEVENT_JOY1_2ND_BUTTON);
        setid (uid, num, ID_BUTTON_OFFSET + 2, 0, port, port ? INPUTEVENT_JOY2_CD32_GREEN : INPUTEVENT_JOY1_CD32_GREEN);
        setid (uid, num, ID_BUTTON_OFFSET + 3, 0, port, port ? INPUTEVENT_JOY2_CD32_YELLOW : INPUTEVENT_JOY1_CD32_YELLOW);
        setid (uid, num, ID_BUTTON_OFFSET + 4, 0, port, port ? INPUTEVENT_JOY2_CD32_RWD : INPUTEVENT_JOY1_CD32_RWD);
        setid (uid, num, ID_BUTTON_OFFSET + 5, 0, port, port ? INPUTEVENT_JOY2_CD32_FFW : INPUTEVENT_JOY1_CD32_FFW);
        setid (uid, num, ID_BUTTON_OFFSET + 6, 0, port, port ? INPUTEVENT_JOY2_CD32_PLAY :  INPUTEVENT_JOY1_CD32_PLAY);
    }
    if (num == 0) {
        return 1;
    }
    return 0;
}

