/*
 * The build's identity: its version and origin, and the build header at the top of the log file.
 *
 * The build header tells from a user's FS-UAE.log.txt which build they ran: the version, who made
 * the build (the origin), from which commit, when, for which triplet, with which compiler and
 * library versions, and in what environment. Everything but the versions FS-UAE runs with is known
 * at build time, from configure (fsuae-build.h, config.h) and the compiler, and is one C string in
 * the executable, which the dist check (od-fs/build/dist/check.py) reads without running it. The
 * origin is self-declared, not proof of where a build comes from: that is what the macOS signature
 * and the release's SHA256SUMS are for.
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "fsuae-buildinfo.h"

#include <Python.h>
#include <SDL3/SDL.h>
#include <string.h>

#include "fsuae-build.h"

// The version string. The dist check (od-fs/build/dist/check.py) looks for it in the executable, as
// a C string of its own, to check that the program has the dist's version, so keep it
// "FS-UAE version <version>".
#define FSUAE_VERSION_STRING "FS-UAE version " FSUAE_VERSION

#define FSUAE_BUILDINFO_STR2(x) #x
#define FSUAE_BUILDINFO_STR(x) FSUAE_BUILDINFO_STR2(x)

#if defined(__clang__) && defined(__apple_build_version__)
#define FSUAE_COMPILER "Apple Clang " __clang_version__
#elif defined(__clang__)
#define FSUAE_COMPILER "Clang " __clang_version__
#elif defined(__GNUC__)
#define FSUAE_COMPILER "GCC " __VERSION__
#elif defined(_MSC_VER)
#define FSUAE_COMPILER "MSVC " FSUAE_BUILDINFO_STR(_MSC_FULL_VER)
#else
#define FSUAE_COMPILER "unknown"
#endif

#define FSUAE_BUILT_WITH_SDL                                                                       \
    FSUAE_BUILDINFO_STR(SDL_MAJOR_VERSION)                                                         \
    "." FSUAE_BUILDINFO_STR(SDL_MINOR_VERSION) "." FSUAE_BUILDINFO_STR(SDL_MICRO_VERSION)

// The part of the build header that is known at build time, as one C string, which the dist check
// (BUILD_HEADER_PATTERN in od-fs/build/dist/check.py) finds in the executable and checks: the first
// line is FSUAE_VERSION_STRING, the others are indented "<name>: <value>" lines. The check reads
// the version, Origin, Commit and Triplet lines; keep their form.
static const char g_build_header[] =
    FSUAE_VERSION_STRING "\n"
                         "  Origin: " FSUAE_BUILD_ORIGIN "\n"
                         "  Commit: " FSUAE_BUILD_COMMIT "\n"
                         "  Build date: " FSUAE_BUILD_DATE "\n"
                         "  Triplet: " HOST_TRIPLET "\n"
                         "  Compiler: " FSUAE_COMPILER "\n"
                         "  Built with: SDL " FSUAE_BUILT_WITH_SDL ", Python " PY_VERSION "\n"
                         "  Build environment: " FSUAE_BUILD_ENVIRONMENT;

static const char g_version_string[] = FSUAE_VERSION_STRING;

const char *fsuae_buildinfo_get_version(void)
{
    return FSUAE_VERSION;
}

const char *fsuae_buildinfo_get_version_string(void)
{
    return g_version_string;
}

const char *fsuae_buildinfo_get_origin(void)
{
    return FSUAE_BUILD_ORIGIN;
}

void fsuae_buildinfo_log_header(void)
{
    // The versions of the libraries FS-UAE runs with, which can differ from those it is built with
    // when they come from the system (Linux distribution packages). Py_GetVersion can be called
    // before Python is initialized; its version is the first word.
    int sdl_version = SDL_GetVersion();
    const char *python_version = Py_GetVersion();
    int python_version_length = (int)strcspn(python_version, " ");
    SDL_Log("%s\n  Running with: SDL %d.%d.%d, Python %.*s", g_build_header,
            SDL_VERSIONNUM_MAJOR(sdl_version), SDL_VERSIONNUM_MINOR(sdl_version),
            SDL_VERSIONNUM_MICRO(sdl_version), python_version_length, python_version);
}
