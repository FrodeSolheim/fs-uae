/*
 * File status and times (stat, fstat, setting a file's time).
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSAPP_FILE_H
#define FSAPP_FILE_H

#include "fsapp-config.h"
#include "fsapp-os.h"

// #ifdef HAVE_CONFIG_H
// #include "config.h"
// #endif

// #include <fs/defines.h>
#include <stdint.h>
#include <stdio.h>
#ifdef _WIN32
#include <winsock2.h>
// struct timeval is defined here...
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <time.h>

// #include <fs/util.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct fsapp_file_stat_t {
    int mode;
    int64_t size;
    time_t atime;
    time_t mtime;
    time_t ctime;
    int atime_nsec;
    int mtime_nsec;
    int ctime_nsec;
    int64_t blocks;
} fsapp_file_stat_t;

int fsapp_file_stat(const char *path, fsapp_file_stat_t *buf);
int fsapp_file_fstat(int fd, fsapp_file_stat_t *buf);

// Sets the modification (and on other systems than Windows, access) time of a file.
int fsapp_file_set_time(const char *path, struct timeval *tv);

#ifdef __cplusplus
}
#endif

#endif /* FSAPP_FILE_H */
