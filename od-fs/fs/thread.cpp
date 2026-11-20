/*
 * Thread utilities that complement SDL's threading API, and telling the system that the
 * program's timing is latency-critical. See thread.h.
 *
 * This module has no dependencies on other FS modules.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "fs/thread.h"

#include <SDL3/SDL.h>

#if !SDL_VERSION_ATLEAST(3, 6, 0)
#if defined(_WIN32)
#include <windows.h>
#else
#include <pthread.h>
#endif // defined(_WIN32)
#if defined(__linux__)
#include <fcntl.h>
#include <sys/auxv.h>
#include <unistd.h>
#endif // defined(__linux__)
#endif // !SDL_VERSION_ATLEAST(3, 6, 0)

#if defined(SDL_PLATFORM_MACOS)
#include <dlfcn.h>
#include <pthread.h>
#include <pthread/qos.h>
#endif // defined(SDL_PLATFORM_MACOS)

#if !SDL_VERSION_ATLEAST(3, 6, 0)

// The longest name kept, in bytes, with the terminating zero (a longer one is cut).
#define THREAD_NAME_BYTES 256

// The calling thread's name, looked up on the first call. Plain arrays, so that they are never
// destroyed while the thread runs (also while exit() runs, on the thread that called it).
static thread_local char t_thread_name[THREAD_NAME_BYTES];
static thread_local bool t_thread_name_looked_up;

// Cuts the text at its first sequence that is not valid UTF-8, such as the start of a character
// that the operating system's length limit cut (Linux keeps 15 bytes of a name).
static void CutAtInvalidUTF8(char *text)
{
    const char *next = text;
    while (*next != '\0') {
        const char *start = next;
        Uint32 codepoint = SDL_StepUTF8(&next, NULL);
        // SDL_StepUTF8 returns SDL_INVALID_UNICODE_CODEPOINT (U+FFFD) for an invalid sequence,
        // and for U+FFFD itself, which is valid (EF BF BD).
        bool replacement = next - start == 3 && SDL_memcmp(start, "\xEF\xBF\xBD", 3) == 0;
        if (codepoint == SDL_INVALID_UNICODE_CODEPOINT && !replacement) {
            text[start - text] = '\0';
            return;
        }
    }
}

// -------------------------------------------------------------------------------------------------
// Windows: the thread's description.

#if defined(_WIN32)
typedef HRESULT(WINAPI *GetThreadDescriptionFunction)(HANDLE, PWSTR *);

// GetThreadDescription, from Windows 10 1607 on (NULL before).
static GetThreadDescriptionFunction FindGetThreadDescription(void)
{
    const wchar_t *module_names[] = {L"kernel32.dll", L"KernelBase.dll"};
    for (size_t i = 0; i < SDL_arraysize(module_names); i++) {
        HMODULE module = GetModuleHandleW(module_names[i]);
        if (module != NULL) {
            void *function = (void *)GetProcAddress(module, "GetThreadDescription");
            if (function != NULL) {
                return (GetThreadDescriptionFunction)function;
            }
        }
    }
    return NULL;
}

// The calling thread's description (which SDL sets to the name a thread was created with) in
// UTF-8 (invalid UTF-16 replaced), cut at a character boundary to fit the buffer; empty when there
// is none.
static void GetOperatingSystemThreadName(char *name, size_t size)
{
    static GetThreadDescriptionFunction get_thread_description = FindGetThreadDescription();
    if (get_thread_description == NULL) {
        return;
    }
    PWSTR description = NULL;
    if (FAILED(get_thread_description(GetCurrentThread(), &description)) || description == NULL) {
        return;
    }
    // Converted whole, then cut, so that a long description is cut rather than lost.
    int utf8_size = WideCharToMultiByte(CP_UTF8, 0, description, -1, NULL, 0, NULL, NULL);
    char *utf8 = utf8_size > 0 ? (char *)SDL_malloc(utf8_size) : NULL;
    if (utf8 != NULL && WideCharToMultiByte(CP_UTF8, 0, description, -1, utf8, utf8_size, NULL,
                                            NULL) == utf8_size) {
        SDL_utf8strlcpy(name, utf8, size);
    }
    SDL_free(utf8);
    LocalFree(description);
}
#endif // defined(_WIN32)

// -------------------------------------------------------------------------------------------------
// Linux: a thread that was not named has the name of the thread that created it.

#if defined(__linux__)
// The longest name Linux keeps for a thread, in bytes (TASK_COMM_LEN, 16, with the terminating
// zero).
#define THREAD_LINUX_NAME_BYTES 16

// The program's name in the operating system: the main thread's name, which Linux gives every
// thread that was not named. A plain struct, so that it is never destroyed (a thread may look up
// its name while exit() runs).
typedef struct ProcessName {
    char name[64];
} ProcessName;

static ProcessName ReadProcessName(void)
{
    ProcessName process_name = {};
    // The main thread's name, as it is now (/proc/self/comm, the name of the process's first
    // thread), with the newline that Linux ends it with (a name may have newlines of its own).
    int file = open("/proc/self/comm", O_RDONLY | O_CLOEXEC);
    ssize_t length = -1;
    if (file >= 0) {
        length = read(file, process_name.name, sizeof(process_name.name) - 1);
        close(file);
    }
    if (length > 0) {
        if (process_name.name[length - 1] == '\n') {
            length--;
        }
        process_name.name[length] = '\0';
        return process_name;
    }
    // Without /proc: the name Linux gives the program when it starts, the file name of the path it
    // was executed from (AT_EXECFN, which exec -a does not change, unlike argv[0]), cut to what
    // Linux keeps.
    const char *path = (const char *)getauxval(AT_EXECFN);
    if (path != NULL) {
        const char *slash = SDL_strrchr(path, '/');
        SDL_strlcpy(process_name.name, slash != NULL ? slash + 1 : path, THREAD_LINUX_NAME_BYTES);
    }
    return process_name;
}

static const char *GetProcessName(void)
{
    static const ProcessName process_name = ReadProcessName();
    return process_name.name;
}
#endif // defined(__linux__)

// -------------------------------------------------------------------------------------------------
// Linux and macOS: the thread's name, which SDL sets with pthread_setname_np.

#if defined(__linux__) || defined(__APPLE__)
// The calling thread's name, cut at a character boundary to fit the buffer; empty when it has
// none. On macOS, a thread that was not named has no name.
static void GetOperatingSystemThreadName(char *name, size_t size)
{
    char os_name[64] = "";
    if (pthread_getname_np(pthread_self(), os_name, sizeof(os_name)) != 0) {
        return;
    }
#if defined(__linux__)
    // A thread that was not named has the name of the thread that created it. Created by the main
    // thread (or by another thread without a name of its own), that is the program's name, which a
    // named thread does not have: it counts as no name. Created by a named thread (the emulation
    // thread, the UI thread), it is that thread's name, as Linux does not tell a name that was set
    // from one that was inherited.
    if (SDL_strcmp(os_name, GetProcessName()) == 0) {
        return;
    }
#endif // defined(__linux__)
    SDL_utf8strlcpy(name, os_name, size);
}
#endif // defined(__linux__) || defined(__APPLE__)

// -------------------------------------------------------------------------------------------------

// Looks up the calling thread's name in the operating system into the buffer, in UTF-8, cut to fit
// at a character boundary; empty when it has none (or on a system without a lookup).
static void LookUpThreadName(char *name, size_t size)
{
    name[0] = '\0';
#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)
    GetOperatingSystemThreadName(name, size);
#endif // defined(_WIN32) || defined(__linux__) || defined(__APPLE__)
    CutAtInvalidUTF8(name);
}

#endif // !SDL_VERSION_ATLEAST(3, 6, 0)

const char *FS_GetCurrentThreadName(void)
{
#if SDL_VERSION_ATLEAST(3, 6, 0)
    // REVIEW: Not built yet: SDL 3.6 is not out. Check what it returns on Linux for a thread that
    // was not named: if it is the program's name (inherited from the main thread), which the
    // lookup for an older SDL counts as no name, the log shows it instead of the thread's id.
    return SDL_GetCurrentThreadName();
#else
    if (!t_thread_name_looked_up) {
        LookUpThreadName(t_thread_name, sizeof(t_thread_name));
        t_thread_name_looked_up = true;
    }
    return t_thread_name[0] != '\0' ? t_thread_name : NULL;
#endif
}

bool FS_SetCurrentThreadInteractive(void)
{
#if defined(SDL_PLATFORM_MACOS)
    int error = pthread_set_qos_class_self_np(QOS_CLASS_USER_INTERACTIVE, 0);
    if (error != 0) {
        return SDL_SetError("pthread_set_qos_class_self_np failed (error %d)", error);
    }
#endif
    return true;
}

#if defined(SDL_PLATFORM_MACOS)

// NSActivityUserInitiatedAllowingIdleSystemSleep | NSActivityLatencyCritical (NSProcessInfo.h; the
// values are part of the API).
#define LATENCY_CRITICAL_ACTIVITY_OPTIONS (0x00efffffULL | 0xff00000000ULL)

// Kept until the process ends, which ends the activity.
static void *g_latency_critical_activity;

// The Objective-C runtime is called through dlsym: it is loaded in every process using Cocoa, but
// FS-UAE is not linked with libobjc (and has no Objective-C sources).
static bool BeginActivity(void)
{
    typedef void *(*GetClassFunction)(const char *);
    typedef void *(*RegisterNameFunction)(const char *);
    GetClassFunction get_class = (GetClassFunction)dlsym(RTLD_DEFAULT, "objc_getClass");
    RegisterNameFunction register_name =
        (RegisterNameFunction)dlsym(RTLD_DEFAULT, "sel_registerName");
    void *msg_send = dlsym(RTLD_DEFAULT, "objc_msgSend");
    if (get_class == NULL || register_name == NULL || msg_send == NULL) {
        return SDL_SetError("The Objective-C runtime is not loaded");
    }
    // objc_msgSend must be called through a pointer of each method's own type. A message to nil
    // (a class not found) returns nil.
    typedef void *(*SendFunction)(void *, void *);
    typedef void *(*SendStringFunction)(void *, void *, const char *);
    typedef void *(*SendActivityFunction)(void *, void *, unsigned long long, void *);
    SendFunction send = (SendFunction)msg_send;

    // The objects returned are autoreleased: the pool releases them (but the activity, retained).
    void *pool =
        send(send(get_class("NSAutoreleasePool"), register_name("alloc")), register_name("init"));
    void *process_info = send(get_class("NSProcessInfo"), register_name("processInfo"));
    void *reason = ((SendStringFunction)msg_send)(get_class("NSString"),
                                                  register_name("stringWithUTF8String:"),
                                                  "Emulation with real-time video and audio");
    if (process_info != NULL && reason != NULL) {
        g_latency_critical_activity = ((SendActivityFunction)msg_send)(
            process_info, register_name("beginActivityWithOptions:reason:"),
            LATENCY_CRITICAL_ACTIVITY_OPTIONS, reason);
        if (g_latency_critical_activity != NULL) {
            send(g_latency_critical_activity, register_name("retain"));
        }
    }
    send(pool, register_name("drain"));
    if (g_latency_critical_activity == NULL) {
        return SDL_SetError("NSProcessInfo did not begin the activity");
    }
    return true;
}

#endif // defined(SDL_PLATFORM_MACOS)

bool FS_BeginLatencyCriticalActivity(void)
{
#if defined(SDL_PLATFORM_MACOS)
    if (g_latency_critical_activity == NULL) {
        return BeginActivity();
    }
#endif
    return true;
}
