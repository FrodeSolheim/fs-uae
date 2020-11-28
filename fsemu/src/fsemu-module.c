#include "fsemu-internal.h"
#include "fsemu-module.h"

#include <stdlib.h>

#include "fsemu-font.h"
#include "fsemu-glib.h"
#include "fsemu-module.h"

#define module fsemu_module

static struct fsemu_module {
    GList *quit_callbacks;
    bool initialized;
} module;

void fsemu_module_on_quit(fsemu_module_on_quit_f function)
{
    module.quit_callbacks = g_list_append(module.quit_callbacks, function);
}

void fsemu_module_quit(void)
{
    printf("fsemu_module_quit\n");
    // It is probably cleanest to deinit modules in reverse order of
    // initialization.
    module.quit_callbacks = g_list_reverse(module.quit_callbacks);
    GList *list = module.quit_callbacks;
    while (list) {
        fsemu_module_on_quit_f function = (fsemu_module_on_quit_f) list->data;
        function();
        list = list->next;
    }
    g_list_free(module.quit_callbacks);
    module.quit_callbacks = NULL;
}

void fsemu_module_init(void)
{
    if (module.initialized) {
        return;
    }
    module.initialized = true;
}

#undef module
