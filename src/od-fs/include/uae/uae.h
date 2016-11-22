#ifndef LIBAMIGA_LIBAMIGA_H_
#define LIBAMIGA_LIBAMIGA_H_

#include <stdint.h>
#include "uae/string.h"

#ifdef __cplusplus
extern "C" {
#endif

void uae_clipboard_update(void);

/* Sets uaem metadata write flags based on chars in flags. You only need
 * to call this function to set non-default behavior. */
void uae_set_uaem_write_flags_from_string(const char *flags);

int amiga_get_vsync_counter();
void amiga_set_vsync_counter(int vsync_counter);

typedef void (uae_callback_function)(void *data);
// old name
typedef void (amiga_callback_function)(void *data);
void amiga_on_save_state_finished(uae_callback_function *function);
void amiga_on_restore_state_finished(uae_callback_function *function);

#ifdef WITH_LUA
#include <lauxlib.h>
void amiga_init_lua(void (*lock)(void), void (*unlock)(void));
void amiga_init_lua_state(lua_State *L);
#endif

#define AMIGA_FLOPPY_LIST_SIZE 20

// FIXME
#define amiga_log_warning printf

#define AMIGA_VIDEO_FORMAT_RGBA 0
#define AMIGA_VIDEO_FORMAT_BGRA 1
#define AMIGA_VIDEO_FORMAT_R5G6B5 2
#define AMIGA_VIDEO_FORMAT_R5G5B5A1 3

int amiga_init(void);
bool amiga_init_jit_compiler(void);

int amiga_set_synchronization_log_file(const char *path);
int amiga_quickstart(int model, int config, int accuracy);
//int amiga_main(int argc, char** argv);

// must call before amiga_main
void amiga_set_video_format(int format);

// must be called before main_main
void amiga_add_rtg_resolution(int width, int height);

// must be called early
void amiga_set_builtin_driveclick_path(const char *path);
// for custom floppy sound files
void amiga_set_floppy_sounds_dir(const char *path);

void amiga_main(void);
int amiga_reset(int hard);
int amiga_pause(int pause);
int amiga_cpu_get_speed();
int amiga_cpu_set_speed(int speed);

void amiga_set_deterministic_mode();

void amiga_set_save_state_compression(int compress);

int amiga_enable_serial_port(const char *serial_name);
int amiga_enable_parallel_port(const char *parallel_name);

void amiga_set_save_image_dir(const char *path);
void amiga_set_module_ripper_dir(const char *path);

int amiga_set_min_first_line(int line, int ntsc);

enum {
    AMIGA_FLOPPY_DRIVE_NONE,
    AMIGA_FLOPPY_DRIVE_35_DD,
    AMIGA_FLOPPY_DRIVE_35_HD,
    AMIGA_FLOPPY_DRIVE_DRIVE_525_SD ,
    AMIGA_FLOPPY_DRIVE_DRIVE_35_DD_ESCOM,
};

int amiga_get_rand_checksum();
int amiga_get_state_checksum(void);
int amiga_get_state_checksum_and_dump(void *data, int size);

void amiga_floppy_set_writable_images(int writable);
const char *amiga_floppy_get_file(int index);
const char *amiga_floppy_get_list_entry(int index);

int amiga_floppy_set_file(int index, const char *file);

int amiga_get_num_cdrom_drives();
int amiga_get_num_floppy_drives();

void amiga_cdrom_eject(int drive);
const char *amiga_cdrom_get_file(int index);
int amiga_cdrom_set_file(int index, const char *file);

int amiga_floppy_get_drive_type(int index);
int amiga_floppy_get_speed();
int amiga_floppy_set_speed(int speed);

// FIMXE: REMOVE
int amiga_floppy_set_from_list(int drive, int index);

int amiga_state_save(int slot);

int amiga_state_load(int slot);

int amiga_quit();

void amiga_set_render_buffer(void *data, int size, int need_redraw,
        void *(*grow)(int width, int height));

#define AMIGA_MAX_LINES 2048

#define AMIGA_VIDEO_RTG_MODE 1
#define AMIGA_VIDEO_LOW_RESOLUTION 2
#define AMIGA_VIDEO_LINE_DOUBLING 4

#define UAE_LED_STATE_ON 1
#define UAE_LED_STATE_WRITE 2
#define UAE_LED_STATE_EXTRA 4

typedef struct _uae_led_data {
    // int df[4];
    int df_t0[4];
    int df_t1[4];
    // int cd;
    // int hd;
    // int md;
    // int power;
} amiga_led_data;

extern struct _uae_led_data g_uae_led_data;

void amiga_on_update_leds(uae_callback_function *function);

typedef struct _RenderData {
    unsigned char* pixels;
    int width;
    int height;
    int limit_x;
    int limit_y;
    int limit_w;
    int limit_h;
    //int updated;
    char line[AMIGA_MAX_LINES];
    int flags;
    void *(*grow)(int width, int height);
    double refresh_rate;
    int bpp;
} RenderData;

typedef void (*event_function)(int);
void amiga_set_event_function(event_function function);

typedef void (*init_function)(void);
void amiga_set_init_function(init_function function);

typedef void (*render_function)(RenderData *rd);
void amiga_set_render_function(render_function function);

typedef void (*display_function)();
void amiga_set_display_function(display_function function);

typedef void (*log_function)(const char *msg);
void amiga_set_log_function(log_function function);
void amiga_set_gui_message_function(log_function function);

typedef void (*amiga_led_function)(int led, int on);
void amiga_set_led_function(amiga_led_function function);

typedef void (*amiga_media_function)(int drive, const char *path);
void amiga_set_media_function(amiga_media_function function);

typedef const char * (*amiga_plugin_lookup_function)(const char *name);
void amiga_set_plugin_lookup_function(amiga_plugin_lookup_function function);

typedef int (*audio_callback)(int type, int16_t *buffer, int size);
int amiga_set_audio_callback(audio_callback func);
int amiga_set_cd_audio_callback(audio_callback func);

int amiga_set_audio_buffer_size(int size);
int amiga_set_audio_frequency(int frequency);

int amiga_set_option(const char *option, const char *value);

typedef void (*amiga_free_function)(void* data);
int amiga_set_option_and_free(const char *option, char *value,
    amiga_free_function free_function);

int amiga_set_hardware_option(const char *option, const char *value);
int amiga_set_int_option(const char *option, int value);

void amiga_map_cd_drives(int enable);
void amiga_set_cpu_idle(int idle);
void amiga_write_uae_config(const char *path);

//void amiga_mouse_set_button(int mouse, int button, int state);
//void amiga_mouse_set_axis(int mouse, int axis, int rel, int abs);
//void amiga_mouse_set_axis(int mouse, int axis, int value, int is_abs);

//void amiga_keyboard_set_host_key(int keycode, int state);
//void amiga_joystick_set_button(int joystick, int button, int state);
//void amiga_joystick_set_axis(int joystick, int axis, double value);


#define AMIGA_JOYPORT_NONE 0
#define AMIGA_JOYPORT_MOUSE 1
#define AMIGA_JOYPORT_DJOY 2
#define AMIGA_JOYPORT_GAMEPAD 3
#define AMIGA_JOYPORT_AJOY 4
#define AMIGA_JOYPORT_CDTVJOY 5
#define AMIGA_JOYPORT_CD32JOY 6
#define AMIGA_JOYPORT_LIGHTPEN 7

/*
int amiga_get_joystick_port_mode(int port);
*/
void amiga_set_joystick_port_mode(int port, int mode);
void amiga_set_joystick_port_autofire(int port, int autofire);
void amiga_enable_auto_mouse_mode(bool enable);

int amiga_send_input_event(int input_event, int state);

void amiga_write_config(const char *path);

void amiga_add_key_dir(const char *path);
int amiga_add_rom_file(const char *path, const char *cache_path);

void amiga_set_paths(const char **rom_paths, const char **floppy_paths,
        const char **cd_paths, const char **hd_paths);
void amiga_set_native_library_dirs(const char **library_dirs);

#ifdef __cplusplus
} // extern "C"
#endif

#ifndef UAE_FS_H

// This section must only be included by external code and cannot be used
// by od-fs code.

#define DEFEVENT(A, B, C, D, E, F) INPUTEVENT_ ## A,
#define DEFEVENT2(A, B, B2, C, D, E, F, G) INPUTEVENT_ ## A,
enum inputevents {
INPUTEVENT_ZERO,
#include "uae_inputevents_def.h"
INPUTEVENT_END
};
#undef DEFEVENT

#endif

#endif // LIBAMIGA_LIBAMIGA_H_
