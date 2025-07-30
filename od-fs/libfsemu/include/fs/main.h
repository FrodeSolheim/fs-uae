// main function support for windows

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>

#undef main
// prevent later imports of SDL to overwrite main
#define _SDL_main_h

extern int g_fs_main_ncmdshow;
extern HINSTANCE g_fs_main_hinstance;

int fs_main_2(int argc, char* argv[]);

static inline int fs_main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    g_fs_main_hinstance = hInstance;
    g_fs_main_ncmdshow = nCmdShow;

    if (AttachConsole(-1) != 0) {
        freopen("CON", "wb", stdout);
        freopen("CON", "wb", stderr);
    }

    int argc = 0;
    wchar_t **wargv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (wargv == NULL) {
        OutputDebugStringA("CommandLineToArgvW returned NULL");
        return 1;
    }
    char **argv = (char **) malloc(sizeof(char *) * (argc + 1));
    for (int i = 0; i < argc; i++) {
        int flags = 0;
        int size = -1;
        int bytes = WideCharToMultiByte(
                CP_UTF8,                   // UINT CodePage,
                flags,                     // DWORD dwFlags,
                wargv[i],                  // LPCWSTR lpWideCharStr,
                size,                      // int cchWideChar,
                NULL,                      // LPSTR lpMultiByteStr,
                0,                         // int cbMultiByte,
                NULL,                      // LPCSTR lpDefaultChar,
                NULL);                     // LPBOOL lpUsedDefaultChar
        if (bytes == 0) {
            fs_log("error convering to utf-8\n");
            argv[i] = strdup("");
            continue;
        }
        char* buffer = (char*) (malloc(bytes + 1));
        bytes = WideCharToMultiByte(
                CP_UTF8,                   // UINT CodePage,
                flags,                     // DWORD dwFlags,
                wargv[i],                  // LPCWSTR lpWideCharStr,
                size,                      // int cchWideChar,
                buffer,                    // LPSTR lpMultiByteStr,
                bytes + 1,                 // int cbMultiByte,
                NULL,                      // LPCSTR lpDefaultChar,
                NULL);                     // LPBOOL lpUsedDefaultChar
        if (bytes == 0) {
            fs_log("error convering to utf-8\n");
            free(buffer);
            argv[i] = strdup("");
            continue;
        }
        argv[i] = buffer;
    }
    LocalFree(wargv);
    argv[argc] = NULL;
    return fs_main_2(argc, argv);
}

#define main(a, b) WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) { \
    return fs_main(hInstance, hPrevInstance, lpCmdLine, nCmdShow); \
} \
int g_fs_main_ncmdshow; \
HINSTANCE g_fs_main_hinstance; \
int fs_main_2(int argc, char* argv[])
#endif
