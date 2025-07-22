#include "sysconfig.h"
#include "sysdeps.h"

#include "uae/fs.h"
#include "uae.h"

#include "fsemu-audiobuffer.h"
#include "fsemu-frame.h"
#include "fsemu-quit.h"
#include "fsemu-python.h"
#include "fsemu-time.h"
//#include <fs/emu/hacks.h>

#include "inputdevice.h"

// -----------------

static bool amiga_parse_option_changed_fixme(const char *option, const char *value, int type)
{
    // some strings are modified during parsing
    char *value2 = strdup(value);
    int result = cfgfile_parse_option(
        &changed_prefs, (char *) option, (char *) value2, type);
    config_changed = 1;
    free(value2);
    write_log(
        "Set option %s = \"%s\"%s\n", option, value, result == 1 ? "" : " FAILED");
    if (result == 1) {
        return true;
    } else {
        gui_message("Option failed: %s = %s", option, value);
        amiga_log_warning(
            "Failed to set option \"%s\" to \"%s\" "
            "(result: %d)\n",
            option,
            value,
            result);
        return false;
    }
}

static void apply_option(const char *name, const char *value)
{
    printf("apply option %s = %s\n", name, value);
    amiga_parse_option_changed_fixme(name, value, 0);

    if (strcmp(name, "quickstart") == 0) {
        uae_log("Resetting Amiga after running quickstart command\n");
        uae_reset(1, 1);
    }
}

void uae_fs_apply_pending_config_changes(void)
{
    fsemu_python_apply_options(apply_option);
}

// -----------------

void uae_fs_begin_frame(float vblank_hz)
{
    // fsemu_frame_update_timing(vblank_hz, currprefs.turbo_emulation);
    // printf("vblank_hz = %0.2f\n", vblank_hz);

    fsemu_frame_start(vblank_hz);
    double adjust = fsemu_audiobuffer_calculate_adjustment();
    amiga_set_audio_frequency_adjust(adjust);

#if 0
    // Now we sleep until the start of the next frame. Using the less
    // accurate sleep function to use less system resources. Accuracy is
    // not that important right here.
    // int64_t before_us = fsemu_time_us();
    // int64_t after_us = fsemu_time_sleep_until_us_2(
    // 	fsemu_frame_begin_at, before_us);
    // fsemu_frame_sleep_duration += after_us - before_us;
    int64_t now_us = fsemu_time_sleep_until_us(fsemu_frame_begin_at);
    fsemu_frame_add_sleep_time(now_us);
#endif

    int slot;
    if (fsemu_frame_check_load_state(&slot)) {
        fsemu_frame_log_epoch("Load state %d\n", slot);
        if (slot == 0) {
            printf("Slot 0 not supported yet\n");
        } else {
            amiga_send_input_event(INPUTEVENT_SPC_STATERESTORE1 - 1 + slot, 1);
        }
    }
    if (fsemu_frame_check_save_state(&slot)) {
        fsemu_frame_log_epoch("Load state %d\n", slot);
        if (slot == 0) {
            printf("Slot 0 not supported yet\n");
        } else {
            // FIXME: Going to save state; store state slot number in a
            // global variable - this should signal that the video module
            // should save a screenshot copy that can be saved together with
            // the savestate.
            amiga_send_input_event(INPUTEVENT_SPC_STATESAVE1 - 1 + slot, 1);
        }
    }
}

#include "fsapp-events.h"
#include "fsuae-messages.h"
#include "savestate.h" // for save_configuration

void uae_fs_end_frame()
{
    fsemu_frame_end();

    // FIXME: Make into a callback instead (later)
    fsuae_messages_process();

    // FIXME: THREADING / SYNC
    // FIXME: Do we want to do this at another point in time, Or, even better,
    // maybe have fsapp main lopp - after rendering a frame - wait until
    // the fs emu frame is complete and any post frame handlers (?) are done.
    // Checking/updating config copy might be one such task.

    // FIXME: PERFORMANCE
    // FIXME: Maybe replace this function with some possibly more efficient
    size_t length;
    //char *config = (char*) save_configuration(&length, false);
    char *config = (char*) save_configuration(&length, true);

    // FIXME: This will do (another) copy of the data, avoid this later by
    // including a free function to free the strings?
    fsapp_events_push_string("UAE_CONFIG", config);

    free(config);

    // FIXME: Additionally, Python will make another copy...!!
    // Maybe use ctypes.c_char_p somehow, or copy/parse into dict before
    // passing onto python? Buffer protocol?
}
