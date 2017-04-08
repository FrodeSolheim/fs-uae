#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define FSE_INTERNAL_API
#include <fs/emu/path.h>

#include <fs/glib.h>

static fs_emu_path_expand_function g_expand_function;

void fs_emu_path_set_expand_function(fs_emu_path_expand_function function)
{
    g_expand_function = function;
}

char *fs_emu_path_expand(const char *path)
{
    if (g_expand_function) {
        return g_expand_function(path);
    } else {
        return g_strdup(path);
    }
}

char *fs_emu_path_expand_and_free(char *path)
{
    char *new_path = fs_emu_path_expand(path);
    g_free(path);
    return new_path;
}
