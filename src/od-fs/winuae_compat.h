#ifndef _UAE_OD_FS_WINUAE_COMPAT_H_
#define _UAE_OD_FS_WINUAE_COMPAT_H_

#ifdef WINDOWS

// Include windef.h now to get RECT and DWORD defined (and not collide with
// later includes of windows.h

#include "windef.h"
#include "windows.h"

// Some WinUAE-derived code which must not be used is guarded by _WIN32
// defines. The code is fixed so compiling without _WIN32 defined works
// when compiling FS-UAE for Windows. FS-UAE code use the WINDOWS define
// instead to avoid collision with WinUAE.

#undef _WIN32
#undef WIN32

#endif

// Use custom versions of these functions for platform-specific behaviour,
// for example uae_fopen may perform charset conversion before opening the
// file.

#define _tfopen uae_fopen
#define _ftelli64 uae_ftello64
#define _fseeki64 uae_fseeko64

// convert windows libc names to standard libc function names, and also
// use char functions instead of wchar string functions.

#define _stprintf sprintf
#define _wunlink unlink
#define _tcscspn strcspn
#define _tcscmp strcmp
#define _tcsncmp strncmp
#define _tcslen strlen
#define _tcscpy strcpy
#define _tcsncpy strncpy
#define _tcsdup strdup
#define _tcscat strcat
#define _tcsncat strncat
#define _tcsspn strspn
#define _tcsicmp strcasecmp
#define _tcsnicmp strncasecmp
#define stricmp strcasecmp
#define strnicmp strncasecmp
#define _tcsrchr strrchr
#define _tcschr strchr
#define _istdigit isdigit
#define _istspace isspace
#define _istupper isupper
#define _tcsstr strstr
#define _tcsftime strftime
#define _tcsftime strftime
#define _tstol atol
#define _tstof atof
#define _tcstod strtod
#define _tcstol strtol
#define _strtoui64 strtoll
#define _totupper toupper
#define _totlower tolower
#define _tcstok strtok
#define _tstoi atoi
#define _tstoi64 atoll
#define _vsntprintf vsnprintf
#define _vsnprintf vsnprintf
#define _sntprintf snprintf
#define _tprintf printf

//#define _timezone timezone
//#define _daylight daylight
#ifdef WINDOWS

#else
extern int _timezone;
extern int _daylight;
#endif

#define _tzset tzset

#define _istalnum isalnum

// needed by e.g drawing.cpp

#define NOINLINE

#ifndef WINDOWS

#define _ftime ftime
#define _timeb timeb

#define _cdecl

#ifndef ULONG
#define ULONG unsigned long
#endif

//typedef unsigned int UAE_DWORD;
typedef unsigned int DWORD;

typedef struct tagRECT {
    int left;
    int top;
    int right;
    int bottom;
} RECT, *PRECT, *PPRECT;

//#ifndef WINDOWS
//#define DWORD UAE_DWORD
//#define RECT UAE_RECT
//#endif
#endif

#define STATIC_INLINE static inline

#endif // _UAE_OD_FS_WINUAE_COMPAT_H_
