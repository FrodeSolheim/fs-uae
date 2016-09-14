#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef MACOSX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mach/mach_time.h>
#include <ApplicationServices/ApplicationServices.h>
#include <CoreServices/CoreServices.h>
#include <Carbon/Carbon.h>
//#include <MacApplication.h>

#include <fs/log.h>
#include <fs/ml.h>
#include "ml_internal.h"

void fs_ml_configure_window(void)
{

}

#if 0
static int64_t g_syncbase = 0;

static int64_t fs_ml_read_clock (void) {
    return mach_absolute_time();
}

void fs_ml_calibrate_clock(void) {
    mach_timebase_info_data_t info;
    mach_timebase_info(&info);
    g_syncbase = 1000000000 * info.denom / info.numer;
}

int64_t fs_ml_monotonic_time() {
    return (1000000 * fs_ml_read_clock()) / g_syncbase;
}
#endif

void fs_ml_usleep(int usec) {
    usleep(usec);
}

void fs_ml_prevent_power_saving(void)
{
    static int counter = 0;
    if (++counter == 50 * 30) {
        // prevent screen saver about every 30 seconds
        UpdateSystemActivity(OverallAct);
        counter = 0;
    }
}

void fs_ml_activate_window_switcher_impl(void)
{
    fs_log("FIXME: Active window switcher not implemented\n");
}

//void fs_ml_set_fullscreen_extra() {
//    // Using LSUIPresentationMode instead
//    //SetSystemUIMode(kUIModeAllHidden, kUIOptionAutoShowMenuBar);
//}

#if 0
int fs_ml_video_mode_get_current(fs_ml_video_mode *mode)
{
    mode->width = 0;
    mode->height = 0;
    mode->fps = 0;
    mode->bpp = 0;
    mode->flags = 0;

#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 1060
    // CGDisplayCurrentMode is deprecated in Mac OS X 10.6, so we use
    // the new API here
    CGDirectDisplayID currentDisplay = CGDisplayPrimaryDisplay(
            kCGDirectMainDisplay);
    CGDisplayModeRef currentMode = CGDisplayCopyDisplayMode(
            currentDisplay);
    mode->width = CGDisplayModeGetWidth(currentMode);
    mode->height = CGDisplayModeGetHeight(currentMode);
    mode->fps = CGDisplayModeGetRefreshRate(currentMode);
    CGDisplayModeRelease(currentMode);
#else
    CFNumberRef number;
    long refresh, width, height;
    CFDictionaryRef currentMode = CGDisplayCurrentMode(kCGDirectMainDisplay);
    number = CFDictionaryGetValue(currentMode, kCGDisplayWidth);
    CFNumberGetValue(number, kCFNumberLongType, &width);
    number = CFDictionaryGetValue(currentMode, kCGDisplayHeight);
    CFNumberGetValue(number, kCFNumberLongType, &height);
    number = CFDictionaryGetValue(currentMode, kCGDisplayRefreshRate);
    CFNumberGetValue(number, kCFNumberLongType, &refresh);
    mode->width = width;
    mode->height = height;
    mode->fps = refresh;
#endif

    //fs_log("WARNING: assuming 60 Hz refresh rate on Mac OS X\n");
    //mode->fps = 60;

    if (mode->fps == 0) {
        fs_log("WARNING: refresh rate was not detected\n");
        fs_log("full video sync will not be enabled automatically, but can "
                "be forced\n");
    }

    return 0;
}
#endif

#define MAX_SCANCODES 200
static int g_key_map[MAX_SCANCODES] = {};

