#include <uae/uae.h>

#ifdef USE_SDL
// we must include SDL first before emu.h, so libfsemu's #definition of main
// is the current one (on Windows) when main is encountered further down
#include <SDL/SDL.h>
#endif

#include <fs/base.h>
#include <fs/emu.h>
#include <fs/i18n.h>
#include <fs/string.h>
#include <fs/thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "fs-uae.h"

//static char *g_default_rom_dir = ".";

static int fs_uae_argc;
static char **fs_uae_argv;

#define LOG_LINE "---------------------------------------------------------" \
        "-------------------\n"

void change_port_device_mode(int data) {
    int modes = INPUTEVENT_AMIGA_JOYPORT_MODE_0_LAST -
            INPUTEVENT_AMIGA_JOYPORT_MODE_0_NONE + 1;
    int port = data / modes;
    int mode = data % modes;
    if (port >= 0 && port < FS_UAE_NUM_INPUT_PORTS) {
        g_fs_uae_input_ports[port].mode = mode;
        g_fs_uae_input_ports[port].new_mode = mode;
        amiga_set_joystick_port_mode(port, mode);
        fs_uae_reconfigure_input_ports_host();
        fs_emu_menu_update_current();
    }
}

void select_port_0_device(int data) {
    printf("--> device index %d\n", data);
    int port = 0;
    if (data == 9) {
        // 9 is currently a hack to indicate the local mouse
        g_fs_uae_input_ports[port].mode = AMIGA_JOYPORT_MOUSE;
        g_fs_uae_input_ports[port].new_mode = AMIGA_JOYPORT_MOUSE;
        strcpy(g_fs_uae_input_ports[port].device, "MOUSE");
        amiga_set_joystick_port_mode(port, AMIGA_JOYPORT_MOUSE);
        // FIXME: not a warning, rather a notification
        fs_emu_warning(_("Port 0: %s"), _("Mouse"));
    }
    else {
        int count = 0;
        int new_mode = new_mode = AMIGA_JOYPORT_DJOY;
        if (g_fs_uae_amiga_model == MODEL_CD32) {
            new_mode = AMIGA_JOYPORT_CD32JOY;
        }
        fs_emu_input_device *devices = fs_emu_get_input_devices(&count);
        if (data < count) {
            g_fs_uae_input_ports[port].mode = new_mode;
            g_fs_uae_input_ports[port].new_mode = new_mode;
            strcpy(g_fs_uae_input_ports[port].device, devices[data].name);
            amiga_set_joystick_port_mode(port, new_mode);
            // FIXME: not a warning, rather a notification
            fs_emu_warning(_("Port 0: %s"), devices[data].name);
        }
    }
    fs_uae_reconfigure_input_ports_host();
    fs_emu_menu_update_current();

    //fs_emu_get_input_devices()
    //g_fs_uae_input_ports[port].mode = mode;
    //amiga_set_joystick_port_mode(port,  mode);
    //g_fs_uae_input_ports[port].new_mode = mode;
    //fs_uae_reconfigure_input_ports_host();
    //fs_emu_menu_update_current();
}

