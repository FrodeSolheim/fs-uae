#ifndef FSLIB_OS_H
#define FSLIB_OS_H

#include "fslib-config.h"

#ifdef _WIN32
#define FSLIB_OS_WINDOWS 1
#endif

#ifdef __APPLE__
#define FSLIB_OS_MACOS 1
#endif

#ifdef __linux__
#define FSLIB_OS_LINUX 1
#endif

#endif // FSLIB_OS_H
