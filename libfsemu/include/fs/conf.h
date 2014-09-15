#ifndef FS_CONFIG_H_
#define FS_CONFIG_H_

#define FS_CONFIG_NONE 0x12345678

#ifdef __cplusplus
extern "C" {
#endif

int fs_config_get_boolean(const char *key);
int fs_config_get_int(const char *key);
int fs_config_get_int_clamped(const char *key, int min, int max);
double fs_config_get_double(const char *key);
double fs_config_get_double_clamped(const char *key, double min, double max);
char *fs_config_get_string(const char *key);
const char *fs_config_get_const_string(const char *key);

int fs_config_read_file(const char *path, int force);
void fs_config_parse_options(int argc, char **argv);
void fs_config_set_string(const char *key, const char *value);
void fs_config_set_string_if_unset(const char *key, const char *value);

#ifdef __cplusplus
}
#endif

#endif // FS_CONFIG_H_
