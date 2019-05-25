#include "sysconfig.h"
#include "sysdeps.h"

#include "autoconf.h"
#include "fsdb.h"
#include "options.h"
#include "serial.h"
#include "sleep.h"
#include "uae.h"
#include "xwin.h"
#include "clipboard.h"
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

int target_sleep_nanos(int nanos)
{
    struct timespec req;
    req.tv_sec = nanos / 1000000;
    req.tv_nsec = nanos % 1000000;
    // struct timespec rem
    nanosleep(&req, NULL);
    return 0;
}

static int sleep_millis2 (int ms, bool main)
{
    /* FIXME: Check sleep_millis2 against od-win32/win32.cpp implementation. */
    // FIXME: HOW EXACT MUST THE SLEEP BE?
    //printf("sleep_millis %d\n", ms);
    // FIXME: check usage of this for CD32
    // usleep(ms * 1000);
    //uae_msleep(ms);
    struct timespec req;
    req.tv_sec = ms / 1000;
    req.tv_nsec = (ms % 1000) * 1000;
    // struct timespec rem
    nanosleep(&req, NULL);
    return 0;
}

int sleep_millis_main (int ms)
{
	return sleep_millis2(ms, true);
}
int sleep_millis (int ms)
{
	return sleep_millis2(ms, false);
}

int sleep_millis_amiga(int ms)
{
	int ret = sleep_millis_main(ms);
	return ret;
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

int console_get_gui (TCHAR *out, int maxlen) {
    STUB("");
    return 0;
}

//#include "fsdb.h"
// FIXME: to fsdb_unix.cpp

int my_setcurrentdir (const TCHAR *curdir, TCHAR *oldcur)
{
    STUB("curdir=\"%s\" oldcur=\"%s\"", curdir, oldcur);
    return 0;
}

bool my_isfilehidden (const TCHAR *path)
{
    STUB("path=\"%s\"", path);
    return 0;
}

void my_setfilehidden (const TCHAR *path, bool hidden)
{
    STUB("path=\"%s\" hidden=%d", path, hidden);
}

int target_get_volume_name (
        struct uaedev_mount_info *mtinf,
        struct uaedev_config_info *ci, bool inserted, bool fullcheck, int cnt)
{
    /* FIXME: Check what target_get_volume_name in od-win32/win32_fileys.cpp
     * does. */
    STUB("");
    return 0;
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

TCHAR *target_expand_environment (const TCHAR *path, TCHAR *out, int maxlen)
{
	if (!path)
		return NULL;
	if (out == NULL) {
		return strdup(path);
	} else {
		_tcscpy(out, path);
		return out;
	}
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

void sleep_cpu_wakeup(void)
{
    UAE_LOG_STUB_MAX(10, "");
}

void target_multipath_modified(struct uae_prefs *p)
{
    UAE_STUB("");
    /*
	if (p != &workprefs)
		return;
	memcpy(&currprefs.path_hardfile, &p->path_hardfile, sizeof(struct multipath));
	memcpy(&currprefs.path_floppy, &p->path_floppy, sizeof(struct multipath));
	memcpy(&currprefs.path_cd, &p->path_cd, sizeof(struct multipath));
	memcpy(&currprefs.path_rom, &p->path_rom, sizeof(struct multipath));
    */
}

void target_reset (void)
{
	clipboard_reset ();
}
