#ifndef FS_LOG_H_
#define FS_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

void fs_log(const char *format, ...);
void fs_log_string(const char *str);
void fs_log_enable_stdout();

void fs_config_set_log_file(const char *path);

#ifdef __cplusplus
}
#endif

#endif // FS_LOG_H_
