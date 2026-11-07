#ifndef UAE_LIMITS_H
#define UAE_LIMITS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

#if defined(_WIN32) && defined(FSUAE)
// Include Windows header files first to avoid warnings about re-defining MAX_PATH. Not for
// WinUAE: there sysdeps.h has already included <winsock.h>, which conflicts with winsock2.h.
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
