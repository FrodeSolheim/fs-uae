#ifndef FS_FILESYS_H_
#define FS_FILESYS_H_

#include <fs/defines.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

struct fs_stat {
    int mode;
    int64_t size;
    time_t atime;
    time_t mtime;
    time_t ctime;
};

int fs_stat(const char *path, struct fs_stat *buf);
int fs_fstat(int fd, struct fs_stat *buf);

struct fs_dir;
typedef struct fs_dir fs_dir;

fs_dir* fs_dir_open(const char *path, int flags);
const char *fs_dir_read_name(fs_dir *dir);
void fs_dir_close(fs_dir *dir);

FILE *fs_fopen(const char *path, const char *mode);
int fs_open(const char *path, int flags, int mode);

int fs_rmdir(const char *path);
int fs_unlink(const char *path);
int fs_rename(const char *old_path, const char *new_path);

int fs_mkdir(const char *path, int mode);
int fs_mkdir_with_parents(const char *path, int mode);

char *fs_path_get_dirname(const char *path) FS_MALLOC;
char *fs_path_get_basename(const char *path) FS_MALLOC;

const char *fs_get_home_dir();
const char *fs_get_documents_dir();
char *fs_get_current_dir();

int fs_path_exists(const char *path);
int fs_path_is_file(const char *path);
int fs_path_is_dir(const char *path);
int fs_path_is_absolute(const char *path);

char *fs_path_join(const char *first_element, ...) FS_MALLOC FS_SENTINEL;

#ifdef __cplusplus
}
#endif

#endif // FS_FILESYS_H_
