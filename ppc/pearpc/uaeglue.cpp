
#include <cstring>
#include <cstdio>
#include <stdarg.h>

#include "system/systhread.h"
#ifdef FSUAE
#include "tools/debug.h"
#include "tools/snprintf.h"
#endif

#include "uae/log.h"

int ht_printf(const char *format,...)
{
	UAE_LOG_VA_ARGS_FULL(format);
	return 0;
}
int ht_fprintf(FILE *f, const char *fmt, ...)
{
	return 0;
}
int ht_vfprintf(FILE *file, const char *fmt, va_list args)
{
	return 0;
}
int ht_snprintf(char *str, size_t count, const char *fmt, ...)
{
	return 0;
}
int ht_vsnprintf(char *str, size_t count, const char *fmt, va_list args)
{
	return 0;
}

void ht_assert_failed(const char *file, int line, const char *assertion)
{
#ifdef FSUAE
	uae_log("ht_assert_failed %s:%d %s\n", file, line, assertion);
	//uae_abort("ht_assert_failed %s:%d %s\n", file, line, assertion);
	abort();
#endif
}

#if 0
void prom_quiesce()
{
}
#endif

#ifdef FSUAE
#include <threaddep/sem.h>
typedef uae_sem_t sys_mutex;
#else
#include "sysconfig.h"
#include "sysdeps.h"
#include <threaddep/thread.h>

typedef void * sys_mutex;
#endif

int sys_lock_mutex(sys_mutex m)
{
	uae_sem_wait(&m);
	return 1;
}

void sys_unlock_mutex(sys_mutex m)
{
	uae_sem_post(&m);
}

int sys_create_mutex(sys_mutex *m)
{
	if (!(*m))
		uae_sem_init(m, 0, 1);
	return 1;
}

void sys_destroy_mutex(sys_mutex m)
{
	uae_sem_destroy(&m);
}
