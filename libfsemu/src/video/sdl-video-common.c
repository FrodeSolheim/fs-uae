#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define FSE_INTERNAL_API
#include <fs/emu/video.h>
#include <fs/emu/input.h>
#include <fs/emu/buffer.h>
#include <fs/emu/options.h>
#include <fs/conf.h>
#include <fs/log.h>
#include <string.h>

#include "SDL.h"

// FIXME: move
#include <fs/emu/benchmark.h>
bool g_fs_emu_benchmark_mode = false;

volatile int g_fs_emu_benchmark_emu_frame_count = 0;
volatile int g_fs_emu_benchmark_sys_frame_count = 0;

int (*fse_main_loop)(void);

static void benchmark(void)
{
    static int64_t last_benchmark_time = 0;
    static int last_emu_frame_count = 0;
    static int last_sys_frame_count = 0;
    if (last_benchmark_time == 0) {
        last_benchmark_time = fs_emu_monotonic_time();
    }

    if (g_fs_emu_benchmark_mode) {
        int64_t t =  fs_emu_monotonic_time();
        int64_t td = t - last_benchmark_time;
        if (td > 1 * 1000 * 1000) {
            int emu_frame_count = g_fs_emu_benchmark_emu_frame_count;
            int sys_frame_count = g_fs_emu_benchmark_sys_frame_count;

            int emu_frames = emu_frame_count - last_emu_frame_count;
            int sys_frames = sys_frame_count - last_sys_frame_count;

            last_emu_frame_count = emu_frame_count;
            last_sys_frame_count = sys_frame_count;

            printf("%d %d\n", sys_frame_count, sys_frames);

            printf("EMU: %5.1f SYS: %5.1f\n",
                   (double) emu_frames * 1000000 / (double) td,
                   (double) sys_frames * 1000000  / (double) td);
            last_benchmark_time = t;
        }
    }
}

// FIXME: move?
int fs_emu_main_loop(void)
{
    int benchmark_frame_count = -1;
    if (getenv("FS_BENCHMARK")) {
        g_fs_emu_benchmark_mode = true;
        benchmark_frame_count = atoi(getenv("FS_BENCHMARK"));

    }

    int buffer = 0;
    while (true) {
        while ((buffer = fs_emu_buffer_wait(buffer, 1000)) == -1) {
            if (fse_input.input_handler()) {
                return 0;
            }
        }
        buffer += 1;
        // printf("render\n");
        fs_emu_video_render();

        g_fs_emu_benchmark_sys_frame_count += 1;
        if (g_fs_emu_benchmark_mode) {
            benchmark();
        }
    }
}
