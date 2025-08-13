#ifndef FSEMU_INIFILE_H_
#define FSEMU_INIFILE_H_

#include "fsemu-config.h"

#ifdef __cplusplus
extern "C" {
#endif

struct fsemu_inifile;
typedef struct fsemu_inifile fsemu_inifile_t;

// FIXME: Deprecated name
typedef struct fsemu_inifile fsemu_inifile;

fsemu_inifile* fsemu_inifile_open(const char* path);
fsemu_inifile* fsemu_inifile_open_data(const char* data, int size);
void fsemu_inifile_destroy(fsemu_inifile* ini_file);

char** fsemu_inifile_get_groups(fsemu_inifile* ini_file, int* length);

char** fsemu_inifile_get_keys(fsemu_inifile* ini_file, const char* group_name, int* length);

int fsemu_inifile_has_group(fsemu_inifile* ini_file, const char* group_name);

char* fsemu_inifile_get_value(fsemu_inifile* ini_file, const char* group_name, const char* key);
char* fsemu_inifile_get_string(fsemu_inifile* ini_file, const char* group_name, const char* key);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_INIFILE_H_
