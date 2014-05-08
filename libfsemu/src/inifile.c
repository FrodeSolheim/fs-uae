#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/inifile.h>

#include <stdlib.h>
#include <fs/hashtable.h>

struct fs_ini_file {
    fs_hash_table *groups;
};

static int ini_parse(const char* filename,
              int (*handler)(void* user, const char* section,
                             const char* name, const char* value),
              void* user);

static void free_group(void *data) {
    fs_hash_table_destroy((fs_hash_table*) data);
}

fs_ini_file* fs_ini_file_create() {
    fs_ini_file *ini_file = malloc(sizeof(ini_file));
    // FIXME: use full
    ini_file->groups = fs_hash_table_new_full(fs_str_hash, fs_str_equal,
            free, free_group);
    return ini_file;
}

static int handler(void *user_data, const char *section, const char *name,
                   const char *value) {
    fs_ini_file *ini_file = (fs_ini_file*) user_data;
    fs_hash_table *group = fs_hash_table_lookup(ini_file->groups, section);
    if (group == NULL) {
        group = fs_hash_table_new_full(fs_str_hash, fs_str_equal, free, free);
        fs_hash_table_insert(ini_file->groups, fs_strdup(section), group);
    }
    fs_hash_table_insert(group, fs_strdup(name), fs_strdup(value));
    return 1;
}

fs_ini_file* fs_ini_file_open(const char *path) {
    fs_ini_file *ini_file = fs_ini_file_create();

    int result = ini_parse(path, handler, ini_file);
    if (result == -1) {
        // could not open file
        fs_ini_file_destroy(ini_file);
        return NULL;
    }

    return ini_file;
}

void fs_ini_file_destroy(fs_ini_file *ini_file) {

    fs_hash_table_destroy(ini_file->groups);
    free(ini_file);
}

static char **retrieve_keys(fs_hash_table *hash_table, int *length) {
    int size = fs_hash_table_size(hash_table);
    char **result = malloc(sizeof(char*) * (size + 1));
    if (length != NULL) {
        *length = size;
    }
    result[size] = NULL;
    fs_list *key_list = fs_hash_table_get_keys(hash_table);
    fs_list *item = key_list;
    int k = 0;
    while (item) {
        result[k++] = fs_strdup(item->data);
        item = item->next;
    }
    fs_list_free(key_list);
    return result;
}

char **fs_ini_file_get_groups(fs_ini_file *ini_file, int *length) {
    return retrieve_keys(ini_file->groups, length);
}

char **fs_ini_file_get_keys(fs_ini_file *ini_file, const char *group_name,
        int *length) {
    fs_hash_table *group = fs_hash_table_lookup(ini_file->groups, group_name);
    if (group == NULL) {
        return NULL;
    }
    return retrieve_keys(group, length);
}

int fs_ini_file_has_group(fs_ini_file *ini_file, const char *group_name) {
    return fs_hash_table_lookup(ini_file->groups, group_name) != NULL;
}

char *fs_ini_file_get_value(fs_ini_file *ini_file, const char *group_name,
        const char *key) {
    fs_hash_table *group = fs_hash_table_lookup(ini_file->groups, group_name);
    if (group == NULL) {
        return NULL;
    }
    char *value = fs_hash_table_lookup(group, key);
    if (value == NULL) {
        return NULL;
    }
    return fs_strdup(value);
}

char *fs_ini_file_get_string(fs_ini_file *ini_file, const char *group_name,
        const char *key) {
    char *value = fs_ini_file_get_value(ini_file, group_name, key);
    // FIXME: fix escape sequences
    return value;
}

/* inih -- simple .INI file parser

inih is released under the New BSD license (see LICENSE.txt). Go to the project
home page for more info:

http://code.google.com/p/inih/

*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>


/* inih -- simple .INI file parser

inih is released under the New BSD license (see LICENSE.txt). Go to the project
home page for more info:

http://code.google.com/p/inih/

*/

#ifndef __INI_H__
#define __INI_H__

/* Make this header file easier to include in C++ code */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

/* Parse given INI-style file. May have [section]s, name=value pairs
   (whitespace stripped), and comments starting with ';' (semicolon). Section
   is "" if name=value pair parsed before any section heading. name:value
   pairs are also supported as a concession to Python's ConfigParser.

   For each name=value pair parsed, call handler function with given user
   pointer as well as section, name, and value (data only valid for duration
   of handler call). Handler should return nonzero on success, zero on error.

   Returns 0 on success, line number of first error on parse error (doesn't
   stop on first error), -1 on file open error, or -2 on memory allocation
   error (only when INI_USE_STACK is zero).
*/
static int ini_parse(const char* filename,
              int (*handler)(void* user, const char* section,
                             const char* name, const char* value),
              void* user);

/* Same as ini_parse(), but takes a FILE* instead of filename. This doesn't
   close the file when it's finished -- the caller must do that. */
static int ini_parse_file(FILE* file,
                   int (*handler)(void* user, const char* section,
                                  const char* name, const char* value),
                   void* user);

/* Nonzero to allow multi-line value parsing, in the style of Python's
   ConfigParser. If allowed, ini_parse() will call the handler with the same
   name for each subsequent line parsed. */
