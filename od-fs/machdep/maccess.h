#if 1

#include "uae/maccess.h"

#else
#ifdef __ARM_EABI__
#include "machdep/arm/maccess.h"
#elif __ppc__
#include "machdep/ppc/maccess.h"
#elif defined(__i386__) || defined(__x86_64__)
#include "machdep/x86/maccess.h"
#else
#include "machdep/generic/maccess.h"
#endif
#endif
