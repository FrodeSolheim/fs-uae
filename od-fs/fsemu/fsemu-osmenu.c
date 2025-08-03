#define FSEMU_INTERNAL
#include "fsemu-osmenu.h"

#include <math.h>

#include "fsemu-control.h"
#include "fsemu-glib.h"
#include "fsemu-hud.h"
#include "fsemu-image.h"
#include "fsemu-layer.h"
#include "fsemu-log.h"
#include "fsemu-mainmenu.h"
#include "fsemu-menu.h"
#include "fsemu-module.h"
#include "fsemu-time.h"
#include "fsemu-util.h"
#include "fsemu-widget.h"

// FIXME: Make sure indivual menus are rendered in reverse order. The top-most
// menu should be rendred first, so OpenGL can avoid rendering many of the
// items of the menus (mostly) hidden below.

// FIXME: A problem with the video layout and offset when the menu is opening
// is that the shadow around the video rectangle is moving before the video
// rectangle itself. This is more easily seen if the menu opens more slowly.

// ----------------------------------------------------------------------------

#define FSEMU_OSMENU_ITEM_FONTSIZE 36
#define FSEMU_OSMENU_HEADING_FONTSIZE 28

#define FSEMU_OSMENU_ITEM_INDENTATION 60
#define FSEMU_OSMENU_HEADING_INDENTATION 40

// #define FSEMU_OSMENU_START_OPEN
#define FSEMU_OSMENU_WIDTH 480
#define FSEMU_OSMENU_SHADOW_WIDTH 20
#define FSEMU_OSMENU_SHADOW_WIDTH_2 12
#define FSEMU_OSMENU_WIDTH_WITH_SHADOW \
    (FSEMU_OSMENU_WIDTH + FSEMU_OSMENU_SHADOW_WIDTH)
#define FSEMU_OSMENU_LEVEL_OFFSET 20

#if 0
#define FSEMU_OSMENU_LEAK_CHECK
#ifdef FSEMU_OSMENU_LEAK_CHECK
#ifndef FSEMU_OSMENU_START_OPEN
#define FSEMU_OSMENU_START_OPEN
#endif
#endif
#endif

// ----------------------------------------------------------------------------

int fsemu_osmenu_log_level = FSEMU_LOG_LEVEL_INFO;

typedef struct {
    fsemu_menu_item_t *item;
    // GList *items;
    fsemu_widget_t *title_w;
} fsemu_osmenu_item_t;

typedef struct {
    // int left;
    // int left_wanted;
    // Position in the menu stack, 0 being the most "main" menu.
    int level;
    fsemu_menu_t *menu;
    fsemu_util_spring_t open_spring;
    GList *ositems;
    // Index of the select item.
    int selected_index;
    fsemu_util_spring_t selected_spring;

    fsemu_widget_t *background_w;
    // Top-level menu has a close/quit button
    fsemu_widget_t *close_w;
    fsemu_widget_t *close_icon_w;
    // Container widget for this particular menu representation. Not to be
    // confused with fsemu_osmenu.container which contains the entire os-screen
    // menu stack, including this container.
    fsemu_widget_t *container_w;
    fsemu_widget_t *selected_w;
    fsemu_widget_t *shadow_w;

} fsemu_osmenu_t;

// ----------------------------------------------------------------------------

static struct {
    fsemu_image_t *drop_shadow;
    bool initialized;
    bool leakcheck;
    bool open;
    int offset;
    int offset_wanted;
    fsemu_util_spring_t open_spring;
    GList *remove_stack;
    GList *stack;
    // The menu will be !open but still visible while it is running the closing
    // animation.
    bool visible;
    // bool was_open;
    struct {
        fsemu_widget_t *container_w;
        fsemu_widget_t *background_w;
    } widgets;
    fsemu_image_t *close_icon;
} fsemu_osmenu;

// ----------------------------------------------------------------------------

static int fsemu_osmenu_item_height(void)
{
    return 60;
}

// After selected_index is changed, this functions needs to be called to update
// the position of the widget indicating the selection.
static void fsemu_osmenu_update_selected_w(fsemu_osmenu_t *osmenu,
                                           bool instant)
{
    // If selected_index is -2, this means no item is selected.
    fsemu_widget_set_visible(osmenu->selected_w, osmenu->selected_index >= -1);
    // We can calculate position even with selected_index -2, no harm.
    int y = 60 + osmenu->selected_index * fsemu_osmenu_item_height();
    // osmenu->selected_y_wanted = y;
    fsemu_util_spring_set_target(&osmenu->selected_spring, y);
    if (instant) {
        fsemu_util_spring_finish(&osmenu->selected_spring);
    }
}

static void fsemu_osmenu_update_osmenu(fsemu_osmenu_t *osmenu)
{
    GList *ositems = osmenu->ositems;
    while (ositems) {
        fsemu_osmenu_item_t *ositem = ositems->data;
        // fsemu_widget_set_text(ositem->title_w,
        //                       fsemu_menu_item_title(ositem->item));

        fsemu_color_t text_color;
        if (fsemu_menu_item_heading(ositem->item)) {
            text_color = FSEMU_COLOR_RGB(0x888888);
        } else if (!fsemu_menu_item_enabled(ositem->item)) {
            text_color = FSEMU_COLOR_RGB(0x666666);
        } else {
            text_color = FSEMU_COLOR_WHITE;
        }
        fsemu_widget_set_text_color(ositem->title_w, text_color);
        fsemu_widget_set_text(ositem->title_w,
                              fsemu_menu_item_title(ositem->item));

        ositems = ositems->next;
    }
}

