#define FSUAE_INTERNAL
#include "fsuae-main.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define _GNU_SOURCE 1
#include <uae/uae.h>
#ifdef USE_SDL
/* We must include SDL first before emu.h, so libfsemu's #definition of main
 * is the current one (on Windows) when main is encountered further down. */
#include <SDL.h>
#endif
#ifdef MACOSX
#include <SDL.h>
#endif
#include <fs/base.h>
#include <fs/data.h>
#include <fs/emu.h>
#include <fs/emu/audio.h>
#include <fs/emu/path.h>
#include <fs/emu/video.h>
#include <fs/glib.h>
#include <fs/i18n.h>
#include <fs/lazyness.h>
#include <fs/main.h>
#include <fs/thread.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "fs-uae.h"
#include "fsuae-input.h"
#include "fsuae-media.h"
#include "fsuae-options.h"
#include "fsuae-oskeyboard.h"
#include "fsuae-path.h"
#include "fsuae-plugins.h"
#include "fsuae-recording.h"
#include "fsuae-savestate.h"
#include "fsuae.h"
#ifdef WITH_CEF
#include <fs/emu/cef.h>
#endif
#include <fs/emu/hacks.h>
#ifdef USE_NFD
#include "nfd.h"
#endif

#include "fsemu-all.h"
#include "fsemu-audio.h"
#include "fsemu-gamemode.h"
#include "fsemu-gui.h"
#include "fsemu-helpbar.h"
#include "fsemu-input.h"
#include "fsemu-inputport.h"
#include "fsemu-leds.h"
#include "fsemu-main.h"
#include "fsemu-option.h"
#include "fsemu-quit.h"
#include "fsemu-time.h"
#include "fsemu-util.h"
#include "fsemu-video.h"
#include "fsemu-window.h"

#ifdef LINUX
#include "../../fsemu/gamemode/lib/gamemode_client.h"
#endif

int fsuae_log_level = FSEMU_LOG_LEVEL_INFO;

int fsemu = 0;

static int fs_uae_argc;
static char **fs_uae_argv;
static int g_warn_about_missing_config_file;

#define LOG_LINE                                                              \
    "-----------------------------------------------------------------------" \
    "-----\n"

static void change_port_device_mode(int data)
{
#ifdef FSUAE_LEGACY
    int modes = INPUTEVENT_AMIGA_JOYPORT_MODE_0_LAST -
                INPUTEVENT_AMIGA_JOYPORT_MODE_0_NONE + 1;
    int port = data / modes;
    int mode = data % modes;
    if (port >= 0 && port < FS_UAE_NUM_INPUT_PORTS) {
        g_fs_uae_input_ports[port].mode = mode;
        g_fs_uae_input_ports[port].new_mode = mode;
        // The fifth port isn't an Amiga port, but rather a fake
        // port used with custom input mapping only
        if (port < 4) {
            amiga_set_joystick_port_mode(port, mode);
        }
        fs_uae_reconfigure_input_ports_host();
        fs_emu_menu_update_current();
    }
#endif
}

static void select_port_0_device(int data)
{
#ifdef FSUAE_LEGACY
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
    } else {
        int count = 0;
        int new_mode = AMIGA_JOYPORT_DJOY;
        if (g_fs_uae_amiga_model == MODEL_CD32) {
            new_mode = AMIGA_JOYPORT_CD32JOY;
        }
        fs_emu_input_device *devices = fs_emu_get_input_devices(&count);
        if (data < count) {
            g_fs_uae_input_ports[port].mode = new_mode;
            g_fs_uae_input_ports[port].new_mode = new_mode;
            uae_strlcpy(g_fs_uae_input_ports[port].device,
                        devices[data].name,
                        sizeof(g_fs_uae_input_ports[port].device));
            amiga_set_joystick_port_mode(port, new_mode);
            // FIXME: not a warning, rather a notification
            fs_emu_warning(_("Port 0: %s"), devices[data].name);
        }
    }
    fs_uae_reconfigure_input_ports_host();
    fs_emu_menu_update_current();

    // fs_emu_get_input_devices()
    // g_fs_uae_input_ports[port].mode = mode;
    // amiga_set_joystick_port_mode(port,  mode);
    // g_fs_uae_input_ports[port].new_mode = mode;
    // fs_uae_reconfigure_input_ports_host();
    // fs_emu_menu_update_current();
#endif
}

int g_fs_uae_last_input_event = 0;
int g_fs_uae_last_input_event_state = 0;
int g_fs_uae_state_number = 0;

static void fsuae_handle_media_action(int drive_index, int media_index)
{
    if (drive_index >= fsemu_media_drive_count()) {
        return;
    }
    fsemu_media_drive_t *drive = fsemu_media_drive_at_index(drive_index);
    int type = fsemu_media_drive_type(drive);
    int type_index = fsemu_media_drive_type_index(drive);
    if (type == FSEMU_MEDIA_DRIVE_TYPE_FLOPPY) {
        if (type_index >= 4) {
            fsemu_warning("Cannot deal with with floppy driveindex >= 4\n");
            return;
        }
        if (media_index < 0) {
            amiga_send_input_event(INPUTEVENT_SPC_EFLOPPY0 + type_index, 1);
        } else if (media_index < 20) {
            amiga_send_input_event(INPUTEVENT_SPC_DISKSWAPPER_0_0 +
                                       drive_index * 20 + media_index,
                                   1);
        } else {
            fsemu_warning("Cannot deal with media index >= 20\n");
        }
    } else if (type == FSEMU_MEDIA_DRIVE_TYPE_CDROM) {
        // Only one CD-ROM drive supported currently.
        if (type_index >= 1) {
            fsemu_warning("Cannot deal with with CD-ROM driveindex >= 1\n");
            return;
        }
        if (media_index < 0) {
            amiga_cdrom_set_file(type_index, "");
        } else if (media_index < 20) {
            char *key = g_strdup_printf("cdrom_image_%d", media_index);
            char *path = fs_config_get_string(key);
            free(key);
            if (path == NULL) {
                fs_emu_log("no CD at this index in CD-ROM list\n");
            }
            path = fsuae_path_expand_and_free(path);
            path = fsuae_path_resolve_and_free(path, FS_UAE_CD_PATHS);
            amiga_cdrom_set_file(type_index, path);
            free(path);
        } else {
            fsemu_warning("Cannot deal with media index >= 20\n");
        }

        // FIXME: Something similar to gui_disk_image_change /
        // UAE_EVENT_CD0PATH, to update the currently inserted path
    } else {
        fsemu_warning("FIXME: Support this media type\n");
    }
}

static void fsuae_handle_port_action(int port_index, int mode_index)
{
    // The first two parts are switched in FS-UAE, so the joystick port is
    // listed first.
    if (port_index < 2) {
        port_index = 1 - port_index;
    }
    amiga_set_joystick_port_mode(port_index, mode_index);
#if 0
    int modes = INPUTEVENT_AMIGA_JOYPORT_MODE_0_LAST -
                INPUTEVENT_AMIGA_JOYPORT_MODE_0_NONE + 1;
    int input_event =
        INPUTEVENT_AMIGA_JOYPORT_MODE_0_NONE + modes * port_index + mode_index;
    // INPUTEVENT_AMIGA_JOYPORT_MODE_1_NONE,
    // INPUTEVENT_AMIGA_JOYPORT_MODE_2_NONE,
    // INPUTEVENT_AMIGA_JOYPORT_MODE_3_NONE,

    if (input_event >= INPUTEVENT_AMIGA_JOYPORT_MODE_0_NONE &&
        input_event < INPUTEVENT_AMIGA_JOYPORT_MODE_3_LAST) {
        amiga_send_input_event(input_event, 1);
    } else {
        printf("WARNING: Ignoring action in fsuae_handle_port_action\n");
    }
#endif
#if 0
    if (port_index >= fsemu_media_drive_count()) {
        return;
    }
    fsemu_media_drive_t *drive = fsemu_media_drive_at_index(drive_index);
    int type = fsemu_media_drive_type(drive);
    int type_index = fsemu_media_drive_type_index(drive);
    if (type == FSEMU_MEDIA_DRIVE_TYPE_FLOPPY) {
        if (type_index >= 4) {
            fsemu_warning("Cannot deal with with floppy driveindex >= 4\n");
            return;
        }
        if (media_index < 0) {
            amiga_send_input_event(INPUTEVENT_SPC_EFLOPPY0 + type_index, 1);
        } else if (media_index < 20) {
            amiga_send_input_event(INPUTEVENT_SPC_DISKSWAPPER_0_0 +
                                       drive_index * 20 + media_index,
                                   1);
        } else {
            fsemu_warning("Cannot deal with media index >= 20\n");
        }
    } else {
        fsemu_warning("FIXME: Support this media type\n");
    }
#endif
}

