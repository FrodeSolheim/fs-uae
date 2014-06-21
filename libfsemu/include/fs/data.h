#ifndef FS_DATA_H_
#define FS_DATA_H_

int fs_data_init(const char *app_name, const char *dat_name);
int fs_data_file_content(const char *name, char **data, int *size);

#endif // FS_DATA_H_
