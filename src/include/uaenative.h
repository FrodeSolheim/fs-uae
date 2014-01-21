/*
* UAE - The Un*x Amiga Emulator
*
* UAE Native Interface (UNI)
*
* Copyright 2013-2014 Frode Solheim
*/

#ifndef _UAE_UAENATIVE_H_
#define _UAE_UAENATIVE_H_

#ifdef WITH_UAENATIVE

#if defined(_WIN32) || defined(WINDOWS)

    #define UNIAPI __declspec(dllimport)
    #define UNICALL __cdecl

#else // _WIN32 not defined

    #define UNIAPI
    #define UNICALL

#endif

#include "uae/uni_common.h"

#define UNI_FLAG_ASYNCHRONOUS 1
#define UNI_FLAG_COMPAT 2
#define UNI_FLAG_NAMED_FUNCTION 4

uae_u32 uaenative_open_library(TrapContext *context, int flags);
uae_u32 uaenative_get_function(TrapContext *context, int flags);
uae_u32 uaenative_call_function(TrapContext *context, int flags);
uae_u32 uaenative_close_library(TrapContext *context, int flags);

void *uaenative_get_uaevar(void);

void uaenative_install ();
uaecptr uaenative_startup (uaecptr resaddr);

/* This function must return a list of directories to look for native
 * libraries in. The returned list must be NULL-terminated, and must not
 * be de-allocated. */
const char **uaenative_get_library_dirs(void);

// the function prototype for the callable native functions
typedef void UNICALL (*uae_uni_native_function)(struct uni *uni);

// the function prototype for the callable native functions (old style)
typedef uae_u32 UNICALL (*uae_uni_native_compat_function)(uae_u32, uae_u32,
        uae_u32, uae_u32, uae_u32, uae_u32, uae_u32, uae_u32, uae_u32,
        uae_u32, uae_u32, uae_u32, void *, uae_u32, void *);

// the function prototype for the native library's uni_init function
typedef void UNICALL (*uni_init_function)(struct uni *uni);

#endif // WITH_UAENATIVE

#endif // _UAE_UAENATIVE_H_