#ifndef INI_ALLOW_MULTILINE
#define INI_ALLOW_MULTILINE 1
#endif

/* Nonzero to allow a UTF-8 BOM sequence (0xEF 0xBB 0xBF) at the start of
   the file. See http://code.google.com/p/inih/issues/detail?id=21 */
#ifndef INI_ALLOW_BOM
#define INI_ALLOW_BOM 1
#endif

/* Nonzero to use stack, zero to use heap (malloc/free). */
#ifndef INI_USE_STACK
#define INI_USE_STACK 0
#endif

/* Maximum line length for any line in INI file. */
#ifndef INI_MAX_LINE
#define INI_MAX_LINE 4096
#endif

#ifdef __cplusplus
}
#endif

#endif /* __INI_H__ */


#if !INI_USE_STACK
#include <stdlib.h>
#endif

#define MAX_SECTION 64
#define MAX_NAME 64

/* Strip whitespace chars off end of given string, in place. Return s. */
static char* rstrip(char* s)
{
    char* p = s + strlen(s);
    while (p > s && isspace((unsigned char)(*--p)))
        *p = '\0';
    return s;
}

/* Return pointer to first non-whitespace char in given string. */
static char* lskip(const char* s)
{
    while (*s && isspace((unsigned char)(*s)))
        s++;
    return (char*)s;
}

/* Return pointer to first char c or ';' comment in given string, or pointer to
   null at end of string if neither found. ';' must be prefixed by a whitespace
   character to register as a comment. */
static char* find_char_or_comment(const char* s, char c)
{
    int was_whitespace = 0;
    while (*s && *s != c && !(was_whitespace && *s == ';')) {
        was_whitespace = isspace((unsigned char)(*s));
        s++;
    }
    return (char*)s;
}

/* Version of strncpy that ensures dest (size bytes) is null-terminated. */
static char* strncpy0(char* dest, const char* src, size_t size)
{
    strncpy(dest, src, size);
    dest[size - 1] = '\0';
    return dest;
}

/* See documentation in header file. */
static int ini_parse_file(FILE* file,
                   int (*handler)(void*, const char*, const char*,
                                  const char*),
                   void* user)
{
    /* Uses a fair bit of stack (use heap instead if you need to) */
#if INI_USE_STACK
    char line[INI_MAX_LINE];
#else
    char* line;
#endif
    char section[MAX_SECTION] = "";
    char prev_name[MAX_NAME] = "";

    char* start;
    char* end;
    char* name;
    char* value;
    int lineno = 0;
    int error = 0;

#if !INI_USE_STACK
    line = (char*)malloc(INI_MAX_LINE);
    if (!line) {
        return -2;
    }
#endif

    /* Scan through file line by line */
    while (fgets(line, INI_MAX_LINE, file) != NULL) {
        lineno++;

        start = line;
#if INI_ALLOW_BOM
        if (lineno == 1 && (unsigned char)start[0] == 0xEF &&
                           (unsigned char)start[1] == 0xBB &&
                           (unsigned char)start[2] == 0xBF) {
            start += 3;
        }
#endif
        start = lskip(rstrip(start));

        if (*start == ';' || *start == '#') {
            /* Per Python ConfigParser, allow '#' comments at start of line */
        }
#if INI_ALLOW_MULTILINE
        else if (*prev_name && *start && start > line) {
            /* Non-black line with leading whitespace, treat as continuation
               of previous name's value (as per Python ConfigParser). */
            if (!handler(user, section, prev_name, start) && !error)
                error = lineno;
        }
#endif
        else if (*start == '[') {
            /* A "[section]" line */
            end = find_char_or_comment(start + 1, ']');
            if (*end == ']') {
                *end = '\0';
                strncpy0(section, start + 1, sizeof(section));
                *prev_name = '\0';
            }
            else if (!error) {
                /* No ']' found on section line */
                error = lineno;
            }
        }
        else if (*start && *start != ';') {
            /* Not a comment, must be a name[=:]value pair */
            end = find_char_or_comment(start, '=');
            if (*end != '=') {
                end = find_char_or_comment(start, ':');
            }
            if (*end == '=' || *end == ':') {
                *end = '\0';
                name = rstrip(start);
                value = lskip(end + 1);
                end = find_char_or_comment(value, '\0');
                if (*end == ';')
                    *end = '\0';
                rstrip(value);

                /* Valid name[=:]value pair found, call handler */
                strncpy0(prev_name, name, sizeof(prev_name));
                if (!handler(user, section, name, value) && !error)
                    error = lineno;
            }
            else if (!error) {
                /* No '=' or ':' found on name[=:]value line */
                error = lineno;
            }
        }
    }

#if !INI_USE_STACK
    free(line);
#endif

    return error;
}

/* See documentation in header file. */
static int ini_parse(const char* filename,
              int (*handler)(void*, const char*, const char*, const char*),
              void* user)
{
    FILE* file;
    int error;

    file = fopen(filename, "r");
    if (!file)
        return -1;
    error = ini_parse_file(file, handler, user);
    fclose(file);
    return error;
}