static void fsemu_osmenu_populate_widgets(fsemu_osmenu_t *osmenu)
{
    fsemu_widget_t *widget;

    widget = osmenu->container_w =
        fsemu_widget_new_with_name("fsemu:osmenu_t:container_w");
    fsemu_widget_add_child(fsemu_osmenu.widgets.container_w, widget);

    // fsemu_widget_set_top(widget, 0);
    // fsemu_widget_set_right(widget, 0);
    // fsemu_widget_set_bottom(widget, 0);
    // fsemu_widget_set_left(widget, 0);
    // fsemu_widget_set_z_index(widget, FSEMU_LAYER_OSMENU);

    // Transparent container widget. Only used to provide position/size and
    // visibility toggling for children.
    fsemu_widget_set_color(widget, FSEMU_COLOR_TRANSPARENT);
    fsemu_widget_set_visible(widget, true);
    // This widget is added to the top level.
    // fsemu_gui_add_item(widget);

    widget = osmenu->shadow_w =
        fsemu_widget_new_with_name("fsemu:osmenu_t:shadow_w");
    fsemu_widget_add_child(osmenu->container_w, widget);
    fsemu_widget_set_left(widget, -FSEMU_OSMENU_SHADOW_WIDTH);
    fsemu_widget_set_right_2(
        widget, FSEMU_OSMENU_SHADOW_WIDTH_2, FSEMU_WIDGET_PARENT_LEFT);
    fsemu_widget_set_image(widget, fsemu_osmenu.drop_shadow, false);
    fsemu_widget_set_visible(widget, true);

    widget = osmenu->background_w =
        fsemu_widget_new_with_name("fsemu:osmenu_t:background_w");
    fsemu_widget_add_child(osmenu->container_w, widget);

    // fsemu_widget_set_top_2(widget, 0, FSEMU_WIDGET_PARENT_TOP);
    // fsemu_widget_set_right_2(widget, 0, FSEMU_WIDGET_PARENT_RIGHT);
    // fsemu_widget_set_bottom_2(widget, 0, FSEMU_WIDGET_PARENT_BOTTOM);
    // fsemu_widget_set_left_2(widget, 0, FSEMU_WIDGET_PARENT_LEFT);
    // fsemu_widget_set_z_index(widget, FSEMU_LAYER_OSMENU);

    fsemu_widget_set_color(widget, FSEMU_COLOR_RGB(0x505050));

    // FIXME: Not needed for widget?
    fsemu_widget_set_visible(widget, true);
    // FIXME: Add to another high-level parent also containing background?
    // fsemu_gui_add_item(w);

    // Create widget for indicating selection in this menu
    widget = osmenu->selected_w =
        fsemu_widget_new_with_name("fsemu:osmenu_t:selected_w");
    fsemu_widget_add_child(osmenu->container_w, widget);
    // fsemu_widget_set_right_2(widget, 0, FSEMU_WIDGET_PARENT_RIGHT);
    // fsemu_widget_set_left_2(widget, 0, FSEMU_WIDGET_PARENT_LEFT);
    fsemu_widget_set_color(widget, FSEMU_COLOR_RGB(0x808080));
    fsemu_widget_set_visible(widget, true);

    // Close button
    if (osmenu->level == 0) {
        widget = osmenu->close_w =
            fsemu_widget_new_with_name("fsemu:osmenu_t:close_w");
        fsemu_widget_set_left_2(widget, -60, FSEMU_WIDGET_PARENT_RIGHT);
        fsemu_widget_set_bottom_2(widget, 60, FSEMU_WIDGET_PARENT_TOP);
        // fsemu_widget_set_color(widget, FSEMU_COLOR_RGB(0xff0000));
        fsemu_widget_add_child(osmenu->container_w, widget);
        fsemu_widget_set_visible(widget, true);
        fsemu_widget_set_image(widget, fsemu_osmenu.close_icon, false);
    }

    int y = 60;
    osmenu->selected_index = -2;

    fsemu_menu_t *menu = osmenu->menu;
    for (int i = 0; i < fsemu_menu_count_items(menu); i++) {
        fsemu_menu_item_t *item = fsemu_menu_get_item(menu, i);
        fsemu_osmenu_log_debug("%d selectable? %d %d\n",
                               i,
                               fsemu_menu_item_selectable(item),
                               fsemu_menu_item_heading(item));
        if (fsemu_menu_item_selectable(item)) {
            if (osmenu->selected_index == -2 ||
                fsemu_menu_item_selected_initially(item)) {
                osmenu->selected_index = i;
            }
        }
        // fsemu_osmenu_log_debug("%p\n", item);
        fsemu_osmenu_item_t *ositem = FSEMU_UTIL_MALLOC0(fsemu_osmenu_item_t);
        ositem->item = item;
        // fsemu_osmenu_log_debug("aaa\n");
        fsemu_menu_item_ref(item);
        // fsemu_osmenu_log_debug("bbb\n");
        osmenu->ositems = g_list_append(osmenu->ositems, ositem);

        widget = ositem->title_w =
            fsemu_widget_new_with_name("osmenu:item:title_w");
        fsemu_widget_add_child(osmenu->container_w, widget);
        fsemu_widget_set_top_2(widget, y, FSEMU_WIDGET_PARENT_TOP);
        // FIXME: AUTO ? (Size to text?)
        fsemu_widget_set_right_2(widget, 0, FSEMU_WIDGET_PARENT_RIGHT);
        // FIXME: AUTO ? (Size to text?)
        fsemu_widget_set_bottom_2(widget, y + 60, FSEMU_WIDGET_PARENT_TOP);
        // fsemu_widget_set_left_2(widget, 0, FSEMU_WIDGET_PARENT_LEFT);
        fsemu_widget_set_color(widget, FSEMU_COLOR_WHITE);
        fsemu_widget_set_visible(widget, true);

        int leftoffset = FSEMU_OSMENU_ITEM_INDENTATION;
        int font_size = FSEMU_OSMENU_ITEM_FONTSIZE;
        // Maybe we only need color? Also, right now, color effectively
        // modulates text_color. Maybe we don't want this...
        fsemu_widget_set_color(widget, FSEMU_COLOR_WHITE);
        if (fsemu_menu_item_heading(item)) {
            font_size = FSEMU_OSMENU_HEADING_FONTSIZE;
            // text_color = FSEMU_COLOR_RGB(0x888888);
            leftoffset = FSEMU_OSMENU_HEADING_INDENTATION;
            fsemu_widget_set_text_valign(widget, 0.9);
        }
        fsemu_widget_set_font_size(widget, font_size);
        fsemu_widget_set_left(widget, leftoffset);
        fsemu_widget_set_text_transform(widget,
                                        FSEMU_WIDGET_TEXT_TRANSFORM_UPPERCASE);

        // FIXME: Or image stretch/centering attributes, i.e.
        // hstrech: 0 vstretch 0 halign 0 valign = center

#if 0
        // Moved to fsemu_menu_update_osmenu
#endif
        y += 60;
    }

    fsemu_osmenu_update_osmenu(osmenu);

    if (osmenu->selected_index == -2) {
        fsemu_warning("No item could be selected\n");
    }
    // And now set the position of the selection widget since selected_index
    // has been determined.
    fsemu_osmenu_update_selected_w(osmenu, true);
}