void fs_ml_initialize_keymap() {
    for (int i = 0; i < MAX_SCANCODES; i++) {
        g_key_map[i] = -1;
    }

    g_key_map[53] = FS_ML_KEY_ESCAPE;
    g_key_map[122] = FS_ML_KEY_F1;
    g_key_map[120] = FS_ML_KEY_F2;
    g_key_map[99] = FS_ML_KEY_F3;
    g_key_map[118] = FS_ML_KEY_F4;
    g_key_map[96] = FS_ML_KEY_F5;
    g_key_map[97] = FS_ML_KEY_F6;
    g_key_map[98] = FS_ML_KEY_F7;
    g_key_map[100] = FS_ML_KEY_F8;
    g_key_map[101] = FS_ML_KEY_F9;
    g_key_map[109] = FS_ML_KEY_F10;
    g_key_map[103] = FS_ML_KEY_F11;
    g_key_map[111] = FS_ML_KEY_F12;

    g_key_map[10] = FS_ML_KEY_BACKQUOTE;
    g_key_map[18] = FS_ML_KEY_1;
    g_key_map[19] = FS_ML_KEY_2;
    g_key_map[20] = FS_ML_KEY_3;
    g_key_map[21] = FS_ML_KEY_4;
    g_key_map[23] = FS_ML_KEY_5;
    g_key_map[22] = FS_ML_KEY_6;
    g_key_map[26] = FS_ML_KEY_7;
    g_key_map[28] = FS_ML_KEY_8;
    g_key_map[25] = FS_ML_KEY_9;
    g_key_map[29] = FS_ML_KEY_0;
    g_key_map[27] = FS_ML_KEY_MINUS;
    g_key_map[24] = FS_ML_KEY_EQUALS;
    g_key_map[15] = FS_ML_KEY_BACKSPACE;

    g_key_map[48] = FS_ML_KEY_TAB;
    g_key_map[12] = FS_ML_KEY_Q;
    g_key_map[13] = FS_ML_KEY_W;
    g_key_map[14] = FS_ML_KEY_E;
    g_key_map[15] = FS_ML_KEY_R;
    g_key_map[17] = FS_ML_KEY_T;
    g_key_map[16] = FS_ML_KEY_Y;
    g_key_map[32] = FS_ML_KEY_U;
    g_key_map[34] = FS_ML_KEY_I;
    g_key_map[31] = FS_ML_KEY_O;
    g_key_map[35] = FS_ML_KEY_P;
    g_key_map[33] = FS_ML_KEY_LEFTBRACKET;
    g_key_map[30] = FS_ML_KEY_RIGHTBRACKET;
    g_key_map[36] = FS_ML_KEY_RETURN;

    //g_key_map[66] = FS_ML_KEY_CAPSLOCK;
    g_key_map[0] = FS_ML_KEY_A;
    g_key_map[1] = FS_ML_KEY_S;
    g_key_map[2] = FS_ML_KEY_D;
    g_key_map[3] = FS_ML_KEY_F;
    g_key_map[5] = FS_ML_KEY_G;
    g_key_map[4] = FS_ML_KEY_H;
    g_key_map[38] = FS_ML_KEY_J;
    g_key_map[40] = FS_ML_KEY_K;
    g_key_map[37] = FS_ML_KEY_L;
    g_key_map[41] = FS_ML_KEY_SEMICOLON;
    g_key_map[39] = FS_ML_KEY_QUOTE;
    g_key_map[42] = FS_ML_KEY_BACKSLASH;

    //g_key_map[50] = FS_ML_KEY_LSHIFT;
    g_key_map[50] = FS_ML_KEY_LESS;
    g_key_map[6] = FS_ML_KEY_Z;
    g_key_map[7] = FS_ML_KEY_X;
    g_key_map[8] = FS_ML_KEY_C;
    g_key_map[9] = FS_ML_KEY_V;
    g_key_map[11] = FS_ML_KEY_B;
    g_key_map[45] = FS_ML_KEY_N;
    g_key_map[46] = FS_ML_KEY_M;
    g_key_map[43] = FS_ML_KEY_COMMA;
    g_key_map[47] = FS_ML_KEY_PERIOD;
    g_key_map[44] = FS_ML_KEY_SLASH;
    //g_key_map[62] = FS_ML_KEY_RSHIFT;

    //g_key_map[37] = FS_ML_KEY_LCTRL;
    //g_key_map[133] = FS_ML_KEY_LSUPER;
    //g_key_map[64] = FS_ML_KEY_LALT;
    g_key_map[49] = FS_ML_KEY_SPACE;
    //g_key_map[108] = FS_ML_KEY_RALT;
    //g_key_map[134] = FS_ML_KEY_RSUPER;
    g_key_map[110] = FS_ML_KEY_MENU;
    //g_key_map[105] = FS_ML_KEY_RCTRL;

    g_key_map[105] = FS_ML_KEY_PRINT;
    g_key_map[107] = FS_ML_KEY_SCROLLOCK;
    g_key_map[113] = FS_ML_KEY_PAUSE;

    //g_key_map[118] = FS_ML_KEY_INSERT;
    //g_key_map[110] = FS_ML_KEY_HOME;
    //g_key_map[112] = FS_ML_KEY_PAGEUP;

    //g_key_map[119] = FS_ML_KEY_DELETE;
    //g_key_map[115] = FS_ML_KEY_END;
    //g_key_map[117] = FS_ML_KEY_PAGEDOWN;

    //g_key_map[111] = FS_ML_KEY_UP;
    //g_key_map[113] = FS_ML_KEY_LEFT;
    //g_key_map[116] = FS_ML_KEY_DOWN;
    //g_key_map[114] = FS_ML_KEY_RIGHT;

    //g_key_map[77] = FS_ML_KEY_NUMLOCK;
    //g_key_map[106] = FS_ML_KEY_KP_DIVIDE;
    //g_key_map[63] = FS_ML_KEY_KP_MULTIPLY;
    //g_key_map[82] = FS_ML_KEY_KP_MINUS;

    //g_key_map[79] = FS_ML_KEY_KP7;
    //g_key_map[80] = FS_ML_KEY_KP8;
    //g_key_map[81] = FS_ML_KEY_KP9;
    //g_key_map[86] = FS_ML_KEY_KP_PLUS;

    //g_key_map[83] = FS_ML_KEY_KP4;
    //g_key_map[84] = FS_ML_KEY_KP5;
    //g_key_map[85] = FS_ML_KEY_KP6;

    //g_key_map[87] = FS_ML_KEY_KP1;
    //g_key_map[88] = FS_ML_KEY_KP2;
    //g_key_map[89] = FS_ML_KEY_KP3;
    //g_key_map[104] = FS_ML_KEY_KP_ENTER;

    //g_key_map[90] = FS_ML_KEY_KP0;
    //g_key_map[91] = FS_ML_KEY_KP_PERIOD;
}

int fs_ml_scancode_to_key(int scancode) {
    if (scancode < 0 || scancode >= MAX_SCANCODES) {
        return -1;
    }
    return g_key_map[scancode];
}

#endif // MACOSX
