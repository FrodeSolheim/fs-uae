#ifndef FSEMU_DATA_H_
#define FSEMU_DATA_H_

#include "fsemu-config.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_data_init(void);
void fsemu_data_load(const char *name, void **data, int *data_size);

#ifdef __cplusplus
}
#endif

#define fsemu_data_log(format, ...) \
    fsemu_log("[FSEMU] [DATA ] " format, ##__VA_ARGS__)

#endif  // FSEMU_DATA_H_
