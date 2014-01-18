#include "sysconfig.h"
#include "sysdeps.h"

#include "traps.h"
#include "uaenative.h"

static const char **g_amiga_native_library_dirs;

extern "C" {

void amiga_set_native_library_dirs(const char **library_dirs) {
    g_amiga_native_library_dirs = library_dirs;
}

} // extern "C"

const char **uaenative_get_library_dirs(void) {
    if (g_amiga_native_library_dirs) {
        return g_amiga_native_library_dirs;
    }

    static const char *paths[1];
    return paths;
}
