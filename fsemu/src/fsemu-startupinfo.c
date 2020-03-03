#define FSEMU_INTERNAL
#include "fsemu-startupinfo.h"

#include "fsemu-fontcache.h"
#include "fsemu-glib.h"
#include "fsemu-gui.h"
#include "fsemu-option.h"
#include "fsemu-options.h"
#include "fsemu-time.h"
#include "fsemu-video.h"
#include "fsemu-window.h"

// ---------------------------------------------------------------------------

static struct {
    int duration;
    bool done;
    bool enabled;

    fsemu_gui_item_t background;
    fsemu_gui_item_t title_item;
    fsemu_gui_item_t platform_item;
    fsemu_gui_item_t year_item;
    fsemu_gui_item_t companies_item;
    fsemu_gui_item_t emulator_item;
    fsemu_gui_item_t fork_item;
    fsemu_gui_item_t fork_2_item;
    fsemu_gui_item_t cover_item;
    // int height;
    // int width;
    char *emulator_name;
    char *fork_info;
} fsemu_startupinfo;

void fsemu_startupinfo_set_emulator_name(const char *emulator_name)
{
    if (fsemu_startupinfo.emulator_name) {
        free(fsemu_startupinfo.emulator_name);
    }
    fsemu_startupinfo.emulator_name = strdup(emulator_name);
}

void fsemu_startupinfo_set_emulator_fork_info(const char *fork_info)
{
    if (fsemu_startupinfo.fork_info) {
        free(fsemu_startupinfo.fork_info);
    }
    fsemu_startupinfo.fork_info = strdup(fork_info);
}

// ---------------------------------------------------------------------------

void fsemu_startupinfo_init(void)
{
    fsemu_return_if_already_initialized();
    fsemu_font_init();
    fsemu_gui_init();
    fsemu_window_init();

    fsemu_startupinfo.duration = 10 * 1000 * 1000;

    int height = 180;
    int y = 1080 - 180;

    fsemu_gui_item_t *item;
    item = &fsemu_startupinfo.background;
    // fsemu_gui_rectangle(item, 0, y, 1920, height, FSEMU_RGBA(0x000000c0));
    fsemu_gui_rectangle(item, 0, y, 1920, height, FSEMU_RGBA(0x101010c0));
    item->coordinates = FSEMU_COORD_1080P;
    item->z_index = 5000;
    fsemu_gui_add_item(item);

    // FIXME: May not need to cache these. Maybe use and throw away instead?
    // Need to fint which font/sizes are reused throughout the app.
    fsemu_font_t *font = fsemu_fontcache_font("SairaCondensed-Bold.ttf", 56);
    fsemu_font_t *font_2 = fsemu_fontcache_font("SairaCondensed-Bold.ttf", 32);
    fsemu_font_t *font_3 = fsemu_fontcache_font("SairaCondensed-Bold.ttf", 24);
    fsemu_font_t *font_4 =
        fsemu_fontcache_font("SairaCondensed-Regular.ttf", 24);

    int x = 240;
    const char *string;
    fsemu_image_t *image;
    uint32_t white = FSEMU_RGB(0xffffff);
    uint32_t white_2 = FSEMU_RGBA(0xffffffd0);
    uint32_t white_3 = FSEMU_RGBA(0xffffffc0);

    if (fsemu_option_read_const_string(FSEMU_OPTION_GAME_COVER, &string)) {
        item = &fsemu_startupinfo.cover_item;
        fsemu_startupinfo_log("Loading %s\n", string);
        // printf("aaa\n");
        image = fsemu_image_load_png_file(string);
        if (image) {
            fsemu_startupinfo_log(
                "Image: %dx%d\n", image->width, image->height);
            // printf("bbb\n");
            int image_height = height - 50;
            int image_width = image_height * image->width / image->height;

            fsemu_gui_image(item,
                            x - image_width - 25,
                            y + 25,
                            image_width,
                            image_height,
                            image);
            // printf("ccc\n");
            item->coordinates = FSEMU_COORD_1080P_LEFT;
            item->z_index = 5001;
            fsemu_gui_add_item(item);
        }
    }

    if (fsemu_option_read_const_string(FSEMU_OPTION_GAME_NAME, &string)) {
        item = &fsemu_startupinfo.title_item;
        image = fsemu_font_render_text_to_image(font, string, white);
        fsemu_gui_image(
            item, x + 25, y + 25 - 2, image->width, image->height, image);
        item->coordinates = FSEMU_COORD_1080P_LEFT;
        item->z_index = 5001;
        fsemu_gui_add_item(item);
    }

    // FIXME: Consider joining platform, year, companies into one
    // string / image for more efficient rendering.

    int x2 = x + 25 - 1;

    if (fsemu_option_read_const_string(FSEMU_OPTION_GAME_YEAR, &string)) {
        item = &fsemu_startupinfo.year_item;
        image = fsemu_font_render_text_to_image(font_2, string, white);
        fsemu_gui_image(item, x2, y + 93, image->width, image->height, image);
        item->coordinates = FSEMU_COORD_1080P_LEFT;
        item->z_index = 5001;
        fsemu_gui_add_item(item);

        x2 += image->width + 25;
    }

    if (fsemu_option_read_const_string(FSEMU_OPTION_GAME_PLATFORM, &string)) {
        item = &fsemu_startupinfo.platform_item;
        gchar *string_2 = g_utf8_strup(string, -1);
        image = fsemu_font_render_text_to_image(font_2, string_2, white);
        g_free(string_2);
        fsemu_gui_image(item, x2, y + 93, image->width, image->height, image);
        item->coordinates = FSEMU_COORD_1080P_LEFT;
        item->z_index = 5001;
        fsemu_gui_add_item(item);

        x2 += image->width + 50;
    }

    if (fsemu_option_read_const_string(FSEMU_OPTION_GAME_COMPANIES, &string)) {
        item = &fsemu_startupinfo.companies_item;
        gchar *string_2 = g_utf8_strup(string, -1);
        image = fsemu_font_render_text_to_image(font_2, string_2, white);
        g_free(string_2);
        fsemu_gui_image(item, x2, y + 93, image->width, image->height, image);
        item->coordinates = FSEMU_COORD_1080P_LEFT;
        item->z_index = 5001;
        fsemu_gui_add_item(item);
    }

    if ((string = fsemu_startupinfo.emulator_name)) {
        item = &fsemu_startupinfo.emulator_item;
        gchar *string_2 = g_utf8_strup(string, -1);
        // gchar *string_3 = g_strdup_printf(
        //     "%s%s EMULATOR", string_2, fsemu_startupinfo.fork_info ? "*" :
        //     "");
        gchar *string_3 = g_strdup_printf("%s", string_2);
        g_free(string_2);
        image = fsemu_font_render_text_to_image(font_3, string_3, white_2);
        g_free(string_3);
        fsemu_gui_image(item,
                        1920 - 49 - image->width,
                        y + 39,
                        image->width,
                        image->height,
                        image);
        item->coordinates = FSEMU_COORD_1080P_RIGHT;
        item->z_index = 5001;
        fsemu_gui_add_item(item);
    }

    if ((string = fsemu_startupinfo.fork_info)) {
        item = &fsemu_startupinfo.fork_item;
        // gchar *string_2 =
        //     g_strdup_printf("* Fork of %s with modifications", string);
        gchar *string_2 =
            g_strdup_printf("This is a modified version of %s", string);
        image = fsemu_font_render_text_to_image(font_4, string_2, white_3);
        g_free(string_2);
        fsemu_gui_image(item,
                        1920 - 49 - image->width,
                        y + 76,
                        image->width,
                        image->height,
                        image);
        item->coordinates = FSEMU_COORD_1080P_RIGHT;
        item->z_index = 5001;
        fsemu_gui_add_item(item);

        item = &fsemu_startupinfo.fork_2_item;
        image = fsemu_font_render_text_to_image(
            font_4,
            "Don't send bug reports upstream without testing original",
            white_3);
        fsemu_gui_image(item,
                        1920 - 49 - image->width,
                        y + 102,
                        image->width,
                        image->height,
                        image);
        item->coordinates = FSEMU_COORD_1080P_RIGHT;
        item->z_index = 5001;
        fsemu_gui_add_item(item);
    }
}

