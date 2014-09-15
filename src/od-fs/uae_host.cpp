#define DO_NOT_INCLUDE_WINUAE_COMPAT_H

#include "sysconfig.h"
#include "sysdeps.h"

#include "uae/fs.h"
#include "uae/io.h"
#include "uae/glib.h"

int64_t uae_ftello64(FILE *stream) {
#if defined(HAVE_FTELLO64)
    return ftello64(stream);
#elif defined(HAVE_FSEEKO)
    // HAVE_FSEEKO implies ftello exists too
    return ftello(stream);
#else
    #error No ftell* with 64-bit offset
#endif
}

int uae_fseeko64(FILE *stream, int64_t offset, int whence) {
#if defined(HAVE_FSEEKO64)
    return fseeko64(stream, offset, whence);
#elif defined(HAVE_FSEEKO)
    return fseeko(stream, offset, whence);
#else
    #error No fseek* with 64-bit offset
#endif
}

char *uae_expand_path(const char *path) {
#if 0
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
    free(lower);
    if (replace) {
        const gchar *src = path + replace;
        printf("... %s\n", g_build_filename(g_get_home_dir(), src, NULL));
        return g_build_filename(g_get_home_dir(), src, NULL);
    }
    else {
        return g_strdup(path);
    }
#endif
    return g_strdup(path);
}

FILE *uae_tfopen(const char *path, const char *mode) {
    char *p = uae_expand_path(path);
    FILE *f = g_fopen(p, mode);
    g_free(p);
    return f;
}
