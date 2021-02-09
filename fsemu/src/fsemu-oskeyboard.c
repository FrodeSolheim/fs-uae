#define FSEMU_INTERNAL 1
#include "fsemu-oskeyboard.h"

#include "fsemu-action.h"
#include "fsemu-hud.h"
#include "fsemu-input.h"
#include "fsemu-layer.h"
#include "fsemu-util.h"
#include "fsemu-video.h"
#include "fsemu-widget.h"

// FIXME: Improvements needed:
// - Key and button repeat to make navigation more efficient
// - Up and down nagivation should go to nearest key based on screen distance
//   between key centers, and not key index.
// - Pressed and held keys must be released when the keyboard is closed (?)
// - Support for moving keyboard up / down
// - Support for holding shift (etc) keys down while pressing other keys
// - Support for mouse input (hovering, clicking, right-clicking to hold)
// - Indicating pressed state for keys
// - Symbols on keys
// - Maybe: change key symbols based on held modifier keys?
// - FIXME: Need special support for automatically held "keys" such as
//   the play button on a C64 tape deck.
// - Alternative: Let back button be mapped to the backspace equivalent on
//   virtual keyboard, and close the keyboard with the open/close button
//   instead of the back button.
// - DONE: IMPORTANT: RELEASE THE SAME KEY AS THE ONE YOU HELD DOWN

// ----------------------------------------------------------------------------

// #define FSEMU_OSKEYBOARD_START_OPEN

#define KEY_WIDTH 56
#define KEY_HEIGHT 56

// ----------------------------------------------------------------------------

int fsemu_oskeyboard_log_level = FSEMU_LOG_LEVEL_INFO;

struct fsemu_oskeyboard {
    fsemu_oskeyboard_key_t *selected_key;
    fsemu_oskeyboard_key_t *pressed_key;
    fsemu_widget_t *selected_w;
    bool dirty;
    fsemu_oskeyboard_row_t *first_row;
    fsemu_oskeyboard_row_t *last_row;
    // Size in key units * 100 (i.e. 300 = 3 standard key widths)
    fsemu_size_t size;
    fsemu_widget_t *widget;
};

struct fsemu_oskeyboard_row {
    int bottom_margin;
    fsemu_oskeyboard_key_t *first_key;
    int height;
    // Index of row in keyboard (bottom row is 0)
    int index;
    fsemu_oskeyboard_t *keyboard;
    fsemu_oskeyboard_row_t *next;
    fsemu_oskeyboard_key_t *last_key;
};

struct fsemu_oskeyboard_key {
    fsemu_action_t action;
    // Index of key in row (left key is 0)
    int height;
    int index;
    int left_margin;
    fsemu_oskeyboard_key_t *next;
    fsemu_oskeyboard_row_t *row;
    fsemu_widget_t *widget;
    int width;
};

// ----------------------------------------------------------------------------

static struct {
    fsemu_oskeyboard_t *active;
    fsemu_widget_t *background_w;
    fsemu_widget_t *container_w;
    bool initialized;
    bool open;
    bool was_open;
} fsemu_oskeyboard;

// ----------------------------------------------------------------------------

fsemu_oskeyboard_t *fsemu_oskeyboard_new(void)
{
    fsemu_oskeyboard_t *keyboard = FSEMU_UTIL_MALLOC0(fsemu_oskeyboard_t);
    return keyboard;
}

static fsemu_oskeyboard_row_t *fsemu_oskeyboard_row_new(void)
{
    fsemu_oskeyboard_row_t *row = FSEMU_UTIL_MALLOC0(fsemu_oskeyboard_row_t);
    row->height = 100;
    return row;
}

fsemu_oskeyboard_row_t *fsemu_oskeyboard_new_row(fsemu_oskeyboard_t *keyboard)
{
    fsemu_oskeyboard_row_t *row = fsemu_oskeyboard_row_new();
    row->keyboard = keyboard;
    if (keyboard->last_row) {
        row->index = keyboard->last_row->index + 1;
        keyboard->last_row->next = row;
        keyboard->last_row = row;
    } else {
        row->index = 0;
        keyboard->first_row = row;
        keyboard->last_row = row;
    }
    return row;
}

