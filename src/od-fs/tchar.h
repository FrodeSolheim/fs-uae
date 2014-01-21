#ifndef _UAE_OD_FS_TCHAR_H_
#define _UAE_OD_FS_TCHAR_H_

// we include this very early, so later #defines of _w* functions is defined
// _after_ wchar.h is processed (because _w* string functions are #defined
// to their char counterparts)

#include <wchar.h>

// libamiga only uses chars and strings in UTF-8 format internally

typedef char TCHAR;
#define _T(x) x

#endif // _UAE_OD_FS_TCHAR_H_
