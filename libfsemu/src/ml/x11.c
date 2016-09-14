#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define _GNU_SOURCE 1
#ifdef USE_X11
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fs/log.h>
#include <fs/ml.h>
#include <fs/base.h>
#include <fs/image.h>
#include <fs/filesys.h>
#include <fs/conf.h>
#include "ml_internal.h"

#ifdef USE_SDL2
#include <SDL.h>
extern SDL_Window* g_fs_ml_window;
#endif

#include <SDL_syswm.h>
// FIXME: REMOVE / replace with fs functions
#include <glib.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>
#ifdef USE_X11_XTEST
#include <X11/extensions/XTest.h>
#endif

Display *g_display = NULL;
Window g_window = 0;

void fs_ml_prevent_power_saving(void)
{

}

void fs_ml_activate_window_switcher_impl(void)
{
    /* The code assumes that Alt+Tab is the window-switching combination
     * and that the left Alt key is already pressed.
     *
     * Works somewhat OK on GNOME 3 at least. The next window will be
     * switched to, but the window selector will not be shown unless
     * the user releases and re-presses the Alt key. */
#ifdef USE_X11_XTEST
    fs_log("Sending XTEST fake (alt) tab key press\n");
#if 0
    XTestFakeKeyEvent(g_display, XK_Alt_L, True, CurrentTime);
    XSync(g_display, False);
#endif
    XTestFakeKeyEvent(g_display, XK_Tab, True, CurrentTime);
    XSync(g_display, False);

    XTestFakeKeyEvent(g_display, XK_Tab, False, CurrentTime);
    XSync(g_display, False);
#if 0
    XTestFakeKeyEvent(g_display, XK_Alt_L, False, CurrentTime);
    XSync(g_display, False);
#endif
#else
    fs_log("Support for XTEST extension not enabled\n");
#endif
}

static void set_window_icon(void)
{
    fs_log("setting _NET_WM_ICON from icon images\n");

    int max_size = (16 * 16 + 32 * 32 + 48 * 48 + 64 * 64 + 128 * 128) * \
            sizeof(unsigned long);
    // add space for width, height cardinals
    max_size += 2 * 5 * sizeof(unsigned long);

    unsigned long *icon_data = (unsigned long *) g_malloc(max_size);
    unsigned long *op = icon_data;
    int card_count  = 0;

    int sizes[] = {128, 64, 48, 32, 16, 0};
    for(int *size = sizes; *size; size++) {
        char *rel = g_strdup_printf("icons/hicolor/%dx%d/apps/fs-uae.png",
                *size, *size);
        char *path = fs_get_data_file(rel);
        g_free(rel);
        if (!path) {
            fs_log("did not find icon for %dx%d\n", *size, *size);
            continue;
        }

        fs_image *image = fs_image_new_from_file(path);
        if (!image) {
            fs_log("could not load icon from %s\n", path);
            continue;
        }
        g_free(path);

        //printf("%d\n", image->width);
        int pixel_count = image->width * image->height;
        unsigned char *p = image->data;
        *op++ = image->width;
        *op++ = image->height;
        for (int i = 0; i < pixel_count; i++) {
            //*op = 0xffff0000;
            *op = (((unsigned long) p[3]) << 24) |
                    (p[0] << 16) |
                    (p[1] << 8) |
                    p[2];
            p += 4;
            op++;
        }
        card_count += 2 + pixel_count;

        fs_unref(image);

        // FIXME

    }

    Atom _NET_WM_ICON = XInternAtom(g_display, "_NET_WM_ICON", False);
    XChangeProperty(g_display, g_window, _NET_WM_ICON, XA_CARDINAL, 32,
            PropModeReplace, (unsigned char *) icon_data, card_count);
    g_free(icon_data);
}

