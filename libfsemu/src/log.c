#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <fs/base.h>
#include <fs/log.h>
#include <fs/config.h>
#include <fs/filesys.h>
#include <fs/string.h>
#include <fs/thread.h>

#ifdef USE_GLIB
#include <glib.h>
#include <glib/gstdio.h>
#endif

int g_fs_log_stdout = 0;

static FILE *g_log_file = NULL;
static char *g_initial_log_file = NULL;
static int g_initialized = 0;
static fs_mutex *g_mutex = NULL;
static int g_flush_log = 0;

static void initialize() {
    if (g_initialized) {
        return;
    }
    g_mutex = fs_mutex_create();
    g_initialized = 1;

    char *dir = fs_path_join(fs_get_user_data_dir(), "fs-uae", NULL);
    if (!fs_path_exists(dir)) {
        if (fs_mkdir_with_parents(dir, 0755) == -1) {
            // could not create directory
            printf("WARNING: could not create directory %s\n", dir);
            free(dir);
            return;
        }
    }
    g_initial_log_file = fs_path_join(dir, "fs-uae.log", NULL);
    g_log_file = fs_fopen(g_initial_log_file, "w");
    if (g_log_file) {
        printf("logging to %s\n", g_initial_log_file);
    }
    free(dir);
}

void fs_log_enable_stdout() {
    g_fs_log_stdout = 1;
}

void fs_config_set_log_file(const char *path) {
    fs_log("switch to log file %s\n", path);
    fs_mutex_lock(g_mutex);
    if (g_log_file) {
        fclose(g_log_file);
    }
    g_log_file = fs_fopen(path, "w");
    if (g_log_file) {
        printf("logging to %s\n", path);
        if (g_initial_log_file) {
            FILE *f = fs_fopen(g_initial_log_file, "r");
            if (f) {
                char *buffer = (char *) malloc(1024);
                int read = fread(buffer, 1, 1024, f);
                while (read > 0) {
                    fwrite(buffer, 1, read, g_log_file);
                    read = fread(buffer, 1, 1024, f);
                }
                free(buffer);
            }
            fclose(f);
        }
    }

    // All options should have been read, so we can no check log options

    if (fs_config_get_boolean("flush_log") == 1) {
        g_flush_log = 1;
    }

    fs_mutex_unlock(g_mutex);

    if (g_flush_log) {
        fs_log_string("flush_log: will flush log after each log line\n");
    }
}

void fs_log_string(const char *str) {
    if (!g_initialized) {
        initialize();
    }
    fs_mutex_lock(g_mutex);
    if (g_fs_log_stdout) {
        printf("%s", str);
    }
    if (g_log_file) {
        fprintf(g_log_file, "%s", str);
    }
    if (g_flush_log) {
        fflush(g_log_file);
    }
    fs_mutex_unlock(g_mutex);
}

void fs_log(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    char *buffer = g_strdup_vprintf(format, ap);
    va_end(ap);
    fs_log_string(buffer);
    free(buffer);
}
