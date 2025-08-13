#ifndef FSEMU_PATH_H
#define FSEMU_PATH_H

#include <time.h>

#include "fslib-config.h"
#include "fslib-log.h"
#include "fslib-os.h"

#define FSLIB_PATH_MAX 4096

#ifdef __cplusplus
extern "C" {
#endif

bool fslib_path_exists(const char* path);

bool fslib_path_executable_path(char* buffer, int buffer_size);
bool fslib_path_executable_dir(char* buffer, int buffer_size);

bool fslib_path_application_dir(char* buffer, int buffer_size);

extern int fslib_path_log_level;

#define fslib_path_log(format, ...) FSLIB_LOG(path, "[LIB][PTH]", format, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_PATH_H
