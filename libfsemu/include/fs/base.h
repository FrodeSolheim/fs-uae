#ifndef FS_BASE_H
#define FS_BASE_H

#include <fs/malloc.h>
#include <fs/defines.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FS_NO_ERROR 0

struct fs_time_val {
  int32_t tv_sec;
  int32_t tv_usec;
};

typedef struct fs_time_val fs_time_val;

const char *fs_get_application_name(void);
void fs_set_application_name(const char *application_name);
const char *fs_get_prgname(void);
void fs_set_prgname(const char *prgname);

const char *fs_data_dir(void);
void fs_set_data_dir(const char *path);

char *fs_get_data_file(const char *relative);
char *fs_get_program_data_file(const char *relative);
int fs_get_program_data(const char *relative, char **data, int *size);

void fs_get_current_time(fs_time_val *result);
void fs_time_val_add(fs_time_val *tv, int usec);
int64_t fs_get_real_time(void);
int64_t fs_get_monotonic_time(void);

void fs_set_argv(int argc, char* argv[]);
int fs_get_application_exe_path(char *buffer, int size);
int fs_get_application_exe_dir(char *buffer, int size);

const char *fs_get_home_dir(void);
const char *fse_user_config_dir(void);
const char *fse_user_data_dir(void);

#ifdef __cplusplus
}
#endif

#endif /* FS_BASE_H */
