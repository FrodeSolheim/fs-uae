#ifndef UAE_OD_FS_FSDB_HOST_H
#define UAE_OD_FS_FSDB_HOST_H

#include <glib.h>

typedef struct fsdb_file_info {
    int type;
    uint32_t mode;
    int days;
    int mins;
    int ticks;
    char *comment;

} fsdb_file_info;

void fsdb_init_file_info(fsdb_file_info *info);
int fsdb_set_file_info(const char *nname, fsdb_file_info *info);

extern int g_fsdb_debug;

// Converts a UTF-8 host file name to ISO-8859-1 (Amiga) and back. The result is freed with g_free,
// and is NULL if the name cannot be converted.

static inline char *fsdb_utf8_to_latin1(const char *src, int len)
{
    return g_convert(src, len, "ISO-8859-1", "UTF-8", NULL, NULL, NULL);
}

static inline char *fsdb_utf8_from_latin1(const char *src, int len)
{
    return g_convert(src, len, "UTF-8", "ISO-8859-1", NULL, NULL, NULL);
}
extern int my_errno;

#endif // UAE_OD_FS_FSDB_HOST_H
