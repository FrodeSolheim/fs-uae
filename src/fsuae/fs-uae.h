#ifndef FS_UAE_FS_UAE_H
#define FS_UAE_FS_UAE_H

#include <fs/emu.h>

#ifndef MAX_PATH
#ifdef WINDOWS
#define MAX_PATH 512
#else
#define MAX_PATH 1024
#endif
#endif

#ifdef FSUAE_LEGACY
void fs_uae_configure_menu();
#endif

void fs_uae_configure_input();
const char *fs_uae_get_state_base_name();
void fs_uae_read_custom_uae_options(int argc, char **argv);

int get_grab_input(void);
void set_grab_input(int mode);

void fs_uae_configure_actions();
void fs_uae_init_video(void);
void fs_uae_init_zoom_mode(void);
void fs_uae_init_audio(void);
void fs_uae_map_keyboard(void);
void fs_uae_handle_keyboard_event(fs_emu_event *event);

void fs_uae_init_mouse(void);
void fs_uae_handle_mouse_event(fs_emu_event *event);

extern int g_fs_uae_video_aspect;
extern int g_fs_uae_video_zoom;

char *fs_uae_encode_path(const char *path);
char *fs_uae_decode_path(const char *path);

void fs_uae_init_configs();
void fs_uae_configure_directories();
void fs_uae_configure_amiga_hardware();
void fs_uae_configure_amiga_model();
void fs_uae_configure_mouse(void);
void fs_uae_load_rom_files(const char *path);

#define FS_UAE_CONFIG_TYPE_JOYSTICK "amiga"
#define FS_UAE_CONFIG_TYPE_MOUSE "amiga_mouse"

#define FS_UAE_NUM_INPUT_PORTS 8
#define MAX_DEVICE_NAME_LEN 128
typedef struct fs_uae_input_port {
    char device[MAX_DEVICE_NAME_LEN + 1];
    int mode;
    int new_mode;
    int autofire_mode;
    int new_autofire_mode;
    const char *config_type;
} fs_uae_input_port;

extern fs_uae_input_port g_fs_uae_input_ports[FS_UAE_NUM_INPUT_PORTS];
void fs_uae_read_override_actions_for_port(int port);

void fs_uae_reconfigure_input_ports_amiga();
void fs_uae_reconfigure_input_ports_host();

#define FS_UAE_FLOPPY_PATHS 0
#define FS_UAE_HD_PATHS 1
#define FS_UAE_CD_PATHS 2
#define FS_UAE_ROM_PATHS 3
#define FS_UAE_DIR_PATHS 4

// extern char *g_fs_uae_default_dir;
extern char *g_fs_uae_config_file_path;
extern char *g_fs_uae_config_dir_path;

extern int g_fs_uae_state_number;
extern int g_fs_uae_last_input_event;
extern int g_fs_uae_last_input_event_state;

void fs_uae_process_input_event(int line, int action, int state, int playback);

#ifdef WITH_LUA
void fs_uae_init_lua_state(lua_State *L);
#endif

void fs_uae_toggle_auto_zoom(void);

extern int g_fs_uae_frame;

#include <uae/uae.h>

enum {
    FS_UAE_ACTION_DUMMY = INPUTEVENT_SPC_LAST + 1,
#if 0
    FS_UAE_ACTION_DRIVE_0_BROWSE = INPUTEVENT_SPC_LAST + 1,
    FS_UAE_ACTION_DRIVE_1_BROWSE,
    FS_UAE_ACTION_DRIVE_2_BROWSE,
    FS_UAE_ACTION_DRIVE_3_BROWSE,
#endif
};

#include <fs/emu/actions.h>

enum {
    FS_UAE_ACTION_FIRST = FS_EMU_ACTION_LOCAL,

    FS_UAE_ACTION_ENTER_DEBUGGER,
    FS_UAE_ACTION_TOGGLE_AUTO_ZOOM,
    FS_UAE_ACTION_DRIVE_0_BROWSE,
    FS_UAE_ACTION_DRIVE_1_BROWSE,
    FS_UAE_ACTION_DRIVE_2_BROWSE,
    FS_UAE_ACTION_DRIVE_3_BROWSE,
};

#endif  // FS_UAE_FS_UAE_H
