#ifndef FSEMU_MENU_H_
#define FSEMU_MENU_H_

#include <stdbool.h>
#include <stdint.h>

#include "fsemu-config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FSEMU_MENU_RESULT_IS_MENU(ptr) ((intptr_t) (ptr) > 0)
#define FSEMU_MENU_RESULT_CLOSE ((void *) -1)
#define FSEMU_MENU_RESULT_POP1 ((void *) -3)
#define FSEMU_MENU_RESULT_POP1_CLOSE ((void *) -5)

struct fsemu_menu;
typedef struct fsemu_menu fsemu_menu_t;

struct fsemu_menu_item;
typedef struct fsemu_menu_item fsemu_menu_item_t;

fsemu_menu_t *fsemu_menu_new(void);

void fsemu_menu_ref(fsemu_menu_t *menu);

void fsemu_menu_unref(fsemu_menu_t *menu);

typedef void (*fsemu_menu_update_function_t)(fsemu_menu_t *menu);

void fsemu_menu_set_update_function(fsemu_menu_t *menu,
                                    fsemu_menu_update_function_t function);

void *fsemu_menu_data(fsemu_menu_t *menu);
void fsemu_menu_set_data(fsemu_menu_t *menu, void *data);

void fsemu_menu_update(fsemu_menu_t *menu);

int fsemu_menu_count_items(fsemu_menu_t *menu);

fsemu_menu_item_t *fsemu_menu_get_item(fsemu_menu_t *menu, int index);

// fsemu_menu_item_t *fsemu_menu_items(void);

// FIXME: Ownership to reference? Terminology; add vs assign?
void fsemu_menu_add_item(fsemu_menu_t *menu, fsemu_menu_item_t *item);

fsemu_menu_item_t *fsemu_menu_item_new(void);

void fsemu_menu_item_ref(fsemu_menu_item_t *item);

void fsemu_menu_item_unref(fsemu_menu_item_t *item);

fsemu_menu_item_t *fsemu_menu_item_new_with_title(const char *title);

fsemu_menu_item_t *fsemu_menu_item_new_heading_with_title(const char *title);

const char *fsemu_menu_item_title(fsemu_menu_item_t *item);

void fsemu_menu_item_set_title(fsemu_menu_item_t *item, const char *title);
void fsemu_menu_item_assign_title(fsemu_menu_item_t *item, char *title);

#define FSEMU_MENU_ITEM_FLAG_HEADING 1
#define FSEMU_MENU_ITEM_FLAG_DISABLED 2

bool fsemu_menu_item_check_flag(fsemu_menu_item_t *item, uint32_t flag);

void fsemu_menu_item_clear_flag(fsemu_menu_item_t *item, uint32_t flag);

void fsemu_menu_item_set_flag(fsemu_menu_item_t *item, uint32_t flag);

bool fsemu_menu_item_heading(fsemu_menu_item_t *item);
void fsemu_menu_item_set_heading(fsemu_menu_item_t *item, bool heading);

bool fsemu_menu_item_selectable(fsemu_menu_item_t *item);

bool fsemu_menu_item_enabled(fsemu_menu_item_t *item);
void fsemu_menu_item_set_enabled(fsemu_menu_item_t *item, bool enabled);

void *fsemu_menu_item_data(fsemu_menu_item_t *item);
void fsemu_menu_item_set_data(fsemu_menu_item_t *item, void *data);
void fsemu_menu_item_set_data_with_int(fsemu_menu_item_t *item,
                                       void *data,
                                       int intvalue);

int fsemu_menu_item_int(fsemu_menu_item_t *item);
void fsemu_menu_item_set_int(fsemu_menu_item_t *item, int intvalue);

typedef fsemu_menu_t *(*fsemu_menu_item_on_activate_f)(
    fsemu_menu_item_t *item);

void fsemu_menu_item_on_activate(fsemu_menu_item_t *item,
                                 fsemu_menu_item_on_activate_f on_activate);

typedef void (*fsemu_menu_item_activate_f)(void);

void fsemu_menu_item_set_on_activate(fsemu_menu_item_t *item,
                                     fsemu_menu_item_activate_f on_activate);

fsemu_menu_t *fsemu_menu_item_activate(fsemu_menu_item_t *item);

bool fsemu_menu_item_selected_initially(fsemu_menu_item_t *item);

// This can be used to mark an menu item for being initially selected. It can
// not be used to change selection.
void fsemu_menu_item_set_selected_initially(fsemu_menu_item_t *item,
                                            bool selected);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_MENU_H_
