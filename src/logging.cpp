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
	if (g_timestamp) {
		double diff_ms = (fsemu_time_us() - g_timestamp) / 1000.0;
		write_log("[UAE] %03d:%03d +%5.2f ", (int) (vsync_counter % 1000), vpos, diff_ms);
	} else {
		write_log("[UAE] %03d:%03d        ", (int) (vsync_counter % 1000), vpos);
	}
#endif
	/* Redirect UAE_LOG_VA_ARGS_FULL to use write_log instead */
#define uae_log write_log
	UAE_LOG_VA_ARGS_FULL(format);
#undef uae_log
}

#ifdef FSUAE

void UAECALL uae_log_reset_timestamp(void)
{
	g_timestamp = fsemu_time_us();
}

#endif