static void fsemu_osmenu_delete(fsemu_osmenu_t *osmenu)
{
    fsemu_osmenu_log_debug("osmenu delete %p\n", osmenu);

    GList *ositems = osmenu->ositems;
    while (ositems) {
        fsemu_osmenu_item_t *ositem = ositems->data;

        fsemu_osmenu_log_debug("Unrefing title widget\n");
        fsemu_widget_set_visible(ositem->title_w, false);
        // FIXME: Unlink from parent? unref?
        // fsemu_widget_unref(ositem->title_w);

        fsemu_widget_remove_child(osmenu->container_w, ositem->title_w);
        fsemu_widget_unref(ositem->title_w);

        fsemu_osmenu_log_debug("Unrefing menu item\n");
        fsemu_menu_item_unref(ositem->item);
        free(ositem);
        ositems = ositems->next;
    }
    g_list_free(osmenu->ositems);

    fsemu_widget_remove_child(osmenu->container_w, osmenu->background_w);
    fsemu_widget_unref(osmenu->background_w);
    fsemu_widget_remove_child(osmenu->container_w, osmenu->selected_w);
    fsemu_widget_unref(osmenu->selected_w);
    fsemu_widget_remove_child(osmenu->container_w, osmenu->shadow_w);
    fsemu_widget_unref(osmenu->shadow_w);

    fsemu_widget_remove_child(fsemu_osmenu.widgets.container_w,
                              osmenu->container_w);
    fsemu_widget_unref(osmenu->container_w);

    // fsemu_widget_set_visible(osmenu->background_w, false);
    // FIXME: Unlink from parent? unref?
    // fsemu_widget_unref(osmenu->background_w);

    // fsemu_widget_set_visible(osmenu->container_w, false);
    // FIXME: Unlink from parent? unref?
    // fsemu_widget_unref(osmenu->container_w);

    fsemu_osmenu_log_debug("Unrefing menu\n");
    fsemu_menu_unref(osmenu->menu);
    fsemu_osmenu_log_debug("Freeing osmenu\n");
    free(osmenu);
}

// ----------------------------------------------------------------------------

bool fsemu_osmenu_open(void)
{
    return fsemu_osmenu.open;
}

int fsemu_osmenu_open_offset(void)
{
    // We never want to return a positive value. That is just extra offset to
    // account for menu shadows and extra menu levels. The menu offset we want
    // to allow other modules to query for is in the range
    // [-FSEMU_OSMENU_MENU_WIDTH, 0].
    int position = fsemu_osmenu.open_spring.current;
    if (position >= 0) {
        return 0;
    }
    return position;
}

static void fsemu_osmenu_cannot_open(const char *message)
{
    fsemu_hud_show_notification(
        FSEMU_HUD_NOTICATION_ID(
            0x6ff589f5, 0xb480, 0x4aca, 0xa45f, 0x30bb3cac4377),
        "Cannot show menu",
        message,
        FSEMU_HUD_NOTIFICATION_WARNING_ICON,
        FSEMU_HUD_NOTIFICATION_DEFAULT_DURATION);
}

