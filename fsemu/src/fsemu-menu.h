#ifndef FSEMU_MENU_H_
#define FSEMU_MENU_H_

#include <stdbool.h>
#include <stdint.h>

#include "fsemu-config.h"

#ifdef __cplusplus
extern "C" {
#endif

struct fsemu_menu;
typedef struct fsemu_menu fsemu_menu_t;

struct fsemu_menu_item;
typedef struct fsemu_menu_item fsemu_menu_item_t;

fsemu_menu_t *fsemu_menu_new(void);

void fsemu_menu_ref(fsemu_menu_t *menu);

void fsemu_menu_unref(fsemu_menu_t *menu);

int fsemu_menu_item_count(void);

fsemu_menu_item_t *fsemu_menu_items(void);

// FIXME: Ownership to reference? Terminology; add vs assign?
void fsemu_menu_add_item(fsemu_menu_t *menu, fsemu_menu_item_t *item);

fsemu_menu_item_t *fsemu_menu_item_new(void);

void fsemu_menu_item_ref(fsemu_menu_item_t *item);

void fsemu_menu_item_unref(fsemu_menu_item_t *item);

fsemu_menu_item_t *fsemu_menu_item_new_with_title(const char *title);

fsemu_menu_item_t *fsemu_menu_item_new_heading_with_title(const char *title);

const char *fsemu_menu_item_title(fsemu_menu_item_t *item);

void fsemu_menu_item_set_title(fsemu_menu_item_t *item, const char *title);

#define FSEMU_MENU_ITEM_FLAG_HEADING 1
#define FSEMU_MENU_ITEM_FLAG_DISABLED 2

bool fsemu_menu_item_check_flag(fsemu_menu_item_t *item, uint32_t flag);

void fsemu_menu_item_clear_flag(fsemu_menu_item_t *item, uint32_t flag);

void fsemu_menu_item_set_flag(fsemu_menu_item_t *item, uint32_t flag);

bool fsemu_menu_item_heading(fsemu_menu_item_t *item);
void fsemu_menu_item_set_heading(fsemu_menu_item_t *item, bool heading);

bool fsemu_menu_item_enabled(fsemu_menu_item_t *item);
void fsemu_menu_item_set_enabled(fsemu_menu_item_t *item, bool enabled);

typedef void (*fsemu_menu_item_activate_f)(void);

void fsemu_menu_item_set_on_activate(fsemu_menu_item_t *item,
                                     fsemu_menu_item_activate_f on_activate);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_MENU_H_
