#ifndef FSEMU_APPLICATION_H
#define FSEMU_APPLICATION_H

#include "fsemu-config.h"
#include "fsemu-types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Can be called before fsemu_application_init
void fsemu_application_set_name(const char *name);

// Can be called before fsemu_application_init
void fsemu_application_set_base_dir(const char *dir_path);

// Can be called before fsemu_application_init
void fsemu_application_set_base_dir_name(const char *dir_name);

void fsemu_application_init(void);

const char *fsemu_application_name(void);

const char *fsemu_application_base_dir(void);

#ifdef FSEMU_INTERNAL

// ----------------------------------------------------------------------------
// Logging
// ----------------------------------------------------------------------------

extern int fsemu_application_log_level;

#define fsemu_application_log(format, ...)                                                         \
    FS_LOG_INFO(fsemu_application_log_level, "[FSE] [APP]", format, ##__VA_ARGS__)

#define fsemu_application_log_debug(format, ...)                                                   \
    FS_LOG_DEBUG(fsemu_application_log_level, "[FSE] [APP]", format, ##__VA_ARGS__)

#define fsemu_application_log_error(format, ...)                                                   \
    FS_LOG_ERROR(fsemu_application_log_level, "[FSE] [APP]", format, ##__VA_ARGS__)

#define fsemu_application_log_info(format, ...)                                                    \
    FS_LOG_INFO(fsemu_application_log_level, "[FSE] [APP]", format, ##__VA_ARGS__)

#define fsemu_application_log_trace(format, ...)                                                   \
    FS_LOG_TRACE(fsemu_application_log_level, "[FSE] [APP]", format, ##__VA_ARGS__)

#define fsemu_application_log_warning(format, ...)                                                 \
    FS_LOG_WARNING(fsemu_application_log_level, "[FSE] [APP]", format, ##__VA_ARGS__)

// ----------------------------------------------------------------------------

#endif // FSEMU_INTERNAL

#ifdef __cplusplus
}
#endif

#endif /* FSEMU_APPLICATION_H */
