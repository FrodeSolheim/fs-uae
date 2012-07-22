#ifndef FS_STRING_H_
#define FS_STRING_H_

#include <fs/defines.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  FS_ASCII_ALNUM  = 1 << 0,
  FS_ASCII_ALPHA  = 1 << 1,
  FS_ASCII_CNTRL  = 1 << 2,
  FS_ASCII_DIGIT  = 1 << 3,
  FS_ASCII_GRAPH  = 1 << 4,
  FS_ASCII_LOWER  = 1 << 5,
  FS_ASCII_PRINT  = 1 << 6,
  FS_ASCII_PUNCT  = 1 << 7,
  FS_ASCII_SPACE  = 1 << 8,
  FS_ASCII_UPPER  = 1 << 9,
  FS_ASCII_XDIGIT = 1 << 10
} fs_ascii_type;

extern const uint16_t * const fs_ascii_table;

#define fs_ascii_isalnum(c) \
  ((fs_ascii_table[(guchar) (c)] & FS_ASCII_ALNUM) != 0)

#define fs_ascii_isalpha(c) \
  ((fs_ascii_table[(guchar) (c)] & FS_ASCII_ALPHA) != 0)

#define fs_ascii_iscntrl(c) \
  ((fs_ascii_table[(guchar) (c)] & FS_ASCII_CNTRL) != 0)

#define fs_ascii_isdigit(c) \
  ((fs_ascii_table[(guchar) (c)] & FS_ASCII_DIGIT) != 0)

#define fs_ascii_isgraph(c) \
  ((fs_ascii_table[(guchar) (c)] & FS_ASCII_GRAPH) != 0)

#define fs_ascii_islower(c) \
  ((fs_ascii_table[(guchar) (c)] & FS_ASCII_LOWER) != 0)

#define fs_ascii_isprint(c) \
  ((fs_ascii_table[(guchar) (c)] & FS_ASCII_PRINT) != 0)

#define fs_ascii_ispunct(c) \
  ((fs_ascii_table[(guchar) (c)] & FS_ASCII_PUNCT) != 0)

#define fs_ascii_isspace(c) \
  ((fs_ascii_table[(guchar) (c)] & FS_ASCII_SPACE) != 0)

#define fs_ascii_isupper(c) \
  ((fs_ascii_table[(guchar) (c)] & FS_ASCII_UPPER) != 0)

#define fs_ascii_isxdigit(c) \
  ((fs_ascii_table[(guchar) (c)] & FS_ASCII_XDIGIT) != 0)


char *fs_strdup_printf(const char *format, ...) FS_MALLOC;
char *fs_strconcat(const char *string1, ...) FS_SENTINEL;

int fs_ascii_strcasecmp(const char *s1, const char *s2);

char fs_ascii_tolower(char c);
char fs_ascii_toupper(char c);
char *fs_ascii_strdown(const char *str, ssize_t len);
char *fs_ascii_strup(const char *str, ssize_t len);

//double fs_ascii_strtod(const char *nptr, char **endptr);
//char *fs_ascii_formatd(char *buffer, int buf_len, const char *format, double d);

int fs_str_has_suffix(const char *str, const char *suffix);
int fs_str_has_prefix(const char *str, const char *prefix);

char* fs_strdup(const char *str) FS_MALLOC;
char* fs_strndup(const char *str, size_t n) FS_MALLOC;

char *fs_strdelimit(char *string, const char *delimiters, char new_delim);

char *fs_strchug(char *string);
char *fs_strchomp(char *string);

#ifdef __cplusplus
}
#endif

#endif // FS_STRING_H_
