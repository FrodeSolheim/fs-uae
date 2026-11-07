#ifdef _WIN32

// The proxy is the FS-UAE.exe at the top of a Windows dist. It starts the real executable in
// Libs\<triplet>\ and stands in for it: the real executable gets the proxy's standard handles
// (stdin, stdout, stderr), the proxy waits for it to exit and returns its exit code, and if the
// proxy is killed, the real executable is killed too. Like the real executable, the proxy is a GUI
// program (-mwindows), so starting it from Explorer does not open a console window.

#include <windows.h>
#include <shlwapi.h>
#include <string.h>
#include <tchar.h>

#define PATH _T("Libs\\x86_64-w64-mingw32\\FS-UAE.exe")
#define PATH_A "Libs\\x86_64-w64-mingw32\\FS-UAE.exe"

static void error(const char* message_a, const TCHAR* message) {
    // Write to stderr when there is one (terminal, redirect, Wine), and also show a message box,
    // which is the only way to see the error when started from Explorer.
    HANDLE err = GetStdHandle(STD_ERROR_HANDLE);
    if (err != NULL && err != INVALID_HANDLE_VALUE) {
        DWORD written;
        WriteFile(err, "FS-UAE: ", 8, &written, NULL);
        WriteFile(err, message_a, (DWORD)strlen(message_a), &written, NULL);
        WriteFile(err, "\r\n", 2, &written, NULL);
    }
    MessageBox(NULL, message, _T("FS-UAE"), MB_ICONERROR);
}

// Returns the handle if it is valid and could be made inheritable, otherwise NULL.
static HANDLE inheritable_std_handle(DWORD which) {
    HANDLE handle = GetStdHandle(which);
    if (handle == NULL || handle == INVALID_HANDLE_VALUE) {
        return NULL;
    }
    if (!SetHandleInformation(handle, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT)) {
        return NULL;
    }
    return handle;
}

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
        error("Did not find " PATH_A, _T("Did not find ") PATH);
        return 1;
    }

    // The real executable is put in a job that is killed when the proxy exits or is killed (the
    // job handle is closed then). Processes that FS-UAE starts itself (a browser opened with
    // SDL_OpenURL, ...) silently break away from the job, so they are not killed with it.
    HANDLE job = CreateJobObject(NULL, NULL);
    if (job != NULL) {
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION info;
        ZeroMemory(&info, sizeof(info));
        info.BasicLimitInformation.LimitFlags =
            JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE | JOB_OBJECT_LIMIT_SILENT_BREAKAWAY_OK;
        if (!SetInformationJobObject(job, JobObjectExtendedLimitInformation, &info,
                                     sizeof(info))) {
            CloseHandle(job);
            job = NULL;
        }
    }

    // Pass on the proxy's standard handles, so output goes to the same terminal, pipe or file
    // as if the real executable had been started directly. When the proxy has no standard
    // handles (started from Explorer), the real executable gets none either.
    HANDLE std_in = inheritable_std_handle(STD_INPUT_HANDLE);
    HANDLE std_out = inheritable_std_handle(STD_OUTPUT_HANDLE);
    HANDLE std_err = inheritable_std_handle(STD_ERROR_HANDLE);

    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = std_in;
    si.hStdOutput = std_out;
    si.hStdError = std_err;

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    BOOL ok = CreateProcess(
        szPath,            // lpApplicationName
        GetCommandLine(),  // lpCommandLine
        NULL,              // lpProcessAttributes
        NULL,              // lpThreadAttributes
        TRUE,              // bInheritHandles (only the standard handles are inheritable)
        // Suspended until it has been put in the job, so that processes it starts right away
        // are not missed.
        CREATE_SUSPENDED,  // dwCreationFlags
        NULL,              // lpEnvironment (the proxy's environment)
        NULL,              // lpCurrentDirectory (the proxy's current directory)
        &si, &pi);

    if (!ok) {
        error("Error starting " PATH_A, _T("Error starting ") PATH);
        return 1;
    }

    // Can fail, e.g. on Windows 7 if the proxy is already in a job; the real executable then
    // simply runs without it.
    if (job != NULL) {
        AssignProcessToJobObject(job, pi.hProcess);
    }
    ResumeThread(pi.hThread);
    CloseHandle(pi.hThread);

    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD exit_code = 1;
    GetExitCodeProcess(pi.hProcess, &exit_code);
    CloseHandle(pi.hProcess);
    if (job != NULL) {
        CloseHandle(job);
    }
    return (int)exit_code;
}

#endif
