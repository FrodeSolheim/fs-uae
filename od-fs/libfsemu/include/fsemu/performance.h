#ifndef FSEMU_PERFORMANCE_H_
#define FSEMU_PERFORMANCE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_performance_init(void);
void fsemu_performance_log(char series[4], int64_t value);
void fsemu_performance_log_with_time(char series[4], int64_t value, int64_t time);
void fsemu_performance_flush(void);

#ifdef __cplusplus
}
#endif

#endif // FSEMU_PERFORMANCE_H_
