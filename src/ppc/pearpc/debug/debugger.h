/* 
 *	HT Editor
 *	debugger.h
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

#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__

//#define HAVE_DEBUGGER

#include "tools/data.h"
#include "tools/str.h"
#include "parsehelper.h"
#include "cpu/cpu.h"

#ifdef HAVE_DEBUGGER
#include "cpu/cpu_generic/ppc_cpu.h"
#endif

enum EvalType {
	ET_INTEGER,
	ET_FLOAT,
	ET_STRING,
	ET_VARARGS
};

/**
 *	An abstract function
 */
class Function: public Object {
public:
	/* new */
	virtual	EvalType	getReturnType() const = 0;
	virtual	SInt64 *	evalInteger() const;
	virtual	Float *		evalFloat() const;
	virtual	String *	evalString() const;
	virtual uint		getArgCount() const;
	virtual Function *	getArg(uint i) const;
	virtual void		setArg(uint i, Function *f);
};

/**
 *	A constant (signed) integer function
 */
class SInt64Function: public Function {
protected:
	sint64 value;
public:
	SInt64Function(sint64 v);
	/* extends Function */
	virtual	EvalType	getReturnType() const;
	virtual	SInt64 *	evalInteger() const;
};

/**
 *	A GPR
 */
class GPRFunction: public Function {
protected:
	int mNumber;
public:
	GPRFunction(int number);
	/* extends Function */
	virtual	EvalType	getReturnType() const;
	virtual	SInt64 *	evalInteger() const;
};

class FPRFunction: public Function {
protected:
	int mNumber;
public:
	FPRFunction(int number);
	/* extends Function */
	virtual	EvalType	getReturnType() const;
	virtual	Float *		evalFloat() const;
};

class UInt32PFunction: public Function {
protected:
	uint32 *mValue;
public:
	UInt32PFunction(uint32 *aValue);
	/* extends Function */
	virtual	EvalType	getReturnType() const;
	virtual	SInt64 *	evalInteger() const;
};
/**
 *	A constant float function
 */
class FloatFunction: public Function {
protected:
	double value;
public:
	FloatFunction(double v);
/* extends Function */
	virtual	EvalType	getReturnType() const;
	virtual	Float *		evalFloat() const;
};

/**
 *	A constant string function
 */
class StringFunction: public Function {
protected:
	String value;
public:
	StringFunction(const byte *str, int len);
/* extends Function */
	virtual	EvalType	getReturnType() const;
	virtual	String *	evalString() const;
};

/**
 *	A parametrized function (implementation helper)
 */
class PFunction: public Function {
protected:
	Array mArgs;
public:
	PFunction();
/* extends Function */
	virtual uint		getArgCount() const;
	virtual Function *	getArg(uint i) const;
	virtual void		setArg(uint i, Function *f);
};

/*
 *
 */
class Watch: public Object {
	String *mName;
	Function *f;
public:
				Watch(String *aName, Function *f);
	virtual			~Watch();
	virtual	int		toString(char *buf, int buflen) const;

};

/*
 *	A debugger
 */
class Debugger {
protected:
	Container *mWatches;
#ifdef HAVE_DEBUGGER
	PPC_CPU_State savedCPUState;
#endif
	bool mForceSinglestep;

		Function *eval_scalarToFunction(eval_scalar &s);
	virtual	Function *matchFunction(const String &name, const Enumerator &params);
public:
	bool	mAlwaysShowRegs;
	bool	mUseColors;

		Debugger();
	virtual ~Debugger();
	bool	parse(const String &str);
	void	enter();
	void	dump();
};

void debugger_init_config();

#endif /* __EVAL_H__ */