static fsemu_oskeyboard_key_t *fsemu_oskeyboard_key_new(void)
{
    fsemu_oskeyboard_key_t *key = FSEMU_UTIL_MALLOC0(fsemu_oskeyboard_key_t);
    key->width = 100;
    key->height = 100;
    return key;
}

fsemu_oskeyboard_key_t *fsemu_oskeyboard_row_new_key_with_name(
    fsemu_oskeyboard_row_t *row, const char *name)
{
    fsemu_oskeyboard_key_t *key = fsemu_oskeyboard_key_new();
    key->row = row;
    if (row->last_key) {
        key->index = row->last_key->index + 1;
        row->last_key->next = key;
        row->last_key = key;
    } else {
        key->index = 0;
        row->first_key = key;
        row->last_key = key;
    }
    key->row->keyboard->dirty = true;
    return key;
}

void fsemu_oskeyboard_row_set_bottom_margin(fsemu_oskeyboard_row_t *row,
                                            int bottom_margin)
{
    row->bottom_margin = bottom_margin;
    row->keyboard->dirty = true;
}

void fsemu_oskeyboard_key_set_left_margin(fsemu_oskeyboard_key_t *key,
                                          int left_margin)
{
    key->left_margin = left_margin;
    key->row->keyboard->dirty = true;
}

void fsemu_oskeyboard_key_set_height(fsemu_oskeyboard_key_t *key, int height)
{
    key->height = height;
    key->row->keyboard->dirty = true;
}

void fsemu_oskeyboard_key_set_width(fsemu_oskeyboard_key_t *key, int width)
{
    key->width = width;
    key->row->keyboard->dirty = true;
}

void fsemu_oskeyboard_key_set_action(fsemu_oskeyboard_key_t *key,
                                     fsemu_action_t action)
{
    key->action = action;
}

bool fsemu_oskeyboard_open(void)
{
    return fsemu_oskeyboard.open;
}

static void fsemu_oskeyboard_release_keys(fsemu_oskeyboard_t *keyboard)
{
    // FIXME: Release currently held keys!
    fsemu_oskeyboard_log_debug(
        "\nFIXME: ON-SCREEN KEYBOARD - RELEASE PRESSED KEYS\n\n");
}

static void fsemu_oskeyboard_cannot_show(const char *message)
{
    fsemu_hud_show_notification(
        FSEMU_HUD_NOTICATION_ID(
            0x411a51a9, 0x7ef6, 0x4169, 0x9240, 0x7058f8ffd15a),
        "Cannot show keyboard",
        message,
        FSEMU_HUD_NOTIFICATION_WARNING_ICON,
        FSEMU_HUD_NOTIFICATION_DEFAULT_DURATION);
}

void fsemu_oskeyboard_set_open(bool open)
{
    fsemu_oskeyboard_log_debug("fsemu_oskeyboard_set_open open=%d\n", open);
    if (!fsemu_oskeyboard.initialized) {
        fsemu_oskeyboard_cannot_show("Keyboard module was not initialized");
        return;
    }

    if (!fsemu_oskeyboard.active) {
        fsemu_oskeyboard_cannot_show("No keyboard activated by emulator");
        return;
    }

    if (!open) {
        if (fsemu_oskeyboard.active) {
            fsemu_oskeyboard_release_keys(fsemu_oskeyboard.active);
        }
    }
    fsemu_oskeyboard.open = open;
    fsemu_widget_set_visible(fsemu_oskeyboard.container_w, open);
}

void fsemu_oskeyboard_toggle_open(void)
{
    fsemu_oskeyboard_set_open(!fsemu_oskeyboard_open());
}

