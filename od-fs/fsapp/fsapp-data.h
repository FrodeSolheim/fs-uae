/*
 * The program's data files: finding them, and loading and streaming them.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSAPP_DATA_H
#define FSAPP_DATA_H

#include <stddef.h>

#include "fsapp-config.h"
#include "fs/log.h"
#include "fsapp-stream.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsapp_data_init(void);
void fsapp_data_load(const char *name, void **data, int *data_size);

fsapp_stream_t *fsapp_data_open_stream(const char *name);

// The path of the data file relative, or NULL if it is not found. Freed with SDL_free.
char *fsapp_data_find_path(const char *relative);

// ----------------------------------------------------------------------------
// Logging
// ----------------------------------------------------------------------------

extern int fsapp_data_log_level;

#define fsapp_data_log(format, ...)                                                                \
    FS_LOG_INFO(fsapp_data_log_level, "[FSE] [DAT]", format, ##__VA_ARGS__)

#define fsapp_data_log_debug(format, ...)                                                          \
    FS_LOG_DEBUG(fsapp_data_log_level, "[FSE] [DAT]", format, ##__VA_ARGS__)

#define fsapp_data_log_error(format, ...)                                                          \
    FS_LOG_ERROR(fsapp_data_log_level, "[FSE] [DAT]", format, ##__VA_ARGS__)

#define fsapp_data_log_info(format, ...)                                                           \
    FS_LOG_INFO(fsapp_data_log_level, "[FSE] [DAT]", format, ##__VA_ARGS__)

#define fsapp_data_log_trace(format, ...)                                                          \
    FS_LOG_TRACE(fsapp_data_log_level, "[FSE] [DAT]", format, ##__VA_ARGS__)

#define fsapp_data_log_warning(format, ...)                                                        \
    FS_LOG_WARNING(fsapp_data_log_level, "[FSE] [DAT]", format, ##__VA_ARGS__)

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* FSAPP_DATA_H */
