#define DO_NOT_INCLUDE_WINUAE_COMPAT_H

#include "sysconfig.h"
#include "sysdeps.h"

#include <stdio.h>
#include <glib.h>
#include <glib/gstdio.h>

int64_t uae_ftello64(FILE *stream) {
#ifdef HAVE_FTELLO64
    return ftello64(stream);
#else
    return ftello(stream);
#endif
}

int uae_fseeko64(FILE *stream, int64_t offset, int whence) {
#ifdef HAVE_FSEEKO64
    return fseeko64(stream, offset, whence);
#else
    return fseeko(stream, offset, whence);
#endif
}

gchar *uae_expand_path(const gchar *path) {
    /*
    write_log("expand_path %s\n", path);
    gchar* lower = g_ascii_strdown(path, -1);
    int replace = 0;
    if (g_str_has_prefix(lower, "~/") || g_str_has_prefix(lower, "~\\")) {
        replace = 2;
    }
    if (g_str_has_prefix(lower, "$home/") ||
            g_str_has_prefix(lower, "$home\\")) {
        replace = 6;
    }
    g_free(lower);
    if (replace) {
        const gchar *src = path + replace;
        printf("... %s\n", g_build_filename(g_get_home_dir(), src, NULL));
        return g_build_filename(g_get_home_dir(), src, NULL);
    }
    else {
        return g_strdup(path);
    }
    */
    return g_strdup(path);
}

FILE *uae_fopen(const char *path, const char *mode) {
    char *p = uae_expand_path(path);
    FILE *f = g_fopen(p, mode);
    g_free(p);
    return f;
}
