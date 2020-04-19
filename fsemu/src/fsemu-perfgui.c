#define FSEMU_INTERNAL
#include "fsemu-perfgui.h"

#include <stdint.h>

#include "fsemu-audio.h"
#include "fsemu-frame.h"
#include "fsemu-gui.h"
#include "fsemu-image.h"
#include "fsemu-option.h"
#include "fsemu-options.h"
#include "fsemu-video.h"
#include "fsemu-widget.h"

static struct {
    int mode;
    int color_set;
    bool refresh;
    struct {
        uint32_t audio_avg;
        uint32_t audio_min;
        uint32_t audio_target;
        uint32_t audio_0;
        uint32_t audio_1;
        uint32_t audio_2;
        uint32_t audio_3;
        uint32_t audio_bg;
        uint32_t line;
        uint32_t video_actual;
        uint32_t video_target;
        uint32_t video_vsync_at;
        uint32_t video_rendered_at;
        uint32_t video_overshoot;
        uint32_t video_wait;
        uint32_t video_emu;
        uint32_t video_gui;
        uint32_t video_render;
        uint32_t video_extra;
        uint32_t video_sleep;
        uint32_t video_other;
        uint32_t video_bg;
    } colors;

    fsemu_image_t audio_image;
    fsemu_image_t video_image;

    fsemu_gui_item_t audio_item;
    fsemu_gui_item_t video_item;
#if 0
    // fsemu_gui_item_t vsync_test_item;
    fsemu_gui_item_t vsync_test_item_bg;
    fsemu_gui_item_t vsync_test_item_a;
    fsemu_gui_item_t vsync_test_item_b;
#endif
} fsemu_perfgui;

static void fsemu_perfgui_init_images(void)
{
    fsemu_perfgui.audio_image.data = (uint8_t *) malloc(4 * 128 * 512);
    fsemu_perfgui.audio_image.width = 128;
    fsemu_perfgui.audio_image.height = 512;
    fsemu_perfgui.audio_image.depth = 32;
    fsemu_perfgui.audio_image.stride = 4 * 128;

    fsemu_perfgui.video_image.data = (uint8_t *) malloc(4 * 128 * 512);
    fsemu_perfgui.video_image.width = 128;
    fsemu_perfgui.video_image.height = 512;
    fsemu_perfgui.video_image.depth = 32;
    fsemu_perfgui.video_image.stride = 4 * 128;

    uint32_t *row = (uint32_t *) malloc(4 * 128);
    for (int x = 0; x < 128; x++) {
#if 1
        row[x] = FSEMU_RGBA(0x00000000);
#else
        if (x >= 116) {
            row[x] = FSEMU_RGBA(0x0c0c0c00);
        } else {
            row[x] = FSEMU_RGB(0x0c0c0c);
        }
#endif
    }

    uint8_t *dst_a = fsemu_perfgui.audio_image.data;
    uint8_t *dst_v = fsemu_perfgui.video_image.data;
    for (int y = 0; y < 512; y++) {
        memcpy(dst_a, row, 4 * 128);
        memcpy(dst_v, row, 4 * 128);
        dst_a += 4 * 128;
        dst_v += 4 * 128;
    }

    free(row);
}

