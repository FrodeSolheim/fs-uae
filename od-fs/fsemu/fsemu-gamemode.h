#ifndef FSEMU_GAMEMODE_H
#define FSEMU_GAMEMODE_H

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

#define fsemu_gamemode_log(format, ...)                                                            \
    FS_LOG_INFO(fsemu_gamemode_log_level, "[FSE] [GMD]", format, ##__VA_ARGS__)

#define fsemu_gamemode_log_debug(format, ...)                                                      \
    FS_LOG_DEBUG(fsemu_gamemode_log_level, "[FSE] [GMD]", format, ##__VA_ARGS__)

#define fsemu_gamemode_log_error(format, ...)                                                      \
    FS_LOG_ERROR(fsemu_gamemode_log_level, "[FSE] [GMD]", format, ##__VA_ARGS__)

#define fsemu_gamemode_log_info(format, ...)                                                       \
    FS_LOG_INFO(fsemu_gamemode_log_level, "[FSE] [GMD]", format, ##__VA_ARGS__)

#define fsemu_gamemode_log_trace(format, ...)                                                      \
    FS_LOG_TRACE(fsemu_gamemode_log_level, "[FSE] [GMD]", format, ##__VA_ARGS__)

#define fsemu_gamemode_log_warning(format, ...)                                                    \
    FS_LOG_WARNING(fsemu_gamemode_log_level, "[FSE] [GMD]", format, ##__VA_ARGS__)

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* FSEMU_GAMEMODE_H */
