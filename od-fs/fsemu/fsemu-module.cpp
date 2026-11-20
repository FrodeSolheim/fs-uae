#define FSEMU_INTERNAL
#include "fsemu-module.h"

#include <stddef.h>
#include <stdlib.h>

#include "fsemu-font.h"
#include "fsemu-glib.h"
#include "fsemu-module.h"

static struct fsemu_module {
    GList* quit_callbacks;
    bool initialized;
} fsemu_module;

fsemu_error_t fsemu_module_init(const char* name, bool* init, fsemu_module_on_quit_f quit) {
    fsemu_log("[...] Init %s module\n", name);
    if (*init) {
        // Already initialized
        return 1;
    }
    *init = true;
    if (quit != NULL) {
        fsemu_module_on_quit(quit);
    }
    return 0;
}

void fsemu_module_on_quit(fsemu_module_on_quit_f function) {
    fsemu_module.quit_callbacks = g_list_append(fsemu_module.quit_callbacks, function);
}

void fsemu_module_quit(void) {
    fsemu_log("[FSE] [...] fsemu_module_quit\n");
    // It is probably cleanest to deinit modules in reverse order of
    // initialization.
    fsemu_module.quit_callbacks = g_list_reverse(fsemu_module.quit_callbacks);
    GList* list = fsemu_module.quit_callbacks;
    while (list) {
        fsemu_module_on_quit_f function = (fsemu_module_on_quit_f)list->data;
        function();
        list = list->next;
    }
    g_list_free(fsemu_module.quit_callbacks);
    fsemu_module.quit_callbacks = NULL;
}

// void fsemu_module_init(void)
// {
//     if (fsemu_module.initialized) {
//         return;
//     }
//     fsemu_module.initialized = true;
// }