static void fsemu_osmenu_update_left_positions(void)
{
    int max_level = g_list_length(fsemu_osmenu.stack) - 1;
    GList *stack = fsemu_osmenu.stack;
    while (stack) {
        fsemu_osmenu_t *osmenu = (fsemu_osmenu_t *) stack->data;
        int offset = -(max_level - osmenu->level) * FSEMU_OSMENU_LEVEL_OFFSET;

        // osmenu->left_wanted = offset;
        fsemu_util_spring_set_target(&osmenu->open_spring, offset);

        // fsemu_widget_set_left(osmenu->container_w, offset);
        // fsemu_widget_set_right(osmenu->container_w, offset);

        stack = stack->next;
    }
}

// When pushing a new menu, this module should hold a reference to the menu.
static void fsemu_osmenu_push_menu(fsemu_menu_t *menu)
{
    fsemu_assert(menu != NULL);

    fsemu_osmenu_t *osmenu = FSEMU_UTIL_MALLOC0(fsemu_osmenu_t);
    fsemu_util_spring_init(&osmenu->open_spring);
    fsemu_util_spring_set_tension(&osmenu->open_spring, 250.0);
    fsemu_util_spring_init(&osmenu->selected_spring);
    fsemu_util_spring_set_tension(&osmenu->selected_spring, 500.0);

    osmenu->menu = menu;
    osmenu->level = g_list_length(fsemu_osmenu.stack);
    fsemu_osmenu_populate_widgets(osmenu);
    if (fsemu_osmenu.stack == NULL) {
        // osmenu->left = 0
        // osmenu->left_wanted = 0;
        // Initial menu. Will start in correct position, but will be animated
        // by the entire osmenu container.
    } else {
        // osmenu->left = FSEMU_OSMENU_WIDTH_WITH_SHADOW;
        osmenu->open_spring.current = FSEMU_OSMENU_WIDTH_WITH_SHADOW;
        osmenu->open_spring.target = FSEMU_OSMENU_WIDTH_WITH_SHADOW;
    }
    fsemu_osmenu.stack = g_list_append(fsemu_osmenu.stack, osmenu);
    fsemu_osmenu_update_left_positions();
}

// Pop and destroy the top-most menu
static void fsemu_osmenu_pop_menu(void)
{
    fsemu_osmenu_t *osmenu =
        (fsemu_osmenu_t *) g_list_last(fsemu_osmenu.stack)->data;
    fsemu_osmenu.stack = g_list_remove(fsemu_osmenu.stack, osmenu);
    fsemu_osmenu.remove_stack =
        g_list_append(fsemu_osmenu.remove_stack, osmenu);
    // osmenu->left_wanted = FSEMU_OSMENU_WIDTH_WITH_SHADOW;
    fsemu_util_spring_set_target(&osmenu->open_spring,
                                 FSEMU_OSMENU_WIDTH_WITH_SHADOW);
    // fsemu_osmenu_delete(osmenu);
    fsemu_osmenu_update_left_positions();
}

void fsemu_osmenu_set_open(bool open)
{
    if (!fsemu_osmenu.initialized) {
        fsemu_osmenu_cannot_open("Menu module was not initialized");
        return;
    }

    if (open == fsemu_osmenu.open) {
        return;
    }

    if (fsemu_osmenu.stack == NULL) {
        fsemu_menu_t *menu = fsemu_mainmenu_get_menu();
        // This assumes that we didn't acquire the ref from
        // fsemu_mainmenu_get_menu. FIXME: Make sure that's correct.
        fsemu_menu_ref(menu);

        fsemu_osmenu_push_menu(menu);
#if 0
        fsemu_osmenu_t *osmenu = FSEMU_UTIL_MALLOC0(fsemu_osmenu_t);
        osmenu->menu = fsemu_mainmenu_get_menu();
        // This assumes that we didn't acquire the ref from
        // fsemu_mainmenu_get_menu. FIXME: Make sure that's correct.
        fsemu_menu_ref(osmenu->menu);
        fsemu_osmenu_populate_widgets(osmenu);
        fsemu_osmenu.stack = g_list_append(NULL, osmenu);
#endif
    }

    if (fsemu_osmenu.stack == NULL) {
        fsemu_osmenu_cannot_open("No menu defined by emulator");
        return;
    }

    if (open) {
        fsemu_osmenu_log("Open on-screen menu\n");
    } else {
        fsemu_osmenu_log("Close on-screen menu\n");
    }

#if 0
    if (!open) {
        if (fsemu_osmenu.active) {
            fsemu_osmenu_release_keys(fsemu_osmenu.active);
        }
    }
#endif
    fsemu_osmenu.open = open;
    // fsemu_widget_set_visible(fsemu_osmenu.widgets.container_w, open);
    if (open) {
        fsemu_osmenu.visible = true;
        fsemu_widget_set_visible(fsemu_osmenu.widgets.container_w, true);
        // fsemu_osmenu.offset_wanted = -FSEMU_OSMENU_WIDTH;
        fsemu_util_spring_set_target(&fsemu_osmenu.open_spring,
                                     -FSEMU_OSMENU_WIDTH);
    } else {
        // FIXME: Include shadow?
        // fsemu_osmenu.offset_wanted = FSEMU_OSMENU_SHADOW_WIDTH;
        // fsemu_util_spring_set_target(&fsemu_osmenu.open_spring,
        // FSEMU_OSMENU_SHADOW_WIDTH); fsemu_osmenu.offset_wanted = 0;
        // FIXME: The offset isn't correct when multiple menu levels are open.
        // Compensate for that here?

        int max_level = g_list_length(fsemu_osmenu.stack) - 1;
        // fsemu_osmenu.offset_wanted += max_level * FSEMU_OSMENU_LEVEL_OFFSET;
        fsemu_util_spring_set_target(
            &fsemu_osmenu.open_spring,
            FSEMU_OSMENU_SHADOW_WIDTH + max_level * FSEMU_OSMENU_LEVEL_OFFSET);
    }
}

