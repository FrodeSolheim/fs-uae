#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef WINDOWS
#define _WIN32_WINNT 0x0501
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <stdio.h>
#include <stddef.h>
#include <glib.h>

#include <fs/lazyness.h>
#include <fs/log.h>
#include "fs/ml.h"
#include "ml_internal.h"

#include <SDL.h>
#include <SDL_syswm.h>

#define NUM_VKEYS 256
#define MAX_VKEY (3 * NUM_VKEYS - 1)
// first NUM_VKEYS entries contain vkeys, then vkeys for E0 and E1
#define E0 NUM_VKEYS
#define E1 (NUM_VKEYS * 2)

#ifndef MAPVK_VK_TO_CHAR
#define MAPVK_VK_TO_CHAR 2
#endif

#define USE_MAKECODES

static char g_key_state[3 * NUM_VKEYS] = {};
static int16_t g_key_mapping[3 * NUM_VKEYS] = {};
static char g_is_modifier_key[FS_ML_KEY_LAST] = {};
//static int16_t g_key_mapping[3 * NUM_VKEYS] = {};

static HWND g_window = 0;
//static HGLRC g_hglrc = 0;
static HKL g_keyboard_layout = 0;

static int g_mod_lalt = 0;
static int g_mod_ralt = 0;
static int g_mod_lctrl = 0;
static int g_mod_rctrl = 0;
static int g_mod_lshift = 0;
static int g_mod_rshift = 0;
static int g_mod_f11 = 0;
static int g_mod_f12 = 0;
static int g_mod = 0;

void fs_ml_clear_keyboard_modifier_state()
{
    fs_log("clearing keyboard modifier state\n");
    g_mod_lalt = 0;
    g_mod_ralt = 0;
    g_mod_lctrl = 0;
    g_mod_rctrl = 0;
    g_mod_lshift = 0;
    g_mod_rshift = 0;
    g_mod_f11 = 0;
    g_mod_f12 = 0;
    g_mod = 0;
}

