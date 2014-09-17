#ifndef FS_UAE_CONFIG_COMMON_H
#define FS_UAE_CONFIG_COMMON_H

#include "config-model.h"

#include <stdbool.h>

bool fs_uae_values_matches(const char *s1, const char *s2);
int fs_uae_read_memory_option(const char *name);
int fs_uae_read_memory_option_small(const char *name);
int fs_uae_memory_kb_from_string(const char *sa, bool asmall);

extern int g_fs_uae_ntsc_mode;

#endif /* FS_UAE_CONFIG_COMMON_H */
