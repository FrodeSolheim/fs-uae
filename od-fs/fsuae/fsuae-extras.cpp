#include "fsuae-extras.h"

#include <SDL3/SDL.h>
#include <stdio.h>

#define DUMMY_KEEP_INCLUDE_ORDER
#include "sysconfig.h"
#include "sysdeps.h"
#undef DUMMY_KEEP_INCLUDE_ORDER

#include "inputdevice.h"
#include "options.h"
#include "savestate.h"
#include "uae/fs.h"
#include "uae/uae.h"
#include "win32gfx.h"

// #define UAE_FS_H

static void save_config(struct uae_prefs* prefs, const char* name) {
    char path[PATH_MAX];
    SDL_strlcpy(path, "od-fs/python/uae/configurations/", PATH_MAX);
    SDL_strlcat(path, name, PATH_MAX);
    SDL_strlcat(path, ".py", PATH_MAX);

    // python/uae/configurations/a500.uae
    size_t len;
    char* s = (char*)save_configuration(&len, true);
    FILE* f = fopen(path, "wb");
    if (f == NULL) {
        printf("Could not open file %s\n", path);
        exit(1);
    }
    fprintf(f, "builtin_config_%s = \"\"\"\\\n", name);
    fputs(s, f);
    fprintf(f, "\"\"\"\n");
    fclose(f);
}

static void load_prefs(int model, int config, int compa) {
    // FIXME: Investigate a bit more? Get different results when clearing
    // memory vs not doing it. Seems built_in_prefs does not reset everything?
    // memset(&currprefs, 0x00, sizeof(currprefs));

    // OK, actually, default_prefs does clear using memset - so that should
    // probably be used first...
    default_prefs(&currprefs, true, 0);

    fixup_prefs(&currprefs, true);

    built_in_prefs(&currprefs, model, config, compa, false);
}

static void dump_quickstarts(void) {
    printf("dump_quickstarts\n");

    keyboard_settrans();
    inputdevice_init();
    enumeratedisplays();

    default_prefs(&currprefs, true, 0);
    save_config(&currprefs, "default");

    load_prefs(0, 0, 0);
    save_config(&currprefs, "a500");

    load_prefs(1, 0, 0);
    save_config(&currprefs, "a500p");

    load_prefs(2, 0, 0);
    save_config(&currprefs, "a600");

    load_prefs(3, 0, 0);
    save_config(&currprefs, "a1000");

    load_prefs(4, 0, 0);
    save_config(&currprefs, "a1200");

    load_prefs(5, 0, 0);
    save_config(&currprefs, "a3000");

    load_prefs(6, 0, 0);
    save_config(&currprefs, "a4000");

    load_prefs(7, 0, 0);
    save_config(&currprefs, "a4000t");

    load_prefs(8, 0, 0);
    save_config(&currprefs, "cd32");

    load_prefs(9, 0, 0);
    save_config(&currprefs, "cdtv");

    // load_prefs(0, 0, 0);
    // save_config(&currprefs, "a500");

    printf("\n");
    printf("Configurations written.\n");
    exit(0);
}

#ifdef __cplusplus
extern "C" {
#endif

void fsuae_extras(int argc, char* argv[]) {
    if (argc > 1) {
        if (strcasecmp(argv[1], "spc:dump-quickstarts") == 0) {
            dump_quickstarts();
        }
    }
}

#ifdef __cplusplus
}
#endif