static void process_keyboard_input(LPRAWINPUT raw_input)
{
    //LPRAWKEYBOARD raw_keyboard = &(raw_input->data.keyboard);
    //int vkey = raw_keyboard->VKey;
    //int vkey = raw_keyboard->VKey;
    //int flags = raw_keyboard->Flags; //
    int vkey = raw_input->data.keyboard.VKey;
    int flags = raw_input->data.keyboard.Flags;
    int make_code = raw_input->data.keyboard.MakeCode;
    if (g_fs_log_input) {
        fs_log("vkey...: %d (0x%x) make %d extra %d flags %d E0 %d E1 %d\n",
                vkey, vkey, raw_input->data.keyboard.MakeCode,
                raw_input->data.keyboard.ExtraInformation, flags,
                (flags & RI_KEY_E0) != 0, (flags & RI_KEY_E1) != 0);
    }
#ifdef USE_MAKECODES
    vkey = make_code;
    if (flags & RI_KEY_E0) {
        vkey += NUM_VKEYS;
    }
    else if (flags & RI_KEY_E1) {
        vkey += 2 * NUM_VKEYS;
    }
#else
    // special cases
    if (make_code == 54) {
        // for some reason, flags is not set properly for right shift
        vkey = VK_SHIFT + E0;
    }
    else if (flags & RI_KEY_E0) {
        vkey += NUM_VKEYS;
    }
    else if (flags & RI_KEY_E1) {
        vkey += 2 * NUM_VKEYS;
    }
#endif

    int update_mod = 0;

    int state = ((flags & RI_KEY_BREAK) == 0);
    switch (vkey) {
    case 42:
        g_mod_lshift = state;
        //mod_shift = g_mod_lshift || g_mod_rshift;
        update_mod = 1;
        break;
    case 54:
        g_mod_rshift = state;
        //mod_shift = g_mod_lshift || g_mod_rshift;
        update_mod = 1;
        break;
    case 29:
        g_mod_lctrl = state;
        //mod_ctrl = g_mod_lctrl || g_mod_rctrl;
        update_mod = 1;
        break;
    case E0 + 29:
        g_mod_rctrl = state;
        //mod_ctrl = g_mod_lctrl || g_mod_rctrl;
        update_mod = 1;
        break;
    case 56:
        g_mod_lalt = state;
        //mod_alt = g_mod_lalt || g_mod_ralt;
        update_mod = 1;
        break;
    case E0 + 56:
        g_mod_ralt = state;
        update_mod = 1;
        break;
    case 87:
        g_mod_f11 = state;
        update_mod = 1;
        break;
    case 88:
        g_mod_f12 = state;
        update_mod = 1;
        break;
    }

    static unsigned char keyboard_state[256] = {};
    //GetKeyboardState(keyboard_state);

    if (update_mod) {
        if (g_mod_lshift || g_mod_rshift) {
            g_mod = g_mod | KMOD_SHIFT;
            keyboard_state[VK_SHIFT] = 0x80;
        }
        else {
            g_mod = g_mod & ~KMOD_SHIFT;
            keyboard_state[VK_SHIFT] = 0x0;
        }
        if (g_mod_lctrl || g_mod_rctrl) {
            g_mod = g_mod | KMOD_CTRL;
            keyboard_state[VK_CONTROL] = 0x80;
        }
        else {
            g_mod = g_mod & ~KMOD_CTRL;
            keyboard_state[VK_CONTROL] = 0x0;
        }
        if (g_mod_lalt || g_mod_ralt) {
            g_mod = g_mod | KMOD_ALT;
            keyboard_state[VK_MENU] = 0x80;
        }
        else {
            g_mod = g_mod & ~KMOD_ALT;
            keyboard_state[VK_MENU] = 0x0;
        }
        if (g_mod_f11) {
            g_mod = g_mod | FS_ML_KEY_MOD_F11;
        }
        else {
            g_mod = g_mod & ~FS_ML_KEY_MOD_F11;
        }
        if (g_mod_f12) {
            g_mod = g_mod | FS_ML_KEY_MOD_F12;
        }
        else {
            g_mod = g_mod & ~FS_ML_KEY_MOD_F12;
        }
    }

    int key_code = g_key_mapping[vkey];
    WORD character = 0;
    if (!g_is_modifier_key[key_code]) {
        ToAsciiEx(raw_input->data.keyboard.VKey,
                raw_input->data.keyboard.MakeCode, keyboard_state,
                &character, 0, g_keyboard_layout);
        //printf("%d %d => %d\n", raw_input->data.keyboard.VKey,
        //        raw_input->data.keyboard.MakeCode, character);
        if (character > 128) {
            character = 0;
        }
        if (state == 1) {
            fs_ml_event *event = fs_ml_alloc_event();
            event->type = FS_ML_TEXTINPUT;
            event->text.text[0] = character;
            event->text.text[1] = '\0';
            fs_ml_post_event(event);
        }
    }

    if (state == 0) {
        // key is up
        if (vkey <= MAX_VKEY && g_key_state[vkey]) {
            g_key_state[vkey] = 0;
            fs_ml_event *event = fs_ml_alloc_event();
            event->type = FS_ML_KEYUP;
            event->key.keysym.sym = key_code;

            event->key.keysym.mod = g_mod;
            //event->key.keysym.unicode = character;
            event->key.state = 0;
            fs_ml_post_event(event);
        }
    }
    else {
        // key is down
        if (vkey <= MAX_VKEY && !g_key_state[vkey]) {
            //fs_log("down\n");
            g_key_state[vkey] = 1;
            fs_ml_event *event = fs_ml_alloc_event();
            event->type = FS_ML_KEYDOWN;
            event->key.keysym.sym = key_code;

            event->key.keysym.mod = g_mod;
            //event->key.keysym.unicode = character;
            event->key.state = 1;
            //fs_log("code: %d\n", event->key.code);
            fs_ml_post_event(event);
        }
    }
}

