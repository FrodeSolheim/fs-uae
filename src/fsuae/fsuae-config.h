#ifndef FSUAE_CONFIG_H_
#define FSUAE_CONFIG_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define FSUAE_DEBUG 1

#ifdef FSUAE_DEBUG
#ifdef FSUAE_INTERNAL
#pragma GCC diagnostic error "-Wdiscarded-qualifiers"
#pragma GCC diagnostic error "-Wincompatible-pointer-types"
#pragma GCC diagnostic error "-Wall"
#endif
#endif

#include <stdbool.h>

#include "fsemu-log.h"
#include "fsuae-model.h"

extern int fsuae_log_level;

#define fsuae_log(format, ...)                                           \
    if (fsemu_likely(fsuae_log_level >= FSEMU_LOG_LEVEL_INFO)) {         \
        fsemu_log_with_level(                                            \
            FSEMU_LOG_LEVEL_INFO, "[FAE] [   ] " format, ##__VA_ARGS__); \
    }

#define fsuae_log_debug(format, ...)                                      \
    if (fsemu_unlikely(fsuae_log_level >= FSEMU_LOG_LEVEL_DEBUG)) {       \
        fsemu_log_with_level(                                             \
            FSEMU_LOG_LEVEL_DEBUG, "[FAE] [   ] " format, ##__VA_ARGS__); \
    }

#define fsuae_log_warning(format, ...)                                      \
    if (fsemu_likely(fsuae_log_level >= FSEMU_LOG_LEVEL_WARNING)) {         \
        fsemu_log_with_level(                                               \
            FSEMU_LOG_LEVEL_WARNING, "[FAE] [   ] " format, ##__VA_ARGS__); \
    }

bool fs_uae_values_matches(const char *s1, const char *s2);
int fs_uae_read_memory_option(const char *name);
int fs_uae_read_memory_option_small(const char *name);
int fs_uae_memory_kb_from_string(const char *sa, bool asmall);

extern int g_fs_uae_ntsc_mode;

#endif  // FSUAE_CONFIG_H_
