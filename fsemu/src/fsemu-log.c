#define FSEMU_INTERNAL 1
#include "fsemu-log.h"

#include <stdarg.h>

#include "fsemu-glib.h"
#include "fsemu-option.h"

// For now...
#include <fs/log.h>

static struct {
    bool initialized;
    bool stdout;
} fsemu_log;

// void fsemu_log_null(const char *format, ...)
// {
// }

// void fsemu_log_string(const char *message)
// {
//     // FIXME: Log to file, optionally to stdout
//     printf("%s", message);
// }

void fsemu_log_with_level(int level, const char *format, ...)
{
    va_list ap;
    if (level <= FSEMU_LOG_LEVEL_WARNING) {
        va_start(ap, format);
        vfprintf(stderr, format, ap);
        va_end(ap);
    } else if (fsemu_log.stdout) {
        va_start(ap, format);
        vprintf(format, ap);
        va_end(ap);
    }

#ifdef FSUAE
    va_start(ap, format);
    char *buffer = g_strdup_vprintf(format, ap);
    va_end(ap);
    // For now...
    fs_log_string(buffer);
    g_free(buffer);
#endif
}

void fsemu_log_setup(void)
{
    if (fsemu_log.initialized) {
        return;
    }
    fsemu_log.initialized = true;
    if (fsemu_option_enabled(FSEMU_OPTION_STDOUT_LOGGING)) {
        fsemu_log.stdout = true;
    }
    // Deprecated
    if (fsemu_option_enabled(FSEMU_OPTION_STDOUT)) {
        fsemu_log.stdout = true;
    }
}
