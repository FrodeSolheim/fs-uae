#ifndef FSEMU_LOG_H_
#define FSEMU_LOG_H_

#include <stdio.h>

#include "fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    FSEMU_LOG_LEVEL_ERROR = 1,
    FSEMU_LOG_LEVEL_WARNING,
    FSEMU_LOG_LEVEL_INFO,
    FSEMU_LOG_LEVEL_DEBUG,
    FSEMU_LOG_LEVEL_TRACE
} fsemu_log_level_t;

void fsemu_log_setup(void);

// void fsemu_log_null(const char *format, ...);

// void fsemu_log_string(const char *message);

void fsemu_log_with_level(int level, const char *format, ...);

#define fsemu_log(format, ...) \
    fsemu_log_with_level(FSEMU_LOG_LEVEL_INFO, format, ##__VA_ARGS__)
#define fsemu_log_debug(format, ...) \
    fsemu_log_with_level(FSEMU_LOG_LEVEL_DEBUG, format, ##__VA_ARGS__)
#define fsemu_log_info(format, ...) \
    fsemu_log_with_level(FSEMU_LOG_LEVEL_INFO, format, ##__VA_ARGS__)
#define fsemu_log_warning(format, ...) \
    fsemu_log_with_level(FSEMU_LOG_LEVEL_WARNING, format, ##__VA_ARGS__)
#define fsemu_log_error(format, ...) \
    fsemu_log_with_level(FSEMU_LOG_LEVEL_ERROR, format, ##__VA_ARGS__)
// FIXME
#define fsemu_warning(format, ...) \
    fsemu_log_with_level(FSEMU_LOG_LEVEL_WARNING, format, ##__VA_ARGS__)

#define FSEMU_LOG(name, prefix, format, ...)                              \
    if (fsemu_likely(fsemu_##name##_log_level >= FSEMU_LOG_LEVEL_INFO)) { \
        fsemu_log(prefix " " format, ##__VA_ARGS__);                      \
    }

#define FSEMU_LOG_DEBUG(name, prefix, format, ...)                           \
    if (fsemu_unlikely(fsemu_##name##_log_level >= FSEMU_LOG_LEVEL_DEBUG)) { \
        fsemu_log_debug(prefix " " format, ##__VA_ARGS__);                   \
    }

#define FSEMU_LOG_ERROR(name, prefix, format, ...)                         \
    if (fsemu_likely(fsemu_##name##_log_level >= FSEMU_LOG_LEVEL_ERROR)) { \
        fsemu_log_error(prefix " " format, ##__VA_ARGS__);                 \
    }

#define FSEMU_LOG_INFO(name, prefix, format, ...)                         \
    if (fsemu_likely(fsemu_##name##_log_level >= FSEMU_LOG_LEVEL_INFO)) { \
        fsemu_log_info(prefix " " format, ##__VA_ARGS__);                 \
    }

#define FSEMU_LOG_TRACE(name, prefix, format, ...)                           \
    if (fsemu_unlikely(fsemu_##name##_log_level >= FSEMU_LOG_LEVEL_TRACE)) { \
        fsemu_log_trace(prefix " " format, ##__VA_ARGS__);                   \
    }

#define FSEMU_LOG_WARNING(name, prefix, format, ...)                         \
    if (fsemu_likely(fsemu_##name##_log_level >= FSEMU_LOG_LEVEL_WARNING)) { \
        fsemu_log_warning(prefix " " format, ##__VA_ARGS__);                 \
    }

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_LOG_H_