void fs_uae_process_input_event(int line, int action, int state, int playback)
{
    // printf("... action %d %d\n", action, FSEMU_ACTION_DRIVE0EJECT);
    static int first_time = 1;
    if (first_time == 1) {
        first_time = 0;
        int load_state_number = fs_config_get_int("load_state");
        if (load_state_number >= 1 && load_state_number <= 9) {
            // FIXME: improvement, check if state file exists and show
            // GUI warning if not...
            fsuae_log("trying to load state number: %d\n", load_state_number);
            amiga_send_input_event(
                INPUTEVENT_SPC_STATERESTORE1 - 1 + load_state_number, 1);
        }
    }

#if 0
    g_fs_uae_last_input_event = input_event;
    g_fs_uae_last_input_event_state = state;
    fs_emu_lua_run_handler("on_fs_uae_input_event");
    // handler can modify input event
    amiga_send_input_event(g_fs_uae_last_input_event,
            g_fs_uae_last_input_event_state);
#endif

#if 0
    if (action == INPUTEVENT_KEY_RETURN) {
        printf("FIXME: ignoring RETURN event for now\n");
        return;
    }
#endif

#ifdef USE_NFD
    /*
    if (action == FS_UAE_ACTION_DRIVE_0_BROWSE && state) {
        printf("FS_UAE_ACTION_DRIVE_0_BROWSE test\n");
        nfdchar_t *out = malloc(1024);
        nfdresult_t result = NFD_OpenDialog("*", "/home", &out);
        printf("nfdresult: %d\n", result);
        if (result == NFD_OKAY) {
            printf("%s\n", out);
        }
        free(out);
    }
    */
#endif

    if (action == 0) {
    }
#if 1
    // FIXME: Handle these in the beginning of the frame only? More like
    // how state loading/saving is handled?
    else if (action == FSEMU_ACTION_SOFTRESET) {
        amiga_send_input_event(INPUTEVENT_SPC_SOFTRESET, 1);
    } else if (action == FSEMU_ACTION_HARDRESET) {
        amiga_send_input_event(INPUTEVENT_SPC_HARDRESET, 1);
    }
#endif
    else if (action >= FSEMU_ACTION_DRIVE0EJECT &&
             action <= FSEMU_ACTION_DRIVE9EJECT) {
        int drive_index = action - FSEMU_ACTION_DRIVE0EJECT;
        fsuae_log("Got drive eject event drive_index=%d\n", drive_index);
        fsuae_handle_media_action(drive_index, -1);
    } else if (action >= FSEMU_ACTION_DRIVE0INSERT0 &&
               action <= FSEMU_ACTION_DRIVE9INSERT19) {
        int offset = action - FSEMU_ACTION_DRIVE0INSERT0;
        int drive_index = offset / FSEMU_MEDIA_MAX_FILE_COUNT;
        int media_index = offset % FSEMU_MEDIA_MAX_FILE_COUNT;
        fsuae_log("Got drive insert event drive_index=%d media_index=%d\n",
                  drive_index,
                  media_index);
        fsuae_handle_media_action(drive_index, media_index);
    }

    else if (action >= FSEMU_ACTION_PORT0TYPE0 &&
             action < FSEMU_ACTION_PORT3TYPE9) {
        int offset = action - FSEMU_ACTION_PORT0TYPE0;
        int port_index = offset / FSEMU_INPUTPORT_MAX_MODES;
        int mode_index = offset % FSEMU_INPUTPORT_MAX_MODES;
        fsuae_log(
            "[INPUT] Got port change event port_index=%d mode_index=%d\n",
            port_index,
            mode_index);
        fsuae_handle_port_action(port_index, mode_index);
    }

    if (action >= INPUTEVENT_AMIGA_JOYPORT_MODE_0_NONE &&
        action < INPUTEVENT_AMIGA_JOYPORT_MODE_3_LAST) {
        change_port_device_mode(action - INPUTEVENT_AMIGA_JOYPORT_MODE_0_NONE);
        return;
    }
    if (action >= INPUTEVENT_AMIGA_JOYPORT_0_DEVICE_0 &&
        action < INPUTEVENT_AMIGA_JOYPORT_0_DEVICE_LAST) {
        select_port_0_device(action - INPUTEVENT_AMIGA_JOYPORT_0_DEVICE_0);
        return;
    }
    if (state && action >= INPUTEVENT_AMIGA_JOYPORT_0_AUTOFIRE &&
        action <= INPUTEVENT_AMIGA_JOYPORT_3_AUTOFIRE) {
        int port = action - INPUTEVENT_AMIGA_JOYPORT_0_AUTOFIRE;
        if (g_fs_uae_input_ports[port].autofire_mode) {
            g_fs_uae_input_ports[port].autofire_mode = 0;
            amiga_set_joystick_port_autofire(port, 0);
            fs_emu_warning(_("Auto-fire disabled for port %d"), port);
        } else {
            g_fs_uae_input_ports[port].autofire_mode = 1;
            amiga_set_joystick_port_autofire(port, 1);
            fs_emu_warning(_("Auto-fire enabled for port %d"), port);
        }
#ifdef FSUAE_LEGACY
        fs_emu_menu_update_current();
#endif
        // this event must be passed on to the Amiga core
    }

    int record_event = 1;
    if (playback) {
        record_event = 0;
    }

#if FSUAE_LEGACY
    int load_state = 0;
    int save_state = 0;
    if (action >= INPUTEVENT_SPC_STATESAVE1 &&
        action <= INPUTEVENT_SPC_STATESAVE9) {
        save_state = action - INPUTEVENT_SPC_STATESAVE1 + 1;
        g_fs_uae_state_number = save_state;
    }

    if (action >= INPUTEVENT_SPC_STATERESTORE1 &&
        action <= INPUTEVENT_SPC_STATERESTORE9) {
        load_state = action - INPUTEVENT_SPC_STATERESTORE1 + 1;
        g_fs_uae_state_number = load_state;
    }

    if (load_state) {
#ifdef WITH_LUA
        fsuae_log("run handler on_fs_uae_load_state\n");
        fs_emu_lua_run_handler("on_fs_uae_load_state");
#endif
        record_event = 0;
    } else if (save_state) {
#ifdef WITH_LUA
        fsuae_log("run handler on_fs_uae_save_state\n");
        fs_emu_lua_run_handler("on_fs_uae_save_state");
#endif
        record_event = 0;
    }
#endif

    if (record_event) {
        fs_uae_record_input_event(line, action, state);
    }
    amiga_send_input_event(action, state);

#if FSUAE_LEGACY
    if (load_state) {
#ifdef WITH_LUA
        fsuae_log("run handler on_fs_uae_load_state_done\n");
        fs_emu_lua_run_handler("on_fs_uae_load_state_done");
#endif
    } else if (save_state) {
#ifdef WITH_LUA
        fsuae_log("run handler on_fs_uae_save_state_done\n");
        fs_emu_lua_run_handler("on_fs_uae_save_state_done");
#endif
    }
#endif
}

int g_fs_uae_frame = 0;

static int input_handler_loop(int line)
{
    if (fsemu) {
        // printf("input_handler_loop\n");
    }

    static int last_frame = -1;
    if (g_fs_uae_frame != last_frame) {
        // only run this for the first input handler loop per frame
#ifdef WITH_LUA
        fs_emu_lua_run_handler("on_fs_uae_read_input");
#endif
        last_frame = g_fs_uae_frame;
    }

#ifdef FSUAE_LEGACY
    if (fsemu) {
#endif
        uint16_t action;
        int16_t state;
        while (fsemu_input_next_action(&action, &state)) {
            // int input_event = action & ~FSEMU_ACTION_EMU_FLAG;

            g_fs_uae_last_input_event = action;
            g_fs_uae_last_input_event_state = state;

            fs_uae_process_input_event(line,
                                       g_fs_uae_last_input_event,
                                       g_fs_uae_last_input_event_state,
                                       0);
        }
#ifdef FSUAE_LEGACY
    } else {
        int action;
        // int reconfigure_input = 0;
        while ((action = fs_emu_get_input_event()) != 0) {
            if (fsemu) {
                printf("event_handler_loop received input action %d\n",
                       action);
            }

            int istate = (action & 0x00ff0000) >> 16;
            // force to -128 to 127 range
            signed char state = (signed char) istate;
            action = action & 0x0000ffff;
            // amiga_keyboard_set_host_key(input_event, state);

            g_fs_uae_last_input_event = action;
            g_fs_uae_last_input_event_state = state;
#ifdef WITH_LUA
            fs_emu_lua_run_handler("on_fs_uae_input_event");
#endif

            // handler can modify input event
            // action = g_fs_uae_last_input_event;
            // state = g_fs_uae_last_input_event_state;
            fs_uae_process_input_event(line,
                                       g_fs_uae_last_input_event,
                                       g_fs_uae_last_input_event_state,
                                       0);
        }
    }
#endif

#if 0
    if (line == 0) {
        int o = 300;
        if (g_fs_uae_frame == o + 600) {
            printf("Fake events!\n");
            fs_uae_process_input_event(line, INPUTEVENT_MOUSE1_HORIZ, 50, 0);
        }
        if (g_fs_uae_frame == o + 650) {
            fs_uae_process_input_event(line, INPUTEVENT_MOUSE1_VERT, 200, 0);
        }
        if (g_fs_uae_frame == o + 675) {
            fs_uae_process_input_event(line, INPUTEVENT_MOUSE1_VERT, 30, 0);
        }
        if (g_fs_uae_frame == o + 700) {
            fs_uae_process_input_event(line, INPUTEVENT_JOY1_FIRE_BUTTON, 1, 0);
        }
        if (g_fs_uae_frame == o + 710) {
            fs_uae_process_input_event(line, INPUTEVENT_JOY1_FIRE_BUTTON, 0, 0);
        }
        if (g_fs_uae_frame == o + 800) {
            fs_uae_process_input_event(line, INPUTEVENT_JOY1_FIRE_BUTTON, 1, 0);
        }
        if (g_fs_uae_frame == o + 810) {
            fs_uae_process_input_event(line, INPUTEVENT_JOY1_FIRE_BUTTON, 0, 0);
        }
        if (g_fs_uae_frame == o + 900) {
            fs_uae_process_input_event(line, INPUTEVENT_JOY1_FIRE_BUTTON, 1, 0);
        }
        if (g_fs_uae_frame == o + 910) {
            fs_uae_process_input_event(line, INPUTEVENT_JOY1_FIRE_BUTTON, 0, 0);
        }
#if 0
        if (g_fs_uae_frame == 1000) {
            fs_uae_process_input_event(line, INPUTEVENT_JOY1_2ND_BUTTON, 1, 0);
        }
        if (g_fs_uae_frame == 1050) {
            fs_uae_process_input_event(line, INPUTEVENT_JOY1_2ND_BUTTON, 0, 0);
        }
#endif
    }
#endif

    {
        int event, state;
        while (fs_uae_get_recorded_input_event(
            g_fs_uae_frame, line, &event, &state)) {
            fs_uae_process_input_event(line, event, state, 1);
        }
    }

    return 1;
}