static void process_input(void *data)
{
    //fs_log("process_input\n");
    LPRAWINPUT raw_input = data;
    if (raw_input->header.dwType == RIM_TYPEMOUSE) {
        //process_mouse_input(raw_input);
    } else if (raw_input->header.dwType == RIM_TYPEKEYBOARD) {
        process_keyboard_input(raw_input);
    }
}

#define RAW_INPUT_MAX_SIZE 1024
static unsigned char *g_raw_input_data[RAW_INPUT_MAX_SIZE];

static WNDPROC g_wndproc = NULL;

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam,
        LPARAM lparam)
{
    //fs_log("WndProc %d\n", message);
    HRAWINPUT raw_input_handle;
    switch (message) {
    case WM_INPUTLANGCHANGE:
        g_keyboard_layout = GetKeyboardLayout(0);
        break;
    case WM_INPUT:
        raw_input_handle = (HRAWINPUT) lparam;
        /*
        unsigned int size;
        if (GetRawInputData(raw_input_handle, RID_INPUT, NULL, &size,
                sizeof(RAWINPUTHEADER)) != -1) {
            void *data = malloc(size);
            if (GetRawInputData(raw_input_handle, RID_INPUT, data, &size,
                    sizeof(RAWINPUTHEADER)) != -1) {
                process_input(data);
            }
            free(data);
        }
        */
        unsigned int size = RAW_INPUT_MAX_SIZE;
        if (GetRawInputData(raw_input_handle, RID_INPUT,
                &g_raw_input_data, &size, sizeof(RAWINPUTHEADER)) != -1) {
            process_input(&g_raw_input_data);
        }
        // must call DefWindowProc according to http://msdn.microsoft.com/
        // en-us/library/windows/desktop/ms645590(v=vs.85).aspx
        // EDIT: We want SDL2 to receive WM_INPUT messages as well (for mouse
        // input) so we let CallWindowProc be called...
        // return DefWindowProc(hwnd, message, wparam, lparam);
    }

    return CallWindowProc(g_wndproc, hwnd, message, wparam, lparam);
}

