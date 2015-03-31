/*
 * UAE - The Un*x Amiga Emulator
 *
 * Not a parser, but parallel and serial emulation for Linux
 *
 * Copyright 2010 Mustafa TUFAN
 */

#include "sysconfig.h"

#undef SERIAL_ENET

#include "config.h"
#include "sysdeps.h"
#include "options.h"
#include "gensound.h"
#include "events.h"
#include "uae.h"
#include "uae/memory.h"
#include "custom.h"
#include "autoconf.h"
#include "newcpu.h"
#include "traps.h"
#include "ahidsound.h"
#include "picasso96.h"
#include "threaddep/thread.h"
#include "serial.h"
#include "parser.h"
#include "parallel.h"
#include "cia.h"
#include "savestate.h"
#include "ahidsound_new.h"
#include "xwin.h"
#include "drawing.h"

#ifdef POSIX_SERIAL
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#endif

#if !defined B300 || !defined B1200 || !defined B2400 || !defined B4800 || !defined B9600
#undef POSIX_SERIAL
#endif
#if !defined B19200 || !defined B57600 || !defined B115200 || !defined B230400
#undef POSIX_SERIAL
#endif

#ifdef POSIX_SERIAL
struct termios tios;
#endif

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

static void uaeser_initdata (void *vsd, void *user)
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
    //write_log("initparallel uae_boot_rom = %d\n", uae_boot_rom);
    write_log("initparallel\n");
#ifdef AHI
    if (uae_boot_rom) {
        write_log("installing ahi_winuae\n");
        uaecptr a = here (); //this install the ahisound
        org (rtarea_base + 0xFFC0);
        calltrap (deftrapres (ahi_demux, 0, _T("ahi_winuae")));
        dw (RTS);
        org (a);
#ifdef AHI_V2
        init_ahi_v2 ();
#endif
    }
#endif
}

extern int flashscreen;

void doflashscreen (void)
{
#if 0
	flashscreen = 10;
	init_colors ();
	picasso_refresh ();
	reset_drawing ();
	flush_screen (gfxvidinfo.outbuffer, 0, 0);
#endif
}

void hsyncstuff (void)
	//only generate Interrupts when
	//writebuffer is complete flushed
	//check state of lwin rwin
{
	//static int keycheck = 0;

#if 0 // DISABLED -- OLD AHI VERSION?
#ifdef AHI
	{ //begin ahi_sound
		static int count;
		if (ahi_on) {
			count++;
			//15625/count freebuffer check
			if(count > ahi_pollrate) {
				ahi_updatesound (1);
				count = 0;
			}
		}
	} //end ahi_sound
#endif
#endif

#if 0 // DISABLED FOR NOW
#ifdef PARALLEL_PORT
	keycheck++;
	if(keycheck >= 1000)
	{
		if (prtopen)
			flushprtbuf ();
		{
			if (flashscreen > 0) {
				flashscreen--;
				if (flashscreen == 0) {
					init_colors ();
					reset_drawing ();
					picasso_refresh ();
					flush_screen (gfxvidinfo.outbuffer, 0, 0);
				}
			}
		}
		keycheck = 0;
	}
	if (currprefs.parallel_autoflush_time && !currprefs.parallel_postscript_detection) {
		parflush++;
		if (parflush / ((currprefs.ntscmode ? MAXVPOS_NTSC : MAXVPOS_PAL) * MAXHPOS_PAL / maxhpos) >= currprefs.parallel_autoflush_time * 50) {
			flushprinter ();
			parflush = 0;
		}
	}
#endif
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

void flushprinter (void) {
    STUB("");
}

// ----- Paula serial emulation host calls -----

static int ser_fd = -1;

int openser (const TCHAR *sername)
{
#ifdef POSIX_SERIAL
    ser_fd = open (currprefs.sername, O_RDWR|O_NONBLOCK|O_BINARY, 0);
    write_log("serial: open '%s' -> fd=%d\n", sername, ser_fd);
    return (ser_fd >= 0);
#else
    return 0;
#endif
}

void closeser (void)
{
#ifdef POSIX_SERIAL
    write_log("serial: close fd=%d\n", ser_fd);
    if(ser_fd >= 0) {
        close(ser_fd);
        ser_fd = 0;
    }
#endif
}

int setbaud (long baud)
{
    if (!currprefs.use_serial) {
        return 1;
    }

#if defined POSIX_SERIAL
    int pspeed;
    
    /* device not open? */
    if (ser_fd < 0) {
        return 0;
    }
    
    /* map to terminal baud rate constant */
    write_log ("serial: setbaud: %ld\n", baud);    
    switch (baud) {
    case 300: pspeed=B300; break;
    case 1200: pspeed=B1200; break;
    case 2400: pspeed=B2400; break;
    case 4800: pspeed=B4800; break;
    case 9600: pspeed=B9600; break;
    case 19200: pspeed=B19200; break;
    case 38400: pspeed=B38400; break;
    case 57600: pspeed=B57600; break;
    case 115200: pspeed=B115200; break;
    case 230400: pspeed=B230400; break;
    default:
        write_log ("serial: unsupported baudrate %ld\n", baud);
        return 0;
    }

    /* Only access hardware when we own it */
    if (tcgetattr (ser_fd, &tios) < 0) {
        write_log ("serial: TCGETATTR failed\n");
        return 0;
    }

    if (cfsetispeed (&tios, pspeed) < 0) {    /* set serial input speed */
        write_log ("serial: CFSETISPEED (%ld bps) failed\n", baud);
        return 0;
    }
    if (cfsetospeed (&tios, pspeed) < 0) {    /* set serial output speed */
        write_log ("serial: CFSETOSPEED (%ld bps) failed\n", baud);
        return 0;
    }

    if (tcsetattr (ser_fd, TCSADRAIN, &tios) < 0) {
        write_log ("serial: TCSETATTR failed\n");
        return 0;
    }
#endif
    return 1;
}

int readseravail (void)
{
    if (!currprefs.use_serial) {
        return 0;
    }

#ifdef POSIX_SERIAL
    /* device is closed */
    if(ser_fd < 0) {
        return 0;
    }
    
    /* poll if read data is available */
    struct timeval tv;
    fd_set fd;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fd);
    FD_SET(ser_fd, &fd);
    int num_ready = select (FD_SETSIZE, &fd, NULL, NULL, &tv);
    return (num_ready == 1);
#else
    return 0;
#endif
}