int event_handler_loop(void) {
    int action;
    //int reconfigure_input = 0;
    while((action = fs_emu_get_input_event()) != 0) {
        //printf("event_handler_loop received input action %d\n", action);
        int istate = (action & 0x00ff0000) >> 16;
        // force to -128 to 127 range
        signed char state = (signed char) istate;
        action = action & 0x0000ffff;
        //amiga_keyboard_set_host_key(input_event, state);

        if (action >= INPUTEVENT_AMIGA_JOYPORT_MODE_0_NONE &&
                action < INPUTEVENT_AMIGA_JOYPORT_MODE_3_LAST) {
            change_port_device_mode(
                    action - INPUTEVENT_AMIGA_JOYPORT_MODE_0_NONE);

        }
        else if (action >= INPUTEVENT_AMIGA_JOYPORT_0_DEVICE_0 &&
                action < INPUTEVENT_AMIGA_JOYPORT_0_DEVICE_LAST) {
            select_port_0_device(action - INPUTEVENT_AMIGA_JOYPORT_0_DEVICE_0);
        }
        else {
            if (state && action >= INPUTEVENT_AMIGA_JOYPORT_0_AUTOFIRE &&
                    action <= INPUTEVENT_AMIGA_JOYPORT_3_AUTOFIRE) {
                int port = action - INPUTEVENT_AMIGA_JOYPORT_0_AUTOFIRE;
                if (g_fs_uae_input_ports[port].autofire_mode) {
                    g_fs_uae_input_ports[port].autofire_mode = 0;
                    amiga_set_joystick_port_autofire(port, 0);
                    fs_emu_warning(_("Auto-fire disabled for port %d"), port);
                }
                else {
                    g_fs_uae_input_ports[port].autofire_mode = 1;
                    amiga_set_joystick_port_autofire(port, 1);
                    fs_emu_warning(_("Auto-fire enabled for port %d"), port);
                }
                fs_emu_menu_update_current();
            }
            amiga_send_input_event(action, state);
        }
    }
    return 1;
}

static void pause_throttle() {
    /*
    if (fs_emu_get_vblank_sync()) {
        return;
    }
    */
    fs_emu_msleep(5);
}

void event_handler(void) {
    //static int busy = 0;
    //static int idle = 0;
    //static int64_t last_time = 0;
    static int frame = 0;
    frame = frame + 1;
    //printf("event_handler frame=%d\n", frame);

    /*
    int64_t t = fs_emu_monotonic_time();
    if (last_time > 0) {
        int dt = (t - last_time) / 1000;
        printf("%d\n", dt);
    }
    */

    fs_emu_wait_for_frame(frame);
    if (frame == 1) {
        // we configure input ports after first frame are confirmed,
        // because otherwise configure events would get lost if initially
        // connected to the server (for net play game), but aborted connection
        // before game started
        fs_uae_reconfigure_input_ports_amiga();
    }

    if (fs_emu_is_quitting()) {
        fs_log("calling amiga_quit\n");
        amiga_quit();
        return;
    }
    event_handler_loop();
    while (fs_emu_is_paused()) {
        if (!event_handler_loop()) {
            break;
        }
        pause_throttle();
        if (fs_emu_is_quitting()) {
            break;
        }
    }

    //last_time = fs_emu_monotonic_time();

}

char *g_fs_uae_config_file_path = NULL;
char *g_fs_uae_config_dir_path = NULL;
//GKeyFile *g_fs_uae_config = NULL;

static int audio_callback_function(int type, int16_t *buffer, int size) {
    if (type == 0) {
        return fs_emu_queue_audio_buffer(0, buffer, size);
    }
    else if (type == 1) {
        fs_emu_audio_pause_stream(0);
        return 0;
    }
    else if (type == 2) {
        fs_emu_audio_resume_stream(0);
        return 0;
    }
    else if (type == 3) {
        // cd audio stream
        if (buffer == NULL) {
            // check status of buffer number given by size
            return fs_emu_check_audio_buffer_done(1, size);
        }
        return fs_emu_queue_audio_buffer(1, buffer, size);
    }
    return -1;
}

void fs_uae_load_rom_files(const char *path) {
    fs_log("fs_uae_load_rom_files %s\n", path);
    fs_dir *dir = fs_dir_open(path, 0);
    if (dir == NULL) {
        fs_log("error opening dir\n");
    }

    amiga_add_key_dir(path);
    const char *name = fs_dir_read_name(dir);
    while (name) {
        char *lname = fs_utf8_strdown(name, -1);
        if (fs_str_has_suffix(lname, ".rom")) {
            fs_log("found file \"%s\"\n", name);
            char *full_path = fs_path_join(path, name, NULL);
            amiga_add_rom_file(full_path);
        }
        free(lname);
        name = fs_dir_read_name(dir);
    }
    fs_dir_close(dir);
    //exit(1);
}

