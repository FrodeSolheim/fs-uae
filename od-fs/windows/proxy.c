#ifdef _WIN32

#include <Windows.h>
#include <shlwapi.h>
#include <tchar.h>

#define PATH _T("Libs\\x86_64-w64-mingw32\\FS-UAE.exe")

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
    OutputDebugString(_T("FS-UAE proxy executable"));

    TCHAR szPath[MAX_PATH];
    GetModuleFileName(NULL, szPath, MAX_PATH);
    PathRemoveFileSpec(szPath);
    if ((MAX_PATH - _tcslen(szPath)) > _tcslen(_T("\\" PATH))) {
        _tcscat(szPath, _T("\\" PATH));
    }
    OutputDebugString(szPath);

    DWORD attrs = GetFileAttributes(szPath);
    if (attrs == INVALID_FILE_ATTRIBUTES || (attrs & FILE_ATTRIBUTE_DIRECTORY)) {
        MessageBox(NULL, _T("Did not find ") PATH, _T("FS-UAE"), MB_ICONERROR);
        return 1;
    }

    STARTUPINFOW si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    BOOL ok = CreateProcess(
        szPath,   // lpApplicationName
        GetCommandLine(), // lpCommandLine

        NULL,  // lpProcessAttributes
        NULL,  // lpThreadAttributes

        // Windows 7: STD_INPUT_HANDLE, STD_OUTPUT_HANDLE, and STD_ERROR_HANDLE are inherited
        // even when the parameter is FALSE.
        FALSE,  // bInheritHandles

        0,  // dwCreationFlags

        // If this parameter is NULL, the new process uses the environment of the calling process
        NULL,  // lpEnvironment

        NULL,  // lpCurrentDirectory
        &si, &pi);

    if (!ok) {
        MessageBox(NULL, _T("Error starting ") PATH, _T("FS-UAE"), MB_ICONERROR);
        return 1;
    }

    // We don't wait; just close handles and exit.
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    return 0;
}

#endif