#ifdef FSUAE_LEGACY
static void pause_throttle(void)
{
    /*
    if (fs_emu_get_vblank_sync()) {
        return;
    }
    */
    fs_emu_msleep(5);
}
#endif

static void event_handler(int line)
{
#if 0
    if (fsemu) {
        if (line <= 1) {
            printf("input : event_handler line %d\n", line);
        }
    }
#endif

    // FIXME: Check at what time this is called. Ideally *before* doing the
    // first line of a frame, but I suspect otherwise..
    // fs_emu_wait_for_frame / fsemu_audio_start_frame makes more sense
    // right at the beginning of a new frame

    // printf("%d\n", line);
    // line = 0;
    if (line >= 0) {
        input_handler_loop(line);
        return;
    }
    // static int busy = 0;
    // static int idle = 0;
    // static int64_t last_time = 0;

#if 0
    if (fsemu) {
        amiga_flush_audio();
        // fsemu_audio_end_frame(g_fs_uae_frame);
        fsemu_frame_end();
    }
#endif

    g_fs_uae_frame = g_fs_uae_frame + 1;

#if 0
    if (g_fs_uae_frame != amiga_get_vsync_counter()) {
        printf("g_fs_uae_frame %d amiga_get_vsync_count %d\n", g_fs_uae_frame, amiga_get_vsync_counter());
    }
#endif
    // printf("event_handler frame=%d\n", frame);

    fs_uae_record_frame(g_fs_uae_frame);

    /*
    int64_t t = fs_emu_monotonic_time();
    if (last_time > 0) {
        int dt = (t - last_time) / 1000;
        printf("%d\n", dt);
    }
    */

    // fs_emu_lua_run_handler("on_fs_uae_frame_start");

    fsemu_assert(line == -1);

    // printf("line %d\n", line);
#ifdef FSEMU
    // FIXME: Move away from event handler!
    // FIXME: Move somewhere else??
    // Move to custom.cpp after/when starting new frame?
    // EDIT: YES
    // double adjust = fsemu_audiobuffer_calculate_adjustment();
    //amiga_set_audio_frequency_adjust(adjust);
    // printf("[INPUT] g_fs_uae_frame = %d\n", g_fs_uae_frame);
#else
    fs_emu_wait_for_frame(g_fs_uae_frame);

    if (g_fs_uae_frame == 1) {
        if (!fs_emu_netplay_enabled()) {
            if (fs_config_true(OPTION_WARP_MODE)) {
                amiga_send_input_event(INPUTEVENT_SPC_WARP, 1);
            }
        }
        /* We configure input ports after first frame are confirmed,
         * because otherwise configure events would get lost if initially
         * connected to the server (for net play game), but aborted connection
         * before game started. */
        fs_uae_reconfigure_input_ports_amiga();
        /* Also configure input ports now (which also makes sure keyboard
         * etc. is initialized) to ensure it is configured at least once. */
        fs_uae_reconfigure_input_ports_host();
    }

    if (fs_emu_is_quitting()) {
        if (fs_emu_is_paused()) {
            fs_emu_pause(0);
        }
        static int quit_called = 0;
        if (quit_called == 0) {
            fsuae_log("calling amiga_quit\n");
            amiga_quit();
            quit_called = 1;
        }
        // return;
    }
    while (fs_emu_is_paused()) {
        /*
        if (!event_handler_loop()) {
            break;
        }
        */
        pause_throttle();
        if (fs_emu_is_quitting()) {
            break;
        }
    }
#endif  // FSUAE_LEGACY
    // last_time = fs_emu_monotonic_time();
}

char *g_fs_uae_disk_file_path = NULL;
char *g_fs_uae_config_file_path = NULL;
char *g_fs_uae_config_dir_path = NULL;
// GKeyFile *g_fs_uae_config = NULL;

static int audio_callback_function(int type, int16_t *buffer, int size)
{
#ifdef FSUAE_LEGACY
    if (fsemu) {
        return -1;
    };

    if (type == 0) {
        return fse_queue_audio_buffer(0, buffer, size);
    } else if (type == 1) {
        fse_set_audio_paused(0, true);
        return 0;
    } else if (type == 2) {
        fse_set_audio_paused(0, false);
        return 0;
    } else if (type == 3) {
        /* CD audio stream */
        if (buffer == NULL) {
            /* Check status of buffer number given by size. */
            return fse_check_audio_buffer(1, size);
        }
        return fse_queue_audio_buffer(1, buffer, size);
    }
#endif
    return -1;
}

// FIXME: Candidate for moving out of this file
void fs_uae_load_rom_files(const char *path)
{
    fsuae_log("fs_uae_load_rom_files %s\n", path);
    GDir *dir = g_dir_open(path, 0, NULL);
    if (dir == NULL) {
        fsuae_log("error opening dir\n");
        return;
    }

    // we include the rom key when generating the cache name for the
    // kickstart cache file, so the cache will be regenerated if rom.key
    // is replaced or removed/added.
    char *key_path = g_build_filename(path, "rom.key", NULL);
    GChecksum *rom_checksum = g_checksum_new(G_CHECKSUM_MD5);
    FILE *f = g_fopen(key_path, "rb");
    if (f != NULL) {
        int64_t key_size = fs_path_get_size(key_path);
        if (key_size > 0 && key_size < 1024 * 1024) {
            guchar *key_data = malloc(key_size);
            if (fread(key_data, key_size, 1, f) == 1) {
                fsuae_log("read rom key file, size = %lld\n",
                          (long long) key_size);
                g_checksum_update(rom_checksum, key_data, key_size);
            }
            free(key_data);
        }
        fclose(f);
    }
    g_free(key_path);

    amiga_add_key_dir(path);
    const char *name = g_dir_read_name(dir);
    while (name) {
        char *lname = g_utf8_strdown(name, -1);
        if (g_str_has_suffix(lname, ".rom") ||
            g_str_has_suffix(lname, ".bin")) {
            fsuae_log("found file \"%s\"\n", name);
            char *full_path = g_build_filename(path, name, NULL);
            // GChecksum *checksum = g_checksum_new(G_CHECKSUM_MD5);
            GChecksum *checksum = g_checksum_copy(rom_checksum);
            g_checksum_update(
                checksum, (guchar *) full_path, strlen(full_path));
            const gchar *cache_name = g_checksum_get_string(checksum);
            char *cache_path = g_build_filename(
                fsuae_path_kickstartcache_dir(), cache_name, NULL);
            amiga_add_rom_file(full_path, cache_path);
            g_free(full_path);
            if (cache_path != NULL) {
                free(cache_path);
            }
            g_checksum_free(checksum);
        }
        free(lname);
        name = g_dir_read_name(dir);
    }
    g_dir_close(dir);

    if (rom_checksum != NULL) {
        g_checksum_free(rom_checksum);
    }
    // exit(1);
}

// FIXME: REMOVE
char *fs_uae_encode_path(const char *path)
{
    // FIXME: libamiga now always accepts UTF-8, so this function is
    // deprecated. Simply returning a duplicate now.
    return g_strdup(path);
}

// FIXME: REMOVE
char *fs_uae_decode_path(const char *path)
{
    // FIXME: libamiga now always accepts UTF-8, so this function is
    // deprecated. Simply returning a duplicate now.
    return g_strdup(path);
}

static void on_gui_message(const char *message)
{
    printf("MESSAGE: %s\n", message);
    fs_emu_warning("%s", message);
}