void fsemu_osmenu_toggle_open(void)
{
    fsemu_osmenu_log_debug("fsemu_osmenu_toggle_open\n");
    fsemu_osmenu_set_open(!fsemu_osmenu_open());
}

// ----------------------------------------------------------------------------

static bool fsemu_osmenu_selectable_item(fsemu_osmenu_t *osmenu, int index)
{
    if (osmenu->level == 0 && index == -1) {
        return true;
    }
    return fsemu_menu_item_selectable(
        fsemu_menu_get_item(osmenu->menu, index));
}

static fsemu_osmenu_t *fsemu_osmenu_navigation_menu(void)
{
    if (fsemu_osmenu.stack == NULL) {
        fsemu_warning("Cannot navigate in null menu\n");
        return NULL;
    }
    fsemu_osmenu_t *osmenu =
        (fsemu_osmenu_t *) g_list_last(fsemu_osmenu.stack)->data;
    return osmenu;
}

static void fsemu_osmenu_navigate_up_down(int navigate,
                                          fsemu_action_state_t state)
{
    if (!state) {
        return;
    }
    fsemu_osmenu_t *osmenu = fsemu_osmenu_navigation_menu();
    if (!osmenu) {
        return;
    }
    int min_index = osmenu->level == 0 ? -1 : 0;
    int index = osmenu->selected_index;
    int length = fsemu_menu_count_items(osmenu->menu);
    if (navigate == FSEMU_GUI_NAVIGATE_UP) {
        // Index can be -2 already if no item at all was selected. This can
        // happen if all items are disabled.
        if (index <= min_index) {
            return;
        }
        index -= 1;
        while (index > min_index &&
               !fsemu_osmenu_selectable_item(osmenu, index)) {
            index -= 1;
        }
    } else {
        if (index == length - 1) {
            return;
        }
        index += 1;
        while (index < length - 1 &&
               !fsemu_osmenu_selectable_item(osmenu, index)) {
            index += 1;
        }
    }
    if (index != osmenu->selected_index) {
        if (fsemu_osmenu_selectable_item(osmenu, index)) {
            fsemu_assert(index >= min_index && index < length);
            osmenu->selected_index = index;
            fsemu_osmenu_update_selected_w(osmenu, false);
        }
    }
}

static void fsemu_osmenu_navigate_primary(fsemu_action_state_t state)
{
    if (!state) {
        return;
    }
    fsemu_osmenu_log_debug("Activate\n");
    fsemu_osmenu_t *osmenu = fsemu_osmenu_navigation_menu();
    if (!osmenu) {
        return;
    }

    if (osmenu->selected_index == -1) {
        // Close button
        printf("Close button\n");
        fsemu_control_quit();
        return;
    }

    fsemu_menu_item_t *item =
        fsemu_menu_get_item(osmenu->menu, osmenu->selected_index);
    if (!fsemu_menu_item_enabled(item)) {
        fsemu_warning("Tried to activate a disabled menu item\n");
        return;
    }
    fsemu_menu_t *result = fsemu_menu_item_activate(item);
    // fsemu_osmenu_log_debug("Result: %p\n", result);
    if (FSEMU_MENU_RESULT_IS_MENU(result)) {
        // We now own the reference to this newly created menu. So we must not
        // ref it again here.
        fsemu_osmenu_push_menu(result);
    } else if (result == FSEMU_MENU_RESULT_CLOSE) {
        fsemu_osmenu_set_open(false);
    } else if (result == FSEMU_MENU_RESULT_POP1) {
        fsemu_osmenu_pop_menu();
    } else if (result == FSEMU_MENU_RESULT_POP1_CLOSE) {
        fsemu_osmenu_pop_menu();
        fsemu_osmenu_set_open(false);
    }
}

static void fsemu_osmenu_navigate_back(fsemu_action_state_t state)
{
    if (!state) {
        return;
    }
    fsemu_osmenu_log_debug("Back\n");
    fsemu_osmenu_t *osmenu = fsemu_osmenu_navigation_menu();
    if (!osmenu) {
        return;
    }
    if (g_list_length(fsemu_osmenu.stack) == 1) {
        fsemu_osmenu_log_debug("Only outer menu left, closing menu\n");
        fsemu_osmenu_set_open(false);
        return;
    }
    fsemu_osmenu_pop_menu();
}

static void fsemu_osmenu_navigate_close(fsemu_action_state_t state)
{
    if (!state) {
        return;
    }
    fsemu_osmenu_set_open(false);
}

