#ifndef FSEMU_APPLICATION_H_
#define FSEMU_APPLICATION_H_

#include "fsemu-config.h"
#include "fsemu-types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Can be called before fsemu_application_init
void fsemu_application_set_name(const char *name);

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

#define fsemu_application_log(format, ...) \
    FSEMU_LOG(application, "[FSE] [APP]", format, ##__VA_ARGS__)

#define fsemu_application_log_debug(format, ...) \
    FSEMU_LOG_DEBUG(application, "[FSE] [APP]", format, ##__VA_ARGS__)

#define fsemu_application_log_error(format, ...) \
    FSEMU_LOG_ERROR(application, "[FSE] [APP]", format, ##__VA_ARGS__)

#define fsemu_application_log_info(format, ...) \
    FSEMU_LOG_INFO(application, "[FSE] [APP]", format, ##__VA_ARGS__)

#define fsemu_application_log_trace(format, ...) \
    FSEMU_LOG_TRACE(application, "[FSE] [APP]", format, ##__VA_ARGS__)

#define fsemu_application_log_warning(format, ...) \
    FSEMU_LOG_WARNING(application, "[FSE] [APP]", format, ##__VA_ARGS__)

// ----------------------------------------------------------------------------

#endif  // FSEMU_INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_APPLICATION_H_
