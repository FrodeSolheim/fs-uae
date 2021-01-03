#ifndef FSEMU_LOG_H_
#define FSEMU_LOG_H_

#include <stdio.h>

#include "fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FSEMU_LOG_ERROR 1
#define FSEMU_LOG_WARN 2
#define FSEMU_LOG_INFO 3
#define FSEMU_LOG_DEBUG 4
#define FSEMU_LOG_TRACE 5

void fsemu_log_null(const char *format, ...);

void fsemu_log_string(const char *message);

#define fsemu_log printf
#define fsemu_log_trace printf
#define fsemu_log_debug printf
#define fsemu_log_info printf
#define fsemu_log_warning printf
#define fsemu_log_error printf
#define fsemu_warning printf

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_LOG_H_
