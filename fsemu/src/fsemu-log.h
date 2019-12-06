#ifndef FSEMU_LOG_H_
#define FSEMU_LOG_H_

#include <stdio.h>

#include "fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

void fsemu_log_null(const char *format, ...);

#define fsemu_log printf
#define fsemu_warning printf

#endif  // FSEMU_LOG_H_
