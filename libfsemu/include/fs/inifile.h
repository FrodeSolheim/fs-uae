#ifndef FS_INIFILE_H
#define FS_INIFILE_H

struct fs_ini_file;
typedef struct fs_ini_file fs_ini_file;

fs_ini_file* fs_ini_file_open(const char *path);
fs_ini_file* fs_ini_file_open_data(const char *data, int size);
void fs_ini_file_destroy(fs_ini_file *ini_file);

char **fs_ini_file_get_groups(fs_ini_file *ini_file, int *length);

char **fs_ini_file_get_keys(fs_ini_file *ini_file, const char *group_name,
        int *length);

int fs_ini_file_has_group(fs_ini_file *ini_file, const char *group_name);

char *fs_ini_file_get_value(fs_ini_file *ini_file, const char *group_name,
        const char *key);
char *fs_ini_file_get_string(fs_ini_file *ini_file, const char *group_name,
        const char *key);

#endif /* FS_INIFILE_H */
