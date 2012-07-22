/*
 * UAE - The Un*x Amiga Emulator
 *
 * Not a parser, but parallel and serial emulation for Linux
 *
 * Copyright 2010 Mustafa TUFAN
 */

#include "sysconfig.h"

#undef SERIAL_ENET

#include "sysdeps.h"
#include "options.h"
#include "gensound.h"
#include "events.h"
#include "uae.h"
#include "include/memory.h"
#include "custom.h"
#include "autoconf.h"
#include "newcpu.h"
#include "traps.h"
#include "threaddep/thread.h"
#include "serial.h"
#include "savestate.h"
#include "xwin.h"
#include "drawing.h"

#define MIN_PRTBYTES 10

struct uaeserialdata
{
    long hCom;
    long evtr, evtw, evtt, evtwce;
    long olr, olw, olwce;
    int writeactive;
    void *readdata, *writedata;
    volatile int threadactive;
    uae_sem_t change_sem, sync_sem;
    void *user;
};

int uaeser_getdatalength (void)
{
    return sizeof (struct uaeserialdata);
}

void uaeser_initdata (void *vsd, void *user)
{
    STUB("");
}

int uaeser_query (void *vsd, uae_u16 *status, uae_u32 *pending)
{
    STUB("");
    return 0;
}

int uaeser_break (void *vsd, int brklen)
{
    STUB("");
    return 0;
}

int uaeser_setparams (void *vsd, int baud, int rbuffer, int bits, int sbits, int rtscts, int parity, uae_u32 xonxoff)
{
    STUB("");
    return 0;
}

void uaeser_trigger (void *vsd)
{
    STUB("");
}

int uaeser_write (void *vsd, uae_u8 *data, uae_u32 len)
{
    STUB("");
    return 0;
}

int uaeser_read (void *vsd, uae_u8 *data, uae_u32 len)
{
    STUB("");
    return 0;
}

void uaeser_clearbuffers (void *vsd)
{
    STUB("");
}

int uaeser_open (void *vsd, void *user, int unit)
{
    STUB("");
    return 0;
}

void uaeser_close (void *vsd)
{
    STUB("");
}

#define SERIAL_WRITE_BUFFER 100
#define SERIAL_READ_BUFFER 100
/*
static uae_u8 outputbuffer[SERIAL_WRITE_BUFFER];
static uae_u8 outputbufferout[SERIAL_WRITE_BUFFER];
static uae_u8 inputbuffer[SERIAL_READ_BUFFER];
static int datainoutput;
static int dataininput, dataininputcnt;
static int writepending;
*/

void initparallel (void) {
#if 0
    if (uae_boot_rom) {
        uaecptr a = here (); //this install the ahisound
        org (rtarea_base + 0xFFC0);
        calltrap (deftrapres (ahi_demux, 0, _T("ahi_winuae")));
        dw (RTS);
        org (a);
        init_ahi_v2 ();
    }
#endif
}

int isprinter (void) {
    return 0;
}

void doprinter (uae_u8 val) {
    //parflush = 0;
    //DoSomeWeirdPrintingStuff (val);
}

int parallel_direct_write_status (uae_u8 v, uae_u8 dir) {
    return 0;
}

int parallel_direct_read_status (uae_u8 *vp) {
    return 0;
}

int parallel_direct_write_data (uae_u8 v, uae_u8 dir) {
    return 0;
}

int parallel_direct_read_data (uae_u8 *v) {
    return 0;
}
