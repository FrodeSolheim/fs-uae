#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// some code adapted from glib

#include <fs/base.h>
#include <fs/list.h>
#include <fs/string.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stddef.h>
#include <locale.h>
#include <string.h>
#include <locale.h>
#include <errno.h>
#include <ctype.h>              /* For tolower() */

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

int fs_str_equal (const void *v1, const void *v2) {
    const char *string1 = v1;
    const char *string2 = v2;
    return strcmp (string1, string2) == 0;
}

unsigned int fs_str_hash (const void *v) {
    const signed char *p;
    uint32_t h = 5381;
    for (p = v; *p != '\0'; p++) {
        h = (h << 5) + h + *p;
    }
    return h;
}

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
    if (src == NULL) {
        return NULL;
    }
    if (dest == NULL) {
        return NULL;
    }
#ifdef HAVE_STPCPY
    return stpcpy (dest, src);
#else
    register char *d = dest;
    register const char *s = src;
    do
        *d++ = *s;
    while (*s++ != '\0');

    return d - 1;
#endif
}

char *fs_strdup_vprintf(const char *format, va_list args) {
    int n;
    int size = 100;     /* Guess we need no more than 100 bytes. */
    char *p, *np;
    va_list ap;

    if ((p = malloc(size)) == NULL)
        return NULL;
    while (1) {
        va_copy(ap, args);
        n = vsnprintf(p, size, format, ap);
        va_end(ap);

        // If that worked, return the string.

        if (n > -1 && n < size)
            return p;

        // Else try again with more space.

        if (n > -1)     // glibc 2.1
            size = n+1; // precisely what is needed
        else            // glibc 2.0
            size *= 2;  // twice the old size

        if ((np = realloc (p, size)) == NULL) {
            free(p);
            return NULL;
        }
        else {
            p = np;
        }
    }
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

    memmove(string, start, strlen((char *) start) + 1);

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

double fs_ascii_strtod(const char *nptr, char **endptr) {
    char *fail_pos;
    double val;
    //struct lconv *locale_data;
    const char *decimal_point;
    int decimal_point_len;
    const char *p, *decimal_point_pos;
    const char *end = NULL; /* Silence gcc */
    int strtod_errno;

    if (nptr == NULL) {
        return 0;
    }

    fail_pos = NULL;

    //locale_data = localeconv();
    //decimal_point = locale_data->decimal_point;
    decimal_point = ".";
    decimal_point_len = strlen(decimal_point);

    //g_assert(decimal_point_len != 0);

    decimal_point_pos = NULL;
    end = NULL;

    if (decimal_point[0] != '.' || decimal_point[1] != 0) {
        p = nptr;
        /* Skip leading space */
        while (fs_ascii_isspace (*p))
            p++;

        /* Skip leading optional sign */
        if (*p == '+' || *p == '-')
            p++;

        if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
            p += 2;
            /* HEX - find the (optional) decimal point */

            while (fs_ascii_isxdigit (*p))
                p++;

            if (*p == '.')
                decimal_point_pos = p++;

            while (fs_ascii_isxdigit (*p))
                p++;

            if (*p == 'p' || *p == 'P')
                p++;
            if (*p == '+' || *p == '-')
                p++;
            while (fs_ascii_isdigit (*p))
                p++;

            end = p;
        }
        else if (fs_ascii_isdigit (*p) || *p == '.') {
            while (fs_ascii_isdigit (*p))
                p++;

            if (*p == '.')
                decimal_point_pos = p++;

            while (fs_ascii_isdigit (*p))
                p++;

            if (*p == 'e' || *p == 'E')
                p++;
            if (*p == '+' || *p == '-')
                p++;
            while (fs_ascii_isdigit (*p))
                p++;

            end = p;
        }
        /* For the other cases, we need not convert the decimal point */
    }

    if (decimal_point_pos) {
        char *copy, *c;

        /* We need to convert the '.' to the locale specific decimal point */
        copy = malloc(end - nptr + 1 + decimal_point_len);

        c = copy;
        memcpy(c, nptr, decimal_point_pos - nptr);
        c += decimal_point_pos - nptr;
        memcpy(c, decimal_point, decimal_point_len);
        c += decimal_point_len;
        memcpy(c, decimal_point_pos + 1, end - (decimal_point_pos + 1));
        c += end - (decimal_point_pos + 1);
        *c = 0;

        errno = 0;
        val = strtod(copy, &fail_pos);
        strtod_errno = errno;

        if (fail_pos) {
            if (fail_pos - copy > decimal_point_pos - nptr)
                fail_pos = (char *) nptr + (fail_pos - copy)
                        - (decimal_point_len - 1);
            else
                fail_pos = (char *) nptr + (fail_pos - copy);
        }

        free(copy);

    }
    else if (end) {
        char *copy;

        copy = malloc(end - (char *) nptr + 1);
        memcpy(copy, nptr, end - nptr);
        *(copy + (end - (char *) nptr)) = 0;

        errno = 0;
        val = strtod(copy, &fail_pos);
        strtod_errno = errno;

        if (fail_pos) {
            fail_pos = (char *) nptr + (fail_pos - copy);
        }

        free(copy);
    }
    else {
        errno = 0;
        val = strtod(nptr, &fail_pos);
        strtod_errno = errno;
    }

    if (endptr)
        *endptr = fail_pos;

    errno = strtod_errno;

    return val;
}

void fs_strfreev(char **str_array) {
    if (str_array) {
        int i;
        for (i = 0; str_array[i] != NULL ; i++) {
            free(str_array[i]);
        }
        free(str_array);
    }
}

char** fs_strsplit(const char *string, const char *delimiter, int max_tokens) {
    fs_list *string_list = NULL, *slist;
    char **str_array, *s;
    unsigned int n = 0;
    const char *remainder;

    if (string == NULL) {
        return NULL;
    }
    if (delimiter == NULL) {
        return NULL;
    }
    if (delimiter[0] == '\0') {
        return NULL;
    }

    if (max_tokens < 1) {
        max_tokens = INT_MAX;
    }

    remainder = string;
    s = strstr(remainder, delimiter);
    if (s) {
        size_t delimiter_len = strlen(delimiter);

        while (--max_tokens && s) {
            size_t len;

            len = s - remainder;
            string_list = fs_list_prepend(string_list,
                    fs_strndup(remainder, len));
            n++;
            remainder = s + delimiter_len;
            s = strstr(remainder, delimiter);
        }
    }
    if (*string) {
        n++;
        string_list = fs_list_prepend(string_list, fs_strdup(remainder));
    }

    str_array = fs_new(char*, n + 1);

    str_array[n--] = NULL;
    for (slist = string_list; slist; slist = slist->next)
        str_array[n--] = slist->data;

    fs_list_free(string_list);

    return str_array;
}
