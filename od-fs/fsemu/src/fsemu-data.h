#ifndef FSEMU_DATA_H_
#define FSEMU_DATA_H_

#include "fsemu-config.h"
#include "fsemu-log.h"
#include "fsemu-stream.h"

#ifdef __cplusplus
extern "C" {
#endif

size_t fsemu_data_dir_strlcpy(char *path, size_t size);

// FIXME: MOVE
int fsemu_application_exe_path(char *buffer, int size);
size_t fsemu_application_exe_dir_strlcpy(char *path, size_t size);

bool fsemu_data_development_mode(void);

void fsemu_data_init(void);
void fsemu_data_load(const char *name, void **data, int *data_size);

fsemu_stream_t *fsemu_data_stream(const char *name);

char *fsemu_data_file_path(const char *relative);

// ----------------------------------------------------------------------------
// Logging
// ----------------------------------------------------------------------------

extern int fsemu_data_log_level;

#define fsemu_data_log(format, ...) \
    FSEMU_LOG(data, "[FSE] [DAT]", format, ##__VA_ARGS__)

#define fsemu_data_log_debug(format, ...) \
    FSEMU_LOG_DEBUG(data, "[FSE] [DAT]", format, ##__VA_ARGS__)

#define fsemu_data_log_error(format, ...) \
    FSEMU_LOG_ERROR(data, "[FSE] [DAT]", format, ##__VA_ARGS__)

#define fsemu_data_log_info(format, ...) \
    FSEMU_LOG_INFO(data, "[FSE] [DAT]", format, ##__VA_ARGS__)

#define fsemu_data_log_trace(format, ...) \
    FSEMU_LOG_TRACE(data, "[FSE] [DAT]", format, ##__VA_ARGS__)

#define fsemu_data_log_warning(format, ...) \
    FSEMU_LOG_WARNING(data, "[FSE] [DAT]", format, ##__VA_ARGS__)

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_DATA_H_
