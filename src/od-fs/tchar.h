#ifndef LIBAMIGA_TCHAR_H_
#define LIBAMIGA_TCHAR_H_

// we include this very early, so later #defines of _w* functions is defined
// _after_ wchar.h is processed

#include <wchar.h>

// libamiga only uses chars and strings in UTF-8 format internally

typedef char TCHAR;
#define _T(x) x
//typedef char WCHAR
//#define WCHAR char

#endif // LIBAMIGA_TCHAR_H_
