
// some code adapted from glib

#include <fs/base.h>
#include <fs/string.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <locale.h>
#include <string.h>
#include <locale.h>
#include <errno.h>
#include <ctype.h>              /* For tolower() */

#include <glib.h>

#define FS_STR_DELIMITERS       "_-|> <."

#define ISSPACE(c)              ((c) == ' ' || (c) == '\f' || (c) == '\n' || \
                                 (c) == '\r' || (c) == '\t' || (c) == '\v')
#define ISUPPER(c)              ((c) >= 'A' && (c) <= 'Z')
#define ISLOWER(c)              ((c) >= 'a' && (c) <= 'z')
#define ISALPHA(c)              (ISUPPER (c) || ISLOWER (c))
#define TOUPPER(c)              (ISLOWER (c) ? (c) - 'a' + 'A' : (c))
#define TOLOWER(c)              (ISUPPER (c) ? (c) - 'A' + 'a' : (c))

static const uint16_t ascii_table_data[256] = { 0x004, 0x004, 0x004, 0x004,
        0x004, 0x004, 0x004, 0x004, 0x004, 0x104, 0x104, 0x004, 0x104, 0x104,
        0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004,
        0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x140, 0x0d0,
        0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0,
        0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x459, 0x459, 0x459, 0x459, 0x459, 0x459,
        0x459, 0x459, 0x459, 0x459, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0,
        0x0d0, 0x653, 0x653, 0x653, 0x653, 0x653, 0x653, 0x253, 0x253, 0x253,
        0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253,
        0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x0d0, 0x0d0, 0x0d0,
        0x0d0, 0x0d0, 0x0d0, 0x473, 0x473, 0x473, 0x473, 0x473, 0x473, 0x073,
        0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073,
        0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x0d0,
        0x0d0, 0x0d0, 0x0d0, 0x004
/* the upper 128 are all zeroes */
};

const uint16_t * const fs_ascii_table = ascii_table_data;

int fs_ascii_strcasecmp(const char *s1, const char *s2) {
    int c1, c2;

    if (s1 == NULL) {
        return 0;
    }
    if (s2 == NULL) {
        return 0;
    }

    while (*s1 && *s2) {
        c1 = (int) (unsigned char) TOLOWER (*s1);
        c2 = (int) (unsigned char) TOLOWER (*s2);
        if (c1 != c2) return (c1 - c2);
        s1++;
        s2++;
    }

    return (((int) (unsigned char) *s1) - ((int) (unsigned char) *s2));
}

char* fs_strdup(const char *str) {
    char *new_str;
    size_t length;

    if (str) {
        length = strlen(str) + 1;
        new_str = fs_new(char, length);
        memcpy(new_str, str, length);
    }
    else {
        new_str = NULL;
    }

    return new_str;
}

void * fs_memdup(const void * mem, unsigned int byte_size) {
    void * new_mem;

    if (mem) {
        new_mem = malloc(byte_size);
        memcpy(new_mem, mem, byte_size);
    }
    else {
        new_mem = NULL;
    }

    return new_mem;
}

char* fs_strndup(const char *str, size_t n) {
    char *new_str;

    if (str) {
        new_str = fs_new (char, n + 1);
        strncpy(new_str, str, n);
        new_str[n] = '\0';
    }
    else
        new_str = NULL;

    return new_str;
}

char *fs_strnfill(size_t length, char fill_char) {
    char *str;

    str = fs_new (char, length + 1);
    memset(str, (unsigned char) fill_char, length);
    str[length] = '\0';

    return str;
}

char *fs_stpcpy(char *dest, const char *src) {
#ifdef HAVE_STPCPY
    g_return_val_if_fail (dest != NULL, NULL);
    g_return_val_if_fail (src != NULL, NULL);
    return stpcpy (dest, src);
#else
    register char *d = dest;
    register const char *s = src;

    if (src == NULL) {
        return NULL;
    }
    if (dest == NULL) {
        return NULL;
    }
    do
        *d++ = *s;
    while (*s++ != '\0');

    return d - 1;
#endif
}