int readser (int *buffer)
{
    if (ser_fd < 0 || !currprefs.use_serial) {
        return 0;
    }

    char b;
    int num = read(ser_fd, &b, 1);
    if (num == 1) {
        *buffer = b;
        return 1;
    } else { 
        return 0;
    }
}

int checkserwrite (void)
{
    if (ser_fd < 0 || !currprefs.use_serial) {
        return 1;
    }
    
    /* we assume that we can write always */
    return 1;
}

void writeser (int c)
{
    if (ser_fd < 0 || !currprefs.use_serial) {
        return;
    }
    
    char b = (char)c;
    if (write(ser_fd, &b, 1) != 1) {
        write_log("WARNING: writeser - 1 byte was not written (errno %d)\n",
                  errno);
    }
}

void getserstat (int *pstatus)
{
    *pstatus = 0;
    
    if (ser_fd < 0 || !currprefs.use_serial) {
        return;
    }

#ifdef POSIX_SERIAL
    int status = 0;
    
    /* read control signals */
    if (ioctl (ser_fd, TIOCMGET, &status) < 0) {
        write_log ("serial: ioctl TIOCMGET failed\n");
        *pstatus = TIOCM_CTS | TIOCM_CAR | TIOCM_DSR;
        return;
    }

    int out = 0;
    if (status & TIOCM_CTS)
        out |= TIOCM_CTS;
    if (status & TIOCM_CAR)
        out |= TIOCM_CAR;
    if (status & TIOCM_DSR)
        out |= TIOCM_DSR;
    if (status & TIOCM_RI)
        out |= TIOCM_RI;
    
    *pstatus = out;
#endif
}

void setserstat (int mask, int onoff)
{
    if (ser_fd < 0 || !currprefs.use_serial) {
        return;
    }
    
#ifdef POSIX_SERIAL
    int status = 0;
    
    /* read control signals */
    if (ioctl (ser_fd, TIOCMGET, &status) < 0) {
        write_log ("serial: ioctl TIOCMGET failed\n");
        return;
    }

    if (mask & TIOCM_DTR) {
        if(onoff) {
            status |= TIOCM_DTR;
        } else {
            status &= ~TIOCM_DTR;
        }
    }
    if (!currprefs.serial_hwctsrts) {
        if (mask & TIOCM_RTS) {
            if(onoff) {
                status |= TIOCM_RTS;
            } else {
                status &= ~TIOCM_RTS;
            }
        }
    }
    
    /* write control signals */
    if(ioctl( ser_fd, TIOCMSET, &status) < 0) {
        write_log ("serial: ioctl TIOCMSET failed\n");
    }
#endif
}

void serialuartbreak (int v)
{
    if (ser_fd < 0 || !currprefs.use_serial) {
        return;
    }
    
#ifdef POSIX_SERIAL
    if(v) {
        /* in posix serial calls we can't fulfill this function interface 
        completely: as we are not able to toggle the break mode with "v".
        We simply trigger a default break here if v is enabled... */
        if(tcsendbreak(ser_fd, 0) < 0) {
            write_log("serial: TCSENDBREAK failed\n");
        }
    }
#endif
}
