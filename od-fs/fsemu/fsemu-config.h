#ifndef FSEMU_CONFIG_H_
#define FSEMU_CONFIG_H_

// ----------------------------------------------------------------------------
// Diagnostics
// ----------------------------------------------------------------------------

// FIXME: RESPECT / CHECK -DDEBUG?
#define FSEMU_DEBUG 1
#define FSEMU_DEVELOPMENT 1

#ifdef FSEMU_INTERNAL
#pragma GCC diagnostic warning "-Wall"
// #pragma GCC diagnostic warning "-Wconversion"
#ifdef __APPLE__
// Not supported?
#else
#ifndef __cplusplus
#pragma GCC diagnostic warning "-Wdiscarded-qualifiers"
#endif
#endif
#pragma GCC diagnostic warning "-Wformat"
#ifndef __cplusplus
#pragma GCC diagnostic warning "-Wincompatible-pointer-types"
#endif
#pragma GCC diagnostic warning "-Wnarrowing"

#ifdef FSEMU_DEVELOPMENT
// #define FSEMU_DEBUG_ALL_WARNINGS_ARE_ERRORS 1
//  #ifdef FSEMU_INTERNAL
//  #endif
#pragma GCC diagnostic error "-Wall"
// #pragma GCC diagnostic error "-Wconversion"
#ifndef __cplusplus
#pragma GCC diagnostic error "-Wdiscarded-qualifiers"
#pragma GCC diagnostic error "-Wincompatible-pointer-types"
#endif
#pragma GCC diagnostic error "-Wformat"
#pragma GCC diagnostic error "-Wnarrowing"

#pragma GCC diagnostic warning "-Wunused-function"
#pragma GCC diagnostic warning "-Wunused-variable"
#pragma GCC diagnostic warning "-Wunused-but-set-variable"

// #pragma GCC diagnostic warning "-Wconversion"
// #pragma GCC diagnostic warning "-Wfloat-conversion"
// #pragma GCC diagnostic warning "-Wsign-conversion"
#endif  // FSEMU_DEVELOPMENT

#ifndef __cplusplus
#pragma GCC diagnostic error "-Wimplicit-function-declaration"
#endif

#endif  // FSEMU_INTERNAL

// ----------------------------------------------------------------------------
// OS defines
// ----------------------------------------------------------------------------

#ifdef _WIN32
#define FSEMU_OS_WINDOWS 1
#endif

#ifdef __APPLE__
#define FSEMU_OS_MACOS 1
#endif

#ifdef __linux__
#define FSEMU_OS_LINUX 1
#endif

// ----------------------------------------------------------------------------
// CPU defines
// ----------------------------------------------------------------------------

#ifdef __x86_64__
#define FSEMU_CPU_X86 1
#define FSEMU_CPU_X86_64 1
#define FSEMU_CPU_64_BIT 1
#elif defined(__i386__)
#define FSEMU_CPU_X86 1
#define FSEMU_CPU_X86_32 1
#define FSEMU_CPU_32_BIT 1
#endif

#ifdef __aarch64__
#define FSEMU_CPU_ARM 1
#define FSEMU_CPU_ARM_64 1
#define FSEMU_CPU_64_BIT 1
#elif defined(__arm__) || defined(__ARM_EABI__)
#define FSEMU_CPU_ARM 1
#define FSEMU_CPU_ARM_32 1
#define FSEMU_CPU_32_BIT 1
#endif

#if defined(FSEMU_CPU_ARM) && defined(FSEMU_OS_LINUX)
#define FSEMU_LINUX_ARM 1
#endif

#ifdef __ppc__
#define FSEMU_CPU_PPC 1
#define FSEMU_CPU_PPC_32 1
#define FSEMU_CPU_32_BIT 1
#endif

#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define FSEMU_CPU_BIGENDIAN 1
#elif defined(__BIG_ENDIAN__) || defined(__BIG_ENDIAN) || defined(_BIG_ENDIAN)
#define FSEMU_CPU_BIGENDIAN 1
#else
#define FSEMU_CPU_LITTLEENDIAN 1
#endif

// ----------------------------------------------------------------------------
// Feature defines
// ----------------------------------------------------------------------------

// #define FSEMU_GLAD 1
#define FSEMU_GLIB 1
// #define FSEMU_MANYMOUSE 1
#define FSEMU_OPENGL 1
#define FSEMU_PNG 1
#define FSEMU_SDL 1

#ifdef FSUAE
// FS-UAE adjusts audio frequency internally
#else
#define FSEMU_SAMPLERATE 1
#endif

/*
#ifdef FSEMU_OS_LINUX
#ifndef FSEMU_ALSA
#define FSEMU_ALSA
#endif
#endif
*/

// ----------------------------------------------------------------------------
// Other
// ----------------------------------------------------------------------------

#if defined(__GNUC__) || defined(__clang__)
#define FSEMU_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define FSEMU_DEPRECATED __declspec(deprecated)
#else
#define FSEMU_DEPRECATED
#endif

#if 1  // def HAVE___BUILTIN_EXPECT
#define fsemu_likely(x) (__builtin_expect(!!(x), 1))
#define fsemu_unlikely(x) (__builtin_expect(!!(x), 0))
#else
#define fsemu_likely(x) x
#define fsemu_unlikely(x) x
#endif

#define FSEMU_FLAG_NONE 0

#define FSEMU_PATH_MAX 4096

// ----------------------------------------------------------------------------
// Windows compatibility
// ----------------------------------------------------------------------------

#ifdef FSEMU_INTERNAL
#define __USE_MINGW_ANSI_STDIO 1
#endif

// ----------------------------------------------------------------------------

#include "fsemu-common.h"

#endif  // FSEMU_CONFIG_H_
