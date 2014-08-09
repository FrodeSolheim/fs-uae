/* 
 *	HT Editor
 *	stdfuncs.h
 *
 *	Copyright (C) 2003 Stefan Weyergraf
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License version 2 as
 *	published by the Free Software Foundation.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __STDFUNCS_H__
#define __STDFUNCS_H__

#include "debugger.h"

#define FUNCTION_MAX_DECL_PARAMS	8

typedef Function *(*FunctionCreator)();

struct FunctionDesc {
	const char *name;
	int declparam_count;
	EvalType declparam[FUNCTION_MAX_DECL_PARAMS];
	FunctionCreator creator;
};

extern FunctionDesc gStdEvalFunctions[];

#endif /* __STDFUNCS_H__ */
