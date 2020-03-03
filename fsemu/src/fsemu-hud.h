#ifndef FSEMU_HUD_H_
#define FSEMU_HUD_H_

#include <stdint.h>

#include "fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FSEMU_HUD_NOTIFICATION_WARNING_ICON "warning"
#define FSEMU_HUD_NOTIFICATION_DEFAULT_DURATION (5 * 1000 * 1000)

typedef uint64_t fsemu_hud_id_t;
#define FSEMU_HUD_NOTICATION_ID(a, b, c, d, e) (a + b + c + d + e)
// FSEMU_HUD_NOTICATION_ID("fsemu_osmenu_no_menu"),

void fsemu_hud_init(void);
void fsemu_hud_update(void);

void fsemu_hud_show_notification(fsemu_hud_id_t notification_id,
                                 const char *title,
                                 const char *sub_title,
                                 const char *icon_name,
                                 int64_t duration_us);

#ifdef FSEMU_INTERNAL

#define fsemu_hud_log(format, ...) \
    fsemu_log("[FSEMU] [ HUD ] " format, ##__VA_ARGS__)

#endif  // FSEMU_INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_HUD_H_
