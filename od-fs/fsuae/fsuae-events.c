#include "fsuae-events.h"

#include "fsemu-input.h"
#include "fsemu-media.h"
#include "fsemu-util.h"

#include <glib.h>

#include <uae/uae.h>

#define fsuae_log SDL_Log

static void fsuae_handle_media_action(int drive_index, int media_index)
{
    if (drive_index >= fsemu_media_drive_count()) {
        return;
    }
    fsemu_media_drive_t* drive = fsemu_media_drive_at_index(drive_index);
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
            amiga_send_input_event(
                INPUTEVENT_SPC_DISKSWAPPER_0_0 + drive_index * 20
                    + media_index,
                1
            );
        } else {
            fsemu_warning("Cannot deal with media index >= 20\n");
        }
    } else if (type == FSEMU_MEDIA_DRIVE_TYPE_CDROM) {
        // Only one CD-ROM drive supported currently.
        if (type_index >= 1) {
            fsemu_warning("Cannot deal with with CD-ROM driveindex >= 1\n");
            return;
        }

        fsemu_warning("CURRENTLY DISABLED / NOT IMPLEMENTED\n");
#if 0
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
#endif
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
}

static void
fsuae_events_process_input_event(int line, int action, int state, int playback)
{
    // printf("... action %d %d\n", action, FSEMU_ACTION_DRIVE0EJECT);
    static int first_time = 1;
#if 0
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
#endif

#if 0
    if (action == INPUTEVENT_KEY_RETURN) {
        printf("FIXME: ignoring RETURN event for now\n");
        return;
    }
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
    else if (action >= FSEMU_ACTION_DRIVE0EJECT
             && action <= FSEMU_ACTION_DRIVE7EJECT) {
        int drive_index = action - FSEMU_ACTION_DRIVE0EJECT;
        fsuae_log("Got drive eject event drive_index=%d\n", drive_index);
        fsuae_handle_media_action(drive_index, -1);
    } else if (action >= FSEMU_ACTION_DRIVE0INSERT0
               && action <= FSEMU_ACTION_DRIVE7INSERT255) {
        int offset = action - FSEMU_ACTION_DRIVE0INSERT0;
        int drive_index = offset / 256; // FSEMU_MEDIA_MAX_FILE_COUNT;
        int media_index = offset % 256; // FSEMU_MEDIA_MAX_FILE_COUNT;
        fsuae_log(
            "Got drive insert event drive_index=%d media_index=%d",
            drive_index, media_index
        );
        fsuae_handle_media_action(drive_index, media_index);
    }

    else if (action >= FSEMU_ACTION_PORT0TYPE0
             && action < FSEMU_ACTION_PORT3TYPE255) {
        int offset = action - FSEMU_ACTION_PORT0TYPE0;
        int port_index = offset / 256; // FSEMU_INPUTPORT_MAX_MODES;
        int mode_index = offset % 256; // FSEMU_INPUTPORT_MAX_MODES;
        fsuae_log(
            "[INPUT] Got port change event port_index=%d mode_index=%d\n",
            port_index, mode_index
        );
        fsuae_handle_port_action(port_index, mode_index);
    }

#if 0
    if (action >= INPUTEVENT_AMIGA_JOYPORT_MODE_0_NONE &&
        action < INPUTEVENT_AMIGA_JOYPORT_MODE_3_LAST) {
        change_port_device_mode(action - INPUTEVENT_AMIGA_JOYPORT_MODE_0_NONE);
        return;
    }
#endif

#if 0
    if (action >= INPUTEVENT_AMIGA_JOYPORT_0_DEVICE_0 &&
        action < INPUTEVENT_AMIGA_JOYPORT_0_DEVICE_LAST) {
        select_port_0_device(action - INPUTEVENT_AMIGA_JOYPORT_0_DEVICE_0);
        return;
    }
#endif

#if 0
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
#endif

    int record_event = 1;
    if (playback) {
        record_event = 0;
    }

#if FSUAE_LEGACY
    int load_state = 0;
    int save_state = 0;
    if (action >= INPUTEVENT_SPC_STATESAVE1
        && action <= INPUTEVENT_SPC_STATESAVE9) {
        save_state = action - INPUTEVENT_SPC_STATESAVE1 + 1;
        g_fs_uae_state_number = save_state;
    }

    if (action >= INPUTEVENT_SPC_STATERESTORE1
        && action <= INPUTEVENT_SPC_STATERESTORE9) {
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

#if 0
    if (record_event) {
        fs_uae_record_input_event(line, action, state);
    }
#endif
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

static int g_fs_uae_frame = 0;

static int fsuae_events_input_handler_loop(int line)
{
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

            // g_fs_uae_last_input_event = action;
            // g_fs_uae_last_input_event_state = state;

            fsuae_events_process_input_event(line, action, state, 0);
        }
#ifdef FSUAE_LEGACY
    } else {
        int action;
        // int reconfigure_input = 0;
        while ((action = fs_emu_get_input_event()) != 0) {
            if (fsemu) {
                printf(
                    "event_handler_loop received input action %d\n", action
                );
            }

            int istate = (action & 0x00ff0000) >> 16;
            // force to -128 to 127 range
            signed char state = (signed char)istate;
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
            fs_uae_process_input_event(
                line, g_fs_uae_last_input_event,
                g_fs_uae_last_input_event_state, 0
            );
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

#if 0
    {
        int event, state;
        while (fs_uae_get_recorded_input_event(
            g_fs_uae_frame, line, &event, &state)) {
            fs_uae_process_input_event(line, event, state, 1);
        }
    }
#endif

    return 1;
}

void fsuae_events_event_handler(int line)
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
        fsuae_events_input_handler_loop(line);
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

#if 0
    fs_uae_record_frame(g_fs_uae_frame);
#endif

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
    // amiga_set_audio_frequency_adjust(adjust);
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
#endif // FSUAE_LEGACY
    // last_time = fs_emu_monotonic_time();
}
