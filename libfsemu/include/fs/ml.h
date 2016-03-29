/* libfsml - a media library for video and input
 * Copyright (C) 2011 Frode Solheim <frode-code@fengestad.no>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef LIBFSML_ML_H_
#define LIBFSML_ML_H_

#include <fs/malloc.h>
#include <fs/base.h>
#include <stdint.h>
#include <stdbool.h>

//#include <fs/ml/opengl.h>

void fs_ml_quit();
int fs_ml_is_quitting();

extern int g_fs_ml_opengl_context_stamp;

bool fs_ml_fullscreen(void);
void fs_ml_set_fullscreen(bool fullscreen);
void fs_ml_toggle_fullscreen(void);

typedef void (*fs_ml_void_function)();
typedef int (*fs_ml_int_function)();

typedef struct fs_ml_video_mode {
    int width;
    int height;
    int fps;
    int bpp;
    int flags;
} fs_ml_video_mode;

void fs_ml_show_cursor(int show, int immediate);
bool fs_ml_mouse_integration(void);
bool fs_ml_cursor_allowed(void);

bool fs_ml_input_grab(void);
void fs_ml_set_input_grab(bool grab);

bool fs_ml_automatic_input_grab(void);
void fs_ml_set_input_grab_on_activate(bool grab);

void fs_ml_activate_window_switcher(void);

static inline int64_t fs_ml_monotonic_time() {
    return fs_get_monotonic_time();
}

void fs_ml_usleep(int usec);

#define FS_ML_VIDEO_EVENT_GRAB_INPUT 1
#define FS_ML_VIDEO_EVENT_UNGRAB_INPUT 2
#define FS_ML_VIDEO_EVENT_SHOW_CURSOR 3
#define FS_ML_VIDEO_EVENT_HIDE_CURSOR 4

void fs_ml_init();
void fs_ml_init_2(void);

void fs_ml_stop();

int fs_ml_main_loop();

int fs_ml_get_video_sync();
double fs_ml_get_refresh_rate(void);
int fs_ml_get_vblank_sync();

int fs_ml_get_vblank_count();
int64_t fs_ml_get_vblank_time();

int fs_ml_video_mode_get_current(fs_ml_video_mode *mode);
int fs_ml_video_create_window(const char *title);

void fs_ml_set_quit_function(fs_ml_void_function function);

void fs_ml_video_set_update_function(fs_ml_int_function function);
void fs_ml_video_set_render_function(fs_ml_void_function function);
void fs_ml_video_set_post_render_function(fs_ml_void_function function);

//void fs_ml_signal_buffer_swap();
//void fs_ml_lock_buffer_swap();
//void fs_ml_unlock_buffer_swap();

void fs_ml_frame_update_begin(int frame);
void fs_ml_frame_update_end(int frame);

void fs_ml_video_screenshot(const char *path);
int fs_ml_video_width();
int fs_ml_video_height();
void fs_ml_video_sync_enable(int enable);
void fs_ml_vblank_sync_enable();
void fs_ml_set_video_fsaa(int fsaa);

int fs_ml_get_fullscreen_width();
int fs_ml_get_fullscreen_height();
int fs_ml_get_windowed_width();
int fs_ml_get_windowed_height();

char *fs_ml_input_unique_device_name(char *name);
char *fs_ml_input_fix_joystick_name(const char *name, int upper);

//int fs_ml_main_iteration();
// FIXME: REMOVE?
//void fs_ml_swap_buffers();
//void fs_ml_wait_vblank();
void fs_ml_video_swap_buffers();

int fs_ml_get_max_texture_size();

typedef enum {
       FS_ML_NOEVENT = 0,
       FS_ML_ACTIVEEVENT,
       FS_ML_KEYDOWN,
       FS_ML_KEYUP,
       FS_ML_MOUSEMOTION,
       FS_ML_MOUSEBUTTONDOWN,
       FS_ML_MOUSEBUTTONUP,
       FS_ML_JOYAXISMOTION,
       FS_ML_JOYBALLMOTION,
       FS_ML_JOYHATMOTION,
       FS_ML_JOYBUTTONDOWN,
       FS_ML_JOYBUTTONUP,
       FS_ML_QUIT,
       FS_ML_SYSWMEVENT,
       FS_ML_EVENT_RESERVEDA,
       FS_ML_EVENT_RESERVEDB,
       FS_ML_VIDEORESIZE,
       FS_ML_VIDEOEXPOSE,
       FS_ML_TEXTINPUT,
       FS_ML_EVENT_RESERVED3,
       FS_ML_EVENT_RESERVED4,
       FS_ML_EVENT_RESERVED5,
       FS_ML_EVENT_RESERVED6,
       FS_ML_EVENT_RESERVED7,
       FS_ML_USEREVENT = 24,
       FS_ML_NUMEVENTS = 32
} FS_ML_EventType;

typedef struct fs_ml_keysym {
    uint16_t scancode;
    uint16_t sym;
    // uint16_t unicode;
    uint16_t mod;
} fs_ml_keysym;

typedef struct fs_ml_ActiveEvent {
    uint8_t type;
    uint8_t gain;
    uint8_t state;
} fs_ml_ActiveEvent;

typedef struct fs_ml_KeyboardEvent {
    uint8_t type;
    uint8_t which;
    uint8_t state;
    fs_ml_keysym keysym;
} fs_ml_KeyboardEvent;

typedef struct fs_ml_MouseMotionEvent {
    uint8_t type;
    uint8_t device;
    uint8_t state;
    uint16_t x, y;
    int16_t xrel;
    int16_t yrel;
} fs_ml_MouseMotionEvent;

typedef struct fs_ml_MouseButtonEvent {
    uint8_t type;
    uint8_t device;
    uint8_t button;
    uint8_t state;
    uint16_t x, y;
} fs_ml_MouseButtonEvent;

typedef struct fs_ml_JoyAxisEvent {
    uint8_t type;
    uint8_t which;
    uint8_t axis;
    int16_t value;
} fs_ml_JoyAxisEvent;

typedef struct fs_ml_JoyBallEvent {
    uint8_t type;
    uint8_t which;
    uint8_t ball;
    int16_t xrel;
    int16_t yrel;
} fs_ml_JoyBallEvent;

typedef struct fs_ml_JoyHatEvent {
    uint8_t type;
    uint8_t which;
    uint8_t hat;
    uint8_t value;
} fs_ml_JoyHatEvent;

typedef struct fs_ml_JoyButtonEvent {
    uint8_t type;
    uint8_t which;
    uint8_t button;
    uint8_t state;
} fs_ml_JoyButtonEvent;

typedef struct fs_ml_ResizeEvent {
    uint8_t type;
    int w;
    int h;
} fs_ml_ResizeEvent;

typedef struct fs_ml_ExposeEvent {
    uint8_t type;
} fs_ml_ExposeEvent;

typedef struct fs_ml_QuitEvent {
    uint8_t type;
} fs_ml_QuitEvent;

typedef struct fs_ml_UserEvent {
    uint8_t type;
    int code;
    void *data1;
    void *data2;
} fs_ml_UserEvent;

struct fs_ml_SysWMmsg;
typedef struct fs_ml_SysWMmsg fs_ml_SysWMmsg;
typedef struct fs_ml_SysWMEvent {
    uint8_t type;
    fs_ml_SysWMmsg *msg;
} fs_ml_SysWMEvent;


#define TEXTINPUTEVENT_TEXT_SIZE (32)

typedef struct fs_ml_TextInputEvent {
    uint8_t type;
    char text[TEXTINPUTEVENT_TEXT_SIZE];
} fs_ml_TextInputEvent;

typedef union fs_ml_event {
    uint8_t type;
    fs_ml_ActiveEvent active;
    fs_ml_KeyboardEvent key;
    fs_ml_MouseMotionEvent motion;
    fs_ml_MouseButtonEvent button;
    fs_ml_JoyAxisEvent jaxis;
    fs_ml_JoyBallEvent jball;
    fs_ml_JoyHatEvent jhat;
    fs_ml_JoyButtonEvent jbutton;
    fs_ml_ResizeEvent resize;
    fs_ml_ExposeEvent expose;
    fs_ml_QuitEvent quit;
    fs_ml_UserEvent user;
    fs_ml_SysWMEvent syswm;
    fs_ml_TextInputEvent text;
} fs_ml_event;

fs_ml_event* fs_ml_alloc_event();
void fs_ml_free_event(fs_ml_event* event);
int fs_ml_post_event(fs_ml_event* event);

typedef int (*fs_ml_input_function)(fs_ml_event* event);
void fs_ml_set_input_function(fs_ml_input_function function);

#define FS_ML_INPUT_DEVICES_MAX 64

#define FS_ML_KEYBOARD 0
#define FS_ML_MOUSE 1
#define FS_ML_JOYSTICK 2

typedef struct fs_ml_input_device {
    int index;
    int type;
    char *name;
    char *alias;
    int buttons;
    int axes;
    int hats;
    int usage;
    int balls;
} fs_ml_input_device;

int fs_ml_input_device_count();
int fs_ml_input_device_get(int index, fs_ml_input_device *device);
fs_ml_input_device *fs_ml_get_input_devices(int* count);

#define FS_ML_HAT_CENTERED        0x00
#define FS_ML_HAT_UP              0x01
#define FS_ML_HAT_RIGHT           0x02
#define FS_ML_HAT_DOWN            0x04
#define FS_ML_HAT_LEFT            0x08
#define FS_ML_HAT_RIGHTUP         (FS_ML_HAT_RIGHT | FS_ML_HAT_UP)
#define FS_ML_HAT_RIGHTDOWN       (FS_ML_HAT_RIGHT | FS_ML_HAT_DOWN)
#define FS_ML_HAT_LEFTUP          (FS_ML_HAT_LEFT | FS_ML_HAT_UP)
#define FS_ML_HAT_LEFTDOWN        (FS_ML_HAT_LEFT | FS_ML_HAT_DOWN)

#define FS_ML_BUTTON_LEFT      1
#define FS_ML_BUTTON_MIDDLE    2
#define FS_ML_BUTTON_RIGHT     3
#define FS_ML_BUTTON_WHEELUP   4
#define FS_ML_BUTTON_WHEELDOWN 5

#define FS_ML_NO_ABSOLUTE_MOUSE_POS 0xffff

typedef enum {
    FS_ML_KEY_UNKNOWN      = 0,
    FS_ML_KEY_FIRST        = 0,
    FS_ML_KEY_BACKSPACE    = 8,
    FS_ML_KEY_TAB          = 9,
    FS_ML_KEY_CLEAR        = 12,
    FS_ML_KEY_RETURN       = 13,
    FS_ML_KEY_PAUSE        = 19,
    FS_ML_KEY_ESCAPE       = 27,
    FS_ML_KEY_SPACE        = 32,
    FS_ML_KEY_EXCLAIM      = 33,
    FS_ML_KEY_QUOTEDBL     = 34,
    FS_ML_KEY_HASH         = 35,
    FS_ML_KEY_DOLLAR       = 36,
    FS_ML_KEY_AMPERSAND    = 38,
    FS_ML_KEY_QUOTE        = 39,
    FS_ML_KEY_LEFTPAREN    = 40,
    FS_ML_KEY_RIGHTPAREN   = 41,
    FS_ML_KEY_ASTERISK     = 42,
    FS_ML_KEY_PLUS         = 43,
    FS_ML_KEY_COMMA        = 44,
    FS_ML_KEY_MINUS        = 45,
    FS_ML_KEY_PERIOD       = 46,
    FS_ML_KEY_SLASH        = 47,
    FS_ML_KEY_0            = 48,
    FS_ML_KEY_1            = 49,
    FS_ML_KEY_2            = 50,
    FS_ML_KEY_3            = 51,
    FS_ML_KEY_4            = 52,
    FS_ML_KEY_5            = 53,
    FS_ML_KEY_6            = 54,
    FS_ML_KEY_7            = 55,
    FS_ML_KEY_8            = 56,
    FS_ML_KEY_9            = 57,
    FS_ML_KEY_COLON        = 58,
    FS_ML_KEY_SEMICOLON    = 59,
    FS_ML_KEY_LESS         = 60,
    FS_ML_KEY_EQUALS       = 61,
    FS_ML_KEY_GREATER      = 62,
    FS_ML_KEY_QUESTION     = 63,
    FS_ML_KEY_AT           = 64,

    FS_ML_KEY_LEFTBRACKET  = 91,
    FS_ML_KEY_BACKSLASH    = 92,
    FS_ML_KEY_RIGHTBRACKET = 93,
    FS_ML_KEY_CARET        = 94,
    FS_ML_KEY_UNDERSCORE   = 95,
    FS_ML_KEY_BACKQUOTE    = 96,
    FS_ML_KEY_A            = 97,
    FS_ML_KEY_B            = 98,
    FS_ML_KEY_C            = 99,
    FS_ML_KEY_D            = 100,
    FS_ML_KEY_E            = 101,
    FS_ML_KEY_F            = 102,
    FS_ML_KEY_G            = 103,
    FS_ML_KEY_H            = 104,
    FS_ML_KEY_I            = 105,
    FS_ML_KEY_J            = 106,
    FS_ML_KEY_K            = 107,
    FS_ML_KEY_L            = 108,
    FS_ML_KEY_M            = 109,
    FS_ML_KEY_N            = 110,
    FS_ML_KEY_O            = 111,
    FS_ML_KEY_P            = 112,
    FS_ML_KEY_Q            = 113,
    FS_ML_KEY_R            = 114,
    FS_ML_KEY_S            = 115,
    FS_ML_KEY_T            = 116,
    FS_ML_KEY_U            = 117,
    FS_ML_KEY_V            = 118,
    FS_ML_KEY_W            = 119,
    FS_ML_KEY_X            = 120,
    FS_ML_KEY_Y            = 121,
    FS_ML_KEY_Z            = 122,
    FS_ML_KEY_DELETE       = 127,

    FS_ML_KEY_WORLD_0      = 160,
    FS_ML_KEY_WORLD_1      = 161,
    FS_ML_KEY_WORLD_2      = 162,
    FS_ML_KEY_WORLD_3      = 163,
    FS_ML_KEY_WORLD_4      = 164,
    FS_ML_KEY_WORLD_5      = 165,
    FS_ML_KEY_WORLD_6      = 166,
    FS_ML_KEY_WORLD_7      = 167,
    FS_ML_KEY_WORLD_8      = 168,
    FS_ML_KEY_WORLD_9      = 169,
    FS_ML_KEY_WORLD_10     = 170,
    FS_ML_KEY_WORLD_11     = 171,
    FS_ML_KEY_WORLD_12     = 172,
    FS_ML_KEY_WORLD_13     = 173,
    FS_ML_KEY_WORLD_14     = 174,
    FS_ML_KEY_WORLD_15     = 175,
    FS_ML_KEY_WORLD_16     = 176,
    FS_ML_KEY_WORLD_17     = 177,
    FS_ML_KEY_WORLD_18     = 178,
    FS_ML_KEY_WORLD_19     = 179,
    FS_ML_KEY_WORLD_20     = 180,
    FS_ML_KEY_WORLD_21     = 181,
    FS_ML_KEY_WORLD_22     = 182,
    FS_ML_KEY_WORLD_23     = 183,
    FS_ML_KEY_WORLD_24     = 184,
    FS_ML_KEY_WORLD_25     = 185,
    FS_ML_KEY_WORLD_26     = 186,
    FS_ML_KEY_WORLD_27     = 187,
    FS_ML_KEY_WORLD_28     = 188,
    FS_ML_KEY_WORLD_29     = 189,
    FS_ML_KEY_WORLD_30     = 190,
    FS_ML_KEY_WORLD_31     = 191,
    FS_ML_KEY_WORLD_32     = 192,
    FS_ML_KEY_WORLD_33     = 193,
    FS_ML_KEY_WORLD_34     = 194,
    FS_ML_KEY_WORLD_35     = 195,
    FS_ML_KEY_WORLD_36     = 196,
    FS_ML_KEY_WORLD_37     = 197,
    FS_ML_KEY_WORLD_38     = 198,
    FS_ML_KEY_WORLD_39     = 199,
    FS_ML_KEY_WORLD_40     = 200,
    FS_ML_KEY_WORLD_41     = 201,
    FS_ML_KEY_WORLD_42     = 202,
    FS_ML_KEY_WORLD_43     = 203,
    FS_ML_KEY_WORLD_44     = 204,
    FS_ML_KEY_WORLD_45     = 205,
    FS_ML_KEY_WORLD_46     = 206,
    FS_ML_KEY_WORLD_47     = 207,
    FS_ML_KEY_WORLD_48     = 208,
    FS_ML_KEY_WORLD_49     = 209,
    FS_ML_KEY_WORLD_50     = 210,
    FS_ML_KEY_WORLD_51     = 211,
    FS_ML_KEY_WORLD_52     = 212,
    FS_ML_KEY_WORLD_53     = 213,
    FS_ML_KEY_WORLD_54     = 214,
    FS_ML_KEY_WORLD_55     = 215,
    FS_ML_KEY_WORLD_56     = 216,
    FS_ML_KEY_WORLD_57     = 217,
    FS_ML_KEY_WORLD_58     = 218,
    FS_ML_KEY_WORLD_59     = 219,
    FS_ML_KEY_WORLD_60     = 220,
    FS_ML_KEY_WORLD_61     = 221,
    FS_ML_KEY_WORLD_62     = 222,
    FS_ML_KEY_WORLD_63     = 223,
    FS_ML_KEY_WORLD_64     = 224,
    FS_ML_KEY_WORLD_65     = 225,
    FS_ML_KEY_WORLD_66     = 226,
    FS_ML_KEY_WORLD_67     = 227,
    FS_ML_KEY_WORLD_68     = 228,
    FS_ML_KEY_WORLD_69     = 229,
    FS_ML_KEY_WORLD_70     = 230,
    FS_ML_KEY_WORLD_71     = 231,
    FS_ML_KEY_WORLD_72     = 232,
    FS_ML_KEY_WORLD_73     = 233,
    FS_ML_KEY_WORLD_74     = 234,
    FS_ML_KEY_WORLD_75     = 235,
    FS_ML_KEY_WORLD_76     = 236,
    FS_ML_KEY_WORLD_77     = 237,
    FS_ML_KEY_WORLD_78     = 238,
    FS_ML_KEY_WORLD_79     = 239,
    FS_ML_KEY_WORLD_80     = 240,
    FS_ML_KEY_WORLD_81     = 241,
    FS_ML_KEY_WORLD_82     = 242,
    FS_ML_KEY_WORLD_83     = 243,
    FS_ML_KEY_WORLD_84     = 244,
    FS_ML_KEY_WORLD_85     = 245,
    FS_ML_KEY_WORLD_86     = 246,
    FS_ML_KEY_WORLD_87     = 247,
    FS_ML_KEY_WORLD_88     = 248,
    FS_ML_KEY_WORLD_89     = 249,
    FS_ML_KEY_WORLD_90     = 250,
    FS_ML_KEY_WORLD_91     = 251,
    FS_ML_KEY_WORLD_92     = 252,
    FS_ML_KEY_WORLD_93     = 253,
    FS_ML_KEY_WORLD_94     = 254,
    FS_ML_KEY_WORLD_95     = 255,

    FS_ML_KEY_KP0          = 256,
    FS_ML_KEY_KP1          = 257,
    FS_ML_KEY_KP2          = 258,
    FS_ML_KEY_KP3          = 259,
    FS_ML_KEY_KP4          = 260,
    FS_ML_KEY_KP5          = 261,
    FS_ML_KEY_KP6          = 262,
    FS_ML_KEY_KP7          = 263,
    FS_ML_KEY_KP8          = 264,
    FS_ML_KEY_KP9          = 265,
    FS_ML_KEY_KP_PERIOD    = 266,
    FS_ML_KEY_KP_DIVIDE    = 267,
    FS_ML_KEY_KP_MULTIPLY  = 268,
    FS_ML_KEY_KP_MINUS     = 269,
    FS_ML_KEY_KP_PLUS      = 270,
    FS_ML_KEY_KP_ENTER     = 271,
    FS_ML_KEY_KP_EQUALS    = 272,

    FS_ML_KEY_UP           = 273,
    FS_ML_KEY_DOWN         = 274,
    FS_ML_KEY_RIGHT        = 275,
    FS_ML_KEY_LEFT         = 276,
    FS_ML_KEY_INSERT       = 277,
    FS_ML_KEY_HOME         = 278,
    FS_ML_KEY_END          = 279,
    FS_ML_KEY_PAGEUP       = 280,
    FS_ML_KEY_PAGEDOWN     = 281,

    FS_ML_KEY_F1           = 282,
    FS_ML_KEY_F2           = 283,
    FS_ML_KEY_F3           = 284,
    FS_ML_KEY_F4           = 285,
    FS_ML_KEY_F5           = 286,
    FS_ML_KEY_F6           = 287,
    FS_ML_KEY_F7           = 288,
    FS_ML_KEY_F8           = 289,
    FS_ML_KEY_F9           = 290,
    FS_ML_KEY_F10          = 291,
    FS_ML_KEY_F11          = 292,
    FS_ML_KEY_F12          = 293,
    FS_ML_KEY_F13          = 294,
    FS_ML_KEY_F14          = 295,
    FS_ML_KEY_F15          = 296,

    FS_ML_KEY_NUMLOCK      = 300,
    FS_ML_KEY_CAPSLOCK     = 301,
    FS_ML_KEY_SCROLLOCK    = 302,
    FS_ML_KEY_RSHIFT       = 303,
    FS_ML_KEY_LSHIFT       = 304,
    FS_ML_KEY_RCTRL        = 305,
    FS_ML_KEY_LCTRL        = 306,
    FS_ML_KEY_RALT         = 307,
    FS_ML_KEY_LALT         = 308,
#if 0
    FS_ML_KEY_RMETA        = 309,
    FS_ML_KEY_LMETA        = 310,
#endif
    FS_ML_KEY_LSUPER       = 311,
    FS_ML_KEY_RSUPER       = 312,
    FS_ML_KEY_MODE         = 313,
    FS_ML_KEY_COMPOSE      = 314,
    FS_ML_KEY_HELP         = 315,
    FS_ML_KEY_PRINT        = 316,
    FS_ML_KEY_SYSREQ       = 317,
    FS_ML_KEY_BREAK        = 318,
    FS_ML_KEY_MENU         = 319,
    FS_ML_KEY_POWER        = 320,
    FS_ML_KEY_EURO         = 321,
    FS_ML_KEY_UNDO         = 322,

    FS_ML_KEY_NEW_SECTION,
    FS_ML_KEY_VOLUMEUP,
    FS_ML_KEY_VOLUMEDOWN,
    FS_ML_KEY_MUTE,
    FS_ML_KEY_AUDIONEXT,
    FS_ML_KEY_AUDIOPREV,
    FS_ML_KEY_AUDIOSTOP,
    FS_ML_KEY_AUDIOPLAY,

    FS_ML_KEY_LAST
} fs_ml_key;

typedef enum {
    FS_ML_KEY_MOD_NONE     = 0x0000,
    FS_ML_KEY_MOD_LSHIFT   = 0x0001,
    FS_ML_KEY_MOD_RSHIFT   = 0x0002,

    FS_ML_KEY_MOD_F11      = 0x0004,
    FS_ML_KEY_MOD_F12      = 0x0010,
    FS_ML_KEY_MOD_SPECIAL  = 0x0020,

    FS_ML_KEY_MOD_LCTRL    = 0x0040,
    FS_ML_KEY_MOD_RCTRL    = 0x0080,
    FS_ML_KEY_MOD_LALT     = 0x0100,
    FS_ML_KEY_MOD_RALT     = 0x0200,
    FS_ML_KEY_MOD_LMETA    = 0x0400,
    FS_ML_KEY_MOD_RMETA    = 0x0800,
    FS_ML_KEY_MOD_NUM      = 0x1000,
    FS_ML_KEY_MOD_CAPS     = 0x2000,
    FS_ML_KEY_MOD_MODE     = 0x4000,
    FS_ML_KEY_MOD_RESERVED = 0x8000
} fs_ml_key_mod;

#define FS_ML_KEY_MOD_CTRL   (FS_ML_KEY_MOD_LCTRL | FS_ML_KEY_MOD_RCTRL)
#define FS_ML_KEY_MOD_SHIFT  (FS_ML_KEY_MOD_LSHIFT | FS_ML_KEY_MOD_RSHIFT)
#define FS_ML_KEY_MOD_ALT    (FS_ML_KEY_MOD_LALT | FS_ML_KEY_MOD_RALT)
#define FS_ML_KEY_MOD_META   (FS_ML_KEY_MOD_LMETA | FS_ML_KEY_MOD_RMETA)

#endif // LIBFSML_ML_H_
