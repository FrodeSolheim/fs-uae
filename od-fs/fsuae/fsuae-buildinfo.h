/*
 * The build's identity: its version and origin, and the build header at the top of the log file.
 *
 * Copyright (c) 2026 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef FSUAE_BUILDINFO_H
#define FSUAE_BUILDINFO_H

#ifdef __cplusplus
extern "C" {
#endif

// The version FS-UAE shows (FSUAE_VERSION in configure.ac), for example "5.0.8".
const char *fsuae_buildinfo_get_version(void);

// "FS-UAE version <version>", as FS-UAE logs it at start-up and as the first line of the build
// header.
const char *fsuae_buildinfo_get_version_string(void);

// Who made the build (FSUAE_BUILD_ORIGIN in configure.ac), for example "Unofficial build".
const char *fsuae_buildinfo_get_origin(void);

// Logs the build header: the version, origin, commit, build date, triplet, compiler, the SDL and
// Python versions FS-UAE is built with and runs with, and the build environment. Called first thing
// in main, right after the log is set up, so that it is at the top of the log file.
void fsuae_buildinfo_log_header(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* FSUAE_BUILDINFO_H */
