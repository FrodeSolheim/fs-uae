#include <fs/unicode.h>
#include <glib.h>

char *fs_utf8_strdown(const char *str, ssize_t len) {
    return g_utf8_strdown(str, len);
}
