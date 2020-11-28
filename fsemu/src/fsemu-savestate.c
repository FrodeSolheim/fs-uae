#include "fsemu-internal.h"
#include "fsemu-savestate.h"

#include <stdio.h>

#include "fsemu-action.h"
#include "fsemu-glib.h"
#include "fsemu-path.h"
#include "fsemu-thread.h"
#include "fsemu-time.h"
#include "fsemu-util.h"

#define FSEMU_SAVESTATE_MAX_SLOT 9

// FIXME: REMOVE
#define FSEMU_SAVESTATE_SLOTS 9

static struct {
    bool initialized;
    fsuae_savestate_path_function_t path_function;
    bool has_state[FSEMU_SAVESTATE_MAX_SLOT + 1];
    char *descriptions[FSEMU_SAVESTATE_MAX_SLOT + 1];

} fsemu_savestate;

void fsemu_savestate_set_path_function(
    fsuae_savestate_path_function_t function)
{
    fsemu_savestate.path_function = function;
}

bool fsemu_savestate_has_state(int slot)
{
    fsemu_thread_assert_main();

    return fsemu_savestate.has_state[slot];
}

void fsemu_savestate_update_slot(int slot)
{
    char *path = NULL;
    char *description = NULL;
    if (fsemu_savestate.path_function) {
        path = fsemu_savestate.path_function(slot);
    }
    if (path && fsemu_path_exists(path)) {
        fsemu_savestate.has_state[slot] = true;

        // FIXME: Store/cache tm_struct and only recreate string description
        // when filetime changes?

        // char *state_file = get_state_file(slot, "uss");
        fsemu_log("Check %s\n", path);
        struct fsemu_path_stat_t buf;
        if (fsemu_path_stat(path, &buf) == 0) {
            // GDate date;
            // g_date_clear(&date, 1);
            // g_date_set_time_t(&date, buf.mtime);
            struct tm tm_struct;
            fsemu_time_localtime_r(&buf.mtime, &tm_struct);
            char strbuf[32];
            // g_date_strftime(strbuf, 32, "");
            strftime(strbuf, 32, "%Y-%m-%d %H:%M:%S", &tm_struct);
            // title = fs_strdup_printf("%d", buf.mtime);
            description = strdup(strbuf);
        } else {
            description = strdup("???");
        }
        // g_free(state_file);

    } else {
        printf("No state found for slot %d\n", slot);
        fsemu_savestate.has_state[slot] = false;
        // description = NULL;
    }

    if (fsemu_savestate.descriptions[slot]) {
        free(fsemu_savestate.descriptions[slot]);
    }
    fsemu_savestate.descriptions[slot] = description;

    if (path) {
        free(path);
    }
}

// FIXME: Consider changing this to return a const string instead, and let
// users of this function copy the string if desired, for better efficiency.
char *fsemu_savestate_description(int slot)
{
    char *description = fsemu_savestate.descriptions[slot];
    if (description) {
        return g_strdup(description);
    } else {
        return g_strdup("Empty");
    }
}

int fsemu_savestate_slots(void)
{
    fsemu_thread_assert_main();

    return FSEMU_SAVESTATE_SLOTS;
}

void fsemu_savestate_load(int slot)
{
    fsemu_thread_assert_main();
    fsemu_assert(slot >= 0 && slot <= FSEMU_SAVESTATE_MAX_SLOT);

    printf("FIXME: fsemu_savestate_load slot=%d\n", slot);
    // Slot 0 is also allowed, but not exposed in GUI.
    if (slot >= 0 && slot <= FSEMU_SAVESTATE_SLOTS) {
        fsemu_action_post_from_main(FSEMU_ACTION_LOADSTATE0 + slot);
    } else {
        fsemu_warning("Invalid savestate slot (%d)\n", slot);
    }
}

void fsemu_savestate_save(int slot)
{
    fsemu_thread_assert_main();
    fsemu_assert(slot >= 0 && slot <= FSEMU_SAVESTATE_MAX_SLOT);

    printf("FIXME: fsemu_savestate_save slot=%d\n", slot);
    // Slot 0 is also allowed, but not exposed in GUI.
    if (slot >= 0 && slot <= FSEMU_SAVESTATE_SLOTS) {
        fsemu_action_post_from_main(FSEMU_ACTION_SAVESTATE0 + slot);
        // The state will be saved, so we just register that it exists already.
        // EDIT: Emulator calls fsemu_savestate_update_slot
        // fsemu_savestate.has_state[slot] = true;
    } else {
        fsemu_warning("Invalid savestate slot (%d)\n", slot);
    }
}

#if 0
// FIXME: UNUSED?
void fsemu_savestate_handle_action(int action)
{
    printf("FIXME... ???\n");
    if (action >= FSEMU_ACTION_LOADSTATE0 &&
        action <= FSEMU_ACTION_LOADSTATE9) {
        int slot = action - FSEMU_ACTION_LOADSTATE0;
    }
    if (action >= FSEMU_ACTION_SAVESTATE0 &&
        action <= FSEMU_ACTION_SAVESTATE9) {
        int slot = action - FSEMU_ACTION_SAVESTATE0;
    }
}
#endif

void fsemu_savestate_init(void)
{
    if (fsemu_savestate.initialized) {
        return;
    }
    fsemu_savestate.initialized = true;
    fsemu_thread_init();
    fsemu_time_init();
    printf("fsemu_savestate_init\n");

    for (int i = 1; i <= FSEMU_SAVESTATE_MAX_SLOT; i++) {
        fsemu_savestate_update_slot(i);
        /*
        if (fsemu_savestate.path_function) {
            char *path = fsemu_savestate.path_function(i);
            if (fsemu_path_exists(path)) {
                fsemu_savestate.has_state[i] = true;
            } else {
                printf("No state found for slot %d\n", i);
            }
            free(path);
        }
        */
    }
}
