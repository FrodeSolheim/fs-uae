#include "sysconfig.h"
#include "sysdeps.h"
#include "uae.h"
#include "gui.h"
#include "uae/fs.h"

#include <stdio.h>
#include <fs/string.h>
#include <glib.h>

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
    }
    else {
        printf("%s", buffer);
    }
    free(buffer);
}

void gui_message (const char *format,...)
{
    va_list args;
    va_start(args, format);
    char *buffer = fs_strdup_vprintf(format, args);
    va_end(args);
    if (g_amiga_gui_message_function) {
        g_amiga_gui_message_function(buffer);
    }
    else {
        printf("%s", buffer);
    }
    free(buffer);
}

void notify_user (int msg)
{
    if (msg == NUMSG_NOCAPS) {
                gui_message(_T("Missing libcapsimage plugin"));
    }
    else {
        gui_message (_T("notify_user msg #%d\n"), msg);
    }
}

void notify_user_parms (int msg, const TCHAR *parms, ...)
{
    gui_message (_T("notify_user msg #%d\n"), msg);
}

void jit_abort (const TCHAR *format,...)
{

    va_list args;
    va_start(args, format);
    char *buffer = fs_strdup_vprintf(format, args);
    va_end(args);
    log_function function = g_libamiga_callbacks.log;
    if (function) {
        function(buffer);
    }
    else {
        printf("%s", buffer);
    }
    free(buffer);

    static int happened;
    //int count;
    if (!happened)
        gui_message (_T("JIT: Serious error:\n%s"), buffer);
    happened = 1;
    uae_reset(1, 1);
}