static void fsemu_oskeyboard_update_keyboard_size(fsemu_oskeyboard_t *keyboard)
{
    int width = 0;
    int height = 0;
    for (fsemu_oskeyboard_row_t *row = keyboard->first_row; row;
         row = row->next) {
        height += 100;
        int row_width = 0;
        for (fsemu_oskeyboard_key_t *key = row->first_key; key;
             key = key->next) {
            row_width += key->left_margin;
            row_width += key->width;
        }
        if (row_width > width) {
            width = row_width;
        }
    }
    keyboard->size.w = width;
    keyboard->size.h = height;
    // fsemu_oskeyboard_log_debug("KEYBOARD %d x %d\n", width, height);
}

static void fsemu_oskeyboard_update_keyboard_key(fsemu_oskeyboard_t *keyboard,
                                                 fsemu_oskeyboard_key_t *key,
                                                 int x,
                                                 int y)
{
    int w = key->width * KEY_WIDTH / 100;
    // int h = key->row->height * KEY_HEIGHT / 100;
    int h = key->height * KEY_HEIGHT / 100;
    int p = 5;

    fsemu_widget_t *widget;
    if (key->widget == NULL) {
        key->widget = fsemu_widget_new();
        fsemu_widget_add_child(keyboard->widget, key->widget);
        // fsemu_gui_add_item(key->widget);

        widget = key->widget;
        fsemu_widget_set_color(widget, FSEMU_RGBA(0xffffff80));
#if 1
        fsemu_widget_set_top_2(
            widget, -(y + h - p), FSEMU_WIDGET_PARENT_BOTTOM);
        fsemu_widget_set_right_2(widget, x + w - p, FSEMU_WIDGET_PARENT_LEFT);
        fsemu_widget_set_bottom_2(
            widget, -(y + p), FSEMU_WIDGET_PARENT_BOTTOM);
        fsemu_widget_set_left_2(widget, x + p, FSEMU_WIDGET_PARENT_LEFT);
#else
        fsemu_widget_set_top_2(
            widget, -(y + h - p), FSEMU_WIDGET_SCREEN_BOTTOM);
        fsemu_widget_set_right_2(widget, x + w - p, FSEMU_WIDGET_SCREEN_LEFT);
        fsemu_widget_set_bottom_2(
            widget, -(y + p), FSEMU_WIDGET_SCREEN_BOTTOM);
        fsemu_widget_set_left_2(widget, x + p, FSEMU_WIDGET_SCREEN_LEFT);
#endif
        fsemu_widget_set_z_index(widget, FSEMU_LAYER_OSKEYBOARD + 2);
        // FIXME: Not needed for widget?
        fsemu_widget_set_visible(widget, true);

        // FIXME: Temp
        static int count = 0;
        if (++count <= 2) {
            fsemu_widget_set_flag(widget, FSEMU_WIDGET_FLAG_DEBUG);
        }
    }

    if (key == keyboard->selected_key) {
        widget = keyboard->selected_w;
#if 1
        fsemu_widget_set_top_2(widget, -(y + h), FSEMU_WIDGET_PARENT_BOTTOM);
        fsemu_widget_set_right_2(widget, x + w, FSEMU_WIDGET_PARENT_LEFT);
        fsemu_widget_set_bottom_2(widget, -(y), FSEMU_WIDGET_PARENT_BOTTOM);
        fsemu_widget_set_left_2(widget, x, FSEMU_WIDGET_PARENT_LEFT);
#else
        fsemu_widget_set_top_2(widget, -(y + h), FSEMU_WIDGET_SCREEN_BOTTOM);
        fsemu_widget_set_right_2(widget, x + w, FSEMU_WIDGET_SCREEN_LEFT);
        fsemu_widget_set_bottom_2(widget, -(y), FSEMU_WIDGET_SCREEN_BOTTOM);
        fsemu_widget_set_left_2(widget, x, FSEMU_WIDGET_SCREEN_LEFT);
#endif
    }
}

