#include "fsemu-common.h"
#include "fsemu-time.h"

#ifdef FSEMU_MACOS

// FIXME: Maybe?

#define clock_gettime fsemu_clock_gettime

#endif
