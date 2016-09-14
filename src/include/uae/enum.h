#ifndef UAE_ENUM_H
#define UAE_ENUM_H

/* Make C-style use of enums compatible with ints in C++: in C++ you cannot
 * assign an enum value to an int */

#define ENUMDECL enum
#define ENUMNAME(name) ; typedef int name ;

#endif /* UAE_ENUM_H */
