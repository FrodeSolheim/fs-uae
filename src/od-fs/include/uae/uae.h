#ifndef LIBAMIGA_LIBAMIGA_H_
#define LIBAMIGA_LIBAMIGA_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AMIGA_FLOPPY_LIST_SIZE 20

// FIXME
#define amiga_log_warning printf

#define AMIGA_VIDEO_FORMAT_RGBA 0
#define AMIGA_VIDEO_FORMAT_BGRA 1
#define AMIGA_VIDEO_FORMAT_R5G6B5 2
#define AMIGA_VIDEO_FORMAT_R5G5B5A1 3

int amiga_init();
int amiga_set_synchronization_log_file(const char *path);
int amiga_quickstart(int model, int config, int accuracy);
//int amiga_main(int argc, char** argv);

// must call before amiga_main
void amiga_set_video_format(int format);

// must be called before main_main
void amiga_add_rtg_resolution(int width, int height);

void amiga_main();
int amiga_reset(int hard);
int amiga_pause(int pause);
int amiga_cpu_get_speed();
int amiga_cpu_set_speed(int speed);

void amiga_enable_netplay_mode();

int amiga_enable_serial_port(const char *serial_name);

void amiga_set_save_image_dir(const char *path);

enum {
    AMIGA_FLOPPY_DRIVE_NONE,
    AMIGA_FLOPPY_DRIVE_35_DD,
    AMIGA_FLOPPY_DRIVE_35_HD,
    AMIGA_FLOPPY_DRIVE_DRIVE_525_SD ,
    AMIGA_FLOPPY_DRIVE_DRIVE_35_DD_ESCOM,
};

int amiga_get_rand_checksum();
int amiga_get_state_checksum();

void amiga_floppy_set_writable_images(int writable);
const char *amiga_floppy_get_file(int index);
const char *amiga_floppy_get_list_entry(int index);

int amiga_floppy_set_file(int index, const char *file);

int amiga_get_num_cdrom_drives();
int amiga_get_num_floppy_drives();
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

#define AMIGA_RTG_BUFFER_FLAG 1

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
} RenderData;

typedef void (*event_function)(void);
typedef void (*init_function)(void);
typedef void (*render_function)(RenderData *rd);
typedef void (*display_function)();
typedef void (*log_function)(const char *msg);
typedef void (*amiga_led_function)(int led, int on);
typedef void (*amiga_media_function)(int drive, const char *path);

void amiga_set_event_function(event_function function);
void amiga_set_init_function(init_function function);
void amiga_set_render_function(render_function function);
void amiga_set_display_function(display_function function);
void amiga_set_log_function(log_function function);
void amiga_set_gui_message_function(log_function function);
void amiga_set_led_function(amiga_led_function function);
void amiga_set_media_function(amiga_media_function function);

typedef int (*audio_callback)(int16_t *buffer, int size);
int amiga_set_audio_callback(audio_callback func);
int amiga_set_cd_audio_callback(audio_callback func);
int amiga_set_audio_buffer_size(int size);
int amiga_set_audio_frequency(int frequency);

int amiga_set_option(const char *option, const char *value);
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

int amiga_send_input_event(int input_event, int state);

void amiga_write_config(const char *path);

void amiga_add_key_dir(const char *path);
int amiga_add_rom_file(const char *path);

void amiga_set_paths(const char **rom_paths, const char **floppy_paths,
        const char **cd_paths, const char **hd_paths);

#ifdef __cplusplus
} // extern "C"
#endif

#ifndef LIBAMIGA_INTERNAL_H_

#define DEFEVENT(A, B, C, D, E, F) INPUTEVENT_ ## A,
enum inputevents {
INPUTEVENT_ZERO,
#include "uae_inputevents_def.h"
INPUTEVENT_END
};
#undef DEFEVENT

#endif

#endif // LIBAMIGA_LIBAMIGA_H_
