#ifndef FSEMU_PATH_H
#define FSEMU_PATH_H

#include "fslib-config.h"

#include "fslib-log.h"
#include "fslib-os.h"

#include <time.h>

#define FSLIB_PATH_MAX 4096

#ifdef __cplusplus
extern "C" {
#endif

// typedef struct fslib_path_stat_t {
//     int mode;
//     int64_t size;
//     time_t atime;
//     time_t mtime;
//     time_t ctime;
//     int atime_nsec;
//     int mtime_nsec;
//     int ctime_nsec;
//     int64_t blocks;
// } fslib_path_stat_t;

bool fslib_path_exists(const char* path);

bool fslib_path_executable_path(char* buffer, int buffer_size);
bool fslib_path_executable_dir(char* buffer, int buffer_size);

bool fslib_path_application_dir(char* buffer, int buffer_size);

// int fslib_path_stat(const char* path, fslib_path_stat_t* buf);
// int fslib_path_fstat(int fd, fslib_path_stat_t* buf);

extern int fslib_path_log_level;

// fslib_log(FSLIB_LOG_DEBUG, "Checking %s\n", path);

#define fslib_path_log(format, ...)                                           \
    FSLIB_LOG(path, "[LIB][PTH]", format, ##__VA_ARGS__)

#define fslib_path_log_debug(format, ...)                                     \
    FSLIB_LOG_DEBUG(path, "[LIB][PTH]", format, ##__VA_ARGS__)

#define fslib_path_log_error(format, ...)                                     \
    FSLIB_LOG_ERROR(path, "[LIB][PTH]", format, ##__VA_ARGS__)

#define fslib_path_log_info(format, ...)                                      \
    FSLIB_LOG_INFO(path, "[LIB][PTH]", format, ##__VA_ARGS__)

#define fslib_path_log_trace(format, ...)                                     \
    FSLIB_LOG_TRACE(path, "[LIB][PTH]", format, ##__VA_ARGS__)

#define fslib_path_log_warning(format, ...)                                   \
    FSLIB_LOG_WARNING(path, "[LIB][PTH]", format, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif // FSEMU_PATH_H
