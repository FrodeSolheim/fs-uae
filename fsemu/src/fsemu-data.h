#ifndef FSEMU_DATA_H_
#define FSEMU_DATA_H_

#include "fsemu-config.h"
#include "fsemu-stream.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_data_init(void);
void fsemu_data_load(const char *name, void **data, int *data_size);

fsemu_stream_t *fsemu_data_stream(const char *name);

char *fsemu_data_file_path(const char *relative);

#ifdef __cplusplus
}
#endif

#define fsemu_data_log(format, ...) \
    fsemu_log("[FSE] [DAT] " format, ##__VA_ARGS__)

#endif  // FSEMU_DATA_H_