static void on_init(void)
{
    fsuae_log("\n");
    fsuae_log(LOG_LINE);
    fsuae_log("uae configuration\n");
    fsuae_log(LOG_LINE);
    fsuae_log("\n");

    amiga_set_gui_message_function(on_gui_message);

    // fs_uae_configure_amiga_model();
    fs_uae_configure_amiga_hardware();

    // fsuae_media_configure_floppies();
    // fsuae_media_configure_hard_drives();
    // fsuae_media_configure_cdrom();

    fs_uae_configure_input();
    fs_uae_configure_directories();

    if (fs_config_get_int("save_state_compression") == 0) {
        amiga_set_save_state_compression(0);
    } else {
        amiga_set_save_state_compression(1);
    }

#if 0
    if (fs_config_get_int("min_first_line_pal") != FS_CONFIG_NONE) {
        amiga_set_min_first_line(fs_config_get_int("min_first_line_pal"), 0);
    }
    if (fs_config_get_int("min_first_line_ntsc") != FS_CONFIG_NONE) {
        amiga_set_min_first_line(fs_config_get_int("min_first_line_ntsc"), 1);
    }
#endif

    if (fs_config_true(OPTION_CLIPBOARD_SHARING)) {
        amiga_set_option("clipboard_sharing", "yes");
    }

    /*
    if (fs_emu_get_video_sync()) {
        fsuae_log("fs_emu_get_video_sync returned true\n");
        amiga_set_option("gfx_vsync", "true");
    }
    else {
        fsuae_log("fs_emu_get_video_sync returned false\n");
    }
    if (fs_emu_netplay_enabled()) {
        fsuae_log("netplay is enabled\n");
        // make sure UAE does not sleep between frames, we must be able
        // to control sleep times for net play
        amiga_set_option("gfx_vsync", "true");
    }
    */

    // With sound_auto set to true, UAE stops audio output if the amiga does
    // not produce sound. We don't want this, at least not at this point.
    amiga_set_option("sound_auto", "false");

#ifdef FSUAE_LEGACY
    if (fsemu) {
#endif
        amiga_set_audio_frequency(fsemu_audio_frequency());
#ifdef FSUAE_LEGACY
    } else {
        amiga_set_audio_frequency(fs_emu_audio_output_frequency());
    }
#endif

    // amiga_set_audio_frequency(22050);

    // set the input frequency to the output frequency, since we configured
    // libamiga to output at the same frequency

    // FIXME: check the actual frequency libuae/libamiga outputs, seems
    // to output at 44100 Hz even though currprefs.freq says 48000.
    // fs_emu_set_audio_buffer_frequency(0, fs_emu_get_audio_frequency());

    // amiga_set_option("gfx_gamma", "40");

    fs_uae_set_uae_paths();
    fs_uae_read_custom_uae_options(fs_uae_argc, fs_uae_argv);

    char *uae_file;

    uae_file = g_build_filename(fsuae_path_logs_dir(), "LastConfig.uae", NULL);
    if (fs_path_exists(uae_file)) {
        g_unlink(uae_file);
    }
    g_free(uae_file);

    uae_file =
        g_build_filename(fsuae_path_logs_dir(), "DebugConfig.uae", NULL);
    if (fs_path_exists(uae_file)) {
        g_unlink(uae_file);
    }
    g_free(uae_file);

    uae_file = g_build_filename(fsuae_path_logs_dir(), "Debug.uae", NULL);
    if (fs_path_exists(uae_file)) {
        g_unlink(uae_file);
    }
    g_free(uae_file);

    uae_file = g_build_filename(fsuae_path_logs_dir(), "debug.uae", NULL);
    amiga_write_uae_config(uae_file);
    g_free(uae_file);

    fsuae_log("\n");
    fsuae_log(LOG_LINE);
    fsuae_log("end of uae configuration\n");
    fsuae_log(LOG_LINE);
    fsuae_log("\n");
}

#ifdef FSUAE_LEGACY
static void pause_function(int pause)
{
    fsuae_log("pause_function %d\n", pause);
    // uae_pause(pause);
    amiga_pause(pause);
}
#endif  // FSUAE_LEGACY

// ----------------------------------------------------------------------------
// FIXME: Move to fsuae-config or fsuae-configfile ?

static int load_config_file(void)
{
    fsuae_log("load config file\n");
#define msg "checking config file %s\n"

    char *data;
    int size;
    if (fs_data_file_content("META-INF/Config.fs-uae", &data, &size) == 0) {
        fs_ini_file *ini_file = fs_ini_file_open_data(data, size);
        if (ini_file == NULL) {
            fsuae_log("error loading config file\n");
            return 1;
        }
        fs_config_parse_ini_file(ini_file, 0);
        fs_ini_file_destroy(ini_file);
        return 0;
    }

    // g_fs_uae_config = g_key_file_new();
    if (g_fs_uae_config_file_path == NULL) {
        char *path =
            g_build_filename(fsuae_path_exe_dir(), "Config.fs-uae", NULL);
        fsuae_log(msg, path);
        if (fs_path_exists(path)) {
            g_fs_uae_config_file_path = path;
        } else {
            free(path);
        }
    }
#ifdef MACOSX
    if (g_fs_uae_config_file_path == NULL) {
        char *path = g_build_filename(
            fsuae_path_exe_dir(), "..", "..", "Config.fs-uae", NULL);
        fsuae_log(msg, path);
        if (fs_path_exists(path)) {
            g_fs_uae_config_file_path = path;
        } else {
            free(path);
        }
    }
#endif
    if (g_fs_uae_config_file_path == NULL) {
        fsuae_log(msg, "Config.fs-uae");
        if (fs_path_exists("Config.fs-uae")) {
            g_fs_uae_config_file_path = "Config.fs-uae";
        }
    }
    if (g_fs_uae_config_file_path == NULL) {
        fsuae_log(msg, "fs-uae.conf");
        if (fs_path_exists("fs-uae.conf")) {
            g_fs_uae_config_file_path = "fs-uae.conf";
        }
    }
    if (g_fs_uae_config_file_path == NULL) {
        char *path = g_build_filename(
            fse_user_config_dir(), "fs-uae", "fs-uae.conf", NULL);
        fsuae_log(msg, path);
        if (fs_path_exists(path)) {
            g_fs_uae_config_file_path = path;
        } else {
            free(path);
        }
    }
    if (g_fs_uae_config_file_path == NULL) {
        char *path =
            g_build_filename(fsuae_path_configs_dir(), "Default.fs-uae", NULL);
        fsuae_log(msg, path);
        if (fs_path_exists(path)) {
            g_fs_uae_config_file_path = path;
        } else {
            free(path);
        }
    }
    if (g_fs_uae_config_file_path) {
        fsuae_log("loading config from %s\n", g_fs_uae_config_file_path);
        fs_config_read_file(g_fs_uae_config_file_path, 0);
        g_fs_uae_config_dir_path =
            g_path_get_dirname(g_fs_uae_config_file_path);
    }
#if 0
    else {
        if (fs_config_get_boolean("end_config") == 1) {
            // do not warn in case end_config was specified via argv
        }
        else {
            fsuae_log("No configuration file was found");
            g_warn_about_missing_config_file = 1;
        }
    }
#endif

    char *path =
        g_build_filename(fsuae_path_configs_dir(), "Host.fs-uae", NULL);
    fsuae_log(msg, path);
    if (fs_path_exists(path)) {
        fs_config_read_file(path, 0);
    }
    g_free(path);

    return 0;
}

static void log_to_libfsemu(const char *message)
{
    static bool initialized;
    static bool ignore;
    if (!initialized) {
        initialized = true;
        ignore = fs_config_false(OPTION_UAELOG);
    }
    if (!ignore) {
        if (fsemu) {
            fsemu_log_with_level(FSEMU_LOG_LEVEL_INFO, message);
        } else {
            fs_log_string(message);
        }
    }
}

static void main_function()
{
    // A lot of functions asserts that they are run from the emulation thread
    // in debug mode.
    if (fsemu) {
        fsemu_thread_set_emu();
    }

    amiga_main();
    fsuae_log("Return from amiga_main\n");
    fs_uae_write_recorded_session();
    if (fsemu) {
        fsemu_quit_maybe();
    }
}

#ifdef WINDOWS
// FIXME: move to fs_putenv
// int _putenv(const char *envstring);
#endif

// ----------------------------------------------------------------------------
// FIXME: Move to fsuae-i18n ?

static void init_i18n(void)
{
    // FIXME: language = 0 instead?
    if (fs_config_get_boolean("localization") == 0) {
        fsuae_log("[I18N ] Localization was forcefully disabled\n");
        return;
    }

    char *locale = setlocale(LC_MESSAGES, "");
    if (locale) {
        fsuae_log("[I18N] Locale is set to %s\n", locale);
    } else {
        fsuae_log("[I18N] Failed to set current locale\n");
    }

    const char *language = fs_config_get_const_string("language");
    if (language) {
        fsuae_log("[I18N] Set environment LANGUAGE=%s\n", language);
        char *env_str = g_strdup_printf("LANGUAGE=%s", language);
#ifdef WINDOWS
        _putenv(env_str);
#else
        putenv(env_str);
#endif
        /* Do not free env_str, it's put directly in the environment. */
    }

#ifndef ANDROID
    textdomain("fs-uae");

    char executable_dir[FS_PATH_MAX];
    fs_get_application_exe_dir(executable_dir, FS_PATH_MAX);
    char *locale_base =
        g_build_filename(executable_dir, "..", "..", "Data", "Locale", NULL);
    if (g_file_test(locale_base, G_FILE_TEST_IS_DIR)) {
        fsuae_log("[I18N] Using locale dir \"%s\"\n", locale_base);
        bindtextdomain("fs-uae", locale_base);
    } else {
        char *path = fs_get_data_file("fs-uae/share-dir");
        if (path) {
            fsuae_log("[I18N] Using data dir \"%s\"\n", path);
            /* Remove trailing "fs-uae/share-dir" from the returned path. */
            int len = strlen(path);
            if (len > 16) {
                path[len - 16] = '\0';
            }
            char *locale_base_2 = g_build_filename(path, "locale", NULL);
            fsuae_log("[I18N] Using locale dir \"%s\"\n", locale_base_2);
            bindtextdomain("fs-uae", locale_base_2);
            free(locale_base_2);
            free(path);
        }
    }
    free(locale_base);
    bind_textdomain_codeset("fs-uae", "UTF-8");
#endif
}