static void set_above_state(void)
{
    printf("_NET_WM_STATE = _NET_WM_STATE_ABOVE\n");
    Atom ATOM = XInternAtom(g_display, "ATOM", False);
    Atom _NET_WM_STATE = XInternAtom(g_display, "_NET_WM_STATE", False);
    Atom _NET_WM_STATE_ABOVE = XInternAtom(
        g_display, "_NET_WM_STATE_ABOVE", False);
    XChangeProperty(g_display, g_window, _NET_WM_STATE, ATOM, 32,
            PropModeReplace, (unsigned char *) &_NET_WM_STATE_ABOVE, 1);

    XEvent ev;
    memset(&ev, 0, sizeof(ev));
    ev.type = ClientMessage;
    ev.xclient.type         = ClientMessage;
    ev.xclient.message_type = XInternAtom(g_display, "_NET_WM_STATE", 0);
    ev.xclient.display      = g_display;
    ev.xclient.window       = g_window;
    ev.xclient.format       = 32;
    ev.xclient.data.l[0]    = 1; // set ? 1 : 0;

    // *_STAYS_ON_TOP is probably KDE-specific
    ev.xclient.data.l[1] = XInternAtom(
        g_display, "_NET_WM_STATE_STAYS_ON_TOP", 0);
    XLockDisplay(g_display);
    XSendEvent(g_display, XDefaultRootWindow(g_display), 0,
               SubstructureRedirectMask | SubstructureNotifyMask, &ev);

    // to work with some non-KDE WMs we should use _NET_WM_STATE_ABOVE
    ev.xclient.data.l[1] = XInternAtom(
        g_display, "_NET_WM_STATE_ABOVE", 0);
    XSendEvent(g_display, XDefaultRootWindow(g_display), 0,
               SubstructureRedirectMask | SubstructureNotifyMask, &ev);
    XUnlockDisplay(g_display);
}

void fs_ml_configure_window(void)
{
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version); // this is important!
#ifdef USE_SDL2
    if (!SDL_GetWindowWMInfo(g_fs_ml_window, &info)) {
        fs_log("error getting window information\n");
        return;
    }
    if (info.subsystem != SDL_SYSWM_X11) {
        fs_log("[SDL] Subsystem is not SDL_SYSWM_X11\n");
        return;
    }
    g_display = info.info.x11.display;
    g_window = info.info.x11.window;
#else
    if (!SDL_GetWMInfo(&info)) {
        fs_log("error getting window information\n");
        return;
    }
    g_display = info.info.x11.display;
    g_window = info.info.x11.wmwindow;
#endif
    if (g_display == NULL) {
        fs_log("[X11] Display is NULL\n");
        return;
    }

    // Set the PID related to the window for the given hostname, if possible
    //   if (data->pid > 0) {
    //       _NET_WM_PID = XInternAtom(display, "_NET_WM_PID", False);
    //       XChangeProperty(display, w, _NET_WM_PID, XA_CARDINAL, 32, PropModeReplace,
    //                        (unsigned char *)&data->pid, 1);

    Atom UTF8_STRING = XInternAtom(g_display, "UTF8_STRING", False);

    fs_log("requesting dark window manager theme\n");
    Atom _GTK_THEME_VARIANT = XInternAtom(g_display, "_GTK_THEME_VARIANT", False);
    XChangeProperty(g_display, g_window, _GTK_THEME_VARIANT, UTF8_STRING, 8,
            PropModeReplace, (const unsigned char *) "dark", 4);

    set_window_icon();
    if (fs_config_get_int("always_on_top") == 1) {
        set_above_state();
    }
    /*
    set_window_icon(32);
    set_window_icon(48);
    set_window_icon(64);
    set_window_icon(128);
    */
}

enum {
    START,
    LINE,
    IGNORE,
    RESOLUTION,
    WAIT_REFRESH,
    REFRESH,
};

#if 0

#define CLOCK_FREQ 10000000
static int64_t g_syncbase = 0;

static int64_t fs_ml_read_clock(void)
{
    struct timespec tp;
    clock_gettime(CLOCK_REALTIME, &tp);
    int64_t t = ((int64_t) tp.tv_sec) * CLOCK_FREQ + \
            ((int64_t) tp.tv_nsec) / 100;
    //printf("%lld\n", t);

    static int64_t base_time = 0;
    if (base_time == 0) {
        base_time = t;
    }
    //    clock_gettime(CLOCK_REALTIME, &tp);
    //    base_secs = tp.tv_sec;
    //}
    //clock_gettime(CLOCK_REALTIME, &tp);
    //tp.tv_sec -= base_secs;
    //return ((int64_t) tp.tv_sec) * CLOCK_FREQ + ((int64_t) tp.tv_nsec) / 100;
    return t - base_time;
}

void fs_ml_calibrate_clock(void)
{
    g_syncbase = CLOCK_FREQ;
}

int64_t fs_ml_monotonic_time()
{
    //return (1000000 * fs_ml_read_clock()) / g_syncbase;
    return fs_ml_read_clock() / 10;
}

#endif

void fs_ml_usleep(int usec)
{
    usleep(usec);
}