static void fsemu_perfgui_init_items(void)
{
    fsemu_gui_item_t *item;
    item = &fsemu_perfgui.audio_item;
    fsemu_gui_image(item,
                    1920 - 256,
                    0,  // 56,
                    256,
                    1080,  // 1024,
                    &fsemu_perfgui.audio_image);
    item->z_index = -9500;
    item->visible = true;
    // fsemu_gui_item_set_visible(item, true);
    fsemu_gui_add_item(item);

    item = &fsemu_perfgui.video_item;
    fsemu_gui_image(item,
                    0,
                    0,  // 56,
                    256,
                    1080,  // 1024,
                    &fsemu_perfgui.video_image);
    item->z_index = -9500;
    item->visible = true;
    // fsemu_gui_item_set_visible(item, true);
    fsemu_gui_add_item(item);

    fsemu_perfgui.audio_item.flags = FSEMU_WIDGET_FLAG_FLIPX;
    fsemu_perfgui.audio_item.coordinates = FSEMU_COORD_1080P_RIGHT;
    fsemu_perfgui.video_item.coordinates = FSEMU_COORD_1080P_LEFT;

#if 0
    fsemu_font_t *font = fsemu_font_load("SairaCondensed-Bold.ttf", 40);
    fsemu_image_t *image;

    item = &fsemu_perfgui.vsync_test_item_a;
    image =
        fsemu_font_render_text_to_image(font, "V-SYNC", FSEMU_RGB(0xff8080));
    fsemu_gui_image(item, 10, 210, image->width, image->height, image);
    item->z_index = -9300;
    // item->visible = true;
    fsemu_gui_add_item(item);

    item = &fsemu_perfgui.vsync_test_item_b;
    image =
        fsemu_font_render_text_to_image(font, "V-SYNC", FSEMU_RGB(0x80ffff));
    fsemu_gui_image(item, 10, 210, image->width, image->height, image);
    item->z_index = -9300;
    // item->visible = true;
    fsemu_gui_add_item(item);

    item = &fsemu_perfgui.vsync_test_item_bg;
    fsemu_gui_rectangle(item,
                        0,
                        200,
                        image->width + 20,
                        image->height + 20,
                        FSEMU_RGB(0xe0e0e0));
    item->z_index = -9400;
    item->visible = true;
    fsemu_gui_add_item(item);
#endif
}

