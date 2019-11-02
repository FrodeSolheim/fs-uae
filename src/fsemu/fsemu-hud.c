#define FSEMU_INTERNAL
#include "fsemu-hud.h"

#include "fsemu-glib.h"
#include "fsemu-gui.h"
#include "fsemu-image.h"
#include "fsemu-keyboard.h"
#include "fsemu-thread.h"
#include "fsemu-titlebar.h"
#include "fsemu-types.h"
#include "fsemu-util.h"

#include "fsemu-glib.h"

typedef struct {
    char *icon;
    char *title;
    char *subtitle;
    fsemu_image_t *icon_image;
    fsemu_image_t *title_image;
    fsemu_image_t *subtitle_image;

    fsemu_gui_item_t background_item;
    fsemu_gui_item_t icon_item;
    fsemu_gui_item_t title_item;
    fsemu_gui_item_t subtitle_item;

    bool visible;
    int64_t visible_after;
    int64_t visible_until;
} fsemu_hud_notice_t;

static struct {
    fsemu_font_t *notice_title_font;
    fsemu_font_t *notice_subtitle_font;
    uint32_t notice_background_color;
    uint32_t notice_title_color;
    uint32_t notice_subtitle_color;
    GList *notices;
    fsemu_hud_notice_t quitkey_notice;
    fsemu_hud_notice_t vsync_notice;
    fsemu_hud_notice_t cursor_notice;
    fsemu_hud_notice_t warp_notice;
    fsemu_hud_notice_t pause_notice;
    bool mouse_was_captured;
    fsemu_gui_item_t startup_fade_item;
} fsemu_hud;

static void fsemu_hud_init_notice(fsemu_hud_notice_t *notice)
{
    fsemu_gui_item_t *item;
    item = &notice->background_item;
    fsemu_gui_rectangle(
        item, 20, 0, 440, 120, fsemu_hud.notice_background_color);
    item->coordinates = FSEMU_COORD_1080P_LEFT;
    item->z_index = 5000;
    fsemu_gui_add_item(item);

    item = &notice->icon_item;
    item->coordinates = FSEMU_COORD_1080P_LEFT;
    item->z_index = 5001;
    fsemu_gui_add_item(item);

    item = &notice->title_item;
    item->coordinates = FSEMU_COORD_1080P_LEFT;
    item->z_index = 5001;
    fsemu_gui_add_item(item);

    item = &notice->subtitle_item;
    item->coordinates = FSEMU_COORD_1080P_LEFT;
    item->z_index = 5001;
    fsemu_gui_add_item(item);
}

static void fsemu_hud_update_notice(fsemu_hud_notice_t *notice)
{
    fsemu_gui_item_t *item;
    item = &notice->icon_item;
    if (notice->icon_image == NULL) {
        notice->icon_image = fsemu_image_load(notice->icon);
        fsemu_gui_image(item,
                        20,
                        0,
                        notice->icon_image->width,
                        notice->icon_image->height,
                        notice->icon_image);
    }

    item = &notice->title_item;
    if (notice->title_image == NULL) {
        notice->title_image =
            fsemu_font_render_text_to_image(fsemu_hud.notice_title_font,
                                            notice->title,
                                            fsemu_hud.notice_title_color);
        fsemu_gui_image(item,
                        20,
                        0,
                        notice->title_image->width,
                        notice->title_image->height,
                        notice->title_image);
    }

    item = &notice->subtitle_item;
    if (notice->subtitle_image == NULL) {
        notice->subtitle_image =
            fsemu_font_render_text_to_image(fsemu_hud.notice_subtitle_font,
                                            notice->subtitle,
                                            fsemu_hud.notice_subtitle_color);
        fsemu_gui_image(item,
                        20,
                        0,
                        notice->subtitle_image->width,
                        notice->subtitle_image->height,
                        notice->subtitle_image);
    }
}