char *fs_uae_encode_path(const char* path) {
    // FIXME: libamiga now always accepts UTF-8, so this function is
    // deprecated. Simply returning a duplicate now.
    return fs_strdup(path);
/*
#ifdef WINDOWS
    return g_locale_from_utf8(path, -1, NULL, NULL, NULL);
#else
    return fs_strdup(path);
#endif
*/
}

char *fs_uae_decode_path(const char* path) {
    // FIXME: libamiga now always accepts UTF-8, so this function is
    // deprecated. Simply returning a duplicate now.
    return fs_strdup(path);
/*
#ifdef WINDOWS
    return g_locale_to_utf8(path, -1, NULL, NULL, NULL);
#else
    return fs_strdup(path);
#endif
*/
}

static void on_init() {
    fs_log("\n");
    fs_log(LOG_LINE);
    fs_log("uae configuration\n");
    fs_log(LOG_LINE);
    fs_log("\n");

    //fs_uae_configure_amiga_model();
    fs_uae_configure_amiga_hardware();
    fs_uae_configure_floppies();
    fs_uae_configure_cdrom();
    fs_uae_configure_hard_drives();
    fs_uae_configure_input();
    fs_uae_configure_directories();

    /*
    if (fs_emu_get_video_sync()) {
        fs_log("fs_emu_get_video_sync returned true\n");
        amiga_set_option("gfx_vsync", "true");
    }
    else {
        fs_log("fs_emu_get_video_sync returned false\n");
    }
    if (fs_emu_netplay_enabled()) {
        fs_log("netplay is enabled\n");
        // make sure UAE does not sleep between frames, we must be able
        // to control sleep times for net play
        amiga_set_option("gfx_vsync", "true");
    }
    */

    // with sound_auto set to true, UAE stops audio output if the amiga does
    // not produce sound, but this just confuses libfsemu which expects
    // continuous output
    //amiga_set_option("sound_auto", "false");
    //amiga_set_audio_frequency(fs_emu_get_audio_frequency());

    //amiga_set_audio_frequency(22050);


    // set the input frequency to the output frequency, since we configured
    // libamiga to output at the same frequency

    // FIXME: check the actual frequency libuae/libamiga outputs, seems
    // to output at 44100 Hz even though currprefs.freq says 48000.
    //fs_emu_set_audio_buffer_frequency(0, fs_emu_get_audio_frequency());

    //amiga_set_option("gfx_gamma", "40");

    fs_uae_set_uae_paths();
    fs_uae_read_custom_uae_options(fs_uae_argc, fs_uae_argv);

    char *uae_file;

    uae_file = fs_path_join(fs_uae_logs_dir(), "LastConfig.uae", NULL);
    if (fs_path_exists(uae_file)) {
        fs_unlink(uae_file);
    }
    free(uae_file);

    uae_file = fs_path_join(fs_uae_logs_dir(), "DebugConfig.uae", NULL);
    amiga_write_uae_config(uae_file);
    free(uae_file);

    fs_log("\n");
    fs_log(LOG_LINE);
    fs_log("end of uae configuration\n");
    fs_log(LOG_LINE);
    fs_log("\n");
}

void pause_function(int pause) {
    fs_log("pause_function %d\n", pause);
    //uae_pause(pause);
    amiga_pause(pause);
}

