#ifndef FSEMU_OSMENU_H_
#define FSEMU_OSMENU_H_

#include <stdint.h>

#include "fsemu-action.h"
#include "fsemu-config.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_osmenu_init(void);

void fsemu_osmenu_update(void);

bool fsemu_osmenu_open(void);

int fsemu_osmenu_open_offset(void);

void fsemu_osmenu_set_open(bool open);

void fsemu_osmenu_toggle_open(void);

void fsemu_osmenu_navigate(int navigate, fsemu_action_state_t state);

extern int fsemu_osmenu_log_level;

#define fsemu_osmenu_log(format, ...)                                   \
    if (fsemu_likely(fsemu_osmenu_log_level >= FSEMU_LOG_LEVEL_INFO)) { \
        fsemu_log("[FSE] [OSM] " format, ##__VA_ARGS__);                \
    }

#define fsemu_osmenu_log_debug(format, ...)                                \
    if (fsemu_unlikely(fsemu_osmenu_log_level >= FSEMU_LOG_LEVEL_DEBUG)) { \
        fsemu_log("[FSE] [OSM] " format, ##__VA_ARGS__);                   \
    }

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_OSMENU_H_
