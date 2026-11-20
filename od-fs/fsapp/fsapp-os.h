/*
 * The operating system the program is built for (FSAPP_OS_*).
 *
 * Copyright (c) 2025-2026 Frode Solheim
 */

#ifndef FSAPP_OS_H
#define FSAPP_OS_H

#include "fsapp-config.h"

#ifdef _WIN32
#define FSAPP_OS_WINDOWS 1
#endif

#ifdef __APPLE__
#define FSAPP_OS_MACOS 1
#endif

#ifdef __linux__
#define FSAPP_OS_LINUX 1
#endif

#endif /* FSAPP_OS_H */
