#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define FSE_INTERNAL_API
#include <fs/emu/video.h>
#include <fs/log.h>

static void register_functions(void)
{

}

void fs_emu_video_dummy_init(void)
{
    fs_log("fs_emu_video_dummy_init\n");
    register_functions();
}
