#ifndef FSLIB_DATA_H
#define FSLIB_DATA_H

#include "fslib-config.h"

#include "fslib-log.h"
#include "fslib-stream.h"

#ifdef __cplusplus
extern "C" {
#endif

void fslib_data_init_prefix(const char* prefix);
void fslib_data_init_dev_prefix(const char* dev_prefix);
void fslib_data_init_dev_test(const char* dev_tst);

// FIXME: Remove?
void fslib_data_init(void);

// FIXME: MOVE
// int fsemu_application_exe_path(char *buffer, int size);
// size_t fsemu_application_exe_dir_strlcpy(char *path, size_t size);

size_t fslib_data_copy_dir_path(char* path, size_t size);

bool fslib_data_development_mode(void);

bool fslib_data_load(const char* name, void** data, int* data_size);

fslib_stream_t* fslib_data_stream(const char* name);

char* fslib_data_file_path(const char* relative);

// ----------------------------------------------------------------------------
// Logging
// ----------------------------------------------------------------------------

extern int fslib_data_log_level;

#define fslib_data_log(format, ...)                                           \
    FSLIB_LOG(data, "[LIB] [DAT]", format, ##__VA_ARGS__)

#define fslib_data_log_debug(format, ...)                                     \
    FSLIB_LOG_DEBUG(data, "[LIB] [DAT]", format, ##__VA_ARGS__)

#define fslib_data_log_error(format, ...)                                     \
    FSLIB_LOG_ERROR(data, "[LIB] [DAT]", format, ##__VA_ARGS__)

#define fslib_data_log_info(format, ...)                                      \
    FSLIB_LOG_INFO(data, "[LIB] [DAT]", format, ##__VA_ARGS__)

#define fslib_data_log_trace(format, ...)                                     \
    FSLIB_LOG_TRACE(data, "[LIB] [DAT]", format, ##__VA_ARGS__)

#define fslib_data_log_warning(format, ...)                                   \
    FSLIB_LOG_WARNING(data, "[LIB] [DAT]", format, ##__VA_ARGS__)

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // FSLIB_DATA_H
