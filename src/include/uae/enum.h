#ifndef UAE_ENUM_H
#define UAE_ENUM_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// make use of enums compatible with ints in C++: in C++ you cannot assign
// an enum value to an int

#define ENUMDECL enum
#define ENUMNAME(name) ; typedef int name ;

#endif // UAE_ENUM_H
