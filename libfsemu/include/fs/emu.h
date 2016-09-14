#ifndef LIBFSEMU_EMU_H_
#define LIBFSEMU_EMU_H_

#include <fs/malloc.h>
#include <fs/fs.h>
#include <fs/ref.h>
#include <fs/log.h>
#include <fs/conf.h>
#include <fs/ml.h>

#if 0

#define USE_GLIB
//#undef WITH_PORTAUDIO

#include <stdint.h>
#ifdef USE_GLIB
#include <glib.h>
#endif
#endif

#include <stdbool.h>

//#ifdef MACOSX
// SDL.h must be included in the compilation unit containing main
// on Mac OS X
//#include <SDL.h>
//#endif

//#ifdef WITH_SDL
//#include <SDL.h>
//#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WITH_LUA
#include <lauxlib.h>
lua_State *fs_emu_get_lua_state(void);
void fs_emu_acquire_lua(void);
void fs_emu_release_lua(void);
#endif

void fs_emu_lua_run_handler(const char *name);

// Can (or should) be called before fs_emu_init

#define fs_emu_log fs_log
#define fs_emu_log_string fs_log_string

typedef void (*fs_emu_simple_function)();

void fs_emu_set_quit_function(fs_emu_simple_function function);

void fs_emu_set_controllers_dir(const char *path);

// initialize libfsemu

void fs_emu_init_overlays(const char **overlay_names);
void fs_emu_init(void);

#define FS_EMU_INIT_VIDEO 1
#define FS_EMU_INIT_AUDIO 2
#define FS_EMU_INIT_INPUT 4
#define FS_EMU_INIT_EVERYTHING 0xffffffff
void fs_emu_init_2(int options);

// ----------

typedef void (*fs_emu_zoom_function)(int);
void fs_emu_set_toggle_zoom_function(fs_emu_zoom_function function);
void fs_emu_toggle_zoom(int flags);

void fs_emu_notification(uint32_t type, const char *format, ...);

void fs_emu_warning(const char *format, ...);
void fs_emu_deprecated(const char *format, ...);

//void fs_emu_warning(const char* warning);

// FIXME: REMOVE
//extern GKeyFile *g_fs_emu_config;

//inline GKeyFile *fs_emu_get_config() {
    //return g_fs_emu_config;
//}
//void fs_emu_set_config(GKeyFile *config);

void fs_emu_reset_input_mapping();
void fs_emu_map_custom_actions();

typedef int (*fs_emu_hotkey_function)(int key_code, int key_mod);
void fs_emu_set_hotkey_function(fs_emu_hotkey_function handler);

typedef struct fs_emu_rect {
    int x;
    int y;
    int w;
    int h;
} fs_emu_rect;

typedef struct fs_emu_input_mapping {
    const char *name;
    int action;
    //int paired_action;
} fs_emu_input_mapping;

typedef struct fs_emu_key_translation {
    int system_key;
    int system_mod;
    int input_event;
} fs_emu_key_translation;

int fs_emu_get_input_event();
void fs_emu_queue_action(int action, int state);

void fs_emu_set_custom_overlay_state(int overlay, int state);

void fs_emu_set_keyboard_translation(fs_emu_key_translation *keymap);

/** Unused (do not remove) */
#define FS_EMU_NOEVENT FS_ML_NOEVENT

/** Application loses/gains visibility */
#define FS_EMU_ACTIVEEVENT FS_ML_ACTIVEEVENT

/** Keys pressed */
#define FS_EMU_KEYDOWN FS_ML_KEYDOWN

/** Keys released */
#define FS_EMU_KEYUP FS_ML_KEYUP

/** Mouse moved */
#define FS_EMU_MOUSEMOTION FS_ML_MOUSEMOTION

/** Mouse button pressed */
#define FS_EMU_MOUSEBUTTONDOWN FS_ML_MOUSEBUTTONDOWN

/** Mouse button released */
#define FS_EMU_MOUSEBUTTONUP FS_ML_MOUSEBUTTONUP

/** Joystick axis motion */
#define FS_EMU_JOYAXISMOTION FS_ML_JOYAXISMOTION

/** Joystick trackball motion */
#define FS_EMU_JOYBALLMOTION FS_ML_JOYBALLMOTION

/** Joystick hat position change */
#define FS_EMU_JOYHATMOTION FS_ML_JOYHATMOTION

/** Joystick button pressed */
#define FS_EMU_JOYBUTTONDOWN FS_ML_JOYBUTTONDOWN

/** Joystick button released */
#define FS_EMU_JOYBUTTONUP FS_ML_JOYBUTTONUP

/** User-requested quit */
#define FS_EMU_QUIT FS_ML_QUIT

/** System specific event */
#define FS_EMU_SYSWMEVENT FS_ML_SYSWMEVENT

/** User resized video mode */
#define FS_EMU_VIDEORESIZE FS_ML_VIDEORESIZE

