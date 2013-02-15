#ifndef FSDB_HOST_H_
#define FSDB_HOST_H_

typedef struct fsdb_file_info {
    int type;
    uint32_t mode;
    int days;
    int mins;
    int ticks;
    char *comment;

} fsdb_file_info;

FILE *fsdb_open_meta_file_for_path(const char *path, int r, int w,
        int e, int d);

void fsdb_init_file_info(fsdb_file_info *info);
int fsdb_get_file_info(const char *nname, fsdb_file_info *info);
int fsdb_set_file_info(const char *nname, fsdb_file_info *info);

extern int g_fsdb_debug;
extern int my_errno;

#endif // FSDB_HOST_H_