static void fsemu_hud_set_notice_visible(fsemu_hud_notice_t *notice,
                                         bool visible)
{
    notice->background_item.visible = visible;
    notice->icon_item.visible = visible;
    notice->title_item.visible = visible;
    notice->subtitle_item.visible = visible;
    notice->visible = visible;
}

static void fsemu_hud_add_notice(fsemu_hud_notice_t *notice)
{
    fsemu_hud.notices = g_list_append(fsemu_hud.notices, notice);
}

static void fsemu_hud_init_and_add_notice(fsemu_hud_notice_t *notice)
{
    fsemu_hud_init_notice(notice);
    fsemu_hud_update_notice(notice);
    fsemu_hud_add_notice(notice);
}

void fsemu_hud_init(void)
{
    fsemu_thread_assert_main();
    fsemu_return_if_already_initialized();

    // fsemu_gui_item_t *item;
    // fsemu_hud_notice_t *notice;

    // FIXME: Medium ?
    fsemu_hud.notice_title_font =
        fsemu_font_load("SairaCondensed-SemiBold.ttf", 32);
    // FIXME: Semi-bold ?
    fsemu_hud.notice_subtitle_font =
        fsemu_font_load("SairaCondensed-Medium.ttf", 28);

    fsemu_hud.notice_background_color = FSEMU_RGBA(0x202020c0);
    fsemu_hud.notice_title_color = FSEMU_RGBA(0xffffffff);
    fsemu_hud.notice_subtitle_color = FSEMU_RGBA(0xffffffaa);

    // int64_t now = fsemu_time_us();

    fsemu_hud.pause_notice.icon = strdup("NotificationPause.png");
    fsemu_hud.pause_notice.title = strdup("Paused");
    fsemu_hud.pause_notice.subtitle =
        g_strdup_printf("%s+P to resume", FSEMU_KEYBOARD_MOD_NAME);
    fsemu_hud_init_and_add_notice(&fsemu_hud.pause_notice);

    fsemu_hud.warp_notice.icon = strdup("NotificationWarp.png");
    fsemu_hud.warp_notice.title = strdup("Fast forwarding");
    fsemu_hud.warp_notice.subtitle = g_strdup_printf(
        "%s+W to resume normal speed", FSEMU_KEYBOARD_MOD_NAME);
    fsemu_hud_init_and_add_notice(&fsemu_hud.warp_notice);

    fsemu_hud.quitkey_notice.icon = strdup("NotificationInfo.png");
    fsemu_hud.quitkey_notice.title =
        g_strdup_printf("Quit key is %s+Q", FSEMU_KEYBOARD_MOD_NAME);
    fsemu_hud.quitkey_notice.subtitle = g_strdup_printf(
        "%s+F1 to view more shortcuts", FSEMU_KEYBOARD_MOD_NAME);
    fsemu_hud_init_and_add_notice(&fsemu_hud.quitkey_notice);
    // fsemu_hud.quitkey_notice.visible_until = now + 10 * 1000 * 1000;

    fsemu_hud.cursor_notice.icon = strdup("NotificationCursor.png");
    fsemu_hud.cursor_notice.title = strdup("Mouse cursor captured");
    fsemu_hud.cursor_notice.subtitle = g_strdup_printf(
        "Middle click or %s+G to release", FSEMU_KEYBOARD_MOD_NAME);
    fsemu_hud_init_and_add_notice(&fsemu_hud.cursor_notice);

    fsemu_hud.vsync_notice.icon = strdup("NotificationWarning.png");
    fsemu_hud.vsync_notice.title = strdup("Vsync disabled");
    fsemu_hud.vsync_notice.subtitle = strdup("Vsync support coming later");
    fsemu_hud_init_and_add_notice(&fsemu_hud.vsync_notice);

    fsemu_gui_rectangle(
        &fsemu_hud.startup_fade_item, 0, 0, 1920, 1080, FSEMU_RGB(0x000000));
    fsemu_gui_item_set_visible(&fsemu_hud.startup_fade_item, true);
    fsemu_hud.startup_fade_item.coordinates = FSEMU_COORD_1080P;
    fsemu_hud.startup_fade_item.z_index = 8999;
    fsemu_gui_add_item(&fsemu_hud.startup_fade_item);
    // fsemu_hud.vsync_notice.visible_until = now + 10 * 1000 * 1000;
}

