#ifndef FSLIB_LOG_H
#define FSLIB_LOG_H

#include "fsemu-common.h"
#include "fsemu-log.h"
#include "fslib-config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    FSLIB_LOG_LEVEL_ERROR = 1,
    FSLIB_LOG_LEVEL_WARNING,
    FSLIB_LOG_LEVEL_INFO,
    FSLIB_LOG_LEVEL_DEBUG,
    FSLIB_LOG_LEVEL_TRACE
} fslib_log_level_t;

typedef enum {
    FSLIB_LOG_ERROR = 1,
    FSLIB_LOG_WARNING,
    FSLIB_LOG_INFO,
    FSLIB_LOG_DEBUG,
    FSLIB_LOG_TRACE
} fslib_log_level_2_t;

#define FSLIB_LOG_FLAG_NO_LAST_UPDATE (1 << 0)

#define fslib_log_with_level(level, format, ...) \
    fsemu_log_with_level_and_flags(level, 0, format, ##__VA_ARGS__)

#define fslib_log(level, format, ...)                               \
    if (level <= FSLIB_LOG_LEVEL) {                                 \
        fsemu_log(FSLIB_LOG_PREFIX " " format "\n", ##__VA_ARGS__); \
    }

#if 0
#define fslib_log(format, ...) fsemu_log_with_level(FSLIB_LOG_LEVEL_INFO, format, ##__VA_ARGS__)
#endif

#define fslib_log_trace(format, ...) \
    fsemu_log_with_level(FSLIB_LOG_LEVEL_TRACE, format, ##__VA_ARGS__)
#define fslib_log_debug(format, ...) \
    fsemu_log_with_level(FSLIB_LOG_LEVEL_DEBUG, format, ##__VA_ARGS__)
#define fslib_log_info(format, ...) \
    fsemu_log_with_level(FSLIB_LOG_LEVEL_INFO, format, ##__VA_ARGS__)
#define fslib_log_warning(format, ...) \
    fsemu_log_with_level(FSLIB_LOG_LEVEL_WARNING, format, ##__VA_ARGS__)
#define fslib_log_error(format, ...) \
    fsemu_log_with_level(FSLIB_LOG_LEVEL_ERROR, format, ##__VA_ARGS__)

#define FSLIB_LOG(name, prefix, format, ...)                              \
    if (fsemu_likely(fslib_##name##_log_level >= FSLIB_LOG_LEVEL_INFO)) { \
        fsemu_log(prefix " " format, ##__VA_ARGS__);                      \
    }

#if 0
#define FSLIB_LOG_DEBUG(name, prefix, format, ...)                           \
    if (fsemu_unlikely(fslib_##name##_log_level >= FSLIB_LOG_LEVEL_DEBUG)) { \
        fsemu_log_debug(prefix " " format, ##__VA_ARGS__);                   \
    }

#define FSLIB_LOG_ERROR(name, prefix, format, ...)                         \
    if (fsemu_likely(fslib_##name##_log_level >= FSLIB_LOG_LEVEL_ERROR)) { \
        fsemu_log_error(prefix " " format, ##__VA_ARGS__);                 \
    }

#define FSLIB_LOG_INFO(name, prefix, format, ...)                         \
    if (fsemu_likely(fslib_##name##_log_level >= FSLIB_LOG_LEVEL_INFO)) { \
        fsemu_log_info(prefix " " format, ##__VA_ARGS__);                 \
    }

#define FSLIB_LOG_TRACE(name, prefix, format, ...)                           \
    if (fsemu_unlikely(fslib_##name##_log_level >= FSLIB_LOG_LEVEL_TRACE)) { \
        fsemu_log_trace(prefix " " format, ##__VA_ARGS__);                   \
    }

#define FSLIB_LOG_WARNING(name, prefix, format, ...)                         \
    if (fsemu_likely(fslib_##name##_log_level >= FSLIB_LOG_LEVEL_WARNING)) { \
        fsemu_log_warning(prefix " " format, ##__VA_ARGS__);                 \
    }
#endif

#ifdef __cplusplus
}
#endif

#endif  // FSLIB_LOG_H
