#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fs/base.h>
#include <fs/conf.h>
#include <fs/glib.h>
#include <fs/log.h>
#include <fs/thread.h>
#include <fs/util.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static struct {
    int use_stdout;
    FILE *file;
    char *initial_path;
    int initialized;
    fs_mutex *mutex;
    int flush;
} log_data;

static void initialize_log()
{
    if (log_data.initialized) {
        return;
    }
    log_data.mutex = fs_mutex_create();
    log_data.initialized = 1;

#ifdef WINDOWS
    char *dir = g_build_filename(fse_user_data_dir(), "FS-UAE", NULL);
#else
    char *dir = g_build_filename(fse_user_data_dir(), "fs-uae", NULL);
#endif
    if (!fs_path_exists(dir)) {
        if (g_mkdir_with_parents(dir, 0755) == -1) {
            // could not create directory
            printf("WARNING: could not create directory %s\n", dir);
            g_free(dir);
            return;
        }
    }
    log_data.initial_path = g_build_filename(dir, "fs-uae.log", NULL);
    log_data.file = g_fopen(log_data.initial_path, "w");
    if (log_data.file) {
        // printf("[LOG] %s\n", log_data.initial_path);
    }
    g_free(dir);
}

void fs_log_enable_stdout()
{
    log_data.use_stdout = 1;
}

void fs_config_set_log_file(const char *path)
{
    fs_log("Switching to log file %s\n", path);
    fs_mutex_lock(log_data.mutex);
    if (log_data.file) {
        fclose(log_data.file);
    }
    log_data.file = g_fopen(path, "w");
    if (log_data.file) {
        printf("LOG: %s\n", path);
        if (log_data.initial_path) {
            FILE *f = g_fopen(log_data.initial_path, "r");
            if (f) {
                char *buffer = (char *) g_malloc(1024);
                int read = fread(buffer, 1, 1024, f);
                while (read > 0) {
                    fwrite(buffer, 1, read, log_data.file);
                    read = fread(buffer, 1, 1024, f);
                }
                g_free(buffer);
                fclose(f);
            }
        }
    }

    /* All options should have been read, so we can now check log options */

    if (fs_config_get_boolean("log_flush") == 1) {
        log_data.flush = 1;
    } else if (fs_config_get_boolean("flush_log") == 1) {
        log_data.flush = 1;
    }

    fs_mutex_unlock(log_data.mutex);

    if (log_data.flush) {
        fs_log_string("flush_log: will flush log after each log line\n");
    }
}

void fs_log_string(const char *str)
{
    if (!log_data.initialized) {
        initialize_log();
    }
    fs_mutex_lock(log_data.mutex);
    if (log_data.use_stdout) {
        printf("%s", str);
        fflush(stdout);
    }
    if (log_data.file) {
        fprintf(log_data.file, "%s", str);
    }
    if (log_data.flush) {
        fflush(log_data.file);
    }
    fs_mutex_unlock(log_data.mutex);
}

void fs_log(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    char *buffer = g_strdup_vprintf(format, ap);
    va_end(ap);
    fs_log_string(buffer);
    g_free(buffer);
}
