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
} log;

static void initialize()
{
    if (log.initialized) {
        return;
    }
    log.mutex = fs_mutex_create();
    log.initialized = 1;

    char *dir = g_build_filename(fse_user_data_dir(), "fs-uae", NULL);
    if (!fs_path_exists(dir)) {
        if (g_mkdir_with_parents(dir, 0755) == -1) {
            // could not create directory
            printf("WARNING: could not create directory %s\n", dir);
            g_free(dir);
            return;
        }
    }
    log.initial_path = g_build_filename(dir, "fs-uae.log", NULL);
    log.file = g_fopen(log.initial_path, "w");
    if (log.file) {
        // printf("[LOG] %s\n", log.initial_path);
    }
    g_free(dir);
}

void fs_log_enable_stdout()
{
    log.use_stdout = 1;
}

void fs_config_set_log_file(const char *path)
{
    fs_log("switch to log file %s\n", path);
    fs_mutex_lock(log.mutex);
    if (log.file) {
        fclose(log.file);
    }
    log.file = g_fopen(path, "w");
    if (log.file) {
        printf("LOG: %s\n", path);
        if (log.initial_path) {
            FILE *f = g_fopen(log.initial_path, "r");
            if (f) {
                char *buffer = (char *) g_malloc(1024);
                int read = fread(buffer, 1, 1024, f);
                while (read > 0) {
                    fwrite(buffer, 1, read, log.file);
                    read = fread(buffer, 1, 1024, f);
                }
                g_free(buffer);
                fclose(f);
            }
        }
    }

    /* All options should have been read, so we can now check log options */

    if (fs_config_get_boolean("log_flush") == 1) {
        log.flush = 1;
    } else if (fs_config_get_boolean("flush_log") == 1) {
        log.flush = 1;
    }

    fs_mutex_unlock(log.mutex);

    if (log.flush) {
        fs_log_string("flush_log: will flush log after each log line\n");
    }
}

void fs_log_string(const char *str)
{
    if (!log.initialized) {
        initialize();
    }
    fs_mutex_lock(log.mutex);
    if (log.use_stdout) {
        printf("%s", str);
        fflush(stdout);
    }
    if (log.file) {
        fprintf(log.file, "%s", str);
    }
    if (log.flush) {
        fflush(log.file);
    }
    fs_mutex_unlock(log.mutex);
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
