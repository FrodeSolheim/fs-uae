
#ifndef UAE_COMPAT_WINDOWS2_H
#define UAE_COMPAT_WINDOWS2_H

#include "uae/compat/windows.h"

typedef intptr_t INT_PTR;

typedef uint64_t ULONGLONG;

typedef LONG_PTR LRESULT;

typedef const WCHAR *LPCWSTR;
typedef DWORD *LPDWORD;

typedef HANDLE HDROP;
typedef HANDLE HFONT;
typedef HANDLE HMENU;
typedef HANDLE HTHEME;

typedef INT_PTR CALLBACK (*DLGPROC)(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

typedef struct _SYSTEM_INFO {
  union {
    DWORD dwOemId;
    struct {
      WORD wProcessorArchitecture;
      WORD wReserved;
    } DUMMYSTRUCTNAME;
  } DUMMYUNIONNAME;
  DWORD     dwPageSize;
  LPVOID    lpMinimumApplicationAddress;
  LPVOID    lpMaximumApplicationAddress;
  DWORD_PTR dwActiveProcessorMask;
  DWORD     dwNumberOfProcessors;
  DWORD     dwProcessorType;
  DWORD     dwAllocationGranularity;
  WORD      wProcessorLevel;
  WORD      wProcessorRevision;
} SYSTEM_INFO, *LPSYSTEM_INFO;

typedef enum _MINIDUMP_TYPE {
  MiniDumpNormal = 0x00000000,
#if 0
  MiniDumpWithDataSegs = 0x00000001,
  MiniDumpWithFullMemory = 0x00000002,
  MiniDumpWithHandleData = 0x00000004,
  MiniDumpFilterMemory = 0x00000008,
  MiniDumpScanMemory = 0x00000010,
  MiniDumpWithUnloadedModules = 0x00000020,
  MiniDumpWithIndirectlyReferencedMemory = 0x00000040,
  MiniDumpFilterModulePaths = 0x00000080,
  MiniDumpWithProcessThreadData = 0x00000100,
  MiniDumpWithPrivateReadWriteMemory = 0x00000200,
  MiniDumpWithoutOptionalData = 0x00000400,
  MiniDumpWithFullMemoryInfo = 0x00000800,
  MiniDumpWithThreadInfo = 0x00001000,
  MiniDumpWithCodeSegs = 0x00002000,
  MiniDumpWithoutAuxiliaryState = 0x00004000,
  MiniDumpWithFullAuxiliaryState = 0x00008000,
  MiniDumpWithPrivateWriteCopyMemory = 0x00010000,
  MiniDumpIgnoreInaccessibleMemory = 0x00020000,
  MiniDumpWithTokenInformation = 0x00040000,
  MiniDumpWithModuleHeaders = 0x00080000,
  MiniDumpFilterTriage = 0x00100000,
  MiniDumpWithAvxXStateContext = 0x00200000,
  MiniDumpWithIptTrace = 0x00400000,
  MiniDumpScanInaccessiblePartialPages = 0x00800000,
  MiniDumpFilterWriteCombinedMemory,
  MiniDumpValidTypeFlags = 0x01ffffff,
  MiniDumpNoIgnoreInaccessibleMemory,
  MiniDumpValidTypeFlagsEx
#endif
} MINIDUMP_TYPE;

// Bogus typedefs

typedef void *LPCDLGTEMPLATE;
typedef void *LPCDLGTEMPLATEW;

typedef void *LPDIRECT3DSURFACE9;

#endif // UAE_COMPAT_WINDOWS2_H
