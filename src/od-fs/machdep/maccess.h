#ifdef __ARM_EABI__
#include "machdep/arm/maccess.h"
#elif __ppc__
#include "machdep/ppc/maccess.h"
#else
#include "machdep/x86/maccess.h"
#endif