char* fs_strdup_vprintf(const char *format, va_list args) {
    /*
    char *string = NULL;
    g_vasprintf(&string, format, args);
    return string;
    */
    return g_strdup_vprintf(format, args);
}

char* fs_strdup_printf(const char *format, ...) {
    char *buffer;
    va_list args;

    va_start(args, format);
    buffer = fs_strdup_vprintf(format, args);
    va_end(args);

    return buffer;
}

char* fs_strconcat(const char *string1, ...) {
    size_t l;
    va_list args;
    char *s;
    char *concat;
    char *ptr;

    if (!string1) return NULL;

    l = 1 + strlen(string1);
    va_start(args, string1);
    s = va_arg (args, char*);
    while (s) {
        l += strlen(s);
        s = va_arg (args, char*);
    }
    va_end(args);

    concat = fs_new (char, l);
    ptr = concat;

    ptr = fs_stpcpy(ptr, string1);
    va_start(args, string1);
    s = va_arg (args, char*);
    while (s) {
        ptr = fs_stpcpy(ptr, s);
        s = va_arg (args, char*);
    }
    va_end(args);

    return concat;
}

int fs_str_has_suffix(const char *str, const char *suffix) {
    int str_len;
    int suffix_len;

    if (str == NULL) {
        return 0;
    }
    if (suffix == NULL) {
        return 0;
    }

    str_len = strlen(str);
    suffix_len = strlen(suffix);

    if (str_len < suffix_len) return FALSE;

    return strcmp(str + str_len - suffix_len, suffix) == 0;
}

int fs_str_has_prefix(const char *str, const char *prefix) {
    int str_len;
    int prefix_len;

    if (str == NULL) {
        return 0;
    }
    if (prefix == NULL) {
        return 0;
    }

    str_len = strlen(str);
    prefix_len = strlen(prefix);

    if (str_len < prefix_len) return FALSE;

    return strncmp(str, prefix, prefix_len) == 0;
}

char *fs_strdelimit(char *string, const char *delimiters, char new_delim) {
    register char *c;
    if (string == NULL) {
        return NULL;
    }

    if (!delimiters) delimiters = FS_STR_DELIMITERS;

    for (c = string; *c; c++) {
        if (strchr(delimiters, *c)) *c = new_delim;
    }

    return string;
}

char *fs_strchug(char *string) {
    unsigned char *start;

    if (string == NULL) {
        return NULL;
    }

    for (start = (unsigned char*) string; *start && fs_ascii_isspace(*start);
            start++)
        ;

    g_memmove(string, start, strlen((char *) start) + 1);

    return string;
}

char *fs_strchomp(char *string) {
    size_t len;

    if (string == NULL) {
        return NULL;
    }

    len = strlen(string);
    while (len--) {
        if (fs_ascii_isspace((unsigned char) string[len])) string[len] = '\0';
        else
            break;
    }

    return string;
}

char fs_ascii_tolower(char c) {
    return fs_ascii_isupper(c) ? c - 'A' + 'a' : c;
}

char fs_ascii_toupper(char c) {
    return fs_ascii_islower(c) ? c - 'a' + 'A' : c;
}

char *fs_ascii_strdown(const char *str, ssize_t len) {
    char *result, *s;

    if (str == NULL) {
        return NULL;
    }

    if (len < 0) len = strlen(str);

    result = fs_strndup(str, len);
    for (s = result; *s; s++)
        *s = fs_ascii_tolower(*s);

    return result;
}

char *fs_ascii_strup(const char *str, ssize_t len) {
    char *result, *s;

    if (str == NULL) {
        return NULL;
    }

    if (len < 0) len = strlen(str);

    result = fs_strndup(str, len);
    for (s = result; *s; s++)
        *s = fs_ascii_toupper(*s);

    return result;
}
