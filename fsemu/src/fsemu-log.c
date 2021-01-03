#include "fsemu-internal.h"
#include "fsemu-log.h"

void fsemu_log_null(const char *format, ...)
{
}

void fsemu_log_string(const char *message)
{
    // FIXME: Log to file, optionally to stdout
    printf("%s", message);
}
