#ifndef FSEMU_OSKEYBOARD_H_
#define FSEMU_OSKEYBOARD_H_

#include <stdint.h>

#include "fsemu-action.h"
#include "fsemu-common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct fsemu_oskeyboard;
typedef struct fsemu_oskeyboard fsemu_oskeyboard_t;

// struct fsemu_oskeyboard_layout;
// typedef struct fsemu_oskeyboard_layout fsemu_oskeyboard_layout_t;

struct fsemu_oskeyboard_row;
typedef struct fsemu_oskeyboard_row fsemu_oskeyboard_row_t;

struct fsemu_oskeyboard_key;
typedef struct fsemu_oskeyboard_key fsemu_oskeyboard_key_t;

void fsemu_oskeyboard_init(void);

void fsemu_oskeyboard_update(void);

bool fsemu_oskeyboard_open(void);

void fsemu_oskeyboard_set_open(bool open);

void fsemu_oskeyboard_toggle_open(void);

void fsemu_oskeyboard_toggle_position(void);

// FIXME: Move to FSEMU_GUI_NAVIGATE_
enum {
    FSEMU_OSKEYBOARD_NAVIGATE_NONE,
    FSEMU_OSKEYBOARD_NAVIGATE_UP,
    FSEMU_OSKEYBOARD_NAVIGATE_RIGHT,
    FSEMU_OSKEYBOARD_NAVIGATE_DOWN,
    FSEMU_OSKEYBOARD_NAVIGATE_LEFT,
    FSEMU_OSKEYBOARD_NAVIGATE_PRIMARY,
    FSEMU_OSKEYBOARD_NAVIGATE_SECONDARY,
    FSEMU_OSKEYBOARD_NAVIGATE_TERTIARY,
    FSEMU_OSKEYBOARD_NAVIGATE_BACK,
    FSEMU_OSKEYBOARD_NAVIGATE_CLOSE,
};

void fsemu_oskeyboard_navigate(int navigate, int32_t state);
// void fsemu_oskeyboard_navigate_up(void);

// FIXME: Auto-set as active keyboard?
fsemu_oskeyboard_t* fsemu_oskeyboard_new(void);

void fsemu_oskeyboard_add(fsemu_oskeyboard_t* keyboard);

void fsemu_oskeyboard_set_active(fsemu_oskeyboard_t* keyboard);

// FIXME: Or fsemu_oskeyboard_new_row or fsemu_oskeyboard_add_row
// fsemu_oskeyboard_row_t *fsemu_oskeyboard_row_new(fsemu_oskeyboard_t
// *keyboard);

fsemu_oskeyboard_row_t* fsemu_oskeyboard_new_row(fsemu_oskeyboard_t* keyboard);

// fsemu_oskeyboard_key_t *fsemu_oskeyboard_key_new(fsemu_oskeyboard_row_t
// *row); fsemu_oskeyboard_key_t
// *fsemu_oskeyboard_key_new_with_name(fsemu_oskeyboard_row_t *row, const char
// *name);

// Or fsemu_oskeyboard_key_new_with_name
fsemu_oskeyboard_key_t* fsemu_oskeyboard_row_new_key_with_name(fsemu_oskeyboard_row_t* row,
                                                               const char* name);
void fsemu_oskeyboard_row_set_bottom_margin(fsemu_oskeyboard_row_t* row, int bottom_margin);

// key = fsemu_oskeyboard_new_key();
// key = fsemu_oskeyboard_new_key_in_layout(layout, "A");
// key = fsemu_oskeyboard_key_new_in_row(row, "A");

void fsemu_oskeyboard_key_set_action(fsemu_oskeyboard_key_t* key, fsemu_action_t action);
void fsemu_oskeyboard_key_set_height(fsemu_oskeyboard_key_t* key, int height);
void fsemu_oskeyboard_key_set_left_margin(fsemu_oskeyboard_key_t* key, int left_margin);
void fsemu_oskeyboard_key_set_width(fsemu_oskeyboard_key_t* key, int width);

extern int fsemu_oskeyboard_log_level;

#define fsemu_oskeyboard_log(format, ...)                                   \
    if (fsemu_likely(fsemu_oskeyboard_log_level >= FSEMU_LOG_LEVEL_INFO)) { \
        fsemu_log("[FSE] [OSK] " format, ##__VA_ARGS__);                    \
    }

#define fsemu_oskeyboard_log_debug(format, ...)                                \
    if (fsemu_unlikely(fsemu_oskeyboard_log_level >= FSEMU_LOG_LEVEL_DEBUG)) { \
        fsemu_log("[FSE] [OSK] " format, ##__VA_ARGS__);                       \
    }

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_OSKEYBOARD_H_
