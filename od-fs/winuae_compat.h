#ifndef UAE_OD_FS_WINUAE_COMPAT_H
#define UAE_OD_FS_WINUAE_COMPAT_H

#include <stdint.h>

#ifdef _WIN32

// Include windef.h now to get RECT and DWORD defined (and not collide with
// later includes of windows.h

#include <winsock2.h>
#include <windows.h>
#include <windef.h>

#else

// Used by createstatusline declaration in statusline.h; defining it as void *
// for now to let code compile.
typedef void * HWND;

#endif

// Use custom versions of these functions for platform-specific behaviour,
// for example uae_tfopen may perform charset conversion before opening the
// file.

// #define _tfopen uae_tfopen
#define _ftelli64 uae_ftello64
#define _fseeki64 uae_fseeko64

// convert windows libc names to standard libc function names, and also
// use char functions instead of wchar string functions.

#include "uae/string.h"

#define stricmp strcasecmp
#define strnicmp strncasecmp

#define _wunlink unlink

//#define _timezone timezone
//#define _daylight daylight

#ifdef _WIN32

#ifndef UNICODE
// Shared WinUAE code calls swscanf_s with TCHAR strings, but TCHAR is char here, so map it to the
// char version as on other platforms. Include the headers which declare the real (wchar_t)
// swscanf_s first, so the macro does not rename those declarations.
// FIXME: The proper fix is for the caller (debug.cpp, which passes a _T() format string) to use
// the TCHAR variant _stscanf_s instead (swscanf_s with UNICODE, sscanf_s without), preferably
// upstream in WinUAE. Then this mapping can go, and the swscanf_s mapping in the non-Windows
// branch below can be replaced with "#define _stscanf_s sscanf" (MinGW's tchar.h already
// provides _stscanf_s).
#include <stdio.h>
#include <wchar.h>
#define swscanf_s sscanf
#endif

#else
extern int _timezone;
extern int _daylight;

#define _tzset tzset
#define _istalnum isalnum

// Names used by (newer) shared WinUAE code which od-unix/tchar.h and
// od-unix/sysconfig.h provide in the upstream unix port.
#define _tfopen fopen
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#define swscanf_s sscanf

#endif

// needed by e.g drawing.cpp

// #define NOINLINE

#ifndef WINDOWS

#define _ftime ftime
#define _timeb timeb

#define _cdecl

typedef uint32_t ULONG;

//#include "uae/compat/windows.h"

//typedef unsigned int UAE_DWORD;
// typedef unsigned int DWORD;

// typedef struct tagRECT {
//     int left;
//     int top;
//     int right;
//     int bottom;
// } RECT, *PRECT, *PPRECT;

//#ifndef WINDOWS
//#define DWORD UAE_DWORD
//#define RECT UAE_RECT
//#endif
#endif

// #define STATIC_INLINE static inline
#include "uae/inline.h"

#endif // UAE_OD_FS_WINUAE_COMPAT_H