// ----------------------------------------------------------------------------

#define LED_STATES_MAX 12

static struct {
    fsemu_led_t *power_led;
    fsemu_led_t *floppy_led;
    fsemu_led_t *disk_led;
    int led_states[LED_STATES_MAX];
    int led_brightness[LED_STATES_MAX];
} leds;

static void led_function(int led, int state, int brightness)
{
#ifdef FSUAE_LEGACY
    /* floppy led status is custom overlay 0..3 */
#if 0
    if (led >= 0) {
        printf("led %d state %d\n", led, state);
    }
#endif
    fs_emu_set_custom_overlay_state(led, state);
#endif
    if (led < LED_STATES_MAX) {
        leds.led_states[led] = state;
        leds.led_brightness[led] = brightness;
    }
}

static void on_update_leds(void *data)
{
#ifdef FSUAE_LEGACY
    amiga_led_data *leds = (amiga_led_data *) data;
    for (int i = 0; i < 4; i++) {
        int led = 12;  // df0_d1
        led = led + i * 2;
        fs_emu_set_custom_overlay_state(led + 0, leds->df_t1[i]);
        fs_emu_set_custom_overlay_state(led + 1, leds->df_t0[i]);
    }
#endif
    int floppy_state = 0;
    for (int i = 1; i <= 4; i++) {
    // for (int i = 0; i <= 3; i++) {
        if (leds.led_states[i]) {
            floppy_state = 1;
        }
    }
    // FIXME: Power led should be dimmed and not completely off when audio
    // filter is enabled (strictly speaking depending on model).
    // FIXME: Support older model style of red power and green floppy led?
    // For example when emulating Amiga 1000?

    // FIXME: Use led numbers from UAE, not OD-FS ones
    fsemu_led_set_state(leds.power_led, leds.led_states[0]);
    fsemu_led_set_brightness(leds.power_led, leds.led_brightness[0]);
    // fsemu_led_set_state(leds.power_led, leds.led_states[8]);
    fsemu_led_set_state(leds.floppy_led, floppy_state);
    fsemu_led_set_state(leds.disk_led, leds.led_states[5]);
    // fsemu_led_set_state(leds.disk_led, leds.led_states[9]);
}

// ----------------------------------------------------------------------------

static void fsuae_init_leds(void)
{
    leds.power_led = fsemu_led_create();
    fsemu_led_set_id(leds.power_led, "PowerLed");
    fsemu_led_set_label(leds.power_led, "POWER");
    fsemu_leds_add_led(leds.power_led);

    leds.floppy_led = fsemu_led_create();
    fsemu_led_set_id(leds.floppy_led, "FloppyLed");
    fsemu_led_set_label(leds.floppy_led, "FLOPPY");
    fsemu_leds_add_led(leds.floppy_led);

    leds.disk_led = fsemu_led_create();
    fsemu_led_set_id(leds.disk_led, "DiskLed");
    fsemu_led_set_label(leds.disk_led, "H.DISK");
    fsemu_leds_add_led(leds.disk_led);
}

// ----------------------------------------------------------------------------

static unsigned int whdload_quit_key = 0;

#warning quit function
#ifdef FSUAE_LEGACY

static int64_t whdload_quit_time = 0;

static void fs_emu_send_whdload_quit_key(int whdload_quit_key)
{
    fsuae_log("Find input event for amiga key %d\n", whdload_quit_key);
    int input_event = amiga_find_input_event_for_key(whdload_quit_key);
    fsuae_log("Found input event %d for amiga key %d\n",
              input_event,
              whdload_quit_key);
    if (input_event) {
        fsuae_log("Sending WHDLoad quit key input");
        fs_emu_queue_action(input_event, 1);
    }
}

static int quit_function()
{
    fsuae_log("quit_function\n");
    if (whdload_quit_key) {
        if (whdload_quit_time > 0) {
            int64_t diff = fs_ml_monotonic_time() - whdload_quit_time;
            if (diff < 0.1 * 1000000) {
                // Duplicate SDL_QUIT event?
                return 0;
            }
            // Press quit twice within one second to force quit
            if (diff < 1.0 * 1000000) {
                return 1;
            }
        }
        fsuae_log("NOT QUITING\n");
        if (whdload_quit_time == 0) {
            // Only show this message once
            fs_emu_warning("Sent WHDLoad quit key ($%02X) to exit gracefully",
                           whdload_quit_key);
        } else {
            fs_emu_warning("Press Quit twice to force quit");
        }
        fs_emu_send_whdload_quit_key(whdload_quit_key);
        whdload_quit_time = fs_ml_monotonic_time();
        return 0;
    }
    return 1;
}
#endif

static void media_function(int drive, const char *path)
{
#ifdef FSUAE_LEGACY
    // media insertion status is custom overlay 4..7
    fs_emu_set_custom_overlay_state(4 + drive, path && path[0]);
#endif
}

int ManyMouse_Init(void);
void ManyMouse_Quit(void);
const char *ManyMouse_DeviceName(unsigned int index);

// ----------------------------------------------------------------------------
// FIXME: Not needed any more? Superseeded by fs-uae-device-helper
#if 0
static void list_joysticks()
{
    printf("# FS-UAE %s\n", PACKAGE_VERSION);
    printf("# listing keyboards\n");
    printf("K: Keyboard\n");
    printf("# listing mice\n");
    printf("M: Mouse\n");
    int count = ManyMouse_Init();
    if (count >= 0) {
        for (int i = 0; i < count; i++) {
            const char *name = ManyMouse_DeviceName(i);
            if (name[0] == 0 || strcasecmp(name, "mouse") == 0) {
                printf("M: Unnamed Mouse\n");
            } else {
                printf("M: %s\n", ManyMouse_DeviceName(i));
            }
        }
        ManyMouse_Quit();
    }

    printf("# listing joysticks\n");
#ifdef USE_SDL
    if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
        printf("# SDL_Init(SDL_INIT_JOYSTICK ) < 0\n");
        return;
    }
    printf("# SDL_NumJoysticks(): %d\n", SDL_NumJoysticks());
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_JoystickName(i)[0] == '\0') {
            printf("J: Unnamed\n");
        } else {
            printf("J: %s\n", SDL_JoystickName(i));
        }
    }
#else
    printf("# USE_SDL is not defined\n");
#endif
    printf("# listing joysticks done\n");
}
#endif

// ----------------------------------------------------------------------------
// FIXME: Move to fsuae-log ?

extern int disk_debug_logging;
extern int g_frame_debug_logging;
extern int g_fsdb_debug;
extern int g_random_debug_logging;
extern int inputdevice_logging;

static void configure_logging(const char *logstr)
{
    /* FIXME: log_bsd */
    // if (fs_config_get_int(OPTION_LOG_BSDSOCKET) == 1) {
    //    log_bsd = 1;
    //}

    if (!logstr) {
        fsuae_log("configure logging: none\n");
        return;
    }
    fsuae_log("configure logging: %s\n", logstr);
    int all = strstr(logstr, "all") != 0;
    int uae_all = all || strstr(logstr, "uae") != 0;
    if (uae_all || strstr(logstr, "uae_disk")) {
        disk_debug_logging = 2;
    }
    if (uae_all || strstr(logstr, "uae_rand")) {
        g_random_debug_logging = 2;
    }
    if (uae_all || strstr(logstr, "uae_input")) {
        inputdevice_logging = 0xffff;
    }
    if (uae_all || strstr(logstr, "uae_fs")) {
        g_fsdb_debug = 1;
    }
    if (uae_all || strstr(logstr, "uae_frame")) {
        g_frame_debug_logging = 1;
    }
}

// ----------------------------------------------------------------------------
// FIXME: Move to fsuae-legacy ?

static void cleanup_old_file(const char *path)
{
    char *p = fsuae_path_expand(path);
    if (fs_path_exists(p)) {
        if (fs_path_is_dir(p)) {
            fsuae_log("trying to remove old directory %s\n", p);
            g_rmdir(p);
        } else {
            fsuae_log("trying to remove old file %s\n", p);
            g_unlink(p);
        }
    }
    free(p);
}

static void cleanup_old_files()
{
    // Logs are now stored in $BASE/Cache/Logs by default
    cleanup_old_file("$BASE/Logs/FS-UAE.log");
    cleanup_old_file("$BASE/Logs/FS-UAE.log.txt");
    cleanup_old_file("$BASE/Logs/DebugConfig.uae");
    cleanup_old_file("$BASE/Logs/Launcher.log.txt");
    cleanup_old_file("$BASE/Logs/Synchronization.log");
    // try to remove the dir - if it now is empty
    cleanup_old_file("$BASE/Logs");
}

