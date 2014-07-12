#include "sysconfig.h"
#include "sysdeps.h"

#include <stdlib.h>
#include <stdarg.h>

#include "clipboard.h"

#include "threaddep/thread.h"
#include "memory_uae.h"
#include "native2amiga_api.h"

int clipboard_debug;

static uaecptr clipboard_data;
// static int vdelay, signaling, initialized;
// static uae_u8 *to_amiga;
// static uae_u32 to_amiga_size;
// static int clipopen;
// static int clipactive;
// static int clipboard_change;
// static void *clipboard_delayed_data;
// static int clipboard_delayed_size;

void amiga_clipboard_die (void)
{
    // signaling = 0;
    write_log ("clipboard not initialized\n");
}

void amiga_clipboard_init (void)
{
    // signaling = 0;
    STUB ("clipboard initialized\n");
    // initialized = 1;
}

void amiga_clipboard_task_start (uaecptr data)
{
    clipboard_data = data;
    // signaling = 1;
    STUB ("clipboard task init: %08x\n", clipboard_data);
}

uaecptr amiga_clipboard_proc_start (void)
{
    STUB ("clipboard process init: %08x\n", clipboard_data);
    // signaling = 1;
    return clipboard_data;
}

void amiga_clipboard_got_data (uaecptr data, uae_u32 size, uae_u32 actual)
{
    // if (!initialized) {
    //     write_log ("clipboard: got_data() before initialized!?\n");
    //     return;
    // }
}