static int load_config_file() {
    fs_log("load config file\n");
    const char *msg = "checking config file %s\n";

    //g_fs_uae_config = g_key_file_new();
    if (g_fs_uae_config_file_path == NULL) {
        char *path = fs_path_join(fs_uae_exe_dir(), "Config.fs-uae",
                NULL);
        fs_log(msg, path);
        if (fs_path_exists(path)) {
            g_fs_uae_config_file_path = path;
        }
        else {
            free(path);
        }
    }
    if (g_fs_uae_config_file_path == NULL) {
        fs_log(msg, "Config.fs-uae");
        if (fs_path_exists("Config.fs-uae")) {
            g_fs_uae_config_file_path = "Config.fs-uae";
        }
    }
    if (g_fs_uae_config_file_path == NULL) {
        fs_log(msg, "fs-uae.conf");
        if (fs_path_exists("fs-uae.conf")) {
            g_fs_uae_config_file_path = "fs-uae.conf";
        }
    }
    if (g_fs_uae_config_file_path == NULL) {
        char *path = fs_path_join(fs_get_user_config_dir(),
                "fs-uae", "fs-uae.conf", NULL);
        fs_log(msg, path);
        if (fs_path_exists(path)) {
            g_fs_uae_config_file_path = path;
        }
        else {
            free(path);
        }
    }
    if (g_fs_uae_config_file_path == NULL) {
        char *path = fs_path_join(fs_uae_configurations_dir(),
                "Default.fs-uae", NULL);
        fs_log(msg, path);
        if (fs_path_exists(path)) {
            g_fs_uae_config_file_path = path;
        }
        else {
            free(path);
        }
    }
    if (g_fs_uae_config_file_path) {
        fs_log("loading config from %s\n", g_fs_uae_config_file_path);
        fs_config_read_file(g_fs_uae_config_file_path, 0);
        g_fs_uae_config_dir_path = fs_path_get_dirname(
                g_fs_uae_config_file_path);
    }
    else {
        if (fs_config_get_boolean("end_config") == 1) {
            // do not warn in case end_config was specified via argv
        }
        else {
            fs_log("No configuration file was found");
            fs_emu_warning(_("No configuration file was found"));
        }
    }

    char *path = fs_path_join(fs_uae_configurations_dir(),
            "Host.fs-uae", NULL);
    fs_log(msg, path);
    if (fs_path_exists(path)) {
        fs_config_read_file(path, 0);
        free(path);
    }

    return 0;
}

static void log_to_libfsemu(const char *message) {
    fs_log_string(message);
}

static void main_function() {
    amiga_main();
    fs_log("amiga_main returned\n");
}

void init_i18n() {
    if (fs_config_get_boolean("localization") == 0) {
        fs_log("localization was forced off\n");
        return;
    }
    char *locale = setlocale(LC_MESSAGES, "");
    if (locale) {
        fs_log("locale is set to %s\n", locale);
    }
    else {
        fs_log("failed to set current locale\n");
    }
#ifndef ANDROID
    textdomain("fs-uae");
    char *path = fs_get_data_file("fs-uae/share-dir");
    if (path) {
        fs_log("using data dir \"%s\"\n", path);
        // remove "fs-uae/share-dir" from the returned path
        int len = strlen(path);
        if (len > 16) {
            path[len - 16] = '\0';
        }
        char *locale_base = fs_path_join(path, "locale", NULL);
        fs_log("using locale dir \"%s\"\n", locale_base);
        bindtextdomain("fs-uae", locale_base);
        free(locale_base);
        free(path);
    }
    bind_textdomain_codeset("fs-uae", "UTF-8");
#endif
}

static void led_function(int led, int state) {
    // floppy led status is custom overlay 0..3
    //if (led >= 0) {
    //    printf("led %d state %d\n", led, state);
    //}
    fs_emu_set_custom_overlay_state(led, state);
}

static void media_function(int drive, const char *path) {
    // media insertion status is custom overlay 4..7
    fs_emu_set_custom_overlay_state(4 + drive, path && path[0]);
}

void list_joysticks() {
    printf("# FS-UAE VERSION %s\n", g_fs_uae_version);
    printf("# listing joysticks\n");
#ifdef USE_SDL
    if (SDL_Init(SDL_INIT_JOYSTICK ) < 0) {
        printf("# SDL_Init(SDL_INIT_JOYSTICK ) < 0\n");
        return;
    }
    printf("# SDL_NumJoysticks(): %d\n", SDL_NumJoysticks());
    for(int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_JoystickName(i)[0] == '\0') {
            printf("Unnamed\n");
        }
        else {
            printf("%s\n", SDL_JoystickName(i));
        }
    }
#else
    printf("# USE_SDL is not defined\n");
#endif
    printf("# listing joysticks done\n");
}

