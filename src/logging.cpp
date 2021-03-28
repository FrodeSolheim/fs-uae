/*
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
*/
#include "sysconfig.h"
#include "sysdeps.h"

#include "uae/log.h"
#include "custom.h"

#include <stdio.h>
#include <stdarg.h>

#ifdef FSUAE
#include <fs/emu/hacks.h>
#include "fsemu-time.h"
static int64_t g_timestamp;
#endif

void UAECALL uae_log(const char *format, ...)
{
#ifdef FSUAE
	// if (g_timestamp) {
	//	double diff_ms = (fsemu_time_us() - g_timestamp) / 1000.0;
	//	write_log("%03d:%03d +%5.2f ", (int) (vsync_counter % 1000), vpos, diff_ms);
	//} else {
		write_log("%03d:%03d        ", (int) (vsync_counter % 1000), vpos);
	//}
#endif
	/* Redirect UAE_LOG_VA_ARGS_FULL to use write_log instead */
#define uae_log write_log
	UAE_LOG_VA_ARGS_FULL(format);
#undef uae_log
}

#ifdef FSUAE

int uae_frametrace_log_enabled = 0;
char *uae_frametrace_prefix = NULL;

static int uae_frametrace_frame = -1;
static FILE *uae_frametrace_file = NULL;

void uae_frametrace_init(void)
{
	if (g_getenv("UAE_FRAMETRACE_PREFIX")) {
		uae_frametrace_prefix = strdup(g_getenv("UAE_FRAMETRACE_PREFIX"));
	} else {
		uae_frametrace_prefix = strdup("frametrace/");
	}
}

void uae_frametrace_enable(void)
{
	uae_frametrace_log_enabled = 1;
	if (uae_frametrace_prefix == NULL) {
		uae_frametrace_init();
	}
}

void uae_frametrace_log_va_args(const char *format, va_list ap)
{
	// if (uae_frametrace_prefix = NULL) {
	// 	uae_frametrace_init();
	// }
	int frame = (int) vsync_counter;

	if (frame >= 752 && frame <= 769) {
		// continue;
	} else {
		return;
	}

	if (frame != uae_frametrace_frame) {
		// New frame
		if (uae_frametrace_file != NULL) {
			fclose(uae_frametrace_file);
		}
		char buffer[MAX_PATH];
		snprintf(buffer, MAX_PATH, "%s%d.log", uae_frametrace_prefix, frame);
		// printf("%s\n", buffer);
		uae_frametrace_file = fopen(buffer, "wb");
		uae_frametrace_frame = frame;
	}

	if (uae_frametrace_file != NULL) {
		vfprintf(uae_frametrace_file, format, ap);
	}
}

void uae_frametrace_log_impl(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	// vfprintf(uae_frametrace_file, format, ap);
	uae_frametrace_log_va_args(format, ap);
	va_end(ap);
}

void UAECALL uae_log_reset_timestamp(void)
{
	g_timestamp = fsemu_time_us();
}

#endif
