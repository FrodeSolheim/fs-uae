#ifndef FSEMU_SAVESTATE_H_
#define FSEMU_SAVESTATE_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef char *(*fsuae_savestate_path_function_t)(int slot);

// ----------------------------------------------------------------------------
// Early functions
// ----------------------------------------------------------------------------

void fsemu_savestate_set_path_function(
    fsuae_savestate_path_function_t function);

// ----------------------------------------------------------------------------
// Callback functions
// ----------------------------------------------------------------------------

uintptr_t fsemu_savestate_on_load_finished(void *data);

uintptr_t fsemu_savestate_on_save_finished(void *data);

// ----------------------------------------------------------------------------

void fsemu_savestate_init(void);

bool fsemu_savestate_available(void);

bool fsemu_savestate_has_state(int slot);

void fsemu_savestate_update_slot(int slot);

char *fsemu_savestate_description(int slot);

int fsemu_savestate_slots(void);

void fsemu_savestate_load(int slot);

void fsemu_savestate_save(int slot);

// void fsemu_savestate_handle_action(int action);

extern int fsemu_savestate_log_level;

#define fsemu_savestate_log(format, ...)                                   \
    if (fsemu_likely(fsemu_savestate_log_level >= FSEMU_LOG_LEVEL_INFO)) { \
        fsemu_log("[FSE] [SAV] " format, ##__VA_ARGS__);                   \
    }

#define fsemu_savestate_log_debug(format, ...)                                \
    if (fsemu_unlikely(fsemu_savestate_log_level >= FSEMU_LOG_LEVEL_DEBUG)) { \
        fsemu_log_debug("[FSE] [SAV] " format, ##__VA_ARGS__);                \
    }

#define fsemu_savestate_log_warning(format, ...)                            \
    if (fsemu_likely(fsemu_savestate_log_level >= FSEMU_LOG_LEVEL_DEBUG)) { \
        fsemu_log_warning("[FSE] [SAV] " format, ##__VA_ARGS__);            \
    }

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_SAVESTATE_H_
