#ifndef UAE_LIMITS_H
#define UAE_LIMITS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

#ifdef _WIN32
// Include Windows header files first to avoid warnings about re-defining MAX_PATH
#include "winsock2.h"
#include "windows.h"
#endif

#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif

#ifndef MAX_DPATH
#ifdef PATH_MAX
#define MAX_DPATH PATH_MAX
#else
// WinUAE define from od-win32/sysconfig.h
#define MAX_DPATH 1000
#endif
#endif

#endif /* UAE_LIMITS_H */