static void fsemu_oskeyboard_update_keyboard_keys(fsemu_oskeyboard_t *keyboard)
{
    int y = 0;
    // FIXME: Temp
    y = 60;
    for (fsemu_oskeyboard_row_t *row = keyboard->first_row; row;
         row = row->next) {
        y += row->bottom_margin * KEY_HEIGHT / 100;
        int x = 0;
        // FIXME: Temp
        // x = 240 + (1440 - keyboard->size.w * KEY_WIDTH / 100) / 2;
        x = (1440 - keyboard->size.w * KEY_WIDTH / 100) / 2;
        for (fsemu_oskeyboard_key_t *key = row->first_key; key;
             key = key->next) {
            x += key->left_margin * KEY_WIDTH / 100;

            fsemu_oskeyboard_update_keyboard_key(keyboard, key, x, y);
            x += key->width * KEY_WIDTH / 100;
        }
        y += row->height * KEY_HEIGHT / 100;
    }
}

static fsemu_oskeyboard_key_t *fsemu_oskeyboard_find_key_by_index(
    fsemu_oskeyboard_t *keyboard, int row_index, int key_index)
{
    for (fsemu_oskeyboard_row_t *row = keyboard->first_row; row;
         row = row->next) {
        if (row->index != row_index) {
            continue;
        }
        for (fsemu_oskeyboard_key_t *key = row->first_key; key;
             key = key->next) {
            if (key->index != key_index) {
                continue;
            }
            return key;
        }
    }
    return NULL;
}

// ----------------------------------------------------------------------------

static void fsemu_oskeyboard_set_position(bool top)
{
    fsemu_widget_t *widget = fsemu_oskeyboard.container_w;
    if (widget == NULL) {
        return;
    }
    if (top) {
        fsemu_widget_set_top_2(widget, 0, FSEMU_WIDGET_SCREEN_TOP);
        fsemu_widget_set_bottom_2(widget, 540, FSEMU_WIDGET_SCREEN_TOP);
    } else {
        fsemu_widget_set_top_2(widget, 540, FSEMU_WIDGET_SCREEN_TOP);
        fsemu_widget_set_bottom_2(widget, 1080, FSEMU_WIDGET_SCREEN_TOP);
    }
}

void fsemu_oskeyboard_toggle_position(void)
{
    fsemu_widget_t *widget = fsemu_oskeyboard.container_w;
    if (widget == NULL) {
        return;
    }
    if (widget->top.offset == 0) {
        fsemu_oskeyboard_set_position(false);
    } else {
        fsemu_oskeyboard_set_position(true);
    }
}

static void fsemu_oskeyboard_navigate_primary(fsemu_oskeyboard_t *keyboard,
                                              int32_t state)
{
    fsemu_oskeyboard_log_debug("\nprimary\n\n");
    // FIXME: We should register that we have "pressed this action" and
    // release the key action when we close the keyboard, if not already
    // released!
    if (state) {
        fsemu_action_t action = keyboard->selected_key->action;
        fsemu_input_process_action(action, FSEMU_ACTION_STATE_MAX);
        keyboard->pressed_key = keyboard->selected_key;
    } else {
        if (keyboard->pressed_key) {
            fsemu_action_t action = keyboard->pressed_key->action;
            fsemu_input_process_action(action, 0);
            keyboard->pressed_key = NULL;
        }
    }
}

static void fsemu_oskeyboard_navigate_secondary(fsemu_oskeyboard_t *keyboard,
                                                int32_t state)
{
    fsemu_oskeyboard_log_debug("\nsecondary\n\n");
    fsemu_action_t action = keyboard->selected_key->action;
    // FIXME: We should register that we have "pressed this action" and
    // release the key action when we close the keyboard, if not already
    // released!
    if (state) {
        fsemu_input_process_action(action, FSEMU_ACTION_STATE_MAX);
    }
    // We are not sending "release events" for this action, because we want
    // the key to be held.
    // FIXME: We might want to do some more book-keeping here...
    // FIXME: Primary action should work to release the key for now. Later,
    // we want to be able to toggle off the hold state by pressing
    // secondary once more.
}

