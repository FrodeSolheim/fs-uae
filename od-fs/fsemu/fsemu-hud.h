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

// This initializes a queue, and should be called as early as possible.
void fsemu_hud_init_early(void);

void fsemu_hud_init(void);
void fsemu_hud_update(void);

#define FSEMU_HUD_ID(s)                                                                           \
    ((uint64_t)(s)[0] | (uint64_t)(s)[1] << 8 | (uint64_t)(s)[2] << 16 | (uint64_t)(s)[3] << 24 | \
     (uint64_t)(s)[4] << 32 | (uint64_t)(s)[5] << 40 | (uint64_t)(s)[6] << 48 |                   \
     (uint64_t)(s)[7] << 56)

void fsemu_hud_notify(fsemu_hud_id_t id, const char* icon, const char* title, const char* subtitle);

void fsemu_hud_notify_with_duration(fsemu_hud_id_t id, const char* icon, const char* title,
                                    const char* subtitle, int64_t duration);

void fsemu_hud_show_notification(fsemu_hud_id_t notification_id, const char* title,
                                 const char* sub_title, const char* icon_name, int64_t duration_us);

#ifdef FSEMU_INTERNAL

#define fsemu_hud_log(format, ...) fsemu_log("[FSE] [HUD] " format, ##__VA_ARGS__)

#endif  // FSEMU_INTERNAL

void fse_notify(uint32_t type, const char* format, ...);
void fs_emu_warning(const char* format, ...);
void fs_emu_deprecated(const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_HUD_H_