static void fsemu_hud_set_notice_position(fsemu_hud_notice_t *notice,
                                          int x,
                                          int y)
{
    notice->background_item.rect.x = x;
    notice->background_item.rect.y = y;

    notice->icon_item.rect.x = x + 20;
    notice->icon_item.rect.y = y + 20;

    notice->title_item.rect.x = x + 80 + 20 + 20;
    notice->title_item.rect.y = y + 18;

    notice->subtitle_item.rect.x = x + 80 + 20 + 20;
    notice->subtitle_item.rect.y = y + 58;
}

static void fsemu_hud_update_notice_positions(void)
{
    // FIXME: titlebar height -- not when using custom frame in windowed mode
    // int first_y = 40 + 20;
    int x = 20;
    int y = 20;
    /*
    fsemu_hud_set_notice_position(&fsemu_hud.quitkey_notice, x, y);
    y += 120 + 20;
    fsemu_hud_set_notice_position(&fsemu_hud.cursor_notice, x, y);
    y += 120 + 20;
    fsemu_hud_set_notice_position(&fsemu_hud.vsync_notice, x, y);
    y += 120 + 20;
    */

    GList *list_item = fsemu_hud.notices;
    while (list_item) {
        fsemu_hud_notice_t *notice = (fsemu_hud_notice_t *) list_item->data;
        if (notice->visible) {
            fsemu_hud_set_notice_position(notice, x, y);
            y += 120 + 20;
        }
        list_item = list_item->next;
    }
}

void fsemu_hud_update(void)
{
    static int64_t first_update_at;
    int64_t now = fsemu_time_us();
    if (first_update_at == 0) {
        first_update_at = now;
    }

    if (fsemu_hud.quitkey_notice.visible_until == 0) {
        // Initially show quit key notice after a small delay
        if (now - first_update_at > 2000 * 1000) {
            fsemu_hud.quitkey_notice.visible_until = now + 8000 * 1000;
        }
    }

    if (fsemu_mouse_captured()) {
        // Initially show cursor notice after a small delay
        if (now - first_update_at > 3000 * 1000) {
            if (!fsemu_hud.mouse_was_captured) {
                // FIXME: Shorter delays later
                // FIXME: Only long duration if cursor captured from start
                fsemu_hud.cursor_notice.visible_until = now + 7000 * 1000;
                fsemu_hud.mouse_was_captured = true;
            }
        }
    } else {
        fsemu_hud.cursor_notice.visible_until = 0;
        fsemu_hud.mouse_was_captured = false;
    }

    GList *list_item = fsemu_hud.notices;
    while (list_item) {
        fsemu_hud_notice_t *notice = (fsemu_hud_notice_t *) list_item->data;
        fsemu_hud_set_notice_visible(notice, notice->visible_until > now);
        list_item = list_item->next;
    }

    fsemu_hud_set_notice_visible(&fsemu_hud.pause_notice,
                                 fsemu_control_paused());
    if (!fsemu_control_paused()) {
        fsemu_hud_set_notice_visible(&fsemu_hud.warp_notice,
                                     fsemu_control_warp());
    }
    fsemu_hud_update_notice_positions();

    if (fsemu_hud.startup_fade_item.visible) {
        int64_t dt = now - first_update_at;
        int alpha = 255 * (1.0 - dt / (1.0 * 1000 * 1000));
        printf("alpha %d\n", alpha);
        if (alpha <= 0) {
            fsemu_hud.startup_fade_item.visible = false;
        } else {
            fsemu_hud.startup_fade_item.color = FSEMU_RGB_A(0x000000, alpha);
            // fsemu_hud.startup_fade_item.color = FSEMU_RGBA(0xff000080);
            // fsemu_hud.startup_fade_item.color = FSEMU_RGBA(alpha);
        }
    }
}