static void fsemu_perfgui_set_colors(int color_set)
{
    if (fsemu_perfgui.color_set == color_set) {
        return;
    }
    if (color_set == 1) {
        // Dim colors
#if 1
        fsemu_perfgui.colors.audio_avg = FSEMU_RGBA(0xffffff30);
        fsemu_perfgui.colors.audio_min = FSEMU_RGBA(0xffffff30);
        fsemu_perfgui.colors.audio_target = FSEMU_RGBA(0xffffff20);
        fsemu_perfgui.colors.audio_0 = FSEMU_RGBA(0xffffff28);
        fsemu_perfgui.colors.audio_1 = FSEMU_RGBA(0xffffff14);
        fsemu_perfgui.colors.audio_2 = FSEMU_RGBA(0xffffff14);
        fsemu_perfgui.colors.audio_3 = FSEMU_RGBA(0xffffff1c);

        fsemu_perfgui.colors.video_actual = FSEMU_RGBA(0xffffff30);
        fsemu_perfgui.colors.video_target = FSEMU_RGBA(0xffffff20);
        fsemu_perfgui.colors.video_vsync_at = FSEMU_RGBA(0xffffff50);
        fsemu_perfgui.colors.video_rendered_at = FSEMU_RGBA(0xffffff30);

        fsemu_perfgui.colors.video_overshoot = FSEMU_RGBA(0xffffff0c);
        fsemu_perfgui.colors.video_wait = FSEMU_RGBA(0xffffff14);
        fsemu_perfgui.colors.video_gui = FSEMU_RGBA(0xffffff10);
        fsemu_perfgui.colors.video_emu = FSEMU_RGBA(0xffffff20);
        fsemu_perfgui.colors.video_render = FSEMU_RGBA(0xffffff10);
        fsemu_perfgui.colors.video_extra = FSEMU_RGBA(0xffffff1c);
        fsemu_perfgui.colors.video_sleep = FSEMU_RGBA(0xffffff14);
        fsemu_perfgui.colors.video_other = FSEMU_RGBA(0xffffff1c);

        fsemu_perfgui.colors.line = FSEMU_RGBA(0xffffff80);
        fsemu_perfgui.colors.audio_bg = FSEMU_RGBA(0x00000000);
        fsemu_perfgui.colors.video_bg = FSEMU_RGBA(0x00000000);
#else
        fsemu_perfgui.colors.audio_avg = FSEMU_RGB(0x303030);
        fsemu_perfgui.colors.audio_min = FSEMU_RGB(0x303030);
        fsemu_perfgui.colors.audio_target = FSEMU_RGB(0x202020);
        fsemu_perfgui.colors.audio_0 = FSEMU_RGB(0x282828);
        fsemu_perfgui.colors.audio_1 = FSEMU_RGB(0x141414);
        fsemu_perfgui.colors.audio_2 = FSEMU_RGB(0x141414);
        fsemu_perfgui.colors.audio_3 = FSEMU_RGB(0x1c1c1c);
        fsemu_perfgui.colors.video_actual = FSEMU_RGB(0x303030);
        fsemu_perfgui.colors.video_target = FSEMU_RGB(0x202020);
        fsemu_perfgui.colors.video_vsync_at = FSEMU_RGB(0x505050);
        fsemu_perfgui.colors.video_rendered_at = FSEMU_RGB(0x303030);
        fsemu_perfgui.colors.video_overshoot = FSEMU_RGB(0x0c0c0c);
        fsemu_perfgui.colors.video_wait = FSEMU_RGB(0x141414);
        fsemu_perfgui.colors.video_gui = FSEMU_RGB(0x000000);
        fsemu_perfgui.colors.video_emu = FSEMU_RGB(0x202020);
        fsemu_perfgui.colors.video_render = FSEMU_RGB(0x000000);
        fsemu_perfgui.colors.video_extra = FSEMU_RGB(0x1c1c1c);
        fsemu_perfgui.colors.video_sleep = FSEMU_RGB(0x141414);
        fsemu_perfgui.colors.video_other = FSEMU_RGB(0x1c1c1c);

        fsemu_perfgui.colors.line = FSEMU_RGB(0x404040);
        fsemu_perfgui.colors.audio_bg = FSEMU_RGB(0x0c0c0c);
        fsemu_perfgui.colors.video_bg = FSEMU_RGB(0x0c0c0c);
#endif
    } else {
        // Bright colors used for debugging
        fsemu_perfgui.colors.audio_avg = FSEMU_RGB(0x00ff00);
        fsemu_perfgui.colors.audio_min = FSEMU_RGB(0x0099ff);
        fsemu_perfgui.colors.audio_target = FSEMU_RGB(0xee8800);
        fsemu_perfgui.colors.audio_0 = FSEMU_RGB(0x505050);
        fsemu_perfgui.colors.audio_1 = FSEMU_RGB(0x383838);
        fsemu_perfgui.colors.audio_2 = FSEMU_RGB(0x282828);
        fsemu_perfgui.colors.audio_3 = FSEMU_RGB(0x181818);
        fsemu_perfgui.colors.line = FSEMU_RGB(0x0099ff);
        fsemu_perfgui.colors.video_actual = FSEMU_RGB(0x00ff00);
        fsemu_perfgui.colors.video_target = FSEMU_RGB(0xee8800);
        fsemu_perfgui.colors.video_vsync_at = FSEMU_RGB(0xff00ff);
        fsemu_perfgui.colors.video_rendered_at = FSEMU_RGB(0x0099ff);
        fsemu_perfgui.colors.video_overshoot = FSEMU_RGB(0x282888);
        fsemu_perfgui.colors.video_wait = FSEMU_RGB(0x202020);
        fsemu_perfgui.colors.video_gui = FSEMU_RGB(0xffff44);
        fsemu_perfgui.colors.video_emu = FSEMU_RGB(0x288828);
        fsemu_perfgui.colors.video_render = FSEMU_RGB(0x440044);
        fsemu_perfgui.colors.video_extra = FSEMU_RGB(0x186818);
        fsemu_perfgui.colors.video_sleep = FSEMU_RGB(0x282828);
        fsemu_perfgui.colors.video_other = FSEMU_RGB(0x444444);

        // fsemu_perfgui.colors.line = FSEMU_RGB(0x606060);
        fsemu_perfgui.colors.audio_bg = FSEMU_RGB(0x0c0c0c);
        fsemu_perfgui.colors.video_bg = FSEMU_RGB(0x0c0c0c);
    }

    fsemu_perfgui.color_set = color_set;
    fsemu_perfgui.refresh = true;
}

