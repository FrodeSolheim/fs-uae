#ifndef FSEMU_OPTION_H
#define FSEMU_OPTION_H

#include "fsemu-common.h"
#include "fsemu-config.h"

#define FSEMU_OPTION_NO_INT 0x12345678

// FIXME: DEPRECATED
#define FSEMU_CONFIG_NONE 0x12345678

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_option_init(void);

// Shortahnd for init + load_from_argv
void fsemu_option_init_from_argv(int argc, char **argv);

void fsemu_option_load_from_argv(int argc, char **argv);

fsemu_error_t fsemu_option_read_config_file(const char *path);

const char **fsemu_option_keys(void);
void fsemu_option_keys_free(const char **keys);

const char *fsemu_option_const_string(const char *name);

// This function is guaranteed to not return a NULL value unless you pass in
// NULL as default_value.
const char *fsemu_option_const_string_default(const char *name, const char *default_value);

// Returns int value of option, or FSEMU_OPTION_NO_INT if option is unset.
// Invalid int will currently be returned as 0, though this might be changed
// to FSEMU_OPTION_NO_INT in the future.
int fsemu_option_int(const char *name);

int fsemu_option_int_default(const char *name, int default_value);

int fsemu_option_int_clamped(const char *name, int min, int max);

int fsemu_option_int_clamped_default(const char *name, int min, int max, int default_value);

int fsemu_option_read_bool_default(const char *name, bool *result, bool default_value);
int fsemu_option_read_int(const char *name, int *result);
// int fsemu_option_read_string(const char *name, int *result, int max_len);
int fsemu_option_read_const_string(const char *name, const char **result);

static inline bool fsemu_option_enabled(const char *name)
{
    bool result;
    fsemu_option_read_bool_default(name, &result, false);
    return result == true;
}

static inline bool fsemu_option_disabled(const char *name)
{
    bool result;
    fsemu_option_read_bool_default(name, &result, true);
    return result == false;
}

static inline int fsemu_option_boolean(const char *name)
{
    return FSEMU_CONFIG_NONE;
}

// ----------------------------------------------------------------------------
// FIXME: REMOVE START
// ----------------------------------------------------------------------------

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

static inline int fsemu_config_boolean(const char *name)
{
    // return FSEMU_CONFIG_NONE;
    return fsemu_option_boolean(name);
}

int fsemu_config_read_bool_default(const char *name, bool *result, bool default_value);

// ----------------------------------------------------------------------------
// FIXME: REMOVE END
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Logging
// ----------------------------------------------------------------------------

extern int fsemu_option_log_level;

#define fsemu_option_log(format, ...)                                                              \
    FS_LOG_INFO(fsemu_option_log_level, "[FSE] [OPT]", format, ##__VA_ARGS__)

#define fsemu_option_log_debug(format, ...)                                                        \
    FS_LOG_DEBUG(fsemu_option_log_level, "[FSE] [OPT]", format, ##__VA_ARGS__)

#define fsemu_option_log_error(format, ...)                                                        \
    FS_LOG_ERROR(fsemu_option_log_level, "[FSE] [OPT]", format, ##__VA_ARGS__)

#define fsemu_option_log_info(format, ...)                                                         \
    FS_LOG_INFO(fsemu_option_log_level, "[FSE] [OPT]", format, ##__VA_ARGS__)

#define fsemu_option_log_trace(format, ...)                                                        \
    FS_LOG_TRACE(fsemu_option_log_level, "[FSE] [OPT]", format, ##__VA_ARGS__)

#define fsemu_option_log_warning(format, ...)                                                      \
    FS_LOG_WARNING(fsemu_option_log_level, "[FSE] [OPT]", format, ##__VA_ARGS__)

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#include "fsemu-options.h"

#endif /* FSEMU_OPTION_H */
