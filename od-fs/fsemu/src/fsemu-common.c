#define FSEMU_INTERNAL
#include "fsemu-common.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fsemu-hud.h"
#include "fsemu-log.h"
#include "fsemu-util.h"

// static struct {
//     char *emulator_name;
//     // const char *fork_info;
// } fsemu_common;

/*
const char *fsemu_emulator_name(void)
{
    if (!fsemu_common.emulator_name) {
        return "FSEMU";
    }
    return fsemu_common.emulator_name;
}

void fsemu_set_emulator_name(const char *emulator_name)
{
    if (fsemu_common.emulator_name) {
        free(fsemu_common.emulator_name);
    }
    fsemu_common.emulator_name = strdup(emulator_name);
}
*/

void fsemu_error_2(const char *msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    vprintf(msg, ap);
    va_end(ap);
}

// ----------------------------------------------------------------------------

void fsemu_warning(const char *message)
{
    fsemu_warning_2(message, NULL);
}

void fsemu_warning_2(const char *message, const char *sub)
{
    fsemu_assert(message != NULL);
    // Log with FSEMU_LOG_FLAG_NO_LAST_UPDATE; Since we show a HUD
    // warning here, we do not want the generic warning HUD notice in addition.
    if (sub != NULL) {
        fsemu_log_with_level_and_flags(FSEMU_LOG_LEVEL_WARNING,
                                       FSEMU_LOG_FLAG_NO_LAST_UPDATE,
                                       "WARNING: %s - %s\n",
                                       message,
                                       sub);
    } else {
        fsemu_log_with_level_and_flags(FSEMU_LOG_LEVEL_WARNING,
                                       FSEMU_LOG_FLAG_NO_LAST_UPDATE,
                                       "WARNING: %s\n",
                                       message);
    }

    int notification_id = 0;
    // FIXME: Icon name
    fsemu_hud_show_notification(
        notification_id, message, sub, "warning", 10 * 1000 * 1000);
}
