#ifndef FSEMU_GAMEMODE_H_
#define FSEMU_GAMEMODE_H_

#include "fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_gamemode_init(void);
void fsemu_gamemode_suspend(void);
void fsemu_gamemode_resume(void);
void fsemu_gamemode_check_linux_cpu_governor(void);

// ----------------------------------------------------------------------------
// Logging

extern int fsemu_gamemode_log_level;

#define fsemu_gamemode_log(format, ...) FSEMU_LOG(gamemode, "[FSE] [GMD]", format, ##__VA_ARGS__)

#define fsemu_gamemode_log_debug(format, ...) \
    FSEMU_LOG_DEBUG(gamemode, "[FSE] [GMD]", format, ##__VA_ARGS__)

#define fsemu_gamemode_log_error(format, ...) \
    FSEMU_LOG_ERROR(gamemode, "[FSE] [GMD]", format, ##__VA_ARGS__)

#define fsemu_gamemode_log_info(format, ...) \
    FSEMU_LOG_INFO(gamemode, "[FSE] [GMD]", format, ##__VA_ARGS__)

#define fsemu_gamemode_log_trace(format, ...) \
    FSEMU_LOG_TRACE(gamemode, "[FSE] [GMD]", format, ##__VA_ARGS__)

#define fsemu_gamemode_log_warning(format, ...) \
    FSEMU_LOG_WARNING(gamemode, "[FSE] [GMD]", format, ##__VA_ARGS__)

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_GAMEMODE_H_
