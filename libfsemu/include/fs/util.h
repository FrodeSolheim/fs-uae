#ifndef FS_UTIL_H
#define FS_UTIL_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/glib.h>
#include <fs/log.h>

static inline char *fs_utf8_to_latin1(const char *src, int len)
{
    return g_convert(src, len, "ISO-8859-1", "UTF-8", NULL, NULL, NULL);
}

static inline char *fs_utf8_from_latin1(const char *src, int len)
{
    return g_convert(src, len, "UTF-8", "ISO-8859-1", NULL, NULL, NULL);
}

static inline int fs_path_exists(const char *path)
{
    return g_file_test(path, G_FILE_TEST_EXISTS);
}

static inline int fs_path_is_file(const char *path)
{
    return g_file_test(path, G_FILE_TEST_IS_REGULAR);
}

static inline int fs_path_is_dir(const char *path)
{
    return g_file_test(path, G_FILE_TEST_IS_DIR);
}

#endif /* FS_UTIL_H */
