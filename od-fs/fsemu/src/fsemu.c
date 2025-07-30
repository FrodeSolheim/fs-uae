#define FSEMU_INTERNAL
#include "fsemu.h"

#include "fsemu-glib.h"
#include "fsemu-helper.h"
#include "fsemu-main.h"
#include "fsemu-option.h"
#include "fsemu-thread.h"
#include "fsemu-time.h"
#include "fsemu-video.h"

static struct {
    int64_t startup_time;
    int64_t startup_position_time;
    fsemu_thread_t *emulation_thread;
    char *emulator_name;
    int video_format_flags;
    char *config_file_extension;
} fsemu;

void fsemu_boot_log(const char *msg)
{
    int64_t t = fsemu_time_us();
    if (fsemu.startup_position_time == 0) {
        fsemu.startup_position_time = t;
    }
    // printf(
    //     "+%03d %s\n", (int) ((t - fsemu.startup_position_time) / 1000),
    //     msg);
    fsemu.startup_position_time = t;
}

#if 0
void fsemu_set_startup_time(void)
{
    fsemu.startup_time = fsemu_time_us();
    fsemu.startup_position_time = fsemu_time_us();
}
#endif

const char *fsemu_emulator_name(void)
{
    if (fsemu.emulator_name == NULL) {
        return "FSEMU";
    }
    return fsemu.emulator_name;
}

void fsemu_set_emulator_name(const char *name)
{
    fsemu_assert(name != NULL);
    if (fsemu.emulator_name != NULL) {
        free(fsemu.emulator_name);
    }
    fsemu.emulator_name = strdup(name);
}

int fsemu_video_format_flags(void)
{
    return fsemu.video_format_flags;
}

void fsemu_set_video_format_flags(int flags)
{
    fsemu.video_format_flags = flags;
}

void fsemu_set_config_file_extension(const char *extension)
{
    fsemu_assert(extension);
    fsemu.config_file_extension = strdup(extension);
}

void fsemu_init(void)
{
    fsemu_boot_log("fsemu_init_with_args");
    fsemu_helper_init_emulator(
        fsemu_emulator_name(), false, FSEMU_VIDEO_RENDERER_GL, false, false);
}

void fsemu_init_with_args(int argc, char *argv[])
{
    fsemu_boot_log("fsemu_init_with_args");
    fsemu_option_init();
    for (int i = 0; i < argc; i++) {
        printf("ARG %d: %s\n", i, argv[i]);
        if (fsemu.config_file_extension) {
            if (g_str_has_suffix(argv[i], fsemu.config_file_extension)) {
                printf("Found config file: %s\n", argv[i]);
                fsemu_option_read_config_file(argv[i]);
            }
        }
    }
    fsemu_init();
}

void fsemu_begin(void)
{
    fsemu_helper_startup_loop();
}

static void *fsemu_emulation_thread_function(void *data)
{
    fsemu_assert_release(data != NULL);

    fsemu_log("Emulation thread started\n");
    fsemu_thread_set_emu();

    void (*function)() = data;
    fsemu_log("Run main emulation function at %p\n", function);
    function();
    fsemu_log("Emulation thread ending\n");
    return NULL;
}

void fsemu_run(void (*function)(void))
{
    fsemu.emulation_thread = fsemu_thread_create(
        "emulation", fsemu_emulation_thread_function, function);
    if (fsemu.emulation_thread == NULL) {
        fsemu_log_error("Error starting emulation thread\n");
        // FIXME: ERROR MESSAGE HERE
        // FIXME: FATAL
    }
}

bool fsemu_running(void)
{
    return fsemu_main_is_running();
}

void fsemu_update(void)
{
    fsemu_main_update_and_snapshot_gui();
}

void fsemu_handle_events(void)
{
    fsemu_main_handle_events_until_next_frame();
}

void fsemu_end(void)
{
    fprintf(stderr, "FIXME: Implement fsemu_end\n");
    if (fsemu.emulation_thread) {
        // FIXME
        // fsemu_thread_wait(fsemu_emulation_thread);
    }
    SDL_Quit();
}
