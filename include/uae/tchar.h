#ifndef UAE_TCHAR_H
#define UAE_TCHAR_H

#ifdef _WIN32
#include <tchar.h>
#ifdef UNICODE
#define SIZEOF_TCHAR 2
#else
#define SIZEOF_TCHAR 1
#endif
#else
typedef char TCHAR;
#define SIZEOF_TCHAR 1
#endif

#ifdef _WIN32

#include <tchar.h>
#include <wchar.h>
#include <stdlib.h>

#ifdef UNICODE
#define SIZEOF_TCHAR 2
#else
#define SIZEOF_TCHAR 1
#endif

#else

typedef char TCHAR;
#define SIZEOF_TCHAR 1

#define _fgetts fgets
#define _fputts fputs
#define _istdigit isdigit
#define _istspace isspace
#define _istupper isupper
#define _istxdigit isxdigit
#define _sntprintf snprintf
#define _stprintf sprintf
#define _stscanf sscanf
#define _tcscat strcat
#define _tcschr strchr
#define _tcscmp strcmp
#define _tcscpy strcpy
#define _tcscspn strcspn
#define _tcsdup strdup
#define _tcsftime strftime
#define _tcsftime strftime
#define _tcsicmp strcasecmp
#define _tcslen strlen
#define _tcsncat strncat
#define _tcsncmp strncmp
#define _tcsncpy strncpy
#define _tcsnicmp strncasecmp
#define _tcsrchr strrchr
#define _tcsspn strspn
#define _tcsstr strstr
#define _tcstod strtod
#define _tcstok strtok
#define _tcstol strtol
#define _tcstoul strtoul 
#define _totlower tolower
#define _totupper toupper
#define _tprintf printf
#define _tstof atof
#define _tstoi atoi
#define _tstoi64 atoll
#define _tstol atol
#define _vsntprintf vsnprintf

#endif

#ifndef _T
#if SIZEOF_TCHAR == 1
#define _T(x) x
#else
#define _T(x) Lx
#endif
#endif

#endif /* UAE_TCHAR_T */
