#ifndef FS_DATA_H
#define FS_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

int fs_data_init(const char *app_name, const char *dat_name);
int fs_data_file_content(const char *name, char **data, int *size);

#ifdef __cplusplus
}
#endif

#endif /* FS_DATA_H */