static void init_key_mapping(void)
{
    // http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=VS.85).aspx

#ifdef USE_MAKECODES

    g_key_mapping[1] = FS_ML_KEY_ESCAPE;
    g_key_mapping[59] = FS_ML_KEY_F1;
    g_key_mapping[60] = FS_ML_KEY_F2;
    g_key_mapping[61] = FS_ML_KEY_F3;
    g_key_mapping[62] = FS_ML_KEY_F4;
    g_key_mapping[63] = FS_ML_KEY_F5;
    g_key_mapping[64] = FS_ML_KEY_F6;
    g_key_mapping[65] = FS_ML_KEY_F7;
    g_key_mapping[66] = FS_ML_KEY_F8;
    g_key_mapping[67] = FS_ML_KEY_F9;
    g_key_mapping[68] = FS_ML_KEY_F10;
    g_key_mapping[87] = FS_ML_KEY_F11;
    g_key_mapping[88] = FS_ML_KEY_F12;

    g_key_mapping[41] = FS_ML_KEY_BACKQUOTE;
    g_key_mapping[2] = FS_ML_KEY_1;
    g_key_mapping[3] = FS_ML_KEY_2;
    g_key_mapping[4] = FS_ML_KEY_3;
    g_key_mapping[5] = FS_ML_KEY_4;
    g_key_mapping[6] = FS_ML_KEY_5;
    g_key_mapping[7] = FS_ML_KEY_6;
    g_key_mapping[8] = FS_ML_KEY_7;
    g_key_mapping[9] = FS_ML_KEY_8;
    g_key_mapping[10] = FS_ML_KEY_9;
    g_key_mapping[11] = FS_ML_KEY_0;
    g_key_mapping[12] = FS_ML_KEY_MINUS;
    g_key_mapping[13] = FS_ML_KEY_EQUALS;
    g_key_mapping[14] = FS_ML_KEY_BACKSPACE;

    g_key_mapping[15] = FS_ML_KEY_TAB;
    g_key_mapping[16] = FS_ML_KEY_Q;
    g_key_mapping[17] = FS_ML_KEY_W;
    g_key_mapping[18] = FS_ML_KEY_E;
    g_key_mapping[19] = FS_ML_KEY_R;
    g_key_mapping[20] = FS_ML_KEY_T;
    g_key_mapping[21] = FS_ML_KEY_Y;
    g_key_mapping[22] = FS_ML_KEY_U;
    g_key_mapping[23] = FS_ML_KEY_I;
    g_key_mapping[24] = FS_ML_KEY_O;
    g_key_mapping[25] = FS_ML_KEY_P;
    g_key_mapping[26] = FS_ML_KEY_LEFTBRACKET;
    g_key_mapping[27] = FS_ML_KEY_RIGHTBRACKET;
    g_key_mapping[28] = FS_ML_KEY_RETURN;

    g_key_mapping[58] = FS_ML_KEY_CAPSLOCK;
    g_key_mapping[30] = FS_ML_KEY_A;
    g_key_mapping[31] = FS_ML_KEY_S;
    g_key_mapping[32] = FS_ML_KEY_D;
    g_key_mapping[33] = FS_ML_KEY_F;
    g_key_mapping[34] = FS_ML_KEY_G;
    g_key_mapping[35] = FS_ML_KEY_H;
    g_key_mapping[36] = FS_ML_KEY_J;
    g_key_mapping[37] = FS_ML_KEY_K;
    g_key_mapping[38] = FS_ML_KEY_L;
    g_key_mapping[39] = FS_ML_KEY_SEMICOLON;
    g_key_mapping[40] = FS_ML_KEY_QUOTE;
    g_key_mapping[43] = FS_ML_KEY_BACKSLASH;

    g_key_mapping[42] = FS_ML_KEY_LSHIFT;
    g_key_mapping[86] = FS_ML_KEY_LESS;
    g_key_mapping[44] = FS_ML_KEY_Z;
    g_key_mapping[45] = FS_ML_KEY_X;
    g_key_mapping[46] = FS_ML_KEY_C;
    g_key_mapping[47] = FS_ML_KEY_V;
    g_key_mapping[48] = FS_ML_KEY_B;
    g_key_mapping[49] = FS_ML_KEY_N;
    g_key_mapping[50] = FS_ML_KEY_M;
    g_key_mapping[51] = FS_ML_KEY_COMMA;
    g_key_mapping[52] = FS_ML_KEY_PERIOD;
    g_key_mapping[53] = FS_ML_KEY_SLASH;
    g_key_mapping[54] = FS_ML_KEY_RSHIFT;

    g_key_mapping[29] = FS_ML_KEY_LCTRL;
    g_key_mapping[E0 + 91] = FS_ML_KEY_LSUPER;
    g_key_mapping[56] = FS_ML_KEY_LALT;
    g_key_mapping[57] = FS_ML_KEY_SPACE;
    g_key_mapping[E0 + 56] = FS_ML_KEY_RALT;
    g_key_mapping[E0 + 92] = FS_ML_KEY_RSUPER;
    g_key_mapping[E0 + 93] = FS_ML_KEY_MENU;
    g_key_mapping[E0 + 29] = FS_ML_KEY_RCTRL;

    g_key_mapping[E0 + 55] = FS_ML_KEY_PRINT;
    //g_key_mapping[E0 + 42] = FS_ML_KEY_PRINT;
    g_key_mapping[70] = FS_ML_KEY_SCROLLOCK;
    g_key_mapping[E1 + 29] = FS_ML_KEY_PAUSE;
    //g_key_mapping[69] = FS_ML_KEY_PAUSE;

    g_key_mapping[E0 + 82] = FS_ML_KEY_INSERT;
    g_key_mapping[E0 + 71] = FS_ML_KEY_HOME;
    g_key_mapping[E0 + 73] = FS_ML_KEY_PAGEUP;

    g_key_mapping[E0 + 83] = FS_ML_KEY_DELETE;
    g_key_mapping[E0 + 79] = FS_ML_KEY_END;
    g_key_mapping[E0 + 81] = FS_ML_KEY_PAGEDOWN;

    g_key_mapping[E0 + 72] = FS_ML_KEY_UP;
    g_key_mapping[E0 + 75] = FS_ML_KEY_LEFT;
    g_key_mapping[E0 + 80] = FS_ML_KEY_DOWN;
    g_key_mapping[E0 + 77] = FS_ML_KEY_RIGHT;

    g_key_mapping[69] = FS_ML_KEY_NUMLOCK;
    g_key_mapping[E0 + 53] = FS_ML_KEY_KP_DIVIDE;
    g_key_mapping[55] = FS_ML_KEY_KP_MULTIPLY;
    g_key_mapping[74] = FS_ML_KEY_KP_MINUS;

    g_key_mapping[71] = FS_ML_KEY_KP7;
    g_key_mapping[72] = FS_ML_KEY_KP8;
    g_key_mapping[73] = FS_ML_KEY_KP9;
    g_key_mapping[78] = FS_ML_KEY_KP_PLUS;

    g_key_mapping[75] = FS_ML_KEY_KP4;
    g_key_mapping[76] = FS_ML_KEY_KP5;
    g_key_mapping[77] = FS_ML_KEY_KP6;

    g_key_mapping[79] = FS_ML_KEY_KP1;
    g_key_mapping[80] = FS_ML_KEY_KP2;
    g_key_mapping[81] = FS_ML_KEY_KP3;
    g_key_mapping[E0 + 28] = FS_ML_KEY_KP_ENTER;

    g_key_mapping[82] = FS_ML_KEY_KP0;
    g_key_mapping[83] = FS_ML_KEY_KP_PERIOD;

    g_is_modifier_key[FS_ML_KEY_LCTRL] = 1;
    g_is_modifier_key[FS_ML_KEY_RCTRL] = 1;
    g_is_modifier_key[FS_ML_KEY_LALT] = 1;
    g_is_modifier_key[FS_ML_KEY_RALT] = 1;
    g_is_modifier_key[FS_ML_KEY_LSHIFT] = 1;
    g_is_modifier_key[FS_ML_KEY_RSHIFT] = 1;
    g_is_modifier_key[FS_ML_KEY_CAPSLOCK] = 1;
    g_is_modifier_key[FS_ML_KEY_LSUPER] = 1;
    g_is_modifier_key[FS_ML_KEY_RSUPER] = 1;

#else
    g_key_mapping[VK_BACK] = FS_ML_KEY_BACKSPACE;
    g_key_mapping[VK_TAB] = FS_ML_KEY_TAB;
    g_key_mapping[VK_CLEAR] = FS_ML_KEY_CLEAR;
    g_key_mapping[VK_RETURN] = FS_ML_KEY_RETURN;
    // FIXME: RSHIFT
    g_key_mapping[VK_SHIFT] = FS_ML_KEY_LSHIFT;
    g_key_mapping[VK_CONTROL] = FS_ML_KEY_LCTRL;
    g_key_mapping[E0 + VK_CONTROL] = FS_ML_KEY_RCTRL;
    g_key_mapping[VK_MENU] = FS_ML_KEY_LALT;
    g_key_mapping[E1 + VK_PAUSE] = FS_ML_KEY_PAUSE;
    g_key_mapping[VK_CAPITAL] = FS_ML_KEY_CAPSLOCK;

    g_key_mapping[VK_ESCAPE] = FS_ML_KEY_ESCAPE;
    g_key_mapping[VK_SPACE] = FS_ML_KEY_SPACE;
    g_key_mapping[E0 + VK_PRIOR] = FS_ML_KEY_PAGEUP;
    g_key_mapping[E0 + VK_NEXT] = FS_ML_KEY_PAGEDOWN;
    g_key_mapping[E0 + VK_END] = FS_ML_KEY_END;
    g_key_mapping[E0 + VK_HOME] = FS_ML_KEY_HOME;

    g_key_mapping[E0 + VK_LEFT] = FS_ML_KEY_LEFT;
    g_key_mapping[E0 + VK_UP] = FS_ML_KEY_UP;
    g_key_mapping[E0 + VK_RIGHT] = FS_ML_KEY_RIGHT;
    g_key_mapping[E0 + VK_DOWN] = FS_ML_KEY_DOWN;
    // SELECT
    // PRINT
    // EXECUTE
    g_key_mapping[E0 + VK_SNAPSHOT] = FS_ML_KEY_PRINT;
    g_key_mapping[E0 + VK_INSERT] = FS_ML_KEY_INSERT;
    g_key_mapping[E0 + VK_DELETE] = FS_ML_KEY_DELETE;
    // HELP
    g_key_mapping[0x30] = FS_ML_KEY_0;
    g_key_mapping[0x31] = FS_ML_KEY_1;
    g_key_mapping[0x32] = FS_ML_KEY_2;
    g_key_mapping[0x33] = FS_ML_KEY_3;
    g_key_mapping[0x34] = FS_ML_KEY_4;
    g_key_mapping[0x35] = FS_ML_KEY_5;
    g_key_mapping[0x36] = FS_ML_KEY_6;
    g_key_mapping[0x37] = FS_ML_KEY_7;
    g_key_mapping[0x38] = FS_ML_KEY_8;
    g_key_mapping[0x39] = FS_ML_KEY_9;

    g_key_mapping[0x41] = FS_ML_KEY_A;
    g_key_mapping[0x42] = FS_ML_KEY_B;
    g_key_mapping[0x43] = FS_ML_KEY_C;
    g_key_mapping[0x44] = FS_ML_KEY_D;
    g_key_mapping[0x45] = FS_ML_KEY_E;
    g_key_mapping[0x46] = FS_ML_KEY_F;
    g_key_mapping[0x47] = FS_ML_KEY_G;
    g_key_mapping[0x48] = FS_ML_KEY_H;
    g_key_mapping[0x49] = FS_ML_KEY_I;
    g_key_mapping[0x4a] = FS_ML_KEY_J;
    g_key_mapping[0x4b] = FS_ML_KEY_K;
    g_key_mapping[0x4c] = FS_ML_KEY_L;
    g_key_mapping[0x4d] = FS_ML_KEY_M;
    g_key_mapping[0x4e] = FS_ML_KEY_N;
    g_key_mapping[0x4f] = FS_ML_KEY_O;
    g_key_mapping[0x50] = FS_ML_KEY_P;
    g_key_mapping[0x51] = FS_ML_KEY_Q;
    g_key_mapping[0x52] = FS_ML_KEY_R;
    g_key_mapping[0x53] = FS_ML_KEY_S;
    g_key_mapping[0x54] = FS_ML_KEY_T;
    g_key_mapping[0x55] = FS_ML_KEY_U;
    g_key_mapping[0x56] = FS_ML_KEY_V;
    g_key_mapping[0x57] = FS_ML_KEY_W;
    g_key_mapping[0x58] = FS_ML_KEY_X;
    g_key_mapping[0x59] = FS_ML_KEY_Y;
    g_key_mapping[0x5a] = FS_ML_KEY_Z;
    g_key_mapping[VK_LWIN] = FS_ML_KEY_LSUPER; // FIXME?
    g_key_mapping[VK_RWIN] = FS_ML_KEY_RSUPER; // FIXME?
    g_key_mapping[VK_APPS] = FS_ML_KEY_MENU; // FIXME? OR MODE?
    // SLEEP
    g_key_mapping[VK_NUMPAD0] = FS_ML_KEY_KP0;
    g_key_mapping[VK_NUMPAD1] = FS_ML_KEY_KP1;
    g_key_mapping[VK_NUMPAD2] = FS_ML_KEY_KP2;
    g_key_mapping[VK_NUMPAD3] = FS_ML_KEY_KP3;
    g_key_mapping[VK_NUMPAD4] = FS_ML_KEY_KP4;
    g_key_mapping[VK_NUMPAD5] = FS_ML_KEY_KP5;
    g_key_mapping[VK_NUMPAD6] = FS_ML_KEY_KP6;
    g_key_mapping[VK_NUMPAD7] = FS_ML_KEY_KP7;
    g_key_mapping[VK_NUMPAD8] = FS_ML_KEY_KP8;
    g_key_mapping[VK_NUMPAD9] = FS_ML_KEY_KP9;

    // ... FIXME ...
    g_key_mapping[VK_F1] = FS_ML_KEY_F1;
    g_key_mapping[VK_F2] = FS_ML_KEY_F2;
    g_key_mapping[VK_F3] = FS_ML_KEY_F3;
    g_key_mapping[VK_F4] = FS_ML_KEY_F4;
    g_key_mapping[VK_F5] = FS_ML_KEY_F5;
    g_key_mapping[VK_F6] = FS_ML_KEY_F6;
    g_key_mapping[VK_F7] = FS_ML_KEY_F7;
    g_key_mapping[VK_F8] = FS_ML_KEY_F8;
    g_key_mapping[VK_F9] = FS_ML_KEY_F9;
    g_key_mapping[VK_F10] = FS_ML_KEY_F10;
    g_key_mapping[VK_F11] = FS_ML_KEY_F11;
    g_key_mapping[VK_F12] = FS_ML_KEY_F12;

    g_key_mapping[VK_NUMLOCK] = FS_ML_KEY_NUMLOCK;
    g_key_mapping[VK_SCROLL] = FS_ML_KEY_SCROLLOCK;

    g_key_mapping[VK_OEM_5] = FS_ML_KEY_BACKQUOTE;
    //g_key_mapping[VK_OEM_5] = FS_ML_KEY_BACKQUOTE;
#endif
}

