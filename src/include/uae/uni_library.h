/*
* UAE - The Un*x Amiga Emulator
*
* UAE Native Interface (UNI)
*
* Copyright 2013-2014 Frode Solheim
*/

#ifndef _UAE_UNI_LIBRARY_H_
#define _UAE_UNI_LIBRARY_H_

#if defined(_WIN32) || defined(WINDOWS)

    #ifdef __cplusplus
    #define UNIAPI __declspec(dllexport)
    #else
    #define UNIAPI __declspec(dllexport) extern "C"
    #endif
    #define UNICALL __cdecl

#else // _WIN32 not defined

    #ifdef __cplusplus
    #define UNIAPI extern "C" __attribute__((__visibility__("default")))
    #else
    #define UNIAPI __attribute__((__visibility__("default")))
    #endif
    #define UNICALL

#endif

#include "uni_common.h"

/*
#define UNI_PVOID_PARAM(index) uni->resolve(uni->params[index])

#define UNI_PCHAR_PARAM(index) ((uni_char *) uni->resolve(uni->params[index]))
#define UNI_PUCHAR_PARAM(index) ((uni_uchar *) uni->resolve(uni->params[index]))
#define UNI_PSHORT_PARAM(index) ((uni_short *) uni->resolve(uni->params[index]))
#define UNI_PUSHORT_PARAM(index) ((uni_ushort *) uni->resolve(uni->params[index]))
#define UNI_PLONG_PARAM(index) ((uni_long *) uni->resolve(uni->params[index]))
#define UNI_PULONG_PARAM(index) ((uni_ulong *) uni->resolve(uni->params[index]))

#define UNI_CHAR_PARAM(index) ((uni_char)uni->params[index])
#define UNI_UCHAR_PARAM(index) ((uni_uchar)uni->params[index])
#define UNI_SHORT_PARAM(index) ((uni_short)uni->params[index])
#define UNI_USHORT_PARAM(index) ((uni_ushort)uni->params[index])
#define UNI_LONG_PARAM(index) ((uni_long)uni->params[index])
#define UNI_ULONG_PARAM(index) ((uni_ulong)uni->params[index])
*/

#define UNI_PVOID_PARAM(reg) uni->resolve(uni->reg)

#define UNI_PCHAR_PARAM(reg) ((uni_char *) uni->resolve(uni->reg))
#define UNI_PUCHAR_PARAM(reg) ((uni_uchar *) uni->resolve(uni->reg))
#define UNI_PSHORT_PARAM(reg) ((uni_short *) uni->resolve(uni->reg))
#define UNI_PUSHORT_PARAM(reg) ((uni_ushort *) uni->resolve(uni->reg))
#define UNI_PLONG_PARAM(reg) ((uni_long *) uni->resolve(uni->reg))
#define UNI_PULONG_PARAM(reg) ((uni_ulong *) uni->resolve(uni->reg))

#define UNI_CHAR_PARAM(reg) ((uni_char)uni->reg)
#define UNI_UCHAR_PARAM(reg) ((uni_uchar)uni->reg)
#define UNI_SHORT_PARAM(reg) ((uni_short)uni->reg)
#define UNI_USHORT_PARAM(reg) ((uni_ushort)uni->reg)
#define UNI_LONG_PARAM(reg) ((uni_long)uni->reg)
#define UNI_ULONG_PARAM(reg) ((uni_ulong)uni->reg)

#ifdef __BIG_ENDIAN__

    #define UNI_SWAP_SHORT(x) (x)
    #define UNI_SWAP_LONG(x)  (x)

#else

    #define UNI_SWAP_SHORT(x) (((x) & 0xff00) >> 8 | ((x) & 0x00ff) << 8)
    #define UNI_SWAP_LONG(x)  (((x) & 0xff000000) >> 24 | \
                               ((x) & 0x00ff0000) >>  8 | \
                               ((x) & 0x0000ff00) <<  8 | \
                               ((x) & 0x000000ff) << 24)

#endif

//#define UNI_CHAR(x) (x)
//#define UNI_PTR(x) uni->resolve((uae_u32)UNI_LONG((x)))
//#define UNI_BEGIN_AMIGA_STRUCTS #pragma pack(2)
//#define UNI_END_AMIGA_STRUCTS #pragma pack()

typedef void (*uni_user_init_function)(struct uni *uni);

#define UNI_DEFINE_LIBRARY(library_version, init_function) \
uni_long g_library_version = library_version; \
void uni_init_2(struct uni *uni, uni_user_init_function function) { \
    if (uni->uni_version < UNI_MIN_VERSION) { \
        uni->error = UNI_ERROR_INTERFACE_TOO_OLD; \
    } \
    else if (function != NULL) { \
        function(uni); \
    } \
} \
UNIAPI void UNICALL uni_init(struct uni *uni) { \
    uni_init_2(uni, (init_function)); \
} \
UNIAPI uni_long UNICALL uni_get_library_version(void) { \
    return g_library_version; \
}

// extern struct uni *g_uni;

#endif // _UAE_UNI_LIBRARY_H_