void fsemu_perfgui_init(void)
{
    fsemu_return_if_already_initialized();

    fsemu_gui_init();

    fsemu_log("Initializing perfgui\n");
    fsemu_perfgui_init_images();
    fsemu_perfgui_init_items();

    // Default to 0 for now
    fsemu_perfgui.mode = 0;

    fsemu_option_read_int(FSEMU_OPTION_PERFORMANCE_GUI, &fsemu_perfgui.mode);
    if (fsemu_perfgui.mode < 0 || fsemu_perfgui.mode > 2) {
        fsemu_log("WARNING: Invalid valid for " FSEMU_OPTION_PERFORMANCE_GUI
                  "\n");
        fsemu_perfgui.mode = 0;
    }

    if (fsemu_perfgui.mode) {
        fsemu_perfgui_set_colors(fsemu_perfgui.mode);
    }
}

static void fsemu_perfgui_draw_line(uint8_t *row)
{
    if (fsemu_perfgui.color_set == 1) {
#if 1
        for (int x = 0; x < 116; x++) {
            // ((uint32_t *) row)[x] = FSEMU_RGB(0x000000);
            ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_bg;
        }
#else
        uint8_t *p = row + 116 * 4 - 1;
        uint8_t c = 0x0c;
        for (int x = 0; x < 116; x++) {
            *p-- = 255;
            *p-- = c;
            *p-- = c;
            *p-- = c;
            c += 1;
        }
#endif
    } else {
        for (int x = 0; x < 116; x++) {
            ((uint32_t *) row)[x] = fsemu_perfgui.colors.line;
        }
    }
}

static void fsemu_perfgui_update_audio(int frame)
{
    fsemu_audio_frame_stats_t stats;
    fsemu_audio_frame_stats(frame, &stats);

    int target_us = stats.target_latency_us;
    int scale = 2 * target_us / 1000;
    if (scale == 0) {
        printf("WARNING: fsemu_perfgui_update_audio: scale is 0\n");
        scale = 40;
    }

    int level1, level2, level3, target, avg, min_level;

    if (fsemu_audio_frequency()) {
        // (128 px = 40 ms)
        // int us = fsemu_audio_bytes_to_us(stats.buffer_bytes -
        // stats.recent_bytes);

        int us = fsemu_audio_bytes_to_us(stats.recent_bytes);
        level1 = us * 128 / (scale * 1000);

        us = fsemu_audio_bytes_to_us(stats.buffer_bytes);
        level2 = us * 128 / (scale * 1000);

        us += fsemu_audio_bytes_to_us(stats.inflight_bytes);
        level3 = us * 128 / (scale * 1000);

        us = fsemu_audio_bytes_to_us(stats.min_buffer_bytes);
        min_level = us * 128 / (scale * 1000);

        target = target_us * 128 / (scale * 1000);
        avg = stats.avg_latency_us * 128 / (scale * 1000);

        // FIXME: Check mednafen - level3 < level2 (inflight bytes negative)
        // Seems to happen if other programs have PulseAudio connections
        // (browsers?)

        // int buffer_fill = fsemu_audio_buffer_fill_us();
        // printf("levels %d %d %d\n", level1, level2, level3);

        // int us = fsemu_audio_bytes_to_us(stats.buffer_bytes +
        // stats.inflight_bytes); int level0 = us * 128 / (scale * 1000);
    } else {
        level1 = -1;
        level2 = -1;
        level3 = -1;
        target = -1;
        avg = -1;
        min_level = -1;
    }

    uint32_t level1_color = fsemu_perfgui.colors.audio_1;
#if 0
    if (stats.underruns) {
        level1_color = FSEMU_RGB(0xff0000);
        level1 = level1 < 2 ? 2 : level1;
    }
#endif

    int y = 2 * (frame % 256);
    uint8_t *row =
        fsemu_perfgui.audio_image.data + y * fsemu_perfgui.audio_image.stride;

    for (int i = 0; i < 2; i++) {
        for (int x = 0; x < 116; x++) {
            if (x == avg) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.audio_avg;
            } else if (x == min_level) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.audio_min;
            } else if (x == target) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.audio_target;
            } else if (x == level3) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.audio_0;
            } else if (x < level1) {
                ((uint32_t *) row)[x] = level1_color;
            } else if (x < level2) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.audio_2;
            } else if (x < level3) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.audio_3;
            } else {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.audio_bg;
            }
        }
        if (stats.underruns) {
            ((uint32_t *) row)[0] = FSEMU_RGB(0xff0000);
            ((uint32_t *) row)[1] = FSEMU_RGB(0xff0000);
            ((uint32_t *) row)[2] = FSEMU_RGB(0xff0000);
            ((uint32_t *) row)[3] = FSEMU_RGB(0xff0000);
        }
        row += fsemu_perfgui.audio_image.stride;
        y += 1;
    }

    if (y == 512) {
        y = 0;
    }
    fsemu_perfgui_draw_line(fsemu_perfgui.audio_image.data +
                            y * fsemu_perfgui.audio_image.stride);
}