/** Screen needs to be redrawn */
#define FS_EMU_VIDEOEXPOSE FS_ML_VIDEOEXPOSE

/** Events SDL_USEREVENT through SDL_MAXEVENTS-1 are for your use */
#define FS_EMU_USEREVENT FS_ML_USEREVENT

typedef fs_ml_event fs_emu_event;

//int fs_emu_get_event(fs_emu_event *event);

int fs_emu_process_event(fs_emu_event *event);

int fs_emu_process_key_event(int key_code, int key_mod, int state);

typedef fs_ml_input_device fs_emu_input_device;
fs_emu_input_device *fs_emu_get_input_devices(int* count);
//void fs_emu_register_input_devices_usage(int usage);

//typedef void (*fs_emu_action_function)(int action, int state);
//void fs_emu_set_action_function(fs_emu_action_function function);

int fs_emu_configure_joystick(
    const char *name, const char *type, fs_emu_input_mapping *mapping,
        int usage, char *out_name, int out_name_len, bool reuse);

void fs_emu_configure_mouse(const char *name, int horiz, int vert, int left,
        int middle, int right, int wheel_axis);


typedef int (*fs_emu_checksum_function)(void);
void fs_emu_set_state_check_function(fs_emu_checksum_function function);
void fs_emu_set_rand_check_function(fs_emu_checksum_function function);

// high level and generic functions

void fs_emu_msleep(int msec);

static inline int64_t fs_emu_monotonic_time() {
    return fs_get_monotonic_time();
}

//void fs_emu_log(const char *format, ...);

void fs_emu_fatal(const char *msg);

//const char *fs_emu_get_share_dir();

typedef void (*fs_emu_main_function)();
int fs_emu_run(fs_emu_main_function function);

typedef void (*fs_emu_pause_function)(int pause);
void fs_emu_set_pause_function(fs_emu_pause_function function);

void fs_emu_pause(int pause);

int fs_emu_is_paused();

void fs_emu_quit();
int fs_emu_thread_running();
int fs_emu_is_quitting();

void fs_emu_disable_throttling();
void fs_emu_disallow_full_sync();

void fs_emu_hud_add_console_line(const char *text, int flags);

int fs_emu_wait_for_frame(int frame);

// net play

void fs_emu_netplay_say(const char *text);
void fs_emu_hud_add_chat_message(const char *text, const char *player);

int fs_emu_netplay_enabled();
int fs_emu_netplay_connected();
void fs_emu_netplay_disconnect();
const char *fs_emu_get_netplay_tag(int player);
int fs_emu_send_netplay_message(const char *text);

// video related functions


//void fs_emu_set_application_title(const char *title);
const char *fs_emu_get_title();
void fs_emu_set_title(const char *title);
const char *fs_emu_get_sub_title();
void fs_emu_set_sub_title(const char *title);

void fs_emu_toggle_fullscreen(void);

double fs_emu_get_average_emu_fps();
double fs_emu_get_average_sys_fps();

// video interface

double fs_emu_get_video_frame_rate(void);
/**
 * Specify the frame rate for emulated video (typically 50 or 60).
 */
void fs_emu_set_video_frame_rate(double frame_rate);

int fs_emu_video_get_aspect_correction();
/**
 * Show with original aspect ratio or stretch to fill the screen.
 */
void fs_emu_video_set_aspect_correction(int correct);
/**
 * Specify the pixel aspect ratio of the emulated video frames.
 */
void fs_emu_set_pixel_aspect_ratio(double ratio);
/**
 * Check whether screen drawing is synced to the host display.
 *
 * This is not the same as fs_emu_get_video_vsync!
 */
static inline int fs_emu_get_vblank_sync() {
    return fs_ml_get_vblank_sync();
}
/**
 * Check whether emulated video is synced to the host display.
 */
static inline int fs_emu_get_video_sync() {
    return fs_ml_get_video_sync();
}

void fs_emu_update_video(const void *frame, int width, int height, int bpp,
        fs_emu_rect *crop);

struct fs_emu_video_data;

//typedef void (*fs_emu_video_raw_function)(unsigned char *output,
//        const unsigned char *input, void *user_data);
typedef void (*fs_emu_video_raw_function)(struct fs_emu_video_data *data);

typedef struct fs_emu_video_data {
    int seq_no;
    uint8_t *data;
    int width;
    int height;
    int bpp;
    fs_emu_rect crop;
    //uint8_t *raw_data;
    //uint8_t *raw_info;
    fs_emu_video_raw_function raw_function;
} fs_emu_video_data;

//void fs_emu_video_raw_data(const void *data, int size,
//        fs_emu_video_raw_function function, void *user_data);
void fs_emu_update_video_with_raw_data(const void *frame, int width,
        int height, int bpp, fs_emu_rect *crop,
        fs_emu_video_raw_function function, int size);
//void fs_emu_update_video_with_raw_data(const void *data,
//        int width, int height, int bpp, int size,
//fs_emu_video_raw_function function, void *user_data);