static void fsemu_oskeyboard_navigate_tertiary(fsemu_oskeyboard_t *keyboard,
                                               int32_t state)
{
    if (state) {
        fsemu_oskeyboard_log_debug("\ntertiary\n\n");
        fsemu_oskeyboard_toggle_position();
    }
}

static void fsemu_oskeyboard_navigate_back(fsemu_oskeyboard_t *keyboard,
                                           int32_t state)
{
    if (state) {
        fsemu_oskeyboard_log_debug("\nback\n\n");
        fsemu_oskeyboard_set_open(false);
    }
}

static void fsemu_oskeyboard_navigate_close(fsemu_oskeyboard_t *keyboard,
                                            int32_t state)
{
    if (state) {
        fsemu_oskeyboard_log_debug("\nclose\n\n");
        fsemu_oskeyboard_set_open(false);
    }
}

void fsemu_oskeyboard_navigate(int navigate, int32_t state)
{
    if (navigate == FSEMU_OSKEYBOARD_NAVIGATE_NONE) {
        return;
    }

    fsemu_oskeyboard_t *keyboard = fsemu_oskeyboard.active;
    if (keyboard == NULL) {
        return;
    }
    fsemu_oskeyboard_key_t *selected = keyboard->selected_key;
    if (selected == NULL) {
        // FIXME: Warning?
        return;
    }
    int row_index = selected->row->index;
    int key_index = selected->index;
    if (navigate == FSEMU_OSKEYBOARD_NAVIGATE_UP) {
        if (!state) {
            return;
        }
        fsemu_oskeyboard_log_debug("\nup\n\n");
        // fsemu_oskeyboard_navigate_up();
        if (row_index == keyboard->last_row->index) {
            // return;
            // Wrap around
            row_index = 0;
        } else {
            row_index += 1;
        }
        fsemu_oskeyboard_row_t *row = keyboard->first_row;
        for (int i = 0; i < row_index; i++) {
            row = row->next;
        }
        if (key_index > row->last_key->index) {
            key_index = row->last_key->index;
        }
        keyboard->selected_key =
            fsemu_oskeyboard_find_key_by_index(keyboard, row_index, key_index);
    } else if (navigate == FSEMU_OSKEYBOARD_NAVIGATE_RIGHT) {
        if (!state) {
            return;
        }
        fsemu_oskeyboard_log_debug("\nright\n\n");
        // fsemu_oskeyboard_navigate_right();
        if (key_index == selected->row->last_key->index) {
            // return;
            // Wrap around
            key_index = 0;
        } else {
            key_index += 1;
        }
        keyboard->selected_key =
            fsemu_oskeyboard_find_key_by_index(keyboard, row_index, key_index);
    } else if (navigate == FSEMU_OSKEYBOARD_NAVIGATE_DOWN) {
        if (!state) {
            return;
        }
        fsemu_oskeyboard_log_debug("\ndown\n\n");
        if (row_index == 0) {
            // return;
            // Wrap around
            row_index = keyboard->last_row->index;
        } else {
            row_index -= 1;
        }
        fsemu_oskeyboard_row_t *row = keyboard->first_row;
        for (int i = 0; i < row_index; i++) {
            row = row->next;
        }
        if (key_index > row->last_key->index) {
            key_index = row->last_key->index;
        }
        // fsemu_oskeyboard_log_debug("\ndown -> %d %d\n\n", row_index,
        // key_index);
        keyboard->selected_key =
            fsemu_oskeyboard_find_key_by_index(keyboard, row_index, key_index);
    } else if (navigate == FSEMU_OSKEYBOARD_NAVIGATE_LEFT) {
        if (!state) {
            return;
        }
        fsemu_oskeyboard_log_debug("\nleft\n\n");
        if (key_index == 0) {
            // return;
            // Wrap around
            key_index = selected->row->last_key->index;
        } else {
            key_index -= 1;
        }
        keyboard->selected_key =
            fsemu_oskeyboard_find_key_by_index(keyboard, row_index, key_index);
    } else if (navigate == FSEMU_OSKEYBOARD_NAVIGATE_PRIMARY) {
        fsemu_oskeyboard_navigate_primary(keyboard, state);
    } else if (navigate == FSEMU_OSKEYBOARD_NAVIGATE_SECONDARY) {
        fsemu_oskeyboard_navigate_secondary(keyboard, state);
    } else if (navigate == FSEMU_OSKEYBOARD_NAVIGATE_TERTIARY) {
        fsemu_oskeyboard_navigate_tertiary(keyboard, state);
    } else if (navigate == FSEMU_OSKEYBOARD_NAVIGATE_BACK) {
        fsemu_oskeyboard_navigate_back(keyboard, state);
    } else if (navigate == FSEMU_OSKEYBOARD_NAVIGATE_CLOSE) {
        fsemu_oskeyboard_navigate_close(keyboard, state);
    } else {
        fsemu_error("Invalid navigation command in %s\n", __func__);
    }
}

