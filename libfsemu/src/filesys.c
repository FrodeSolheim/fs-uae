#include <fs/base.h>
#include <fs/filesys.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <glib.h>
#include <glib/gstdio.h>

// some code adapted from glib

int fs_stat(const char *path, struct fs_stat *buf) {
    struct stat st;
    int result = g_stat(path, &st);
    if (result == 0) {
        buf->atime = st.st_atime;
        buf->mtime = st.st_mtime;
        buf->ctime = st.st_ctime;
        buf->size = st.st_size;
        buf->mode = st.st_mode;
    }
    return result;
}

int fs_fstat(int fd, struct fs_stat *buf) {
    struct stat st;
    int result = fstat(fd, &st);
    if (result == 0) {
        buf->atime = st.st_atime;
        buf->mtime = st.st_mtime;
        buf->ctime = st.st_ctime;
        buf->size = st.st_size;
        buf->mode = st.st_mode;
    }
    return result;
}

struct fs_dir {
    GDir *gdir;
};

fs_dir* fs_dir_open(const char *path, int flags) {
    GDir* gdir = g_dir_open(path, flags, NULL);
    if (!gdir) {
        return NULL;
    }
    fs_dir *dir = fs_new(fs_dir, 1);
    dir->gdir = gdir;
    return dir;
}

const char *fs_dir_read_name(fs_dir *dir) {
    return g_dir_read_name(dir->gdir);
}

void fs_dir_close(fs_dir *dir) {
    g_dir_close(dir->gdir);
    free(dir);
}

FILE *fs_fopen(const char *path, const char *mode) {
    return g_fopen(path, mode);
}

int fs_open(const char *path, int flags, int mode) {
    return g_open(path, flags, mode);
}

int fs_rmdir(const char *path) {
    return g_rmdir(path);
}

int fs_unlink(const char *path) {
    return g_unlink(path);
}

int fs_rename(const char *old_path, const char *new_path) {
    return g_rename(old_path, new_path);
}

int fs_mkdir(const char *path, int mode) {
    return g_mkdir(path, mode);
}

int fs_mkdir_with_parents(const char *path, int mode) {
    return g_mkdir_with_parents(path, mode);
}

char *fs_path_get_dirname(const char *path) {
    return g_path_get_dirname(path);
}

char *fs_path_get_basename(const char *path) {
    return g_path_get_basename(path);
}

const char *fs_get_home_dir() {
    return g_get_home_dir();
}

const char *fs_get_documents_dir() {
    return g_get_user_special_dir(G_USER_DIRECTORY_DOCUMENTS);
}

char *fs_get_current_dir() {
    return g_get_current_dir();
}

int fs_path_exists(const char *path) {
    return g_file_test(path, G_FILE_TEST_EXISTS);
}

int fs_path_is_file(const char *path) {
    return g_file_test(path, G_FILE_TEST_IS_REGULAR);
}

int fs_path_is_dir(const char *path) {
    return g_file_test(path, G_FILE_TEST_IS_DIR);
}

int fs_path_is_absolute(const char *path) {
    return g_path_is_absolute(path);
}

static gchar *g_build_path_va(const gchar *separator,
        const gchar *first_element, va_list *args, gchar **str_array) {
    GString *result;
    gint separator_len = strlen(separator);
    int is_first = TRUE;
    int have_leading = FALSE;
    const gchar *single_element = NULL;
    const gchar *next_element;
    const gchar *last_trailing = NULL;
    gint i = 0;

    result = g_string_new(NULL);

    if (str_array) next_element = str_array[i++];
    else
        next_element = first_element;

    while (TRUE) {
        const gchar *element;
        const gchar *start;
        const gchar *end;

        if (next_element) {
            element = next_element;
            if (str_array) next_element = str_array[i++];
            else
                next_element = va_arg (*args, gchar *);
        }
        else
            break;

        /* Ignore empty elements */
        if (!*element) continue;

        start = element;

        if (separator_len) {
            while (strncmp(start, separator, separator_len) == 0)
                start += separator_len;
        }

        end = start + strlen(start);

        if (separator_len) {
            while (end >= start + separator_len
                    && strncmp(end - separator_len, separator, separator_len)
                            == 0)
                end -= separator_len;

            last_trailing = end;
            while (last_trailing >= element + separator_len
                    && strncmp(last_trailing - separator_len, separator,
                            separator_len) == 0)
                last_trailing -= separator_len;

            if (!have_leading) {
                /* If the leading and trailing separator strings are in the
                 * same element and overlap, the result is exactly that element
                 */
                if (last_trailing <= start) single_element = element;

                g_string_append_len(result, element, start - element);
                have_leading = TRUE;
            }
            else
                single_element = NULL;
        }

        if (end == start) continue;

        if (!is_first) g_string_append(result, separator);

        g_string_append_len(result, start, end - start);
        is_first = FALSE;
    }

    if (single_element) {
        g_string_free(result, TRUE);
        return g_strdup(single_element);
    }
    else {
        if (last_trailing) g_string_append(result, last_trailing);

        return g_string_free(result, FALSE);
    }
}

static gchar *g_build_pathname_va(const gchar *first_element, va_list *args,
        gchar **str_array) {
    /* Code copied from g_build_pathv(), and modified to use two
     * alternative single-character separators.
     */
    GString *result;
    int is_first = TRUE;
    int have_leading = FALSE;
    const gchar *single_element = NULL;
    const gchar *next_element;
    const gchar *last_trailing = NULL;
    gchar current_separator = '\\';
    gint i = 0;

    result = g_string_new(NULL);

    if (str_array) next_element = str_array[i++];
    else
        next_element = first_element;

    while (TRUE) {
        const gchar *element;
        const gchar *start;
        const gchar *end;

        if (next_element) {
            element = next_element;
            if (str_array) next_element = str_array[i++];
            else
                next_element = va_arg (*args, gchar *);
        }
        else
            break;

        /* Ignore empty elements */
        if (!*element) continue;

        start = element;

        if (TRUE) {
            while (start && (*start == '\\' || *start == '/')) {
                current_separator = *start;
                start++;
            }
        }

        end = start + strlen(start);

        if (TRUE) {
            while (end >= start + 1 && (end[-1] == '\\' || end[-1] == '/')) {
                current_separator = end[-1];
                end--;
            }

            last_trailing = end;
            while (last_trailing >= element + 1
                    && (last_trailing[-1] == '\\' || last_trailing[-1] == '/'))
                last_trailing--;

            if (!have_leading) {
                /* If the leading and trailing separator strings are in the
                 * same element and overlap, the result is exactly that element
                 */
                if (last_trailing <= start) single_element = element;

                g_string_append_len(result, element, start - element);
                have_leading = TRUE;
            }
            else
                single_element = NULL;
        }

        if (end == start) continue;

        if (!is_first) g_string_append_len(result, &current_separator, 1);

        g_string_append_len(result, start, end - start);
        is_first = FALSE;
    }

    if (single_element) {
        g_string_free(result, TRUE);
        return g_strdup(single_element);
    }
    else {
        if (last_trailing) g_string_append(result, last_trailing);

        return g_string_free(result, FALSE);
    }
}

char *fs_path_join(const char *first_element, ...) {
    gchar *str;
    va_list args;
    va_start(args, first_element);
#ifndef WINDOWS
    str = g_build_path_va("/", first_element, &args, NULL);
#else
    str = g_build_pathname_va(first_element, &args, NULL);
#endif
    va_end(args);
    return str;
}
