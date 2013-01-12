#include <fs/unicode.h>
#include <fs/string.h>

#ifdef USE_GLIB
#include <glib.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef USE_ICONV
#include <iconv.h>
#endif

char *fs_utf8_strdown(const char *str, ssize_t len) {
#ifdef USE_GLIB
    return g_utf8_strdown(str, len);
#else
    // FIXME: should handle unicode characters
    return fs_ascii_strdown(str, len);
#endif
}

char *fs_utf8_strup(const char *str, ssize_t len) {
#ifdef USE_GLIB
    return g_utf8_strup(str, len);
#else
    // FIXME: should handle unicode characters
    return fs_ascii_strup(str, len);
#endif
}

static char* convert(const char *src, int len, const char *from_code,
        const char *to_code ){
#ifdef USE_GLIB
    gsize bytes_read, bytes_written;
    char *result = g_convert(src, len, to_code, from_code, &bytes_read,
            &bytes_written, NULL);
    return result;

#elif defined(ANDROID)
    // FIXME:
    //#warning no character conversion routine
    return fs_strdup(src);
#else
    iconv_t converter = iconv_open(to_code, from_code);
    if (converter == (iconv_t) -1) {
        return NULL;
    }

    if (len == -1) {
        len = strlen(src);
    }

    int size = 128;
    char *dst = malloc(size + 1);
    size_t outbytesleft;
    while(1) {
        char *inbuf = (char*) src;
        char *outbuf = dst;
        size_t inbytesleft = len;
        outbytesleft = size;
        size_t result = iconv(converter, &inbuf, &inbytesleft, &outbuf,
                &outbytesleft);
        if (result == -1) {
            if (errno == E2BIG) {
                size *= 2;
                dst = realloc(dst, size + 1);
                continue;
            }
            else {
                // invalid multibyte sequence is encountered in the input
                free(dst);
                iconv_close(converter);
                return NULL;
            }
        }
        break;
    }
    iconv_close(converter);
    // FIXME: is output already NULL-terminated?
    dst[size - outbytesleft] = 0;
    return dst;
#endif
}

char *fs_utf8_to_latin1(const char *src, int len) {
    return convert(src, len, "UTF-8", "ISO-8859-1");
}

char *fs_utf8_from_latin1(const char *src, int len) {
    return convert(src, len, "ISO-8859-1", "UTF-8");
}
