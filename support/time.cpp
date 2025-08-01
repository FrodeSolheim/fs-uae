#include "sysconfig.h"
#include "sysdeps.h"
#include "uae/time.h"
#include "custom.h"
#include "options.h"
#include "events.h"
#include "uae.h"

#ifdef WINUAE
#define USE_LEGACY_WIN32_TIME_FUNCTIONS 1
#else
#define USE_GLIB_TIME_FUNCTIONS 1
#endif

#ifdef USE_GLIB_TIME_FUNCTIONS

#include <glib.h>

static gint64 uae_time_epoch;

uae_time_t uae_time(void)
{
	int64_t t = g_get_monotonic_time() - uae_time_epoch;
	// Will overflow in 49.71 days. Whether that is a problem depends on usage.
	// Should go through all old uses of read_processor_time / uae_time and
	// make sure to use overflow-safe code or move to 64-bit timestamps.
	return (uae_time_t) t;
}

// Since GLib 2.53.3, g_get_monotonic times uses QueryPerformanceCounter on
// Windows. So we can now use this on all platforms.
// I don't think the value can be negative, but resolution is high enough that
// we don't need to care about signed/unsigned, so we just use int64_t.

int64_t uae_time_us(void)
{
	// We subtrach epoch here so that this function uses the same epoch as
	// the 32-bit uae_time legacy function. Maybe not necessary.
	return g_get_monotonic_time() - uae_time_epoch;
}

#if 0
// Don't think we need this kind of precision in slirp_uae
int64_t uae_time_ns(void)
{
#ifdef _WIN32

else
	// FIXME: Should probably copy time functions from Glib if neccessary to
	// get nanosecond precision, so that we can use the macOS nanosecond
	// implementation, and then use the same code for the usec one (except
	// dividing by 1000) so that the timers are compatible (same epoch).
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec * 1000000000LL + ts.tv_nsec;
#endif
}
#endif

void uae_time_calibrate(void)
{

}

void uae_time_init(void)
{
	if (uae_time_epoch == 0) {
		uae_time_epoch = g_get_monotonic_time();
	}
}

#endif // USE_GLIB_TIME_FUNCTIONS

#ifdef USE_LEGACY_WIN32_TIME_FUNCTIONS

#include <process.h>

static int userdtsc = 0;
static int qpcdivisor = 0;
static SYSTEM_INFO si;

static frame_time_t read_processor_time_qpf(void)
{
	LARGE_INTEGER counter;
	frame_time_t t;
	QueryPerformanceCounter(&counter);
	if (qpcdivisor == 0)
		t = (frame_time_t) (counter.QuadPart);
	else
		t = (frame_time_t) (counter.QuadPart >> qpcdivisor);
	if (!t)
		t++;
	return t;
}

uae_s64 read_processor_time_rdtsc(void)
{
#ifdef __arm__
	return read_processor_time_qpf();
#else
	return __rdtsc();
#endif
}

uae_time_t uae_time(void)
{
	uae_time_t t;
#if 0
	static int cnt;

	cnt++;
	if (cnt > 1000000) {
		write_log(_T("**************\n"));
		cnt = 0;
	}
#endif
	if (userdtsc)
		t = read_processor_time_rdtsc();
	else
		t = read_processor_time_qpf();
	return t;
}

uae_s64 read_system_time(void)
{
	return GetTickCount64();
}

static volatile int dummythread_die;

static void _cdecl dummythread(void *dummy)
{
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
	while (!dummythread_die);
}

static uae_s64 win32_read_processor_time(void)
{
#if defined(X86_MSVC_ASSEMBLY)
	uae_u32 foo, bar;
	__asm
	{
		cpuid
			rdtsc
			mov foo, eax
			mov bar, edx
	}
	return (((uae_s64)bar) << 32) | foo;
#else
	return 0;
#endif
}

static void figure_processor_speed_rdtsc(void)
{
	static int freqset;
	frame_time_t clockrate;
	int oldpri;
	HANDLE th;

	if (freqset)
		return;
	th = GetCurrentThread ();
	freqset = 1;
	oldpri = GetThreadPriority(th);
	SetThreadPriority(th, THREAD_PRIORITY_HIGHEST);
	dummythread_die = -1;
	_beginthread(&dummythread, 0, 0);
	sleep_millis(500);
	clockrate = win32_read_processor_time();
	sleep_millis(500);
	clockrate = (win32_read_processor_time() - clockrate) * 2;
	dummythread_die = 0;
	SetThreadPriority(th, oldpri);
	write_log(_T("CLOCKFREQ: RDTSC %.2fMHz\n"), clockrate / 1000000.0);
	syncbase = clockrate >> 6;
}

static void figure_processor_speed_qpf(void)
{
	LARGE_INTEGER freq;
	static LARGE_INTEGER freq2;
	uae_u64 qpfrate;

	if (!QueryPerformanceFrequency (&freq))
		return;
	if (freq.QuadPart == freq2.QuadPart)
		return;
	freq2.QuadPart = freq.QuadPart;
	qpfrate = freq.QuadPart;
	/* limit to 10MHz */
	qpcdivisor = 0;
	while (qpfrate > 10000000) {
		qpfrate >>= 1;
		qpcdivisor++;
	}
	write_log(_T("CLOCKFREQ: QPF %.2fMHz (%.2fMHz, DIV=%d)\n"),
		  freq.QuadPart / 1000000.0,
		  qpfrate / 1000000.0, 1 << qpcdivisor);
	syncbase = qpfrate;
}

void uae_time_calibrate(void)
{
	if (userdtsc) {
		figure_processor_speed_rdtsc();
	}
	if (!userdtsc) {
		figure_processor_speed_qpf();
	}
}

void uae_time_use_rdtsc(bool enable)
{
	userdtsc = enable;
}

void uae_time_init(void)
{
	static bool initialized = false;
	if (initialized) {
		return;
	}
#ifdef _WIN32
	GetSystemInfo(&si);
#endif
	uae_time_calibrate();
	initialized = true;
}

#endif // USE_WIN32_TIME_FUNCTIONS

#ifdef FSUAE

void uae_deterministic_amiga_time(int *days, int *mins, int *ticks)
{
	// FIXME: Would be nice if the netplay server could broadcast a suitable
	// start time.
	// FIXME: Also integrate this with battery clock emulation
	// FIXME: This works quite well for PAL (ticks = 1/50 sec). Should tune for
	// NTSC...

	long t = vsync_counter;

	int ticks_per_min = 50 * 60;
	int ticks_per_day = 24 * 60 * ticks_per_min;

	*days = t / ticks_per_day;
	t -= *days * ticks_per_day;
	*mins = t / ticks_per_min;
	t -= *mins * ticks_per_min;
	*ticks = t;
	// Start at day one so time looks more valid for certain programs?
	// *days += 1;
}

#endif
