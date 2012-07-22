// we do this to be able to reference a static function in inputdevice.cpp

#include "../inputdevice.cpp"

int amiga_handle_input_event (int nr, int state, int max,
        int autofire, bool canstopplayback, bool playbackevent) {
    return handle_input_event(nr, state, max, autofire, canstopplayback,
            playbackevent);
}

#if 0
void amiga_configure_port_from_input_event(int input_event) {
    iscd32(input_event);
    isparport(input_event);
    ismouse(input_event);
    isanalog(input_event);
    isdigitalbutton(input_event);
}
#endif

/*
iscd32 (ei);
isparport (ei);
ismouse (ei);
isanalog (ei);
isdigitalbutton (ei);

static int ip_joy1[];
static int ip_joy2[];
static int ip_joypad1[];
static int ip_joypad2[];
static int ip_joycd321[];
static int ip_joycd322[];
static int ip_parjoy1[];
static int ip_parjoy2[];
static int ip_parjoy1default[];
static int ip_parjoy2default[];
static int ip_mouse1[];
static int ip_mouse2[];
static int ip_mousecdtv[];
static int ip_mediacdtv[];
static int ip_arcadia[];
static int ip_lightpen1[];
static int ip_lightpen2[];
static int ip_analog1[];
static int ip_analog2[];
*/

extern "C" {

#if 0
int amiga_get_joystick_port_mode(int port) {
    return currprefs.jports[port].mode;
}
#endif

void amiga_set_joystick_port_mode(int port, int mode) {
    write_log("***** amiga_set_joystick_port_mode port=%d mode=%d\n", port,
            mode);
    int *ip = NULL;
    //parport_joystick_enabled = 0;
    if (port == 0 || port == 1) {
        mouse_port[port] = 0;
        cd32_pad_enabled[port] = 0;
        for (int j = 0; j < 2; j++) {
            digital_port[port][j] = 0;
            analog_port[port][j] = 0;
            joydirpot[port][j] = 128 / (312 * 100
                    / currprefs.input_analog_joystick_mult)
                    + (128 * currprefs.input_analog_joystick_mult / 100)
                    + currprefs.input_analog_joystick_offset;
        }
    }

    if (port == 0) {
        if (mode == AMIGA_JOYPORT_MOUSE) {
            ip = ip_mouse1;
        }
        else if (mode == AMIGA_JOYPORT_CD32JOY) {
            ip = ip_joycd321;
        }
        else {
            ip = ip_joy1;
        }
    }
    else if (port == 1) {
        if (mode == AMIGA_JOYPORT_MOUSE) {
            ip = ip_mouse2;
        }
        else if (mode == AMIGA_JOYPORT_CD32JOY) {
            ip = ip_joycd322;
        }
        else {
            ip = ip_joy2;
        }
    }
    else if (port == 2) {
        if (mode == AMIGA_JOYPORT_DJOY) {
            ip = ip_parjoy1;
        }
    }
    else if (port == 3) {
        if (mode == AMIGA_JOYPORT_DJOY) {
            ip = ip_parjoy2;
        }
    }

    if (ip) {
        while (*ip != -1) {
            iscd32(*ip);
            isparport(*ip);
            ismouse(*ip);
            isanalog(*ip);
            isdigitalbutton(*ip);
            ip++;
        }
    }
    //changed_prefs.jports[port].mode = mode;
    //config_changed = 1;
    //inputdevice_updateconfig(&currprefs);
}

}
