#include "sysconfig.h"
#include "sysdeps.h"

#include "autoconf.h"
#include "fsdb.h"
#include "options.h"
#include "serial.h"
#include "sleep.h"
#include "uae.h"
#include "xwin.h"
#include "uae/fs.h"
#include "../od-win32/debug_win32.h"

#ifndef PICASSO96
// just to make ncr_scsi compile. it will not work, of course,
// so do not try to use functions in ncr_scsi
uaecptr p96ram_start;
#endif

#ifdef WINDOWS
#else
// need to fake _timezone because some code from WinUAE depend on it (a
// Windows global variable)
int _timezone;
int _daylight;
#endif

int pause_emulation = 0;
int uaelib_debug = 0;

int sleep_resolution = 1000 / 1;
int pissoff_value = 15000 * CYCLE_UNIT;

//int uaeser_getdatalenght (void) {
//    return uaeser_getdatalength();
//}

void target_default_options (struct uae_prefs *p, int type) {
    //write_log("STUB: target_default_options p=%p type=%d\n", p, type);
    write_log("target_default_options p=%p type=%d\n", p, type);
    // FIXME: move out of here - into a (lib)amiga_ function
    write_log("target_default_options: enabling floppy sounds\n");

    for (int i = 0; i < 4; i++) {
        p->floppyslots[i].dfxclick = 1;
    }

    if (type == 2 || type == 0) {
        // if this isn't set to -1, will caused problems for parallel
        // port joysticks
        p->win32_samplersoundcard = -1;
    }

    p->win32_rtgvblankrate = 0;

#ifdef WORDS_BIGENDIAN
    p->picasso96_modeflags = 0x442;
#else
    p->picasso96_modeflags = 0x212;
#endif

    return;
}

/**
 * sleep_millis_main was introduced to custom.cpp in WinUAE 2.4.0b5.
 * FIXME: what does _main signify here?
 */
void sleep_millis_main (int ms) {
    // FIXME: HOW EXACT MUST THE SLEEP BE?
    //printf("sleep_millis_main %d\n", ms);
    usleep(ms * 1000);
    //uae_msleep(ms);
}

void sleep_millis (int ms) {
    // FIXME: HOW EXACT MUST THE SLEEP BE?
    //printf("sleep_millis %d\n", ms);
    // FIXME: check usage of this for CD32
    usleep(ms * 1000);
    //uae_msleep(ms);
}

void console_out_f(const TCHAR *fmt, ...) {
    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    vprintf(fmt, arg_ptr);
    va_end(arg_ptr);
}

/* FIXME: change void *f to FILE* f */
void f_out(void *f, const TCHAR *format, ...)
{
    if (f == NULL) {
        return;
    }
    va_list arg_ptr;
    va_start(arg_ptr, format);
    vfprintf((FILE*) f, format, arg_ptr);
    va_end(arg_ptr);
}

void console_out (const TCHAR *msg) {
    printf("%s", msg);
}

int console_get_gui (TCHAR *out, int maxlen) {
    STUB("");
    return 0;
}

int console_get(TCHAR *in, int maxlen) {
    TCHAR *res = fgets(in, maxlen, stdin);
    if (res == NULL) {
        return -1;
    }
    int len = strlen(in);
    return len - 1;
}

void console_flush(void) {
    fflush(stdout);
}

TCHAR console_getch (void) {
    STUB("");
    return 0;
}

void close_console (void) {
    STUB("");
}

extern void activate_console (void) {
    STUB("");
}

bool console_isch (void)
{
    STUB("");
    return false;
}

/*
struct uae_filter usedfilter_storage
struct uae_filter *usedfilter = &usedfilter_storage;
*/

//struct uae_prefs currprefs;
/*
uae_u8 *mapped_malloc (size_t s, TCHAR *file)
{
    return xmalloc (uae_u8, s);
}

void mapped_free (uae_u8 *p)
{
    xfree (p);
}
*/

//#include "fsdb.h"
// FIXME: to fsdb_unix.cpp

int my_setcurrentdir (const TCHAR *curdir, TCHAR *oldcur) {
    STUB("curdir=\"%s\" oldcur=\"%s\"", curdir, oldcur);
    return 0;
}

bool my_isfilehidden (const TCHAR *path) {
    STUB("path=\"%s\"", path);
    return 0;
}

void my_setfilehidden (const TCHAR *path, bool hidden) {
    STUB("path=\"%s\" hidden=%d", path, hidden);
}

int target_get_volume_name (struct uaedev_mount_info *mtinf,
        const TCHAR *volumepath, TCHAR *volumename, int size, bool inserted,
        bool fullcheck) {
    STUB("");
    return 0;
}

static char *console_buffer;
static int console_buffer_size;

char *setconsolemode (char *buffer, int maxlen) {
    char *ret = NULL;
    if (buffer) {
        console_buffer = buffer;
        console_buffer_size = maxlen;
    }
    else {
        ret = console_buffer;
        console_buffer = NULL;
    }
    return ret;
}

// writelog
TCHAR* buf_out (TCHAR *buffer, int *bufsize, const TCHAR *format, ...) {
    if (buffer == NULL) {
        return 0;
    }
    va_list parms;
    va_start (parms, format);
    vsnprintf (buffer, (*bufsize) - 1, format, parms);
    va_end (parms);
    *bufsize -= _tcslen (buffer);
    return buffer + _tcslen (buffer);
}

void to_lower (TCHAR *s, int len) {
    for (int i = 0; i < len; i++) {
        s[i] = tolower(s[i]);
    }
}

void to_upper (TCHAR *s, int len) {
    for (int i = 0; i < len; i++) {
        s[i] = toupper(s[i]);
    }
}

TCHAR *target_expand_environment (const TCHAR *path) {
    // FIXME:
    return strdup(path);
}

#if 0
#include <signal.h>
#include "debug.h"
#ifdef __cplusplus_disabled
static RETSIGTYPE sigbrkhandler(...)
#else
static RETSIGTYPE sigbrkhandler (int foo)
#endif
{
#ifdef DEBUGGER
    activate_debugger ();
#endif

#if !defined(__unix) || defined(__NeXT__)
    signal (SIGINT, sigbrkhandler);
#endif
}
#endif

void setup_brkhandler (void)
{
    /*
#if defined(__unix) && !defined(__NeXT__)
    struct sigaction sa;
    sa.sa_handler = sigbrkhandler;
    sa.sa_flags = 0;
#ifdef SA_RESTART
    sa.sa_flags = SA_RESTART;
#endif
    sigemptyset (&sa.sa_mask);
    sigaction (SIGINT, &sa, NULL);
#else
    signal (SIGINT, sigbrkhandler);
#endif
    */
}
