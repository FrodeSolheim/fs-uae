#ifndef FSEMU_TITLEBAR_H_
#define FSEMU_TITLEBAR_H_

#include "fsemu-common.h"
#include "fsemu-mouse.h"
#include "fsemu-render.h"
#include "fsemu-window.h"

#ifdef __cplusplus
extern "C" {
#endif

void fsemu_titlebar_init(void);

bool fsemu_titlebar_use_system(void);

uint32_t fsemu_titlebar_background_color(void);

bool fsemu_titlebar_visible(void);
int fsemu_titlebar_unscaled_height(void);
int fsemu_titlebar_height(void);

static inline bool fsemu_titlebar_static(void)
{
    return !fsemu_window_fullscreen() && !fsemu_titlebar_use_system();
}

static inline int fsemu_titlebar_static_height(void)
{
    return fsemu_titlebar_static() ? fsemu_titlebar_height() : 0;
}

void fsemu_titlebar_update(void);

// void fsemu_titlebar_set_size(int w, int h);
void fsemu_titlebar_set_width(int w);

#ifdef FSEMU_INTERNAL

bool fsemu_titlebar_want_cursor(void);
bool fsemu_titlebar_handle_mouse(fsemu_mouse_event_t *event);

// ----------------------------------------------------------------------------
// Logging
// ----------------------------------------------------------------------------

extern int fsemu_titlebar_log_level;

#define fsemu_titlebar_log(format, ...) \
    FSEMU_LOG(titlebar, "[FSE] [TBR]", format, ##__VA_ARGS__)

#define fsemu_titlebar_log_debug(format, ...) \
    FSEMU_LOG_DEBUG(titlebar, "[FSE] [TBR]", format, ##__VA_ARGS__)

#define fsemu_titlebar_log_error(format, ...) \
    FSEMU_LOG_ERROR(titlebar, "[FSE] [TBR]", format, ##__VA_ARGS__)

#define fsemu_titlebar_log_info(format, ...) \
    FSEMU_LOG_INFO(titlebar, "[FSE] [TBR]", format, ##__VA_ARGS__)

#define fsemu_titlebar_log_trace(format, ...) \
    FSEMU_LOG_TRACE(titlebar, "[FSE] [TBR]", format, ##__VA_ARGS__)

#define fsemu_titlebar_log_warning(format, ...) \
    FSEMU_LOG_WARNING(titlebar, "[FSE] [TBR]", format, ##__VA_ARGS__)

// ----------------------------------------------------------------------------

#endif  // FSEMU_INTERNAL

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_TITLEBAR_H_