static void fsemu_perfgui_update_video(int frame)
{
    fsemu_video_frame_stats_t stats;
    fsemu_video_frame_stats(frame, &stats);

    // int scale = 1000 / stats.frame_hz;
    int target_us = 1000000 / stats.frame_hz;
    int scale_us = 2 * 1000000 / stats.frame_hz;

    // printf("get frame stats for frame %d\n", frame);
    // printf("%lld %lld\n", (long long) stats.rendered_at, (long long)
    // stats.origin_at); printf("levels %d (frame %d)\n", stats.buffer_bytes,
    // frame);
#if 0
    printf("levels sleep %d emu %d (frame %d)\n",
           stats.sleep_us,
           stats.emu_us,
           frame);
#endif
    // (128 px = 40 ms)

    int us = 0;

    us += stats.overshoot_us;
    int overshoot_level = us * 128 / scale_us;

    us += stats.wait_us;
    int wait_level = us * 128 / scale_us;

    us += stats.gui_us;
    int gui_level = us * 128 / scale_us;

    us += stats.emu_us;
    int emu_level = us * 128 / scale_us;

    us += stats.render_us;
    int render_level = us * 128 / scale_us;

    us += stats.extra_us;
    int extra_level = us * 128 / scale_us;

    us += stats.sleep_us;
    int sleep_level = us * 128 / scale_us;

    us += stats.other_us;
    int other_level = us * 128 / scale_us;
    int actual = us * 128 / scale_us;

    // FIXME: If vsync, maybe set actual to swapped?

    int target;
    if (stats.frame_warp) {
        target = -1;
    } else {
        target = target_us * 128 / scale_us;
    }

    // printf("%lld %lld %lld\n", (int64_t) stats.began_at, (int64_t)
    // stats.rendered_at, (int64_t) stats.swapped_at);
    int rendered = (stats.rendered_at - stats.origin_at) * 128 / scale_us;
    int swapped = (stats.swapped_at - stats.origin_at) * 128 / scale_us;

    int y = 2 * (frame % 256);
    uint8_t *row =
        fsemu_perfgui.video_image.data + y * fsemu_perfgui.video_image.stride;

    // FIXME: improve this check, also make dependent on hz

    // FIXME: Underrun - swap too late
    bool problem = (stats.swapped_at - stats.origin_at) > 30000;

    // FIXME: Overrun - too many emulated frames
    // printf("Origin at   %lld\n", (long long) stats.origin_at);
    // printf("Rendered at %lld\n", (long long) stats.rendered_at);
    bool problem_2 = (stats.rendered_at < stats.origin_at);

    for (int i = 0; i < 2; i++) {
        for (int x = 0; x < 116; x++) {
            if (x == swapped) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_vsync_at;
            } else if (x == rendered) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_rendered_at;
            } else if (x == actual) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_actual;
            } else if (x == target) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_target;
            } else if (x < overshoot_level) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_overshoot;
            } else if (x < wait_level) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_wait;
            } else if (x < gui_level) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_gui;
            } else if (x < emu_level) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_emu;
            } else if (x < render_level) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_render;
            } else if (x < extra_level) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_extra;
            } else if (x < sleep_level) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_sleep;
            } else if (x < other_level) {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_other;
            } else {
                ((uint32_t *) row)[x] = fsemu_perfgui.colors.video_bg;
            }
        }
        if (problem) {
            ((uint32_t *) row)[0] = FSEMU_RGB(0xff0000);
            ((uint32_t *) row)[1] = FSEMU_RGB(0xff0000);
            ((uint32_t *) row)[2] = FSEMU_RGB(0xff0000);
            ((uint32_t *) row)[3] = FSEMU_RGB(0xff0000);
        } else if (problem_2) {
            ((uint32_t *) row)[0] = FSEMU_RGB(0xff00ff);
            ((uint32_t *) row)[1] = FSEMU_RGB(0xff00ff);
            ((uint32_t *) row)[2] = FSEMU_RGB(0xff00ff);
            ((uint32_t *) row)[3] = FSEMU_RGB(0xff00ff);
        }
        row += fsemu_perfgui.video_image.stride;
        y += 1;
    }

    if (y == 512) {
        y = 0;
    }
    // row =
    //     fsemu_perfgui.video_image.data + y *
    //     fsemu_perfgui.video_image.stride;
    // for (int x = 0; x < 116; x++) {
    //     ((uint32_t *) row)[x] = fsemu_perfgui.colors.line;
    // }
    fsemu_perfgui_draw_line(fsemu_perfgui.video_image.data +
                            y * fsemu_perfgui.video_image.stride);
}

