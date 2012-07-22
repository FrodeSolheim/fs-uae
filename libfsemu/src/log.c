#include <fs/log.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <stdio.h>
#include <stdarg.h>

//#define MODE_STDOUT 0
//#define MODE_FILE 1
//static int g_mode = MODE_STDOUT;
static FILE *g_log_file = NULL;
static gchar *g_initial_log_file = NULL;
static int g_initialized = 0;
int g_fs_log_stdout = 0;
static GMutex *g_mutex = NULL;


static void initialize() {
    if (g_initialized) {
        return;
    }
    g_mutex = g_mutex_new();
    //g_mutex_lock(g_mutex);
    g_initialized = 1;
    /*
    if (g_fs_log_stdout) {

    }
    else {

    }
    */
    gchar *dir = g_build_filename(g_get_user_data_dir(), "fs-uae", NULL);
    if (!g_file_test(dir, G_FILE_TEST_EXISTS)) {
        if (g_mkdir_with_parents(dir, 0755) == -1) {
            // could not create directory
            printf("WARNING: could not create directory %s\n", dir);
            g_free(dir);
            return;
        }
    }
    g_initial_log_file = g_build_filename(dir, "fs-uae.log", NULL);
    g_log_file = fopen(g_initial_log_file, "w");
    if (g_log_file) {
        printf("logging to %s\n", g_initial_log_file);
        //g_mode = MODE_FILE;
    }
    g_free(dir);
    //}
}

void fs_log_enable_stdout() {
    g_fs_log_stdout = 1;
}

void fs_config_set_log_file(const char *path) {
    fs_log("switch to log file %s\n", path);
    g_mutex_lock(g_mutex);
    if (g_log_file) {
        fclose(g_log_file);
    }
    g_log_file = fopen(path, "w");
    if (g_log_file) {
        printf("logging to %s\n", path);
        if (g_initial_log_file) {
            FILE *f = fopen(g_initial_log_file, "r");
            if (f) {
                char *buffer = (char *) g_malloc(1024);
                int read = fread(buffer, 1, 1024, f);
                while (read > 0) {
                    fwrite(buffer, 1, read, g_log_file);
                    read = fread(buffer, 1, 1024, f);
                }
                g_free(buffer);
            }
            fclose(f);
        }
    }
    g_mutex_unlock(g_mutex);
}

void fs_log_string(const char *str) {
    if (!g_initialized) {
        initialize();
    }
    g_mutex_lock(g_mutex);
    if (g_fs_log_stdout) {
        printf("L: %s", str);
    }
    if (g_log_file) {
        fprintf(g_log_file, "%s", str);
        //fflush(g_log_file);
    }
    g_mutex_unlock(g_mutex);
}

void fs_log(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    gchar *buffer = g_strdup_vprintf(format, ap);
    va_end(ap);
    fs_log_string(buffer);
    g_free(buffer);
}
