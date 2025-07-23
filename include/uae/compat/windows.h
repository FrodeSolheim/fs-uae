
#ifndef UAE_COMPAT_WINDOWS_H
#define UAE_COMPAT_WINDOWS_H

/*
 * Define Windows compatibility functions used by WinUAE throughout the source.
 * For example, _byteswap_ulong is used directly in the modified PCem source
 * code.
 *
 * Copyright (c) 2025 Frode Solheim
 */

#include "uae/byteswap.h"
#include "uae/log.h"
#include "uae/tchar.h"

#include <stddef.h>
#include <stdint.h>

#define _byteswap_ushort uae_bswap16
#define _byteswap_ulong uae_bswap32

// This allows some (Windows-only) declarations in multi-platform code without
// having to ifdef those away.

// Windows calling convention modifiers

#define WINAPI
#define CALLBACK

// Common Windows API types

typedef void VOID;

typedef unsigned char BYTE;

typedef BYTE BOOLEAN;

typedef int BOOL;

typedef uint16_t WORD;
typedef uint32_t DWORD;

typedef int INT;

typedef unsigned int UINT;
typedef uint8_t UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
typedef uint64_t UINT64;

typedef int32_t LONG;

typedef uint32_t ULONG;
typedef uint32_t ULONG32;
typedef uint64_t ULONG64;

typedef size_t SIZE_T;

typedef UINT *PUINT;
typedef ULONG *PULONG;
typedef void *LPVOID;
typedef void *PVOID;

typedef uintptr_t UINT_PTR;
typedef uintptr_t ULONG_PTR;
typedef intptr_t LONG_PTR;

typedef ULONG_PTR DWORD_PTR;
typedef ULONG_PTR *PULONG_PTR;

typedef PVOID HANDLE;

typedef HANDLE HBRUSH;
typedef HANDLE HDC;
typedef HANDLE HICON;
typedef HANDLE HINSTANCE;
typedef HANDLE HKEY;
typedef HANDLE HMONITOR;
typedef HANDLE HWND;

typedef HINSTANCE HMODULE;
typedef HICON HCURSOR;
typedef DWORD COLORREF;
typedef LONG HRESULT;

// typedef unsigned short WORD;

typedef LONG_PTR LPARAM;
typedef UINT_PTR WPARAM;

typedef wchar_t WCHAR;

typedef struct _LUID {
  DWORD LowPart;
  LONG  HighPart;
} LUID, *PLUID;

typedef struct _GUID {
  uint32_t Data1;
  uint16_t Data2;
  uint16_t Data3;
  uint8_t  Data4[8];
} GUID;

typedef struct tagRECT {
  int32_t left;
  int32_t top;
  int32_t right;
  int32_t bottom;
} RECT, *PRECT, *NPRECT, *LPRECT;

typedef struct _OSVERSIONINFOW {
  DWORD dwOSVersionInfoSize;
  DWORD dwMajorVersion;
  DWORD dwMinorVersion;
  DWORD dwBuildNumber;
  DWORD dwPlatformId;
  WCHAR szCSDVersion[128];
} OSVERSIONINFOW, *POSVERSIONINFOW, *LPOSVERSIONINFOW, RTL_OSVERSIONINFOW, *PRTL_OSVERSIONINFOW;

#define OSVERSIONINFO OSVERSIONINFOW

static inline BOOL ResetEvent(HANDLE hEvent)
{
    UAE_LOG_STUB("");
    return false;
}

typedef void* CRITICAL_SECTION;
typedef CRITICAL_SECTION* LPCRITICAL_SECTION;

static inline void InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{

}

static inline void EnterCriticalSection (LPCRITICAL_SECTION lpCriticalSection)
{

}

static inline void LeaveCriticalSection (LPCRITICAL_SECTION lpCriticalSection)
{

}

#define TIME_ONESHOT 0x000
#define TIME_CALLBACK_FUNCTION 0x0000

typedef void ( CALLBACK *LPTIMECALLBACK)(
   UINT      uTimerID,
   UINT      uMsg,
   DWORD_PTR dwUser,
   DWORD_PTR dw1,
   DWORD_PTR dw2
);

typedef int MMRESULT;

MMRESULT timeSetEvent(
   UINT           uDelay,
   UINT           uResolution,
   LPTIMECALLBACK lpTimeProc,
   DWORD_PTR      dwUser,
   UINT           fuEvent
);

#endif // UAE_COMPAT_WINDOWS_H
