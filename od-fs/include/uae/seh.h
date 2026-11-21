/*
 * Structured exception handling
 *
 * Copyright (c) 2014 Frode Solheim
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef UAE_SEH_H
#define UAE_SEH_H

#ifdef _MSC_VER

/* Structured exception handling is available */

#else

/* MinGW defines __try / __except */
#undef __try
#undef __except

/* Structured exception handling is not available - do nothing */
#define __try
#define __except(x)

#endif

#endif /* UAE_SEH_H */