#if 0
int fs_ml_video_mode_get_current(fs_ml_video_mode *mode)
{
    mode->width = 0;
    mode->height = 0;
    mode->fps = 0;
    mode->bpp = 0;
    mode->flags = 0;

    int last_refresh = 0;
    int mode_count = 0;
    int increasing_by_one = 1;

    FILE *f = popen("/usr/bin/env xrandr", "r");
    if (f == NULL) {
        return 1;
    }
    int state = START;
    char buf[24];
    int bp = 0; // buffer position
    char c;
    while(1) {
        int num_read = fread(&c, 1, 1, f);
        if (num_read == 0) {
            break;
        }
        if (state == START && c == ' ') {
            state = LINE;
        }
        else if (state == START) {
            state = IGNORE;
        }
        else if (state == IGNORE) {
            // yup
        }
        else if (state == LINE && c != ' ') {
            state = RESOLUTION;
        }
        else if (state == RESOLUTION && c == ' ') {
            state = WAIT_REFRESH;
        }
        else if (state == WAIT_REFRESH && c != ' ') {
            if (c != '\n') {
                buf[bp++] = c;
                state = REFRESH;
            }
        }
        else if (state == REFRESH && (c == ' ' || c == '*' || c == '\n')) {
            buf[bp] = '\0';
            bp = 0;
            int refresh = atoi(buf);
            if (last_refresh == 0) {
                last_refresh = refresh - 1;
            }
            if (refresh != last_refresh + 1) {
                increasing_by_one = 0;
            }
            last_refresh = refresh;
            mode_count += 1;
            //fs_log("%s %d\n", buf, refresh);
            if (c == '*') {
                fs_log("detected refresh rate: %d\n", refresh);
                mode->fps = refresh;
            }
            state = WAIT_REFRESH;
        }
        else if (state == REFRESH) {
            if (bp < 23) {
                buf[bp++] = c;
            }
        }
        // newline always resets state
        if (c == '\n') {
            state = START;
        }
    }
    pclose(f);

    if (mode_count > 1 && increasing_by_one) {
        // nVIDIA xrandr hack detected
        fs_log("xrandr is lying about about refresh rates; ignoring result\n");
        mode->fps = 0;
    }
    else if (mode_count == 1) {
        fs_log("don't trust refresh rate (from xrandr) with only one mode\n");
        mode->fps = 0;
    }
    return 0;
}
#endif

#define MAX_SCANCODES 200
static int g_key_map[MAX_SCANCODES] = {};