void fsemu_osmenu_navigate(int navigate, fsemu_action_state_t state)
{
    // FIXME: FSEMU_GUI_NAVIGATE_NONE
    if (navigate == FSEMU_GUI_NAVIGATE_NONE) {
        return;
    }
    // fsemu_osmenu_log_debug("fsemu_osmenu_navigate %d %d\n", navigate,
    // state);

    if (navigate == FSEMU_GUI_NAVIGATE_UP) {
        fsemu_osmenu_navigate_up_down(navigate, state);
    } else if (navigate == FSEMU_GUI_NAVIGATE_RIGHT) {
    } else if (navigate == FSEMU_GUI_NAVIGATE_DOWN) {
        fsemu_osmenu_navigate_up_down(navigate, state);
    } else if (navigate == FSEMU_GUI_NAVIGATE_LEFT) {
    } else if (navigate == FSEMU_GUI_NAVIGATE_PRIMARY) {
        fsemu_osmenu_navigate_primary(state);
    } else if (navigate == FSEMU_GUI_NAVIGATE_SECONDARY) {
        // FSEMU_GUI_NAVIGATE_secondary(keyboard, state);
    } else if (navigate == FSEMU_GUI_NAVIGATE_TERTIARY) {
        // FSEMU_GUI_NAVIGATE_tertiary(keyboard, state);
    } else if (navigate == FSEMU_GUI_NAVIGATE_BACK) {
        fsemu_osmenu_navigate_back(state);
    } else if (navigate == FSEMU_GUI_NAVIGATE_CLOSE) {
        fsemu_osmenu_navigate_close(state);
    } else {
        fsemu_error("Invalid navigation command in %s\n", __func__);
    }
}

static void fsemu_osmenu_update_main_animation(void)
{
#if 1
    int64_t now_us = fsemu_time_us();
    fsemu_util_spring_update_with_time(&fsemu_osmenu.open_spring, now_us);

    int position = fsemu_osmenu.open_spring.current;
    // fsemu_osmenu_log_debug("%d\n", position);
    fsemu_widget_set_left_2(
        fsemu_osmenu.widgets.container_w, position, FSEMU_WIDGET_SCREEN_RIGHT);
    fsemu_widget_set_right_2(fsemu_osmenu.widgets.container_w,
                             position + FSEMU_OSMENU_WIDTH,
                             FSEMU_WIDGET_SCREEN_RIGHT);
    if (!fsemu_osmenu.open && fsemu_osmenu.open_spring.stopped) {
        // Closing animation is done.
        fsemu_osmenu.visible = false;
        fsemu_widget_set_visible(fsemu_osmenu.widgets.container_w, false);
    }

#else
    // FIXME: Crude animation. Can do 40 at a time because the full offset
    // is -480.
    if (fsemu_osmenu.offset < fsemu_osmenu.offset_wanted) {
        fsemu_osmenu.offset += 40;
    } else if (fsemu_osmenu.offset > fsemu_osmenu.offset_wanted) {
        fsemu_osmenu.offset -= 40;
    }
    // fsemu_osmenu_log_debug("%d\n", fsemu_osmenu.offset);
    fsemu_widget_set_left_2(fsemu_osmenu.widgets.container_w,
                            fsemu_osmenu.offset,
                            FSEMU_WIDGET_SCREEN_RIGHT);
    fsemu_widget_set_right_2(fsemu_osmenu.widgets.container_w,
                             fsemu_osmenu.offset + FSEMU_OSMENU_WIDTH,
                             FSEMU_WIDGET_SCREEN_RIGHT);

    if (!fsemu_osmenu.open &&
        fsemu_osmenu.offset >= fsemu_osmenu.offset_wanted) {
        // Closing animation is done.
        fsemu_osmenu.visible = false;
        fsemu_widget_set_visible(fsemu_osmenu.widgets.container_w, false);
    }
#endif
#if 0
#if 1
    static fsemu_util_spring_t spring;
    spring.tension = 170;
    spring.friction = 26;
    spring.mass = 1;
    spring.target = fsemu_osmenu.offset_wanted;

    int64_t now_us = fsemu_time_us();
    fsemu_util_spring_update(&spring, now_us);

    fsemu_widget_set_left_2(fsemu_osmenu.widgets.container_w,
                            spring.current,
                            FSEMU_WIDGET_SCREEN_RIGHT);
    fsemu_widget_set_right_2(fsemu_osmenu.widgets.container_w,
                             spring.current + FSEMU_OSMENU_WIDTH,
                             FSEMU_WIDGET_SCREEN_RIGHT);

#else
    static double position;
    double to = fsemu_osmenu.offset_wanted;
    double tension = 170.0;
    double friction = 26.0;
    double mass = 1.0;
    static double velocity;
    static int64_t last_us;
    int64_t now_us = fsemu_time_us();
    if (now_us - last_us > 30000) {
        last_us = now_us;
    }
    int numsteps = (now_us - last_us) / 1000;
    for (int i = 0; i < numsteps; i++) {
        double force = -tension * (position - to);
        double damping = -friction * velocity;
        double acceleration = (force + damping) / mass;
        velocity = velocity + acceleration / 1000;
        position = position + velocity / 1000;
    }

    last_us = now_us;
    fsemu_osmenu_log_debug("%d %d\n", (int) position, (int) to);

    fsemu_widget_set_left_2(
        fsemu_osmenu.widgets.container_w, position, FSEMU_WIDGET_SCREEN_RIGHT);
    fsemu_widget_set_right_2(fsemu_osmenu.widgets.container_w,
                             position + FSEMU_OSMENU_WIDTH,
                             FSEMU_WIDGET_SCREEN_RIGHT);
#endif
#endif
}

