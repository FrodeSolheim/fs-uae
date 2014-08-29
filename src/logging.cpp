#include "uae/log.h"
#include <stdio.h>
#include <stdarg.h>

//UAEAPI void UAECALL uae_log(const char *format, ...)
void UAECALL uae_log(const char *format, ...)
{
	/* temp hack to be able to use UAE_LOG_VA_ARGS_FULL without
	 * getting a circular dependency */
#define uae_log write_log
	UAE_LOG_VA_ARGS_FULL(format);
}
