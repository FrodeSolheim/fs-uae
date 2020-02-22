#ifndef FSEMU_BACKGROUND_H_
#define FSEMU_BACKGROUND_H_

#include "fsemu-common.h"
#include "fsemu-gui.h"
#include "fsemu-image.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef fsemu_gui_item_t fsemu_widget_t;

void fsemu_background_init(void);

/*
void fsemu_widget_init(fsemu_widget_t *widget);
void fsemu_widget_set_image(fsemu_widget_t *widget, fsemu_image_t *image);
*/

#ifdef __cplusplus
}
#endif

#endif  // FSEMU_BACKGROUND_H_