void fsemu_oskeyboard_add(fsemu_oskeyboard_t *keyboard)
{
    // No-op for now?
}

void fsemu_oskeyboard_set_active(fsemu_oskeyboard_t *keyboard)
{
    fsemu_oskeyboard.active = keyboard;
}

void fsemu_oskeyboard_update(void)
{
    if (!fsemu_oskeyboard.open) {
        return;
    }
    fsemu_oskeyboard_t *keyboard = fsemu_oskeyboard.active;
    if (keyboard == NULL) {
        return;
    }

    if (keyboard->widget == NULL) {
        keyboard->widget =
            fsemu_widget_new_with_name("fsemu_oskeyboard_keyboard");
        fsemu_widget_add_child(fsemu_oskeyboard.container_w, keyboard->widget);

        fsemu_widget_t *widget = keyboard->widget;
        fsemu_widget_set_color(widget, FSEMU_COLOR_TRANSPARENT);
        // FIXME: Reconsider position/size of this. Enable debug color to see.
        // fsemu_widget_set_color(widget, FSEMU_COLOR_RGBA(0xff000080));

#if 1
        fsemu_widget_set_top_2(widget, 0, FSEMU_WIDGET_PARENT_TOP);
        fsemu_widget_set_right_2(widget, -240, FSEMU_WIDGET_PARENT_RIGHT);
        fsemu_widget_set_bottom_2(widget, 0, FSEMU_WIDGET_PARENT_BOTTOM);
        // fsemu_widget_set_left_2(widget, 240, FSEMU_WIDGET_PARENT_LEFT);
        fsemu_widget_set_left_2(widget, 0, FSEMU_WIDGET_PARENT_LEFT);
#else
        fsemu_widget_set_top_2(widget, -540, FSEMU_WIDGET_SCREEN_BOTTOM);  //
        fsemu_widget_set_right_2(widget, 1440 + 240, FSEMU_WIDGET_SCREEN_LEFT);
        fsemu_widget_set_bottom_2(widget, 0, FSEMU_WIDGET_SCREEN_BOTTOM);
        fsemu_widget_set_left_2(widget, 240, FSEMU_WIDGET_SCREEN_LEFT);
#endif

        fsemu_widget_set_z_index(widget, FSEMU_LAYER_OSKEYBOARD + 1);
        // FIXME: Not needed for widget?
        fsemu_widget_set_visible(widget, true);
        // FIXME: Add to fsemu_oskeyboard.background_w instead?
        // (or another high-level parent also containing background)
        // fsemu_gui_add_item(w);
    }

    if (keyboard->selected_w == NULL) {
        keyboard->selected_w =
            fsemu_widget_new_with_name("fsemu_oskeyboard_selected_key");
        fsemu_widget_add_child(keyboard->widget, keyboard->selected_w);

        fsemu_widget_t *widget = keyboard->selected_w;
        fsemu_widget_set_color(widget, FSEMU_RGBA(0x0000ff80));
        // fsemu_widget_set_top_2(w, -540, FSEMU_WIDGET_SCREEN_BOTTOM);  //
        // FIXME: fsemu_widget_set_right_2(w, 1440 + 240,
        // FSEMU_WIDGET_SCREEN_LEFT); fsemu_widget_set_bottom_2(w, 0,
        // FSEMU_WIDGET_SCREEN_BOTTOM); fsemu_widget_set_left_2(w, 240,
        // FSEMU_WIDGET_SCREEN_LEFT);
        fsemu_widget_set_z_index(widget, FSEMU_LAYER_OSKEYBOARD + 2);
        // FIXME: Not needed for widget?
        fsemu_widget_set_visible(widget, true);
        // FIXME: Add to fsemu_oskeyboard.background_w instead?
        // (or another high-level parent also containing background)
        // fsemu_gui_add_item(widget);
    }

    if (keyboard->selected_key == NULL) {
        keyboard->selected_key =
            fsemu_oskeyboard_find_key_by_index(keyboard, 0, 0);
    }

    fsemu_oskeyboard_update_keyboard_size(keyboard);
    fsemu_oskeyboard_update_keyboard_keys(keyboard);
}