// ----------------------------------------------------------------------------

static bool check_extension(const char *path, const char *ext)
{
    if (path == NULL) {
        return false;
    }
    gchar *path_lower = g_ascii_strdown(path, -1);
    bool result = g_str_has_suffix(path_lower, ext);
    g_free(path_lower);
    return result;
}

#ifdef LINUX
// FIXME: Merge with code in fsemu-gamemode.c
static void check_linux_cpu_governor()
{
    gchar *governor;
    if (!g_file_get_contents(
            "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor",
            &governor,
            NULL,
            NULL)) {
        return;
    }
    g_strstrip(governor);
    fsuae_log("CPU scaling governor: '%s'\n", governor);
    if (fs_config_get_boolean(OPTION_GOVERNOR_WARNING) == 0) {
        return;
    }
    if (strcmp(governor, "performance") != 0) {
        fs_emu_warning(_("CPU scaling governor is '%s', not '%s'"),
                       governor,
                       "performance");
        fs_emu_warning(_("Emulation frame rate may suffer"));
    }
    g_free(governor);
}
#endif

#ifdef FSUAE_LEGACY
static const char *overlay_names[] = {
    "df0_led",    // 0
    "df1_led",    // 1
    "df2_led",    // 2
    "df3_led",    // 3
    "df0_disk",   // 4
    "df1_disk",   // 5
    "df2_disk",   // 6
    "df3_disk",   // 7
    "power_led",  // 8
    "hd_led",     // 9
    "cd_led",     // 10
    "md_led",     // 11

    "df0_d1",  // 12
    "df0_d0",    "df1_d1", "df1_d0", "df2_d1",
    "df2_d0",    "df3_d1", "df3_d0", NULL,
};
#endif

#if defined(__x86_64__) || defined(_M_AMD64) || defined(_M_X64)
#define ARCH_NAME_2 "x86-64"
#elif defined(__i386__) || defined(_M_IX86)
#define ARCH_NAME_2 "x86"
#elif defined(__ppc__)
#define ARCH_NAME_2 "PPC"
#elif defined(__arm__)
#define ARCH_NAME_2 "ARM"
#else
#define ARCH_NAME_2 "?"
#endif

// clang-format off
#define COPYRIGHT_NOTICE                                                      \
"FS-UAE %s %s %s\n"                                                           \
"\n"                                                                          \
"Copyright 2011-2021 Frode Solheim and others.\n"                             \
"Based on %s, Copyright 1999-2021 Toni Wilen and others.\n"                   \
"Based on UAE, Copyright 1995-2002 Bernd Schmidt and others.\n"               \
"\n"                                                                          \
"This is free software; see the file COPYING for copying conditions. There "  \
"is NO\nwarranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR "  \
"PURPOSE. See\nthe file README and the source code for more info and list "   \
"of contributors.\n"                                                          \
"\n"

#define EXTRA_HELP_TEXT                                                       \
""                                                                            \
"Documentation is available at https://fs-uae.net/docs\n"                     \
"(See https://fs-uae.net/docs/options for a list of configuration "           \
"options)\n"                                                                  \
"\n"
// clang-format on

FILE *g_state_log_file = NULL;

#ifdef FSUAE_LEGACY
#else

static void *emulation_thread(void *data)
{
    fsemu_assert_release(data != NULL);

    fsemu_log("Emulation thread started\n");
    fsemu_thread_set_emu();

#if 0
#ifdef WINDOWS
    set_windows_thread_priority();
#endif
#ifdef WITH_NETPLAY
    if (fs_emu_netplay_enabled()) {
        fse_log("[NETPLAY] Enabled - waiting for connection...\n");
        while (!fs_emu_netplay_connected()) {
            /* Waiting for connection... */
            fs_emu_msleep(10);
            if (!fs_emu_netplay_enabled()) {
                /* Net play mode was aborted. */
                fse_log("netplay aborted\n");
                break;
            }
        }
    }
#endif
#endif

    // g_fs_emu_emulation_thread_running = 1;
    void (*main_function)() = data;
    fsemu_log("Run main function at %p\n", data);
    main_function();

    /* Call fs_ml_quit in case quit was not explicitly requested already. */
    // fs_ml_quit();
    // g_fs_emu_emulation_thread_running = 0;
    /* With this set, and fs_ml_quit being called, the frame render
     * function will call fs_ml_stop when the fadeout effect is done. */
    // g_fs_emu_emulation_thread_stopped = 1;
    return NULL;
}

static fs_thread *g_emulation_thread;

int fs_emu_run(fs_emu_main_function function)
{
    g_emulation_thread =
        fs_thread_create("emulation", emulation_thread, function);
    if (g_emulation_thread == NULL) {
        fsemu_log("Error starting emulation thread\n");
        // FIXME: ERROR MESSAGE HERE
        // FIXME: FATAL
    }

    return 0;
}

#endif

static void handle_events_from_uae(void)
{
    int event;
    void *data;
    int intdata;
    while (uae_main_next_event(&event, &data, &intdata)) {
        fsuae_log_debug("Main received event %d (data: %p)\n", event, data);
        switch (event) {
            case UAE_EVENT_FLOPPY0PATH:
            case UAE_EVENT_FLOPPY1PATH:
            case UAE_EVENT_FLOPPY2PATH:
            case UAE_EVENT_FLOPPY3PATH:
                fsuae_media_handle_uae_event(event, data);
                break;
                // default:
                //     printf(
                //         "Warning: Unhandled event %d (possibly
                //         memory " "leak)\n", event);

            case UAE_EVENT_PORT0MODE:
            case UAE_EVENT_PORT1MODE:
            case UAE_EVENT_PORT2MODE:
            case UAE_EVENT_PORT3MODE:
                fsuae_input_handle_uae_event(event, data, intdata);
                break;
        }
        uae_main_free_event(event, data);
    }
}

static void handle_clipboard_integration(void)
{
    char *clipboard_uae = uae_clipboard_get_text();
    if (clipboard_uae) {
        SDL_SetClipboardText(clipboard_uae);
        uae_clipboard_free_text(clipboard_uae);
    } else {
        // FIXME: Ideally, we would want to avoid this alloc/free when the
        // clipboard hasn't changed.
        char *clipboard_host = SDL_GetClipboardText();
        uae_clipboard_put_text(clipboard_host);
        SDL_free(clipboard_host);
    }
}

static void main_loop(void)
{
    while (fsemu_main_is_running()) {
        // FIXME: Events from UAE could also be injected into the SDL
        // event system via custom events...
        handle_events_from_uae();

        fsemu_main_update_and_snapshot_gui();
        handle_clipboard_integration();
        fsemu_main_handle_events_until_next_frame();
    }
    // fsemu_main_quit();
#if 0
    // FIXME: Or get snapshot from fsemu helper?
    // fsemu_gui_item_t *snapshot = NULL;
    // if (fsemu_video_is_threaded()) {

    // } else {
    //     NULL;  // = fsemu_gui_snapshot();
    //             // We do not need to initialize snapshot, new_host_frame
    //             // will force a snapshot to be created in the main loop.
    // bool new_host_frame = true;

    // if (fsemu_video_is_threaded()) {
        while (fsemu_main_is_running()) {
            // FIXME: Events from UAE could also be injected into the SDL
            // event system via custom events...
            handle_events_from_uae();

            fsemu_main_update_and_snapshot_gui();

            handle_clipboard_integration();

            fsemu_main_handle_events_until_next_frame();
        }
        return;
    // }
#if 0
    while (!fsemu_quit_check()) {
        // main_iteration / window_iteration

        static int64_t old_frame = 0;
        int64_t frame = fsemu_frame_counter();
        if (frame != old_frame) {
            fsemu_fade_update();
            fsemu_hud_update();
            fsemu_oskeyboard_update();
            fsemu_osmenu_update();
            fsemu_perfgui_update();
            fsemu_startupinfo_update();
            fsemu_titlebar_update();
            old_frame = frame;
        }

        if (new_host_frame) {
            snapshot = fsemu_gui_snapshot();

            if (!fsemu_video_is_threaded()) {
                fsemu_video_render_gui_early(snapshot);
            }

            new_host_frame = false;

            handle_clipboard_integration();
        }

        // The SDL window subsystem will put input events into a queue which
        // can be read by the SDL input system right afterwards. So the order
        // here matters.
        fsemu_window_work(0);
        fsemu_input_work(0);

        handle_events_from_uae();

        if (!fsemu_video_is_threaded()) {
            fsemu_video_work(1000);
            if (fsemu_video_ready()) {
                fsemu_video_render();
                fsemu_video_render_gui(snapshot);
                fsemu_gui_free_snapshot(snapshot);
                snapshot = NULL;

                fsemu_video_display();

                new_host_frame = true;
            }
        }
    }

    if (snapshot) {
        fsemu_gui_free_snapshot(snapshot);
    }
#endif
#endif
}

