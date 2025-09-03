#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fsapp-instance.h"

#ifdef _WIN32
#include <windows.h>
static HANDLE g_instance_mutex = NULL;
static char* g_mutex_name = NULL;
#else
#include <errno.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
static int g_lock_fd = -1;
static char* g_lock_path = NULL;
#endif

static bool g_lock_acquired = false;

#ifndef _WIN32
static char* get_lock_file_path(const char* app_name) {
    const char* temp_dir = getenv("TMPDIR");
    if (temp_dir == NULL) {
        temp_dir = getenv("TMP");
    }
    if (temp_dir == NULL) {
        temp_dir = "/tmp";
    }

    // Include user ID in lock file name to avoid conflicts between users
    size_t path_len = strlen(temp_dir) + strlen(app_name) + 64;  // Extra space for filename and uid
    char* lock_path = malloc(path_len);
    if (lock_path == NULL) {
        return NULL;
    }

    snprintf(lock_path, path_len, "%s/%s-instance.lock.%d", temp_dir, app_name, (int)getuid());
    return lock_path;
}
#endif

fsapp_instance_result_t fsapp_instance_acquire_lock(const char* app_name) {
    if (app_name == NULL || strlen(app_name) == 0) {
        return FSAPP_INSTANCE_ERROR;
    }

    if (g_lock_acquired) {
        return FSAPP_INSTANCE_OK;  // Already acquired
    }

#ifdef _WIN32
    // Create mutex name: "Global\\AppName-InstanceMutex"
    size_t mutex_name_len = strlen(app_name) + 32;  // Extra space for prefix/suffix
    g_mutex_name = malloc(mutex_name_len);
    if (g_mutex_name == NULL) {
        return FSAPP_INSTANCE_ERROR;
    }
    snprintf(g_mutex_name, mutex_name_len, "Global\\%s-InstanceMutex", app_name);

    g_instance_mutex = CreateMutexA(NULL, TRUE, g_mutex_name);

    if (g_instance_mutex == NULL) {
        free(g_mutex_name);
        g_mutex_name = NULL;
        return FSAPP_INSTANCE_ERROR;
    }

    DWORD result = GetLastError();
    if (result == ERROR_ALREADY_EXISTS) {
        CloseHandle(g_instance_mutex);
        g_instance_mutex = NULL;
        free(g_mutex_name);
        g_mutex_name = NULL;
        return FSAPP_INSTANCE_ALREADY_RUNNING;
    }

    g_lock_acquired = true;
    return FSAPP_INSTANCE_OK;
#else
    g_lock_path = get_lock_file_path(app_name);
    if (g_lock_path == NULL) {
        return FSAPP_INSTANCE_ERROR;
    }

    // Create/open the lock file
    g_lock_fd = open(g_lock_path, O_CREAT | O_WRONLY, 0600);
    if (g_lock_fd == -1) {
        free(g_lock_path);
        g_lock_path = NULL;
        return FSAPP_INSTANCE_ERROR;
    }

    // Try to acquire exclusive lock (non-blocking)
    if (flock(g_lock_fd, LOCK_EX | LOCK_NB) == -1) {
        if (errno == EWOULDBLOCK) {
            close(g_lock_fd);
            g_lock_fd = -1;
            free(g_lock_path);
            g_lock_path = NULL;
            return FSAPP_INSTANCE_ALREADY_RUNNING;
        } else {
            close(g_lock_fd);
            g_lock_fd = -1;
            free(g_lock_path);
            g_lock_path = NULL;
            return FSAPP_INSTANCE_ERROR;
        }
    }

    // Write our PID to the lock file (optional, for debugging)
    char pid_str[32];
    snprintf(pid_str, sizeof(pid_str), "%d\n", (int)getpid());
    if (write(g_lock_fd, pid_str, strlen(pid_str)) == -1) {
        // Non-fatal, continue anyway
    }

    g_lock_acquired = true;
    return FSAPP_INSTANCE_OK;
#endif
}

void fsapp_instance_release_lock(void) {
    if (!g_lock_acquired) {
        return;  // Nothing to release
    }

#ifdef _WIN32
    if (g_instance_mutex != NULL) {
        ReleaseMutex(g_instance_mutex);
        CloseHandle(g_instance_mutex);
        g_instance_mutex = NULL;
    }

    if (g_mutex_name != NULL) {
        free(g_mutex_name);
        g_mutex_name = NULL;
    }
#else
    if (g_lock_fd != -1) {
        // Lock is automatically released when file descriptor is closed
        close(g_lock_fd);
        g_lock_fd = -1;
    }

    if (g_lock_path != NULL) {
        // Clean up the lock file
        unlink(g_lock_path);
        free(g_lock_path);
        g_lock_path = NULL;
    }
#endif

    g_lock_acquired = false;
}
