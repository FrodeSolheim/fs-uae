#define FSEMU_INTERNAL
#include "fsemu-log.h"

#include <stdarg.h>

#include "fsemu-glib.h"
#include "fsemu-hud.h"
#include "fsemu-option.h"
#include "fsemu-time.h"

static struct {
    bool initialized;
    bool stdout_logging;
    int64_t last_warning_at;
    int64_t last_error_at;
} fsemu_log;

int64_t fsemu_log_last_warning_at(void) {
    return fsemu_log.last_warning_at;
}

int64_t fsemu_log_last_error_at(void) {
    return fsemu_log.last_error_at;
}

// void fsemu_log_null(const char *format, ...)
// {
// }

// void fsemu_log_string(const char *message)
// {
//     // FIXME: Log to file, optionally to stdout
//     printf("%s", message);
// }

void fsemu_log_with_level_and_flags(int level, int flags, const char* format, ...) {
    va_list ap;
    if (level <= FSEMU_LOG_LEVEL_WARNING) {
        va_start(ap, format);
        vfprintf(stderr, format, ap);
        va_end(ap);
    } else if (fsemu_log.stdout_logging) {
        // va_start(ap, format);
        // vprintf(format, ap);
        // va_end(ap);
    }

#ifdef FSUAE
    va_start(ap, format);
    char* buffer = g_strdup_vprintf(format, ap);
    va_end(ap);
    // For now...
    // fs_log_string(buffer);
    SDL_Log("%s", buffer);
    g_free(buffer);
#endif
    if (level == FSEMU_LOG_LEVEL_WARNING) {
        if (!(flags & FSEMU_LOG_FLAG_NO_LAST_UPDATE)) {
            fsemu_log.last_warning_at = fsemu_time_us();
        }
    } else if (level == FSEMU_LOG_LEVEL_ERROR) {
        if (!(flags & FSEMU_LOG_FLAG_NO_LAST_UPDATE)) {
            fsemu_log.last_error_at = fsemu_time_us();
        }
    }
}

void fsemu_log_setup(void) {
    if (fsemu_log.initialized) {
        return;
    }
    fsemu_log.initialized = true;
    if (fsemu_option_enabled(FSEMU_OPTION_STDOUT_LOGGING)) {
        fsemu_log.stdout_logging = true;
    }
    // Deprecated
    if (fsemu_option_enabled(FSEMU_OPTION_STDOUT)) {
        fsemu_log.stdout_logging = true;
    }

    // FIXME: HACK
    fsemu_log.stdout_logging = 1;
}