static const char *overlay_names[] = {
    "df0_led",
    "df1_led",
    "df2_led",
    "df3_led",
    "df0_disk",
    "df1_disk",
    "df2_disk",
    "df3_disk",
    "power_led",
    "hd_led",
    "cd_led",
    "md_led",
    NULL,
};

#define COPYRIGHT_NOTICE "\nFS-UAE VERSION %s\n" \
"Copyright 1995-2002 Bernd Schmidt, 1999-2012 Toni Wilen,\n" \
"2003-2007 Richard Drummond, 2006-2011 Mustafa 'GnoStiC' Tufan,\n" \
"2011-2013 Frode Solheim, and contributors.\n" \
"\n" \
"This is free software; see the file COPYING for copying conditions. There\n" \
"is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR\n" \
"PURPOSE. See the README for more copyright info, and the source code for\n" \
"a full list of contributors\n\n"

int main(int argc, char* argv[]) {
    int result;
    fs_uae_argc = argc;
    fs_uae_argv = argv;

    fs_set_argv(argc, argv);

    char **arg;
    arg = argv + 1;
    while (arg && *arg) {
        if (strcmp(*arg, "--list-joysticks") == 0) {
            list_joysticks();
            exit(0);
        }
        arg++;
    }

    fs_init();

    fs_set_prgname("fs-uae");
    fs_set_application_name("Amiga Emulator");
    amiga_set_log_function(log_to_libfsemu);

    //result = parse_options(argc, argv);

    printf(COPYRIGHT_NOTICE, g_fs_uae_version);
    fs_log(COPYRIGHT_NOTICE, g_fs_uae_version);

    char *current_dir = fs_get_current_dir();
    fs_log("current directory is %s\n", current_dir);
    free(current_dir);

    amiga_init();

    // skip first entry
    arg = argv + 1;
    if (g_fs_uae_config_file_path == NULL) {
        while (arg && *arg) {
            if (fs_path_exists(*arg)) {
                g_fs_uae_config_file_path = fs_strdup(*arg);
            }
            arg++;
        }
    }

    // parse options first, in case base_dir, logging  options etc is
    // specified on the command line
    fs_config_parse_options(argc - 1, argv + 1);

    fs_log("\n");
    fs_log(LOG_LINE);
    fs_log("libfsemu init\n");
    fs_log(LOG_LINE);
    fs_log("\n");

    init_i18n();

    fs_emu_init_overlays(overlay_names);
    fs_emu_init();

    // then load the config file
    load_config_file();
    // parse options again, overwriting options loaded from configuration
    // file
    //fs_config_parse_options(argc - 1, argv + 1);

    fs_log("\n");
    fs_log(LOG_LINE);
    fs_log("fs-uae init\n");
    fs_log(LOG_LINE);
    fs_log("\n");

    fs_emu_set_state_check_function(amiga_get_state_checksum);
    fs_emu_set_rand_check_function(amiga_get_rand_checksum);

    // force creation of some recommended default directories
    fs_uae_kickstarts_dir();
    fs_uae_configurations_dir();
    fs_uae_init_path_resolver();

    // must be called early, before fs_emu_init -affects video output
    fs_uae_configure_amiga_model();

    if (g_fs_uae_fastest_possible) {
        //fs_log("fastest possible mode - disabling frame throttling\n");
        //fs_emu_disable_throttling();
        fs_log("fastest possible mode - disallowing full sync\n");
        //fs_emu_disallow_full_sync();
    }

    // force creation of state directories
    //fs_uae_flash_memory_dir();
    //fs_uae_save_states_dir();
    //fs_uae_floppy_overlays_dir();
    fs_uae_state_dir();

    const char *controllers_dir = fs_uae_controllers_dir();
    if (controllers_dir) {
        fs_emu_set_controllers_dir(controllers_dir);
    }
    const char *logs_dir = fs_uae_logs_dir();
    if (logs_dir) {
        char *log_file;

        log_file = fs_path_join(logs_dir, "FS-UAE.log", NULL);
        if (fs_path_exists(log_file)) {
            fs_unlink(log_file);
        }
        free(log_file);

        log_file = fs_path_join(logs_dir, "FS-UAE.log.txt", NULL);
        fs_config_set_log_file(log_file);
        free(log_file);
    }

    fs_config_set_string_if_unset("themes_dir", fs_uae_themes_dir());

    fs_emu_set_pause_function(pause_function);

    //fs_uae_init_input();
    fs_emu_init_2(FS_EMU_INIT_EVERYTHING);
    if (fs_emu_netplay_enabled()) {
        amiga_enable_netplay_mode();
    }

    if (logs_dir) {
        if (fs_emu_netplay_enabled()) {
            char *sync_log_file = fs_path_join(logs_dir,
                    "Synchronization.log", NULL);
            amiga_set_synchronization_log_file(sync_log_file);
            free(sync_log_file);
        }
    }

    // this stream is for paula output and drive clicks
    // FIXME: could mix drive clicks in its own stream instead, -might
    // give higher quality mixing
    fs_emu_audio_stream_options options;
    options.struct_size = sizeof(fs_emu_audio_stream_options);
    fs_emu_init_audio_stream_options(&options);
    fs_emu_init_audio_stream(0, &options);
    amiga_set_audio_buffer_size(options.buffer_size);

    // this stream is for CD audio output (CDTV/CD32)
    fs_emu_init_audio_stream_options(&options);
    // 12 * 2352 is CDDA_BUFFERS * 2352 (blkdev_cdimage.cpp)
    options.buffer_size = 12 * 2352;
    // begin playing with only one buffer queued
    options.min_buffers = 1;
    fs_emu_init_audio_stream(1, &options);

    amiga_set_audio_callback(audio_callback_function);
    amiga_set_cd_audio_callback(audio_callback_function);

    //amiga_set_cd_audio_callback(audio_callback_function);
    amiga_set_event_function(event_handler);
    amiga_set_led_function(led_function);
    amiga_set_media_function(media_function);
    amiga_set_init_function(on_init);

#ifdef WITH_LUA
    amiga_init_lua(fs_emu_acquire_lua, fs_emu_release_lua);
    amiga_init_lua_state(fs_emu_get_lua_state());
#endif

    if (fs_emu_get_video_format() == FS_EMU_VIDEO_FORMAT_RGBA) {
        amiga_set_video_format(AMIGA_VIDEO_FORMAT_RGBA);
    }
    else if (fs_emu_get_video_format() == FS_EMU_VIDEO_FORMAT_BGRA) {
        amiga_set_video_format(AMIGA_VIDEO_FORMAT_BGRA);
    }
    else if (fs_emu_get_video_format() == FS_EMU_VIDEO_FORMAT_R5G6B5) {
        amiga_set_video_format(AMIGA_VIDEO_FORMAT_R5G6B5);
    }
    else if (fs_emu_get_video_format() == FS_EMU_VIDEO_FORMAT_R5G5B5A1) {
        amiga_set_video_format(AMIGA_VIDEO_FORMAT_R5G5B5A1);
    }
    else {
        fs_emu_warning("Unsupported video format requested");
    }
    amiga_add_rtg_resolution(672, 540);
    amiga_add_rtg_resolution(672 * 2, 540 * 2);
    amiga_add_rtg_resolution(fs_emu_get_windowed_width(),
            fs_emu_get_windowed_height());
    amiga_add_rtg_resolution(fs_emu_get_fullscreen_width(),
            fs_emu_get_fullscreen_height());
    fs_uae_init_video();

    //fs_uae_init_keyboard();
    //fs_uae_init_mouse();
    fs_uae_configure_menu();

    fs_emu_run(main_function);
    fs_log("fs-uae shutting down, fs_emu_run returned\n");
    if (fs_rmdir(fs_uae_state_dir()) == 0) {
        fs_log("state dir %s was removed because it was empty\n",
                fs_uae_state_dir());
    }
    else {
        fs_log("state dir %s was not removed (non-empty)\n",
                fs_uae_state_dir());
    }
    fs_log("end of main function\n");
    return 0;
}
