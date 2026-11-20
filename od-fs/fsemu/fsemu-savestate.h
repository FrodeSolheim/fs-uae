#ifndef FSEMU_SAVESTATE_H
#define FSEMU_SAVESTATE_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef char *(*fsemu_savestate_path_function_t)(int slot);

// ----------------------------------------------------------------------------
// Early functions
// ----------------------------------------------------------------------------

void fsemu_savestate_set_path_function(fsemu_savestate_path_function_t function);

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

#define fsemu_savestate_log(format, ...)                                                           \
    FS_LOG_INFO(fsemu_savestate_log_level, "[FSE] [SAV]", format, ##__VA_ARGS__)

#define fsemu_savestate_log_debug(format, ...)                                                     \
    FS_LOG_DEBUG(fsemu_savestate_log_level, "[FSE] [SAV]", format, ##__VA_ARGS__)

// A warning, but logged only when the module's debug lines are (off by default, as it always was).
#define fsemu_savestate_log_warning(format, ...)                                                   \
    do {                                                                                           \
        if (FS_LOG_UNLIKELY(SDL_LOG_PRIORITY_DEBUG >= fsemu_savestate_log_level)) {                \
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN,                    \
                           "[FSE] [SAV] " format, ##__VA_ARGS__);                                  \
        }                                                                                          \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif /* FSEMU_SAVESTATE_H */