void fs_ml_init_raw_input(void)
{
    fs_log("fs_ml_init_raw_input\n");

    //list_input_devices();
    init_key_mapping();

    SDL_SysWMinfo info;
    SDL_VERSION(&info.version); // this is important!
#if SDL_VERSION_ATLEAST(2, 0, 0)
    if (SDL_GetWindowWMInfo(g_fs_ml_window, &info)) {
#else
    if (SDL_GetWMInfo(&info)) {
#endif
#if SDL_VERSION_ATLEAST(2, 0, 0)
        g_window = info.info.win.window;
#else
        g_window = info.window;
#endif
        //g_hglrc = info.hglrc;
    }
 
    g_wndproc = (WNDPROC) GetWindowLongPtr(g_window, GWLP_WNDPROC);
    SetWindowLongPtr(g_window, GWLP_WNDPROC, (LONG_PTR) WndProc);
    fs_log("[RAWINPUT ]Old wndproc: %p new wndproc: %p\n", g_wndproc, WndProc);

    RAWINPUTDEVICE rid;
    /*
    // mice
    rid.usUsagePage = 0x01;
    rid.usUsage = 0x02;
    rid.dwFlags = 0;
    //rid.dwFlags = RIDEV_INPUTSINK;
    //rid.dwFlags = RIDEV_NOLEGACY;
    rid.hwndTarget = hWnd;
    RegisterRawInputDevices(&rid, 1, sizeof(rid));
    */
    // keyboards
    rid.usUsagePage = 0x01;
    rid.usUsage = 0x06;
    rid.dwFlags = RIDEV_NOLEGACY | RIDEV_NOHOTKEYS;
    rid.hwndTarget = g_window;
    RegisterRawInputDevices(&rid, 1, sizeof(rid));

    g_keyboard_layout = GetKeyboardLayout(0);
}

#else

int libfsemu_ml_rawinput_dummy;

#endif
