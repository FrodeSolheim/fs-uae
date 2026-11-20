#define FSEMU_INTERNAL
#include "fsemu.h"

#include "fsapp-time.h"
#include "fsemu-glib.h"
#include "fsemu-option.h"
#include "fsemu-video.h"

static struct {
    int64_t startup_time_ns;
    int64_t startup_position_time_ns;
    char *emulator_name;
    int video_format_flags;
    char *config_file_extension;
} fsemu;

void fsemu_boot_log(const char *msg)
{
    int64_t now_ns = (int64_t)SDL_GetTicksNS();
    if (fsemu.startup_position_time_ns == 0) {
        fsemu.startup_position_time_ns = now_ns;
    }
    // printf("+%03d %s\n", (int)SDL_NS_TO_MS(now_ns - fsemu.startup_position_time_ns), msg);
    fsemu.startup_position_time_ns = now_ns;
}

#if 0
void fsemu_set_startup_time(void)
{
    fsemu.startup_time_ns = (int64_t)SDL_GetTicksNS();
    fsemu.startup_position_time_ns = fsemu.startup_time_ns;
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
    fsapp_assert(name != NULL);
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
    fsapp_assert(extension);
    fsemu.config_file_extension = strdup(extension);
}

// void fsemu_init_with_args(int argc, char *argv[])
// {
//     fsemu_boot_log("fsemu_init_with_args");
//     fsemu_option_init();
//     for (int i = 0; i < argc; i++) {
//         printf("ARG %d: %s\n", i, argv[i]);
//         if (fsemu.config_file_extension) {
//             if (g_str_has_suffix(argv[i], fsemu.config_file_extension)) {
//                 printf("Found config file: %s\n", argv[i]);
//                 fsemu_option_read_config_file(argv[i]);
//             }
//         }
//     }
//     fsemu_init();
// }