static void fsemu_startupinfo_set_visible(bool visible)
{
#if 1
    // FIXME: Temporarily disabled
    visible = false;
#endif

    fsemu_gui_item_set_visible(&fsemu_startupinfo.background, visible);
    fsemu_gui_item_set_visible(&fsemu_startupinfo.cover_item, visible);
    fsemu_gui_item_set_visible(&fsemu_startupinfo.title_item, visible);
    fsemu_gui_item_set_visible(&fsemu_startupinfo.platform_item, visible);
    fsemu_gui_item_set_visible(&fsemu_startupinfo.year_item, visible);
    fsemu_gui_item_set_visible(&fsemu_startupinfo.companies_item, visible);
    fsemu_gui_item_set_visible(&fsemu_startupinfo.emulator_item, visible);
    fsemu_gui_item_set_visible(&fsemu_startupinfo.fork_item, visible);
    fsemu_gui_item_set_visible(&fsemu_startupinfo.fork_2_item, visible);
}

// ---------------------------------------------------------------------------
/*
static void fsemu_startupinfo_remove(void)
{
    fsemu_startupinfo_set_visible(false);
    fsemu_startupinfo.done = true;
}
*/
// ---------------------------------------------------------------------------

static bool fsemu_startupinfo_visible(void)
{
    return !fsemu_startupinfo.done || fsemu_startupinfo.enabled;
}

void fsemu_startupinfo_update(void)
{
    /*
    if (fsemu_startupinfo.done) {
        return;
    }
    */

    if (!fsemu_startupinfo.done) {
        // FIXME: Use less exact time set once per frame to avoid cost?
        int64_t now = fsemu_time_us();

        static int64_t initial_time;
        if (initial_time == 0) {
            initial_time = now;
        }

        if (now > initial_time + fsemu_startupinfo.duration) {
            fsemu_startupinfo.done = true;
        }
    }

    fsemu_startupinfo_set_visible(fsemu_startupinfo_visible());
}

void fsemu_startupinfo_toggle(void)
{
    if (!fsemu_startupinfo.done) {
        fsemu_startupinfo.done = true;
        return;
    }

    fsemu_startupinfo.enabled = !fsemu_startupinfo.enabled;
}
