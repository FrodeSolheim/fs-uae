#include "sysconfig.h"
#include "sysdeps.h"

#include "uae.h"
#include "gui.h"
#include "compemu.h"
#include "uae/fs.h"
#include "uae/glib.h"

#include <stdio.h>

int log_scsi = 0;
int log_net = 0;

void write_log (const TCHAR *format, ...)
{
    va_list args;
    va_start(args, format);
    char *buffer = g_strdup_vprintf(format, args);
    va_end(args);
    log_function function = g_libamiga_callbacks.log;
    if (function) {
        function(buffer);
    } else {
        printf("%s", buffer);
    }
    free(buffer);
}

void gui_message (const char *format,...)
{
    va_list args;
    va_start(args, format);
    char *buffer = g_strdup_vprintf(format, args);
    va_end(args);
    if (g_amiga_gui_message_function) {
        g_amiga_gui_message_function(buffer);
    } else {
        printf("%s", buffer);
    }
    g_free(buffer);
}

static const char *get_message(int msg)
{
    if (msg == NUMSG_NOCAPS) {
        return "IPF support plugin (capsimg) is not installed";
    } else if (msg == NUMSG_ROMNEED) {
        return "One of the following system ROMs is required: %s";
    } else if (msg == NUMSG_EXPROMNEED) {
        return "One of the following expansion boot ROMs is required: %s";
    } else if (msg == NUMSG_KS68020) {
        return "The selected system ROM requires a 68020 with 32-bit "
               "addressing or 68030 or higher CPU";
    } else if (msg == NUMSG_KS68030) {
        return "The selected system ROM requires a 68030 CPU";
    } else if (msg == NUMSG_MODRIP_NOTFOUND) {
        return "Module ripper: No music modules or packed data found";
    } else if (msg == NUMSG_MODRIP_FINISHED) {
        return "Module ripper: Scan finished";
    } else if (msg == NUMSG_NO_PPC) {
        return "PPC CPU was started but qemu-uae plugin was not found";
    } else if (msg == NUMSG_UAEBOOTROM_PPC) {
        return "UAE boot ROM cannot be used with PPC native OS";
    }

    return NULL;
}

void notify_user (int msg)
{
    const char *message = get_message(msg);
    if (message) {
        gui_message(message);
    } else {
        gui_message (_T("notify_user msg #%d"), msg);
    }
}

int translate_message (int msg, TCHAR *out)
{
    const char *message = get_message(msg);
    if (message) {
        snprintf(out, MAX_DPATH, "%s", message);
    } else {
        snprintf(out, MAX_DPATH, "translate_message #%d\n", msg);
    }
    return 1;
}

void notify_user_parms (int msg, const TCHAR *parms, ...)
{
    gui_message (_T("notify_user msg #%d\n"), msg);
}

void jit_abort (const TCHAR *format,...)
{

    va_list args;
    va_start(args, format);
    char *buffer = g_strdup_vprintf(format, args);
    va_end(args);
    log_function function = g_libamiga_callbacks.log;
    if (function) {
        function(buffer);
    } else {
        printf("%s", buffer);
    }
#if 1
    fprintf(stderr, "JIT: %s\n", buffer);
    abort();
#else
    static int happened;
    //int count;
    if (!happened)
        gui_message (_T("JIT: Serious error:\n%s"), buffer);
    happened = 1;
    uae_reset(1, 1);
#endif
    g_free(buffer);
}