void fsemu_osmenu_update(void)
{
    if (!fsemu_osmenu.open && !fsemu_osmenu.visible) {
        return;
    }

    if (fsemu_osmenu.leakcheck) {
        static int update_counter;
        if (update_counter++ == 10) {
            fsemu_control_quit();
        }
    }

    int64_t now_us = fsemu_time_us();
    // fsemu_osmenu_log_debug("fsemu_osmenu_update\n");

    fsemu_osmenu_update_main_animation();

    // int max_level = g_list_length(fsemu_osmenu.stack) - 1;
    GList *listitem = fsemu_osmenu.stack;
    while (listitem) {
        fsemu_osmenu_t *osmenu = (fsemu_osmenu_t *) listitem->data;
        // The menu entries themselves may have changed, update menu first.
        fsemu_menu_update(osmenu->menu);
        // Then we may have to sync menu changes with on-screen representation.
        fsemu_osmenu_update_osmenu(osmenu);

#if 1
        fsemu_util_spring_update_with_time(&osmenu->open_spring, now_us);
        int position = osmenu->open_spring.current;
        // fsemu_osmenu_log_debug("position %d stopped %d\n", position,
        // osmenu->open_spring.stopped);
        fsemu_widget_set_left(osmenu->container_w, position);
        fsemu_widget_set_right(osmenu->container_w, position);
#else
        // Crude animation
        if (osmenu->left < osmenu->left_wanted) {
            osmenu->left += 20;
        } else if (osmenu->left > osmenu->left_wanted) {
            osmenu->left -= 20;
        }

        fsemu_widget_set_left(osmenu->container_w, osmenu->left);
        fsemu_widget_set_right(osmenu->container_w, osmenu->left);
#endif

        // If selected_index is -2, this means no item is selected.
        fsemu_widget_set_visible(osmenu->selected_w,
                                 osmenu->selected_index >= -1);
        // We can calculate position even with selected_index -2, no harm.
        // int h = fsemu_osmenu_item_height();
        // int y = 60 + osmenu->selected_index * h;
        // osmenu->selected_top_wanted = y;
#if 1
        fsemu_util_spring_update(&osmenu->selected_spring);
        int selected_y = osmenu->selected_spring.current;
        fsemu_widget_set_top(osmenu->selected_w, selected_y);
        fsemu_widget_set_bottom_2(osmenu->selected_w,
                                  selected_y + fsemu_osmenu_item_height(),
                                  FSEMU_WIDGET_PARENT_TOP);

#else
        if (osmenu->selected_y < osmenu->selected_y_wanted) {
            osmenu->selected_y += 20;
        } else if (osmenu->selected_y > osmenu->selected_y_wanted) {
            osmenu->selected_y -= 20;
        }
        fsemu_widget_set_top(osmenu->selected_w, osmenu->selected_y);
        fsemu_widget_set_bottom_2(
            osmenu->selected_w,
            osmenu->selected_y + fsemu_osmenu_item_height(),
            FSEMU_WIDGET_PARENT_TOP);
#endif
        listitem = listitem->next;
    }
    // Now we animate all the osmenus that have been dismissed, and then
    // we delete those that have gone off-screen.
    listitem = fsemu_osmenu.remove_stack;
    while (listitem) {
        fsemu_osmenu_t *osmenu = (fsemu_osmenu_t *) listitem->data;

#if 1
        fsemu_util_spring_update_with_time(&osmenu->open_spring, now_us);
        int position = osmenu->open_spring.current;
        fsemu_widget_set_left(osmenu->container_w, position);
        fsemu_widget_set_right(osmenu->container_w, position);

        if (osmenu->open_spring.stopped) {
            fsemu_osmenu_log_debug("Deleting osmenu\n");
            fsemu_osmenu_delete(osmenu);
            GList *deleteitem = listitem;
            listitem = listitem->next;
            fsemu_osmenu.remove_stack =
                g_list_delete_link(fsemu_osmenu.remove_stack, deleteitem);
            continue;
        }

#else
        // Crude animation
        if (osmenu->left < osmenu->left_wanted) {
            osmenu->left += 20;
        } else if (osmenu->left > osmenu->left_wanted) {
            osmenu->left -= 20;
        }

        if (osmenu->left >= FSEMU_OSMENU_WIDTH_WITH_SHADOW) {
            fsemu_osmenu_log_debug("Deleting osmenu\n");
            fsemu_osmenu_delete(osmenu);
            GList *deleteitem = listitem;
            listitem = listitem->next;
            fsemu_osmenu.remove_stack =
                g_list_delete_link(fsemu_osmenu.remove_stack, deleteitem);
            continue;
        }

        fsemu_widget_set_left(osmenu->container_w, osmenu->left);
        fsemu_widget_set_right(osmenu->container_w, osmenu->left);
#endif

        listitem = listitem->next;
    }
    // fsemu_osmenu_log_debug("fsemu_osmenu_update\n");
}

static void fsemu_osmenu_init_container(void)
{
    fsemu_osmenu.widgets.container_w =
        fsemu_widget_new_with_name("fsemu:osmenu:container_w");
    fsemu_widget_t *widget = fsemu_osmenu.widgets.container_w;

    fsemu_widget_set_top_2(widget, 0, FSEMU_WIDGET_SCREEN_TOP);

    // fsemu_widget_set_right_2(widget, 0, FSEMU_WIDGET_SCREEN_RIGHT);
    // fsemu_widget_set_left_2(
    //     widget, -FSEMU_OSMENU_WIDTH, FSEMU_WIDGET_SCREEN_RIGHT);

    fsemu_widget_set_right_2(widget,
                             FSEMU_OSMENU_SHADOW_WIDTH + FSEMU_OSMENU_WIDTH,
                             FSEMU_WIDGET_SCREEN_RIGHT);
    fsemu_widget_set_left_2(
        widget, FSEMU_OSMENU_SHADOW_WIDTH, FSEMU_WIDGET_SCREEN_RIGHT);
    fsemu_widget_set_bottom_2(widget, 0, FSEMU_WIDGET_SCREEN_BOTTOM);
    fsemu_widget_set_z_index(widget, FSEMU_LAYER_OSMENU);

    // Transparent container widget. Only used to provide position/size and
    // visibility toggling for children.
    fsemu_widget_set_color(widget, FSEMU_COLOR_TRANSPARENT);
    // The on-screen menu container determinies whether the entire menu
    // is visible. Initially hidden.
    fsemu_widget_set_visible(widget, false);
    // This widget is added to the top level.
    fsemu_gui_add_item(widget);
}

