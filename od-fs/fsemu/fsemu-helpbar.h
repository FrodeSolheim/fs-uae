#ifndef FSEMU_HELPBAR_H_
#define FSEMU_HELPBAR_H_

#include "fsemu-config.h"
#include "fsemu-led.h"
#include "fsemu-widget.h"

#ifdef __cplusplus
extern "C" {
#endif

struct fsemu_helpbar_t;
typedef struct fsemu_helpbar_t fsemu_helpbar_t;

struct fsemu_helpbar_command_t;
typedef struct fsemu_helpbar_command_t fsemu_helpbar_command_t;

fsemu_helpbar_t *fsemu_helpbar_create(void);

fsemu_widget_t *fsemu_helpbar_widget(fsemu_helpbar_t *helpbar);

void fsemu_helpbar_update_widget(fsemu_helpbar_t *helpbar);

// ----------------------------------------------------------------------------

fsemu_helpbar_command_t *fsemu_helpbar_create_command(fsemu_helpbar_t *helpbar,
                                                      const char *label,
                                                      int modifiers,
                                                      int key);

// ----------------------------------------------------------------------------

// void fsemu_helpbar_command_set_label(fsemu_helpbar_command_t *command,
//                                    const char *label);

void fsemu_helpbar_command_add_key(fsemu_helpbar_command_t *command,
                                   int modifiers,
                                   int key);
void fsemu_helpbar_command_add_button(fsemu_helpbar_command_t *command,
                                      int modifiers,
                                      int button);

// ----------------------------------------------------------------------------

// FIXME: REMOVE
void fsemu_helpbar_init(void);

// FIXME: REMOVE
void fsemu_helpbar_update(void);

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_HELPBAR_H_