//void fs_emu_video_frame_finish();

void fs_emu_video_render_synchronous();

#define FS_EMU_MAX_LINES 2048

#define FS_EMU_NO_SCANLINES_FLAG 1
#define FS_EMU_FORCE_VIEWPORT_CROP_FLAG 2
#define FS_EMU_FILL_BORDER_FLAG 4

typedef struct fs_emu_video_buffer {
    int seq;
    void *data;
    int width;
    int height;
    int bpp;
    double aspect;
    int size;
    fs_emu_rect crop;
    //int buffer_width;
    //int buffer_height;
    char line[FS_EMU_MAX_LINES];
    int flags;
} fs_emu_video_buffer;

int fs_emu_video_buffer_init(int width, int height, int bpp);
fs_emu_video_buffer *fs_emu_video_buffer_get_available(int copy);
void fs_emu_video_buffer_update_lines(fs_emu_video_buffer *buffer);
void fs_emu_video_buffer_set_current(fs_emu_video_buffer *buffer);
fs_emu_video_buffer *fs_emu_video_buffer_get_current();
int fs_emu_video_buffer_grow(fs_emu_video_buffer *buffer, int width,
        int height);

// audio interface

#include <fs/emu/audio.h>

bool fs_emu_mouse_integration(void);
void fs_emu_show_cursor(int show);
void fs_emu_show_cursor_msec(int duration);
int fs_emu_is_cursor_visible(void);
bool fs_emu_cursor_allowed(void);
int64_t fs_emu_cursor_is_visible_to(void);

bool fs_emu_input_grab(void);
void fs_emu_set_input_grab(bool grab);
void fs_emu_set_input_grab_and_visibility(bool grab, int duration);

void fs_emu_screenshot(const char *path, int crop);

#define FS_EMU_MENU_ITEM_TYPE_ITEM 0
#define FS_EMU_MENU_ITEM_TYPE_HEADING 1
#define FS_EMU_MENU_ITEM_TYPE_MENU 2

#define FS_EMU_MENU_RESULT_NONE 0
#define FS_EMU_MENU_RESULT_MENU 1
#define FS_EMU_MENU_RESULT_CLOSE 2
#define FS_EMU_MENU_RESULT_UPDATE 4
#define FS_EMU_MENU_RESULT_BACK 8
#define FS_EMU_MENU_RESULT_ROOT 16

// how long pointer is visible after moving mouse (in ms)
#define FS_EMU_MOUSE_DEFAULT_DURATION 1000

// menu support functions

#define FS_EMU_MENU_MAX_ITEMS 100

typedef struct fs_emu_menu_item {
    struct fs_ref ref;
    int type;
    char *title;
    int disabled;
    int idata;
    void *pdata;
    char *screen_path;
    int (*activate)(struct fs_emu_menu_item* menu_item, void **result);
} fs_emu_menu_item;

typedef struct fs_emu_menu {
    struct fs_ref ref;
    int index;
    fs_emu_menu_item *items[FS_EMU_MENU_MAX_ITEMS];
    int count;
    void (*update)(struct fs_emu_menu* menu);
    int idata;
} fs_emu_menu;

bool fs_emu_menu_mode(void);
void fs_emu_set_menu_mode(bool mode);

static inline void fs_emu_menu_toggle(void)
{
    fs_emu_set_menu_mode(!fs_emu_menu_mode());
}

void fs_emu_menu_update_current();

void fs_emu_menu_set_current(fs_emu_menu *menu);

fs_emu_menu *fs_emu_menu_new();

void fs_emu_menu_set_update_function(fs_emu_menu *menu,
        void (*function)(fs_emu_menu *menu));

fs_emu_menu_item *fs_emu_menu_item_new();

void fs_emu_menu_append_item(fs_emu_menu *menu,
        fs_emu_menu_item *item);

fs_emu_menu_item *fs_emu_menu_item_at(fs_emu_menu *menu, int index);

void fs_emu_menu_item_set_type(fs_emu_menu_item *item, int type);

void fs_emu_menu_item_set_enabled(fs_emu_menu_item *item, int enabled);

void fs_emu_menu_item_set_title(fs_emu_menu_item *item, const char* title);

int fs_emu_menu_item_get_idata(fs_emu_menu_item *item);

void *fs_emu_menu_item_get_pdata(fs_emu_menu_item *item);

void fs_emu_menu_item_set_idata(fs_emu_menu_item *item, int idata);

void fs_emu_menu_item_set_pdata(fs_emu_menu_item *item, void *pdata);

void fs_emu_menu_item_set_activate_function(fs_emu_menu_item *item,
        int (*function)(fs_emu_menu_item *menu_item, void **data));

void fs_emu_acquire_gui_lock();
void fs_emu_assert_gui_lock();
void fs_emu_release_gui_lock();

#include <fs/emu/dialog.h>
#include <fs/emu/video.h>

#ifdef __cplusplus
}
#endif

#endif // LIBFSEMU_EMU_H_
