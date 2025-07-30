#ifndef FS_CONF_H
#define FS_CONF_H

#include <stdbool.h>
#include <fs/inifile.h>

#define FS_CONFIG_NONE 0x12345678
#define FS_CONFIG_AUTO "auto"

#ifdef __cplusplus
extern "C" {
#endif

void fse_init_conf(void);

int fs_config_read_file(const char *path, int force);
void fs_config_parse_ini_file(fs_ini_file *ini_file, int force);
void fs_config_parse_options(int argc, char **argv);

void fs_config_parse_options(int argc, char **argv);

bool fs_config_exists(const char *key);

bool fs_config_check_auto(const char *key, const char *value);
bool fs_config_check_enabled(const char *key, const char *value);
bool fs_config_check_disabled(const char *key, const char *value);

int fs_config_get_boolean(const char *key);
int fs_config_get_int(const char *key);
int fs_config_get_int_clamped(const char *key, int min, int max);
double fs_config_get_double(const char *key);
double fs_config_get_double_clamped(const char *key, double min, double max);
char *fs_config_get_string(const char *key);
const char *fs_config_get_const_string(const char *key);

void fs_config_set_string(const char *key, const char *value);
void fs_config_set_string_if_unset(const char *key, const char *value);

static inline bool fs_config_true(const char *key)
{
    return fs_config_get_int(key) == 1;
}

static inline bool fs_config_false(const char *key)
{
    return fs_config_get_int(key) == 0;
}

#ifdef __cplusplus
}
#endif

#endif /* FS_CONF_H */