int main(int argc, char *argv[])
{
    fs_uae_argc = argc;
    fs_uae_argv = argv;
    fs_set_argv(argc, argv);

#ifdef WITH_CEF
    cef_init(argc, argv);
#endif

    fsemu_hud_init_early();

    char **arg;
    arg = argv + 1;
    while (arg && *arg) {
        if (strcmp(*arg, "--version") == 0) {
            printf("%s\n", PACKAGE_VERSION);
            exit(0);
        } else if (strcmp(*arg, "--help") == 0) {
            printf(COPYRIGHT_NOTICE,
                   PACKAGE_VERSION,
                   OS_NAME_2,
                   ARCH_NAME_2,
                   UAE_BASE_VERSION);
            printf(EXTRA_HELP_TEXT);
            exit(0);
        }
        arg++;
    }

    fs_init();
    int error = fs_data_init("fs-uae", "fs-uae.dat");
    if (error) {
        printf("WARNING: error (%d) loading fs-uae.dat\n", error);
    }

    fs_set_prgname("fs-uae");
    fs_set_application_name("Amiga Emulator");

    fprintf(stderr,
            COPYRIGHT_NOTICE,
            PACKAGE_VERSION,
            OS_NAME_2,
            ARCH_NAME_2,
            UAE_BASE_VERSION);
    fsemu_log_with_level(FSEMU_LOG_LEVEL_INFO,
                         COPYRIGHT_NOTICE,
                         PACKAGE_VERSION,
                         OS_NAME_2,
                         ARCH_NAME_2,
                         UAE_BASE_VERSION);

    char *current_dir = g_get_current_dir();
    fsuae_log("current directory is %s\n", current_dir);
    g_free(current_dir);

#ifdef MACOSX
    /* Can be overriden via environment variable (or launcher setting):
     *  SDL_VIDEO_MAC_FULLSCREEN_SPACES = 1 */
    SDL_SetHint(SDL_HINT_VIDEO_MAC_FULLSCREEN_SPACES, "0");

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_PumpEvents();
    SDL_Event event;
    fsuae_log("OS X: Check for pending SDL_DROPFILE event\n");
    while (SDL_PollEvent(&event)) {
        fsuae_log("Got SDL event 0x%x\n", event.type);
        if (event.type == SDL_DROPFILE) {
            if (event.drop.file != NULL) {
                g_fs_uae_config_file_path = strdup(event.drop.file);
            }
            SDL_free(event.drop.file);
        }
    }
#endif

    // skip first entry
    arg = argv + 1;
    if (g_fs_uae_config_file_path == NULL) {
        while (arg && *arg) {
            const gchar *test_path = *arg;
            if (test_path && fs_path_exists(test_path)) {
                if (check_extension(test_path, ".fs-uae")) {
                    g_fs_uae_config_file_path = g_strdup(test_path);
                } else if (check_extension(test_path, ".conf")) {
                    g_fs_uae_config_file_path = g_strdup(test_path);
                } else if (check_extension(test_path, ".adf")) {
                    g_fs_uae_disk_file_path = g_strdup(test_path);
                } else if (check_extension(test_path, ".ipf")) {
                    g_fs_uae_disk_file_path = g_strdup(test_path);
                } else if (check_extension(test_path, ".dms")) {
                    g_fs_uae_disk_file_path = g_strdup(test_path);
                }
            }
            arg++;
        }
    }

    /* Parse options first, in case base_dir, logging  options etc is
     * specified on the command line. */
    fs_config_parse_options(argc - 1, argv + 1);

    fsuae_log("\n");
    fsuae_log(LOG_LINE);
    fsuae_log("libfsemu init\n");
    fsuae_log(LOG_LINE);
    fsuae_log("\n");

    // FIXME: Replace with n calls to fsemu_path_add_prefix ?

#ifdef FSUAE_LEGACY
    fs_emu_path_set_expand_function(fsuae_path_expand);
    fs_emu_init_overlays(overlay_names);
    fse_init_early();
#endif

    /* Then load the config file and set data dir */
    load_config_file();
    fs_set_data_dir(fsuae_path_data_dir());

    amiga_set_log_function(log_to_libfsemu);
    amiga_init();

    fsemu = 1;

    // This call will also register the main thread.
    fsemu_thread_init();
    // Register main thread as video thread also.
    fsemu_thread_set_video();

    fsemu_option_init_from_argv(argc, argv);


    fsemu_log_setup();
    // fsemu_audio_init(0);
    // fsemu_window_init();

#if 0
    // fsemu_audio_init(0);
    fsemu_window_init();
    fsemu_video_init();
    fsemu_titlebar_init();

    fsemu_audio_init();

    fsemu_perfgui_init();
    fsemu_titlebar_update();
#endif

    fsemu_set_emulator_name("FS-UAE");
    fsemu_window_set_title("FS-UAE");
    // fsemu_video_set_renderer(renderer);
    // fsemu_startupinfo_set_emulator_name("FS-UAE");
    // fsemu_startupinfo_set_emulator_fork_info("Mednafen");
#if 0
    // FIXME: Review
    if (fullscreen == 1) {
        fsemu_window_set_fullscreen(true);
    }
#endif
#if 0
    // FIXME: Review
    if (vsync == 1) {
        fsemu_video_set_vsync(1);
    }
#endif
    // fsemu_video_set_vsync(1);

    fsemu_application_set_base_dir(fsuae_path_base_dir());

    fsemu_video_set_renderer(FSEMU_VIDEO_RENDERER_OPENGL);

    fsemu_control_set_soft_reset_allowed(true);
    fsemu_control_set_hard_reset_allowed(true);

    fsemu_window_init();

    // fsemu_video_set_format(FSEMU_VIDEO_FORMAT_RGB565);

    fsemu_video_init();

    fsemu_fade_init();
    fsemu_titlebar_init();

    fsemu_helper_startup_loop();

    fsemu_background_init();
    fsemu_hud_init();
    fsemu_oskeyboard_init();
    fsemu_osmenu_init();
    fsemu_perfgui_init();
    fsemu_screenshot_init();
    fsemu_startupinfo_init();

    fsemu_audio_init();
    fsemu_input_init();

    fsemu_leds_init();
    fsemu_helpbar_init();

    fsemu_action_init();

    init_i18n();
    fsuae_init_leds();

    if (g_fs_uae_disk_file_path) {
        fs_config_set_string(OPTION_FLOPPY_DRIVE_0, g_fs_uae_disk_file_path);
        g_warn_about_missing_config_file = 0;
    }

    if (g_warn_about_missing_config_file) {
        fs_emu_warning(_("No configuration file was found"));
    }

    const char *expect_version =
        fs_config_get_const_string(OPTION_EXPECT_VERSION);
    if (expect_version && strcmp(expect_version, PACKAGE_VERSION) != 0) {
        fs_emu_warning(_("Expected FS-UAE version %s, got %s"),
                       expect_version,
                       PACKAGE_VERSION);
    }

    if (fsemu) {
        fsemu_gamemode_init();
    } else {
#ifdef LINUX
        if (fs_config_get_boolean(OPTION_GAME_MODE) != 0) {
            if (gamemode_request_start() < 0) {
                fsuae_log("GameMode: Request failed: %s\n",
                          gamemode_error_string());
            } else {
                fsuae_log("GameMode: Enabled game mode\n");
            }
        }
        check_linux_cpu_governor();
#endif
    }

    fsuae_log("\n");
    fsuae_log(LOG_LINE);
    fsuae_log("fs-uae init\n");
    fsuae_log(LOG_LINE);
    fsuae_log("\n");

    configure_logging(fs_config_get_const_string("log"));

    fsuae_log("[GLIB] Version %d.%d.%d (Compiled against %d.%d.%d)\n",
              glib_major_version,
              glib_minor_version,
              glib_micro_version,
              GLIB_MAJOR_VERSION,
              GLIB_MINOR_VERSION,
              GLIB_MICRO_VERSION);
#if 0
    fsuae_log("[GLIB] Using system malloc: %s\n",
           g_mem_is_system_malloc() ? "Yes" : "No");
#endif

#ifdef FSUAE_LEGACY
    fs_emu_set_state_check_function(amiga_get_state_checksum);
    fs_emu_set_rand_check_function(amiga_get_rand_checksum);
    fs_emu_set_quit_function(quit_function);
#endif

    // force creation of some recommended default directories
    fsuae_path_kickstarts_dir();
    fsuae_path_configs_dir();
    fs_uae_init_path_resolver();

    fs_uae_plugins_init();

    // must be called early, before fs_emu_init -affects video output
    fs_uae_configure_amiga_model();

#ifdef FSUAE_LEGACY
    const char *controllers_dir = fsuae_path_controllers_dir();
    if (controllers_dir) {
        fs_emu_set_controllers_dir(controllers_dir);
    }
#endif

    const char *logs_dir = fsuae_path_logs_dir();
    if (logs_dir) {
        char *log_file;

        log_file = g_build_filename(logs_dir, "FS-UAE.log", NULL);
        if (fs_path_exists(log_file)) {
            g_unlink(log_file);
        }
        g_free(log_file);

        log_file = g_build_filename(logs_dir, "FS-UAE.log.txt", NULL);
        if (fs_path_exists(log_file)) {
            g_unlink(log_file);
        }
        g_free(log_file);

        log_file = g_build_filename(logs_dir, "Emulator.log.txt", NULL);
        if (fs_path_exists(log_file)) {
            g_unlink(log_file);
        }
        g_free(log_file);

        log_file = g_build_filename(logs_dir, "fs-uae.log.txt", NULL);
        fs_config_set_log_file(log_file);
        g_free(log_file);
    }

    fs_config_set_string_if_unset("themes_dir", fsuae_path_themes_dir());

#ifdef FSUAE_LEGACY
    fs_emu_set_pause_function(pause_function);

    // fs_uae_init_input();
    if (fsemu) {
        fse_init(FS_EMU_INIT_INPUT);
    } else {
        fse_init(FS_EMU_INIT_EVERYTHING);
    }
#endif

    // We initialize the recording module whether it is used or not, so it
    // can delete state-specific recordings (if necessary) when states are
    // saved.
    fs_uae_init_recording();

    int deterministic_mode = 0;
    const char *record_file = fs_config_get_const_string("record");
    if (record_file) {
        fsuae_log("record file specified: %s, forcing deterministic mode\n",
                  record_file);
        deterministic_mode = 1;
        fs_uae_enable_recording(record_file);
    } else {
        fsuae_log("not running in record mode\n");
    }

    if (
#ifdef FSUAE_LEGACY
        fs_emu_netplay_enabled() ||
#endif
        fs_config_get_boolean(OPTION_DETERMINISTIC) == 1) {
        deterministic_mode = 1;
    }
    if (deterministic_mode) {
        amiga_set_deterministic_mode();
    }

    if (logs_dir) {
#ifdef FSUAE_LEGACY
        if (fs_emu_netplay_enabled()) {
            char *sync_log_file =
                g_build_filename(logs_dir, "Synchronization.log", NULL);
            amiga_set_synchronization_log_file(sync_log_file);
            free(sync_log_file);
        }
#endif
    }

    if (fsemu) {
        amiga_set_audio_buffer_size(128);
    } else {
#ifdef FSUAE_LEGACY
#if 1  // def FSE_DRIVERS
        fse_audio_stream_options **options =
            fs_emu_audio_alloc_stream_options(2);
        options[0]->frequency = fs_emu_audio_output_frequency();
        /* 12 * 2352 is CDDA_BUFFERS * 2352 (blkdev_cdimage.cpp) */
        options[1]->buffer_size = 12 * 2352;
        // begin playing with only one buffer queued
        options[1]->min_buffers = 1;
        fs_emu_audio_configure(options);
        amiga_set_audio_buffer_size(options[0]->buffer_size);
        fs_emu_audio_free_stream_options(options);
#else
        // this stream is for paula output and drive clicks
        // FIXME: could mix drive clicks in its own stream instead, -might
        // give higher quality mixing
        fse_audio_stream_options options;
        options.struct_size = sizeof(fse_audio_stream_options);
        fs_emu_init_audio_stream_options(&options);
        options.frequency = fs_emu_audio_output_frequency();
        fs_emu_init_audio_stream(0, &options);
        amiga_set_audio_buffer_size(options.buffer_size);

        // this stream is for CD audio output (CDTV/CD32)
        fs_emu_init_audio_stream_options(&options);
        // 12 * 2352 is CDDA_BUFFERS * 2352 (blkdev_cdimage.cpp)
        options.buffer_size = 12 * 2352;
        // begin playing with only one buffer queued
        options.min_buffers = 1;
        fs_emu_init_audio_stream(1, &options);
#endif
#endif
    }

    amiga_set_audio_callback(audio_callback_function);
    amiga_set_cd_audio_callback(audio_callback_function);
    amiga_set_event_function(event_handler);

    amiga_set_led_function(led_function);
    amiga_on_update_leds(on_update_leds);

    amiga_set_media_function(media_function);
    amiga_set_init_function(on_init);

    if (fs_config_get_boolean(OPTION_JIT_COMPILER) == 1) {
        amiga_init_jit_compiler();
    }

#ifdef WITH_LUA
    amiga_init_lua(fs_emu_acquire_lua, fs_emu_release_lua);
    amiga_init_lua_state(fs_emu_get_lua_state());
    fs_uae_init_lua_state(fs_emu_get_lua_state());
#endif

    // FS_EMU_VIDEO_FORMAT_BGRA
    // amiga_set_video_format(AMIGA_VIDEO_FORMAT_BGRA);

    amiga_set_video_format(fsemu_video_format());

    int video_format = fsemu_video_format();
    if (video_format == FSEMU_VIDEO_FORMAT_RGBA) {
        amiga_set_video_format(AMIGA_VIDEO_FORMAT_RGBA);
    } else if (video_format == FSEMU_VIDEO_FORMAT_BGRA) {
        amiga_set_video_format(AMIGA_VIDEO_FORMAT_BGRA);
    } else if (video_format == FSEMU_VIDEO_FORMAT_RGB565) {
        amiga_set_video_format(AMIGA_VIDEO_FORMAT_R5G6B5);
        // } else if (video_format == FSEMU_VIDEO_FORMAT_R5G5B5A1) {
        //     amiga_set_video_format(AMIGA_VIDEO_FORMAT_R5G5B5A1);
    } else {
        fs_emu_warning("Unsupported video format requested");
    }

#ifdef FSUAE_LEGACY
    if (fs_emu_get_video_format() == FS_EMU_VIDEO_FORMAT_RGBA) {
        amiga_set_video_format(AMIGA_VIDEO_FORMAT_RGBA);
    } else if (fs_emu_get_video_format() == FS_EMU_VIDEO_FORMAT_BGRA) {
        amiga_set_video_format(AMIGA_VIDEO_FORMAT_BGRA);
    } else if (fs_emu_get_video_format() == FS_EMU_VIDEO_FORMAT_R5G6B5) {
        amiga_set_video_format(AMIGA_VIDEO_FORMAT_R5G6B5);
    } else if (fs_emu_get_video_format() == FS_EMU_VIDEO_FORMAT_R5G5B5A1) {
        amiga_set_video_format(AMIGA_VIDEO_FORMAT_R5G5B5A1);
    } else {
        fs_emu_warning("Unsupported video format requested");
    }
    amiga_add_rtg_resolution(fs_emu_get_windowed_width(),
                             fs_emu_get_windowed_height());
    amiga_add_rtg_resolution(fs_emu_get_fullscreen_width(),
                             fs_emu_get_fullscreen_height());
#endif
#warning re-add fullscreen resolution
    fs_uae_init_video();

    // fs_uae_init_keyboard();
    fs_uae_init_mouse();

#ifdef FSUAE_LEGACY
    fs_uae_configure_menu();
#endif

    if (fsemu) {
        fsuae_inputport_init();
        fsuae_keyboard_init();
        fsuae_media_init();
        // fsuae_menu_init();
        // fsuae_mouse_init();
        fsuae_oskeyboard_init();
        // fsuae_osmenu_init();

        // Sets up some savestate callbacks
        fsuae_savestate_init();

        // FIXME: MOVE SOMEWHERE ELSE
        fsemu_input_autofill_devices();
        // FIXME: MOVE SOMEWHERE ELSE
        fsemu_input_reconfigure();
    }

    const char *value = fs_config_get_const_string("whdload_quit_key");
    if (value) {
        sscanf(value, "%02x", &whdload_quit_key);
        if (whdload_quit_key) {
            fsuae_log("Using WHDLoad quit key: %02X\n", whdload_quit_key);
        }
    }

#if 0
    if (fsemu) {
#if 0
        // fsemu_audio_init(0);
        fsemu_window_init();

        fsemu_video_init();

        fsemu_titlebar_init();

        fsemu_audio_init();

        fsemu_perfgui_init();
        fsemu_titlebar_update();
#endif

        fsemu_set_emulator_name("FS-UAE");
        fsemu_window_set_title("FS-UAE");
        // fsemu_video_set_renderer(renderer);
        fsemu_startupinfo_set_emulator_name("FS-UAE");
        // fsemu_startupinfo_set_emulator_fork_info("Mednafen");
#if 0
        // FIXME: Review
        if (fullscreen == 1) {
            fsemu_window_set_fullscreen(true);
        }
        // FIXME: Review
        if (vsync == 1) {
            fsemu_video_set_vsync(1);
        }
#endif
        fsemu_window_init();
        fsemu_video_init();
        fsemu_titlebar_init();

//        fsemu_helper_startup_loop();

        fsemu_perfgui_init();
//        fsemu_startupinfo_init();
//        fsemu_hud_init();

        fsemu_audio_init();
        fsemu_input_init(FSEMU_FLAG_NONE);
    }
#endif

    fs_emu_run(main_function);

    if (fsemu) {
        main_loop();
        amiga_quit();
    }

    fsuae_log("fs-uae shutting down, fs_emu_run returned\n");
    if (g_rmdir(fsuae_path_state_dir()) == 0) {
        fsuae_log("state dir %s was removed because it was empty\n",
                  fsuae_path_state_dir());
    } else {
        fsuae_log("state dir %s was not removed (non-empty)\n",
                  fsuae_path_state_dir());
    }
    fsuae_log("end of main function\n");
    cleanup_old_files();

    // Fixes PGO issues?
    fs_thread_wait(g_emulation_thread);

    // FIXME: Probably call some more high-level fsemu_quit function instead
    // and let that one call fsemu_module_quit.
    fsemu_module_quit();

#if 0
    int a = 3;
    int b = 0;
    printf("%d\n", a / b);
#endif

    return 0;
}
