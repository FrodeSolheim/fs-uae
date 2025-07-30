#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/inifile.h>
#include <fs/filesys.h>
#include <fs/glib.h>

struct fs_ini_file {
    GHashTable *groups;
};

#define INI_MAX_LINE 1024
#include "../libinih/ini.h"

static void free_group(void *data)
{
    g_hash_table_destroy((GHashTable*) data);
}

static fs_ini_file* fs_ini_file_create()
{
    fs_ini_file *ini_file = g_malloc(sizeof(fs_ini_file));
    ini_file->groups = g_hash_table_new_full(g_str_hash, g_str_equal,
            g_free, free_group);
    return ini_file;
}

static int handler(void *user_data, const char *section, const char *name,
                   const char *value)
{
    fs_ini_file *ini_file = (fs_ini_file*) user_data;
    GHashTable *group = g_hash_table_lookup(ini_file->groups, section);
    if (group == NULL) {
        group = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
        g_hash_table_insert(ini_file->groups, g_strdup(section), group);
    }
    g_hash_table_insert(group, g_strdup(name), g_strdup(value));
    return 1;
}

fs_ini_file* fs_ini_file_open(const char *path)
{
    fs_ini_file *ini_file = fs_ini_file_create();
    int result = ini_parse(path, handler, ini_file);
    if (result == -1) {
        /* Could not open file */
        fs_ini_file_destroy(ini_file);
        return NULL;
    }
    return ini_file;
}

struct data_stream
{
    const char *data;
    int size;
    const char *pos;
};

static char *data_stream_fgets(char *str, int num, void *stream)
{
    struct data_stream *s = (struct data_stream *) stream;
    char *p = str;
    int read = 0;
    while (read < num - 1 && s->pos < (s->data + s->size)) {
        *p++ = *(s->pos)++;
        read++;
        if (*(p - 1) == '\n') {
            break;
        }
    }
    if (read == 0) {
        return NULL;
    }
    *p = '\0';
    return str;
}

fs_ini_file* fs_ini_file_open_data(const char *data, int size)
{
    fs_ini_file *ini_file = fs_ini_file_create();
    struct data_stream stream;
    stream.data = data;
    stream.size = size;
    stream.pos = data;
    int result = ini_parse_stream(
        (ini_reader) data_stream_fgets, &stream, handler, ini_file);
    if (result == -1) {
        /* Could not open file */
        fs_ini_file_destroy(ini_file);
        return NULL;
    }
    return ini_file;
}

void fs_ini_file_destroy(fs_ini_file *ini_file)
{
    g_hash_table_destroy(ini_file->groups);
    g_free(ini_file);
}

static gint compare_strings(gconstpointer a, gconstpointer b)
{
    return g_strcmp0((const char*) a, (const char*) b);
}

static char **retrieve_keys(GHashTable *hash_table, int *length)
{
    int size = g_hash_table_size(hash_table);
    char **result = g_malloc(sizeof(char*) * (size + 1));
    if (length != NULL) {
        *length = size;
    }
    result[size] = NULL;
    GList *key_list = g_hash_table_get_keys(hash_table);
    if (key_list) {
        key_list = g_list_sort(key_list, compare_strings);
    }
    GList *item = key_list;
    int k = 0;
    while (item) {
        result[k++] = g_strdup(item->data);
        item = item->next;
    }
    g_list_free(key_list);
    return result;
}

char **fs_ini_file_get_groups(fs_ini_file *ini_file, int *length)
{
    return retrieve_keys(ini_file->groups, length);
}

char **fs_ini_file_get_keys(
        fs_ini_file *ini_file, const char *group_name, int *length)
{
    GHashTable *group = g_hash_table_lookup(ini_file->groups, group_name);
    if (group == NULL) {
        return NULL;
    }
    return retrieve_keys(group, length);
}

int fs_ini_file_has_group(fs_ini_file *ini_file, const char *group_name)
{
    return g_hash_table_lookup(ini_file->groups, group_name) != NULL;
}

char *fs_ini_file_get_value(
        fs_ini_file *ini_file, const char *group_name, const char *key)
{
    GHashTable *group = g_hash_table_lookup(ini_file->groups, group_name);
    if (group == NULL) {
        return NULL;
    }
    char *value = g_hash_table_lookup(group, key);
    if (value == NULL) {
        return NULL;
    }
    return g_strdup(value);
}

char *fs_ini_file_get_string(
        fs_ini_file *ini_file, const char *group_name, const char *key)
{
    char *value = fs_ini_file_get_value(ini_file, group_name, key);
    // FIXME: fix escape sequences
    return value;
}