void fsemu_perfgui_update(void)
{
    static int next_frame;
    int frame = fsemu_frame_counter();

    if (fsemu_perfgui.refresh) {
        fsemu_log("Need refreshing perfgui (colors changed)\n");
        next_frame = frame - 256;
#if 1
        if (next_frame < 0) {
            next_frame = 0;
        }
#endif
        fsemu_perfgui.refresh = false;
    }

    int f = next_frame;
    if (frame - f > 256) {
        f = frame - 256;
    }
    // Not inclusive frame is on purpose.
    while (f < frame) {
        fsemu_perfgui_update_audio(f);
        fsemu_perfgui_update_video(f);
        f += 1;
    }
    next_frame = f;

    bool visible = fsemu_perfgui.mode > 0;
    fsemu_gui_item_set_visible(&fsemu_perfgui.audio_item, visible);
    fsemu_gui_item_set_visible(&fsemu_perfgui.video_item, visible);

#if 0
    static int alternating = 0;
    alternating = !alternating;
    fsemu_gui_item_set_visible(&fsemu_perfgui.vsync_test_item_a,
                               visible && alternating);
    fsemu_gui_item_set_visible(&fsemu_perfgui.vsync_test_item_b,
                               visible && !alternating);
#endif
}

void fsemu_perfgui_cycle(void)
{
    if (--fsemu_perfgui.mode == -1) {
        fsemu_perfgui.mode = 2;
    }

    if (fsemu_perfgui.mode > 0) {
        fsemu_perfgui_set_colors(fsemu_perfgui.mode);
    }
}

int fsemu_perfgui_mode(void)
{
    return fsemu_perfgui.mode;
}