void fs_ml_initialize_keymap()
{
    for (int i = 0; i < MAX_SCANCODES; i++) {
        g_key_map[i] = -1;
    }

    g_key_map[9] = FS_ML_KEY_ESCAPE;
    g_key_map[67] = FS_ML_KEY_F1;
    g_key_map[68] = FS_ML_KEY_F2;
    g_key_map[69] = FS_ML_KEY_F3;
    g_key_map[70] = FS_ML_KEY_F4;
    g_key_map[71] = FS_ML_KEY_F5;
    g_key_map[72] = FS_ML_KEY_F6;
    g_key_map[73] = FS_ML_KEY_F7;
    g_key_map[74] = FS_ML_KEY_F8;
    g_key_map[75] = FS_ML_KEY_F9;
    g_key_map[76] = FS_ML_KEY_F10;
    g_key_map[95] = FS_ML_KEY_F11;
    g_key_map[96] = FS_ML_KEY_F12;

    g_key_map[49] = FS_ML_KEY_BACKQUOTE;
    g_key_map[10] = FS_ML_KEY_1;
    g_key_map[11] = FS_ML_KEY_2;
    g_key_map[12] = FS_ML_KEY_3;
    g_key_map[13] = FS_ML_KEY_4;
    g_key_map[14] = FS_ML_KEY_5;
    g_key_map[15] = FS_ML_KEY_6;
    g_key_map[16] = FS_ML_KEY_7;
    g_key_map[17] = FS_ML_KEY_8;
    g_key_map[18] = FS_ML_KEY_9;
    g_key_map[19] = FS_ML_KEY_0;
    g_key_map[20] = FS_ML_KEY_MINUS;
    g_key_map[21] = FS_ML_KEY_EQUALS;
    g_key_map[22] = FS_ML_KEY_BACKSPACE;

    g_key_map[23] = FS_ML_KEY_TAB;
    g_key_map[24] = FS_ML_KEY_Q;
    g_key_map[25] = FS_ML_KEY_W;
    g_key_map[26] = FS_ML_KEY_E;
    g_key_map[27] = FS_ML_KEY_R;
    g_key_map[28] = FS_ML_KEY_T;
    g_key_map[29] = FS_ML_KEY_Y;
    g_key_map[30] = FS_ML_KEY_U;
    g_key_map[31] = FS_ML_KEY_I;
    g_key_map[32] = FS_ML_KEY_O;
    g_key_map[33] = FS_ML_KEY_P;
    g_key_map[34] = FS_ML_KEY_LEFTBRACKET;
    g_key_map[35] = FS_ML_KEY_RIGHTBRACKET;
    g_key_map[36] = FS_ML_KEY_RETURN;

    g_key_map[66] = FS_ML_KEY_CAPSLOCK;
    g_key_map[38] = FS_ML_KEY_A;
    g_key_map[39] = FS_ML_KEY_S;
    g_key_map[40] = FS_ML_KEY_D;
    g_key_map[41] = FS_ML_KEY_F;
    g_key_map[42] = FS_ML_KEY_G;
    g_key_map[43] = FS_ML_KEY_H;
    g_key_map[44] = FS_ML_KEY_J;
    g_key_map[45] = FS_ML_KEY_K;
    g_key_map[46] = FS_ML_KEY_L;
    g_key_map[47] = FS_ML_KEY_SEMICOLON;
    g_key_map[48] = FS_ML_KEY_QUOTE;
    g_key_map[51] = FS_ML_KEY_BACKSLASH;

    g_key_map[50] = FS_ML_KEY_LSHIFT;
    g_key_map[94] = FS_ML_KEY_LESS;
    g_key_map[52] = FS_ML_KEY_Z;
    g_key_map[53] = FS_ML_KEY_X;
    g_key_map[54] = FS_ML_KEY_C;
    g_key_map[55] = FS_ML_KEY_V;
    g_key_map[56] = FS_ML_KEY_B;
    g_key_map[57] = FS_ML_KEY_N;
    g_key_map[58] = FS_ML_KEY_M;
    g_key_map[59] = FS_ML_KEY_COMMA;
    g_key_map[60] = FS_ML_KEY_PERIOD;
    g_key_map[61] = FS_ML_KEY_SLASH;
    g_key_map[62] = FS_ML_KEY_RSHIFT;

    g_key_map[37] = FS_ML_KEY_LCTRL;
    g_key_map[64] = FS_ML_KEY_LALT;
    g_key_map[65] = FS_ML_KEY_SPACE;

    g_key_map[78] = FS_ML_KEY_SCROLLOCK;

    g_key_map[77] = FS_ML_KEY_NUMLOCK;
    g_key_map[63] = FS_ML_KEY_KP_MULTIPLY;
    g_key_map[82] = FS_ML_KEY_KP_MINUS;

    g_key_map[79] = FS_ML_KEY_KP7;
    g_key_map[80] = FS_ML_KEY_KP8;
    g_key_map[81] = FS_ML_KEY_KP9;
    g_key_map[86] = FS_ML_KEY_KP_PLUS;

    g_key_map[83] = FS_ML_KEY_KP4;
    g_key_map[84] = FS_ML_KEY_KP5;
    g_key_map[85] = FS_ML_KEY_KP6;

    g_key_map[87] = FS_ML_KEY_KP1;
    g_key_map[88] = FS_ML_KEY_KP2;
    g_key_map[89] = FS_ML_KEY_KP3;

    g_key_map[90] = FS_ML_KEY_KP0;
    g_key_map[91] = FS_ML_KEY_KP_PERIOD;

    // keys codes above 96 differ between evdev / kdb Xorg drivers, but
    // on the other hand, these keys does (probably) not differ between
    // international keyboards, so we can use the translated keysyms from SDL
    // instead for these.
    /*
    g_key_map[104] = FS_ML_KEY_KP_ENTER;
    g_key_map[105] = FS_ML_KEY_RCTRL;
    g_key_map[106] = FS_ML_KEY_KP_DIVIDE;
    g_key_map[107] = FS_ML_KEY_PRINT;
    g_key_map[108] = FS_ML_KEY_RALT;
    g_key_map[110] = FS_ML_KEY_HOME;
    g_key_map[111] = FS_ML_KEY_UP;
    g_key_map[112] = FS_ML_KEY_PAGEUP;
    g_key_map[113] = FS_ML_KEY_LEFT;
    g_key_map[114] = FS_ML_KEY_RIGHT;
    g_key_map[115] = FS_ML_KEY_END;
    g_key_map[116] = FS_ML_KEY_DOWN;
    g_key_map[117] = FS_ML_KEY_PAGEDOWN;
    g_key_map[118] = FS_ML_KEY_INSERT;
    g_key_map[119] = FS_ML_KEY_DELETE;
    g_key_map[127] = FS_ML_KEY_PAUSE;
    g_key_map[133] = FS_ML_KEY_LSUPER;
    g_key_map[134] = FS_ML_KEY_RSUPER;
    g_key_map[135] = FS_ML_KEY_MENU;
    */
}

int fs_ml_scancode_to_key(int scancode)
{
    if (scancode < 0 || scancode >= MAX_SCANCODES) {
        return -1;
    }
    return g_key_map[scancode];
}

#else

int libfsemu_x11_dummy;

#endif // USE_X11
