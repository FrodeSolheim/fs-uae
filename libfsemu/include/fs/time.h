#ifndef FS_TIME_H_
#define FS_TIME_H_

#include <fs/defines.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

void fs_time_init(void);
struct tm *fs_localtime_r(const time_t *timep, struct tm *result);
struct tm *fs_gmtime_r(const time_t *timep, struct tm *result);
time_t fs_timegm(struct tm *tm);
int fs_get_local_time_offset(time_t time);

#ifdef __cplusplus
}
#endif

#endif // FS_TIME_H_