static void fsemu_oskeyboard_init_container(void)
{
    fsemu_oskeyboard.container_w =
        fsemu_widget_new_with_name("fsemu_oskeyboard_container");
    fsemu_widget_t *widget = fsemu_oskeyboard.container_w;

    // Top and bottom coordinates are set afterwards.
    fsemu_widget_set_right_2(widget, 0, FSEMU_WIDGET_SCREEN_RIGHT);
    fsemu_widget_set_left_2(widget, 0, FSEMU_WIDGET_SCREEN_LEFT);
    fsemu_widget_set_z_index(widget, FSEMU_LAYER_OSKEYBOARD);

    // Transparent container widget. Only used to provide position/size and
    // visibility toggling for children.
    fsemu_widget_set_color(widget, FSEMU_RGBA(0x00000000));
    // The on-screen keyboard container determinies whether the entire keyboard
    // is visible. Initially hidden.
    fsemu_widget_set_visible(widget, false);
    // This widget is added to the top level.
    fsemu_gui_add_item(widget);
}

static void fsemu_oskeyboard_init_background(void)
{
    fsemu_oskeyboard.background_w =
        fsemu_widget_new_with_name("fsemu_oskeyboard_background");
    fsemu_widget_t *widget = fsemu_oskeyboard.background_w;

    fsemu_widget_add_child(fsemu_oskeyboard.container_w, widget);
    fsemu_widget_set_color(widget, FSEMU_RGBA(0x0080ff80));
    fsemu_widget_set_top_2(widget, 0, FSEMU_WIDGET_PARENT_TOP);
    fsemu_widget_set_right_2(widget, 0, FSEMU_WIDGET_PARENT_RIGHT);
    fsemu_widget_set_bottom_2(widget, 0, FSEMU_WIDGET_PARENT_BOTTOM);
    fsemu_widget_set_left_2(widget, 0, FSEMU_WIDGET_PARENT_LEFT);
    fsemu_widget_set_z_index(widget, FSEMU_LAYER_OSKEYBOARD);
    // FIXME: Not needed for widget?
    fsemu_widget_set_visible(widget, true);
    // FIXME: Add to another high-level parent also containing background?
    // fsemu_gui_add_item(w);
}

void fsemu_oskeyboard_init(void)
{
    if (fsemu_oskeyboard.initialized) {
        return;
    }
    fsemu_oskeyboard.initialized = true;
    fsemu_oskeyboard_log("Initializing oskeyboard module\n");
    // FIXME: Cleanup function
    fsemu_oskeyboard_init_container();
    // This will update top and bottom coordinates. Positioned at bottom of
    // screen (top=false) by default).
    fsemu_oskeyboard_set_position(false);
    fsemu_oskeyboard_init_background();
#ifdef FSEMU_OSKEYBOARD_START_OPEN
    fsemu_oskeyboard_set_open(true);
#endif
}
