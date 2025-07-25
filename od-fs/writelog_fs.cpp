#include "sysconfig.h"
#include "sysdeps.h"

#include "uae.h"
#include "gui.h"
#include "compemu.h"
#include "uae/fs.h"
#include "uae/glib.h"
#include "custom.h"
#include "debug.h"

#include <sys/timeb.h>
#include <stdio.h>

#include "fsemu-mutex.h"
#include "libuae.h"

#include "uae/compat/windows.h"

//int log_scsi = 0;
//int log_net = 0;

void gui_message (const char *format,...)
{
    va_list args;
    va_start(args, format);
    char *buffer = g_strdup_vprintf(format, args);
	// Remove trailing newline, if any
	int len = strlen(buffer);
	if (len > 0 && buffer[len - 1] == '\n') {
		buffer[len - 1] = '\0';
	}
    va_end(args);
    if (libuae_cb_gui_message) {
        libuae_cb_gui_message(buffer);
    } else {
        printf("gui_message: '%s'\n", buffer);
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

#define SHOW_CONSOLE 0

static int nodatestamps = 0;

int consoleopen = 0;
static int realconsole;
static int bootlogmode;
static int cs_init;

FILE *debugfile = NULL;
int console_logging = 0;
static int debugger_type = -1;
extern int seriallog;
static int console_input_linemode = -1;
int always_flush_log = 1;

#ifdef _WIN32
static HANDLE stdinput,stdoutput;
static CRITICAL_SECTION cs;
extern BOOL debuggerinitializing;
#endif

#define WRITE_LOG_BUF_SIZE 4096

/* console functions for debugger */

#ifdef _WIN32

static HWND myGetConsoleWindow (void)
{
	return GetConsoleWindow ();
}

static void set_console_input_mode(int line)
{
	if (console_input_linemode < 0)
		return;
	if (line == console_input_linemode)
		return;
	SetConsoleMode (stdinput, ENABLE_PROCESSED_INPUT | ENABLE_PROCESSED_OUTPUT | (line ? (ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT) : 0));
	console_input_linemode = line;
}

static BOOL WINAPI ctrlchandler(DWORD ct)
{
	if (ct == CTRL_C_EVENT || ct == CTRL_CLOSE_EVENT) {
#ifdef FSUAE
#else
		systray(hHiddenWnd, TRUE);
#endif
	}
	return FALSE;
}

static void getconsole (void)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	stdinput = GetStdHandle (STD_INPUT_HANDLE);
	stdoutput = GetStdHandle (STD_OUTPUT_HANDLE);
	SetConsoleMode (stdinput, ENABLE_PROCESSED_INPUT|ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT|ENABLE_PROCESSED_OUTPUT);
	console_input_linemode = 1;
	SetConsoleCP (65001);
	SetConsoleOutputCP (65001);
	if (GetConsoleScreenBufferInfo (stdoutput, &csbi)) {
		if (csbi.dwMaximumWindowSize.Y < 5000) {
			csbi.dwMaximumWindowSize.Y = 5000;
			SetConsoleScreenBufferSize (stdoutput, csbi.dwMaximumWindowSize);
		}
	}
	SetConsoleCtrlHandler(ctrlchandler, TRUE);
}

static void flushmsgpump(void)
{
	MSG msg;
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

#endif

void deactivate_console(void)
{
#if 0
	if (previousactivewindow) {
		SetForegroundWindow(previousactivewindow);
		previousactivewindow = NULL;
	}
#endif
}

void activate_console (void)
{
	if (!consoleopen)
		return;
#ifdef _WIN32
	SetForegroundWindow (GetConsoleWindow ());
#else
	STUB("");
#endif
}

static void open_console_window (void)
{
#ifdef _WIN32
	AllocConsole ();
	getconsole ();
#endif
	consoleopen = -1;
	reopen_console ();
}

#ifdef FSUAE

#ifdef _WIN32
BOOL debuggerinitializing = FALSE;
#endif

static bool open_debug_window()
{
	STUB("");
	return false;
}

static void close_debug_window(void)
{
	STUB("");
}

static void WriteOutput(const TCHAR *out, int len)
{
	STUB("");
}

static int console_get_gui (TCHAR *out, int maxlen) {
	STUB("");
	return 0;
}

#endif

static void openconsole (void)
{
#ifdef _WIN32
	if (realconsole) {
		if (debugger_type == 2) {
			open_debug_window ();
			consoleopen = 1;
		} else {
			close_debug_window ();
			consoleopen = -1;
		}
		return;
	}
	if (debugger_active && (debugger_type < 0 || debugger_type == 2)) {
		if (consoleopen > 0 || debuggerinitializing)
			return;
		if (debugger_type < 0) {
#ifdef FSUAE
#else
			regqueryint (NULL, _T("DebuggerType"), &debugger_type);
#endif
			if (debugger_type <= 0)
				debugger_type = 1;
			openconsole();
			return;
		}
		close_console ();
		if (open_debug_window ()) {
			consoleopen = 1;
			return;
		}
		open_console_window ();
	} else {
		if (consoleopen < 0)
			return;
		close_console ();
		open_console_window ();
	}
#endif
}

void debugger_change (int mode)
{
	if (mode < 0)
		debugger_type = debugger_type == 2 ? 1 : 2;
	else
		debugger_type = mode;
	if (debugger_type != 1 && debugger_type != 2)
		debugger_type = 2;
#ifdef FSUAE
	// Not using the registry
#else
	regsetint (NULL, _T("DebuggerType"), debugger_type);
#endif
	openconsole ();
}

void open_console(void)
{
	if (!consoleopen) {
		openconsole();
	}
}

void reopen_console (void)
{
#ifdef _WIN32
	HWND hwnd;

	if (realconsole)
		return;
	if (consoleopen >= 0)
		return;
	hwnd = myGetConsoleWindow ();
	if (hwnd) {
		int newpos = 1;
		int x, y, w, h;
#ifdef FSUAE
		newpos = 0;
#else
		if (!regqueryint (NULL, _T("LoggerPosX"), &x))
			newpos = 0;
		if (!regqueryint (NULL, _T("LoggerPosY"), &y))
			newpos = 0;
		if (!regqueryint (NULL, _T("LoggerPosW"), &w))
			newpos = 0;
		if (!regqueryint (NULL, _T("LoggerPosH"), &h))
			newpos = 0;
#endif
		if (newpos) {
			RECT rc;
			rc.left = x;
			rc.top = y;
			rc.right = x + w;
			rc.bottom = y + h;
			if (MonitorFromRect (&rc, MONITOR_DEFAULTTONULL) != NULL) {
				SetForegroundWindow (hwnd);
				SetWindowPos (hwnd, HWND_TOP, x, y, w, h, SWP_NOACTIVATE);

			}
		}
	}
#endif
}

void close_console (void)
{
#ifdef _WIN32
	if (realconsole)
		return;
	if (consoleopen > 0) {
		close_debug_window ();
	} else if (consoleopen < 0) {
		HWND hwnd = myGetConsoleWindow ();
		if (hwnd && !IsIconic (hwnd)) {
			RECT r;
			if (GetWindowRect (hwnd, &r)) {
				r.bottom -= r.top;
				r.right -= r.left;
#ifdef FSUAE
#else
				regsetint (NULL, _T("LoggerPosX"), r.left);
				regsetint (NULL, _T("LoggerPosY"), r.top);
				regsetint (NULL, _T("LoggerPosW"), r.right);
				regsetint (NULL, _T("LoggerPosH"), r.bottom);
#endif
			}
		}
		FreeConsole ();
	}
#else
	STUB("");
#endif
	consoleopen = 0;
}

int read_log(void)
{
	STUB("");
	return -1;
}

static void writeconsole_2 (const TCHAR *buffer)
{
	DWORD temp;

	if (!consoleopen)
		openconsole ();

	if (consoleopen > 0) {
#ifdef _WIN32
		WriteOutput (buffer, _tcslen (buffer));
#endif
	} else if (realconsole) {
#ifdef FSUAE
		fputs (buffer, stdout);
#else
		fputws (buffer, stdout);
#endif
		fflush (stdout);
	} else if (consoleopen < 0) {
#ifdef _WIN32
		WriteConsole (stdoutput, buffer, _tcslen (buffer), &temp, 0);
#endif
	}
}

static void writeconsole (const TCHAR *buffer)
{
	if (_tcslen (buffer) > 256) {
		TCHAR *p = my_strdup (buffer);
		TCHAR *p2 = p;
		while (_tcslen (p) > 256) {
			TCHAR tmp = p[256];
			p[256] = 0;
			writeconsole_2 (p);
			p[256] = tmp;
			p += 256;
		}
		writeconsole_2 (p);
		xfree (p2);
	} else {
		writeconsole_2 (buffer);
	}
}

static void flushconsole (void)
{
	if (consoleopen > 0) {
		fflush (stdout);
	} else if (realconsole) {
		fflush (stdout);
	} else if (consoleopen < 0) {
#ifdef _WIN32
		FlushFileBuffers  (stdoutput);
#endif
	}
}

static TCHAR *console_buffer;
static int console_buffer_size;

TCHAR *setconsolemode (TCHAR *buffer, int maxlen)
{
	TCHAR *ret = NULL;
	if (buffer) {
		console_buffer = buffer;
		console_buffer_size = maxlen;
	} else { 
		ret = console_buffer;
		console_buffer = NULL;
	}
	return ret;
}

static void console_put (const TCHAR *buffer)
{
	if (console_buffer) {
		if (_tcslen (console_buffer) + _tcslen (buffer) < console_buffer_size)
			_tcscat (console_buffer, buffer);
	} else {
		openconsole ();
		writeconsole (buffer);
	}
}

static int console_buf_len = 100000;

void console_out_f (const TCHAR *format,...)
{
#ifdef _WIN32
	int len;
	va_list parms;
	TCHAR *pbuf;
	TCHAR buffer[WRITE_LOG_BUF_SIZE];
	TCHAR *bigbuf = NULL;

	pbuf = buffer;
	va_start (parms, format);
	len = _vsntprintf (pbuf, WRITE_LOG_BUF_SIZE - 1, format, parms);
	if (!len)
		return;
	if (len < 0 || len >= WRITE_LOG_BUF_SIZE - 2) {
		int buflen = console_buf_len;
		for (;;) {
			bigbuf = xmalloc(TCHAR, buflen);
			if (!bigbuf)
				return;
			len = _vsntprintf(bigbuf, buflen - 1, format, parms);
			if (!len)
				return;
			if (len > 0 && len < buflen - 2)
				break;
			xfree(bigbuf);
			buflen += 100000;
			if (buflen > 10000000)
				return;
		}
		pbuf = bigbuf;
		console_buf_len = buflen;
	}
	va_end (parms);
	console_put (pbuf);
	if (bigbuf)
		xfree(bigbuf);
#else
    va_list arg_ptr;
    va_start(arg_ptr, format);
    vprintf(format, arg_ptr);
    va_end(arg_ptr);

#endif
}

void console_out (const TCHAR *txt)
{
#ifdef _WIN32
	console_put (txt);
#else
	printf("%s", txt);
#endif
}

bool console_isch (void)
{
#ifdef _WIN32
	flushmsgpump();
	if (console_buffer) {
		return 0;
	} else if (realconsole) {
		return false;
	} else if (consoleopen < 0) {
		DWORD events = 0;
		GetNumberOfConsoleInputEvents (stdinput, &events);
		return events > 0;
	}
	return false;
#else
	STUB("");
	return false;
#endif
}

TCHAR console_getch (void)
{
#ifdef _WIN32
	flushmsgpump();
	if (console_buffer) {
		return 0;
	} else if (realconsole) {
		return getwc (stdin);
	} else if (consoleopen < 0) {
		DWORD len;
		TCHAR out[2];
		
		for (;;) {
			out[0] = 0;
			ReadConsole (stdinput, out, 1, &len, 0);
			if (len > 0)
				return out[0];
		}
	}
	return 0;
#else
	STUB("");
	return 0;
#endif
}

int console_get (TCHAR *out, int maxlen)
{
#ifdef _WIN32
	*out = 0;

	flushmsgpump();
	set_console_input_mode(1);
	if (consoleopen > 0) {
		return console_get_gui (out, maxlen);
	} else if (realconsole) {
		DWORD totallen;

		*out = 0;
		totallen = 0;
		while (maxlen > 0) {
			*out = getwc (stdin);
			if (*out == 13)
				break;
			out++;
			maxlen--;
			totallen++;
		}
		*out = 0;
		return totallen;
	} else if (consoleopen < 0) {
		DWORD len, totallen;

		*out = 0;
		totallen = 0;
		while(maxlen > 0) {
			ReadConsole (stdinput, out, 1, &len, 0);
			if(*out == 13)
				break;
			out++;
			maxlen--;
			totallen++;
		}
		*out = 0;
		return totallen;
	}
	return 0;
#else
	TCHAR *res = fgets(out, maxlen, stdin);
	if (res == NULL) {
		return -1;
	}
	int len = strlen(out);
	return len - 1;
#endif
}

void console_flush (void)
{
#ifdef _WIN32
	flushconsole ();
#else
	fflush(stdout);
#endif
}

TCHAR *write_log_get_ts(void)
{
	struct tm *t;
#ifdef _WIN32
	struct timeb tb;
#else
	struct _timeb tb;
#endif
	static TCHAR out[100];
	TCHAR *p;
	static TCHAR lastts[100];
	TCHAR curts[100];

#if 0
	if (bootlogmode)
		return NULL;
#endif
	if (nodatestamps)
		return NULL;
	if (!vsync_counter)
		return NULL;
	ftime (&tb);
	t = localtime (&tb.time);
	strftime (curts, sizeof curts / sizeof (TCHAR), _T("%Y-%m-%d %H:%M:%S\n"), t);
	p = out;
	*p = 0;
#if 0
    _stprintf (p, _T("XX-XXX"));
    p += _tcslen (p);
#else
	if (_tcsncmp (curts, lastts, _tcslen (curts) - 3)) { // "xx\n"
		_tcscat (p, curts);
		p += _tcslen (p);
		_tcscpy (lastts, curts);
	}
	strftime (p, sizeof out / sizeof (TCHAR) - (p - out) , _T("%S-"), t);
	p += _tcslen (p);
	_stprintf (p, _T("%03d"), tb.millitm);
	p += _tcslen (p);
#endif
	if (vsync_counter != 0xffffffff)
		_stprintf (p, _T(" [%lu %03d%s%03d]"), vsync_counter, current_hpos_safe (), lof_store ? _T("-") : _T("="), vpos);
	_tcscat (p, _T(": "));
	return out;
}

void write_logx(const char *format, ...)
{
    // FIXME
}

#include <glib/gprintf.h>
#define MAX_LINE 4068

void write_log (const TCHAR *format, ...)
{
	// write_log is sometimes called multiple times for a single line. We
	// do line buffering here to get complete lines passed on further. We use
	// a mutex to make this function somewhat thread-safe, but line buffering
	// can still get mixed content from different threads. Should ideally
	// use thread-local storage for line buffer.
	static fsemu_mutex *mutex;
	if (mutex == NULL) {
		mutex = fsemu_mutex_create();
	}

	fsemu_mutex_lock(mutex);

    va_list args;
    va_start(args, format);
    // char *buffer = g_strdup_vprintf(format, args);

	static char buffer[MAX_LINE];
	static int partial;

	if (partial == 0) {
		// strcpy(buffer, "[ UAE ] ");
		// partial = 8;
		// strcpy(buffer, "[ UAE ] [     ] ");
		g_snprintf(buffer, 4096, "[%03d] ", vpos);
		partial = 6;
		// g_snprintf(buffer, 4096, "[UAE/%03d] ", vpos);
		// partial = 10;
	}

	int result = g_vsnprintf(buffer + partial, MAX_LINE - partial, format, args);
    va_end(args);

	// FIXME: Take a closer look at this code
	int len = strlen(buffer + partial);
	// printf("partial %d len %d: %s\n", partial, len, buffer);
	if (len > 0 && buffer[partial + len - 1] != '\n') {
		if (partial + len >= MAX_LINE - 1) {
			// We need to flush this
			buffer[MAX_LINE - 2] = '\n';
		} else {
			partial += len;
			fsemu_mutex_unlock(mutex);
			return;
		}
	}

	// Remove trailing newline
	buffer[partial + len - 1] = '\0';

    char *buffer2 = NULL;
#if 0
    TCHAR *ts = write_log_get_ts();
    // printf("%s\n", ts ? ts : "");
    if (ts) {
        buffer2 = g_strdup_printf("%s%s", ts, buffer);
    }
#endif

    // log_function function = g_libamiga_callbacks.log;
	// FIXME
	// function = NULL;
    if (libuae_cb_write_log) {
        if (buffer2) {
            libuae_cb_write_log(buffer2);
        } else {
            libuae_cb_write_log(buffer);
        }
    } else {
        printf("write_log: '%s'\n", buffer);
    }
    // free(buffer);
    if (buffer2) {
        free(buffer2);
    }

	partial = 0;
	fsemu_mutex_unlock(mutex);
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
