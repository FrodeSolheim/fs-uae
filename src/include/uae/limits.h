#ifndef UAE_LIMITS_H
#define UAE_LIMITS_H

#define MAX_DPATH 1000

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif

#endif // UAE_LIMITS_H
