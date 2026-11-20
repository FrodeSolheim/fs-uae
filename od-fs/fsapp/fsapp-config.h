/*
 * fsapp's compiler attributes and debug switches.
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSAPP_CONFIG_H
#define FSAPP_CONFIG_H

#include <stdbool.h>
#include <stdint.h>

// #define FSAPP_DEBUG_CLEANUP

#if defined(__GNUC__) || defined(__clang__)
#define FSAPP_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define FSAPP_DEPRECATED __declspec(deprecated)
#else
#define FSAPP_DEPRECATED
#endif

// For functions taking a NULL-terminated list of arguments: warns when the NULL is missing.
#if defined(__GNUC__) || defined(__clang__)
#define FSAPP_SENTINEL __attribute__((sentinel))
#else
#define FSAPP_SENTINEL
#endif

#define fsapp_likely(x) (__builtin_expect(!!(x), 1))
#define fsapp_unlikely(x) (__builtin_expect(!!(x), 0))

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* FSAPP_CONFIG_H */