static void fsemu_osmenu_init_background(void)
{
    // FIXME: This might not be needed (separate backgrounds per
    // fsemu_osmenu_t)
    fsemu_osmenu.widgets.background_w =
        fsemu_widget_new_with_name("fsemu:osmenu:background_w");
    fsemu_widget_t *widget = fsemu_osmenu.widgets.background_w;

    fsemu_widget_add_child(fsemu_osmenu.widgets.container_w, widget);
    // fsemu_widget_set_top(widget, 0);
    // fsemu_widget_set_right(widget, 0);
    // fsemu_widget_set_bottom(widget, 0);
    // fsemu_widget_set_left(widget, 0);
    fsemu_widget_set_z_index(widget, FSEMU_LAYER_OSMENU);

    fsemu_widget_set_color(widget, FSEMU_RGB(0x808080));

    // FIXME: Not needed for widget?
    fsemu_widget_set_visible(widget, true);
    // FIXME: Add to another high-level parent also containing background?
    // fsemu_gui_add_item(w);
}

#define FSEMU_OSMENU_SHADOW_POW 3.0

static fsemu_image_t *fsemu_osmenu_create_drop_shadow(void)
{
    // FIXME: fsemu_image_new_from_size instead?
    fsemu_image_t *image = fsemu_image_from_size(
        FSEMU_OSMENU_SHADOW_WIDTH + FSEMU_OSMENU_SHADOW_WIDTH_2, 1);
    double dw = FSEMU_OSMENU_SHADOW_WIDTH;
    uint8_t *data = image->data;
    uint8_t *line = data;
    for (int y = 0; y < image->height; y++) {
        uint8_t *pixel = line;
        for (int x = 0; x < image->width; x++) {
            double d = (dw - x) / dw;
            if (d < 0) {
                d = 0;
            }
            double a = pow(1.0 - d, FSEMU_OSMENU_SHADOW_POW);
            pixel[0] = 0x00;  // Red
            pixel[1] = 0x00;  // Green
            pixel[2] = 0x00;  // Blue
            pixel[3] = 255 * a + 0.5;
            pixel += 4;
        }
        line += image->stride;
    }
    return image;
}

static void fsemu_osmenu_quit(void)
{
    fsemu_osmenu_log("fsemu_osmenu_quit\n");
    GList *listitem;

    listitem = fsemu_osmenu.stack;
    while (listitem) {
        fsemu_osmenu_t *osmenu = (fsemu_osmenu_t *) listitem->data;
        fsemu_osmenu_delete(osmenu);
        listitem = listitem->next;
    }
    g_list_free(fsemu_osmenu.stack);

    listitem = fsemu_osmenu.remove_stack;
    while (listitem) {
        fsemu_osmenu_t *osmenu = (fsemu_osmenu_t *) listitem->data;
        fsemu_osmenu_delete(osmenu);
        listitem = listitem->next;
    }
    g_list_free(fsemu_osmenu.remove_stack);

    // fsemu_widget_unref(fsemu_osmenu.widgets.background_w);

    // In theory, this should unref all children?
    // EDIT: Actually, only as long as we have unrefed local references.
    fsemu_widget_unref(fsemu_osmenu.widgets.container_w);
    fsemu_image_unref(fsemu_osmenu.drop_shadow);
}

void fsemu_osmenu_init(void)
{
    if (fsemu_osmenu.initialized) {
        return;
    }
    fsemu_osmenu.initialized = true;
    fsemu_osmenu_log("Initializing osmenu module\n");
    fsemu_module_on_quit(fsemu_osmenu_quit);

    fsemu_osmenu.drop_shadow = fsemu_osmenu_create_drop_shadow();

    // fsemu_osmenu.offset_wanted = FSEMU_OSMENU_SHADOW_WIDTH;
    fsemu_util_spring_init(&fsemu_osmenu.open_spring);
    fsemu_util_spring_set_tension(&fsemu_osmenu.open_spring, 340.0);
    fsemu_osmenu.open_spring.current = FSEMU_OSMENU_SHADOW_WIDTH;
    fsemu_osmenu.open_spring.target = FSEMU_OSMENU_SHADOW_WIDTH;

    fsemu_osmenu_init_container();
    // This will update top and bottom coordinates. Positioned at bottom of
    // screen (top=false) by default).
    // fsemu_osmenu_set_position(false);
    fsemu_osmenu_init_background();
#ifdef FSEMU_OSMENU_START_OPEN
    fsemu_osmenu_set_open(true);
#endif
    if (strcmp(fsemu_getenv("FSEMU_LEAK_CHECK"), "1") == 0) {
        fsemu_osmenu.leakcheck = true;
        fsemu_osmenu_set_open(true);
    }

    // FIXME
    fsemu_osmenu.close_icon = fsemu_image_load("TitleBarCloseIcon.png");
}
