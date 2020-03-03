#define FSEMU_INTERNAL
#include "fsemu-fontcache.h"

#include "fsemu-font.h"
#include "fsemu-glib.h"
#include "fsemu-module.h"

#define module fsemu_fontcache

// ----------------------------------------------------------------------------

static struct fsemu_fontcache {
    GHashTable *hashtable;
    bool initialized;
} module;

fsemu_font_t *fsemu_fontcache_font(const char *name, int size)
{
    fsemu_fontcache_init();

    char key[512];
    snprintf(key, 511, "%s:%d", name, size);
    key[511] = '\0';

    fsemu_font_t *font =
        (fsemu_font_t *) g_hash_table_lookup(module.hashtable, key);
    if (font == NULL) {
        font = fsemu_font_load(name, size);
        g_hash_table_insert(module.hashtable, g_strdup(key), font);
    }
    return font;
}

static void fsemu_fontcache_quit_iterate(gpointer key_v,
                                         gpointer value_v,
                                         gpointer user_data)
{
    const char *key = (const char *) key_v;
    fsemu_font_t *font = (fsemu_font_t *) value_v;
    printf("[FSEMU] [FONT ] Font: %s\n", key);
    // FIXME: Check if refcount is 1, and print warning if not?
    fsemu_font_unref(font);
}

static void fsemu_fontcache_quit(void)
{
    printf("[FSEMU] [FONT ] fsemu_fontcache_quit\n");
    printf("[FSEMU] [FONT ] Font cache summary:\n");
    g_hash_table_foreach(module.hashtable, fsemu_fontcache_quit_iterate, NULL);
    g_hash_table_destroy(module.hashtable);
}

void fsemu_fontcache_init(void)
{
    if (module.initialized) {
        return;
    }
    module.initialized = true;
    fsemu_log("[FSEMU] [FONT ] Initializing fontcache module\n");
    fsemu_module_on_quit(fsemu_fontcache_quit);
    module.hashtable =
        g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
}

// ----------------------------------------------------------------------------

#undef module
